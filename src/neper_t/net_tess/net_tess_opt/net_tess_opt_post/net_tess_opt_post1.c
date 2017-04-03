/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_post_.h"

void
net_tess_opt_post (struct MTESS *pMTess, struct TESS *Tess,
		   int dtess, int dcell, int tessid,
		   struct POLY *Poly, struct TOPT TOpt,
		   struct SEEDSET *SSet)
{
  int i, cell;
  struct TESS *pTess = Tess + tessid;
  struct SEEDSET *pSSet = SSet + tessid;

  net_polys_tess (Tess[dtess].Level + 1, TOpt.SSet, tessid, Poly, pTess);

  net_tess3d_domain (Tess[dtess], dcell, tessid, pMTess, pTess);
  if (!strncmp ((*pTess).Type, "periodic", 8))
    net_tess3d_periodic (pTess);

#ifdef DEVEL_DEBUGGING_TEST
  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0)
    ut_print_message (2, 2, "The tessellation is not valid.\n");
#endif

  if (TOpt.tarqty > 0 && !strcmp (TOpt.tarvar[0], "tesr"))
  {
    (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    for (i = 1; i <= (*pTess).CellQty; i++)
    {
      cell = TOpt.SCellCell[i];
      if (TOpt.tartesr.CellId)
	(*pTess).CellId[i] = TOpt.tartesr.CellId[cell];
      else
	(*pTess).CellId[i] = cell;
    }

    if (TOpt.tartesr.CellOri)
    {
      (*pTess).CellOri = ut_alloc_2d ((*pTess).CellQty + 1, 4);
      for (i = 1; i <= (*pTess).CellQty; i++)
      {
	cell = TOpt.SCellCell[i];
	ut_array_1d_memcpy ((*pTess).CellOri[i], 4,
			    TOpt.tartesr.CellOri[cell]);
      }
    }
  }
  else
  {
    (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    for (i = 1; i <= (*pTess).CellQty; i++)
    {
      cell = TOpt.SCellCell[i];
      (*pTess).CellId[i] = cell;
    }
  }

  double **coo = ut_alloc_2d (TOpt.SSet.N + 1, 3);
  double *weight = ut_alloc_1d (TOpt.SSet.N + 1);
  ut_array_2d_memcpy (coo + 1, (*pTess).CellQty, 3, (*pTess).SeedCoo + 1);
  ut_array_1d_memcpy (weight + 1, (*pTess).CellQty, (*pTess).SeedWeight + 1);

  neut_seedset_memcpy (TOpt.SSet, pSSet);

  // Below is a fix to make sure that the seeds and the cell coincide.
  // This is to handle the case where TOpt.CellSCellList[i] != i,
  // which is obtained by sphere packing.
  // This is a workaround to the fact that the cell-seeds relationship
  // is not recorded yet in the TESS structure.
  for (i = 1; i <= (*pTess).CellQty; i++)
  {
    ut_array_1d_memcpy ((*pTess).SeedCoo[i], 3,
			coo[TOpt.CellSCellList[i][0]]);
    (*pTess).SeedWeight[i] = weight[TOpt.CellSCellList[i][0]];

    ut_array_1d_memcpy ((*pSSet).SeedCoo[i], 3,
			coo[TOpt.CellSCellList[i][0]]);
    (*pSSet).SeedWeight[i] = weight[TOpt.CellSCellList[i][0]];
  }

  if (TOpt.tartesrscale)
  {
    neut_tess_scale (pTess,
	             1 / TOpt.tartesrscale[0],
	             1 / TOpt.tartesrscale[1],
	             1 / TOpt.tartesrscale[2]);
  }

  net_tess_opt_post_modes (TOpt, Tess, tessid);

  ut_free_2d (coo, (*pTess).CellQty + 1);
  ut_free_1d (weight);

  return;
}
