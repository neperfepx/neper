/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order_dim_record (struct MESH *pMesh, int *nodes, int node_o2)
{
  int i, j, **sec = NULL, nodeqty_o1;
  int *pos = ut_alloc_1d_int (2);
  int comelt, *comelts = NULL, comeltqty;

  neut_elt_orderarrays ((*pMesh).EltType, (*pMesh).Dimension, NULL, NULL,
                        &sec, &nodeqty_o1);

  neut_mesh_nodes_comelts (*pMesh, nodes, 2, &comelts, &comeltqty);

  for (i = 0; i < comeltqty; i++)
  {
    comelt = comelts[i];

    for (j = 0; j < 2; j++)
      pos[j] =
        ut_array_1d_int_eltpos ((*pMesh).EltNodes[comelt], nodeqty_o1,
                                nodes[j]);
    (*pMesh).EltNodes[comelt][sec[pos[0]][pos[1]]] = node_o2;
  }

  ut_free_1d_int (&pos);
  ut_free_1d_int (&comelts);

  return;
}

void
nem_order_periodic_edges (struct TESS Tess, struct NODES *pNodes,
                          struct MESH *Mesh)
{
  int i, j, k, l, nodeqty_tot, nodeqty_o1, nodes[2], nodemasters[2], node,
    nodemaster;
  int **fir = NULL;
  int *node_slave = ut_alloc_1d_int ((*pNodes).NodeQty);
  int *shift = ut_alloc_1d_int (3);

  neut_elt_orderarrays (Mesh[1].EltType, Mesh[1].Dimension, &fir,
                        &nodeqty_tot, NULL, &nodeqty_o1);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (neut_tess_edge_isperslave (Tess, i))
      for (j = 1; j <= Mesh[1].Elsets[i][0]; j++)
        for (k = nodeqty_o1; k < nodeqty_tot; k++)
        {
          ut_array_1d_int_memcpy (Tess.PerEdgeShift[i], 3, shift);
          ut_array_1d_int_scale (shift, 3, -1);

          for (l = 0; l < 2; l++)
          {
            nodes[l] = Mesh[1].EltNodes[Mesh[1].Elsets[i][j]][fir[k][l]];
            neut_nodes_node_shift_pernode (*pNodes, nodes[l], shift,
                                           nodemasters + l);
          }

          neut_mesh_order1nodes_order2node (Mesh[1], nodes[0], nodes[1],
                                            &node);
          neut_mesh_order1nodes_order2node (Mesh[1], nodemasters[0],
                                            nodemasters[1], &nodemaster);

          if (!node_slave[node])
          {
            neut_nodes_markasslave (pNodes, node, nodemaster,
                                    Tess.PerEdgeShift[i]);
            node_slave[node] = 1;
          }
        }

  ut_free_2d_int (&fir, nodeqty_tot);
  ut_free_1d_int (&node_slave);
  ut_free_1d_int (&shift);

  return;
}

void
nem_order_periodic_faces (struct TESS Tess, struct NODES *pNodes,
                          struct MESH *Mesh)
{
  int i, j, k, l, nodeqty_tot, nodeqty_o1, nodes[2], nodemasters[2], node,
    nodemaster;
  int **fir = NULL;
  int *node_slave = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  int *shift = ut_alloc_1d_int (3);

  neut_elt_orderarrays (Mesh[2].EltType, Mesh[2].Dimension, &fir,
                        &nodeqty_tot, NULL, &nodeqty_o1);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (neut_tess_face_isperslave (Tess, i))
      for (j = 1; j <= Mesh[2].Elsets[i][0]; j++)
        for (k = nodeqty_o1; k < nodeqty_tot; k++)
        {
          ut_array_1d_int_memcpy (Tess.PerFaceShift[i], 3, shift);
          ut_array_1d_int_scale (shift, 3, -1);

          for (l = 0; l < 2; l++)
          {
            nodes[l] = Mesh[2].EltNodes[Mesh[2].Elsets[i][j]][fir[k][l]];
            neut_nodes_node_shift_pernode (*pNodes, nodes[l], shift,
                                           nodemasters + l);
          }

          neut_mesh_order1nodes_order2node (Mesh[2], nodes[0], nodes[1],
                                            &node);
          neut_mesh_order1nodes_order2node (Mesh[2], nodemasters[0],
                                            nodemasters[1], &nodemaster);

          if (neut_mesh_node_dim_min
              (Mesh[0], Mesh[1], Mesh[2], Mesh[3], node) > 1
              && !node_slave[node])
          {
            neut_nodes_markasslave (pNodes, node, nodemaster,
                                    Tess.PerFaceShift[i]);
            node_slave[node] = 1;
          }
        }

  ut_free_2d_int (&fir, nodeqty_tot);
  ut_free_1d_int (&node_slave);
  ut_free_1d_int (&shift);

  return;
}
