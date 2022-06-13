/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"ut.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif /* HAVE_OPENMP */

#ifdef HAVE_MUPARSER
#include "muParser.h"
#endif

void ut_math_functions (char ***pfcts, int *pfctqty);
int ut_math_string_isfunction (char *string);

int
ut_math_eval (char *inexpr, int varqty, char **vars, double *vals, double *pres)
{
#ifdef HAVE_OPENMP
  if (omp_get_thread_num() != 0)
  {
    printf ("ut_math_eval should not be multithreaded.\n");
    abort ();
  }
#endif /* HAVE_OPENMP */

#ifdef HAVE_MUPARSER
  using namespace mu;
  int i, j, status;
  char **newvars = ut_alloc_1d_pchar (varqty);
  char *expr = ut_alloc_1d_char (strlen (inexpr) * 10);
  int *length = ut_alloc_1d_int (varqty);
  int id, *ids = ut_alloc_1d_int (varqty);

  expr = strcpy (expr, inexpr);

  if (strstr (expr, "[") || strstr (expr, "]") || strstr (expr, "{")
      || strstr (expr, "}"))
    abort ();

  // will be processing the variables from the longest to the shortest,
  // in case a short variable is contained in a larger variable, which
  // would cause erroneous substitution in expr
  for (i = 0; i < varqty; i++)
    length[i] = strlen (vars[i]);

  ut_array_1d_int_sort_des_index (length, varqty, ids);

  for (i = 0; i < varqty; i++)
  {
    id = ids[i];
    // testing if it's all letters
    status = 0;
    for (j = 0; j < varqty; j++)
      if (!isalpha (vars[id][j]))
      {
        status = -1;
        break;
      }

    // if not, we need to replace it, as muparser won't take it
    if (!status)
    {
      ut_string_random (8, id, newvars + id);
      ut_string_fnrs (expr, vars[id], newvars[id], INT_MAX);
    }
    else
      ut_string_string (vars[id], newvars + id);
  }

  status = 0;

  try
  {
    Parser p;
    for (i = 0; i < varqty; i++)
      p.DefineVar (newvars[i], vals + i);
    // p.DefineFun("MySqr", MySqr);
    p.SetExpr (expr);

    *pres = p.Eval ();
  }

  catch (Parser::exception_type & e)
  {
    status = -1;
  }

  ut_free_2d_char (&newvars, varqty);
  ut_free_1d_char (&expr);
  ut_free_1d_int (&length);
  ut_free_1d_int (&ids);

  return status;

#else
  (void) inexpr;
  (void) varqty;
  (void) vars;
  (void) vals;
  (void) pres;
  ut_print_message (2, 0, "compiled without muparser\n");
  abort ();

#endif /* HAVE_MUPARSER */
}

int
ut_math_evals (char *inexpr, int varqty, char **vars, double **vals, int evalqty, double *res)
{
#ifdef HAVE_MUPARSER
  using namespace mu;
  int i, j, status;
  char **newvars = ut_alloc_1d_pchar (varqty);
  char *expr = ut_alloc_1d_char (strlen (inexpr) * 10);
  int *length = ut_alloc_1d_int (varqty);
  int id, *ids = ut_alloc_1d_int (varqty);

  expr = strcpy (expr, inexpr);

  if (strstr (expr, "[") || strstr (expr, "]") || strstr (expr, "{")
      || strstr (expr, "}"))
    abort ();

  // will be processing the variables from the longest to the shortest,
  // in case a short variable is contained in a larger variable, which
  // would cause erroneous substitution in expr
  for (i = 0; i < varqty; i++)
    length[i] = strlen (vars[i]);

  ut_array_1d_int_sort_des_index (length, varqty, ids);

  for (i = 0; i < varqty; i++)
  {
    id = ids[i];
    // testing if it's all letters
    status = 0;
    for (j = 0; j < varqty; j++)
      if (!isalpha (vars[id][j]))
      {
        status = -1;
        break;
      }

    // if not, we need to replace it, as muparser won't take it
    if (!status)
    {
      ut_string_random (8, id, newvars + id);
      ut_string_fnrs (expr, vars[id], newvars[id], INT_MAX);
    }
    else
      ut_string_string (vars[id], newvars + id);
  }

  status = 0;

  try
  {
    Parser p;
    for (i = 0; i < varqty; i++)
      p.DefineVar (newvars[i], vals[i]);
    // p.DefineFun("MySqr", MySqr);
    p.SetExpr (expr);

    p.Eval (res, evalqty);
  }

  catch (Parser::exception_type & e)
  {
    status = -1;
  }

  ut_free_2d_char (&newvars, varqty);
  ut_free_1d_char (&expr);
  ut_free_1d_int (&length);
  ut_free_1d_int (&ids);

  return status;

#else
  (void) inexpr;
  (void) varqty;
  (void) vars;
  (void) vals;
  (void) evalqty;
  (void) res;
  ut_print_message (2, 0, "compiled without muparser\n");
  abort ();

#endif /* HAVE_MUPARSER */
}

int
ut_math_eval_int (char *expr, int varqty, char **vars, double *vals,
                  int *pres)
{
  int status;
  double res;

  status = ut_math_eval (expr, varqty, vars, vals, &res);

  if (!status)
    (*pres) = ut_num_d2ri (res);

  return status;
}

