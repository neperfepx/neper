/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include<assert.h>
#include"ut.h"

#ifdef HAVE_LIBMATHEVAL
#include<matheval.h>
#endif

int
ut_math_eval_ww (char* expr, int var_qty, char** vars, double* vals,
		 double* pres)
{
  int i;
  int status;
  int val, val2;
  int knownexpr = -1;
  unsigned int length;
  char* format = ut_alloc_1d_char (10);

  status = -1;

  ut_string_format (expr, format);

  if (format[strlen(format) - 1] == 'd'
   || format[strlen(format) - 1] == 'f')
  {
    sscanf (expr, "%lf", pres);
    status = 1;
  }
  else
  {
    for (i = 0; i < var_qty; i++)
      if (strncmp (expr, vars[i], strlen (vars[i])) == 0)
      {
	knownexpr = i;
	break;
      }

    if (knownexpr != -1)
    {
      status = 1;
      val = vals[i];
      length = strlen (vars[i]);

      if (strlen (expr) == length)
	(*pres) = vals[i];
      else if (strlen (expr) == length)
	(*pres) = vals[i];
      else if (expr[0] == '-' && strlen (expr) == length + 1)
	(*pres) = -vals[i];
      else if (strncmp (expr + length, "==", 2) == 0 \
       && sscanf (expr + length + 2, "%d", &val2) == 1)
	(*pres) = (val == val2) ? 1 : 0;
      else if (strncmp (expr + length, ">=", 2) == 0 \
       && sscanf (expr + length + 2, "%d", &val2) == 1)
	(*pres) = (val >= val2) ? 1 : 0;
      else if (strncmp (expr + length, "<=", 2) == 0 \
       && sscanf (expr + length + 2, "%d", &val2) == 1)
	(*pres) = (val <= val2) ? 1 : 0;
      else if (strncmp (expr + length, "!=", 2) == 0 \
       && sscanf (expr + length + 2, "%d", &val2) == 1)
	(*pres) = (val != val2) ? 1 : 0;
      else if (strncmp (expr + length, ">", 1) == 0 \
       && sscanf (expr + length + 1, "%d", &val2) == 1)
	(*pres) = (val > val2) ? 1 : 0;
      else if (strncmp (expr + length, "<", 1) == 0 \
       && sscanf (expr + length + 1, "%d", &val2) == 1)
	(*pres) = (val < val2) ? 1 : 0;
      else
	status = -1;
    }
  }

  ut_free_1d_char (format);

  return status;
}

