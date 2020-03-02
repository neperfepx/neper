/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order_pre (struct IN_M In, struct MESH *Mesh, int *domesh)
{
  int i, dim = neut_mesh_array_dim (Mesh);

  for (i = 0; i <= dim; i++)
    if (Mesh[i].EltQty > 0)
      domesh[i] = 1;

  if (!strcmp (In.interface, "cohesive"))
    domesh[4] = 1;

  if (dim == 2 && !strcmp (Mesh[2].EltType, "tri") && Mesh[4].EltQty > 0
      && !strcmp (Mesh[4].EltType, "quad"))
    ut_string_string ("quad6", &Mesh[4].EltType);

  return;
}

void
nem_order_dim (struct IN_M In, struct NODES *pNodes, struct MESH *pMeshU,
               struct MESH *pMesh)
{
  int i, j, node_o2, nodeqty_o1, nodeqty_tot, nodes[2], **fir = NULL;
  double coo[3];

  (*pMesh).EltOrder = 2;
  if ((*pMesh).Dimension == 2 && !strcmp (In.elttype, "quad9"))
    ut_string_string (In.elttype, &((*pMesh).EltType)); // needed by quad9

  neut_elt_orderarrays ((*pMesh).EltType, (*pMesh).Dimension, &fir,
                        &nodeqty_tot, NULL, &nodeqty_o1);

  ut_realloc_2d_int_size2 ((*pMesh).EltNodes + 1, (*pMesh).EltQty, nodeqty_o1,
                           nodeqty_tot);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = nodeqty_o1; j < nodeqty_tot; j++)
      if (!(*pMesh).EltNodes[i][j])
      {
        nodes[0] = (*pMesh).EltNodes[i][fir[j][0]];
        nodes[1] = (*pMesh).EltNodes[i][fir[j][1]];

        if (pMeshU)
          neut_mesh_order1nodes_order2node (*pMeshU, nodes[0], nodes[1],
                                            &node_o2);

        if (pMeshU && node_o2 != -1)
          nem_order_dim_record (pMesh, nodes, node_o2);
        else
        {
          neut_nodes_bary (*pNodes, nodes, 2, coo, NULL);
          node_o2 = neut_nodes_addnode (pNodes, coo, -1);
          nem_order_dim_record (pMesh, nodes, node_o2);
        }
      }

  neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

  ut_free_2d_int (&fir, nodeqty_tot);

  return;
}

void
nem_order_periodic (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  nem_order_periodic_edges (Tess, pNodes, Mesh);

  if (Tess.Dim == 3)
    nem_order_periodic_faces (Tess, pNodes, Mesh);

  return;
}

void
nem_order_post (struct IN_M In, int NodeQty_before, struct NODES *pNodes)
{
  int i;

  if (strcmp (In.interface, "continuous"))
  {
    (*pNodes).DupNodeSlaveNb =
      ut_realloc_1d_pint ((*pNodes).DupNodeSlaveNb, (*pNodes).NodeQty + 1);
    for (i = NodeQty_before; i <= (*pNodes).NodeQty; i++)
      (*pNodes).DupNodeSlaveNb[i] = NULL;
  }

  return;
}
