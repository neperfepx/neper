/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
    ut_array_1d_int_fprintf (file, Nodes.PerNodeNb + 1, Nodes.PerNodeQty,
                             "%d");
    fprintf (file, "PerNodeNb PerNodeMaster PerNodeShift\n");
    for (i = 1; i <= Nodes.PerNodeQty; i++)
    {
      id = Nodes.PerNodeNb[i];
      fprintf (file, "%d %d %d %d %d\n", id, Nodes.PerNodeMaster[id],
               Nodes.PerNodeShift[id][0], Nodes.PerNodeShift[id][1],
               Nodes.PerNodeShift[id][2]);
    }

    fprintf (file, "i PerNodeSlaveQty PerNodeSlaveNb\n");
    for (i = 1; i <= Nodes.PerNodeQty; i++)
    {
      fprintf (file, "%d %d ", i, Nodes.PerNodeSlaveQty[i]);
      ut_array_1d_int_fprintf (file, Nodes.PerNodeSlaveNb[i] + 1,
                               Nodes.PerNodeSlaveQty[i], "%d");
    }
  }

  fprintf (file, "Nodes.DupNodeQty = %d\n", Nodes.DupNodeQty);
  if (Nodes.DupNodeQty > 0)
  {
    int i;
    fprintf (file, "DupNodeNb = \n");
    ut_array_1d_int_fprintf (file, Nodes.DupNodeNb + 1, Nodes.DupNodeQty,
                             "%d");
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
    if (Mesh.Elsets)
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

  fprintf (file, "NodeQty = \n");
  for (i = 1; i <= NSet.qty; i++)
    fprintf (file, "%d\n", NSet.NodeQty[i]);

  fprintf (file, "nodes = \n");
  for (i = 1; i <= NSet.qty; i++)
    ut_array_1d_int_fprintf (file, NSet.nodes[i], NSet.NodeQty[i], "%d");

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
    ut_array_2d_fprintf (file, Tess.CellOri + 1, Tess.CellQty, 4, REAL_PRINT_FORMAT);
  else
    fprintf (file, "is NULL\n");

  /*
  fprintf (file, "[id] CellBody =\n");
  if (Tess.CellBody == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
      fprintf (file, "%d %d\n", i, Tess.CellBody[i]);
      */

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
      ut_array_1d_fprintf (file, Tess.SeedCoo[i], 3, REAL_PRINT_FORMAT);
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
        ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, REAL_PRINT_FORMAT);
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
        ut_array_1d_fprintf (file, Tess.FaceEq[i], 4, REAL_PRINT_FORMAT);

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
        ut_array_1d_fprintf (file, Tess.FacePtCoo[i], 3, REAL_PRINT_FORMAT);
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
      ut_array_1d_fprintf (file, Tess.DomVerCoo[i], 3, REAL_PRINT_FORMAT);
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
  if (!Tess.DomTessVerNb)
    printf ("is NULL\n");
  else
    for (i = 1; i <= Tess.DomVerQty; i++)
      fprintf (file, "%d %d\n", i, Tess.DomTessVerNb[i]);

  // Domain edge
  fprintf (file, "DomEdgeQty = %d\n", Tess.DomEdgeQty);
  fflush (file);

  fprintf (file, "[id] DomEdgeLabel =\n");
  if (!Tess.DomEdgeLabel)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      fprintf (file, "%d ", i);
      if (Tess.DomEdgeLabel[i])
        fprintf (file, "%s\n", Tess.DomEdgeLabel[i]);
      else
        fprintf (file, "NULL\n");
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
  if (!Tess.DomTessEdgeNb)
    printf ("is NULL\n");
  else
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
      ut_array_1d_fprintf (file, Tess.DomFaceEq[i], 4, REAL_PRINT_FORMAT);
  else
    fprintf (file, "is NULL\n");
  fflush (file);

  fprintf (file, "DomFaceType =\n");
  if (Tess.DomFaceType != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
      fprintf (file, "%s\n", Tess.DomFaceType[i]);
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

  fprintf (file, "[id] DomFaceEdgeQty then DomFaceEdgeNb =\n");
  if (Tess.DomFaceEdgeQty != NULL && Tess.DomFaceEdgeNb != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.DomFaceEdgeQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomFaceEdgeNb[i] + 1,
                               Tess.DomFaceEdgeQty[i], "%d");
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
    ut_array_1d_fprintf (file, Tess.PeriodicDist, 3, REAL_PRINT_FORMAT);
  fprintf (file, "NULL\n");

  // PerSeed

  fprintf (file, "PerSeedQty = %d\n", Tess.PerSeedQty);

  fprintf (file, "id | PerSeedMaster | PerSeedShift =\n");
  for (i = 1; i <= Tess.SeedQty; i++)
    fprintf (file, "%2d %2d %2d %2d %2d\n", i,
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
    fprintf (file, "%2d %2d %2d %2d %2d\n", i,
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
    fprintf (file, "%2d %2d %2d %2d %2d %2d\n", i,
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
    fprintf (file, "%2d %2d %2d %2d %2d %2d\n", i,
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
    {
      printf ("%d: ", i);
      fflush (file);
      ut_array_1d_fprintf (file, SSet.SeedCoo0[i], 3, REAL_PRINT_FORMAT);
    }

  fprintf (file, "SeedCoo =\n");
  if (!SSet.SeedCoo)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.Nall; i++)
      ut_array_1d_fprintf (file, SSet.SeedCoo[i], 3, REAL_PRINT_FORMAT);

  fprintf (file, "SeedWeight =\n");
  if (!SSet.SeedWeight)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.Nall; i++)
      fprintf (file, "%f\n", SSet.SeedWeight[i]);

  fprintf (file, "SeedOri =\n");
  if (!SSet.SeedOri)
    fprintf (file, "is NULL.\n");
  else
    for (i = 1; i <= SSet.N; i++)
      ut_array_1d_fprintf (file, SSet.SeedOri[i], 4, REAL_PRINT_FORMAT);

  fprintf (file, "LamEq = ");
  if (SSet.LamEq == NULL)
    fprintf (file, "(null)\n");
  else
    ut_array_2d_fprintf (file, SSet.LamEq + 1, SSet.N, 4, REAL_PRINT_FORMAT);

  fprintf (file, "LamWidth = ");
  if (SSet.LamEq == NULL)
    fprintf (file, "(null)\n");
  else
    ut_array_1d_fprintf (file, SSet.LamWidth + 1, SSet.N, REAL_PRINT_FORMAT);

  fprintf (file, "Periodic =\n");
  if (!SSet.Periodic)
    fprintf (file, "is NULL.\n");
  else
    ut_array_1d_int_fprintf (file, SSet.Periodic, 3, "%d");

  fprintf (file, "PeriodicDist =\n");
  if (!SSet.PeriodicDist)
    fprintf (file, "is NULL.\n");
  else
    ut_array_1d_fprintf (file, SSet.PeriodicDist, 3, REAL_PRINT_FORMAT);

  fprintf (file, "Size =\n");
  if (!SSet.Size)
    fprintf (file, "is NULL.\n");
  else
    ut_array_2d_fprintf (file, SSet.Size, 3, 2, REAL_PRINT_FORMAT);

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
  ut_array_2d_fprintf (file, Poly.VerCoo + 1, Poly.VerQty, 3, REAL_PRINT_FORMAT);

  fprintf (file, "FacePoly = \n");
  ut_array_1d_int_fprintf (file, Poly.FacePoly + 1, Poly.FaceQty, "%d");

  fprintf (file, "FaceEq = \n");
  ut_array_2d_fprintf (file, Poly.FaceEq + 1, Poly.FaceQty, 4, REAL_PRINT_FORMAT);

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
  ut_array_2d_fprintf (file, Polymod.VerCoo + 1, Polymod.VerQty, 3, REAL_PRINT_FORMAT);

  fprintf (file, "VerFace = \n");
  ut_array_2d_int_fprintf (file, Polymod.VerFace + 1, Polymod.VerQty, 3,
                           "%d");

  fprintf (file, "FaceUse = \n");
  ut_array_1d_int_fprintf (file, Polymod.FaceUse + 1, Polymod.FaceQty, "%d");

  fprintf (file, "FacePoly = \n");
  ut_array_1d_int_fprintf (file, Polymod.FacePoly + 1, Polymod.FaceQty, "%d");

  fprintf (file, "FaceEq = \n");
  ut_array_2d_fprintf (file, Polymod.FaceEq + 1, Polymod.FaceQty, 4, REAL_PRINT_FORMAT);

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
neut_debug_data (FILE * file, struct DATA Data)
{
  fprintf (file, "Qty = %d\n", Data.Qty);
  fprintf (file, "Dim = %d\n", Data.Dim);

  fprintf (file, "ColDataType = %s\n", Data.ColDataType);

  fprintf (file, "ColData = \n");
  if (Data.ColData)
  {
    if (!strcmp (Data.ColDataType, "col")
        || !strncmp (Data.ColDataType, "ori", 3))
      ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, 3,
                           REAL_PRINT_FORMAT);
    else if (!strcmp (Data.ColDataType, "rad")
             || !strcmp (Data.ColDataType, "real"))
      ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, 1,
                           REAL_PRINT_FORMAT);
    else
      abort ();
  }

  fprintf (file, "Col = \n");
  if (Data.Col)
    ut_array_2d_int_fprintf (file, Data.Col + 1, Data.Qty, 3,
                             "%d");

  fprintf (file, "ColScheme = %s\n", Data.ColScheme);

  fprintf (file, "RadDataType = %s\n", Data.RadDataType);

  fprintf (file, "RadData = \n");
  if (Data.RadData)
    ut_array_2d_fprintf (file, Data.RadData + 1, Data.Qty, 3,
                         REAL_PRINT_FORMAT);

  fprintf (file, "Rad = \n");
  if (Data.Rad)
    ut_array_1d_fprintf (file, Data.Rad + 1, Data.Qty, "%f\n");

  fprintf (file, "Scale = %s\n", Data.Scale);

  fprintf (file, "BCol = \n");
  if (Data.BCol)
    ut_array_1d_int_fprintf (file, Data.BCol, 3, "%d\n");

  fprintf (file, "BRad = %f\n", Data.BRad);

  return;
}

void
neut_debug_data_name (char *filename, struct DATA Data)
{
  FILE *file = ut_file_open (filename, "W");

  neut_debug_data (file, Data);

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
    ut_array_2d_fprintf (file, Tesr.CellOri + 1, Tesr.CellQty, 4, REAL_PRINT_FORMAT);
  else
    fprintf (file, "is NULL\n");

  fprintf (file, "size = ");
  ut_array_1d_int_fprintf (file, Tesr.size, 3, "%d");
  fprintf (file, "vsize = ");
  ut_array_1d_fprintf (file, Tesr.vsize, 3, REAL_PRINT_FORMAT);

  fprintf (file, "VoxCell = ");
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        fprintf (file, "%d ", Tesr.VoxCell[i][j][k]);
  fprintf (file, "\n");

  return;
}

void
neut_debug_prim (FILE * file, struct PRIM Prim)
{
  fprintf (file, "Type = ");
  fprintf (file, "%s\n", Prim.Type);

  fprintf (file, "ParmQty = ");
  fprintf (file, "%d\n", Prim.ParmQty);

  fprintf (file, "Parms = ");
  if (Prim.Parms)
    ut_array_1d_fprintf (file, Prim.Parms, Prim.ParmQty, REAL_PRINT_FORMAT);
  else
    fprintf (file, "NULL\n");

  fprintf (file, "Eq = ");
  if (Prim.Eq)
    ut_array_1d_fprintf (file, Prim.Eq, 4, REAL_PRINT_FORMAT);
  else
    fprintf (file, "NULL\n");

  fprintf (file, "Base = ");
  if (Prim.Base)
    ut_array_1d_fprintf (file, Prim.Base, 3, REAL_PRINT_FORMAT);
  else
    fprintf (file, "NULL\n");

  fprintf (file, "Dir = ");
  if (Prim.Dir)
    ut_array_1d_fprintf (file, Prim.Dir, 3, REAL_PRINT_FORMAT);
  else
    fprintf (file, "NULL\n");

  fprintf (file, "Rad = %f\n", Prim.Rad[0]);

  return;
}

void
neut_debug_mtess (FILE * file, struct MTESS MTess, struct TESS *Tess)
{
  int i, j;

  fprintf (file, "LevelQty = %d\n", MTess.LevelQty);

  fprintf (file, "LevelTessQty =\n");
  if (!MTess.LevelTessQty)
    fprintf (file, "is NULL\n");
  else
    ut_array_1d_int_fprintf (file, MTess.LevelTessQty + 1, MTess.LevelQty,
                             "%d");

  fprintf (file, "LevelTess =\n");
  if (!MTess.LevelTess)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= MTess.LevelQty; i++)
      ut_array_1d_int_fprintf (file, MTess.LevelTess[i] + 1,
                               MTess.LevelTessQty[i], "%d");

  fprintf (file, "TessQty = %d\n", MTess.TessQty);

  fprintf (file, "TessDom =\n");
  if (!MTess.TessDom)
    printf ("is NULL\n");
  else
    ut_array_2d_int_fprintf (file, MTess.TessDom + 1, MTess.TessQty, 2, "%d");

  fprintf (file, "DomTess =\n");
  fprintf (file, "FIXME\n");

  fprintf (file, "TessDQty = %d\n", MTess.TessDQty);

  fprintf (file, "TessDomVerNb =\n");
  if (!MTess.TessDomVerNb)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= MTess.TessQty; i++)
      ut_array_1d_int_fprintf (file, MTess.TessDomVerNb[i] + 1,
                               Tess[i].DomVerQty, "%d");

  fprintf (file, "TessDomEdgeNb =\n");
  if (!MTess.TessDomEdgeNb)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= MTess.TessQty; i++)
      ut_array_1d_int_fprintf (file, MTess.TessDomEdgeNb[i] + 1,
                               Tess[i].DomEdgeQty, "%d");

  fprintf (file, "TessDomFaceNb =\n");
  if (!MTess.TessDomFaceNb)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= MTess.TessQty; i++)
      ut_array_1d_int_fprintf (file, MTess.TessDomFaceNb[i] + 1,
                               Tess[i].DomFaceQty, "%d");

  fprintf (file, "DomTessFaceNb =\n");
  if (!MTess.DomTessFaceNb)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= MTess.TessQty; i++)
      for (j = 1; j <= Tess[MTess.TessQty].FaceQty; j++)
      {
        fprintf (file, "tess %d face %d: ", i, j);
        ut_array_1d_int_fprintf (file, MTess.DomTessFaceNb[i][j], 2, "%d");
      }

  return;
}

