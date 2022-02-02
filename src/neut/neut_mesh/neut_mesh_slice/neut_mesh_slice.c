/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_slice_.h"

void
neut_mesh_slice (struct NODES Nodes, struct MESH Mesh,
                 struct DATA NodeData, struct DATA *MeshData,
                 char *slice, int *pSQty, struct NODES **pN, struct MESH **pM,
                 struct DATA **pData, struct DATA ***pMeshData,
                 int ***pEltNewOld)
{
  int i, j;
  char **label = NULL;
  double *eq = ut_alloc_1d (4);

  ut_list_break (slice, NEUT_SEP_NODEP, &label, pSQty);

  (*pN) = malloc (*pSQty * sizeof (struct NODES));
  (*pM) = malloc (*pSQty * sizeof (struct MESH));
  (*pData) = malloc (*pSQty * sizeof (struct DATA));
  (*pMeshData) = malloc (*pSQty * sizeof (struct DATA *));
  (*pEltNewOld) = ut_alloc_1d_pint (*pSQty);

  for (i = 0; i < *pSQty; i++)
  {
    neut_nodes_set_zero (&((*pN)[i]));
    neut_data_set_default (&((*pData)[i]));

    neut_mesh_set_zero (&((*pM)[i]));
    (*pMeshData)[i] = malloc (4 * sizeof (struct DATA));
    for (j = 0; j <= 3; j++)
      neut_data_set_default (&((*pMeshData)[i][j]));
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

    neut_mesh3d_slice (Nodes, Mesh, eq, &((*pN)[i]), &((*pM)[i]),
                       (*pEltNewOld) + i, &node_newold, &node_fact);

    if (NodeData.Coo)
      Nodes.NodeCoo = coo;

    neut_data_mesh2slice_nodes (NodeData, (*pN)[i], node_newold, node_fact,
                                &((*pData)[i]));
    neut_data_mesh2slice_elts (MeshData, (*pM)[i], (*pEltNewOld)[i],
                               &((*pMeshData)[i]));
  }

  ut_free_1d (&eq);
  ut_free_2d_char (&label, *pSQty);

  return;
}
