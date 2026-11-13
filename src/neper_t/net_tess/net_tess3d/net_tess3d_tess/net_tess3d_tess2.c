/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_tess_.h"

void
net_tess3d_tess_general (struct SEEDSET SSet, int level, int tessid, struct TESS *pTess)
{
  (*pTess).CellQty = SSet.N;
  (*pTess).PolyQty = SSet.N;

  ut_string_string (SSet.Type ? SSet.Type : "standard", &(*pTess).Type);

  (*pTess).Dim = 3;

  (*pTess).Level = level;
  (*pTess).TessId = tessid;

  ut_free_1d_int (&(*pTess).Periodic);
  (*pTess).Periodic = ut_alloc_1d_int (3);
  ut_free_1d (&(*pTess).PeriodicDist);
  (*pTess).PeriodicDist = ut_alloc_1d (3);
  if (SSet.Periodic)
    ut_array_1d_int_memcpy (SSet.Periodic, 3, (*pTess).Periodic);
  if (SSet.PeriodicDist)
    ut_array_1d_memcpy (SSet.PeriodicDist, 3, (*pTess).PeriodicDist);

  ut_string_string (SSet.crysym ? SSet.crysym : "triclinic", &(*pTess).CellCrySym);

  return;
}

/* Recording of TFNb & TVNb */
void
net_tess3d_tess_txnb (struct POLY *Poly, int **TFNb, int **TVNb, struct TESS *pTess)
{
  (*pTess).FaceQty = net_tess3d_tess_txnb_tfnb (Poly, (*pTess).PolyQty, TFNb);
  (*pTess).VerQty = net_tess3d_tess_txnb_tvnb (Poly, (*pTess).PolyQty, TVNb);

  return;
}

/* Recording of FacePoly, FaceEq, FaceVerQty & FaceVerNb */
void
net_tess3d_tess_face (struct POLY *Poly, int **TFNb, int **TVNb, struct TESS *pTess)
{
  int i, j, k, FNb, reverse;

  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);
  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);

  /* for every face of every polyhedron, if it has not been treated
   * previously */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
        FNb = TFNb[i][j];

        (*pTess).FacePoly[FNb][0] = i;
        (*pTess).FacePoly[FNb][1] = Poly[i].FacePoly[j];

        reverse = net_tess3d_tess_face_eq (Poly, i, j, (*pTess).FaceEq[FNb]);

        (*pTess).FaceVerQty[FNb] = Poly[i].FaceVerQty[j];
        (*pTess).FaceVerNb[FNb] = ut_alloc_1d_int ((*pTess).FaceVerQty[FNb] + 1);
        for (k = 1; k <= (*pTess).FaceVerQty[FNb]; k++)
          (*pTess).FaceVerNb[FNb][k] = TVNb[i][Poly[i].FaceVerNb[j][k]];

        // reversing the order of the vertices (but the first one)
        if (reverse)
          ut_array_1d_int_reverseelts ((*pTess).FaceVerNb[FNb] + 2, (*pTess).FaceVerQty[FNb] - 1);
      }

  return;
}

/* Recording of VerCoo and of the parent seeds of the vertices */
void
net_tess3d_tess_ver (struct POLY *Poly, int **TVNb, struct TESS *pTess, int ***pVerSeed)
{
  int i, j, k, VNb;

  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pVerSeed) = ut_alloc_2d_int ((*pTess).VerQty + 1, 4);

  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].VerQty; j++)
      ut_array_1d_memcpy (Poly[i].VerCoo[j], 3, (*pTess).VerCoo[TVNb[i][j]]);

  /* For every poly, for every ver, if the ver has not been treated yet,
   * the 4 parent seeds are recorded. The first one is positive, the
   * three others can be negative (poly initial faces).
   */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].VerQty; j++)
    {
      VNb = TVNb[i][j];
      if ((*pVerSeed)[VNb][0] == 0)
      {
        (*pVerSeed)[VNb][0] = i;
        for (k = 1; k <= 3; k++)
          (*pVerSeed)[VNb][k] = Poly[i].FacePoly[Poly[i].VerFace[j][k - 1]];
      }
    }

  return;
}

/* Recording of PolyFaceQty, PolyFaceNb & PolyFaceOri */
void
net_tess3d_tess_poly (struct POLY *Poly, int **TVNb, int **TFNb, struct TESS *pTess)
{
  int i, num;
  double centre[3];

  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyState = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  for (num = 1; num <= (*pTess).PolyQty; num++)
  {
    (*pTess).PolyFaceQty[num] = Poly[num].FaceQty;
    (*pTess).PolyFaceNb[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    (*pTess).PolyFaceOri[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);

    if (Poly[num].FaceQty == 0)
    {
      (*pTess).PolyState[num] = -1;
      continue;
    }

    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pTess).PolyFaceNb[num][i] = TFNb[num][i];

    // centre of the polyhedron vertices
    ut_array_1d_zero (centre, 3);
    for (i = 1; i <= Poly[num].VerQty; i++)
      ut_array_1d_add (centre, (*pTess).VerCoo[TVNb[num][i]], 3, centre);
    ut_array_1d_scale (centre, 3, 1. / Poly[num].VerQty);

    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pTess).PolyFaceOri[num][i] = net_tess3d_tess_poly_faceori (pTess, num, i, centre);
  }

  return;
}

/* Recording of the edges, i.e. EdgeVerNb, FaceEdgeNb, FaceEdgeOri,
 * VerEdgeNb, EdgeFaceNb and EdgeLength */
