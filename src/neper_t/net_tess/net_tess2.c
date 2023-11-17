/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_.h"

void
net_tess_perdomain (struct IN_T In, struct TESS PTess, int cell,
                    struct TESS *pDom)
{
  double *coo = ut_alloc_1d (3);

  if (ut_array_1d_int_sum (In.periodic, 3) == 0)
    ut_print_neperbug ();

  neut_tess_reset (pDom);

  neut_tess_cell_centroid (PTess, cell, coo);
  neut_tess_poly_tess (PTess, cell, pDom);
  neut_tess_shift (pDom, -coo[0], -coo[1], -coo[2]);

  neut_tess_scale (pDom, 1 + 2 * In.periodic[0], 1 + 2 * In.periodic[1],
                   1 + 2 * In.periodic[2]);

  neut_tess_shift (pDom, coo[0], coo[1], coo[2]);

  ut_free_1d (&coo);

  return;
}

int
net_tess_3dto2d (struct TESS *pTess)
{
  int id;
  struct TESS T2;

  neut_tess_set_zero (&T2);

  if ((*pTess).DomFaceQty == 0)
    ut_print_neperbug ();

  if (neut_tess_domface_label_id (*pTess, "z0", &id) != 0)
  {
    neut_tess_init_domfacez0 (pTess);

    if (neut_tess_domface_label_id (*pTess, "z0", &id) != 0)
      ut_print_neperbug ();
  }

  neut_tess_3dcolumnar_2d (*pTess, &T2);
  neut_tess_tess (T2, pTess);
  neut_tess_free (&T2);

#ifdef DEVEL_DEBUGGING_TEST
  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0)
    ut_print_message (2, 2, "The tessellation is not valid.\n");
#endif

  return 0;
}

int
net_tess_finalize (struct TESS PTess, struct SEEDSET SSet, struct TESS *pTess)
{
  int i;

  if ((*pTess).CellQty == 0)
    return 1;

  (*pTess).PseudoDim = PTess.PseudoDim;
  (*pTess).PseudoSize = PTess.PseudoSize;

  if (!(*pTess).CellOri)
  {
    (*pTess).CellOri = ut_alloc_2d ((*pTess).CellQty + 1, 4);
    ut_array_2d_memcpy (SSet.SeedOri + 1, (*pTess).CellQty, 4,
                        (*pTess).CellOri + 1);
  }

  if (SSet.SeedOriDistrib && !(*pTess).CellOriDistrib)
  {
    (*pTess).CellOriDistrib = ut_alloc_1d_pchar ((*pTess).CellQty + 1);
    for (i = 0; i <= (*pTess).CellQty; i++)
      ut_string_string (SSet.SeedOriDistrib[i], (*pTess).CellOriDistrib + i);
  }

#ifdef DEVEL_DEBUGGING_TEST
  if ((*pTess).Dim > 1
      && (neut_tess_test (*pTess, 0) != 0
          || neut_tess_test_dom (*pTess, 0) != 0))
    ut_print_message (2, 2, "The tessellation is not valid.\n");
#endif

  return 0;
}
