/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_.h"

// net_transform_tess_rmseed_seedset sets the seed set of the kept seeds
// of a tessellation (old_new[i] is the new id of seed i, 0 if removed),
// periodic seeds included.
void
net_transform_tess_rmseed_seedset (struct TESS Tess, int *old_new,
                                   struct TESS Dom, struct SEEDSET *pSSet)
{
  int i, *periodic = ut_alloc_1d_int (3);

  (*pSSet).Dim = Tess.Dim;
  (*pSSet).Size = ut_alloc_2d (3, 2);
  neut_tess_bbox (Dom, (*pSSet).Size);
  ut_string_string (Tess.CellCrySym, &(*pSSet).crysym);

  for (i = 1; i <= Tess.CellQty; i++)
    if (old_new[i])
      neut_seedset_addseed (pSSet, Tess.SeedCoo[i], Tess.SeedWeight[i]);

  if (Tess.Periodic)
    ut_array_1d_int_memcpy (Tess.Periodic, 3, periodic);

  // (allocated again by neut_seedset_periodize)
  ut_free_1d_int (&(*pSSet).PerSeedMaster);
  ut_free_2d_int (&(*pSSet).PerSeedShift, (*pSSet).Nall + 1);

  neut_seedset_periodize (periodic, 0, Dom, 1, pSSet);

  ut_free_1d_int (&periodic);

  return;
}

// net_transform_tess_rmseed_tess recomputes a tessellation from a seed set,
// as done at generation (see net_tess), but for the cut by a curved domain.
void
net_transform_tess_rmseed_tess (struct IN_T In, struct TESS Dom,
                                struct SEEDSET SSet, struct TESS *pTess)
{
  int dim = (*pTess).Dim;
  struct TESS DomPer;

  neut_tess_set_zero (&DomPer);
  neut_tess_free (pTess);

  if (!strcmp (SSet.Type, "periodic"))
  {
    net_tess_perdomain (In, Dom, 1, &DomPer);
    net_tess3d (DomPer, 1, SSet, 1, NULL, pTess);
    net_tess3d_domain (Dom, 1, 1, NULL, pTess);
    neut_tess_free_periodic (pTess);
    net_tess3d_periodic (pTess);
  }
  else
    net_tess3d (Dom, 1, SSet, 1, NULL, pTess);

  if (dim == 2)
    net_tess_3dto2d (pTess);

  neut_tess_free (&DomPer);

  return;
}

// net_transform_tess_rmseed_celldata copies the general data and the cell
// data of the kept cells of the previous tessellation (old_new[i] is the
// new id of cell i, 0 if removed).
void
net_transform_tess_rmseed_celldata (struct TESS Old, int *old_new,
                                    struct TESS *pTess)
{
  int i, new, qty = (*pTess).CellQty;

  neut_tess_tess_gen (Old, pTess);

  if (Old.DomType)
    ut_string_string (Old.DomType, &(*pTess).DomType);
  if (Old.CellOriDes)
    ut_string_string (Old.CellOriDes, &(*pTess).CellOriDes);
  if (Old.CellCrySym)
    ut_string_string (Old.CellCrySym, &(*pTess).CellCrySym);
  (*pTess).ScaleQty = Old.ScaleQty;

  if (Old.CellId)
    (*pTess).CellId = ut_alloc_1d_int (qty + 1);
  if (Old.CellWeight)
    (*pTess).CellWeight = ut_alloc_1d (qty + 1);
  if (Old.CellOri)
    (*pTess).CellOri = ut_alloc_2d (qty + 1, 4);
  if (Old.CellOriDistrib)
    (*pTess).CellOriDistrib = ut_alloc_2d_char (qty + 1, 1);
  if (Old.CellModeId)
    (*pTess).CellModeId = ut_alloc_1d_int (qty + 1);
  if (Old.CellGroup)
    (*pTess).CellGroup = ut_alloc_1d_int (qty + 1);
  if (Old.ScaleCellId)
    (*pTess).ScaleCellId = ut_alloc_2d_int (qty + 1, Old.ScaleQty + 1);

  for (i = 1; i <= Old.CellQty; i++)
  {
    new = old_new[i];
    if (!new)
      continue;

    if (Old.CellId)
      (*pTess).CellId[new] = Old.CellId[i];
    if (Old.CellWeight)
      (*pTess).CellWeight[new] = Old.CellWeight[i];
    if (Old.CellOri && Old.CellOri[i])
      ut_array_1d_memcpy (Old.CellOri[i], 4, (*pTess).CellOri[new]);
    if (Old.CellOriDistrib)
      ut_string_string (Old.CellOriDistrib[i], (*pTess).CellOriDistrib + new);
    if (Old.CellModeId)
      (*pTess).CellModeId[new] = Old.CellModeId[i];
    if (Old.CellGroup)
      (*pTess).CellGroup[new] = Old.CellGroup[i];
    if (Old.ScaleCellId)
      ut_array_1d_int_memcpy (Old.ScaleCellId[i], Old.ScaleQty + 1,
                              (*pTess).ScaleCellId[new]);
  }

  return;
}
