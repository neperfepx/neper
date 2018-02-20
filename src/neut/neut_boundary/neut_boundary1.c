/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_boundary_.h"

void
neut_boundary_bound_nodes (struct MESH Mesh, struct BOUNDARY Bound, int id, int side,
                           int **pnodes, int *pnodeqty)
{
  if (Mesh.Dimension == 2)
    neut_boundary_bound_nodes_2d (Mesh, Bound, id, side, pnodes, pnodeqty);
  else if (Mesh.Dimension == 3)
    neut_boundary_bound_nodes_3d (Mesh, Bound, id, side, pnodes, pnodeqty);

  return;
}
