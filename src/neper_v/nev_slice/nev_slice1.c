/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_slice_.h"

void
nev_slice (char **argv, int *pi, struct PRINT *pPrint)
{
  if (strcmp (argv[*pi], "-slicemesh") == 0)
  {
    (*pi)++;
    (*pPrint).slice = ut_alloc_1d_char (strlen (argv[(*pi)]) + 1);
    sscanf (argv[(*pi)], "%s", (*pPrint).slice);
  }
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  if ((*pPrint).showslice)
  {
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showtesr == -1)
      (*pPrint).showtesr = 0;
    if ((*pPrint).showmesh == -1)
      (*pPrint).showmesh = 0;
  }

  sscanf (argv[(*pi)], "%s", (*pPrint).slice);

  return;
}

void
nev_slice_mesh (struct NODES Nodes, struct MESH Mesh,
		struct NODEDATA NodeData,
		struct MESHDATA *MeshData,
		char *slice, int *pSQty,
		struct NODES **pN, struct MESH **pM,
		struct NODEDATA **pNodeData,
		struct MESHDATA ***pMeshData, int ***pEltNewOld)
{
  int i, j;
  char **label = NULL;
  double *eq = ut_alloc_1d (4);

  ut_string_separate (slice, NEUT_SEP_NODEP, &label, pSQty);

  (*pN) = malloc (*pSQty * sizeof (struct NODES));
  (*pM) = malloc (*pSQty * sizeof (struct MESH));
  (*pNodeData) = malloc (*pSQty * sizeof (struct NODEDATA));
  (*pMeshData) = malloc (*pSQty * sizeof (struct MESHDATA *));
  (*pEltNewOld) = ut_alloc_1d_pint (*pSQty);

  for (i = 0; i < *pSQty; i++)
  {
    neut_nodes_set_zero (&((*pN)[i]));
    neut_nodedata_set_default (&((*pNodeData)[i]));

    neut_mesh_set_zero (&((*pM)[i]));
    (*pMeshData)[i] = malloc (4 * sizeof (struct MESHDATA));
    for (j = 0; j <= 3; j++)
      neut_meshdata_set_default (&((*pMeshData)[i][j]));
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

    neut_nodedata_mesh2slice (NodeData, (*pN)[i], node_newold, node_fact,
			      &((*pNodeData)[i]));
    neut_meshdata_mesh2slice (MeshData, (*pM)[i], (*pEltNewOld)[i],
			      &((*pMeshData)[i]));
  }

  ut_free_1d (eq);
  ut_free_2d_char (label, *pSQty);

  return;
}
