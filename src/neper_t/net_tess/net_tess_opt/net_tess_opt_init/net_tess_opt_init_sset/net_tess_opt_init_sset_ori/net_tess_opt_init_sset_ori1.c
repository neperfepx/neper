/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori (struct IN_T In, int level, struct MTESS MTess,
                            struct TESS *Tess, int dtess, int dcell,
                            struct SEEDSET *SSet, struct SEEDSET *pSSet)
{
  ut_print_message (0, 2, "Generating crystal orientations...\n");
  net_ori (In, level, MTess, Tess, SSet, dtess, dcell, pSSet, 3);

  return;
}
