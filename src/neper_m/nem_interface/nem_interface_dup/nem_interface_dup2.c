/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup_ver (struct TESS Tess, struct NODES *pNodes,
		   struct MESH *Mesh)
{
  int i;
  int nodeqty, *nodes = NULL;
  int cellqty, *cells = NULL;

  for (i = 1; i <= Tess.VerQty; i++)
  {
    neut_tess_ver_cells (Tess, i, &cells, &cellqty);
    neut_mesh_elset_nodes (Mesh[0], i, &nodes, &nodeqty);
    nem_interface_duplicate (Tess.Dim, cells, cellqty, nodes, nodeqty, pNodes, Mesh);
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (cells);

  return;
}

void
nem_interface_dup_edge (struct TESS Tess, struct NODES *pNodes,
		         struct MESH *Mesh)
{
  int i;
  int nodeqty, *nodes = NULL;
  int cellqty, *cells = NULL;

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_tess_edge_cells (Tess, i, &cells, &cellqty);
    neut_mesh_elset1d_bodynodes (Tess, Mesh, i, &nodes, &nodeqty);
    nem_interface_duplicate (Tess.Dim, cells, cellqty, nodes, nodeqty, pNodes, Mesh);
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (cells);

  return;
}

void
nem_interface_dup_face (struct TESS Tess, struct NODES *pNodes,
		         struct MESH *Mesh)
{
  int i;
  int nodeqty, *nodes = NULL;
  int polyqty, *polys = NULL;

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_tess_face_polys (Tess, i, &polys, &polyqty);
    neut_mesh_elset2d_bodynodes (Tess, Mesh, i, &nodes, &nodeqty);
    nem_interface_duplicate (Tess.Dim, polys, polyqty, nodes, nodeqty, pNodes, Mesh);
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (polys);

  return;
}

void
nem_interface_dup_boundelts_3d (struct TESS Tess, struct NODES Nodes,
			    struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int i, j, k, l, elt2d, elt3d, surf;
  int* nodes2d = ut_alloc_1d_int (3);

  (*pBound).BoundQty = Tess.FaceQty;
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy ((*pBound).BoundDom + 1, (*pBound).BoundQty, 2,
                          Tess.FaceDom + 1);
  (*pBound).BoundCell = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy ((*pBound).BoundCell + 1, (*pBound).BoundQty, 2,
                          Tess.FacePoly + 1);
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundEltFacets = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceDom[i][0] == -1)
    {
      ut_array_1d_int_sort ((*pBound).BoundCell[i], 2);

      (*pBound).BoundEltQty[i] = Mesh[2].Elsets[i][0];
      (*pBound).BoundElts[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);
      (*pBound).BoundEltFacets[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);

      for (j = 0; j < 2; j++)
	for (k = 1; k <= (*pBound).BoundEltQty[i]; k++)
	{
	  elt2d = Mesh[2].Elsets[i][k];

	  for (l = 0; l < 3; l++)
	  {
	    neut_nodes_node_cell_dupnode (Nodes,
		Mesh[2].EltNodes[elt2d][l], (*pBound).BoundCell[i][j], nodes2d + l);
	    if (nodes2d[l] == -1)
	      nodes2d[l] = Mesh[2].EltNodes[elt2d][l];
	  }

	  neut_mesh_nodes_comelt (Mesh[3], nodes2d, 3, &elt3d);
	  if (elt3d <= 0)
	    abort ();
	  (*pBound).BoundElts[i][j][k] = elt3d;

	  neut_elt_nodes_facet (3, Mesh[3].EltNodes[elt3d],
				    nodes2d, &surf);
	  (*pBound).BoundEltFacets[i][j][k] = surf;
	}
    }

  (*pBound).BoundNames = ut_alloc_3d_char ((*pBound).BoundQty + 1, 2, 100);
  for (i = 1; i <= (*pBound).BoundQty; i++)
    for (j = 0; j < 2; j++)
      sprintf ((*pBound).BoundNames[i][j], "bound%d-cell%d", i,
	       (*pBound).BoundCell[i][j]);

  ut_free_1d_int (nodes2d);

  return;
}