int
ut_math_vars (char *expr, char ***pvars, int *pvarqty)
{
  int i, j, stringqty, fctqty, var, found, bracket, length;
  char *tmp = NULL;
  char **strings = NULL;
  char **fcts = NULL;

  ut_math_functions (&fcts, &fctqty);

  ut_string_string (expr, &tmp);

  ut_string_fnrs (tmp, "euler-", "euler_", INT_MAX);
  ut_string_fnrs (tmp, ":active", "_active", INT_MAX);
  ut_string_fnrs (tmp, ":passive", "_passive", INT_MAX);

  var = 0;
  bracket = 0;
  length = 0;
  for (i = 0; i < (int) strlen (tmp); i++)
  {
    // if not recording a variable
    if (var == 0)
    {
      // if character, start recording
      if (isalpha (tmp[i]) || tmp[i] == '_')
      {
        var = 1;
        length = 1;
      }

      // otherwise, erasing
      else
        tmp[i] = ' ';
    }

    // if in the process of recording a variable
    else
    {
      // is a character - continue recording
      if (isalpha (tmp[i]) || tmp[i] == '_')
        length++;

      // is a digit - continue recording
      else if (isdigit (tmp[i]))
        length++;

      // is an opening bracket - mark as such and continue recording
      else if (tmp[i] == '(')
      {
        char *tmp2 = ut_alloc_1d_char (length + 1);
        tmp2 = strncpy (tmp2, tmp + i - length, length);
        if (ut_math_string_isfunction (tmp2))
        {
          // erasing function and opening bracket
          for (j = i - length; j <= i; j++)
            tmp[j] = ' ';
          var = 0;
          tmp[i] = ' ';
        }
        else
          bracket++;
        ut_free_1d_char (&tmp2);
      }

      // is a closing bracket - testing
      else if (tmp[i] == ')')
      {
        // closes a bracket - recording
        if (bracket > 0)
          bracket--;
        // does not close a bracket - erasing
        else
          tmp[i] = ' ';
      }

      // is in a bracket - continue
      else if (bracket > 0) {}

      // erasing
      else
      {
        tmp[i] = ' ';
        var = 0;
      }
    }
  }

  ut_string_substrings (tmp, &strings, &stringqty);

  (*pvarqty) = 0;
  (*pvars) = NULL;

  for (i = 0; i < stringqty; i++)
    if (!ut_math_string_isfunction (strings[i]))
    {
      found = 0;
      for (j = 0; j < *pvarqty; j++)
        if (!strcmp ((*pvars)[j], strings[i]))
        {
          found = 1;
          break;
        }
      if (found)
        continue;

      (*pvars) =
        ut_realloc_2d_char_addline (*pvars, ++(*pvarqty),
                                    strlen (strings[i]) + 1);
      strcpy ((*pvars)[*pvarqty - 1], strings[i]);
    }

  for (i = 0; i < *pvarqty; i++)
  {
    ut_string_fnrs ((*pvars)[i], "euler_bunge", "euler-bunge", 1);
    ut_string_fnrs ((*pvars)[i], "_active", ":active", 1);
    ut_string_fnrs ((*pvars)[i], "_passive", ":passive", 1);
  }

  ut_free_1d_char (&tmp);
  ut_free_2d_char (&strings, stringqty);
  ut_free_2d_char (&fcts, fctqty);

  return 0;
}

void
ut_math_functions (char ***pfcts, int *pfctqty)
{
  int id = 0;
  (*pfctqty) = 25;
  (*pfcts) = ut_alloc_1d_pchar (*pfctqty);

  ut_string_string ("sin", (*pfcts) + id++);
  ut_string_string ("cos", (*pfcts) + id++);
  ut_string_string ("tan", (*pfcts) + id++);
  ut_string_string ("asin", (*pfcts) + id++);
  ut_string_string ("acos", (*pfcts) + id++);
  ut_string_string ("atan", (*pfcts) + id++);
  ut_string_string ("sinh", (*pfcts) + id++);
  ut_string_string ("cosh", (*pfcts) + id++);
  ut_string_string ("tanh", (*pfcts) + id++);
  ut_string_string ("asinh", (*pfcts) + id++);
  ut_string_string ("acosh", (*pfcts) + id++);
  ut_string_string ("atanh", (*pfcts) + id++);
  ut_string_string ("log2", (*pfcts) + id++);
  ut_string_string ("log10", (*pfcts) + id++);
  ut_string_string ("log", (*pfcts) + id++);
  ut_string_string ("ln", (*pfcts) + id++);
  ut_string_string ("exp", (*pfcts) + id++);
  ut_string_string ("sqrt", (*pfcts) + id++);
  ut_string_string ("sign", (*pfcts) + id++);
  ut_string_string ("rint", (*pfcts) + id++);
  ut_string_string ("abs", (*pfcts) + id++);
  ut_string_string ("min", (*pfcts) + id++);
  ut_string_string ("max", (*pfcts) + id++);
  ut_string_string ("sum", (*pfcts) + id++);
  ut_string_string ("avg", (*pfcts) + id++);

  return;
}

int
ut_math_string_isfunction (char *string)
{
  int i, fctqty, status;
  char **fcts = NULL;

  ut_math_functions (&fcts, &fctqty);

  status = 0;
  for (i = 0; i < fctqty; i++)
    if (!strcmp (string, fcts[i]))
    {
      status = 1;
      break;
    }

  ut_free_2d_char (&fcts, fctqty);

  return status;
}

int
ut_math_eval_exprislogical (char *string)
{
  if (strstr (string, "&&")
   || strstr (string, "||")
   || strstr (string, "<")
   || strstr (string, ">")
   || strstr (string, "=")
   || strstr (string, "?"))
    return 1;
  else
    return 0;
}
