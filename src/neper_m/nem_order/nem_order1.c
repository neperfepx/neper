/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
           struct MESH *Mesh)
{
  int i, NodeQty_before = (*pNodes).NodeQty, *domesh = ut_alloc_1d_int (5);
  int dim = neut_mesh_array_dim (Mesh);

  nem_order_pre (In, Mesh, domesh);

  nem_order_dim (In, pNodes, NULL, Mesh + dim);

  for (i = dim - 1; i > 0; i--)
    if (domesh[i])
      nem_order_dim (In, pNodes, Mesh + i + 1, Mesh + i);

  if (domesh[4])
    nem_order_dim (In, pNodes, Mesh + dim, Mesh + 4);

  if (!strcmp (Tess.Type, "periodic"))
    nem_order_periodic (Tess, pNodes, Mesh);

  nem_order_post (In, NodeQty_before, pNodes);

  ut_free_1d_int (&domesh);

  return;
}
