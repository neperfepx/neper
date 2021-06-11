/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the tori-level directory. */

#include "neut_tesr_ori_.h"

void
neut_tesr_cell_orianiso (struct TESR Tesr, int cell, double **evect,
                         double *eval)
{
  struct OL_SET Set;

  neut_tesr_cell_olset (Tesr, cell, &Set);

  ol_set_disoriset (Set, &Set);

  ol_set_aniso (Set, evect, eval);

  ol_set_free (Set);

  return;
}

void
neut_tesr_cell_orianiso_delta (struct TESR Tesr, int cell, double **evect,
                               double *delta)
{
  struct OL_SET Set;
  double *eval = ut_alloc_1d (3);
  int i, j, *distriblength = ut_alloc_1d_int (3);
  double step, *distribfirstx = ut_alloc_1d(3), **distrib = ut_alloc_1d_pdouble (3);
  struct FCT Fct, CFct;

  ut_fct_set_zero (&Fct);
  ut_fct_set_zero (&CFct);

  neut_tesr_cell_olset (Tesr, cell, &Set);

  ol_set_disoriset (Set, &Set);

  ol_set_aniso (Set, evect, eval);

  step = eval[0] / 1000;

  ol_set_aniso_thetadistrib (Set, evect, step, distrib, distribfirstx, distriblength);

  for (i = 0; i < 3; i++)
  {
    ut_fct_set_array (&Fct, distrib[i], distriblength[i], distribfirstx[i], step);
    ut_fct_integralfct (Fct, &CFct);

    double mode1 = -DBL_MAX, mode2 = -DBL_MAX;
    for (j = 0; j < CFct.size; j++)
    {
      if (mode1 == -DBL_MAX && CFct.y[j] > 0.25)
        mode1 = CFct.x[j];
      if (mode2 == -DBL_MAX && CFct.y[j] > 0.75)
      {
        mode2 = CFct.x[j];
        break;
      }
    }

    delta[i] = mode2 - mode1;
  }

  ol_set_free (Set);
  ut_free_2d (&distrib, 3);
  ut_free_1d (&distribfirstx);
  ut_fct_free (&Fct);
  ut_free_1d (&eval);

  return;
}
