/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_.h"

int
net_tess (struct IN_T In, int level, struct TESS *Tess, int dtess, int dcell,
          struct SEEDSET *SSet, int TessId, struct MTESS *pMTess)
{
  int status = -1, periodic;
  int dim = (In.levelqty > 1 && In.dim == 2) ? 3 : In.dim;
  struct TESS Dom;
  char *mid = NULL;
  char *morpho = NULL;

  if (neut_tess_isreg (Tess[dtess]))
  {
    printf ("\n");
    ut_print_message (2, 2, "Parent tessellation is regularized.\n");
  }

  neut_tess_set_zero (&Dom);

  // case where the domain is void (empty cell of the parent tessellation),
  // which can happen when -morphooptiini is used.  It seems fine to let this
  // pass through.
  if (neut_tess_cell_isvoid (Tess[dtess], dcell))
    return 0;

  if (ut_string_isfilename (In.morpho[level])
      && !ut_file_testformat (In.morpho[level], "tess"))
  {
    neut_mtess_tess_poly_mid (*pMTess, Tess[dtess], dcell, &mid);

    morpho = ut_alloc_1d_char (1000);
    net_multiscale_arg_0d_char_fscanf (In.morpho[level], mid, morpho);
  }
  else
    ut_string_string (In.morpho[level], &morpho);

  // initializing domain (do not use net_tess_poly_tess for the
  // non-periodic case as it will loose domain information needed for
  // multiscale).
  periodic = ut_array_1d_int_sum (In.periodic, 3) && (level == 1);
  if (periodic)
    net_tess_perdomain (In, Tess[dtess], dcell, &Dom);
  else
    neut_tess_poly_tess (Tess[dtess], dcell, &Dom);

  // regular tessellations: cube and square
  if (!strncmp (morpho, "cube", 4) || !strncmp (morpho, "square", 6))
    status =
      net_tess_cube (In, level, morpho, pMTess, Tess, dtess, dcell, TessId,
                     SSet);

  // standard Voronoi/Laguerre tessellation
  else if (!strncmp (morpho, "lamellar", 8))
    status =
      net_tess_lam (In, level, morpho, pMTess, Tess, dtess, dcell, TessId,
                    SSet);

  // standard Voronoi/Laguerre tessellation
  else if (!strncmp (morpho, "tocta", 5))
    status =
      net_tess_tocta (In, level, morpho, pMTess, Tess, dtess, dcell, TessId,
                      SSet);

  // standard Voronoi/Laguerre tessellation
  else if (ut_string_isfilename (morpho)
           && ut_file_testformat (morpho, "tess"))
    status =
      net_tess_file (level, morpho, pMTess, Tess, dtess, dcell, TessId, SSet);

  // other tessellations
  else
    status =
      net_tess_opt (In, level, morpho, Tess, dtess, dcell, TessId, pMTess,
                    SSet);

  // finalizing
  if (!status)
  {
    if (dim == 2 && Tess[TessId].Dim == 3)
      net_tess_3dto2d (Tess + TessId);

    net_tess_finalize (Tess[dtess], SSet[TessId], Tess + TessId);
  }

  neut_tess_free (&Dom);
  ut_free_1d_char (&mid);
  ut_free_1d_char (&morpho);

  return status;
}
