/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_0D_.h"

void
nem_meshing_0D_ver (struct TESS Tess, int ver, struct MESHPARA MeshPara,
		    struct NODES *pNodes, struct MESH *Mesh)
{
  int i, edge;
  double mindist;
  double cl, pl;
  double clmin = MeshPara.clmin;

  neut_nodes_free (pNodes);
  neut_mesh_free (Mesh + 0);

/***********************************************************************
 * Initializing nodes and mesh */

  /* nodes */
  (*pNodes).NodeQty = 1;
  (*pNodes).NodeCoo = ut_alloc_2d (2, 3);
  (*pNodes).NodeCl = ut_alloc_1d (2);

  ut_array_1d_memcpy ((*pNodes).NodeCoo[1], 3, Tess.VerCoo[ver]);

  /* mesh */
  Mesh[0].Dimension = 0;
  Mesh[0].EltOrder = 1;
  Mesh[0].EltQty = 1;
  Mesh[0].EltNodes = ut_alloc_2d_int (2, 1);
  Mesh[0].EltNodes[1][0] = 1;
  Mesh[0].ElsetQty = 1;
  Mesh[0].Elsets = ut_alloc_2d_int (2, 2);
  Mesh[0].Elsets[1][0] = 1;
  Mesh[0].Elsets[1][1] = 1;

/***********************************************************************
 * Nodes: calculating characteristic length from cl, pl & clmin (&
 * mindist) */
  pl = MeshPara.pl;
  cl = MeshPara.ver_cl[ver];

  int *edgelist = NULL, edgeqty;
  neut_tess_ver_edges (Tess, ver, &edgelist, &edgeqty);

  if (edgeqty > 0)
  {
    mindist = DBL_MAX;
    for (i = 0; i < edgeqty; i++)
    {
      edge = edgelist[i];
      if (edge > 0)
	mindist = ut_num_min (mindist, Tess.EdgeLength[edge]);
    }

    if (mindist > cl / (1 + pl))
      (*pNodes).NodeCl[1] = cl;
    else
      (*pNodes).NodeCl[1] = mindist * pl;

    (*pNodes).NodeCl[1] = ut_num_max ((*pNodes).NodeCl[1], clmin);
  }
  else
    (*pNodes).NodeCl[1] = cl;

  ut_free_1d_int (edgelist);

  return;
}

void
nem_meshing_0D_addvermesh (struct NODES N, struct MESH M,
			   int master, int *shift,
			   struct NODES *pNodes, struct MESH *Mesh)
{
  /* renumbering mesh nodes / elts to match global nodes */
  M.EltNodes[1][0] = (*pNodes).NodeQty + 1;
  M.Elsets[1][1] = (*pNodes).NodeQty + 1;

  neut_nodes_addnode (pNodes, N.NodeCoo[1], N.NodeCl[1]);
  if (master > 0)
    neut_nodes_markasslave (pNodes, (*pNodes).NodeQty, master, shift);

  neut_mesh_addelt (Mesh + 0, M.EltNodes[1]);
  neut_mesh_addelset (Mesh + 0, M.Elsets[1] + 1, M.Elsets[1][0]);

  return;
}
