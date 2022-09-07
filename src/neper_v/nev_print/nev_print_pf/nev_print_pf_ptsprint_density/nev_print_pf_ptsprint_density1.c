/* This file is part of the 'hermes' program. */
/* Copyright (C) 2007-2010, Romain Quey. */
/* See the COPYINPFG file in the top-level directory. */

#include "nev_print_pf_ptsprint_density_.h"

void
nev_print_pf_ptsprint_density (FILE *file, char *basename, struct PRINT Print,
                      struct DATA Data,
                      double **borderpts, int borderptqty, double **pos,
                      double *wgt, int pqty)
{
  double **density = NULL, **ldensity = NULL, min, max;
  double ***grid = NULL, ***lgrid = NULL, ***Pgrid = NULL;

  nev_print_pf_ptsprint_density_grid (Print, &grid, &density, &Pgrid, &lgrid, &ldensity);
  nev_print_pf_ptsprint_density_odf (Print, grid, Pgrid, borderpts, borderptqty, pos,
                            wgt, pqty, density, &min, &max);
  nev_print_pf_ptsprint_density_ldensity (Print, density, lgrid, ldensity);
  nev_print_pf_ptsprint_density_write (file, basename, Print, Data, lgrid, ldensity, min, max);
  nev_print_pf_ptsprint_density_write_mask (file, Print, borderpts, borderptqty);
  nev_print_pf_ptsprint_density_write_text0 (file, Print);

  ut_free_3d (&grid, Print.pfgridsize, Print.pfgridsize);
  ut_free_3d (&Pgrid, Print.pfgridsize, Print.pfgridsize);
  ut_free_2d (&density, Print.pfgridsize);
  ut_free_3d (&lgrid, Print.pfgridsize + 2, Print.pfgridsize + 2);
  ut_free_2d (&ldensity, Print.pfgridsize + 2);

  return;
}
