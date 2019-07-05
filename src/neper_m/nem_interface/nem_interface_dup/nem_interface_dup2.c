/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup_pre (struct IN_M In, struct TESS Tess,
                       int **pvers, int *pverqty,
                       int **pedges, int *pedgeqty,
                       int **pfaces, int *pfaceqty)
{
  int i, scale, partqty;
  char **parts = NULL;
  char **interface = ut_alloc_1d_pchar (Tess.ScaleQty + 1);

  ut_string_separate (In.interface, NEUT_SEP_FRAC, &parts, &partqty);

  partqty = ut_num_min (partqty, Tess.ScaleQty);

  ut_string_string ("continuous", interface);
  for (i = 1; i <= partqty; i++)
    ut_string_string (parts[i - 1], interface + i);
  for (i = partqty + 1; i <= Tess.ScaleQty; i++)
    ut_string_string (parts[partqty - 1], interface + i);

  if (Tess.Dim == 3)
  {
    (*pfaceqty) = 0;
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      neut_tess_face_scale (Tess, i, &scale);

      if (strcmp (interface[scale], "continuous"))
        ut_array_1d_int_list_addelt_nocheck (pfaces, pfaceqty, i);
    }

    neut_tess_faces_edges (Tess, *pfaces, *pfaceqty, pedges, pedgeqty);
    neut_tess_faces_vers (Tess, *pfaces, *pfaceqty, pvers, pverqty);
  }
  else
  {
    (*pfaceqty) = 0;
    (*pedgeqty) = 0;
    for (i = 1; i <= Tess.EdgeQty; i++)
    {
      neut_tess_edge_scale (Tess, i, &scale);

      if (strcmp (interface[scale], "continuous"))
        ut_array_1d_int_list_addelt_nocheck (pedges, pedgeqty, i);
    }

    neut_tess_edges_vers (Tess, *pedges, *pedgeqty, pvers, pverqty);
  }

  ut_free_2d_char (interface, Tess.ScaleQty + 1);
  ut_free_2d_char (parts, partqty);

  return;
}

// The principle is to duplicate all vers but those belonging to slave
// interfaces.  To do so, we work from the interfaces.  By using
// "interfaces", we can deal with 2D and 3D.
void
nem_interface_dup_ver (struct TESS Tess, int *vers, int verqty,
                       struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, qty, *tmp = NULL;
  int nodeqty, *nodes = NULL;
  int seedqty, *seeds = NULL;
  int inter, interqty, *inters = NULL;
  int ver;

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    neut_tess_ver_inters (Tess, ver, &inters, &interqty);

    seedqty = 0;
    for (j = 0; j < interqty; j++)
    {
      inter = inters[j];
      neut_tess_inter_seeds (Tess, inter, &tmp, &qty);

      if (!neut_tess_inter_isperslave (Tess, inter))
	ut_array_1d_int_list_addelts (&seeds, &seedqty, tmp, qty);
    }

    ut_array_1d_int_sort (seeds, seedqty);

    neut_mesh_elset_nodes (Mesh[0], ver, &nodes, &nodeqty);
    nem_interface_duplicate (Tess.Dim, seeds, seedqty, nodes, nodeqty, pNodes, Mesh);
  }

  ut_free_1d_int (tmp);
  ut_free_1d_int (nodes);
  ut_free_1d_int (seeds);
  ut_free_1d_int (inters);

  return;
}

void
nem_interface_dup_edge (struct TESS Tess, int *edges, int edgeqty,
                        struct NODES *pNodes, struct MESH *Mesh)
{
  int i, edge;
  int nodeqty, *nodes = NULL;
  int seedqty, *seeds = NULL;

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    neut_tess_edge_seeds (Tess, edge, &seeds, &seedqty);
    if (neut_tess_edge_isperslave (Tess, edge))
      neut_tess_edge_masterseeds (Tess, edge, &seeds, &seedqty);

