/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transform_.h"

void
nem_transform_smooth (char *smooth, struct TESS Tess, struct NODES *pNodes,
                      struct MESH *Mesh)
{
  int i, dim, iterqty;
  double factor;
  char *type = ut_alloc_1d_char (strlen (smooth));
  char *message = ut_alloc_1d_char (1000);
  char *prevmessage = ut_alloc_1d_char (1000);

  if (sscanf (smooth, "smooth(%lf,%d,%s", &factor, &iterqty, type) != 3)
    ut_print_message (2, 4, "Failed to parse expression `%s'.\n", smooth);
  type[strlen (type) - 1] = '\0';

  ut_print_message (0, 4, "");
  for (i = 1; i <= iterqty; i++)
  {
    sprintf (message, "Iteration %4d/%d", i, iterqty);
    ut_print_progress (stdout, 4, INT_MAX, message, prevmessage);

    for (dim = 1; dim < neut_mesh_array_dim (Mesh); dim++)
      nem_smoothing_laplacian (Tess, pNodes, Mesh, dim, factor, 1, type);
  }

  ut_free_1d_char (type);
  ut_free_1d_char (message);
  ut_free_1d_char (prevmessage);

  return;
}
