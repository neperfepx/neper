/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

// orilib, a command-line program to run the Orilib functions.
//
// The functions are described in table ol_coms, by their arguments, in the
// order of the C function: <name>:<type> for an input, ><name>:<type> for an
// output and <<name>:<type> for the returned value (when it is the result of
// the function).  An output list (several values of a type) is written
// <type>*<n> (at most n values, the number of which is returned by the
// following argument, int *), <type>*=<n> (n values, n being an integer or the
// name of an integer input) or <type>* (allocated by the function, with the
// number of values returned by the following argument, int *).  A function is
// called through an "adapter", which casts it to its C prototype (see
// orilib_call).  The functions that do not fit (lists of structures, tables)
// have specific code (ol_custom).

#include "orilib_.h"
#include <setjmp.h>

void orilib_printf (int, const char *, ...);

static int orilib_interactive = 0;
static int orilib_argv = 1;
static jmp_buf orilib_nextcom;

// formats of the real output values, which can be set using environment
// variable ORILIB_FORMAT (e.g. "%.17g", for full precision)
static char orilib_fmt[100] = "%12.9f";

typedef void (*ol_fct) (void);

// the returned value is the result of the function / a status (0 for success)
#define OL_RET 1
#define OL_STATUS 2

struct OL_COM
{
  const char *name;
  ol_fct fct;
  char ret;                     // C return type: v (void), i (int) or d (double)
  const char *args;
  int retkind;                  // 0, OL_RET or OL_STATUS
  const char *msg;              // message on failure (OL_STATUS), with %s for
                                // the first string input
};

// types: number of values, and kind (r: reals, m: 3x3 matrix, i: integers,
// s: string)
struct OL_TYPE
{
  const char *name;
  int qty;
  char kind;
};

static const struct OL_TYPE ol_types[] = {
  {"e", 3, 'r'}, {"q", 4, 'r'}, {"R", 3, 'r'}, {"rtheta", 4, 'r'}, {"r", 3, 'r'},
  {"vect", 3, 'r'}, {"p", 2, 'r'}, {"theta", 1, 'r'}, {"real", 1, 'r'},
  {"real3", 3, 'r'}, {"real9", 9, 'r'}, {"g", 9, 'm'}, {"real3x3", 9, 'm'},
  {"m", 6, 'i'}, {"pole", 3, 'i'}, {"hpole", 4, 'i'}, {"int", 1, 'i'}, {"int3", 3, 'i'},
  {"str", 1, 's'}, {NULL, 0, 0}
};

// an argument
struct OL_ARG
{
  char name[100];
  const struct OL_TYPE *type;
  int dir;                      // 0: input, 1: output, 2: returned value
  int islist;                   // list: 1: at most max values, with count,
                                // 2: max values, 3: allocated, with count
  int max;
  char maxname[100];            // (max from an integer input)

  double d[9];                  // values
  double *rows[3];
  int i[9];
  char s[1000];
  int qty;                      // list: number of values
  double **dl;                  // list values
  double ***ml;
  int **il;
  void *ptr;                    // pointer passed to the function
};

// invalid input data: if the end of the input or `quit', quits; otherwise,
// prints a warning and goes to the next command (interactive mode) or
// prints an error (non-interactive mode)
void
ut_oneorexit (int val)
{
  int c;
  char tok[1000];

  if (val == 1)
    return;

  if (feof (stdin) || scanf ("%999s", tok) != 1)
    exit (0);

  if (!strcmp (tok, "quit") || !strcmp (tok, "q"))
    exit (0);

  // non-interactive mode: error (aborts)
  ut_print_message (orilib_interactive ? 1 : 2, 0, "Invalid input data `%s'.\n", tok);

  while ((c = getchar ()) != '\n' && c != EOF);

  longjmp (orilib_nextcom, 1);
}

// error: warning and next command (interactive mode) or error (aborts)
static void
orilib_error (const char *format, const char *arg)
{
  char msg[1000];

  snprintf (msg, 1000, format, arg ? arg : "");
  ut_print_message (orilib_interactive ? 1 : 2, 0, "%s\n", msg);

  longjmp (orilib_nextcom, 1);
}

static const struct OL_TYPE *
orilib_type (const char *name)
{
  int i;

  for (i = 0; ol_types[i].name; i++)
    if (!strcmp (ol_types[i].name, name))
      return ol_types + i;

  ut_print_message (2, 0, "Unknown type `%s' (orilib bug).\n", name);

  return NULL;
}

// parsing the arguments of a command
static int
orilib_args (const char *spec, struct OL_ARG *args)
{
  int qty = 0;
  char *copy = NULL, *tok = NULL, *star = NULL, *colon = NULL;

  ut_string_string (spec, &copy);

  for (tok = strtok (copy, " "); tok; tok = strtok (NULL, " "))
  {
    struct OL_ARG *arg = args + qty++;

    memset (arg, 0, sizeof (struct OL_ARG));
    arg->dir = (tok[0] == '>') ? 1 : (tok[0] == '<') ? 2 : 0;
    if (arg->dir)
      tok++;

    colon = strchr (tok, ':');
    *colon = '\0';
    strcpy (arg->name, tok);

    star = strchr (colon + 1, '*');
    if (star)
    {
      *star = '\0';
      if (star[1] == '\0')
        arg->islist = 3;
      else if (star[1] == '=')
      {
        arg->islist = 2;
        if (sscanf (star + 2, "%d", &arg->max) != 1)
          strcpy (arg->maxname, star + 2);
      }
      else
      {
        arg->islist = 1;
        sscanf (star + 1, "%d", &arg->max);
      }
    }

    arg->type = orilib_type (colon + 1);
  }

  ut_free_1d_char (&copy);

  return qty;
}

static void
orilib_read (struct OL_ARG *arg)
{
  int j;

  orilib_printf (orilib_argv, "%s = \n", arg->name);

  if (arg->type->kind == 'r' || arg->type->kind == 'm')
    for (j = 0; j < arg->type->qty; j++)
      ut_oneorexit (fscanf (stdin, "%lf", arg->d + j) == 1);
  else if (arg->type->kind == 'i')
    for (j = 0; j < arg->type->qty; j++)
      ut_oneorexit (fscanf (stdin, "%d", arg->i + j) == 1);
  else
    ut_oneorexit (fscanf (stdin, "%999s", arg->s) == 1);
}

// printing values of a type
static void
orilib_print_values (const struct OL_TYPE *type, double *d, int *i, char *s)
{
  int j;

  if (type->kind == 'r' || type->kind == 'm')
    for (j = 0; j < type->qty; j++)
    {
      printf (orilib_fmt, d[j]);
      printf ((type->kind == 'm' ? j % 3 == 2 : j == type->qty - 1) ? "\n" : " ");
    }
  else if (type->kind == 'i')
    for (j = 0; j < type->qty; j++)
      printf (type->qty == 1 ? "%d%s" : "%3d%s", i[j], j == type->qty - 1 ? "\n" : " ");
  else
    printf ("%s\n", s);
}

static void
orilib_print (struct OL_ARG *arg)
{
  int k, j;

  orilib_printf (orilib_argv, "%s = \n", arg->name);

  if (!arg->islist)
    orilib_print_values (arg->type, arg->d, arg->i, arg->s);

  else
    for (k = 0; k < arg->qty; k++)
    {
      if (arg->type->kind == 'm')
      {
        for (j = 0; j < 9; j++)
          arg->d[j] = arg->ml[k][j / 3][j % 3];
        orilib_print_values (arg->type, arg->d, NULL, NULL);
      }
      else
        orilib_print_values (arg->type, arg->dl ? arg->dl[k] : NULL,
                             arg->il ? arg->il[k] : NULL, NULL);
    }
}