int
ut_math_eval (char* expr, int var_qty, char** vars, double* vals,
              double* pres)
{
#ifdef HAVE_LIBMATHEVAL
  FILE* f = NULL;
#endif
  int pos;
  int i, status;
  int opqty;
  char** oplist = ut_alloc_2d_char (8, 3);
  char* op = ut_alloc_1d_char (3);
  double val1, val2;
  char* exprcpy        = ut_alloc_1d_char (20 * strlen (expr) + 1);
  char* subexpr        = ut_alloc_1d_char (20 * strlen (expr) + 1);
  char* subexpr_before = ut_alloc_1d_char (20 * strlen (expr) + 1);
  char* subexpr_after  = ut_alloc_1d_char (20 * strlen (expr) + 1);
  char* expr_right     = ut_alloc_1d_char (20 * strlen (expr) + 1);
  char* expr_left      = ut_alloc_1d_char (20 * strlen (expr) + 1);

  opqty = 0;
  sprintf (oplist[opqty++], "==");
  sprintf (oplist[opqty++], "!=");
  sprintf (oplist[opqty++], "<=");
  sprintf (oplist[opqty++], ">=");
  sprintf (oplist[opqty++], "<");
  sprintf (oplist[opqty++], ">");
  sprintf (oplist[opqty++], "||");
  sprintf (oplist[opqty++], "&&");

  sprintf (exprcpy, "%s", expr);
  while (ut_math_eval_subeval (exprcpy, subexpr, subexpr_before, subexpr_after) == 1)
  {
    ut_math_eval (subexpr, var_qty, vars, vals, pres);
    sprintf (exprcpy, "%s%.12g%s", subexpr_before, *pres, subexpr_after);
  }

  pos = -1;
  for (i = 0; i < opqty; i++)
  {
    pos = ut_string_stringpos (exprcpy, oplist[i], 1);
    if (pos >= 0)
    {
      sprintf (op, "%s", oplist[i]);
      break;
    }
  }

  ut_math_eval_substitute (exprcpy, &exprcpy);

  if (pos == -1)
  {
    status = ut_math_eval_ww (exprcpy, var_qty, vars, vals, pres);

    if (status == -1)
    {
#ifdef HAVE_LIBMATHEVAL
      f = evaluator_create (exprcpy);
      if (f == NULL)
      {
	ut_print_message (2, 0, "matheval failed to process the expression.  The expression syntax is probably incorrect.  This may also be caused by an unproper matheval installation (expression = %s).\n", exprcpy);
	abort ();
      }

      (*pres) = evaluator_evaluate (f, var_qty, (char**) vars, vals);
      evaluator_destroy (f);
# else
      ut_print_messagewnc (2, 72, "The expression evaluation failed because the program was compiled without libmatheval support.");
      printf ("expression = %s\n", exprcpy);
      abort ();
#endif
    }
  }
  else
  {
    strncpy (expr_left, exprcpy, pos);
    expr_left[pos] = '\0';
    sprintf (expr_right, "%s", exprcpy + pos + strlen (op));

    status = ut_math_eval_ww (expr_left, var_qty, vars, vals, &val1);

    if (status == -1)
    {
#ifdef HAVE_LIBMATHEVAL
      f = evaluator_create (expr_left);
      if (f == NULL)
      {
	ut_print_message (2, 0, "matheval failed to process the expression.  The expression syntax is probably incorrect.  This may also be caused by an unproper matheval installation (expression = %s).\n", expr_left);
	abort ();
      }

      val1 = evaluator_evaluate (f, var_qty, (char**) vars, vals);
      evaluator_destroy (f);
#else
      ut_print_messagewnc (2, 72, "The expression evaluation failed because the program was compiled without libmatheval support.");
      printf ("expression = %s\n", expr_left);
      abort ();
#endif
    }

    status = ut_math_eval_ww (expr_right, var_qty, vars, vals, &val2);

    if (status == -1)
    {
#ifdef HAVE_LIBMATHEVAL
      f = evaluator_create (expr_right);
      if (f == NULL)
      {
	ut_print_message (2, 0, "matheval failed to process the expression.  The expression syntax is probably incorrect.  This may also be caused by an unproper matheval installation (expression = %s).\n", expr_right);
	return -1;
      }

      val2 = evaluator_evaluate (f, var_qty, (char**) vars, vals);
      evaluator_destroy (f);
#else
      ut_print_messagewnc (2, 72, "The expression evaluation failed because the program was compiled without libmatheval support.");
      printf ("expression = %s\n", expr_right);
      abort ();
#endif
    }

    if (strcmp (op, "==") == 0)
      (*pres) = (val1 == val2)? 1: 0;
    if (strcmp (op, "!=") == 0)
      (*pres) = (val1 != val2)? 1: 0;
    else if (strcmp (op, "<=") == 0)
      (*pres) = (val1 <= val2)? 1: 0;
    else if (strcmp (op, ">=") == 0)
      (*pres) = (val1 >= val2)? 1: 0;
    else if (strcmp (op, "<") == 0)
      (*pres) = (val1 < val2)? 1: 0;
    else if (strcmp (op, ">") == 0)
      (*pres) = (val1 > val2)? 1: 0;
    else if (strcmp (op, "||") == 0)
      (*pres) = (ut_num_d2ri (val1) || ut_num_d2ri (val2))? 1: 0;
    else if (strcmp (op, "&&") == 0)
      (*pres) = ((ut_num_d2ri (val1) == 1) && (ut_num_d2ri (val2) == 1))? 1: 0;
  }

  ut_free_1d_char (expr_right);
  ut_free_1d_char (expr_left);
  ut_free_2d_char (oplist, opqty);
  ut_free_1d_char (op);
  ut_free_1d_char (exprcpy);
  ut_free_1d_char (subexpr);
  ut_free_1d_char (subexpr_before);
  ut_free_1d_char (subexpr_after);

  return 0;
}

