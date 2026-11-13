/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

// file of an auxiliary ODF result (.<expr>_<aux>.step*) from the file of the
// ODF result (<expr>.step*)
void
nes_pproc_entity_builtin_odf_auxfile (char *file, char *expr, char *aux, char **pfilename)
{
  char *expr_step = NULL, *expr2_step = NULL;

  ut_string_sprintf (&expr_step, (char *) "%s.step", expr);
  ut_string_sprintf (&expr2_step, (char *) ".%s_%s.step", expr, aux);

  ut_string_string (file, pfilename);
  (*pfilename) = ut_realloc_1d_char (*pfilename, strlen (*pfilename) + strlen (expr2_step) + 1);
  ut_string_fnrs (*pfilename, expr_step, expr2_step, 1);

  ut_free_1d_char (&expr_step);
  ut_free_1d_char (&expr2_step);

  return;
}

// equivalent value of a symmetric tensor result (11, 22, 33, 23, 31, 12),
// as computed by FEPX: sqrt (3/2 t':t') for a stress and sqrt (2/3 t':t')
// otherwise (strain, deformation rate, etc.), where t' is the deviatoric part
void
nes_pproc_entity_builtin_elsets_gen_eq (char *res, double *t, double *peq)
{
  double tr = (t[0] + t[1] + t[2]) / 3;
  double prod = (t[0] - tr) * (t[0] - tr) + (t[1] - tr) * (t[1] - tr) + (t[2] - tr) * (t[2] - tr)
              + 2 * (t[3] * t[3] + t[4] * t[4] + t[5] * t[5]);

  (*peq) = sqrt ((!strncmp (res, (char *) "stress", 6) ? 1.5 : 2. / 3) * prod);

  return;
}
