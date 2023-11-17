/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

void
nes_pproc_entity_builtin_odf_writeconfig (struct SIMRES *pSimRes, char *name, struct ODF Odf)
{
  char *hiddenfile = NULL;
  FILE *file = NULL;

  hiddenfile = ut_string_paste3 ((*pSimRes).dir, "/.", name);
  file = ut_file_open (hiddenfile, "W");
  fprintf (file, "%.12f", Odf.sigma * 180 / M_PI);
  ut_file_close (file, hiddenfile, "W");

  ut_free_1d_char (&hiddenfile);

  return;
}
