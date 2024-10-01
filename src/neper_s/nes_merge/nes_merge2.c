/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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
  char *entitydir = NULL;
  char *filename2 = ut_alloc_1d_char (1000);
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  neut_sim_entity_res (Sim, entity, &res, &resqty);
  neut_sim_entity_entitydir (entity, &entitydir);

  for (i = 0; i < resqty; i++)
  {
    neut_sim_simres (Sim, entity, res[i], &SimRes);
    sprintf (filename2, "%s/results/%s/%s/%s.step%d", sim2, entitydir, res[i], res[i], step);
    ut_file_cp (SimRes.file, filename2);
  }

  ut_free_1d_char (&filename2);
  ut_free_2d_char (&res, resqty);
  ut_free_1d_char (&entitydir);
  neut_simres_free (&SimRes);

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

void
nes_merge_restart (struct SIM Sim, char *indir, char *outdir)
{
  int i;
  char *dir = ut_string_paste (outdir, "/restart");
  char *filename = ut_alloc_1d_char (1000);
  char *infile = ut_alloc_1d_char (1000);
  char *outfile = ut_alloc_1d_char (1000);

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir (dir);

  sprintf (filename, "rst%d.control", Sim.RestartId);
  sprintf (infile, "%s/restart/%s", indir, filename);
  sprintf (outfile, "%s/restart/%s", outdir, filename);

  ut_print_message (0, 4, "%s...\n", filename);
  ut_file_cp (infile, outfile);

  ut_print_message (0, 4, "rst%d.field.core*...\n", Sim.RestartId);
  for (i = 1; i <= Sim.PartQty; i++)
  {
    sprintf (filename, "rst%d.field.core%d", Sim.RestartId, i);
    sprintf (infile, "%s/restart/%s", indir, filename);
    sprintf (outfile, "%s/restart/%s", outdir, filename);

    ut_file_cp (infile, outfile);
  }

  ut_free_1d_char (&dir);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&infile);
  ut_free_1d_char (&outfile);

  return;
}
