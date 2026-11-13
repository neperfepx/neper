/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ut.h"

void
ut_arg_nextasint (char **argv, int *pi, char *option, int min, int max,
                  int *pres)
{
  (*pi)++;

  if (!argv[*pi])
    ut_arg_error (option, "missing value.");

  if (ut_math_eval_int (argv[*pi], 0, NULL, NULL, pres) != 0)
    ut_arg_error (option, "bad int value.");

  if (*pres < min || *pres > max)
  {
    printf ("min = %d max = %d\n", min, max);
    ut_arg_error (option, "bad int value.");
  }

  return;
}

void
ut_arg_nextaslogical (char **argv, int *pi, char *option, int *pres)
{
  ut_arg_nextasint (argv, pi, option, 0, 1, pres);

  return;
}


void
ut_arg_nextasreal (char **argv, int *pi, char *option, double min, double max,
                   double *pres)
{
  double eps = 1e-15;

  /* incrementing i */
  (*pi)++;

  if (!argv[*pi])
    ut_arg_error (option, "missing value.");

  if (ut_math_eval (argv[*pi], 0, NULL, NULL, pres) != 0)
    ut_arg_error (option, "bad real value.");

  if (*pres < min - eps || *pres > max + eps)
  {
    printf ("min = %f max = %f\n", min, max);
    ut_arg_error (option, "bad real value.");
  }

  return;
}

void
ut_arg_nextasstring (char **argv, int *pi, char *option, char **pstring)
{
  /* incrementing i */
  (*pi)++;

  if (!argv[*pi])
    ut_arg_error (option, "missing value.");

  ut_string_string (argv[*pi], pstring);

  return;
}


void
ut_arg_badarg (void)
{
  ut_print_lineheader (2);
  printf ("Bad arguments!\n\n");
  abort ();

  return;
}

void
ut_arg_error (char *arg, char *explaination)
{
  ut_print_lineheader (2);
  printf ("argument \'%s\': %s\n", arg, explaination);
  abort ();

  return;
}

int
ut_arg_listqty (char *arg)
{
  return 1 + ut_string_thischarqty (arg, ',');
}

char *
ut_arg_listmember (char *arg, int qty, int nb)
{
  int i, pos1, pos2, length;
  char *res;

  /* searching pos1, the character/comma before the arg */
  pos1 = ut_string_thischarpos (arg, ',', nb - 1);

  /* searching pos2, the character/comma after the arg */
  if (nb < qty)
    pos2 = ut_string_thischarpos (arg, ',', nb);
  else
    pos2 = strlen (arg);

  length = pos2 - pos1 - 1;
  res = ut_alloc_1d_char (length + 1);
  for (i = 0; i < length; i++)
    res[i] = arg[pos1 + i + 1];
  res[length] = '\0';

  return res;
}

