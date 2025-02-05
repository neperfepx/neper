/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_.h"

int
net_tess (struct IN_T In, int level, struct TESS *Tess, int dtess, int dcell,
          struct SEEDSET *SSet, int TessId, struct MTESS *pMTess)
{
  int status = -1, periodic;
  int dim = (In.levelqty > 1 && In.dim == 2) ? 3 : In.dim;
  struct TESS Dom;
  char *morpho = NULL, *ori = NULL;

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

  // Generating morphology -----------------------------------------------------

  neut_mtess_argument_process (*pMTess, Tess, dtess, dcell, In.morpho[level], &morpho);

  // initializing domain (do not use net_tess_poly_tess for the
  // non-periodic case as it will loose domain information needed for
  // multiscale).
  periodic = ut_array_1d_int_sum (In.periodic, 3) && (level == 1);
  if (periodic)
    net_tess_perdomain (In, Tess[dtess], dcell, &Dom);
  else
    neut_tess_poly_tess (Tess[dtess], dcell, &Dom);

  if (!strcmp (In.n[level], "1") && In.levelqty == 1)
    status = net_tess_onecell (In, pMTess, Tess, dtess, dcell, TessId, SSet);

  // regular tessellations: cube and square
  else if (!strncmp (morpho, "cube", 4) || !strncmp (morpho, "square", 6))
    status = net_tess_cube (In, level, morpho, pMTess, Tess, dtess, dcell, TessId, SSet);

  // lamellar tessellation
  else if (!strncmp (morpho, "lamellar", 8))
    status = net_tess_lam (In, level, morpho, pMTess, Tess, dtess, dcell, TessId, SSet);

  // truncated-octahedron tessellation
  else if (!strncmp (morpho, "tocta", 5))
    status = net_tess_tocta (In, morpho, pMTess, Tess, dtess, dcell, TessId, SSet);

  // tessellation read from file
  else if (ut_string_isfilename (morpho) && ut_file_testformat (morpho, "tess"))
    status = net_tess_file (level, morpho, pMTess, Tess, dtess, dcell, TessId, SSet);

  else
    status = net_tess_opt (In, level, "morpho", morpho, Tess, dtess, dcell, TessId, pMTess, SSet);

  // Generating cell orientations ----------------------------------------------

  // won't take "file(...)" argument
  // neut_mtess_argument_process (*pMTess, Tess, dtess, dcell, In.ori[level], &ori);
  ut_string_string (In.ori[level], &ori);

  ut_print_message (0, 1, "Generating crystal orientations...\n");
  net_ori (In, level, *pMTess, Tess, SSet, dtess, dcell, SSet + TessId, 2);
  if (Tess[TessId].CellCrySym && !strcmp (Tess[TessId].CellCrySym, "triclinic") && strcmp (SSet[TessId].crysym, "triclinic"))
    ut_string_string (SSet[TessId].crysym, &(Tess[TessId].CellCrySym));

  if (!strncmp (ori, "odf", 3) && !strcmp (In.orisampling[level], "uniform"))
    status = net_tess_opt (In, level, "ori", ut_string_paste ("ori:", ori), Tess, dtess, dcell, TessId, pMTess, SSet);

  // Finalizing ----------------------------------------------------------------

  if (!status)
  {
    if (dim == 2 && Tess[TessId].Dim == 3)
      net_tess_3dto2d (Tess + TessId);

    net_tess_finalize (Tess[dtess], SSet[TessId], Tess + TessId);
  }

  neut_tess_free (&Dom);
  ut_free_1d_char (&morpho);
  ut_free_1d_char (&ori);

  return status;
}
