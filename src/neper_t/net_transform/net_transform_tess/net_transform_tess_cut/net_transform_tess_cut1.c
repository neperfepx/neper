/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_cut_.h"

void
net_transform_tess_cut (char *expr, struct TESS Dom, struct TESS *pTess)
{
  int i, intseedqty, *intseed_oldseed = NULL;
  int *cutcells = NULL, cutcellqty;
  int *mirrorseeds = NULL, *mirrorseedprims = NULL, mirrorseedqty;
  struct SEEDSET SSet;
  struct TESS TessGen;
  struct PRIM *Prim = NULL;
  int PrimQty;

  neut_seedset_set_zero (&SSet);
  neut_tess_set_zero (&TessGen);

  if (Dom.CellQty == 0)
    ut_print_message (2, 3, "Dom is void (you probably used `-loadtess'.)\n");

  // preparing

  net_transform_tess_cut_pre (*pTess, expr, &TessGen, &Prim, &PrimQty);

  // recording interior seeds

  net_transform_tess_cut_seedset_interior (Prim, PrimQty, *pTess, &SSet,
                                           &intseed_oldseed, &intseedqty);

  if (intseedqty > 0)
  {
    // tessellating with interior seeds

    net_transform_tess_cut_tess (Dom, SSet, TessGen, pTess);

    // mirroring seeds of boundary cells

    net_transform_tess_cut_seedset_mirror (Prim, PrimQty, *pTess, &SSet,
                                           &cutcells, &cutcellqty,
                                           &mirrorseeds, &mirrorseedprims,
                                           &mirrorseedqty);

    // tessellating with all seeds

    net_transform_tess_cut_tess (Dom, SSet, TessGen, pTess);

    // removing exterior cells

    net_transform_tess_cut_clean (Prim, PrimQty,
                                  cutcells, cutcellqty, mirrorseeds,
                                  mirrorseedprims, mirrorseedqty, pTess);
    // finalizing

    net_transform_tess_cut_post (TessGen, intseed_oldseed, intseedqty, pTess);
  }

  else
    neut_tess_free (pTess);

  neut_tess_init_edgelength (pTess);

  ut_free_1d_int (intseed_oldseed);
  neut_seedset_free (&SSet);
  neut_tess_free (&TessGen);
  for (i = 0; i < PrimQty; i++)
    neut_prim_free (Prim + i);
  free (Prim);
  ut_free_1d_int (cutcells);
  ut_free_1d_int (mirrorseeds);
  ut_free_1d_int (mirrorseedprims);

  return;
}
