/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_merge_.h"

void
nes_merge_input (struct SIM Sim, char *sim2)
{
  int i, fileqty;
  char **fullfiles = NULL, **files = NULL;
  char *fullfile2 = ut_alloc_1d_char (1000);

  neut_sim_input_files (Sim, &fullfiles, &files, &fileqty);

  for (i = 0; i < fileqty; i++)
  {
    ut_print_message (0, 3, "%s...\n", files[i]);
    sprintf (fullfile2, "%s", fullfiles[i]);
    ut_string_fnrs (fullfile2, Sim.simdir, sim2, 1);
    ut_file_cp (fullfiles[i], fullfile2);
  }

  ut_free_1d_char (&fullfile2);
  ut_free_2d_char (&fullfiles, fileqty);
  ut_free_2d_char (&files, fileqty);

  return;
}

void
nes_merge_entity_res (struct SIM Sim, char *entity, int step, char *sim2)
{
  int i, resqty;
  char **res = NULL;
  char *dir = NULL;
  char *filename1 = ut_alloc_1d_char (1000);
  char *filename2 = ut_alloc_1d_char (1000);

  neut_sim_entity_res (Sim, entity, &res, &resqty);
  neut_sim_entity_dir (entity, &dir);

  for (i = 0; i < resqty; i++)
  {
    neut_sim_res_file (Sim, "node", Sim.NodeRes[i], filename1);
    sprintf (filename2, "%s/%s/%s/%s.step%d",
             sim2, dir, Sim.NodeRes[i], Sim.NodeRes[i], step);
    ut_file_cp (filename1, filename2);
  }

  ut_free_1d_char (&filename1);
  ut_free_1d_char (&filename2);
  ut_free_2d_char (&res, resqty);
  ut_free_1d_char (&dir);

  return;
}

void
nes_merge_restart (struct SIM Sim, char *sim1, char *sim2)
{
  nes_sim_write_restart (ut_string_paste (sim1, "/restart"), sim2, Sim);

  return;
}

void
nes_merge_post (struct IN_S In, char *sim, int stepqty,
                struct SIM LastSim, struct SIM *pFullSim)
{
  neut_sim_fscanf (sim, pFullSim, "R");
  ut_string_string (In.simdir, &(*pFullSim).simdir);
  (*pFullSim).StepQty = stepqty;
  (*pFullSim).RestartFiles = LastSim.RestartFiles;

  neut_sim_fprintf (In.simdir, *pFullSim, "W");

  return;
}
