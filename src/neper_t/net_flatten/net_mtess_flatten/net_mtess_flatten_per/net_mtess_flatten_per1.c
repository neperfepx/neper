/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_per_.h"

// For a periodic tessellation, each slave seed of a face gets a
// "virtual" child tessellation, which is the child tessellation of its
// master cell translated to the slave position.  Its domain vertices,
// edges and faces are mapped to the vertices, edges and faces of the
// tessellation they coincide with (or 0), so that the periodic faces,
// edges and vertices are flattened from the child tessellations on
// both sides, as the other faces, edges and vertices.  Only the
// entities of the virtual tessellations on these faces, edges and
// vertices are flattened.
void
net_mtess_flatten_per_init (struct MTESS MTess, struct TESS *Tess, int TessId,
                            struct MTESS *pMTess2, struct TESS **pTess2,
                            int **pVTessIds, int *pVTessQty)
{
  int i, j, t, s, master, ctess, vtess, slaveqty, tessqty;
  int *slaves = NULL;
  struct TESS PTess = Tess[TessId];
  double *shift = ut_alloc_1d (3);

  // slave seeds of the faces
  slaves = ut_alloc_1d_int (2 * PTess.FaceQty);
  slaveqty = 0;
  for (i = 1; i <= PTess.FaceQty; i++)
    for (t = 0; t < 2; t++)
      if (PTess.FacePoly[i][t] > PTess.CellQty)
        slaves[slaveqty++] = PTess.FacePoly[i][t];
  ut_array_1d_int_sort_uniq (slaves, slaveqty, &slaveqty);

  // slave seeds whose master cell has a (non-void) child tessellation
  (*pVTessQty) = 0;
  (*pVTessIds) = ut_alloc_1d_int (slaveqty);
  for (i = 0; i < slaveqty; i++)
  {
    ctess = MTess.DomTess[TessId][PTess.PerSeedMaster[slaves[i]]];
    if (ctess > 0 && Tess[ctess].CellQty > 0)
      slaves[(*pVTessQty)++] = slaves[i];
  }

  tessqty = MTess.TessQty + *pVTessQty;

  (*pTess2) = calloc (tessqty + 1, sizeof (struct TESS));
  for (i = 0; i <= MTess.TessQty; i++)
    (*pTess2)[i] = Tess[i];

  (*pMTess2) = MTess;
  (*pMTess2).TessQty = tessqty;
  (*pMTess2).TessDom = ut_alloc_1d_pint (tessqty + 1);
  (*pMTess2).DomTess = ut_alloc_1d_pint (tessqty + 1);
  (*pMTess2).TessDomVerNb = ut_alloc_1d_pint (tessqty + 1);
  (*pMTess2).TessDomEdgeNb = ut_alloc_1d_pint (tessqty + 1);
  (*pMTess2).TessDomFaceNb = ut_alloc_1d_pint (tessqty + 1);
  (*pMTess2).DomTessFaceNb = ut_alloc_1d_ppint (tessqty + 1);
  for (i = 0; i <= MTess.TessQty; i++)
  {
    (*pMTess2).TessDom[i] = MTess.TessDom[i];
    (*pMTess2).DomTess[i] = MTess.DomTess[i];
    (*pMTess2).TessDomVerNb[i] = MTess.TessDomVerNb[i];
    (*pMTess2).TessDomEdgeNb[i] = MTess.TessDomEdgeNb[i];
    (*pMTess2).TessDomFaceNb[i] = MTess.TessDomFaceNb[i];
    (*pMTess2).DomTessFaceNb[i] = MTess.DomTessFaceNb[i];
  }

  // the rows of the tessellation are extended to the slave seeds
  (*pMTess2).DomTess[TessId] = ut_alloc_1d_int (PTess.SeedQty + 1);
  ut_array_1d_int_memcpy (MTess.DomTess[TessId], PTess.PolyQty + 1,
                          (*pMTess2).DomTess[TessId]);
  (*pMTess2).DomTessFaceNb[TessId] = ut_alloc_2d_int (PTess.FaceQty + 1, 2);
  for (i = 1; i <= PTess.FaceQty; i++)
    ut_array_1d_int_memcpy (MTess.DomTessFaceNb[TessId][i], 2,
                            (*pMTess2).DomTessFaceNb[TessId][i]);

  for (i = 0; i < *pVTessQty; i++)
  {
    s = slaves[i];
    master = PTess.PerSeedMaster[s];
    ctess = MTess.DomTess[TessId][master];
    vtess = MTess.TessQty + i + 1;
    (*pVTessIds)[i] = vtess;

    neut_tess_set_zero ((*pTess2) + vtess);
    neut_tess_tess (Tess[ctess], (*pTess2) + vtess);
    for (j = 0; j < 3; j++)
      shift[j] = PTess.PerSeedShift[s][j] * PTess.PeriodicDist[j];
    neut_tess_shift ((*pTess2) + vtess, shift[0], shift[1], shift[2]);
    (*pTess2)[vtess].TessId = vtess;

    (*pMTess2).TessDom[vtess] = ut_alloc_1d_int (2);
    (*pMTess2).TessDom[vtess][0] = TessId;
    (*pMTess2).TessDom[vtess][1] = s;
    (*pMTess2).DomTess[TessId][s] = vtess;

    (*pMTess2).TessDomVerNb[vtess] =
      ut_alloc_1d_int (Tess[ctess].DomVerQty + 1);
    for (j = 1; j <= Tess[ctess].DomVerQty; j++)
      (*pMTess2).TessDomVerNb[vtess][j] =
        net_mtess_flatten_per_ver (PTess, MTess.TessDomVerNb[ctess][j],
                                   PTess.PerSeedShift[s]);

    (*pMTess2).TessDomEdgeNb[vtess] =
      ut_alloc_1d_int (Tess[ctess].DomEdgeQty + 1);
    for (j = 1; j <= Tess[ctess].DomEdgeQty; j++)
      (*pMTess2).TessDomEdgeNb[vtess][j] =
        net_mtess_flatten_per_edge (PTess, MTess.TessDomEdgeNb[ctess][j],
                                    PTess.PerSeedShift[s]);

    (*pMTess2).TessDomFaceNb[vtess] =
      ut_alloc_1d_int (Tess[ctess].DomFaceQty + 1);
    for (j = 1; j <= Tess[ctess].DomFaceQty; j++)
      (*pMTess2).TessDomFaceNb[vtess][j] =
        net_mtess_flatten_per_face (PTess, MTess.TessDomFaceNb[ctess][j],
                                    PTess.PerSeedShift[s]);
  }

  // domain faces of the virtual tessellations on the faces
  for (i = 1; i <= PTess.FaceQty; i++)
    for (t = 0; t < 2; t++)
    {
      s = PTess.FacePoly[i][t];
      if (s > PTess.CellQty && (*pMTess2).DomTess[TessId][s] > 0)
      {
        vtess = (*pMTess2).DomTess[TessId][s];
        j = 1 + ut_array_1d_int_eltpos ((*pMTess2).TessDomFaceNb[vtess] + 1,
                                        (*pTess2)[vtess].DomFaceQty, i);
        if (j == 0)
          ut_print_neperbug ();
        (*pMTess2).DomTessFaceNb[TessId][i][t] = j;
      }
    }

  ut_free_1d_int (&slaves);
  ut_free_1d (&shift);

  return;
}

