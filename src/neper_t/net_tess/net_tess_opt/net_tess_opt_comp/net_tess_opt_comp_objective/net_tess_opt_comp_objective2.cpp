/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_.h"
#include "neut_nfcloud_struct.hpp"

extern void net_polycomp (struct POLY Domain, struct SEEDSET SeedSet,
                          NFCLOUD * pnf_cloud, NFTREE ** pnf_index,
                          int **pptid_seedid, int **pseedid_ptid,
                          struct POLY **pPoly, int *seed_changed,
                          int seed_changedqty, struct TDYN *);

void
net_tess_opt_comp_objective_x_morpho (const double *x, struct TOPT *pTOpt)
{
  int i, seed;

  (*pTOpt).TDyn.varchangedqty = 0;
  (*pTOpt).TDyn.seedchangedqty = 0;
  (*pTOpt).TDyn.seedmovedqty = 0;

  for (i = 0; i < (*pTOpt).xqty; i++)
    if ((*pTOpt).iter <= 1 || (*((*pTOpt).x_pvar[i])) != x[i])
    {
      (*pTOpt).TDyn.varchangedqty++;
      *((*pTOpt).x_pvar[i]) = x[i];
      ut_array_1d_int_list_addval (&(*pTOpt).TDyn.seedchanged,
                                   &(*pTOpt).TDyn.seedchangedqty,
                                   (*pTOpt).x_seed[i]);
      if ((*pTOpt).x_var[i] != 3)
        ut_array_1d_int_list_addval (&(*pTOpt).TDyn.seedmoved,
                                     &(*pTOpt).TDyn.seedmovedqty,
                                     (*pTOpt).x_seed[i]);
    }

  ut_array_1d_int_sort_uniq ((*pTOpt).TDyn.seedchanged,
                             (*pTOpt).TDyn.seedchangedqty,
                             &(*pTOpt).TDyn.seedchangedqty);

  ut_array_1d_int_sort_uniq ((*pTOpt).TDyn.seedmoved,
                             (*pTOpt).TDyn.seedmovedqty,
                             &(*pTOpt).TDyn.seedmovedqty);

  for (i = 0; i < (*pTOpt).TDyn.seedchangedqty; i++)
  {
    seed = (*pTOpt).TDyn.seedchanged[i];
    neut_seedset_seed_update_fromseedcoo0 (&((*pTOpt).SSet), seed);
    if (!strncmp ((*pTOpt).SSet.Type, "periodic", 8))
      neut_seedset_seed_updateslaves (&((*pTOpt).SSet), seed);
  }

  return;
}

void
net_tess_opt_comp_objective_x_ori (const double *x, struct TOPT *pTOpt)
{
  int i, seed;

  (*pTOpt).TDyn.varchangedqty = 0;
  (*pTOpt).TDyn.seedchangedqty = 0;
  (*pTOpt).TDyn.seedmovedqty = 0;

  for (i = 0; i < (*pTOpt).xqty; i++)
    if ((*pTOpt).iter <= 1 || (*((*pTOpt).x_pvar[i])) != x[i])
    {
      (*pTOpt).TDyn.varchangedqty++;
      *((*pTOpt).x_pvar[i]) = x[i];
      ut_array_1d_int_list_addval (&(*pTOpt).TDyn.seedchanged,
                                   &(*pTOpt).TDyn.seedchangedqty,
                                   (*pTOpt).x_seed[i]);
      if ((*pTOpt).x_var[i] != 3)
        ut_array_1d_int_list_addval (&(*pTOpt).TDyn.seedmoved,
                                     &(*pTOpt).TDyn.seedmovedqty,
                                     (*pTOpt).x_seed[i]);
    }

  ut_array_1d_int_sort_uniq ((*pTOpt).TDyn.seedchanged,
                             (*pTOpt).TDyn.seedchangedqty,
                             &(*pTOpt).TDyn.seedchangedqty);

  ut_array_1d_int_sort_uniq ((*pTOpt).TDyn.seedmoved,
                             (*pTOpt).TDyn.seedmovedqty,
                             &(*pTOpt).TDyn.seedmovedqty);

  for (i = 0; i < (*pTOpt).TDyn.seedchangedqty; i++)
  {
    seed = (*pTOpt).TDyn.seedchanged[i];
    neut_seedset_seed_update_fromseedorir (&((*pTOpt).SSet), seed);
  }

  return;
}

