/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_sim_fscanf_.h"

void
neut_sim_fscanf (char *dir, struct SIM *pSim)
{
  neut_sim_reset (pSim);

  ut_string_string (dir, &(*pSim).simdir);

  // legacy .sim file: migrating its definitions to the directory, then
  // ignoring it
  if (ut_file_exist ("%s/.sim", dir))
    neut_sim_fscanf_legacy (dir);

  neut_sim_fscanf_inputs (pSim);

  neut_sim_fscanf_steps (pSim);

  neut_sim_fscanf_results (pSim);

  neut_sim_fscanf_restart (pSim);

  return;
}
