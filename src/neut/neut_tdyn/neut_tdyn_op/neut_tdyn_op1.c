/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tdyn_op_.h"

void
neut_tdyn_set_zero (struct TDYN *pTD)
{
  (*pTD).iter = 0;

  (*pTD).N = 0;
  (*pTD).Nall = 0;
  (*pTD).avdiameq = 0;

  (*pTD).shift = NULL;
  (*pTD).shiftmax = 0;
  (*pTD).shiftmean = 0;

  (*pTD).varchangedqty = 0;
  (*pTD).seedchangedqty = 0;
  (*pTD).seedchanged = NULL;
  (*pTD).seedmovedqty = 0;
  (*pTD).seedmoved = NULL;
  (*pTD).cellchangedqty = 0;
  (*pTD).cellchanged = NULL;
  (*pTD).cellvoidqty = 0;
  (*pTD).cellvoid = NULL;
  (*pTD).domcellqty = 0;
  (*pTD).domcells = NULL;

  (*pTD).changedneighs = NULL;
  (*pTD).changedneighqty = 0;

  (*pTD).shiftw = NULL;
  (*pTD).shiftwmax = 0;
  (*pTD).shiftwmean = 0;

  (*pTD).neighlist = NULL;
  (*pTD).neighqty = NULL;
  (*pTD).neighdist = NULL;
  (*pTD).neighrefcoo = NULL;
  (*pTD).neighrefw = NULL;

  (*pTD).cell_kdtree_dur = 0;
  (*pTD).cell_shift_dur = 0;
  (*pTD).cell_neigh_dur = 0;
  (*pTD).cell_cell_dur = 0;
  (*pTD).cell_other_dur = 0;
  (*pTD).cell_total_dur = 0;

  (*pTD).var_dur = 0;
  (*pTD).seed_dur = 0;
  (*pTD).val_dur = 0;
  (*pTD).val_init_dur = 0;
  (*pTD).val_penalty_dur = 0;
  (*pTD).val_val_dur = 0;
  (*pTD).val_val_cellval_dur = 0;
  (*pTD).val_val_comp_dur = 0;
  (*pTD).val_comp_dur = 0;
  (*pTD).total_dur = 0;
  (*pTD).cumtotal_dur = 0;

  (*pTD).logtime = NULL;
  (*pTD).logvar = NULL;
  (*pTD).logdis = NULL;
  (*pTD).logval = NULL;
  (*pTD).logtesr = NULL;
  (*pTD).body = NULL;
  (*pTD).logtime_fp = NULL;
  (*pTD).logvar_fp = NULL;
  (*pTD).logdis_qty = 0;
  (*pTD).logdis_fp = NULL;
  (*pTD).logval_fp = NULL;
  (*pTD).logtesr_fp = NULL;

  return;
}

void
neut_tdyn_alloc (struct TDYN *pTD, int N, int Nall)
{
  (*pTD).N = N;
  (*pTD).Nall = Nall;

  (*pTD).shift = ut_alloc_1d (N + 1);
  (*pTD).seedchanged = ut_alloc_1d_int (N + 1);
  (*pTD).cellchanged = ut_alloc_1d_int (N + 1);
  (*pTD).neighlist = ut_alloc_1d_pint (Nall + 1);
  (*pTD).neighqty = ut_alloc_1d_int (Nall + 1);
  (*pTD).neighdist = ut_alloc_1d_pdouble (Nall + 1);
  (*pTD).neighrefcoo = ut_alloc_2d (Nall + 1, 3);
  (*pTD).neighrefw = ut_alloc_1d (Nall + 1);
  (*pTD).shiftw = ut_alloc_1d (N + 1);

  return;
}

void
neut_tdyn_free (struct TDYN *pTD)
{
  ut_free_2d (&(*pTD).neighrefcoo, (*pTD).Nall + 1);
  ut_free_1d (&(*pTD).neighrefw);
  ut_free_2d_int (&(*pTD).neighlist, (*pTD).Nall + 1);
  ut_free_1d_int (&(*pTD).neighqty);
  ut_free_2d (&(*pTD).neighdist, (*pTD).Nall + 1);

  ut_free_1d_int (&(*pTD).seedchanged);
  ut_free_1d_int (&(*pTD).cellchanged);
  ut_free_1d_int (&(*pTD).cellvoid);
  ut_free_1d_int (&(*pTD).domcells);
  ut_free_1d (&(*pTD).shift);
  ut_free_1d (&(*pTD).shiftw);

  ut_free_1d_char (&(*pTD).logtime);
  ut_free_1d_char (&(*pTD).logvar);
  ut_free_1d_char (&(*pTD).logdis);
  ut_free_1d_char (&(*pTD).logtesr);
  ut_free_1d_char (&(*pTD).logval);
  ut_free_1d_char (&(*pTD).body);

  ut_free_1d_int (&(*pTD).changedneighs);

  neut_tdyn_set_zero (pTD);

  return;
}

void
neut_tdyn_init_otherdur (struct TDYN *pTD)
{
  (*pTD).cell_other_dur =
    (*pTD).cell_total_dur - (*pTD).cell_init_dur - (*pTD).cell_kdtree_dur -
    (*pTD).cell_shift_dur - (*pTD).cell_neigh_dur - (*pTD).cell_cell_dur;

  return;
}

void
neut_tdyn_init_avdiameq (struct TDYN *pTD, struct SEEDSET SSet,
                         struct POLY Domain)
{
  double size;

  if (SSet.Dim == 3)
  {
    if (!strcmp (SSet.Type, "periodic"))
      neut_seedset_bbox_size (SSet, &size);
    else
      neut_poly_volume (Domain, &size);
    size /= SSet.N;
    ut_space_volume_diameq (size, &(*pTD).avdiameq);
  }
  else if (SSet.Dim == 2)
  {
    if (!strcmp (SSet.Type, "periodic"))
      neut_seedset_bbox_size (SSet, &size);
    else
      neut_poly_volume_2d (Domain, SSet.Size[2][1] - SSet.Size[2][0], &size);
    size /= SSet.N;
    ut_space_area_diameq (size, &(*pTD).avdiameq);
  }
  else
    ut_print_neperbug ();

  return;
}
