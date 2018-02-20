/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_camera_.h"

void
nev_camera_v (int dim, double *v)
{
  if (dim >= 0 && dim <= 2)
  {
    v[0] = 0;
    v[1] = 0;
    v[2] = 8;
  }

  else if (dim == 3)
  {
    v[0] = 3.462;
    v[1] = -5.770;
    v[2] = 4.327;
  }

  else
    ut_error_reportbug ();

  return;
}

void
nev_camera_expr_coo (double *X, double *v, char *expr, double *coo)
{
  char **expr2 = NULL;
  char **def = ut_alloc_2d_char (3, 100);
  int i, status, var_qty, qty;
  ut_string_separate (expr, NEUT_SEP_DEP, &expr2, &qty);
  if (qty != 3)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
		      expr);

  var_qty = 2;
  char **vars = ut_alloc_2d_char (var_qty, 15);
  double *vals = ut_alloc_1d (var_qty);

  for (i = 0; i < 3; i++)
  {
    sprintf (vars[0], "v%c", 'x' + i);
    sprintf (vars[1], "%c", 'x' + i);

    vals[0] = v[i];
    vals[1] = X[i];

    sprintf (def[0], "%s", vars[0]);
    sprintf (def[1], "%s", vars[1]);
    sprintf (def[2], "%s+%s", vars[1], vars[0]);

    if (!strcmp (expr2[i], def[0]))
      coo[i] = vals[0];
    else if (!strcmp (expr2[i], def[1]))
      coo[i] = vals[1];
    else if (!strcmp (expr2[i], def[2]))
      coo[i] = vals[0] + vals[1];
    else
    {
      status = ut_math_eval (expr2[i], var_qty, vars, vals, &(coo[i]));
      if (status == -1)
	abort ();
    }
  }

  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);
  ut_free_2d_char (def, 3);

  return;
}