int
ut_math_eval_int (char* expr, int var_qty, char** vars, double* vals,
                  int* pres)
{
  double tmp;

  ut_math_eval (expr, var_qty, vars, vals, &tmp);
  (*pres) = ut_num_d2ri (tmp);

  return 0;
}

int
ut_math_eval_subeval (char* in, char* out, char* before_out, char* after_out)
{
  int i, j;
  int length = strlen (in);
  int openpos, closepos;
  int status;
  int oppos;
  int skipop;
  char* op = ut_alloc_1d_char (strlen  (in) + 1);

  skipop = 0;
  openpos = closepos = -1;
  status = -1;
  // looking for an opening parenthesis
  for (i = 0; i < length; i++)
  {
    if (in[i] == '(')
    {
      // checking if it's a valid '(' by looking at the string right before
      // first going back to the beginning
      // if there is a "word" such as cos, log10, etc., '(' is not valid.
      oppos = 0;
      for (j = i - 1; j >= 0; j--)
	if ((in[j] < 'a' || in[j] > 'z')
	 && (in[j] < 'A' || in[j] > 'Z')
	 && (in[j] < '0' || in[j] > '9'))
	{
	  oppos = j + 1;
	  break;
	}

      if (oppos == i) // mean no operator before, valid
      {
	openpos = i;
	skipop = 0;
      }
      else if (openpos != -1)
	skipop++;
    }

    if (in[i] == ')')
    {
      if (skipop > 0)
        skipop--;
      else
      {
        closepos = i;
        break;
      }
    }
  }

  status = 0;
  if (openpos != -1 && closepos != -1)
  {
    strncpy (before_out, in, openpos);
    before_out[openpos] = '\0';
    strncpy (out       , in + openpos + 1, closepos - openpos - 1);
    out[closepos - openpos - 1] = '\0';
    sprintf (after_out , "%s", in + closepos + 1);
    status = 1;
    if (strlen (before_out) == 0 && strlen (after_out) == 0)
      status = 0;
  }

  ut_free_1d_char (op);

  return status;
}

int
ut_math_eval_substitute (char* in, char** pout)
{
  char* tmp = NULL;
  char* in_cpy = ut_alloc_1d_char (strlen (in) + 1);

  strcpy (in_cpy, in);

  (*pout) = ut_alloc_1d_char (strlen (in_cpy) + 1);
  strcpy ((*pout), in_cpy);

  ut_string_fnr (*pout, '[', '(');
  ut_string_fnr (*pout, '{', '(');
  ut_string_fnr (*pout, ']', ')');
  ut_string_fnr (*pout, '}', ')');

  while (ut_math_eval_substitute_max ((*pout), &tmp) > 0)
  {
    ut_free_1d_char (*pout);
    (*pout) = ut_alloc_1d_char (strlen (tmp) + 1);
    strcpy ((*pout), tmp);
    ut_free_1d_char (tmp);
  }

  while (ut_math_eval_substitute_min ((*pout), &tmp) > 0)
  {
    ut_free_1d_char (*pout);
    (*pout) = ut_alloc_1d_char (strlen (tmp) + 1);
    strcpy ((*pout), tmp);
    ut_free_1d_char (tmp);
  }

  ut_free_1d_char (in_cpy);

  // FILE* f = evaluator_create (*pout);
  // if (f == NULL)
    // return -1;

  return 0;
}