// list outputs: memory (pre-allocated lists, or allocated by the function)
static void
orilib_list_alloc (struct OL_ARG *arg, struct OL_ARG *args, int argqty)
{
  int j;

  if (arg->maxname[0])
    for (j = 0; j < argqty; j++)
      if (!strcmp (args[j].name, arg->maxname))
        arg->max = args[j].i[0];

  if (arg->max < 0)
    orilib_error ("Invalid number of values.", NULL);

  arg->qty = (arg->islist == 2) ? arg->max : 0;

  if (arg->islist == 3)
    arg->ptr = (arg->type->kind == 'i') ? (void *) &arg->il : (void *) &arg->dl;
  else if (arg->type->kind == 'm')
    arg->ptr = arg->ml = ut_alloc_3d (arg->max, 3, 3);
  else if (arg->type->kind == 'i')
    arg->ptr = arg->il = ut_alloc_2d_int (arg->max, arg->type->qty);
  else
    arg->ptr = arg->dl = ut_alloc_2d (arg->max, arg->type->qty);
}

static void
orilib_list_free (struct OL_ARG *arg)
{
  int qty = (arg->islist == 3) ? arg->qty : arg->max;

  if (arg->ml)
    ut_free_3d (&arg->ml, qty, 3);
  if (arg->il)
    ut_free_2d_int (&arg->il, qty);
  if (arg->dl)
    ut_free_2d (&arg->dl, qty);
}

// C parameters: kind (as in the adapters) and pointer
struct OL_PARAM
{
  const char *kind;
  struct OL_ARG *arg;
  void *ptr;
};

static int
orilib_params (struct OL_ARG *args, int argqty, struct OL_PARAM *params, char *shape)
{
  int j, qty = 0;
  struct OL_ARG *arg = NULL;

  for (j = 0; j < argqty; j++)
  {
    const char *kind = NULL;

    arg = args + j;

    if (arg->dir == 2)
      continue;

    if (arg->islist)
    {
      if (arg->type->kind == 'm')
        kind = "PPP";
      else if (arg->type->kind == 'i')
        kind = (arg->islist == 3) ? "IPPP" : "IPP";
      else
        kind = (arg->islist == 3) ? "PPP" : "PP";
      params[qty].ptr = arg->ptr;
    }
    else if (arg->type->kind == 'm')
    {
      kind = "PP";
      arg->rows[0] = arg->d;
      arg->rows[1] = arg->d + 3;
      arg->rows[2] = arg->d + 6;
      params[qty].ptr = arg->rows;
    }
    else if (arg->type->kind == 's')
    {
      kind = "C";
      params[qty].ptr = arg->s;
    }
    else if (arg->type->kind == 'i')
    {
      kind = (arg->dir || arg->type->qty > 1) ? "IP" : "I";
      params[qty].ptr = arg->i;
    }
    else
    {
      kind = (arg->dir || arg->type->qty > 1) ? "P" : "D";
      params[qty].ptr = arg->d;
    }

    params[qty].kind = kind;
    params[qty++].arg = arg;

    // count of a list
    if (arg->islist == 1 || arg->islist == 3)
    {
      params[qty].kind = "IP";
      params[qty].arg = arg;
      params[qty++].ptr = &arg->qty;
    }
  }

  for (j = 0; j < qty; j++)
    sprintf (shape + strlen (shape), "%s%s", j ? "," : "", params[j].kind);

  return qty;
}

#define A(k) (params[k].ptr)
#define D(k) (params[k].arg->d[0])
#define I(k) (params[k].arg->i[0])

