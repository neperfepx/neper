/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_point_.h"

void
nev_show_points (char **argv, int *pi, struct POINT Point,
		 struct PRINT *pPrint)
{
  // int i, j, varqty;
  int status;
  // char** vars = NULL;
  // double* vals = NULL;

  (*pi)++;
  (*pPrint).showpoint =
    ut_realloc_1d_int ((*pPrint).showpoint, Point.PointQty + 1);
  ut_array_1d_int_zero ((*pPrint).showpoint, Point.PointQty + 1);

  status =
    nev_show_genexpr (argv[(*pi)], (*pPrint).showpoint, Point.PointQty);

  if (status != 0)
    abort ();
  /*
     {
     neut_point_var_list ("point", &vars, &varqty);
     vals = ut_alloc_1d (varqty);

     for (i = 1; i <= Point.PointQty; i++)
     {
     for (j = 0; j < varqty; j++)
     if (strstr (argv[(*pi)], vars[j]))
     neut_point_var_val (Point, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
     Part, Tess,
     (*pPrint).showelt0d, (*pPrint).showelt1d,
     (*pPrint).showelt2d, (*pPrint).showelt3d,
     NULL, 0, "point", i, vars[j], &(vals[j]), NULL);

     status = ut_math_eval_int (argv[(*pi)], varqty, vars, vals,
     &((*pPrint).showpoint[i]));
     if (status != 0)
     ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
     argv[(*pi)]);
     }
     }
   */

  (*pPrint).showpoint[0] = ut_array_1d_int_sum ((*pPrint).showpoint + 1,
						Point.PointQty);

  // ut_free_2d_char (vars, varqty);
  // ut_free_1d (vals);

  return;
}
