/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transform_.h"

void
nem_transform_smooth (char *smooth, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, dim, iterqty;
  double factor;

  if (sscanf (smooth, "smooth(%lf,%d)", &factor, &iterqty) != 2)
    ut_print_message (2, 4, "Failed to parse expression `%s'.\n", smooth);

  for (i = 1; i <= iterqty; i++)
    for (dim = 1; dim < neut_mesh_array_dim (Mesh); dim++)
    {
      ut_print_message (0, 4, "iter %d dimension %d\n", i, dim);
      nem_smoothing_laplacian (pNodes, Mesh, dim, factor, iterqty);
    }

  return;
}
