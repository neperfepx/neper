/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_cut_.h"

void
net_transform_tess_cut (char *expr, struct TESS Dom, struct TESS *pTess)
{
  int i, qty;
  char *algo = NULL;
  char **parts = NULL, *expr2 = ut_alloc_1d_char (strlen (expr) + 1);

  ut_list_break (expr, NEUT_SEP_NODEP, &parts, &qty);

  ut_string_string ("direct", &algo);
  for (i = 0; i < qty; i++)
    if (!strncmp (parts[i], "algo", 4))
      ut_list_break_2 (parts[i], "=", NULL, &algo);

  ut_string_string (expr, &expr2);
  ut_string_fnrs (expr2, ",algo=direct", "", INT_MAX);
  ut_string_fnrs (expr2, ",algo=legacy", "", INT_MAX);

  if (!strcmp (algo, "direct"))
    net_transform_tess_cut_direct (expr2, pTess);
  else
    net_transform_tess_cut_legacy (expr2, Dom, pTess);

  ut_free_1d_char (&algo);
  ut_free_1d_char (&expr2);
  ut_free_2d_char (&parts, qty);

  return;
}

void
net_transform_tess_cut_legacy (char *expr, struct TESS Dom, struct TESS *pTess)
{
  int i, intseedqty, *intseed_oldseed = NULL;
  int *cutcells = NULL, cutcellqty;
  int *mirrorseeds = NULL, *mirrorseedprims = NULL, mirrorseedqty;
  struct SEEDSET SSet;
  struct TESS TessGen;
  struct PRIM *Prim = NULL;
  int PrimQty, dim = (*pTess).Dim;
  int *DomFaceState = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);

  neut_seedset_set_zero (&SSet);
  neut_tess_set_zero (&TessGen);

  if (Dom.CellQty == 0)
    ut_print_message (2, 3, "The domain could not be recovered from the tessellation.\n");

  // preparing

  net_transform_tess_cut_pre (*pTess, expr, &TessGen, &Prim, &PrimQty, DomFaceState);

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

    net_transform_tess_cut_clean (Prim, PrimQty, DomFaceState, cutcells, cutcellqty,
                                  mirrorseeds, mirrorseedprims, mirrorseedqty,
                                  pTess, intseed_oldseed, &intseedqty);

    // finalizing

    net_transform_tess_cut_post (TessGen, intseed_oldseed, intseedqty, pTess);
  }

  else
    neut_tess_reset (pTess);

  if (dim == 2)
  {
    (*pTess).Dim = 3;
    net_tess_3dto2d (pTess);
  }

  neut_tess_init_edgelength (pTess);

  ut_free_1d_int (&intseed_oldseed);
  neut_seedset_free (&SSet);
  neut_tess_free (&TessGen);
  for (i = 0; i < PrimQty; i++)
    neut_prim_free (Prim + i);
  free (Prim);
  ut_free_1d_int (&cutcells);
  ut_free_1d_int (&mirrorseeds);
  ut_free_1d_int (&mirrorseedprims);
  ut_free_1d_int (&DomFaceState);

  return;
}

void
net_transform_tess_cut_direct (char *expr, struct TESS *pTess)
{
  int i, PrimQty;
  struct PRIM *Prim = NULL;
  struct TESS TessGen;

  neut_tess_set_zero (&TessGen);

  // Extracting primitives from expression
  net_transform_tess_cut_pre (*pTess, expr, &TessGen, &Prim, &PrimQty, NULL);

  net_transform_tess_cut_direct_prims (Prim, PrimQty, pTess);

  for (i = 0; i < PrimQty; i++)
    neut_prim_free (Prim + i);
  free (Prim);
  neut_tess_free (&TessGen);

  return;
}

// net_transform_tess_cut_prim cuts a tessellation by primitives (see
// net_transform_tess_cut_direct).
void
net_transform_tess_cut_prim (struct PRIM *Prim, int PrimQty, struct TESS *pTess)
{
  net_transform_tess_cut_direct_prims (Prim, PrimQty, pTess);

  return;
}
