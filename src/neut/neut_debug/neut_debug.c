/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_debug_.h"

void
neut_debug_nodes (FILE * file, struct NODES Nodes)
{
  fprintf (file,
	   "====== Beginning of Nodes ================================\n");
  fprintf (file, "Nodes:\n");
  fprintf (file, "NodeQty = %d\n", Nodes.NodeQty);

  if (Nodes.NodeQty > 0)
  {
    fprintf (file, "NodeCoo =\n");
    ut_array_2d_fprintf (file, Nodes.NodeCoo + 1, Nodes.NodeQty, 3, "%.15f");
    fprintf (file, "NodeCl =\n");
    if (Nodes.NodeCl == NULL)
      fprintf (file, "null\n");
    else
      ut_array_1d_fprintf (file, Nodes.NodeCl + 1, Nodes.NodeQty, "%f\n");
  }

  fprintf (file, "Nodes.PerNodeQty = %d\n", Nodes.PerNodeQty);
  if (Nodes.PerNodeQty > 0)
  {
    int i, id;
    fprintf (file, "PerNodeNb = \n");
    ut_array_1d_int_fprintf (file, Nodes.PerNodeNb + 1,
			     Nodes.PerNodeQty, "%d");
    fprintf (file, "PerNodeNb PerNodeMaster PerNodeShift\n");
    for (i = 1; i <= Nodes.PerNodeQty; i++)
    {
      id = Nodes.PerNodeNb[i];
      fprintf (file, "%d %d %d %d %d\n", id, Nodes.PerNodeMaster[id],
					     Nodes.PerNodeShift[id][0],
					     Nodes.PerNodeShift[id][1],
					     Nodes.PerNodeShift[id][2]);
    }
  }

  fprintf (file, "Nodes.DupNodeQty = %d\n", Nodes.DupNodeQty);
  if (Nodes.DupNodeQty > 0)
  {
    int i;
    fprintf (file, "DupNodeNb = \n");
    ut_array_1d_int_fprintf (file, Nodes.DupNodeNb + 1,
			     Nodes.DupNodeQty, "%d");
    fprintf (file, "DupNodeNb DupNodeMaster DupNodeSeed\n");
    for (i = Nodes.NodeQty - Nodes.DupNodeQty + 1; i <= Nodes.NodeQty; i++)
      fprintf (file, "%d %d %d\n", i, Nodes.DupNodeMaster[i],
				      Nodes.DupNodeSeed[i]);
  }

  fprintf (file,
	   "====== End of Nodes ======================================\n");

  return;
}

void
neut_debug_nodes_name (char *filename, struct NODES Nodes)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_nodes (file, Nodes);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_mesh (FILE * file, struct MESH Mesh)
{
  int i;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  fprintf (file,
	   "====== Beginning of Mesh =================================\n");
  fprintf (file, "Mesh:\n");
  fprintf (file, "Dimension = %d\n", Mesh.Dimension);
  fprintf (file, "EltOrder = %d\n", Mesh.EltOrder);
  fprintf (file, "EltType = %s\n", Mesh.EltType);
  fprintf (file, "EltQty = %d\n", Mesh.EltQty);
  if (Mesh.EltQty > 0)
  {
    fprintf (file, "EltNodes = \n");
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Mesh.EltNodes[i], eltnodeqty, "%d");
    }
    fprintf (file, "EltElset = \n");
    if (Mesh.EltElset != NULL)
      for (i = 1; i <= Mesh.EltQty; i++)
	fprintf (file, "%d\n", Mesh.EltElset[i]);
    else
      fprintf (file, "(null)\n");
    fprintf (file, "ElsetQty = %d\n", Mesh.ElsetQty);
    fprintf (file, "Elsets = (quantity then ids of elements)\n");
    for (i = 1; i <= Mesh.ElsetQty; i++)
      ut_array_1d_int_fprintf (file, Mesh.Elsets[i], Mesh.Elsets[i][0] + 1,
			       "%d");
  }

  fprintf (file, "NodeElts = \n");
  if (Mesh.NodeElts == NULL)
    fprintf (file, "is NULL\n");
  else
  {
    fprintf (file, "eltid nodeids\n");
    for (i = 1; i <= Mesh.NodeQty; i++)
    {
      fprintf (file, "%2d  ", i);
      ut_array_1d_int_fprintf (file, Mesh.NodeElts[i] + 1,
			       Mesh.NodeElts[i][0], "%d");
    }
  }

  fprintf (file,
	   "====== End of Mesh =======================================\n");
  return;
}

void
neut_debug_mesh_name (char *filename, struct MESH Mesh)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_mesh (file, Mesh);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_nset (FILE * file, struct NSET NSet)
{
  int i;

  fprintf (file,
	   "====== Beginning of NSet =================================\n");
  fprintf (file, "NSet:\n");
  fprintf (file, "qty = %d\n", NSet.qty);

  fprintf (file, "names = \n");
  for (i = 1; i <= NSet.qty; i++)
    fprintf (file, "%s\n", NSet.names[i]);

  fprintf (file, "nodeqty = \n");
  for (i = 1; i <= NSet.qty; i++)
    fprintf (file, "%d\n", NSet.nodeqty[i]);

  fprintf (file, "nodes = \n");
  for (i = 1; i <= NSet.qty; i++)
    ut_array_1d_int_fprintf (file, NSet.nodes[i], NSet.nodeqty[i], "%d");

  fprintf (file,
	   "====== End of NSet =======================================\n");
  return;
}

