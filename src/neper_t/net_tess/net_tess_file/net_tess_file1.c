/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_file_.h"

int
net_tess_file (int level, char *morpho, struct MTESS *pMTess,
               struct TESS *Tess, int dtess, int dcell, int TessId,
               struct SEEDSET *SSet)
{
  ut_print_message (0, 2, "Reading tessellation...\n");

  net_tess_file_tess (level, morpho, Tess, TessId, SSet);

  net_tess_file_updatedomain (pMTess, Tess, dtess, dcell, TessId);

  net_tess_file_mtess (pMTess, Tess, dtess, dcell, TessId);

  return 0;
}
