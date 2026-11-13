/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_compress_.h"

// Moving the data of a cell (and of its seed) from old to new
void
neut_tess_compress_movecell (struct TESS *pTess, int old, int new)
{
  int i;
  char *tmp = NULL;

  if ((*pTess).CellId)
    (*pTess).CellId[new] = (*pTess).CellId[old];
  if ((*pTess).CellOri)
    ut_array_1d_memcpy ((*pTess).CellOri[old], 4, (*pTess).CellOri[new]);
  if ((*pTess).CellWeight)
    (*pTess).CellWeight[new] = (*pTess).CellWeight[old];
  // (swapped, so that the string of new is freed with the unused cells)
  if ((*pTess).CellOriDistrib)
  {
    tmp = (*pTess).CellOriDistrib[new];
    (*pTess).CellOriDistrib[new] = (*pTess).CellOriDistrib[old];
    (*pTess).CellOriDistrib[old] = tmp;
  }
  if ((*pTess).CellLamId)
    (*pTess).CellLamId[new] = (*pTess).CellLamId[old];
  if ((*pTess).CellModeId)
    (*pTess).CellModeId[new] = (*pTess).CellModeId[old];
  if ((*pTess).CellGroup)
    (*pTess).CellGroup[new] = (*pTess).CellGroup[old];
  if ((*pTess).CellBody)
    for (i = 0; i < (*pTess).CellBodyQty; i++)
      (*pTess).CellBody[i][new] = (*pTess).CellBody[i][old];
  if ((*pTess).ScaleCellId)
    ut_array_1d_int_memcpy ((*pTess).ScaleCellId[old], (*pTess).ScaleQty + 1,
                            (*pTess).ScaleCellId[new]);

  if ((*pTess).SeedCoo)
    ut_array_1d_memcpy ((*pTess).SeedCoo[old], 3, (*pTess).SeedCoo[new]);
  if ((*pTess).SeedWeight)
    (*pTess).SeedWeight[new] = (*pTess).SeedWeight[old];

  return;
}