void
neut_debug_nset_name (char *filename, struct NSET NSet)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_nset (file, NSet);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_tess (FILE * file, struct TESS Tess)
{
  int i;

  fprintf (file,
	   "====== Beginning of Tess ==================================\n");
  fprintf (file, "Dim = %d\n", Tess.Dim);
  fprintf (file, "Level = %d\n", Tess.Level);
  fprintf (file, "TessId = %d\n", Tess.TessId);

  fprintf (file, "CellQty = %d\n", Tess.CellQty);

  fprintf (file, "Type = %s\n", Tess.Type);

  fprintf (file, "VerQty = %d\n", Tess.VerQty);
  fprintf (file, "EdgeQty = %d\n", Tess.EdgeQty);
  fprintf (file, "FaceQty = %d\n", Tess.FaceQty);
  fprintf (file, "PolyQty = %d\n", Tess.PolyQty);

  fprintf (file, "== Cells =================\n");

  fprintf (file, "CellQty = %d\n", Tess.CellQty);

  fprintf (file, "CellId =\n");
  if (Tess.CellId)
    ut_array_1d_int_fprintf (file, Tess.CellId + 1, Tess.CellQty, "%d");
  else
    fprintf (file, "is NULL\n");

  fprintf (file, "[id] CellModeId =\n");
  if (Tess.CellModeId == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
      fprintf (file, "%d %d\n", i, Tess.CellModeId[i]);

  fprintf (file, "[id] CellLamId =\n");
  if (Tess.CellLamId == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
      fprintf (file, "%d %d\n", i, Tess.CellLamId[i]);

  fprintf (file, "CellOri =\n");
  if (Tess.CellOri)
    ut_array_2d_fprintf (file, Tess.CellOri + 1, Tess.CellQty, 4, "%f");
  else
    fprintf (file, "is NULL\n");

  fprintf (file, "[id] CellTrue =\n");
  if (Tess.CellTrue == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
      fprintf (file, "%d %d\n", i, Tess.CellTrue[i]);

  fprintf (file, "[id] CellBody =\n");
  if (Tess.CellBody == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
      fprintf (file, "%d %d\n", i, Tess.CellBody[i]);

  fprintf (file, "CellCrySym = %s\n", Tess.CellCrySym);

  fprintf (file, "== Seeds =================\n");

  fprintf (file, "SeedQty = %d\n", Tess.SeedQty);

  fprintf (file, "[id] SeedCoo =\n");
  if (Tess.SeedCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Tess.SeedCoo[i], 3, "%f");
    }

  fprintf (file, "[id] SeedWeight =\n");
  if (Tess.SeedWeight == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, "%d ", i);
      fprintf (file, "%f\n", Tess.SeedWeight[i]);
    }

  fprintf (file, "== Vertices =================\n");
  if (Tess.VerQty > 0)
  {
    fprintf (file, "[id] VerCoo =\n");
    if (Tess.VerCoo == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.VerQty; i++)
      {
	fprintf (file, "%d ", i);
	ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, "%f");
      }

    fprintf (file, "[id] VerEdgeQty then VerEdgeNb =\n");
    if (Tess.VerEdgeQty == NULL || Tess.VerEdgeNb == NULL)
      fprintf (file, "one is NULL\n");
    else
      for (i = 1; i <= Tess.VerQty; i++)
      {
	fprintf (file, "%d %d ", i, Tess.VerEdgeQty[i]);
	ut_array_1d_int_fprintf (file, Tess.VerEdgeNb[i], Tess.VerEdgeQty[i],
				 "%d");
      }

    fprintf (file, "[id] VerDom =\n");
    if (Tess.VerDom == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.VerQty; i++)
      {
	fprintf (file, "%d ", i);
	ut_array_1d_int_fprintf (file, Tess.VerDom[i], 2, "%d");
      }

    fprintf (file, "[id] VerState =\n");
    if (Tess.VerState == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.VerQty; i++)
	fprintf (file, "%d %d\n", i, Tess.VerState[i]);
  }

  fprintf (file, "== Edges =================\n");

  if (Tess.EdgeQty > 0)
  {
    fprintf (file, "[id] EdgeVerNb =\n");
    if (Tess.EdgeVerNb == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.EdgeQty; i++)
	fprintf (file, "%d %d %d\n", i, Tess.EdgeVerNb[i][0],
		 Tess.EdgeVerNb[i][1]);

    fprintf (file, "[id] EdgeFaceQty then EdgeFaceNb =\n");
    if (Tess.EdgeFaceQty == NULL || Tess.EdgeFaceNb == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.EdgeQty; i++)
      {
	fprintf (file, "%d %d ", i, Tess.EdgeFaceQty[i]);
	ut_array_1d_int_fprintf (file, Tess.EdgeFaceNb[i],
				 Tess.EdgeFaceQty[i], "%d");
      }

    fprintf (file, "[id] EdgeLength =\n");
    if (Tess.EdgeLength == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.EdgeQty; i++)
	fprintf (file, "%d %f\n", i, Tess.EdgeLength[i]);

    fprintf (file, "[id] EdgeState =\n");
    if (Tess.EdgeState == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.EdgeQty; i++)
	fprintf (file, "%d %d\n", i, Tess.EdgeState[i]);

    fprintf (file, "[id] EdgeDel =\n");
    if (Tess.EdgeDel == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.EdgeQty; i++)
	fprintf (file, "%d %d\n", i, Tess.EdgeDel[i]);

    fprintf (file, "[id] EdgeDom =\n");
    if (Tess.EdgeDom == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.EdgeQty; i++)
      {
	fprintf (file, "%d ", i);
	ut_array_1d_int_fprintf (file, Tess.EdgeDom[i], 2, "%d");
      }
  }

  fprintf (file, "== Faces =================\n");

  if (Tess.FaceQty > 0)
  {
    fprintf (file, "[id] FacePoly =\n");
    if (Tess.FacePoly == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
	fprintf (file, "%d %d %d\n", i, Tess.FacePoly[i][0],
		 Tess.FacePoly[i][1]);

    fprintf (file, "[id] FaceVerQty then FaceVerNb =\n");
    if (Tess.FaceVerQty == NULL || Tess.FaceVerNb == NULL)
      fprintf (file, "one is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
      {
	fprintf (file, "%d %d ", i, Tess.FaceVerQty[i]);
	ut_array_1d_int_fprintf (file, Tess.FaceVerNb[i] + 1,
				 Tess.FaceVerQty[i], "%d");
      }

    fprintf (file, "[id] FaceEdgeQty then FaceEdgeNb =\n");
    if (Tess.FaceVerQty == NULL || Tess.FaceEdgeNb == NULL)
      fprintf (file, "one is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
      {
	fprintf (file, "%d %d ", i, Tess.FaceVerQty[i]);
	ut_array_1d_int_fprintf (file, Tess.FaceEdgeNb[i] + 1,
				 Tess.FaceVerQty[i], "%d");
      }

    fprintf (file, "[id] FaceEdgeQty then Ori =\n");
    if (Tess.FaceVerQty == NULL || Tess.FaceEdgeOri == NULL)
      fprintf (file, "one is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
      {
	fprintf (file, "%d %d ", i, Tess.FaceVerQty[i]);
	ut_array_1d_int_fprintf (file, Tess.FaceEdgeOri[i] + 1,
				 Tess.FaceVerQty[i], "%d");
      }

    fprintf (file, "[id] FaceEq =\n");
    if (Tess.FaceEq == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
	ut_array_1d_fprintf (file, Tess.FaceEq[i], 4, "%f");

    fprintf (file, "[id] FaceState =\n");
    if (Tess.FaceState == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
	fprintf (file, "%d %d\n", i, Tess.FaceState[i]);

    fprintf (file, "[id] FacePt =\n");
    if (Tess.FacePt == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
	fprintf (file, "%d %d\n", i, Tess.FacePt[i]);

    fprintf (file, "[id] FacePtCoo =\n");
    if (Tess.FacePtCoo == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
      {
	fprintf (file, "%d ", i);
	ut_array_1d_fprintf (file, Tess.FacePtCoo[i], 3, "%f");
      }

    fprintf (file, "[id] FaceDom =\n");
    if (Tess.FaceDom == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.FaceQty; i++)
      {
	fprintf (file, "%d ", i);
	ut_array_1d_int_fprintf (file, Tess.FaceDom[i], 2, "%d");
      }
  }

  fprintf (file, "== Polys =================\n");

  if (Tess.PolyQty > 0)
  {
    fprintf (file, "[id] PolyFaceQty then PolyFaceNb =\n");
    if (Tess.PolyFaceQty == NULL || Tess.PolyFaceNb == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.PolyQty; i++)
      {
	fprintf (file, "%d: %d ", i, Tess.PolyFaceQty[i]);
	ut_array_1d_int_fprintf (file, Tess.PolyFaceNb[i] + 1,
				 Tess.PolyFaceQty[i], "%d");
      }

    fprintf (file, "[id] PolyFaceQty then PolyFaceOri =\n");
    if (Tess.PolyFaceQty == NULL || Tess.PolyFaceOri == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.PolyQty; i++)
      {
	fprintf (file, "%d: %d ", i, Tess.PolyFaceQty[i]);
	ut_array_1d_int_fprintf (file, Tess.PolyFaceOri[i] + 1,
				 Tess.PolyFaceQty[i], "%d");
      }

    fprintf (file, "[id] PolyState =\n");
    if (Tess.PolyState == NULL)
      fprintf (file, "is NULL\n");
    else
      for (i = 1; i <= Tess.PolyQty; i++)
	fprintf (file, "%d %d\n", i, Tess.PolyState[i]);
  }

  fprintf (file, "== Domain =================\n");

  // Domain
  if (Tess.DomType != NULL)
    fprintf (file, "DomType = %s\n", Tess.DomType);

  // Domain ver
  fprintf (file, "DomVerQty = %d\n", Tess.DomVerQty);
  fflush (file);

  fprintf (file, "[id] DomVerCoo =\n");
  if (Tess.DomVerCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.DomVerQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Tess.DomVerCoo[i], 3, "%f");
    }

  fprintf (file, "[id] DomVerLabel =\n");
  if (Tess.DomVerLabel == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.DomVerQty; i++)
    {
      fprintf (file, "%d ", i);
      fprintf (file, "%s\n", Tess.DomVerLabel[i]);
    }

  fprintf (file, "[id] DomVerEdgeQty then DomVerEdgeNb\n");
  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    fprintf (file, "%d ", i);
    if (Tess.DomVerEdgeQty)
    {
      fprintf (file, "%d ", Tess.DomVerEdgeQty[i]);
      if (Tess.DomVerEdgeQty[i] > 0)
	ut_array_1d_int_fprintf (file, Tess.DomVerEdgeNb[i],
				 Tess.DomVerEdgeQty[i], "%d");
    }
    else
      fprintf (file, "\n");
  }

  fprintf (file, "[id] DomTessVerNb\n");
  for (i = 1; i <= Tess.DomVerQty; i++)
    fprintf (file, "%d %d\n", i, Tess.DomTessVerNb[i]);

  // Domain edge
  fprintf (file, "DomEdgeQty = %d\n", Tess.DomEdgeQty);
  fflush (file);

  fprintf (file, "[id] DomEdgeLabel =\n");
  if (Tess.DomEdgeLabel == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      fprintf (file, "%d ", i);
      fprintf (file, "%s\n", Tess.DomEdgeLabel[i]);
    }

  fprintf (file, "[id] DomEdgeVerNb\n");
  if (Tess.DomEdgeVerNb)
    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Tess.DomEdgeVerNb[i], 2, "%d");
    }

  fprintf (file, "[id] DomEdgeFaceNb\n");
  if (Tess.DomEdgeFaceNb)
    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Tess.DomEdgeFaceNb[i], 2, "%d");
    }

  fprintf (file, "[id] DomTessEdgeQty then DomTessEdgeNb\n");
  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    fprintf (file, "%d %d ", i, Tess.DomTessEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomTessEdgeNb[i] + 1,
			     Tess.DomTessEdgeQty[i], "%d");
  }

  // Domain face
  fprintf (file, "DomFaceQty = %d\n", Tess.DomFaceQty);
  fflush (file);

  fprintf (file, "[id] DomFaceLabel =\n");
  if (Tess.DomFaceLabel == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, "%d ", i);
      fprintf (file, "%s\n", Tess.DomFaceLabel[i]);
    }

  fprintf (file, "DomFaceEq =\n");
  if (Tess.DomFaceEq != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
      ut_array_1d_fprintf (file, Tess.DomFaceEq[i], 4, "%f");
  else
    fprintf (file, "is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomFaceVerQty then DomFaceVerNb =\n");
  if (Tess.DomFaceVerQty != NULL && Tess.DomFaceVerNb != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.DomFaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomFaceVerNb[i] + 1,
			       Tess.DomFaceVerQty[i], "%d");
    }
  else
    fprintf (file, "is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomFaceVerQty then DomFaceEdgeNb =\n");
  if (Tess.DomFaceVerQty != NULL && Tess.DomFaceEdgeNb != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.DomFaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomFaceEdgeNb[i] + 1,
			       Tess.DomFaceVerQty[i], "%d");
    }
  else
    fprintf (file, "is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomTessFaceQty then DomTessFaceNb =\n");
  if (Tess.DomTessFaceQty != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.DomTessFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomTessFaceNb[i] + 1,
			       Tess.DomTessFaceQty[i], "%d");
    }
  else
    fprintf (file, "is NULL\n");
  fflush (file);

  fprintf (file, "== Periodicity =================\n");

  fprintf (file, "Periodic = ");
  if (Tess.Periodic)
    ut_array_1d_int_fprintf (file, Tess.Periodic, 3, "%d");
  fprintf (file, "NULL\n");

  fprintf (file, "PeriodicDist = ");
  if (Tess.PeriodicDist)
    ut_array_1d_fprintf (file, Tess.PeriodicDist, 3, "%f");
  fprintf (file, "NULL\n");

  // PerSeed

  fprintf (file, "PerSeedQty = %d\n", Tess.PerSeedQty);

  fprintf (file, "id | PerSeedMaster | PerSeedShift =\n");
  for (i = 1; i <= Tess.SeedQty; i++)
    fprintf (file, "%2d %2d %2d %2d %2d\n",
      i,
      Tess.PerSeedMaster ? Tess.PerSeedMaster[i] : -1,
      Tess.PerSeedShift ? Tess.PerSeedShift[i][0] : -2,
      Tess.PerSeedShift ? Tess.PerSeedShift[i][1] : -2,
      Tess.PerSeedShift ? Tess.PerSeedShift[i][2] : -2);

  fprintf (file, "id | PerSeedSlave =\n");
  if (Tess.PerSeedSlave)
    for (i = 1; i <= Tess.SeedQty; i++)
    {
      fprintf (file, "%2d ", i);
      ut_array_1d_int_fprintf (file, Tess.PerSeedSlave[i], 27, "%d");
    }

  // PerVer

  fprintf (file, "PerVerQty = %d\n", Tess.PerVerQty);

  fprintf (file, "PerVerNb = \n");
  ut_array_1d_int_fprintf (file, Tess.PerVerNb + 1, Tess.PerVerQty, "%d");

  fprintf (file, "id | PerVerMaster | PerVerShift =\n");
  for (i = 1; i <= Tess.VerQty; i++)
    fprintf (file, "%2d %2d %2d %2d %2d\n",
      i,
      Tess.PerVerMaster ? Tess.PerVerMaster[i] : -1,
      Tess.PerVerShift ? Tess.PerVerShift[i][0] : -2,
      Tess.PerVerShift ? Tess.PerVerShift[i][1] : -2,
      Tess.PerVerShift ? Tess.PerVerShift[i][2] : -2);

  fprintf (file, "id | PerVerSlaveQty | PerVerSlaveNb =\n");
  if (Tess.PerVerSlaveQty)
    for (i = 1; i <= Tess.VerQty; i++)
    {
      fprintf (file, "%2d %2d ", i, Tess.PerVerSlaveQty[i]);
      ut_array_1d_int_fprintf (file, Tess.PerVerSlaveNb[i] + 1,
			       Tess.PerVerSlaveQty[i], "%2d");
    }
  else
    fprintf (file, "NULL\n");

  // PerEdge

  fprintf (file, "PerEdgeQty = %d\n", Tess.PerEdgeQty);

  fprintf (file, "PerEdgeNb = \n");
  ut_array_1d_int_fprintf (file, Tess.PerEdgeNb + 1, Tess.PerEdgeQty, "%d");

  fprintf (file, "id | PerEdgeMaster | PerEdgeShift | PerEdgeOri =\n");
  for (i = 1; i <= Tess.EdgeQty; i++)
    fprintf (file, "%2d %2d %2d %2d %2d %2d\n",
      i,
      Tess.PerEdgeMaster ? Tess.PerEdgeMaster[i] : -1,
      Tess.PerEdgeShift ? Tess.PerEdgeShift[i][0] : -2,
      Tess.PerEdgeShift ? Tess.PerEdgeShift[i][1] : -2,
      Tess.PerEdgeShift ? Tess.PerEdgeShift[i][2] : -2,
      Tess.PerEdgeOri ? Tess.PerEdgeOri[i] : -2);

  fprintf (file, "id | PerEdgeSlaveQty | PerEdgeSlaveNb =\n");
  if (Tess.PerEdgeSlaveQty)
    for (i = 1; i <= Tess.EdgeQty; i++)
    {
      fprintf (file, "%2d %2d ", i, Tess.PerEdgeSlaveQty[i]);
      ut_array_1d_int_fprintf (file, Tess.PerEdgeSlaveNb[i] + 1,
			       Tess.PerEdgeSlaveQty[i], "%2d");
    }

  // PerFace

  fprintf (file, "PerFaceQty = %d\n", Tess.PerFaceQty);

  fprintf (file, "PerFaceNb = \n");
  ut_array_1d_int_fprintf (file, Tess.PerFaceNb + 1, Tess.PerFaceQty, "%d");

  fprintf (file, "id | PerFaceMaster | PerFaceShift | PerFaceOri =\n");
  for (i = 1; i <= Tess.FaceQty; i++)
    fprintf (file, "%2d %2d %2d %2d %2d %2d\n",
      i,
      Tess.PerFaceMaster ? Tess.PerFaceMaster[i] : -1,
      Tess.PerFaceShift ? Tess.PerFaceShift[i][0] : -2,
      Tess.PerFaceShift ? Tess.PerFaceShift[i][1] : -2,
      Tess.PerFaceShift ? Tess.PerFaceShift[i][2] : -2,
      Tess.PerFaceOri ? Tess.PerFaceOri[i] : -2);

  fprintf (file, "id | PerFaceSlaveNb =\n");
  if (Tess.PerFaceSlaveNb)
    for (i = 1; i <= Tess.FaceQty; i++)
      fprintf (file, "%2d %2d\n", i, Tess.PerFaceSlaveNb[i]);

  fprintf (file,
	   "====== End of Tess ========================================\n");
  fflush (file);

  return;
}