void
neut_debug_sim (FILE *file, struct SIM Sim)
{
  int i, j;

  fprintf (file, "simdir = %s\n", Sim.simdir);
  fprintf (file, "StepQty = %d\n", Sim.StepQty);
  fprintf (file, "OriDes = %s\n", Sim.OriDes);
  fprintf (file, "StepState = ");
  ut_array_1d_int_fprintf (file, Sim.StepState, Sim.StepQty, "%d");
  fprintf (file, "RestartId = %d\n", Sim.RestartId);
  fprintf (file, "RestartFiles = %d\n", Sim.RestartFiles);
  fprintf (file, "\n");
  fprintf (file, "EntityQty = %d\n", Sim.EntityQty);
  fprintf (file, "Entities =");
  for (i = 0; i < Sim.EntityQty; i++)
    fprintf (file, " %s", Sim.Entities[i]);
  fprintf (file, "\n");

  fprintf (file, "EntityType =\n");
  for (i = 0; i < Sim.EntityQty; i++)
    fprintf (file, "%s: %s\n", Sim.Entities[i], Sim.EntityType[i]);

  fprintf (file, "EntityMemberQty =\n");
  for (i = 0; i < Sim.EntityQty; i++)
    fprintf (file, "%s: %d\n", Sim.Entities[i], Sim.EntityMemberQty[i]);

  fprintf (file, "EntityMemberExpr =\n");
  for (i = 0; i < Sim.EntityQty; i++)
  {
    fprintf (file, "%s: ", Sim.Entities[i]);
    if (Sim.EntityMemberExpr[i])
      for (j = 0; j < Sim.EntityMemberQty[i]; j++)
        fprintf (file, " %s", Sim.EntityMemberExpr[i][j]);
    else
      fprintf (file, "unalloc'ed\n");
    fprintf (file, "\n");
  }

  fprintf (file, "EntityMembers =\n");
  for (i = 0; i < Sim.EntityQty; i++)
  {
    fprintf (file, "%s: ", Sim.Entities[i]);
    for (j = 0; j < Sim.EntityMemberQty[i]; j++)
    {
      if (Sim.EntityMembers[i] && Sim.EntityMembers[i][j])
        ut_array_1d_int_fprintf (file, Sim.EntityMembers[i][j] + 1, Sim.EntityMembers[i][j][0], "%d");
      else
        fprintf (file, "unalloc'ed\n");
    }
  }

  fprintf (file, "EntityResQty =\n");
  for (i = 0; i < Sim.EntityQty; i++)
    fprintf (file, "%s: %d\n", Sim.Entities[i], Sim.EntityResQty[i]);

  return;
}