    neut_mesh_elset1d_bodynodes (Tess, Mesh, edge, &nodes, &nodeqty);
    nem_interface_duplicate (Tess.Dim, seeds, seedqty, nodes, nodeqty, pNodes, Mesh);
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (seeds);

  return;
}

void
nem_interface_dup_face (struct TESS Tess, int *faces, int faceqty,
                        struct NODES *pNodes, struct MESH *Mesh)
{
  int i, face;
  int nodeqty, *nodes = NULL;
  int seedqty, *seeds = NULL;

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    neut_tess_face_seeds (Tess, face, &seeds, &seedqty);
    if (neut_tess_face_isperslave (Tess, face))
      neut_tess_face_masterseeds (Tess, face, &seeds, &seedqty);

    neut_mesh_elset2d_bodynodes (Tess, Mesh, face, &nodes, &nodeqty);
    nem_interface_duplicate (Tess.Dim, seeds, seedqty, nodes, nodeqty, pNodes, Mesh);
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (seeds);

  return;
}

void
nem_interface_dup_boundelts_3d (struct TESS Tess, int *faces, int faceqty,
                                struct NODES Nodes, struct MESH *Mesh,
                                struct BOUNDARY *pBound)
{
  int i, j, k, l, elt2d, elt3d, surf, surfori, face;
  int* nodes2d = ut_alloc_1d_int (3);
  int nodeqty, *nodes = NULL;

  (*pBound).BoundQty = Tess.FaceQty;
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy ((*pBound).BoundDom + 1, (*pBound).BoundQty, 2,
                          Tess.FaceDom + 1);
  (*pBound).BoundSeed = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy ((*pBound).BoundSeed + 1, (*pBound).BoundQty, 2,
                          Tess.FacePoly + 1);
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundEltFacets = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundNodeQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundNodes = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    if (Tess.FaceDom[face][0] == -1)
    {
      ut_array_1d_int_sort ((*pBound).BoundSeed[face], 2);

      (*pBound).BoundEltQty[face] = Mesh[2].Elsets[face][0];
      (*pBound).BoundElts[face] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[face] + 1);
      (*pBound).BoundEltFacets[face] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[face] + 1);

      for (j = 0; j < 2; j++)
	for (k = 1; k <= (*pBound).BoundEltQty[face]; k++)
	{
	  elt2d = Mesh[2].Elsets[face][k];

	  for (l = 0; l < 3; l++)
	  {
	    neut_nodes_node_seed_dupnode (Nodes,
		Mesh[2].EltNodes[elt2d][l], (*pBound).BoundSeed[face][j], nodes2d + l);
	    if (nodes2d[l] == -1)
	      nodes2d[l] = Mesh[2].EltNodes[elt2d][l];
	  }

	  neut_mesh_nodes_comelt (Mesh[3], nodes2d, 3, &elt3d);
	  if (elt3d <= 0)
	    abort ();
	  (*pBound).BoundElts[face][j][k] = elt3d;

	  neut_elt_nodes_facet (3, Mesh[3].EltNodes[elt3d],
				    nodes2d, &surf, &surfori);
	  (*pBound).BoundEltFacets[face][j][k] = surf;
	}

      // initializing BoundNodes and BoundNodeQty
      neut_boundary_bound_nodes (Mesh[3], *pBound, face, 0, &nodes,
				 &nodeqty);

      if (nodeqty != 3 * (*pBound).BoundEltQty[face])
	abort ();

      (*pBound).BoundNodeQty[face] = nodeqty;
      (*pBound).BoundNodes[face] = ut_alloc_2d_int (2, nodeqty + 1);

      ut_array_1d_int_memcpy ((*pBound).BoundNodes[face][0] + 1, nodeqty,
			      nodes);

      for (j = 1; j <= (*pBound).BoundNodeQty[face]; j++)
	neut_nodes_node_seed_dupnode (Nodes, (*pBound).BoundNodes[face][0][j],
				      (*pBound).BoundSeed[face][1],
				      (*pBound).BoundNodes[face][1] + j);

      ut_free_1d_int_ (&nodes);
    }
  }

  (*pBound).BoundNames = ut_alloc_3d_char ((*pBound).BoundQty + 1, 2, 100);
  for (i = 1; i <= (*pBound).BoundQty; i++)
    for (j = 0; j < 2; j++)
      sprintf ((*pBound).BoundNames[i][j], "bound%d-cell%d", i,
	       (*pBound).BoundSeed[i][j]);

  ut_free_1d_int (nodes2d);

  return;
}