void
net_mtess_flatten_per_free (struct MTESS MTess, int TessId,
                            struct MTESS *pMTess2, struct TESS **pTess2,
                            int **pVTessIds, int VTessQty)
{
  int i, vtess;

  for (i = 0; i < VTessQty; i++)
  {
    vtess = (*pVTessIds)[i];
    neut_tess_free ((*pTess2) + vtess);
    ut_free_1d_int (&(*pMTess2).TessDom[vtess]);
    ut_free_1d_int (&(*pMTess2).TessDomVerNb[vtess]);
    ut_free_1d_int (&(*pMTess2).TessDomEdgeNb[vtess]);
    ut_free_1d_int (&(*pMTess2).TessDomFaceNb[vtess]);
  }

  ut_free_1d_int (&(*pMTess2).DomTess[TessId]);
  ut_free_2d_int (&(*pMTess2).DomTessFaceNb[TessId],
                  (*pTess2)[TessId].FaceQty + 1);

  free ((*pMTess2).TessDom);
  free ((*pMTess2).DomTess);
  free ((*pMTess2).TessDomVerNb);
  free ((*pMTess2).TessDomEdgeNb);
  free ((*pMTess2).TessDomFaceNb);
  free ((*pMTess2).DomTessFaceNb);
  free (*pTess2);
  (*pTess2) = NULL;
  ut_free_1d_int (pVTessIds);

  (*pMTess2) = MTess;

  return;
}