void
neut_debug_tess_name (char *filename, struct TESS Tess)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_tess (file, Tess);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_seedset (FILE * file, struct SEEDSET SSet)
{
  int i;

  fprintf (file, "N = %d\n", SSet.N);
  fprintf (file, "Nall = %d\n", SSet.Nall);
  fprintf (file, "Id = %d\n", SSet.Id);
  fprintf (file, "Random = %ld\n", SSet.Random);

  fprintf (file, "SeedCoo0 =\n");
  if (!SSet.SeedCoo0)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.N; i++)
      ut_array_1d_fprintf (file, SSet.SeedCoo0[i], 3, "%f");

  fprintf (file, "SeedCoo =\n");
  if (!SSet.SeedCoo)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.Nall; i++)
      ut_array_1d_fprintf (file, SSet.SeedCoo[i], 3, "%f");

  fprintf (file, "SeedWeight =\n");
  if (!SSet.SeedWeight)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.Nall; i++)
      fprintf (file, "%f\n", SSet.SeedWeight[i]);

  fprintf (file, "q =\n");
  if (!SSet.q)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.Nall; i++)
      ut_array_1d_fprintf (file, SSet.q[i], 4, "%f");

  fprintf (file, "LamEq = ");
  if (SSet.LamEq == NULL)
    fprintf (file, "(null)\n");
  else
    ut_array_2d_fprintf (file, SSet.LamEq + 1, SSet.N, 4, "%f");

  fprintf (file, "LamWidth = ");
  if (SSet.LamEq == NULL)
    fprintf (file, "(null)\n");
  else
    ut_array_1d_fprintf (file, SSet.LamWidth + 1, SSet.N, "%f");

  fprintf (file, "Periodic =\n");
  if (!SSet.Periodic)
    fprintf (file, "is NULL.\n");
  else
    ut_array_1d_int_fprintf (file, SSet.Periodic, 3, "%d");

  fprintf (file, "PeriodicDist =\n");
  if (!SSet.PeriodicDist)
    fprintf (file, "is NULL.\n");
  else
    ut_array_1d_fprintf (file, SSet.PeriodicDist, 3, "%f");

  fprintf (file, "Size =\n");
  if (!SSet.Size)
    fprintf (file, "is NULL.\n");
  else
    ut_array_2d_fprintf (file, SSet.Size, 3, 2, "%f");

  return;
}

