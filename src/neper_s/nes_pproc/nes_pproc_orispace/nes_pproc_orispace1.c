/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nes_pproc_orispace_.h"

void
nes_pproc_orispace (struct IN_S In, struct SIM *pSim)
{
  int varqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (In.orispace, &fct, &vars, &vals, &varqty);

  // rodrigues(fr<N>)
  if (strcmp (fct, "rodrigues") || varqty != 1 || vars[0] || strncmp (vals[0], "fr", 2)
      || !vals[0][2] || strspn (vals[0] + 2, "0123456789") != strlen (vals[0] + 2))
    ut_print_message (2, 2, "Cannot process `%s' (expected rodrigues(fr<N>)).\n", In.orispace);

  // only recording the names: the space is read by the results that need it
  ut_string_string (fct, &(*pSim).Sp.space);
  ut_string_string (vals[0], &(*pSim).Sp.fr);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}
