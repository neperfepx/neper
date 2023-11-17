/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the tori-level directory. */

#include "neut_tesr_ori_.h"

void
neut_tesr_cell_orianiso (struct TESR Tesr, int cell, double **evect,
                         double *eval)
{
  struct OL_SET Set;

  neut_tesr_cell_olset (Tesr, cell, &Set);

  ol_set_disoriset (Set, &Set);

  ol_set_aniso (Set, evect, eval);

  ol_set_free (&Set);

  return;
}

void
neut_tesr_cell_orianiso_delta (struct TESR Tesr, int cell, double **evect_in,
                               double *delta_in)
{
  struct OL_SET Set;
  double **evect = ut_alloc_2d (3, 3);
  double *eval = ut_alloc_1d (3);
  double *delta = ut_alloc_1d (3);

  neut_tesr_cell_olset (Tesr, cell, &Set);

  ol_set_disoriset (Set, &Set);

  ol_set_aniso_delta (Set, evect, eval, delta);

  if (evect_in)
    ut_array_2d_memcpy (evect, 3, 3, evect_in);
  if (delta_in)
    ut_array_1d_memcpy (delta, 3, delta_in);

  ol_set_free (&Set);
  ut_free_2d (&evect, 3);
  ut_free_1d (&eval);
  ut_free_1d (&delta);

  return;
}

void
neut_tesr_cells_olset (struct TESR Tesr, double **CellOri, char *crysym,
                       int *cells, int cellqty, struct OL_SET *pOSet)
{
  int i, cell;

  *pOSet = ol_set_alloc (cellqty, crysym);

  for (i = 0; i < cellqty; i++)
  {
    cell = cells ? cells[i] : i + 1;
    neut_tesr_cell_size (Tesr, cell, (*pOSet).weight + i);
    ol_q_memcpy (CellOri ? CellOri[cell] : Tesr.CellOri[cell], (*pOSet).q[i]);
  }

  if (crysym)
    ut_string_string (crysym, &(*pOSet).crysym);

  return;
}

void
neut_tesr_voxels_olset (struct TESR Tesr, struct OL_SET *pOSet)
{
  int i, j, k, qty, pos;

  qty = 0;
  for (i = 1; i <= Tesr.size[0]; i++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (k = 1; k <= Tesr.size[2]; k++)
        if (!Tesr.VoxOriDef || Tesr.VoxOriDef[i][j][k])
          qty++;

  (*pOSet) = ol_set_alloc (qty, Tesr.CellCrySym);

  pos = 0;
  for (i = 1; i <= Tesr.size[0]; i++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (k = 1; k <= Tesr.size[2]; k++)
        if (!Tesr.VoxOriDef || Tesr.VoxOriDef[i][j][k])
        {
          if (Tesr.VoxOri)
            ol_q_memcpy (Tesr.VoxOri[i][j][k], (*pOSet).q[pos++]);
          else
            ol_q_memcpy (Tesr.CellOri[Tesr.VoxCell[i][j][k]], (*pOSet).q[pos++]);
        }

  return;
}
