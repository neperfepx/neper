/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_sort_.h"

void
nem_sort_nodes (struct IN_M In, struct TESS *pTess, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, max, dim = neut_mesh_array_dim (Mesh);
  double *val = ut_alloc_1d ((*pNodes).NodeQty + 1);
  int *id = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  int *invid = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  neut_mesh_entity_expr_val (*pNodes, Mesh, pTess, NULL, NULL, NULL, NULL, "node",
                             In.sortnode, val, NULL);

  ut_array_1d_sort_index (val + 1, (*pNodes).NodeQty, id + 1);
  ut_array_1d_int_addval (id + 1, (*pNodes).NodeQty, 1, id + 1);
  ut_array_1d_int_inv (id + 1, (*pNodes).NodeQty, &invid, &max);
  ut_array_1d_int_addval (invid + 1, (*pNodes).NodeQty, 1, invid + 1);

  neut_nodes_renumber_switch (pNodes, invid);

  for (i = 0; i <= dim; i++)
  {
    neut_mesh_nodes_switch (Mesh + i, invid);
    neut_mesh_init_nodeelts (Mesh + i, (*pNodes).NodeQty);
  }

  ut_free_1d (&val);
  ut_free_1d_int (&id);
  ut_free_1d_int (&invid);

  return;
}

void
nem_sort_elts (struct IN_M In, struct TESS *pTess, struct NODES *pNodes, struct MESH *Mesh)
{
  int max, dim = neut_mesh_array_dim (Mesh);
  int qty = Mesh[dim].EltQty;
  double *val = ut_alloc_1d (qty + 1);
  int *id = ut_alloc_1d_int (qty + 1);
  int *invid = ut_alloc_1d_int (qty + 1);
  char *entity = ut_alloc_1d_char (10);
  sprintf (entity, "elt%dd", dim);

  neut_mesh_entity_expr_val (*pNodes, Mesh, pTess, NULL, NULL, NULL, NULL, entity,
                             In.sortelt, val, NULL);

  ut_array_1d_sort_index (val + 1, qty, id + 1);
  ut_array_1d_int_addval (id + 1, qty, 1, id + 1);
  ut_array_1d_int_inv (id + 1, qty, &invid, &max);
  ut_array_1d_int_addval (invid + 1, qty, 1, invid + 1);

  neut_mesh_elts_switch (Mesh + dim, invid);

  ut_free_1d (&val);
  ut_free_1d_int (&id);
  ut_free_1d_int (&invid);
  ut_free_1d_char (&entity);

  return;
}
