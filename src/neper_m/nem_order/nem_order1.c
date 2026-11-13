/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
           struct MESH *Mesh)
{
  int i, *domesh = ut_alloc_1d_int (6);
  int dim = neut_mesh_array_dim (Mesh);

  nem_order_pre (In, Mesh, domesh);

  nem_order_dim (In, pNodes, NULL, Mesh + dim);

  for (i = dim - 1; i > 0; i--)
    if (domesh[i])
      nem_order_dim (In, pNodes, Mesh + i + 1, Mesh + i);

  for (i = 4; i <= 5; i++)
    if (domesh[i])
      nem_order_dim (In, pNodes, Mesh + dim, Mesh + i);

  nem_order_curved (Tess, pNodes, Mesh);

  if (Tess.Type && !strcmp (Tess.Type, "periodic"))
    nem_order_periodic (Tess, pNodes, Mesh);

  ut_free_1d_int (&domesh);

  return;
}

void
nem_order_str (struct IN_M In, struct MESHPARA MeshPara, struct NODES *pNodes,
               struct MESH *Mesh)
{
  int i, dim = neut_mesh_array_dim (Mesh);
  int NodeQty_prev = (*pNodes).NodeQty;

  nem_order_dim (In, pNodes, NULL, Mesh + dim);

  for (i = dim - 1; i > 0; i--)
    if (Mesh[i].EltQty > 0)
      nem_order_dim (In, pNodes, Mesh + i + 1, Mesh + i);

  if (Mesh[5].EltQty > 0)
  {
    if (!Mesh[5].NodeElts)
      neut_mesh_init_nodeelts (Mesh + 5, (*pNodes).NodeQty);
    nem_order_dim (In, pNodes, Mesh + dim, Mesh + 5);
  }

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

void
nem_order_1 (struct NODES *pNodes, struct MESH *Mesh, struct NSET *NSet)
{
  int i;

  for (i = 0; i <= 5; i++)
    if (Mesh[i].EltQty > 0 && Mesh[i].EltOrder == 2)
      nem_order_1_elts (Mesh + i);

  nem_order_1_nodes (pNodes, Mesh, NSet);

  return;
}