int
ut_math_eval_substitute_max (char* in, char** pout)
{
  int i;
  int pos_max, pos_opn, pos_cls, pos_com, pos;
  char* arg1 = NULL;
  char* arg2 = NULL;
  char* in_bef = NULL;
  char* in_aft = NULL;

  pos_max = ut_string_finds (in, "max");

  if (pos_max < 0)
  {
    (*pout) = ut_alloc_1d_char (strlen (in) + 1);
    sprintf ((*pout), "%s", in);
    return 0;
  }

  arg1 = ut_alloc_1d_char (strlen (in + 1));
  arg2 = ut_alloc_1d_char (strlen (in + 1));
  in_bef = NULL;
  in_aft = NULL;

  pos_opn = pos_max + 3;
  if (in[pos_opn] != '(')
    return -1;

  pos = pos_opn + 1;
  ut_string_untilchar (in, ',', &pos, arg1);
  pos++;
  ut_string_untilchar (in, ')', &pos, arg2);

  pos_com = pos_opn + ut_string_find (in + pos_opn, ',');
  if (pos_com < pos_opn)
    return -1;

  pos_cls = pos_com + ut_string_find (in + pos_com, ')');
  if (pos_cls < pos_com)
    return -1;

  in_bef = ut_alloc_1d_char (strlen (in + 1));
  in_aft = ut_alloc_1d_char (strlen (in + 1));

  for (i = 0; i < pos_max; i++)
    in_bef[i] = in[i];
  in_bef[i] = '\0';

  for (i = pos_cls + 1; i < (int) strlen (in); i++)
    in_aft[i - (pos_cls + 1)] = in[i];
  in_aft[i  - (pos_cls + 1)] = '\0';

  (*pout) = ut_alloc_1d_char (strlen (in) + 24);
  sprintf (*pout, "%s((%s+%s)/2+sqrt(((%s-%s)/2)^2))%s", in_bef, arg1, arg2, arg1, arg2,in_aft);

  ut_free_1d_char (arg1);
  ut_free_1d_char (arg2);
  ut_free_1d_char (in_bef);
  ut_free_1d_char (in_aft);

  return 1;
}

int
ut_math_eval_substitute_min (char* in, char** pout)
{
  int i;
  int pos_min, pos_opn, pos_cls, pos_com, pos;
  char* arg1 = NULL;
  char* arg2 = NULL;
  char* in_bef = NULL;
  char* in_aft = NULL;

  pos_min = ut_string_finds (in, "min");

  if (pos_min < 0)
  {
    (*pout) = ut_alloc_1d_char (strlen (in) + 1);
    sprintf ((*pout), "%s", in);
    return 0;
  }

  arg1 = ut_alloc_1d_char (strlen (in + 1));
  arg2 = ut_alloc_1d_char (strlen (in + 1));
  in_bef = NULL;
  in_aft = NULL;

  pos_opn = pos_min + 3;
  if (in[pos_opn] != '(')
    return -1;

  pos = pos_opn + 1;
  ut_string_untilchar (in, ',', &pos, arg1);
  pos++;
  ut_string_untilchar (in, ')', &pos, arg2);

  pos_com = pos_opn + ut_string_find (in + pos_opn, ',');
  if (pos_com < pos_opn)
    return -1;

  pos_cls = pos_com + ut_string_find (in + pos_com, ')');
  if (pos_cls < pos_com)
    return -1;

  in_bef = ut_alloc_1d_char (strlen (in + 1));
  in_aft = ut_alloc_1d_char (strlen (in + 1));

  for (i = 0; i < pos_min; i++)
    in_bef[i] = in[i];
  in_bef[i] = '\0';

  for (i = pos_cls + 1; i < (int) strlen (in); i++)
    in_aft[i - (pos_cls + 1)] = in[i];
  in_aft[i  - (pos_cls + 1)] = '\0';

  (*pout) = ut_alloc_1d_char (strlen (in) + 24);
  sprintf (*pout, "%s((%s+%s)/2-sqrt(((%s-%s)/2)^2))%s", in_bef, arg1, arg2, arg1, arg2,in_aft);

  ut_free_1d_char (arg1);
  ut_free_1d_char (arg2);
  ut_free_1d_char (in_bef);
  ut_free_1d_char (in_aft);

  return 1;
}
