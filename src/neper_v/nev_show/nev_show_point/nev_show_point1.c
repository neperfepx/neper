/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_point_.h"

void
nev_show_points (char **argv, int *pi, struct POINT Point, int **pshow)
{
  int i, j, varqty, status;
  char** vars = NULL;
  double *vals = NULL;
  double *tmp = NULL;
  int tmpqty;
  struct TESS Tess;
  struct NODES Nodes;
  struct MESH Mesh;

  neut_tess_set_zero (&Tess);
  neut_nodes_set_zero (&Nodes);
  neut_mesh_set_zero (&Mesh);

  (*pi)++;

  *pshow = ut_realloc_1d_int (*pshow, Point.Qty + 1);
  ut_array_1d_int_zero (*pshow, Point.Qty + 1);

  status =
    nev_show_genexpr (argv[(*pi)], *pshow, Point.Qty);

  if (status)
  {
    ut_math_vars (argv[(*pi)], &vars, &varqty);

    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Point.Qty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        neut_point_var_val (Point, i, Tess, Nodes, Mesh,
                            vars[j], &tmp, &tmpqty, NULL);
        vals[j] = tmp[0];
      }

      status = ut_math_eval_int (argv[(*pi)], varqty, vars, vals, (*pshow) + i);

      if (status != 0)
        ut_print_exprbug (argv[(*pi)]);
    }
  }

  (*pshow)[0] =
    ut_array_1d_int_sum (*pshow + 1, Point.Qty);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&tmp);
  ut_free_1d (&vals);
  neut_tess_free (&Tess);
  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh);

  return;
}
