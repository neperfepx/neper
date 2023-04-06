/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_cut_.h"

void
net_transform_tess_cut_pre (struct TESS Tess, char *expr,
                            struct TESS *pTessGen, struct PRIM **pPrim,
                            int *pPrimQty)
{
  neut_tess_tess_gen (Tess, pTessGen);
  neut_tess_tess_cell (Tess, pTessGen);

  net_transform_tess_cut_pre_prim (expr, pPrim, pPrimQty, 1);

  return;
}

void
net_transform_tess_cut_seedset_interior (struct PRIM *Prim, int PrimQty,
                                         struct TESS Tess,
                                         struct SEEDSET *pSSet,
                                         int **pintseed_oldseed,
                                         int *pintseedqty)
{
  int j, i, side, interior;

  neut_seedset_set_zero (pSSet);
  (*pintseed_oldseed) = ut_alloc_1d_int (1);

  ut_string_string (Tess.Type, &(*pSSet).Type);

  if (strcmp ((*pSSet).Type, "standard"))
    ut_print_message (2, 2,
                      "Option `-transform *cut not available for periodic tessellations.\n");

  for (i = 1; i <= Tess.CellQty; i++)
  {
    interior = 1;

    for (j = 0; j < PrimQty; j++)
    {
      neut_prim_point_side (Prim[j], Tess.SeedCoo[i], &side);
      if (side == -1)
      {
        interior = 0;
        break;
      }
    }

    if (interior)
    {
      neut_seedset_addseed (pSSet, Tess.SeedCoo[i], Tess.SeedWeight[i]);
      (*pintseed_oldseed) =
        ut_realloc_1d_int (*pintseed_oldseed, (*pSSet).N + 1);
      (*pintseed_oldseed)[(*pSSet).N] = i;
    }
  }

  (*pintseedqty) = (*pSSet).N;

  return;
}

void
net_transform_tess_cut_seedset_mirror (struct PRIM *Prim, int PrimQty,
                                       struct TESS Tess,
                                       struct SEEDSET *pSSet, int **pcutcells,
                                       int *pcutcellqty, int **pmirrorseeds,
                                       int **pmirrorseedprims,
                                       int *pmirrorseedqty)
{
  int i, j, k, side;
  double *mirror = ut_alloc_1d (3);

  srand48 (1);

  ut_free_1d_int (pcutcells);
  (*pcutcellqty) = 0;

  ut_free_1d_int (pmirrorseeds);
  ut_free_1d_int (pmirrorseedprims);
  (*pmirrorseedqty) = 0;

  for (i = 0; i < PrimQty; i++)
    for (j = 1; j <= Tess.CellQty; j++)
    {
      neut_tess_cell_primside (Tess, j, Prim[i], &side);

      if (!side)
      {
        ut_array_1d_int_list_addval (pcutcells, pcutcellqty, j);

        neut_prim_point_mirror (Prim[i], (*pSSet).SeedCoo[j], mirror);

        for (k = 0; k < 3; k++)
          mirror[k] += 1e-9 * drand48 ();

        neut_seedset_addseed (pSSet, mirror, (*pSSet).SeedWeight[j]);
        (*pmirrorseedqty)++;
        *pmirrorseeds = ut_realloc_1d_int (*pmirrorseeds, *pmirrorseedqty);
        *pmirrorseedprims =
          ut_realloc_1d_int (*pmirrorseedprims, *pmirrorseedqty);
        (*pmirrorseeds)[*pmirrorseedqty - 1] = (*pSSet).N;
        (*pmirrorseedprims)[*pmirrorseedqty - 1] = i;
      }
    }

  ut_free_1d (&mirror);

  return;
}

void
net_transform_tess_cut_tess (struct TESS Dom, struct SEEDSET SSet,
                             struct TESS TessGen, struct TESS *pTess)
{
  net_tess3d (Dom, 1, SSet, TessGen.TessId, NULL, pTess);
  net_tess3d_domain (Dom, 1, TessGen.TessId, NULL, pTess);