void
neut_debug_seedset_name (char *filename, struct SEEDSET SSet)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_seedset (file, SSet);
  ut_file_close (file, filename, "w");

  return;
}

/*
void
neut_debug_tessdata (FILE* file, struct TESSDATA TessData)
{
  int i;

  // Poly stuff
  fprintf (file, "polydataqty = %d\n", TessData.polydataqty);

  if (TessData.polydatatype != NULL)
  {
    fprintf (file, "polydatatype =\n");
    for (i = 0; i < TessData.polydataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.polydatatype[i]);
  }
  else
    fprintf (file, "polydatatype = NULL\n");

  if (TessData.polydatasize != NULL)
  {
    fprintf (file, "polydatasize =\n");
    for (i = 0; i < TessData.polydataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.polydatasize[i][0], TessData.polydatasize[i][1]);
  }
  else
    fprintf (file, "polydatasize = NULL\n");

  if (TessData.polydata != NULL)
  {
    for (i = 0; i < TessData.polydataqty; i++)
    {
      fprintf (file, "polydata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.polydata[i] + 1,
			   TessData.polydatasize[i][0], TessData.polydatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "polydata = NULL\n");

  if (TessData.polycol != NULL)
  {
    fprintf (file, "polycol =\n");
    ut_array_2d_int_fprintf (file, TessData.polycol + 1,
                             TessData.polyqty, 3, "%d");
  }
  else
    fprintf (file, "polycol = NULL\n");

  if (TessData.polycolourscheme != NULL)
    fprintf (file, "polycolourscheme = %s\n", TessData.polycolourscheme);
  else
    fprintf (file, "polycolourscheme = NULL\n");

  // Face stuff
  fprintf (file, "facedataqty = %d\n", TessData.facedataqty);

  if (TessData.facedatatype != NULL)
  {
    fprintf (file, "facedatatype =\n");
    for (i = 0; i < TessData.facedataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.facedatatype[i]);
  }
  else
    fprintf (file, "facedatatype = NULL\n");

  if (TessData.facedatasize != NULL)
  {
    fprintf (file, "facedatasize =\n");
    for (i = 0; i < TessData.facedataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.facedatasize[i][0], TessData.facedatasize[i][1]);
  }
  else
    fprintf (file, "facedatasize = NULL\n");

  if (TessData.facedata != NULL)
  {
    for (i = 0; i < TessData.facedataqty; i++)
    {
      fprintf (file, "facedata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.facedata[i] + 1,
			   TessData.facedatasize[i][0], TessData.facedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "facedata = NULL\n");

  if (TessData.facewidth != NULL)
  {
    fprintf (file, "facewidth =\n");
    ut_array_1d_fprintf (file, TessData.facewidth + 1,
			 TessData.faceqty, "%f");
  }
  else
    fprintf (file, "facewidth = NULL\n");

  if (TessData.facecol != NULL)
  {
    fprintf (file, "facecol =\n");
    ut_array_2d_int_fprintf (file, TessData.facecol + 1,
                             TessData.faceqty, 3, "%d");
  }
  else
    fprintf (file, "facecol = NULL\n");

  if (TessData.facecolourscheme != NULL)
    fprintf (file, "facecolourscheme = %s\n", TessData.facecolourscheme);
  else
    fprintf (file, "facecolourscheme = NULL\n");

  // Edge stuff
  fprintf (file, "edgedataqty = %d\n", TessData.edgedataqty);

  if (TessData.edgedatatype != NULL)
  {
    fprintf (file, "edgedatatype =\n");
    for (i = 0; i < TessData.edgedataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.edgedatatype[i]);
  }
  else
    fprintf (file, "edgedatatype = NULL\n");

  if (TessData.edgedatasize != NULL)
  {
    fprintf (file, "edgedatasize =\n");
    for (i = 0; i < TessData.edgedataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.edgedatasize[i][0], TessData.edgedatasize[i][1]);
  }
  else
    fprintf (file, "edgedatasize = NULL\n");

  if (TessData.edgedata != NULL)
  {
    for (i = 0; i < TessData.edgedataqty; i++)
    {
      fprintf (file, "edgedata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.edgedata[i] + 1,
			   TessData.edgedatasize[i][0], TessData.edgedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "edgedata = NULL\n");

  if (TessData.edgerad != NULL)
  {
    fprintf (file, "edgerad =\n");
    ut_array_1d_fprintf (file, TessData.edgerad + 1,
			 TessData.edgeqty, "%f");
  }
  else
    fprintf (file, "edgerad = NULL\n");

  if (TessData.edgecol != NULL)
  {
    fprintf (file, "edgecol =\n");
    ut_array_2d_int_fprintf (file, TessData.edgecol + 1,
                             TessData.edgeqty, 3, "%d");
  }
  else
    fprintf (file, "edgecol = NULL\n");

  if (TessData.edgecolourscheme != NULL)
    fprintf (file, "edgecolourscheme = %s\n", TessData.edgecolourscheme);
  else
    fprintf (file, "edgecolourscheme = NULL\n");

  // Ver stuff
  fprintf (file, "verdataqty = %d\n", TessData.edgedataqty);

  if (TessData.verdatatype != NULL)
  {
    fprintf (file, "verdatatype =\n");
    for (i = 0; i < TessData.verdataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.verdatatype[i]);
  }
  else
    fprintf (file, "verdatatype = NULL\n");

  if (TessData.verdatasize != NULL)
  {
    fprintf (file, "verdatasize =\n");
    for (i = 0; i < TessData.verdataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.verdatasize[i][0], TessData.edgedatasize[i][1]);
  }
  else
    fprintf (file, "verdatasize = NULL\n");

  if (TessData.verdata != NULL)
  {
    for (i = 0; i < TessData.verdataqty; i++)
    {
      fprintf (file, "verdata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.verdata[i] + 1,
			   TessData.verdatasize[i][0], TessData.edgedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "verdata = NULL\n");

  if (TessData.verrad != NULL)
  {
    fprintf (file, "verrad =\n");
    ut_array_1d_fprintf (file, TessData.verrad + 1,
			 TessData.verqty, "%f");
  }
  else
    fprintf (file, "verrad = NULL\n");

  if (TessData.vercol != NULL)
  {
    fprintf (file, "vercol =\n");
    ut_array_2d_int_fprintf (file, TessData.vercol + 1,
                             TessData.verqty, 3, "%d");
  }
  else
    fprintf (file, "vercol = NULL\n");

  if (TessData.vercolourscheme != NULL)
    fprintf (file, "vercolourscheme = %s\n", TessData.edgecolourscheme);
  else
    fprintf (file, "vercolourscheme = NULL\n");

  return;
}
*/