void
net_tess3d_tess_edge (int **VerSeed, struct TESS *pTess)
{
  int i, j, k, S1, S2, edge, nb = 0, qty = 0;
  int seeds[16];

  (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);

  /* qty is an upper bound of the number of edges */
  for (i = 1; i <= (*pTess).FaceQty; i++)
    qty += (*pTess).FaceVerQty[i];
  (*pTess).EdgeVerNb = ut_alloc_1d_pint (qty + 1);

  (*pTess).FaceEdgeQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);

  /* For every face, for every couple of neighbor vertices, we search if
   * the edge has already been recorded (from a previous face), i.e. if it
   * is an edge of the first vertex.  If not, a new edge is recorded.
   */
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    // the face is a polygon: edge j links vertices j and j + 1
    (*pTess).FaceEdgeQty[i] = (*pTess).FaceVerQty[i];
    (*pTess).FaceEdgeNb[i] = ut_alloc_1d_int ((*pTess).FaceEdgeQty[i] + 1);

    for (j = 1; j <= (*pTess).FaceEdgeQty[i]; j++)
    {
      S1 = (*pTess).FaceVerNb[i][j];
      S2 = (*pTess).FaceVerNb[i][j < (*pTess).FaceVerQty[i] ? j + 1 : 1];

      /* The two vertices must have 3 common parent seeds (those of the
       * edge). */
      if (net_tess3d_tess_edge_commonseeds (VerSeed, S1, S2, seeds) != 3)
        ut_print_message (2, 3, "Found degenerate configuration.\n");

      edge = 0;
      for (k = 0; k < (*pTess).VerEdgeQty[S1] && !edge; k++)
        if ((*pTess).EdgeVerNb[(*pTess).VerEdgeNb[S1][k]][0] == S2
            || (*pTess).EdgeVerNb[(*pTess).VerEdgeNb[S1][k]][1] == S2)
          edge = (*pTess).VerEdgeNb[S1][k];

      if (!edge)
      {
        edge = ++nb;
        (*pTess).EdgeVerNb[edge] = ut_alloc_1d_int (2);
        (*pTess).EdgeVerNb[edge][0] = S1;
        (*pTess).EdgeVerNb[edge][1] = S2;
        ut_array_1d_int_list_addval_nocheck ((*pTess).VerEdgeNb + S1, (*pTess).VerEdgeQty + S1,
                                             edge);
        ut_array_1d_int_list_addval_nocheck ((*pTess).VerEdgeNb + S2, (*pTess).VerEdgeQty + S2,
                                             edge);
      }

      (*pTess).FaceEdgeNb[i][j] = edge;
    }
  }

  // the vertices have generally 4 edges, and 3 on the domain; their edge
  // arrays have at least 4 entries
  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerEdgeQty[i] < 4)
    {
      (*pTess).VerEdgeNb[i] = ut_realloc_1d_int ((*pTess).VerEdgeNb[i], 4);
      ut_array_1d_int_zero ((*pTess).VerEdgeNb[i] + (*pTess).VerEdgeQty[i],
                            4 - (*pTess).VerEdgeQty[i]);
    }

  (*pTess).EdgeQty = nb;
  (*pTess).EdgeVerNb = ut_realloc_1d_pint ((*pTess).EdgeVerNb, (*pTess).EdgeQty + 1);
  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  /* Recording of FaceEdgeOri */
  net_tess3d_tess_edge_faceedgeori (pTess);

  /* Recording of EdgeFaceQty & EdgeFaceNb */
  net_tess3d_tess_edge_edgeface (pTess);

  neut_tess_init_edgelength (pTess);

  return;
}

// Recording of the properties of the seeds
void
net_tess3d_tess_seed (struct SEEDSET SSet, struct TESS *pTess)
{
  (*pTess).SeedQty = SSet.Nall;
  (*pTess).PerSeedQty = SSet.Nall - SSet.N;

  if (SSet.Nall == 0)
    return;

  (*pTess).SeedCoo = ut_alloc_2d (SSet.Nall + 1, 3);
  (*pTess).SeedWeight = ut_alloc_1d (SSet.Nall + 1);

  ut_array_2d_memcpy (SSet.SeedCoo + 1, SSet.Nall, 3, (*pTess).SeedCoo + 1);
  ut_array_1d_memcpy (SSet.SeedWeight + 1, SSet.Nall, (*pTess).SeedWeight + 1);

  if (!strcmp ((*pTess).Type, "periodic"))
  {
    (*pTess).PerSeedMaster = ut_alloc_1d_int (SSet.Nall + 1);
    if (SSet.PerSeedMaster)
      ut_array_1d_int_memcpy (SSet.PerSeedMaster + 1, SSet.Nall, (*pTess).PerSeedMaster + 1);

    (*pTess).PerSeedShift = ut_alloc_2d_int (SSet.Nall + 1, 3);
    if (SSet.PerSeedShift)
      ut_array_2d_int_memcpy (SSet.PerSeedShift + 1, SSet.Nall, 3, (*pTess).PerSeedShift + 1);
  }

  if (SSet.LamWidthId)
  {
    (*pTess).CellLamId = ut_alloc_1d_int (SSet.N + 1);
    ut_array_1d_int_memcpy (SSet.LamWidthId + 1, SSet.N, (*pTess).CellLamId + 1);
  }

  return;
}
