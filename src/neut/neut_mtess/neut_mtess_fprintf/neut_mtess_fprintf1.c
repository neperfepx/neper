/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fprintf_.h"

void
neut_mtess_fprintf (FILE * file, struct MTESS MTess, struct TESS *Tess)
{
  neut_mtess_fprintf_header (file, MTess, Tess);

  neut_mtess_fprintf_tess (file, MTess, Tess);

  return;
}