  return;
}

void
net_transform_tess_cut_clean (struct PRIM *Prim, int PrimQty, int *cutcells,
                              int cutcellqty, int *mirrorseeds,
                              int *mirrorseedprims, int mirrorseedqty,
                              struct TESS *pTess)
{
  int i, j;
  int *seeds = NULL, seedqty;
  int *newdomfaces = ut_alloc_1d_int (PrimQty);

  for (i = 0; i < PrimQty; i++)
  {
    seedqty = 0;
    for (j = 0; j < mirrorseedqty; j++)
      if (mirrorseedprims[j] == i)
        ut_array_1d_int_list_addval (&seeds, &seedqty, mirrorseeds[j]);

    if (seedqty > 0)
      net_transform_tess_cut_clean_remove (Prim[i], seeds, seedqty, cutcells,
                                           cutcellqty, newdomfaces + i,
                                           pTess);

    ut_free_1d_int (&seeds);
  }

  int domfaceqty_old = (*pTess).DomFaceQty;

  neut_tess_compress (pTess);

  ut_array_1d_int_addval (newdomfaces, PrimQty,
                          (*pTess).DomFaceQty - domfaceqty_old, newdomfaces);

  net_transform_tess_cut_clean_proj (newdomfaces, PrimQty, pTess);

  net_transform_tess_cut_clean_faceequpdate (newdomfaces, PrimQty, pTess);

  net_transform_tess_cut_post_update_domain (pTess);

  ut_free_1d_int (&newdomfaces);

  return;
}

void
net_transform_tess_cut_post (struct TESS TessGen, int *intseed_oldseed,
                             int intseedqty, struct TESS *pTess)
{
  int i, oldseed_max;

  neut_tess_tess_gen (TessGen, pTess);

  ut_string_string (TessGen.CellCrySym, &(*pTess).CellCrySym);

  // CellId
  if ((*pTess).CellQty && TessGen.CellId)
  {
    oldseed_max = ut_array_1d_int_max (intseed_oldseed + 1, intseedqty);

    TessGen.CellId = ut_alloc_1d_int (oldseed_max + 1);
    ut_array_1d_int_set_id (TessGen.CellId, oldseed_max + 1);
  }

  if (TessGen.CellId)
  {
    (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    for (i = 1; i <= (*pTess).CellQty; i++)
      (*pTess).CellId[i] = TessGen.CellId[intseed_oldseed[i]];
  }

  // CellOri
  if (TessGen.CellOri)
  {
    (*pTess).CellOri = ut_alloc_2d ((*pTess).CellQty + 1, 4);
    for (i = 1; i <= (*pTess).CellQty; i++)
      ut_array_1d_memcpy (TessGen.CellOri[intseed_oldseed[i]], 4,
                          (*pTess).CellOri[i]);
  }

  // CellOriDes
  ut_string_string (TessGen.CellOriDes, &(*pTess).CellOriDes);

  // CellLamId
  if (TessGen.CellLamId)
  {
    (*pTess).CellLamId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    for (i = 1; i <= (*pTess).CellQty; i++)
      (*pTess).CellLamId[i] = TessGen.CellLamId[intseed_oldseed[i]];
  }

  // CellModeId
  if (TessGen.CellModeId)
  {
    (*pTess).CellModeId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    for (i = 1; i <= (*pTess).CellQty; i++)
      (*pTess).CellModeId[i] = TessGen.CellModeId[intseed_oldseed[i]];
  }

  /*
  // CellBody
  if (TessGen.CellBody)
  {
    (*pTess).CellBody = ut_alloc_1d_int ((*pTess).CellQty + 1);
    for (i = 1; i <= (*pTess).CellQty; i++)
      (*pTess).CellBody[i] = TessGen.CellBody[intseed_oldseed[i]];
  }
  */

  return;
}
