/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_sim_fscanf_.h"

void
neut_sim_fscanf (char *dir, struct SIM *pSim, char *mode)
{
  int level;
  char *string = ut_alloc_1d_char (1000);
  char *version = NULL;
  char *sim = NULL;
  FILE *file = NULL;

  neut_sim_fscanf_version (dir, &version);

  if (!strcmp (version, "report"))
    neut_sim_fscanf_report (dir, pSim, mode);

  else
  {
    sim = ut_string_paste (dir, "/.sim");
    file = ut_file_open (sim, mode);

    neut_sim_reset (pSim);

    ut_string_string (dir, &(*pSim).simdir);

    neut_sim_fscanf_head (file, &version);

    while (!ut_file_nextstring_sectionlevel (file, &level) && level == 2)
    {
      ut_file_nextstring (file, string);

      if (!strcmp (string, "**input"))
        neut_sim_fscanf_input (pSim, file);

      else if (!strcmp (string, "**general"))
        neut_sim_fscanf_general (pSim, file);

      else if (!strcmp (string, "**entity"))
        neut_sim_fscanf_entity (pSim, file);

      else if (!strcmp (string, "**orispace"))
        neut_sim_fscanf_orispace (pSim, file);

      else if (!strcmp (string, "**step"))
        neut_sim_fscanf_step (pSim, file);

      else
        ut_print_message (2, 2, "Could not read field `%s'.\n", string);
    }

    neut_sim_fscanf_foot (file);

    ut_file_close (file, sim, mode);

    ut_free_1d_char (&sim);
  }

  ut_free_1d_char (&version);
  ut_free_1d_char (&string);

  return;
}
