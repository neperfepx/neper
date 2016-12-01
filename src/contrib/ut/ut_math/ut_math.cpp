/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

#ifdef HAVE_MUPARSER
#include "../muparser/include/muParser.h"
#endif

int
ut_math_eval (char* expr, int var_qty, char** vars, double* vals,
              double* pres)
{
#ifdef HAVE_MUPARSER
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

#else
  (void) expr;
  (void) var_qty;
  (void) vars;
  (void) vals;
  (void) pres;
  printf ("\n");
  ut_print_message (2, 0, "Cannot process expression (muparser not available).\n");
  abort ();
#endif
  
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
