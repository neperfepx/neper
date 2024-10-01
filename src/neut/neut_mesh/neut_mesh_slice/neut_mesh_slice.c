/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_slice_.h"

void
neut_mesh_slice (struct NODES Nodes, struct MESH Mesh,
                 struct DATA NodeData, struct DATA MeshData,
                 char *slice, int *pSQty, struct NODES **pN, struct MESH ***pM,
                 struct DATA **pData, struct DATA ****pSMeshData,
                 int ***pEltNewOld)
{
  int i, j, k;
  char **label = NULL;
  double *eq = ut_alloc_1d (4);

  ut_list_break (slice, NEUT_SEP_NODEP, &label, pSQty);

  // allocation
  (*pN) = malloc (*pSQty * sizeof (struct NODES));
  (*pData) = malloc (*pSQty * sizeof (struct DATA));

  (*pM) = malloc (*pSQty * sizeof (struct MESH*));
  for (i = 0; i < *pSQty; i++)
    (*pM)[i] = malloc (3 * sizeof (struct MESH));

  (*pSMeshData) = malloc (*pSQty * sizeof (struct DATA **));
  for (i = 0; i < *pSQty; i++)
  {
    (*pSMeshData)[i] = malloc (4 * sizeof (struct DATA *));
    for (j = 0; j <= 3; j++)
      (*pSMeshData)[i][j] = malloc (3 * sizeof (struct DATA));
  }

  (*pEltNewOld) = ut_alloc_1d_pint (*pSQty);

  // initialization
  for (i = 0; i < *pSQty; i++)
    for (j = 0; j < 3; j++)
      neut_mesh_set_zero ((*pM)[i] + j);

  for (i = 0; i < *pSQty; i++)
  {
    neut_nodes_set_zero (&((*pN)[i]));
    neut_data_set_default (&((*pData)[i]));
    for (j = 0; j <= 3; j++)
      for (k = 0; k < 3; k++)
        neut_data_set_default ((*pSMeshData)[i][j] + k);
  }

  for (i = 0; i < *pSQty; i++)
  {
    ut_space_string_plane (label[i], eq);

    int **node_newold = NULL;
    double *node_fact = NULL;

    double **coo = NULL;
    if (NodeData.Coo)
    {
      coo = Nodes.NodeCoo;
      Nodes.NodeCoo = NodeData.Coo;
    }

    neut_mesh3d_slice (Nodes, Mesh, eq, &((*pN)[i]), &((*pM)[i][2]),
                       (*pEltNewOld) + i, &node_newold, &node_fact);

    if (NodeData.Coo)
      Nodes.NodeCoo = coo;

    neut_data_mesh2slice_nodes (NodeData, (*pN)[i], node_newold, node_fact,
                                &((*pData)[i]));
    neut_data_mesh2slice_elts (MeshData, (*pM)[i][2], (*pEltNewOld)[i],
                               &((*pSMeshData)[i]));

    neut_mesh_init_nodeelts ((*pM)[i] + 2, 0);
    neut_mesh2d_mesh1d ((*pN)[i], (*pM)[i][2], (*pM)[i] + 1, NULL, NULL, NULL, 0);
    neut_mesh1d_mesh0d ((*pM)[i][1], (*pM)[i], NULL, NULL, NULL, 0);
  }

  ut_free_1d (&eq);
  ut_free_2d_char (&label, *pSQty);

  return;
}