// calling a function (through its adapter); returns the returned value
static double
orilib_call (const struct OL_COM *com, struct OL_PARAM *params, char *shape)
{
  int reti = 0;
  double retd = 0;
  ol_fct fct = com->fct;

  if (0)
  {
  }
  else if (!strcmp (shape, "d:IP,C,IP"))
    retd = ((double (*) (int *, char *, int *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "d:P,C,P"))
    retd = ((double (*) (double *, char *, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "d:PP,C,PP"))
    retd = ((double (*) (double **, char *, double **)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "i:C"))
    reti = ((int (*) (char *)) fct) (A (0));
  else if (!strcmp (shape, "i:C,P"))
    reti = ((int (*) (char *, double *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "i:C,PP"))
    reti = ((int (*) (char *, double **)) fct) (A (0), A (1));
  else if (!strcmp (shape, "i:P"))
    reti = ((int (*) (double *)) fct) (A (0));
  else if (!strcmp (shape, "i:P,C"))
    reti = ((int (*) (double *, char *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "i:P,C,P"))
    reti = ((int (*) (double *, char *, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "i:P,P"))
    reti = ((int (*) (double *, double *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "i:P,P,C,I,P,C,IP"))
    reti = ((int (*) (double *, double *, char *, int, double *, char *, int *)) fct)
      (A (0), A (1), A (2), I (3), A (4), A (5), A (6));
  else if (!strcmp (shape, "i:P,P,C,P"))
    reti = ((int (*) (double *, double *, char *, double *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "i:P,P,P"))
    reti = ((int (*) (double *, double *, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "i:PP,P"))
    reti = ((int (*) (double **, double *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "i:PP,PP,C,P"))
    reti = ((int (*) (double **, double **, char *, double *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "i:PP,PP,C,PP"))
    reti = ((int (*) (double **, double **, char *, double **)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "i:PP,PP,P"))
    reti = ((int (*) (double **, double **, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "i:PP,PP,PP"))
    reti = ((int (*) (double **, double **, double **)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:D,D,D,D,P"))
    ((void (*) (double, double, double, double, double *)) fct) (D (0), D (1), D (2), D (3), A (4));
  else if (!strcmp (shape, "v:D,D,D,P"))
    ((void (*) (double, double, double, double *)) fct) (D (0), D (1), D (2), A (3));
  else if (!strcmp (shape, "v:D,D,P"))
    ((void (*) (double, double, double *)) fct) (D (0), D (1), A (2));
  else if (!strcmp (shape, "v:D,P"))
    ((void (*) (double, double *)) fct) (D (0), A (1));
  else if (!strcmp (shape, "v:I,C"))
    ((void (*) (int, char *)) fct) (I (0), A (1));
  else if (!strcmp (shape, "v:I,I,PP"))
    ((void (*) (int, int, double **)) fct) (I (0), I (1), A (2));
  else if (!strcmp (shape, "v:I,P"))
    ((void (*) (int, double *)) fct) (I (0), A (1));
  else if (!strcmp (shape, "v:IP,C,I,IP"))
    ((void (*) (int *, char *, int, int *)) fct) (A (0), A (1), I (2), A (3));
  else if (!strcmp (shape, "v:IP,C,IP"))
    ((void (*) (int *, char *, int *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:IP,C,IPPP,IP"))
    ((void (*) (int *, char *, int ***, int *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "v:IP,I,IP"))
    ((void (*) (int *, int, int *)) fct) (A (0), I (1), A (2));
  else if (!strcmp (shape, "v:IP,IPP,IP"))
    ((void (*) (int *, int **, int *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:IP,P"))
    ((void (*) (int *, double *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:IP,PP"))
    ((void (*) (int *, double **)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:P,C,I,P"))
    ((void (*) (double *, char *, int, double *)) fct) (A (0), A (1), I (2), A (3));
  else if (!strcmp (shape, "v:P,C,PP,IP"))
    ((void (*) (double *, char *, double **, int *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "v:P,C,PPP,IP"))
    ((void (*) (double *, char *, double ***, int *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "v:P,I,P"))
    ((void (*) (double *, int, double *)) fct) (A (0), I (1), A (2));
  else if (!strcmp (shape, "v:P,IP"))
    ((void (*) (double *, int *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:P,P"))
    ((void (*) (double *, double *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:P,P,C,P"))
    ((void (*) (double *, double *, char *, double *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "v:P,P,P"))
    ((void (*) (double *, double *, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:P,P,PP"))
    ((void (*) (double *, double *, double **)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:P,PP"))
    ((void (*) (double *, double **)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:P,PP,IP"))
    ((void (*) (double *, double **, int *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:P,PP,P"))
    ((void (*) (double *, double **, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:PP,C,I,PP"))
    ((void (*) (double **, char *, int, double **)) fct) (A (0), A (1), I (2), A (3));
  else if (!strcmp (shape, "v:PP,I,PP"))
    ((void (*) (double **, int, double **)) fct) (A (0), I (1), A (2));
  else if (!strcmp (shape, "v:PP,P"))
    ((void (*) (double **, double *)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:PP,P,C,P"))
    ((void (*) (double **, double *, char *, double *)) fct) (A (0), A (1), A (2), A (3));
  else if (!strcmp (shape, "v:PP,P,P"))
    ((void (*) (double **, double *, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:PP,PP"))
    ((void (*) (double **, double **)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:PP,PP,P"))
    ((void (*) (double **, double **, double *)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:PP,PP,PP"))
    ((void (*) (double **, double **, double **)) fct) (A (0), A (1), A (2));
  else if (!strcmp (shape, "v:PP,PPP"))
    ((void (*) (double **, double ***)) fct) (A (0), A (1));
  else if (!strcmp (shape, "v:PP,PPP,IP"))
    ((void (*) (double **, double ***, int *)) fct) (A (0), A (1), A (2));
  else
    ut_print_message (2, 0, "No adapter for `%s' (%s, orilib bug).\n", com->name, shape);

  return com->ret == 'd' ? retd : reti;
}

#undef A
#undef D
#undef I

// running a command of the table
static void
orilib_run (const struct OL_COM *com)
{
  int j, argqty, paramqty;
  double ret;
  char shape[1000];
  const char *str = NULL;
  struct OL_ARG args[20];
  struct OL_PARAM params[30];

  argqty = orilib_args (com->args, args);

  for (j = 0; j < argqty; j++)
    if (args[j].dir == 0)
    {
      orilib_read (args + j);
      if (args[j].type->kind == 's' && !str)
        str = args[j].s;
    }

  for (j = 0; j < argqty; j++)
    if (args[j].islist)
      orilib_list_alloc (args + j, args, argqty);

  sprintf (shape, "%c:", com->ret);
  paramqty = orilib_params (args, argqty, params, shape);
  (void) paramqty;

  ret = orilib_call (com, params, shape);

  if (com->retkind == OL_STATUS && ret != 0)
  {
    for (j = 0; j < argqty; j++)
      if (args[j].islist)
        orilib_list_free (args + j);
    orilib_error (com->msg, str);
  }

  for (j = 0; j < argqty; j++)
    if (args[j].dir == 2)
    {
      args[j].d[0] = ret;
      args[j].i[0] = (int) ret;
    }

  for (j = 0; j < argqty; j++)
    if (args[j].dir)
      orilib_print (args + j);

  for (j = 0; j < argqty; j++)
    if (args[j].islist)
      orilib_list_free (args + j);
}

// commands with specific code ---------------------------------------------

static void
orilib_read_g (const char *name, double **g)
{
  orilib_printf (orilib_argv, "%s = \n", name);
  ut_oneorexit (ol_g_fscanf (stdin, g));
}

static void
orilib_read_string (const char *name, char *s)
{
  orilib_printf (orilib_argv, "%s = \n", name);
  ut_oneorexit (fscanf (stdin, "%999s", s) == 1);
}

// ol_g_m and ol_q_m: Miller indices close to an orientation, written as a
// table (m, disorientation angle, Sigma and mq)
static void
orilib_gq_m (int isq)
{
  int i, j, maxindex, maxresqty, resqty;
  double alphaq, theta;
  double *q = ol_q_alloc ();
  double **g = ol_g_alloc ();
  double **gtmp = ol_g_alloc ();
  double *mq = NULL;
  int **allm = NULL;
  char crysym[1000];

  if (isq)
  {
    orilib_printf (orilib_argv, "q = \n");
    ut_oneorexit (ol_q_fscanf (stdin, q));
  }
  else
    orilib_read_g ("g", g);
  orilib_read_string ("crysym", crysym);
  orilib_printf (orilib_argv, "maxindex = \n");
  ut_oneorexit (fscanf (stdin, "%d", &maxindex) == 1);
  orilib_printf (orilib_argv, "maxresqty = \n");
  ut_oneorexit (fscanf (stdin, "%d", &maxresqty) == 1);
  orilib_printf (orilib_argv, "alphaq = \n");
  ut_oneorexit (fscanf (stdin, "%lf", &alphaq) == 1);

  if (maxresqty < 1)
    orilib_error ("Invalid number of values.", NULL);

  allm = ut_alloc_2d_int (maxresqty, 6);
  mq = ut_alloc_1d (maxresqty);

  if (isq)
  {
    ol_q_m (q, crysym, maxindex, maxresqty, alphaq, allm, mq, &resqty);
    ol_q_g (q, g);
  }
  else
    ol_g_m (g, crysym, maxindex, maxresqty, alphaq, allm, mq, &resqty);

  orilib_printf (orilib_argv, "m theta Sigma mq = \n");
  for (i = 0; i < resqty; i++)
  {
    double vals[3];

    ol_m_g (allm[i], gtmp);
    ol_g_g_disori (g, gtmp, crysym, &theta);

    vals[0] = theta;
    vals[1] = (alphaq < 0.9999) ? 2 * mq[i] - theta
                                : (double) ut_array_1d_int_abssum (allm[i], 6);
    vals[2] = mq[i];

    for (j = 0; j < 6; j++)
      printf ("%3d ", allm[i][j]);
    for (j = 0; j < 3; j++)
    {
      printf (orilib_fmt, vals[j]);
      printf (j < 2 ? " " : "\n");
    }
  }

  ut_free_2d_int (&allm, maxresqty);
  ut_free_1d (&mq);
  ol_q_free (&q);
  ol_g_free (&g);
  ol_g_free (&gtmp);
}

static void
orilib_g_m (void)
{
  orilib_gq_m (0);
}

static void
orilib_q_m (void)
{
  orilib_gq_m (1);
}

// ol_set_filter and ol_set_filter_iter: filtering of a set of orientations
// (quaternions, cubic symmetry)
static void
orilib_set_filter_gen (int iter)
{
  int i;
  size_t size;
  double factor;
  struct OL_SET Set, Set2;

  orilib_printf (orilib_argv, "Set.size = \n");
  ut_oneorexit (fscanf (stdin, "%zu", &size) == 1);
  Set = ol_set_alloc (size, "cubic");
  Set2 = ol_set_alloc (size, "cubic");
  orilib_printf (orilib_argv, "Set.q = \n");
  for (i = 0; i < (int) Set.size; i++)
    ut_oneorexit (ol_q_fscanf (stdin, Set.q[i]));
  orilib_printf (orilib_argv, "factor = \n");
  ut_oneorexit (fscanf (stdin, "%lf", &factor) == 1);

  if (iter)
    ol_set_filter_iter (Set, factor, &Set2, NULL, NULL);
  else
    ol_set_filter (Set, factor, &Set2, NULL, NULL);

  orilib_printf (orilib_argv, "Set2.size = \n");
  printf ("%zu\n", Set2.size);
  orilib_printf (orilib_argv, "Set2.q = \n");
  for (i = 0; i < (int) Set2.size; i++)
    orilib_print_values (orilib_type ("q"), Set2.q[i], NULL, NULL);

  ol_set_free (&Set);
  ol_set_free (&Set2);
}

static void
orilib_set_filter (void)
{
  orilib_set_filter_gen (0);
}

static void
orilib_set_filter_iter (void)
{
  orilib_set_filter_gen (1);
}

// ol_q_des: descriptor values (their number depends on the descriptor)
static void
orilib_q_des (void)
{
  int size;
  double q[4], vec[9];
  char des[1000];

  orilib_printf (orilib_argv, "q = \n");
  ut_oneorexit (ol_q_fscanf (stdin, q));
  orilib_read_string ("des", des);

  size = ol_des_size (des);
  if (size < 1 || ol_q_des (q, des, vec) != 0)
    orilib_error ("Unsupported descriptor `%s'.", des);

  orilib_printf (orilib_argv, "values = \n");
  for (int k = 0; k < size; k++)
  {
    printf (orilib_fmt, vec[k]);
    printf (k < size - 1 ? " " : "\n");
  }
}

// ol_g_oneline: rotation matrix on one line
static void
orilib_g_oneline (void)
{
  int i;
  double **g = ol_g_alloc ();

  orilib_read_g ("g", g);
  orilib_printf (orilib_argv, "g = \n");
  for (i = 0; i < 9; i++)
  {
    printf (orilib_fmt, g[i / 3][i % 3]);
    printf (i < 8 ? " " : "\n");
  }

  ol_g_free (&g);
}

struct OL_CUSTOM
{
  const char *name;
  void (*fct) (void);
  const char *args;             // (for `signatures'; NULL if they are not
                                // those of a single item)
};

static const struct OL_CUSTOM ol_customs[] = {
  {"ol_g_m", orilib_g_m, NULL},
  {"ol_g_oneline", orilib_g_oneline, "g:g >g:real9"},
  {"ol_q_des", orilib_q_des, "q:q des:str >values:real*"},
  {"ol_q_m", orilib_q_m, NULL},
  {"ol_set_filter", orilib_set_filter, NULL},
  {"ol_set_filter_iter", orilib_set_filter_iter, NULL},
  {NULL, NULL, NULL}
};

// the commands ----------------------------------------------------------

static const struct OL_COM ol_coms[] = {
  {"ol_R_Rcrysym", (ol_fct) ol_R_Rcrysym, 'd', "R:R crysym:str >R2:R", 0, NULL},
  {"ol_R_g", (ol_fct) ol_R_g, 'v', "R:R >g:g", 0, NULL},
  {"ol_R_q", (ol_fct) ol_R_q, 'v', "R:R >q:q", 0, NULL},
  {"ol_R_rtheta", (ol_fct) ol_R_rtheta, 'v', "R:R >rtheta:rtheta", 0, NULL},
  {"ol_R_rtheta_rad", (ol_fct) ol_R_rtheta_rad, 'v', "R:R >rtheta:rtheta", 0, NULL},
  {"ol_crysym_qty", (ol_fct) ol_crysym_qty, 'i', "crysym:str <qty:int", OL_RET, NULL},
  {"ol_des_size", (ol_fct) ol_des_size, 'i', "des:str <size:int", OL_RET, NULL},
  {"ol_e_deg2rad", (ol_fct) ol_e_deg2rad, 'v', "e:e >e:e", 0, NULL},
  {"ol_e_e", (ol_fct) ol_e_e, 'v', "e:e >e:e", 0, NULL},
  {"ol_e_e_rad", (ol_fct) ol_e_e_rad, 'v', "e:e >e:e", 0, NULL},
  {"ol_e_ecrysym", (ol_fct) ol_e_ecrysym, 'd', "e:e crysym:str >e2:e", 0, NULL},
  {"ol_e_ecrysym_orthotropic", (ol_fct) ol_e_ecrysym_orthotropic, 'v',
   "e:e crysym:str >e2:e*8", 0, NULL},
  {"ol_e_ecrysym_orthotropic_rad", (ol_fct) ol_e_ecrysym_orthotropic_rad, 'v',
   "e:e crysym:str >e2:e*8", 0, NULL},
  {"ol_e_ecrysym_rad", (ol_fct) ol_e_ecrysym_rad, 'd', "e:e crysym:str >e2:e", 0, NULL},
  {"ol_e_ek", (ol_fct) ol_e_ek, 'v', "e:e >ek:e", 0, NULL},
  {"ol_e_ek_rad", (ol_fct) ol_e_ek_rad, 'v', "e:e >ek:e", 0, NULL},
  {"ol_e_er", (ol_fct) ol_e_er, 'v', "e:e >er:e", 0, NULL},
  {"ol_e_er_rad", (ol_fct) ol_e_er_rad, 'v', "e:e >er:e", 0, NULL},
  {"ol_e_g", (ol_fct) ol_e_g, 'v', "e:e >g:g", 0, NULL},
  {"ol_e_g_rad", (ol_fct) ol_e_g_rad, 'v', "e:e >g:g", 0, NULL},
  {"ol_e_q", (ol_fct) ol_e_q, 'v', "e:e >q:q", 0, NULL},
  {"ol_e_q_rad", (ol_fct) ol_e_q_rad, 'v', "e:e >q:q", 0, NULL},
  {"ol_e_rad2deg", (ol_fct) ol_e_rad2deg, 'v', "e:e >e:e", 0, NULL},
  {"ol_eaproj_vect", (ol_fct) ol_eaproj_vect, 'v', "p:p >vect:vect", 0, NULL},
  {"ol_eaproj_vect_rad", (ol_fct) ol_eaproj_vect_rad, 'v', "p:p >vect:vect", 0, NULL},
  {"ol_eaprojxy_vect", (ol_fct) ol_eaprojxy_vect, 'v', "p:p >vect:vect", 0, NULL},
  {"ol_ek_e", (ol_fct) ol_ek_e, 'v', "e:e >e:e", 0, NULL},
  {"ol_ek_e_rad", (ol_fct) ol_ek_e_rad, 'v', "e:e >e:e", 0, NULL},
  {"ol_er_e", (ol_fct) ol_er_e, 'v', "er:e >e:e", 0, NULL},
  {"ol_er_e_rad", (ol_fct) ol_er_e_rad, 'v', "er:e >e:e", 0, NULL},
  {"ol_g_R", (ol_fct) ol_g_R, 'v', "g:g >R:R", 0, NULL},
  {"ol_g_crysym", (ol_fct) ol_g_crysym, 'v', "g:g crysym:str i:int >g2:g", 0, NULL},
  {"ol_g_csys", (ol_fct) ol_g_csys, 'v', "g:g gs:g >g':g", 0, NULL},
  {"ol_g_e", (ol_fct) ol_g_e, 'v', "g:g >e:e", 0, NULL},
  {"ol_g_e_rad", (ol_fct) ol_g_e_rad, 'v', "g:g >e:e", 0, NULL},
  {"ol_g_g_disori", (ol_fct) ol_g_g_disori, 'i', "g1:g g2:g crysym:str >theta:theta", 0, NULL},
  {"ol_g_g_disori_orthotropic", (ol_fct) ol_g_g_disori_orthotropic, 'i',
   "g1:g g2:g crysym:str >theta:theta", 0, NULL},
  {"ol_g_g_disori_orthotropic_rad", (ol_fct) ol_g_g_disori_orthotropic_rad, 'i',
   "g1:g g2:g crysym:str >theta:theta", 0, NULL},
  {"ol_g_g_disori_rad", (ol_fct) ol_g_g_disori_rad, 'i',
   "g1:g g2:g crysym:str >theta:theta", 0, NULL},
  {"ol_g_g_g", (ol_fct) ol_g_g_g, 'v', "g1:g g2:g >g3:g", 0, NULL},
  {"ol_g_g_g_cur", (ol_fct) ol_g_g_g_cur, 'v', "g1:g g2:g >g3:g", 0, NULL},
  {"ol_g_g_g_ref", (ol_fct) ol_g_g_g_ref, 'v', "g1:g g2:g >gres:g", 0, NULL},
  {"ol_g_g_gdisori", (ol_fct) ol_g_g_gdisori, 'i', "g1:g g2:g crysym:str >gd:g", 0, NULL},
  {"ol_g_g_gdisori_cur", (ol_fct) ol_g_g_gdisori_cur, 'i', "g1:g g2:g crysym:str >gd:g", 0, NULL},
  {"ol_g_g_gdisori_orthotropic", (ol_fct) ol_g_g_gdisori_orthotropic, 'i',
   "g1:g g2:g crysym:str >gd:g", 0, NULL},
  {"ol_g_g_gdisori_ref", (ol_fct) ol_g_g_gdisori_ref, 'i', "g1:g g2:g crysym:str >gd:g", 0, NULL},
  {"ol_g_g_gmisori", (ol_fct) ol_g_g_gmisori, 'v', "g1:g g2:g >gm:g", 0, NULL},
  {"ol_g_g_gmisori_cur", (ol_fct) ol_g_g_gmisori_cur, 'v', "g1:g g2:g >gm:g", 0, NULL},
  {"ol_g_g_gmisori_orthotropic", (ol_fct) ol_g_g_gmisori_orthotropic, 'i',
   "g1:g g2:g >gm:g", 0, NULL},
  {"ol_g_g_gmisori_ref", (ol_fct) ol_g_g_gmisori_ref, 'v', "g1:g g2:g >gm:g", 0, NULL},
  {"ol_g_g_misori", (ol_fct) ol_g_g_misori, 'v', "g1:g g2:g >theta:theta", 0, NULL},
  {"ol_g_g_misori_orthotropic", (ol_fct) ol_g_g_misori_orthotropic, 'i',
   "g1:g g2:g >theta:theta", 0, NULL},
  {"ol_g_g_misori_orthotropic_rad", (ol_fct) ol_g_g_misori_orthotropic_rad, 'i',
   "g1:g g2:g >theta:theta", 0, NULL},
  {"ol_g_g_misori_rad", (ol_fct) ol_g_g_misori_rad, 'v', "g1:g g2:g >theta:theta", 0, NULL},
  {"ol_g_gcrysym", (ol_fct) ol_g_gcrysym, 'd', "g:g crysym:str >g2:g", 0, NULL},
  {"ol_g_homochoric", (ol_fct) ol_g_homochoric, 'v', "g:g >h:real3", 0, NULL},
  {"ol_g_inverse", (ol_fct) ol_g_inverse, 'v', "g:g >ginv:g", 0, NULL},
  {"ol_g_ipf_crysym_eaproj", (ol_fct) ol_g_ipf_crysym_eaproj, 'v',
   "g:g vect:vect crysym:str >p:p", 0, NULL},
  {"ol_g_ipf_crysym_eaproj_rad", (ol_fct) ol_g_ipf_crysym_eaproj_rad, 'v',
   "g:g vect:vect crysym:str >p:p", 0, NULL},
  {"ol_g_ipf_crysym_eaprojxy", (ol_fct) ol_g_ipf_crysym_eaprojxy, 'v',
   "g:g vect:vect crysym:str >p:p", 0, NULL},
  {"ol_g_ipf_crysym_stproj", (ol_fct) ol_g_ipf_crysym_stproj, 'v',
   "g:g vect:vect crysym:str >p:p", 0, NULL},
  {"ol_g_ipf_crysym_stproj_rad", (ol_fct) ol_g_ipf_crysym_stproj_rad, 'v',
   "g:g vect:vect crysym:str >p:p", 0, NULL},
  {"ol_g_ipf_crysym_stprojxy", (ol_fct) ol_g_ipf_crysym_stprojxy, 'v',
   "g:g vect:vect crysym:str >p:p", 0, NULL},
  {"ol_g_ipf_eaproj", (ol_fct) ol_g_ipf_eaproj, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_ipf_eaproj_rad", (ol_fct) ol_g_ipf_eaproj_rad, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_ipf_eaprojxy", (ol_fct) ol_g_ipf_eaprojxy, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_ipf_stproj", (ol_fct) ol_g_ipf_stproj, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_ipf_stproj_rad", (ol_fct) ol_g_ipf_stproj_rad, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_ipf_stprojxy", (ol_fct) ol_g_ipf_stprojxy, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_lng", (ol_fct) ol_g_lng, 'v', "g:g >lng:real3x3", 0, NULL},
  {"ol_g_pf_eaproj", (ol_fct) ol_g_pf_eaproj, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_pf_eaproj_rad", (ol_fct) ol_g_pf_eaproj_rad, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_pf_eaprojxy", (ol_fct) ol_g_pf_eaprojxy, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_pf_stproj", (ol_fct) ol_g_pf_stproj, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_pf_stproj_rad", (ol_fct) ol_g_pf_stproj_rad, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_pf_stprojxy", (ol_fct) ol_g_pf_stprojxy, 'v', "g:g vect:vect >p:p", 0, NULL},
  {"ol_g_q", (ol_fct) ol_g_q, 'v', "g:g >q:q", 0, NULL},
  {"ol_g_r", (ol_fct) ol_g_r, 'i', "g:g >r:r", OL_STATUS, "Not a rotation matrix."},
  {"ol_g_refcsys", (ol_fct) ol_g_refcsys, 'v', "g:g gs:g >gp:g", 0, NULL},
  {"ol_g_refsym", (ol_fct) ol_g_refsym, 'v', "g:g i:int >g2:g", 0, NULL},
  {"ol_g_refsym_g", (ol_fct) ol_g_refsym_g, 'v', "g:g i:int >gs:g", 0, NULL},
  {"ol_g_refsym_monoclinic", (ol_fct) ol_g_refsym_monoclinic, 'v', "g:g >g2:g*4", 0, NULL},
  {"ol_g_refsym_orthotropic", (ol_fct) ol_g_refsym_orthotropic, 'v', "g:g >g2:g*4", 0, NULL},
  {"ol_g_refsym_orthotropic_all", (ol_fct) ol_g_refsym_orthotropic_all, 'v',
   "g:g >g2:g*=4", 0, NULL},
  {"ol_g_rtheta", (ol_fct) ol_g_rtheta, 'i',
   "g:g >rtheta:rtheta", OL_STATUS, "Not a rotation matrix."},
  {"ol_g_rtheta_rad", (ol_fct) ol_g_rtheta_rad, 'i',
   "g:g >rtheta:rtheta", OL_STATUS, "Not a rotation matrix."},
  {"ol_g_theta", (ol_fct) ol_g_theta, 'v', "g:g >theta:theta", 0, NULL},
  {"ol_g_theta_rad", (ol_fct) ol_g_theta_rad, 'v', "g:g >theta:theta", 0, NULL},
  {"ol_g_vect_vect", (ol_fct) ol_g_vect_vect, 'v', "g:g v1:vect >v2:vect", 0, NULL},
  {"ol_hpole_vect", (ol_fct) ol_hpole_vect, 'v', "hpole:hpole >vect:vect", 0, NULL},
  {"ol_ipf_g_stprojxy", (ol_fct) ol_ipf_g_stprojxy, 'v', "p:p vect:vect >g:g", 0, NULL},
  {"ol_ipf_q_stprojxy", (ol_fct) ol_ipf_q_stprojxy, 'v', "p:p vect:vect >q:q", 0, NULL},
  {"ol_ipfweight_rgb", (ol_fct) ol_ipfweight_rgb, 'v', "weight:real3 >rgb:int3", 0, NULL},
  {"ol_label_g", (ol_fct) ol_label_g, 'i', "label:str >g:g", OL_STATUS, "Unknown label `%s'."},
  {"ol_label_q", (ol_fct) ol_label_q, 'i', "label:str >q:q", OL_STATUS, "Unknown label `%s'."},
  {"ol_lng_g", (ol_fct) ol_lng_g, 'v', "lng:real3x3 >g:g", 0, NULL},
  {"ol_lnq_q", (ol_fct) ol_lnq_q, 'v', "lnq:real3 >q:q", 0, NULL},
  {"ol_m_crysym", (ol_fct) ol_m_crysym, 'v', "m:m crysym:str i:int >m2:m", 0, NULL},
  {"ol_m_g", (ol_fct) ol_m_g, 'v', "m:m >g:g", 0, NULL},
  {"ol_m_mcrysym", (ol_fct) ol_m_mcrysym, 'd', "m:m crysym:str >m2:m", 0, NULL},
  {"ol_m_q", (ol_fct) ol_m_q, 'v', "m:m >q:q", 0, NULL},
  {"ol_m_refsym", (ol_fct) ol_m_refsym, 'v', "m:m i:int >m2:m", 0, NULL},
  {"ol_m_refsym_monoclinic", (ol_fct) ol_m_refsym_monoclinic, 'v', "m:m >m2:m*4", 0, NULL},
  {"ol_m_refsym_orthotropic", (ol_fct) ol_m_refsym_orthotropic, 'v', "m:m >m2:m*4", 0, NULL},
  {"ol_nb_e", (ol_fct) ol_nb_e, 'v', "nb1:real nb2:real nb3:real >e:e", 0, NULL},
  {"ol_nb_e_rad", (ol_fct) ol_nb_e_rad, 'v', "nb1:real nb2:real nb3:real >e:e", 0, NULL},
  {"ol_nb_max_rtheta", (ol_fct) ol_nb_max_rtheta, 'v',
   "nb1:real nb2:real nb3:real thetamax:real >rtheta:rtheta", 0, NULL},
  {"ol_nb_max_rtheta_mis", (ol_fct) ol_nb_max_rtheta_mis, 'v',
   "nb1:real nb2:real nb3:real thetamax:real >rtheta:rtheta", 0, NULL},
  {"ol_nb_max_rtheta_mis_rad", (ol_fct) ol_nb_max_rtheta_mis_rad, 'v',
   "nb1:real nb2:real nb3:real thetamax:real >rtheta:rtheta", 0, NULL},
  {"ol_nb_max_rtheta_rad", (ol_fct) ol_nb_max_rtheta_rad, 'v',
   "nb1:real nb2:real nb3:real thetamax:real >rtheta:rtheta", 0, NULL},
  {"ol_nb_max_theta", (ol_fct) ol_nb_max_theta, 'v',
   "nb3:real thetamax:real >theta:theta", 0, NULL},
  {"ol_nb_max_theta_mis", (ol_fct) ol_nb_max_theta_mis, 'v',
   "nb3:real thetamax:real >theta:theta", 0, NULL},
  {"ol_nb_max_theta_mis_rad", (ol_fct) ol_nb_max_theta_mis_rad, 'v',
   "nb3:real thetamax:real >theta:theta", 0, NULL},
  {"ol_nb_max_theta_rad", (ol_fct) ol_nb_max_theta_rad, 'v',
   "nb3:real thetamax:real >theta:theta", 0, NULL},
  {"ol_nb_r", (ol_fct) ol_nb_r, 'v', "nb1:real nb2:real >r:r", 0, NULL},
  {"ol_pole_crysym", (ol_fct) ol_pole_crysym, 'v',
   "pole:pole crysym:str i:int >pole2:pole", 0, NULL},
  {"ol_pole_polecrysym", (ol_fct) ol_pole_polecrysym, 'v',
   "pole:pole crysym:str >pole2:pole", 0, NULL},
  {"ol_pole_vect", (ol_fct) ol_pole_vect, 'v', "pole:pole >vect:vect", 0, NULL},
  {"ol_polef_polecrysym", (ol_fct) ol_polef_polecrysym, 'v',
   "pole:pole crysym:str >pole2:pole*", 0, NULL},
  {"ol_q_R", (ol_fct) ol_q_R, 'v', "q:q >R:R", 0, NULL},
  {"ol_q_aniso_theta", (ol_fct) ol_q_aniso_theta, 'v', "q:q v:real3x3 >theta:real3", 0, NULL},
  {"ol_q_crysym", (ol_fct) ol_q_crysym, 'v', "q:q crysym:str i:int >q2:q", 0, NULL},
  {"ol_q_crysym_nopos", (ol_fct) ol_q_crysym_nopos, 'v', "q:q crysym:str i:int >q2:q", 0, NULL},
  {"ol_q_csys", (ol_fct) ol_q_csys, 'v', "q:q qs:q >q':q", 0, NULL},
  {"ol_q_e", (ol_fct) ol_q_e, 'v', "q:q >e:e", 0, NULL},
  {"ol_q_e_rad", (ol_fct) ol_q_e_rad, 'v', "q:q >e:e", 0, NULL},
  {"ol_q_g", (ol_fct) ol_q_g, 'v', "q:q >g:g", 0, NULL},
  {"ol_q_gv", (ol_fct) ol_q_gv, 'v', "q:q >gv:real9", 0, NULL},
  {"ol_q_homochoric", (ol_fct) ol_q_homochoric, 'v', "q:q >h:real3", 0, NULL},
  {"ol_q_inverse", (ol_fct) ol_q_inverse, 'v', "q:q >qinv:q", 0, NULL},
  {"ol_q_ipf_crysym_eaproj", (ol_fct) ol_q_ipf_crysym_eaproj, 'v',
   "q:q vect:vect crysym:str >p:p", 0, NULL},
  {"ol_q_ipf_crysym_eaproj_rad", (ol_fct) ol_q_ipf_crysym_eaproj_rad, 'v',
   "q:q vect:vect crysym:str >p:p", 0, NULL},
  {"ol_q_ipf_crysym_eaprojxy", (ol_fct) ol_q_ipf_crysym_eaprojxy, 'v',
   "q:q vect:vect crysym:str >p:p", 0, NULL},
  {"ol_q_ipf_crysym_stproj", (ol_fct) ol_q_ipf_crysym_stproj, 'v',
   "q:q vect:vect crysym:str >p:p", 0, NULL},
  {"ol_q_ipf_crysym_stproj_rad", (ol_fct) ol_q_ipf_crysym_stproj_rad, 'v',
   "q:q vect:vect crysym:str >p:p", 0, NULL},
  {"ol_q_ipf_crysym_stprojxy", (ol_fct) ol_q_ipf_crysym_stprojxy, 'v',
   "q:q vect:vect crysym:str >p:p", 0, NULL},
  {"ol_q_ipf_eaproj", (ol_fct) ol_q_ipf_eaproj, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_ipf_eaproj_rad", (ol_fct) ol_q_ipf_eaproj_rad, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_ipf_eaprojxy", (ol_fct) ol_q_ipf_eaprojxy, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_ipf_stproj", (ol_fct) ol_q_ipf_stproj, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_ipf_stproj_rad", (ol_fct) ol_q_ipf_stproj_rad, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_ipf_stprojxy", (ol_fct) ol_q_ipf_stprojxy, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_isspecial", (ol_fct) ol_q_isspecial, 'i', "q:q <special:int", OL_RET, NULL},
  {"ol_q_isuniform", (ol_fct) ol_q_isuniform, 'i', "q:q <uniform:int", OL_RET, NULL},
  {"ol_q_label", (ol_fct) ol_q_label, 'i',
   "q:q >label:str", OL_STATUS, "Not a special orientation."},
  {"ol_q_lnq", (ol_fct) ol_q_lnq, 'v', "q:q >lnq:real3", 0, NULL},
  {"ol_q_pf_eaproj", (ol_fct) ol_q_pf_eaproj, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_pf_eaproj_rad", (ol_fct) ol_q_pf_eaproj_rad, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_pf_eaprojxy", (ol_fct) ol_q_pf_eaprojxy, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_pf_stproj", (ol_fct) ol_q_pf_stproj, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_pf_stproj_rad", (ol_fct) ol_q_pf_stproj_rad, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_pf_stprojxy", (ol_fct) ol_q_pf_stprojxy, 'v', "q:q vect:vect >p:p", 0, NULL},
  {"ol_q_q", (ol_fct) ol_q_q, 'v', "q:q >q:q", 0, NULL},
  {"ol_q_q_disori", (ol_fct) ol_q_q_disori, 'i', "q1:q q2:q crysym:str >theta:theta", 0, NULL},
  {"ol_q_q_disori_fr_rad", (ol_fct) ol_q_q_disori_fr_rad, 'i',
   "q1:q q2:q crysym:str >theta:theta", 0, NULL},
  {"ol_q_q_disori_orthotropic", (ol_fct) ol_q_q_disori_orthotropic, 'i',
   "q1:q q2:q crysym:str >theta:theta", 0, NULL},
  {"ol_q_q_disori_orthotropic_rad", (ol_fct) ol_q_q_disori_orthotropic_rad, 'i',
   "q1:q q2:q crysym:str >theta:theta", 0, NULL},
  {"ol_q_q_disori_rad", (ol_fct) ol_q_q_disori_rad, 'i',
   "q1:q q2:q crysym:str >theta:theta", 0, NULL},
  {"ol_q_q_misori", (ol_fct) ol_q_q_misori, 'v', "q1:q q2:q >theta:theta", 0, NULL},
  {"ol_q_q_misori_orthotropic", (ol_fct) ol_q_q_misori_orthotropic, 'i',
   "q1:q q2:q >theta:theta", 0, NULL},
  {"ol_q_q_misori_orthotropic_rad", (ol_fct) ol_q_q_misori_orthotropic_rad, 'i',
   "q1:q q2:q >theta:theta", 0, NULL},
  {"ol_q_q_misori_rad", (ol_fct) ol_q_q_misori_rad, 'v', "q1:q q2:q >theta:theta", 0, NULL},
  {"ol_q_q_q", (ol_fct) ol_q_q_q, 'v', "q1:q q2:q >qres:q", 0, NULL},
  {"ol_q_q_q_cur", (ol_fct) ol_q_q_q_cur, 'v', "q1:q q2:q >qres:q", 0, NULL},
  {"ol_q_q_q_ref", (ol_fct) ol_q_q_q_ref, 'v', "q1:q q2:q >qres:q", 0, NULL},
  {"ol_q_q_qdisori", (ol_fct) ol_q_q_qdisori, 'i', "q1:q q2:q crysym:str >qd:q", 0, NULL},
  {"ol_q_q_qdisori_cur", (ol_fct) ol_q_q_qdisori_cur, 'i', "q1:q q2:q crysym:str >qd:q", 0, NULL},
  {"ol_q_q_qdisori_orthotropic", (ol_fct) ol_q_q_qdisori_orthotropic, 'i',
   "q1:q q2:q crysym:str >qd:q", 0, NULL},
  {"ol_q_q_qdisori_ref", (ol_fct) ol_q_q_qdisori_ref, 'i', "q1:q q2:q crysym:str >qd:q", 0, NULL},
  {"ol_q_q_qdisori_update", (ol_fct) ol_q_q_qdisori_update, 'i',
   "q1:q q2:q crysym:str prevnb:int >qd:q flag:str >status:int", 0, NULL},
  {"ol_q_q_qmisori", (ol_fct) ol_q_q_qmisori, 'v', "q1:q q2:q >qm:q", 0, NULL},
  {"ol_q_q_qmisori_cur", (ol_fct) ol_q_q_qmisori_cur, 'v', "q1:q q2:q >qm:q", 0, NULL},
  {"ol_q_q_qmisori_orthotropic", (ol_fct) ol_q_q_qmisori_orthotropic, 'i',
   "q1:q q2:q >qd:q", 0, NULL},
  {"ol_q_q_qmisori_ref", (ol_fct) ol_q_q_qmisori_ref, 'v', "q1:q q2:q >qm:q", 0, NULL},
  {"ol_q_qcrysym", (ol_fct) ol_q_qcrysym, 'd', "q:q crysym:str >q2:q", 0, NULL},
  {"ol_q_qrefsym_orthotropic", (ol_fct) ol_q_qrefsym_orthotropic, 'i', "q:q >q2:q", 0, NULL},
  {"ol_q_r", (ol_fct) ol_q_r, 'v', "q:q >r:r", 0, NULL},
  {"ol_q_refcsys", (ol_fct) ol_q_refcsys, 'v', "q:q qs:q >qp:q", 0, NULL},
  {"ol_q_refsym", (ol_fct) ol_q_refsym, 'v', "q:q i:int >q2:q", 0, NULL},
  {"ol_q_refsym_monoclinic", (ol_fct) ol_q_refsym_monoclinic, 'v', "q:q >q2:q*4", 0, NULL},
  {"ol_q_refsym_orthotropic", (ol_fct) ol_q_refsym_orthotropic, 'v', "q:q >q2:q*4", 0, NULL},
  {"ol_q_refsym_orthotropic_all", (ol_fct) ol_q_refsym_orthotropic_all, 'v',
   "q:q >q2:q*=4", 0, NULL},
  {"ol_q_refsym_q", (ol_fct) ol_q_refsym_q, 'v', "q:q i:int >qs:q", 0, NULL},
  {"ol_q_rtheta", (ol_fct) ol_q_rtheta, 'v', "q:q >rtheta:rtheta", 0, NULL},
  {"ol_q_rtheta_rad", (ol_fct) ol_q_rtheta_rad, 'v', "q:q >rtheta:rtheta", 0, NULL},
  {"ol_q_specialid", (ol_fct) ol_q_specialid, 'i', "q:q <id:int", OL_RET, NULL},
  {"ol_q_theta", (ol_fct) ol_q_theta, 'v', "q:q >theta:theta", 0, NULL},
  {"ol_q_theta_rad", (ol_fct) ol_q_theta_rad, 'v', "q:q >theta:theta", 0, NULL},
  {"ol_q_vect_vect", (ol_fct) ol_q_vect_vect, 'v', "q:q v1:vect >v2:real3", 0, NULL},
  {"ol_rtheta_R", (ol_fct) ol_rtheta_R, 'v', "rtheta:rtheta >R:R", 0, NULL},
  {"ol_rtheta_R_rad", (ol_fct) ol_rtheta_R_rad, 'v', "rtheta:rtheta >R:R", 0, NULL},
  {"ol_rtheta_g", (ol_fct) ol_rtheta_g, 'v', "rtheta:rtheta >g:g", 0, NULL},
  {"ol_rtheta_g_rad", (ol_fct) ol_rtheta_g_rad, 'v', "rtheta:rtheta >g:g", 0, NULL},
  {"ol_rtheta_q", (ol_fct) ol_rtheta_q, 'v', "rtheta:rtheta >q:q", 0, NULL},
  {"ol_rtheta_q_rad", (ol_fct) ol_rtheta_q_rad, 'v', "rtheta:rtheta >q:q", 0, NULL},
  {"ol_rtheta_rtheta", (ol_fct) ol_rtheta_rtheta, 'v', "rtheta:rtheta >rtheta:rtheta", 0, NULL},
  {"ol_rtheta_rtheta_rad", (ol_fct) ol_rtheta_rtheta_rad, 'v',
   "rtheta:rtheta >rtheta:rtheta", 0, NULL},
  {"ol_specialid_label", (ol_fct) ol_specialid_label, 'v', "id:int >label:str", 0, NULL},
  {"ol_specialid_q", (ol_fct) ol_specialid_q, 'v', "id:int >q:q", 0, NULL},
  {"ol_srand_e", (ol_fct) ol_srand_e, 'v', "seed:int qty:int >e:e*=qty", 0, NULL},
  {"ol_srand_e_rad", (ol_fct) ol_srand_e_rad, 'v', "seed:int qty:int >e:e*=qty", 0, NULL},
  {"ol_stproj_vect", (ol_fct) ol_stproj_vect, 'v', "p:p >vect:vect", 0, NULL},
  {"ol_stproj_vect_rad", (ol_fct) ol_stproj_vect_rad, 'v', "p:p >vect:vect", 0, NULL},
  {"ol_stprojxy_vect", (ol_fct) ol_stprojxy_vect, 'v', "p:p >vect:vect", 0, NULL},
  {"ol_theta_deg2rad", (ol_fct) ol_theta_deg2rad, 'v', "theta:theta >theta:theta", 0, NULL},
  {"ol_theta_rad2deg", (ol_fct) ol_theta_rad2deg, 'v', "theta:theta >theta:theta", 0, NULL},
  {"ol_vect_crysym", (ol_fct) ol_vect_crysym, 'v', "v:vect crysym:str i:int >v2:real3", 0, NULL},
  {"ol_vect_crysym_all", (ol_fct) ol_vect_crysym_all, 'v', "v:vect crysym:str >v2:vect*", 0, NULL},
  {"ol_vect_eaproj", (ol_fct) ol_vect_eaproj, 'v', "vect:vect >p:p", 0, NULL},
  {"ol_vect_eaproj_rad", (ol_fct) ol_vect_eaproj_rad, 'v', "vect:vect >p:p", 0, NULL},
  {"ol_vect_eaprojxy", (ol_fct) ol_vect_eaprojxy, 'v', "vect:vect >p:p", 0, NULL},
  {"ol_vect_ipfweight", (ol_fct) ol_vect_ipfweight, 'i', "vect:vect crysym:str >weight:e", 0, NULL},
  {"ol_vect_refsym", (ol_fct) ol_vect_refsym, 'v', "vect:vect i:int >vect2:vect", 0, NULL},
  {"ol_vect_stproj", (ol_fct) ol_vect_stproj, 'v', "vect:vect >p:p", 0, NULL},
  {"ol_vect_stproj_rad", (ol_fct) ol_vect_stproj_rad, 'v', "vect:vect >p:p", 0, NULL},
  {"ol_vect_stprojxy", (ol_fct) ol_vect_stprojxy, 'v', "vect:vect >p:p", 0, NULL},
  {"ol_vect_vect_g", (ol_fct) ol_vect_vect_g, 'v', "v1:vect v2:vect >g:g", 0, NULL},
  {"ol_vect_vect_q", (ol_fct) ol_vect_vect_q, 'v', "v1:vect v2:vect >q:q", 0, NULL},
  {"ol_vect_vect_rtheta", (ol_fct) ol_vect_vect_rtheta, 'v', "v1:r v2:r >rtheta:rtheta", 0, NULL},
  {"ol_vect_vect_rtheta_rad", (ol_fct) ol_vect_vect_rtheta_rad, 'v',
   "v1:r v2:r >rtheta:rtheta", 0, NULL},
  {"ol_vect_vect_theta", (ol_fct) ol_vect_vect_theta, 'v', "v1:r v2:r >theta:theta", 0, NULL},
  {"ol_vect_vect_theta_rad", (ol_fct) ol_vect_vect_theta_rad, 'v',
   "v1:r v2:r >theta:theta", 0, NULL},
  {NULL, NULL, 0, NULL, 0, NULL}
};

// general commands --------------------------------------------------------

static int
orilib_strcmp (const void *a, const void *b)
{
  return strcmp (*(const char **) a, *(const char **) b);
}

static void
orilib_listall (void)
{
  int i, qty = 0;
  const char *names[1000];

  for (i = 0; ol_coms[i].name; i++)
    names[qty++] = ol_coms[i].name;
  for (i = 0; ol_customs[i].name; i++)
    names[qty++] = ol_customs[i].name;

  qsort (names, qty, sizeof (char *), orilib_strcmp);

  for (i = 0; i < qty; i++)
    printf ("    %s\n", names[i]);
}

// signature of a command: its inputs and outputs, as <name>:<type> and
// ><name>:<type> (a list being written <type>*)
static void
orilib_signature (const char *name, const char *spec)
{
  int j, argqty;
  struct OL_ARG args[20];

  argqty = orilib_args (spec, args);

  printf ("%s", name);
  for (j = 0; j < argqty; j++)
    if (args[j].dir == 0)
      printf (" %s:%s", args[j].name, args[j].type->name);
  for (j = 0; j < argqty; j++)
    if (args[j].dir)
      printf (" >%s:%s%s", args[j].name, args[j].type->name, args[j].islist ? "*" : "");
  printf ("\n");
}

static void
orilib_signatures (void)
{
  int i;

  for (i = 0; ol_coms[i].name; i++)
    orilib_signature (ol_coms[i].name, ol_coms[i].args);
  for (i = 0; ol_customs[i].name; i++)
    if (ol_customs[i].args)
      orilib_signature (ol_customs[i].name, ol_customs[i].args);
}

static void
orilib_help (void)
{
  printf ("    This is the interactive program of Orilib.  It makes it possible\n");
  printf ("    to run its functions (but memory management, input / output and\n");
  printf ("    structure-based ones).  Type `listall' to list them, and\n");
  printf ("    `signatures' to list their inputs and outputs.  To run a\n");
  printf ("    function, type its name and give the requested input data.  To\n");
  printf ("    quit, type `quit'.  For more help, please refer to the\n");
  printf ("    documentation.\n");
}

static void
orilib_license (void)
{
  printf ("    Orilib\n");
  printf ("    Copyright (C) 2007-2026 Romain Quey, CNRS\n");
  printf ("\n");
  printf ("    This program is free software: you can redistribute it and/or modify\n");
  printf ("    it under the terms of the GNU General Public License as published by\n");
  printf ("    the Free Software Foundation, either version 3 of the License, or\n");
  printf ("    (at your option) any later version.\n");
  printf ("\n");
  printf ("    This program is distributed in the hope that it will be useful,\n");
  printf ("    but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  printf ("    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  printf ("    GNU General Public License for more details.\n");
  printf ("\n");
  printf ("    You should have received a copy of the GNU General Public License\n");
  printf ("    along with this program.  If not, see\n");
  printf ("    <http://www.gnu.org/licenses/>.\n");
}

// running a command; returns 0, or -1 if the command is unknown
static int
orilib_command (const char *com)
{
  int i;

  for (i = 0; ol_coms[i].name; i++)
    if (!strcmp (com, ol_coms[i].name))
    {
      orilib_run (ol_coms + i);
      return 0;
    }

  for (i = 0; ol_customs[i].name; i++)
    if (!strcmp (com, ol_customs[i].name))
    {
      ol_customs[i].fct ();
      return 0;
    }

  if (!strcmp (com, "help") || !strcmp (com, "h"))
    orilib_help ();
  else if (!strcmp (com, "listall"))
    orilib_listall ();
  else if (!strcmp (com, "signatures"))
    orilib_signatures ();
  else if (!strcmp (com, "license"))
    orilib_license ();
  else
    return -1;

  return 0;
}

int
main (int argv, char **argc)
{
  volatile int loop;
  char com[1000];

  orilib_argv = argv;
  orilib_interactive = (argv == 1);

  if (getenv ("ORILIB_FORMAT") && strlen (getenv ("ORILIB_FORMAT")) < 100)
    strcpy (orilib_fmt, getenv ("ORILIB_FORMAT"));

  if (argv == 1)
  {
    printf ("\n======================    O   r   i   l   i   b    =====================\n");
    ut_print_message (0, 0, "A library of rotation and orientation routines.\n");
    ut_print_message (0, 0, "Version %s\n", NEPER_VERSION);
    ut_print_message (0, 0, "<https://neper.info>\n");
    ut_print_message (0, 0, "Copyright (C) 2007-2026, and GNU GPL'd, by Romain Quey, CNRS.\n");
    ut_print_message (0, 0, "To list all functions, type `listall'.  To get help, type `help'.  "
                            "To print the license, type `license'.  To quit, type `quit'.\n");
    printf ("========================================================================\n\n");
  }

  loop = 0;
  strcpy (com, "");

  if (argv == 2)
  {
    if (!strcmp (argc[1], "loop") || !strcmp (argc[1], "noloop"))
      return 0;
    strcpy (com, argc[1]);
  }
  else if (argv == 3)
  {
    if (!strcmp (argc[1], "loop"))
      loop = 1;
    else if (strcmp (argc[1], "noloop"))
    {
      ut_print_message (2, 0, "First argument must be one of: [no]loop.\n");
      return -1;
    }
    strcpy (com, argc[2]);
  }
  else if (argv > 3)
  {
    ut_print_message (2, 0, "Too many arguments.\n");
    return -1;
  }

  do
  {
    // back here on invalid input data (interactive mode)
    setjmp (orilib_nextcom);

    // interactive mode: reading the command (unless repeated in loop mode)
    if (argv == 1 && !(loop && strcmp (com, "loop") && strcmp (com, "")))
    {
      // end of input: quit
      if (scanf ("%999s", com) != 1)
        break;
    }

    if (!strcmp (com, "quit") || !strcmp (com, "q"))
      break;
    else if (!strcmp (com, "loop"))
      loop = 1;
    else if (!strcmp (com, "noloop"))
      loop = 0;
    else if (orilib_command (com))
    {
      // non-interactive mode: error (aborts)
      if (argv != 1)
        ut_print_message (2, 0, "Function `%s' unknown.\n", com);

      printf ("Function unknown.  Type `help' for help.\n");

      // interactive loop mode: ask for the function again
      if (loop)
        strcpy (com, "loop");
    }

    // end of the output of a command, marked if environment variable
    // ORILIB_ENDMARK is set (to parse the outputs of loop mode)
    if (argv == 3 && getenv ("ORILIB_ENDMARK"))
      printf ("%s\n", getenv ("ORILIB_ENDMARK"));
  }
  while (argv == 1 || (argv == 3 && loop));

  return 0;
}

void
orilib_printf (int argv, const char *s, ...)
{
  va_list args;
  va_start (args, s);

  if (argv == 1)
    vfprintf (stdout, s, args);

  va_end (args);

  return;
}
