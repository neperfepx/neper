/* This file is part of the 'hermes' program. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYINPFG file in the top-level directory. */

#include "nev_print_pf_ptsprint_density_.h"

void
nev_print_pf_ptsprint_density (struct IN_V In, struct PF Pf, FILE *file, char *basename, struct PRINT Print,
                      struct DATA Data,
                      double **pos,
                      double *wgt, int pqty)
{
  double **density = NULL, **ldensity = NULL, min, max;

  nev_print_pf_ptsprint_density_grid (Pf, &density, &ldensity);
  nev_print_pf_ptsprint_density_odf (Pf, pos, wgt, pqty, density, &min, &max);
  nev_print_pf_ptsprint_density_ldensity (Pf, density, ldensity);
  nev_print_pf_ptsprint_density_write (Pf, Print, file, basename, Data, ldensity, min, max);
  nev_print_pf_ptsprint_density_write_mask (In, file, Pf);
  // nev_print_pf_ptsprint_density_write_text0 (file, Print);

  ut_free_2d (&density, Pf.gridsize[0]);
  ut_free_2d (&ldensity, Pf.gridsize[0] + 2);

  return;
}