void
nem_interface_dup_boundelts_2d (struct TESS Tess, struct NODES Nodes,
                            struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int i, j, k, l, elt1d, elt2d, surf, surfori;
  int* nodes1d = ut_alloc_1d_int (3);
  int nodeqty, *nodes = NULL;

  (*pBound).BoundQty = Tess.EdgeQty;
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy ((*pBound).BoundDom + 1, (*pBound).BoundQty, 2,
                          Tess.EdgeDom + 1);
  (*pBound).BoundSeed = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  for (i = 1; i <= (*pBound).BoundQty; i++)
  {
    ut_array_1d_int_memcpy ((*pBound).BoundSeed[i], Tess.EdgeFaceQty[i],
			    Tess.EdgeFaceNb[i]);
    if (Tess.EdgeFaceQty[i] == 1)
      (*pBound).BoundSeed[i][1] = -1;
  }
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundEltFacets = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundNodeQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundNodes = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeDom[i][0] == -1 && !neut_tess_edge_isperslave (Tess, i))
    {
      ut_array_1d_int_sort ((*pBound).BoundSeed[i], 2);

      (*pBound).BoundEltQty[i] = Mesh[1].Elsets[i][0];
      (*pBound).BoundElts[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);
      (*pBound).BoundEltFacets[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);

      // the if statement below has the effect of skipping the slave
      // edges of a periodic tessellation
      for (j = 0; j < 2; j++)
	if ((*pBound).BoundSeed[i][j] <= Tess.CellQty)
	  for (k = 1; k <= (*pBound).BoundEltQty[i]; k++)
	  {
	    elt1d = Mesh[1].Elsets[i][k];

	    for (l = 0; l < 2; l++)
	    {
	      neut_nodes_node_seed_dupnode (Nodes,
		  Mesh[1].EltNodes[elt1d][l], (*pBound).BoundSeed[i][j], nodes1d + l);
	      if (nodes1d[l] == -1)
		nodes1d[l] = Mesh[1].EltNodes[elt1d][l];
	    }

	    neut_mesh_nodes_comelt (Mesh[2], nodes1d, 2, &elt2d);
	    if (elt2d <= 0)
	      abort ();
	    (*pBound).BoundElts[i][j][k] = elt2d;

	    neut_elt_nodes_facet (2, Mesh[2].EltNodes[elt2d],
				      nodes1d, &surf, &surfori);
	    (*pBound).BoundEltFacets[i][j][k] = surf;
	  }

      neut_boundary_bound_nodes (Mesh[2], *pBound, i, 0, &nodes,
				 &nodeqty);
      if (nodeqty != (*pBound).BoundEltQty[i] + 1)
	abort ();

      (*pBound).BoundNodeQty[i] = nodeqty;
      (*pBound).BoundNodes[i] = ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 2);

      ut_array_1d_int_memcpy ((*pBound).BoundNodes[i][0] + 1, nodeqty,
			      nodes);

      for (j = 1; j <= (*pBound).BoundEltQty[i] + 1; j++)
      {
	neut_nodes_node_seed_dupnode (Nodes, (*pBound).BoundNodes[i][0][j],
				      (*pBound).BoundSeed[i][1],
				      (*pBound).BoundNodes[i][1] + j);

	if ((*pBound).BoundNodes[i][0][j] == (*pBound).BoundNodes[i][1][j])
	{
	  printf ("node %d and dupnode seed %d are the same\n",
		  (*pBound).BoundNodes[i][0][j],
		  (*pBound).BoundSeed[i][1]);
	  abort ();
	}
      }

      for (j = 1; j <= (*pBound).BoundEltQty[i] + 1; j++)
	if ((*pBound).BoundNodes[i][0][j] == (*pBound).BoundNodes[i][1][j])
	  abort ();

      ut_free_1d_int_ (&nodes);
    }

  (*pBound).BoundNames = ut_alloc_3d_char ((*pBound).BoundQty + 1, 2, 100);
  for (i = 1; i <= (*pBound).BoundQty; i++)
    for (j = 0; j < 2; j++)
      sprintf ((*pBound).BoundNames[i][j], "bound%d-cell%d", i,
	       (*pBound).BoundSeed[i][j]);

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
	neut_nodes_node_seed_dupnode (Nodes, node, mastercell,
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
	neut_nodes_node_seed_dupnode (Nodes, node, mastercell,
	    &dupnode);
	if (dupnode != -1)
	  Mesh[2].EltNodes[elt][k] = dupnode;
      }
    }
  }

  return;
}