// Sets the periodicity of the flattened tessellation: the slave seeds
// are the cells of the child tessellations of the master cells of the
// slave seeds of the tessellation, and the faces on the periodic faces
// of the tessellation are bounded by the slave seeds of the virtual
// tessellations.
void
net_mtess_flatten_per (struct MTESS MTess, struct TESS *Tess, int TessId,
                       int *VTessIds, int VTessQty, struct TESSE *TessE,
                       struct TESS *pFTess)
{
  int i, j, k, l, s, ctess, vtess, cell, seed, poly, fface;
  struct TESS PTess = Tess[TessId];
  int *firstslave = ut_alloc_1d_int (PTess.SeedQty + 1);

  ut_string_string ("periodic", &(*pFTess).Type);
  ut_free_1d_int (&(*pFTess).Periodic);
  (*pFTess).Periodic = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy (PTess.Periodic, 3, (*pFTess).Periodic);
  ut_free_1d (&(*pFTess).PeriodicDist);
  (*pFTess).PeriodicDist = ut_alloc_1d (3);
  ut_array_1d_memcpy (PTess.PeriodicDist, 3, (*pFTess).PeriodicDist);

  // slave seeds
  (*pFTess).PerSeedMaster = ut_alloc_1d_int ((*pFTess).SeedQty + 1);
  (*pFTess).PerSeedShift = ut_alloc_2d_int ((*pFTess).SeedQty + 1, 3);

  for (s = PTess.CellQty + 1; s <= PTess.SeedQty; s++)
  {
    firstslave[s] = (*pFTess).SeedQty + 1;
    ctess = MTess.DomTess[TessId][PTess.PerSeedMaster[s]];
    if (ctess <= 0)
      continue;

    for (k = 1; k <= Tess[ctess].CellQty; k++)
    {
      cell = TessE[ctess].PolyFPolyNb[k][0];
      seed = ++(*pFTess).SeedQty;

      (*pFTess).SeedCoo = ut_realloc_2d_addline ((*pFTess).SeedCoo, seed + 1, 3);
      for (j = 0; j < 3; j++)
        (*pFTess).SeedCoo[seed][j] = (*pFTess).SeedCoo[cell][j]
                                   + PTess.PerSeedShift[s][j] * PTess.PeriodicDist[j];
      (*pFTess).SeedWeight = ut_realloc_1d ((*pFTess).SeedWeight, seed + 1);
      (*pFTess).SeedWeight[seed] = (*pFTess).SeedWeight[cell];

      (*pFTess).PerSeedMaster = ut_realloc_1d_int ((*pFTess).PerSeedMaster, seed + 1);
      (*pFTess).PerSeedMaster[seed] = cell;
      (*pFTess).PerSeedShift = ut_realloc_2d_int_addline ((*pFTess).PerSeedShift, seed + 1, 3);
      ut_array_1d_int_memcpy (PTess.PerSeedShift[s], 3, (*pFTess).PerSeedShift[seed]);
    }
  }

  (*pFTess).PerSeedQty = (*pFTess).SeedQty - (*pFTess).CellQty;

  // faces on the periodic faces
  for (i = 0; i < VTessQty; i++)
  {
    vtess = VTessIds[i];
    s = MTess.TessDom[vtess][1];

    for (j = 1; j <= Tess[vtess].FaceQty; j++)
    {
      poly = ut_array_1d_int_max (Tess[vtess].FacePoly[j], 2);

      for (l = 0; l < TessE[vtess].FaceFFaceQty[j]; l++)
      {
        fface = TessE[vtess].FaceFFaceNb[j][l];
        if ((*pFTess).FacePoly[fface][1] != 0)
          ut_print_neperbug ();
        (*pFTess).FacePoly[fface][1] = firstslave[s] + poly - 1;
      }
    }
  }

  net_tess3d_periodic (pFTess);

  ut_free_1d_int (&firstslave);

  return;
}