void
neut_debug_poly (FILE * file, struct POLY Poly)
{
  int i;

  fprintf (file,
	   "====== Beginning of Poly ================================\n");

  fprintf (file, "VerQty = %d\n", Poly.VerQty);
  fprintf (file, "FaceQty = %d\n", Poly.FaceQty);

  fprintf (file, "VerFace = \n");
  ut_array_2d_int_fprintf (file, Poly.VerFace + 1, Poly.VerQty, 3, "%d");

  fprintf (file, "VerCoo = \n");
  ut_array_2d_fprintf (file, Poly.VerCoo + 1, Poly.VerQty, 3, "%f");

  fprintf (file, "FacePoly = \n");
  ut_array_1d_int_fprintf (file, Poly.FacePoly + 1, Poly.FaceQty, "%d");

  fprintf (file, "FaceEq = \n");
  ut_array_2d_fprintf (file, Poly.FaceEq + 1, Poly.FaceQty, 4, "%f");

  fprintf (file, "FaceVerQty: FaceVerNb = \n");
  for (i = 1; i <= Poly.FaceQty; i++)
  {
    fprintf (file, "%d: ", Poly.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Poly.FaceVerNb[i] + 1, Poly.FaceVerQty[i],
			     "%d");
  }

  fprintf (file,
	   "====== End of Poly ======================================\n");

  return;
}