void
nem_interface_dup_per (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, k, permaster, perslave, dupnode, mnodeqty, *mnodes = NULL;
  int dupnodeseed, masterseed, elset, newslave, dupnodeslave, status;
  int tmpv[3];
  int pairqty, **pairs = NULL;
  int dim = Tess.Dim;

  neut_mesh_init_nodeelts (Mesh + dim, (*pNodes).NodeQty);

  neut_nodes_permasters (*pNodes, &mnodes, &mnodeqty);

  for (i = 0; i < mnodeqty; i++)
  {
    permaster = mnodes[i];

    pairs = ut_alloc_2d_int ((*pNodes).DupNodeSlaveQty[permaster] + 1, 2);

    pairqty = 0;
    for (j = 1; j <= (*pNodes).DupNodeSlaveQty[permaster]; j++)
    {
      dupnode = (*pNodes).DupNodeSlaveNb[permaster][j];
      dupnodeseed = (*pNodes).DupNodeSeed[dupnode];

      if (neut_tess_seed_isper (Tess, dupnodeseed))
      {
	neut_tess_seed_masterseed (Tess, dupnodeseed, &masterseed);
	ut_array_1d_int_memcpy (tmpv, 3, Tess.PerSeedShift[dupnodeseed]);
	ut_array_1d_int_scale (tmpv, 3, -1);
	status = neut_nodes_node_shift_pernode (*pNodes, permaster, tmpv, &perslave);
	if (status != 0)
	{
	  printf ("node not found.\n");
	  abort ();
	}
	neut_mesh_node_elset (Mesh[dim], perslave, &elset);
	if (elset == -1)
	  abort ();

	if (elset != masterseed)
	{
	  newslave = -1;
	  for (k = 1; k <= (*pNodes).DupNodeSlaveQty[perslave]; k++)
	  {
	    dupnodeslave = (*pNodes).DupNodeSlaveNb[perslave][k];
	    neut_mesh_node_elset (Mesh[dim], dupnodeslave, &elset);
	    if (elset == masterseed)
	    {
	      newslave = dupnodeslave;
	      break;
	    }
	  }

	  if (newslave == -1)
	    abort ();

	  perslave = newslave;
	}

	pairqty++;
	ut_array_1d_int_set_2 (pairs[pairqty - 1], dupnode, perslave);
      }
    }

    for (j = 0; j < pairqty; j++)
    {
      dupnode   = pairs[j][0];
      perslave  = pairs[j][1];
      (*pNodes).PerNodeMaster[perslave] = dupnode;
    }
    (*pNodes).PerNodeSlaveQty[permaster] = 0;

    ut_free_2d_int_ (&pairs, pairqty);
  }

  ut_free_1d_int (mnodes);

  return;
}
