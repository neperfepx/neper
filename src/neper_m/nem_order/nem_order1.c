/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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

  if (Tess.Type && !strcmp (Tess.Type, "periodic"))
    nem_order_periodic (Tess, pNodes, Mesh);

  nem_order_post (In, NodeQty_before, pNodes);

  ut_free_1d_int (&domesh);

  return;
}

void
nem_order_str (struct IN_M In, struct MESHPARA MeshPara, struct NODES *pNodes,
               struct MESH *Mesh)
{
  int dim = neut_mesh_array_dim (Mesh);
  int NodeQty_prev = (*pNodes).NodeQty;

  nem_order_dim (In, pNodes, NULL, Mesh + dim);

  neut_mesh_init_nodeelts (Mesh + dim, (*pNodes).NodeQty);

  if (ut_array_1d_int_sum ((*pNodes).Periodic, 3))
  {
    if (dim == 2)
      nem_order_str_per_2d (NodeQty_prev, MeshPara, pNodes, Mesh);
    else
      nem_order_str_per_3d (NodeQty_prev, MeshPara, pNodes, Mesh);
  }

  return;
}