void
neut_debug_poly_name (char *filename, struct POLY Poly)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_poly (file, Poly);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_polymod (FILE * file, struct POLYMOD Polymod)
{
  int i;

  fprintf (file,
	   "====== Beginning of Polymod =============================\n");

  fprintf (file, "VerQty = %d\n", Polymod.VerQty);
  fprintf (file, "FaceQty = %d\n", Polymod.FaceQty);

  fprintf (file, "VerUse = \n");
  ut_array_1d_int_fprintf (file, Polymod.VerUse + 1, Polymod.VerQty, "%d");

  fprintf (file, "VerCoo = \n");
  ut_array_2d_fprintf (file, Polymod.VerCoo + 1, Polymod.VerQty, 3, "%f");

  fprintf (file, "VerFace = \n");
  ut_array_2d_int_fprintf (file, Polymod.VerFace + 1, Polymod.VerQty, 3,
			   "%d");

  fprintf (file, "FaceUse = \n");
  ut_array_1d_int_fprintf (file, Polymod.FaceUse + 1, Polymod.FaceQty, "%d");

  fprintf (file, "FacePoly = \n");
  ut_array_1d_int_fprintf (file, Polymod.FacePoly + 1, Polymod.FaceQty, "%d");

  fprintf (file, "FaceEq = \n");
  ut_array_2d_fprintf (file, Polymod.FaceEq + 1, Polymod.FaceQty, 4, "%f");

  fprintf (file, "FaceVerQty: FaceVerNb = \n");
  for (i = 1; i <= Polymod.FaceQty; i++)
  {
    fprintf (file, "%d: ", Polymod.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Polymod.FaceVerNb[i] + 1,
			     Polymod.FaceVerQty[i], "%d");
  }

  fprintf (file,
	   "====== End of Polymod ===================================\n");

  return;
}

