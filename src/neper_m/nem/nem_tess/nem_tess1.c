/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_tess_.h"

void
nem_tess_face_1delts (struct TESS Tess, struct MESH *Mesh, int
		      face, int **pelts, int *peltqty)
{
  int i, j, EdgeQty, edge, ori;

  (*peltqty) = 0;

  EdgeQty = Tess.FaceVerQty[face];
  for (i = 1; i <= EdgeQty; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];
    (*peltqty) += Mesh[1].Elsets[edge][0];
  }

  (*pelts) = ut_alloc_1d_int (*peltqty);
  (*peltqty) = 0;

  for (i = 1; i <= EdgeQty; i++)
  {
    ori = Tess.FaceEdgeOri[face][i];
    edge = Tess.FaceEdgeNb[face][i];

    if (ori == 1)
      for (j = 1; j <= Mesh[1].Elsets[edge][0]; j++)
	(*pelts)[(*peltqty)++] = Mesh[1].Elsets[edge][j];
    else
      for (j = Mesh[1].Elsets[edge][0]; j >= 1; j--)
	(*pelts)[(*peltqty)++] = -Mesh[1].Elsets[edge][j];
  }

  return;
}

void
nem_tess_updatefrommesh_geom (struct TESS *pTess, struct NODES
			      Nodes, struct MESH *Mesh)
{
  if (Mesh[3].EltQty > 0)
    nem_tess_updatefrommesh_geom_seed (pTess, Nodes, Mesh);

  if (Mesh[2].EltQty > 0)
    nem_tess_updatefrommesh_geom_face (pTess, Nodes, Mesh);

  if (Mesh[1].EltQty > 0)
    nem_tess_updatefrommesh_geom_edge (pTess, Nodes, Mesh);

  if (Mesh[0].EltQty > 0)
    nem_tess_updatefrommesh_geom_ver (pTess, Nodes, Mesh);

  return;
}

void
nem_tess_edgebodies_nodes (struct TESS Tess, struct MESH *Mesh,
			   int **pnodes, int *pnodeqty)
{
  int i, j, elt, qty;
  int *tmp = NULL;

  // determining the nodes of the body of the edges
  (*pnodeqty) = 0;
  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_mesh_elset_nodes (Mesh[1], i, &tmp, &qty);

    for (j = 0; j < 2; j++)
    {
      elt = Mesh[0].Elsets[Tess.EdgeVerNb[i][j]][1];

      if (ut_array_1d_int_deletencompress (tmp, qty,
					   Mesh[0].EltNodes[elt][0], 1) != 1)
	ut_error_reportbug ();

      qty--;
    }

    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + qty);
    ut_array_1d_int_memcpy ((*pnodes) + (*pnodeqty), qty, tmp);
    (*pnodeqty) += qty;

    ut_free_1d_int (tmp);
    tmp = NULL;
  }

  return;
}

void
nem_tess_facebodies_nodes (struct TESS Tess, struct MESH *Mesh,
			   int **pnodes, int *pnodeqty)
{
  int i, j, k, qty;
  int *tmp = NULL;
  int *edgenodes = NULL;
  int edgenodeqty;

  // determining the nodes of the body of the faces
  (*pnodeqty) = 0;
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_mesh_elset_nodes (Mesh[2], i, &tmp, &qty);

    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
    {
      neut_mesh_elset_nodes (Mesh[1], Tess.FaceEdgeNb[i][j], &edgenodes,
			     &edgenodeqty);

      for (k = 0; k < edgenodeqty; k++)
	qty -= ut_array_1d_int_deletencompress (tmp, qty, edgenodes[k], 1);

      ut_free_1d_int (edgenodes);
      edgenodes = NULL;
    }

    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + qty);
    ut_array_1d_int_memcpy ((*pnodes) + (*pnodeqty), qty, tmp);
    (*pnodeqty) += qty;

    ut_free_1d_int (tmp);
    tmp = NULL;
  }

  return;
}