void
ut_arg_expand (int argc, char **argv, int *pexpargc, char ***pexpargv)
{
  int i, ibeg, iend, j, k, length;
  char *var = NULL;
  char *nextvar = NULL;
  char *prevvar = NULL;
  double value, beg, stp, end, eps;
  char *tmp = NULL;
  char *tmp2 = ut_alloc_1d_char (1000);
  char *tmp3 = ut_alloc_1d_char (1000);
  char *tmp4 = ut_alloc_1d_char (1000);
  char *format = ut_alloc_1d_char (1000);
  char conv;

  (*pexpargc) = 0;
  (*pexpargv) = ut_alloc_1d_pchar (1);

  for (i = 1; i < argc; i++)
  {
    if (strcmp (argv[i], "-loop") == 0)
    {
      if (i + 4 >= argc)
        ut_arg_error ("-loop", "missing values.");

      ut_string_string (argv[++i], &var);
      if (sscanf (argv[++i], "%lf", &beg) != 1)
        ut_arg_error ("-loop", "bad start value.");
      ut_string_format (argv[++i], format);
      if (sscanf (argv[i], "%lf", &stp) != 1 || stp <= 0)
        ut_arg_error ("-loop", "bad step value.");
      if (sscanf (argv[++i], "%lf", &end) != 1)
        ut_arg_error ("-loop", "bad end value.");
      eps = stp * 1e-3;

      conv = format[strlen (format) - 1];
      if (conv != 'f' && conv != 'd')
        ut_arg_error ("-loop", "bad step value.");

      ut_string_sprintf (&prevvar, "PREV%s", var);
      ut_string_sprintf (&nextvar, "NEXT%s", var);

      ibeg = ++i;
      iend = argc - 1;

      for (j = ibeg; j < argc; j++)
        if (strcmp (argv[j], "-endloop") == 0)
        {
          iend = j - 1;
          break;
        }

      for (k = 0; (value = beg + k * stp) <= end + eps; k++)
      {
        for (i = ibeg; i <= iend; i++)
        {
          if (conv == 'f')
          {
            snprintf (tmp2, 1000, format, value);
            snprintf (tmp3, 1000, format, value - stp);
            snprintf (tmp4, 1000, format, value + stp);
          }
          else
          {
            snprintf (tmp2, 1000, format, ut_num_d2ri (value));
            snprintf (tmp3, 1000, format, ut_num_d2ri (value - stp));
            snprintf (tmp4, 1000, format, ut_num_d2ri (value + stp));
          }

          // the replacements are done in place, so making room for them
          length = ut_num_max_int (strlen (tmp2),
                                   ut_num_max_int (strlen (tmp3), strlen (tmp4)));
          tmp = ut_realloc_1d_char (tmp, (strlen (argv[i]) + 1) * (length + 1));
          strcpy (tmp, argv[i]);

          ut_string_fnrs (tmp, prevvar, tmp3, 100);
          ut_string_fnrs (tmp, nextvar, tmp4, 100);
          ut_string_fnrs (tmp, var, tmp2, 100);

          (*pexpargc)++;
          (*pexpargv) = ut_realloc_1d_pchar_null ((*pexpargv), (*pexpargc) + 1, 1);
          ut_string_string (tmp, (*pexpargv) + (*pexpargc));
        }
      }
    }
    else
    {
      (*pexpargc)++;
      (*pexpargv) = ut_realloc_1d_pchar_null ((*pexpargv), (*pexpargc) + 1, 1);
      ut_string_string (argv[i], (*pexpargv) + (*pexpargc));
    }
  }

  ut_free_1d_char (&var);
  ut_free_1d_char (&prevvar);
  ut_free_1d_char (&nextvar);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);
  ut_free_1d_char (&tmp3);
  ut_free_1d_char (&tmp4);
  ut_free_1d_char (&format);

  return;
}

void
ut_arg_readfromfile (char *argfile, char *program, char *module, int *pargc,
                     char ***pargv)
{
  char Trash[1000];
  int test;
  FILE *file = NULL;

  if (!ut_file_exist (argfile))
  {
    (*pargc) = 0;
    return;
  }

  file = ut_file_open (argfile, "R");

  while (fscanf (file, "%999s", Trash) == 1)
    if (strcmp (Trash, program) == 0)
      if (fscanf (file, "%999s", Trash) == 1)
        if (strcmp (Trash, module) == 0)
          break;

  /* Reading args */
  (*pargc) = 1;
  (*pargv) = ut_alloc_1d_pchar ((*pargc) + 1);
  (*pargv)[0] = NULL;
  do
  {
    (*pargv) = ut_realloc_1d_pchar ((*pargv), (*pargc) + 1);
    (*pargv)[(*pargc)] = ut_alloc_1d_char (1000);
    test = fscanf (file, "%999s", (*pargv)[(*pargc)]);
    (*pargv)[(*pargc)] =
      ut_realloc_1d_char ((*pargv)[(*pargc)],
                          strlen ((*pargv)[(*pargc)]) + 1);
    (*pargc)++;
  }
  while (test != -1 && strcmp ((*pargv)[(*pargc) - 1], program) != 0);

  (*pargc)--;                   /* the last one is nok. */

  ut_file_close (file, argfile, "R");

  return;
}
