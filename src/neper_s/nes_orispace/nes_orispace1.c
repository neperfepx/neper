/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nes_orispace_.h"

void
nes_orispace (struct IN_S In, struct SIM *pSim)
{
  char *filename = NULL;

  ut_file_squashname (In.orispace, &(*pSim).OriSpace);

  ut_sys_mkdir ("%s/orispace", (*pSim).simdir);

  filename = ut_string_paste3 ((*pSim).simdir, "/orispace/", (*pSim).OriSpace);

  ut_file_cp (In.orispace, filename);

  neut_sim_fprintf (In.simdir, *pSim, "W");

  ut_free_1d_char (&filename);

  return;
}