void
nem_interface_dup_boundelts_2d (struct TESS Tess, struct NODES Nodes,
                            struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int i, j, k, l, elt1d, elt2d, surf;
  int* nodes1d = ut_alloc_1d_int (3);

  (*pBound).BoundQty = Tess.EdgeQty;
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy ((*pBound).BoundDom + 1, (*pBound).BoundQty, 2,
                          Tess.EdgeDom + 1);
  (*pBound).BoundCell = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  for (i = 1; i <= (*pBound).BoundQty; i++)
  {
    ut_array_1d_int_memcpy ((*pBound).BoundCell[i], Tess.EdgeFaceQty[i],
			    Tess.EdgeFaceNb[i]);
    if (Tess.EdgeFaceQty[i] == 1)
      (*pBound).BoundCell[i][1] = -1;
  }
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundEltFacets = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeDom[i][0] == -1)
    {
      ut_array_1d_int_sort ((*pBound).BoundCell[i], 2);

      (*pBound).BoundEltQty[i] = Mesh[1].Elsets[i][0];
      (*pBound).BoundElts[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);
      (*pBound).BoundEltFacets[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);

      for (j = 0; j < 2; j++)
	for (k = 1; k <= (*pBound).BoundEltQty[i]; k++)
	{
	  elt1d = Mesh[1].Elsets[i][k];

	  for (l = 0; l < 2; l++)
	  {
	    neut_nodes_node_cell_dupnode (Nodes,
		Mesh[1].EltNodes[elt1d][l], (*pBound).BoundCell[i][j], nodes1d + l);
	    if (nodes1d[l] == -1)
	      nodes1d[l] = Mesh[1].EltNodes[elt1d][l];
	  }

	  neut_mesh_nodes_comelt (Mesh[2], nodes1d, 2, &elt2d);
	  if (elt2d <= 0)
	    abort ();
	  (*pBound).BoundElts[i][j][k] = elt2d;

	  neut_elt_nodes_facet (2, Mesh[2].EltNodes[elt2d],
				    nodes1d, &surf);
	  (*pBound).BoundEltFacets[i][j][k] = surf;
	}
    }

  (*pBound).BoundNames = ut_alloc_3d_char ((*pBound).BoundQty + 1, 2, 100);
  for (i = 1; i <= (*pBound).BoundQty; i++)
    for (j = 0; j < 2; j++)
      sprintf ((*pBound).BoundNames[i][j], "bound%d-cell%d", i,
	       (*pBound).BoundCell[i][j]);

  ut_free_1d_int (nodes1d);

  return;
}

// making sure that all nodes of 1D elts belongs to the same (master,
// or lower-index) cell the edge belongs to.
void
nem_interface_dup_renumber_1d (struct TESS Tess, struct NODES Nodes,
			   struct MESH *Mesh)
{
  int i, j, k, node, dupnode, elt, eltnodeqty;
  int cellqty, *cells = NULL, mastercell;

  eltnodeqty = neut_elt_nodeqty (Mesh[1].EltType, Mesh[1].Dimension, Mesh[1].EltOrder);

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_tess_edge_cells (Tess, i, &cells, &cellqty);
    mastercell = cells[0];

    for (j = 1; j <= Mesh[1].Elsets[i][0]; j++)
    {
      elt = Mesh[1].Elsets[i][j];

      for (k = 0; k < eltnodeqty; k++)
      {
	node = Mesh[1].EltNodes[elt][k];
	neut_nodes_node_cell_dupnode (Nodes, node, mastercell,
	    &dupnode);
	if (dupnode != -1)
	  Mesh[1].EltNodes[elt][k] = dupnode;
      }
    }
  }

  return;
}

// making sure that all nodes of 2D elts belongs to the same (master,
// or lower-index) cell the face belongs to.
void
nem_interface_dup_renumber_2d (struct TESS Tess, struct NODES Nodes,
			   struct MESH *Mesh)
{
  int i, j, k, node, dupnode, elt, eltnodeqty;
  int cellqty, *cells = NULL, mastercell;

  eltnodeqty = neut_elt_nodeqty (Mesh[2].EltType, Mesh[2].Dimension, Mesh[2].EltOrder);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_tess_face_cells (Tess, i, &cells, &cellqty);
    mastercell = cells[0];

    for (j = 1; j <= Mesh[2].Elsets[i][0]; j++)
    {
      elt = Mesh[2].Elsets[i][j];

      for (k = 0; k < eltnodeqty; k++)
      {
	node = Mesh[2].EltNodes[elt][k];
	neut_nodes_node_cell_dupnode (Nodes, node, mastercell,
	    &dupnode);
	if (dupnode != -1)
	  Mesh[2].EltNodes[elt][k] = dupnode;
      }
    }
  }

  return;
}
