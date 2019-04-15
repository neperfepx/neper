/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"ut.h"

#ifdef HAVE_MUPARSER
#include <muParser.h>

int
ut_math_eval (char* expr, int var_qty, char** vars, double* vals,
              double* pres)
{
  using namespace mu;
  int i;

  if (strstr (expr, "[") || strstr (expr, "]") || strstr (expr, "{")
   || strstr (expr, "}"))
    abort ();

  try
  {
    Parser p;
    for (i = 0; i < var_qty; i++)
      p.DefineVar(vars[i], vals + i);
    // p.DefineFun("MySqr", MySqr);
    p.SetExpr(expr);

    (*pres) = p.Eval();
  }

  catch (Parser::exception_type &e)
  {
    return -1;
  }

  return 0;
}

int
ut_math_eval_int (char* expr, int var_qty, char** vars, double* vals,
		  int* pres)
{
  int status;
  double res;

  status = ut_math_eval (expr, var_qty, vars, vals, &res);

  if (!status)
    (*pres) = ut_num_d2ri (res);

  return status;
}

int
ut_math_vars (char* expr, char*** pvars, int *pvarqty)
{
  int i, stringqty, fctqty;
  char *tmp = NULL;
  char **strings = NULL;
  char **fcts = NULL;

  ut_math_functions (&fcts, &fctqty);

  ut_string_string (expr, &tmp);

  for (i = 0; i < (int) strlen (tmp); i++)
    if (! isalpha (tmp[i]))
      tmp[i] = ' ';

  ut_string_substrings (tmp, &strings, &stringqty);

  (*pvarqty) = 0;
  (*pvars) = NULL;

  for (i = 0; i < stringqty; i++)
    if (! ut_math_string_isfunction (strings[i]))
    {
      (*pvars) = ut_realloc_2d_char_addline (*pvars, ++(*pvarqty), strlen (strings[i]) + 1);
      strcpy ((*pvars)[*pvarqty - 1], strings[i]);
    }

  ut_free_1d_char (tmp);
  ut_free_2d_char (strings, stringqty);
  ut_free_2d_char (fcts, fctqty);

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

  ut_free_2d_char (fcts, fctqty);

  return status;
}
#endif // HAVE_MUPARSER
