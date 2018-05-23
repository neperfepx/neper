/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_mesh_3d_.h"

void
nev_print_mesh_3d_compress (struct PRINT Print, struct NODES Nodes,
			    struct MESH Mesh3D,
			    struct NODES *pN,
			    struct MESH *pM2D,
			    int ***pprintelt3dface,
			    int **pelt2delt3d, int **pnodes_new_old)
{
  int i, j, k, eltqty, elt3d, elt3dqty;
  int *elts = ut_alloc_1d_int (Mesh3D.EltQty);
  int *eltsinv = NULL;
  int eltmax;
  int elt2dnodeqty = neut_elt_nodeqty (Mesh3D.EltType, 2, 1);
  int **seq3 = NULL;
  int *elts3d = NULL;
  int *elset_full = ut_alloc_1d_int (Mesh3D.ElsetQty + 1);
  (*pelt2delt3d) = ut_alloc_1d_int (1);
  int lefteltqty;
  int *leftelts = NULL;
  int *printelt3d = ut_alloc_1d_int (Mesh3D.EltQty + 1);
  int *nodes = NULL;
  int elt3dfaceqty;

  (*pM2D).EltType = ut_alloc_1d_char (strlen (Mesh3D.EltType) + 1);
  (*pM2D).Dimension = 2;
  (*pM2D).EltOrder = 1;
  strcpy ((*pM2D).EltType, Mesh3D.EltType);

  neut_elt_boundlist (Mesh3D.EltType, 3, &seq3, &elt3dfaceqty, NULL);
  elt2dnodeqty = neut_elt_nodeqty (Mesh3D.EltType, 2, 1);
  nodes = ut_alloc_1d_int (elt2dnodeqty);

  eltqty = 0;
  if (ut_array_1d_int_sum (Print.showelt3d + 1, Mesh3D.EltQty) == Mesh3D.EltQty)
  {
    eltqty = Mesh3D.EltQty;
    eltmax = Mesh3D.EltQty;
    eltsinv = ut_alloc_1d_int (Mesh3D.EltQty + 1);
    for (i = 0; i < Mesh3D.EltQty; i++)
    {
      elts[i] = i + 1;
      eltsinv[i + 1] = i;
    }
  }
  else
  {
    for (i = 1; i <= Mesh3D.EltQty; i++)
      if (Print.showelt3d[i] == 1)
        elts[eltqty++] = i;
    if (eltqty > 0)
      ut_array_1d_int_inv (elts, eltqty, &eltsinv, &eltmax);
  }

  elts = ut_realloc_1d_int (elts, eltqty);

  if (Print.datareduction == 1)
  {
    ut_array_1d_int_set (elset_full + 1, Mesh3D.ElsetQty, 1);
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
      for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
	if (Print.showelt3d[Mesh3D.Elsets[i][j]] == 0)
	{
	  elset_full[i] = 0;
	  break;
	}

    if (eltqty > 0)
      neut_mesh_elts_boundelts (Mesh3D, elts, eltqty, &leftelts, &lefteltqty);
    else
      lefteltqty = 0;

    for (i = 0; i < lefteltqty; i++)
      printelt3d[leftelts[i]] = 1;
  }
  else
  {
    lefteltqty = eltqty;
    leftelts = elts;
  }

  for (i = 0; i < lefteltqty; i++)
  {
    elt3d = leftelts[i];

    for (j = 0; j < elt3dfaceqty; j++)
    {
      for (k = 0; k < elt2dnodeqty; k++)
	nodes[k] = Mesh3D.EltNodes[elt3d][seq3[j][k]];

      neut_mesh_nodes_comelts (Mesh3D, nodes, elt2dnodeqty, &elts3d,
			       &elt3dqty);

      int print;

      print = 0;

      if (elt3dqty == 1)
	print = 1;

      else if (elt3dqty == 2)
      {
	// keep the first condition, or combine w the second
	// (because of the `else if')
	if ((Mesh3D.EltElset[elts3d[0]] == Mesh3D.EltElset[elts3d[1]]))
	{
	  if (elset_full[Mesh3D.EltElset[elts3d[0]]] == 1)
	    print = 0;
	  else if (printelt3d[elts3d[0]] == 0 || printelt3d[elts3d[1]] == 0)
	    print = 1;
	}
	else
	{
	  if (elset_full[Mesh3D.EltElset[elts3d[0]]] == 1
	      && elset_full[Mesh3D.EltElset[elts3d[1]]] == 1)
	    print = 0;
	  else
	    print = 1;
	}
      }

      if (print)
      {
	(*pprintelt3dface)[elt3d][j] = 1;
	neut_mesh_addelt (pM2D, nodes);
	(*pelt2delt3d) = ut_realloc_1d_int (*pelt2delt3d, (*pM2D).EltQty + 1);
	(*pelt2delt3d)[(*pM2D).EltQty] = elt3d;
      }
    }
  }

  int *node_status = ut_alloc_1d_int (Nodes.NodeQty + 1);
  for (i = 1; i <= (*pM2D).EltQty; i++)
    for (j = 0; j < elt2dnodeqty; j++)
      node_status[(*pM2D).EltNodes[i][j]] = 1;

  int max = 0;
  for (i = 1; i <= Nodes.NodeQty; i++)
    if (node_status[i] == 1)
      max = i;

  int *nodes_old_new = ut_alloc_1d_int (max + 1);
  (*pnodes_new_old) = ut_alloc_1d_int (Nodes.NodeQty + 1);
  int id = 0;

  for (i = 1; i <= max; i++)
    if (node_status[i] == 1)
    {
      nodes_old_new[i] = ++id;
      (*pnodes_new_old)[id] = i;
      neut_nodes_addnode (pN, Nodes.NodeCoo[i], 0);
    }

  neut_mesh_switch (pM2D, nodes_old_new, NULL, NULL);
  neut_mesh_init_nodeelts (pM2D, Nodes.NodeQty);

  ut_free_1d_int (elts);
  ut_free_1d_int (node_status);
  ut_free_1d_int (nodes_old_new);

  return;
}

void
nev_print_mesh_3d_print (FILE * file, struct PRINT Print, struct NODES N,
			 struct MESH M1D, struct MESH M2D,
			 int *elt2delt3d, int *nodes_new_old,
			 struct NODEDATA NodeData, struct MESHDATA *MeshData)
{
  nev_print_mesh_3d_print_faces (file, Print, N, M2D,
				 elt2delt3d, nodes_new_old,
				 NodeData, MeshData);

  nev_print_mesh_3d_print_edges (file, Print.showshadow, N, M1D, MeshData);

  return;
}
