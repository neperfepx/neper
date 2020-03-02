/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesl_tess_.h"

void
neut_tesl_tess_general (struct TESL Tesl, struct SEEDSET SSet, int level,
                        int tessid, struct TESS *pTess)
{
  (*pTess).CellQty = Tesl.PolyQty;

  ut_string_string (SSet.Type ? SSet.Type : "standard", &(*pTess).Type);

  (*pTess).Dim = 3;

  (*pTess).Level = level;
  (*pTess).TessId = tessid;

  (*pTess).Periodic = ut_alloc_1d_int (3);
  (*pTess).PeriodicDist = ut_alloc_1d (3);
  if (SSet.Periodic)
    ut_array_1d_int_memcpy (SSet.Periodic, 3, (*pTess).Periodic);
  if (SSet.PeriodicDist)
    ut_array_1d_memcpy (SSet.PeriodicDist, 3, (*pTess).PeriodicDist);

  ut_string_string (SSet.crysym ? SSet.crysym : "triclinic",
                    &(*pTess).CellCrySym);

  return;
}

/* neut_tesl_tesstializing the properties of the vertices */
void
neut_tesl_tess_ver (struct TESL Tesl, struct TESS *pTess)
{
  (*pTess).VerQty = Tesl.VerQty;

  neut_tesl_tessVerCoo (Tesl, pTess);
  neut_tesl_tessVerEdge (Tesl, pTess);
  neut_tesl_tessVerState (pTess);

  return;
}

/* neut_tesl_tesstializing the properties of the edges */
void
neut_tesl_tess_edge (struct TESL Tesl, struct TESS *pTess)
{
  (*pTess).EdgeQty = Tesl.EdgeQty;

  neut_tesl_tessEdgeVerNb (Tesl, pTess);
  neut_tesl_tessEdgeFace (Tesl, pTess);
  neut_tess_init_edgelength (pTess);
  neut_tesl_tessEdgeState (pTess);
  neut_tesl_tessEdgeDel (pTess);

  return;
}

/* neut_tesl_tesstializing the properties of the faces */
void
neut_tesl_tess_face (struct TESL Tesl, struct TESS *pTess)
{
  (*pTess).FaceQty = Tesl.FaceQty;

  neut_tesl_tessFacePoly (Tesl, pTess);
  neut_tesl_tessFaceVer (Tesl, pTess);
  neut_tesl_tessFaceEdge (Tesl, pTess);
  neut_tesl_tessFaceState (pTess);
  neut_tesl_tessFacePt (pTess);
  neut_tesl_tessFaceEq (Tesl, pTess);

  return;
}

/* neut_tesl_tesstializing the properties of the polyhedra */
void
neut_tesl_tess_poly (struct TESL Tesl, struct TESS *pTess)
{
  (*pTess).PolyQty = Tesl.PolyQty;

  (*pTess).CellTrue = ut_alloc_1d_int (Tesl.PolyQty + 1);
  (*pTess).CellBody = ut_alloc_1d_int (Tesl.PolyQty + 1);

  neut_tesl_tessPolyFace (Tesl, pTess);
  neut_tesl_tessPolyState (pTess);

  return;
}

// initializing the properties of the seeds
void
neut_tesl_tess_seed (struct SEEDSET SSet, struct TESS *pTess)
{
  (*pTess).SeedQty = SSet.Nall;
  (*pTess).PerSeedQty = SSet.Nall - SSet.N;

  if (SSet.Nall == 0)
    return;

  (*pTess).SeedCoo = ut_alloc_2d (SSet.Nall + 1, 3);
  (*pTess).SeedWeight = ut_alloc_1d (SSet.Nall + 1);

  ut_array_2d_memcpy (SSet.SeedCoo + 1, SSet.Nall, 3, (*pTess).SeedCoo + 1);
  ut_array_1d_memcpy (SSet.SeedWeight + 1, SSet.Nall,
                      (*pTess).SeedWeight + 1);

  if (!strcmp ((*pTess).Type, "periodic"))
  {
    (*pTess).PerSeedMaster = ut_alloc_1d_int (SSet.Nall + 1);
    if (SSet.PerSeedMaster)
      ut_array_1d_int_memcpy (SSet.PerSeedMaster + 1, SSet.Nall,
                              (*pTess).PerSeedMaster + 1);

    (*pTess).PerSeedShift = ut_alloc_2d_int (SSet.Nall + 1, 3);
    if (SSet.PerSeedShift)
      ut_array_2d_int_memcpy (SSet.PerSeedShift + 1, SSet.Nall, 3,
                              (*pTess).PerSeedShift + 1);
  }

  if (SSet.LamWidthId)
  {
    (*pTess).CellLamId = ut_alloc_1d_int (SSet.N + 1);
    ut_array_1d_int_memcpy (SSet.LamWidthId + 1, SSet.N,
                            (*pTess).CellLamId + 1);
  }

  return;
}