// FIXME, below, we do not need to copy all values
void
net_tess_opt_comp_objective_x_crystal (const double *x, struct TOPT *pTOpt)
{
  int i;

  for (i = 0; i < (*pTOpt).xqty; i++)
    *((*pTOpt).x_pvar[i]) = x[i];

  return;
}

void
net_tess_opt_comp_objective_centroidal_update (struct TOPT *pTOpt)
{
  int i, seed;

  for (i = 0; i < (*pTOpt).TDyn.seedchangedqty; i++)
  {
    seed = (*pTOpt).TDyn.seedchanged[i];

    ut_array_1d_memcpy ((*pTOpt).SSet.SeedCoo[seed],
                        (*pTOpt).tarcellvalqty[0],
                        (*pTOpt).tarcellval[0][seed]);
  }

  return;
}

int
net_tess_opt_comp_objective_poly (struct TOPT *pTOpt)
{
  int i, cell, scell;

  ut_string_string ((char *) "custom", &((*pTOpt).SSet).weight);

  net_polycomp ((*pTOpt).DomPoly, (*pTOpt).SSet,
                (NFCLOUD *) (*pTOpt).pnf_cloud, (NFTREE **) (*pTOpt).pnf_tree,
                &(*pTOpt).ptid_seedid, &(*pTOpt).seedid_ptid, &(*pTOpt).Poly,
                (*pTOpt).TDyn.seedchanged, (*pTOpt).TDyn.seedchangedqty,
                &(*pTOpt).TDyn);

  (*pTOpt).scellchangedqty = ((*pTOpt).TDyn).cellchangedqty;
  (*pTOpt).scellchanged =
    ut_realloc_1d_int ((*pTOpt).scellchanged, (*pTOpt).scellchangedqty);
  ut_array_1d_int_memcpy (((*pTOpt).TDyn).cellchanged,
                          (*pTOpt).scellchangedqty, (*pTOpt).scellchanged);

  (*pTOpt).cellchangedqty = 0;
  for (i = 0; i < (*pTOpt).scellchangedqty; i++)
  {
    scell = (*pTOpt).scellchanged[i];
    cell = (*pTOpt).SCellCell[scell];
    ut_array_1d_int_list_addval (&((*pTOpt).cellchanged),
                                 &((*pTOpt).cellchangedqty), cell);
  }
  ut_array_1d_int_sort ((*pTOpt).cellchanged, (*pTOpt).cellchangedqty);

  return 0;
}

void
net_tess_opt_comp_objective_debugtest (struct TOPT TOpt)
{
  int i;
  double *vol = ut_alloc_1d (TOpt.CellQty + 1);
  double domvol, totvol;

  neut_tess_volume (TOpt.Dom, &domvol);
  for (i = 1; i <= TOpt.CellQty; i++)
    neut_polys_volume (TOpt.Poly, TOpt.CellSCellList[i], TOpt.CellSCellQty[i],
                       vol + i);

  totvol = ut_array_1d_sum (vol + 1, TOpt.CellQty);

  if (!ut_num_requal (totvol, domvol, 1e-6))
  {
    printf ("\n");
    ut_print_lineheader (2);
    printf ("Wrong cell update in net_polycomp.\n");
    printf ("TDyn.seedchanged = ");
    ut_array_1d_int_fprintf (stdout, TOpt.TDyn.seedchanged,
                             TOpt.TDyn.seedchangedqty, "%d");
    printf ("TDyn.cellchanged = ");
    ut_array_1d_int_fprintf (stdout, TOpt.TDyn.cellchanged,
                             TOpt.TDyn.cellchangedqty, "%d");
    printf ("cell vols: ");
    ut_array_1d_fprintf (stdout, vol + 1, TOpt.CellQty, REAL_PRINT_FORMAT);
    printf ("totvol = " REAL_PRINT_FORMAT " != domvol = " REAL_PRINT_FORMAT " (ratio " REAL_PRINT_FORMAT ")\n", totvol,
            domvol, totvol / domvol);
    abort ();
  }

  ut_free_1d (&vol);

  return;
}