void
neut_debug_polymod_name (char *filename, struct POLYMOD Polymod)
{
  FILE *file = ut_file_open (filename, "w");
  neut_debug_polymod (file, Polymod);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_meshdata (FILE * file, struct MESHDATA MeshData)
{
  fprintf (file, "EltQty = %d\n", MeshData.EltQty);
  fprintf (file, "Dimension = %d\n", MeshData.Dimension);

  fprintf (file, "ColDataType = %s\n", MeshData.ColDataType);

  fprintf (file, "ColData = \n");
  if (MeshData.ColData)
  {
    if (!strcmp (MeshData.ColDataType, "col")
	|| !strncmp (MeshData.ColDataType, "ori", 3))
      ut_array_2d_fprintf (file, MeshData.ColData + 1, MeshData.EltQty, 3,
			   "%f");
    else if (!strcmp (MeshData.ColDataType, "rad")
	     || !strcmp (MeshData.ColDataType, "scal"))
      ut_array_2d_fprintf (file, MeshData.ColData + 1, MeshData.EltQty, 1,
			   "%f");
    else
      abort ();
  }

  fprintf (file, "Col = \n");
  if (MeshData.Col)
    ut_array_2d_int_fprintf (file, MeshData.Col + 1, MeshData.EltQty, 3,
			     "%d");

  fprintf (file, "ColScheme = %s\n", MeshData.ColScheme);

  fprintf (file, "RadDataType = %s\n", MeshData.RadDataType);

  fprintf (file, "RadData = \n");
  if (MeshData.RadData)
    ut_array_2d_fprintf (file, MeshData.RadData + 1, MeshData.EltQty, 3,
			 "%f");

  fprintf (file, "Rad = \n");
  if (MeshData.Rad)
    ut_array_1d_fprintf (file, MeshData.Rad + 1, MeshData.EltQty, "%f\n");

  fprintf (file, "Scale = %s\n", MeshData.Scale);

  fprintf (file, "BCol = \n");
  if (MeshData.BCol)
    ut_array_1d_int_fprintf (file, MeshData.BCol, 3, "%d\n");

  fprintf (file, "BRad = %f\n", MeshData.BRad);

  return;
}

void
neut_debug_meshdata_name (char *filename, struct MESHDATA MeshData)
{
  FILE *file = ut_file_open (filename, "W");

  neut_debug_meshdata (file, MeshData);

  ut_file_close (file, filename, "W");

  return;
}

void
neut_debug_multim (FILE * file, struct MULTIM Multim)
{
  int i;

  fprintf (file, "algoqty = %d\n", Multim.algoqty);

  fprintf (file, "algos =\n");
  for (i = 0; i < Multim.algoqty; i++)
    fprintf (file, "%s %s\n", Multim.algos[i][0], Multim.algos[i][1]);

  fprintf (file, "algohit =\n");
  for (i = 0; i < Multim.algoqty; i++)
    fprintf (file, "%d\n", Multim.algohit[i]);

  return;
}

void
neut_debug_tesr (FILE * file, struct TESR Tesr)
{
  int i, j, k;

  fprintf (file, "Dim = %d\n", Tesr.Dim);
  fprintf (file, "CellQty = %d\n", Tesr.CellQty);

  fprintf (file, "CellId = ");
  if (Tesr.CellId)
    ut_array_1d_int_fprintf (file, Tesr.CellId + 1, Tesr.CellQty, "%d");
  else
    fprintf (file, "is NULL\n");

  fprintf (file, "CellOri =\n");
  if (Tesr.CellOri)
    ut_array_2d_fprintf (file, Tesr.CellOri + 1, Tesr.CellQty, 4, "%f");
  else
    fprintf (file, "is NULL\n");

  fprintf (file, "size = ");
  ut_array_1d_int_fprintf (file, Tesr.size, 3, "%d");
  fprintf (file, "vsize = ");
  ut_array_1d_fprintf (file, Tesr.vsize, 3, "%f");

  fprintf (file, "VoxCell = ");
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	fprintf (file, "%d ", Tesr.VoxCell[i][j][k]);
  fprintf (file, "\n");

  return;
}
