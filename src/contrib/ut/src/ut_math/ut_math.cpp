/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>
#include"ut.h"

#ifdef HAVE_MUPARSER
#include "muParser.h"
#endif

void ut_math_functions (char ***pfcts, int *pfctqty);
int ut_math_string_isfunction (char *string);

// replacing the variables of an expression by names muparser accepts
// (utvar<i>), matching whole names only (not followed or preceded by a
// letter, digit or underscore), from the longest to the shortest, and not
// followed by an opening parenthesis (functions)
static void
ut_math_expr_vars (char *inexpr, int varqty, char **vars, char **pexpr,
                   char ***pnewvars)
{
  int i, j, k, id, pos, length, match;
  int *lengths = ut_alloc_1d_int (varqty);
  int *ids = ut_alloc_1d_int (varqty);
  char *expr = NULL;

  for (i = 0; i < varqty; i++)
    lengths[i] = strlen (vars[i]);

  ut_array_1d_int_sort_des_index (lengths, varqty, ids);

  (*pnewvars) = ut_alloc_1d_pchar (varqty);
  for (i = 0; i < varqty; i++)
    ut_string_sprintf ((*pnewvars) + i, "utvar%d", i);

  // each replacement is at most 8 + 10 characters long
  length = strlen (inexpr);
  expr = ut_alloc_1d_char (19 * length + 1);

  pos = 0;
  for (i = 0; i < length; i++)
  {
    match = -1;

    if (i == 0 || !(isalnum (inexpr[i - 1]) || inexpr[i - 1] == '_'))
      for (j = 0; j < varqty && match == -1; j++)
      {
        id = ids[j];
        if (lengths[id] == 0 || strncmp (inexpr + i, vars[id], lengths[id]))
          continue;

        k = i + lengths[id];
        if (isalnum (inexpr[k]) || inexpr[k] == '_')
          continue;

        while (inexpr[k] == ' ')
          k++;
        if (inexpr[k] == '(')
          continue;

        match = id;
      }

    if (match != -1)
    {
      pos += sprintf (expr + pos, "%s", (*pnewvars)[match]);
      i += lengths[match] - 1;
    }
    else
      expr[pos++] = inexpr[i];
  }
  expr[pos] = '\0';

  (*pexpr) = expr;

  ut_free_1d_int (&lengths);
  ut_free_1d_int (&ids);

  return;
}

int
ut_math_eval (char *inexpr, int varqty, char **vars, double *vals, double *pres)
{
#ifdef HAVE_MUPARSER
  using namespace mu;
  int i, status;
  char **newvars = NULL;
  char *expr = NULL;

  if (strstr (inexpr, "[") || strstr (inexpr, "]") || strstr (inexpr, "{")
      || strstr (inexpr, "}"))
    abort ();

  ut_math_expr_vars (inexpr, varqty, vars, &expr, &newvars);

  status = 0;

  try
  {
    Parser p;
    for (i = 0; i < varqty; i++)
      p.DefineVar (newvars[i], vals + i);
    p.SetExpr (expr);

    *pres = p.Eval ();
  }

  catch (Parser::exception_type & e)
  {
    status = -1;
  }

  ut_free_2d_char (&newvars, varqty);
  ut_free_1d_char (&expr);

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
  int i, status;
  char **newvars = NULL;
  char *expr = NULL;

  if (strstr (inexpr, "[") || strstr (inexpr, "]") || strstr (inexpr, "{")
      || strstr (inexpr, "}"))
    abort ();

  ut_math_expr_vars (inexpr, varqty, vars, &expr, &newvars);

  status = 0;

  try
  {
    Parser p;
    for (i = 0; i < varqty; i++)
      p.DefineVar (newvars[i], vals[i]);
    p.SetExpr (expr);

    p.Eval (res, evalqty);
  }

  catch (Parser::exception_type & e)
  {
    status = -1;
  }

  ut_free_2d_char (&newvars, varqty);
  ut_free_1d_char (&expr);

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

      // if number, erasing it, including its exponent (as in 5e-1)
      else if (isdigit (tmp[i]) || tmp[i] == '.')
      {
        j = i;
        while (isdigit (tmp[j + 1]) || tmp[j + 1] == '.')
          j++;

        if ((tmp[j + 1] == 'e' || tmp[j + 1] == 'E')
            && (isdigit (tmp[j + 2])
                || ((tmp[j + 2] == '+' || tmp[j + 2] == '-')
                    && isdigit (tmp[j + 3]))))
        {
          j += isdigit (tmp[j + 2]) ? 2 : 3;
          while (isdigit (tmp[j + 1]))
            j++;
        }

        for (; i < j; i++)
          tmp[i] = ' ';
        tmp[i] = ' ';
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

  // the trailing spaces are messing up ut_string_substrings
  while (strlen (tmp) > 0 && tmp[strlen (tmp) - 1] == ' ')
    tmp[strlen (tmp) - 1] = '\0';

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
