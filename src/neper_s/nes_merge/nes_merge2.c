/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nes_merge_.h"

void
nes_merge_input (struct SIM Sim, char *sim2)
{
  int i, fileqty;
  char **fullfiles = NULL, **files = NULL;
  char *fullfile2 = NULL;

  neut_sim_input_files (Sim, &fullfiles, &files, &fileqty);

  for (i = 0; i < fileqty; i++)
  {
    ut_print_message (0, 3, "%s...\n", files[i]);
    ut_string_sprintf (&fullfile2, "%s%s", sim2, fullfiles[i] + strlen (Sim.simdir));
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
  char *filename2 = NULL;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  neut_sim_entity_res (Sim, entity, &res, &resqty);
  neut_sim_entity_entitydir (entity, &entitydir);

  for (i = 0; i < resqty; i++)
  {
    neut_sim_simres (Sim, entity, res[i], &SimRes);
    ut_string_sprintf (&filename2, "%s/results/%s/%s/%s.step%d", sim2, entitydir, res[i], res[i], step);
    ut_file_cp (SimRes.file, filename2);
  }

  ut_free_1d_char (&filename2);
  ut_free_2d_char (&res, resqty);
  ut_free_1d_char (&entitydir);
  neut_simres_free (&SimRes);

  return;
}

void
nes_merge_post (struct IN_S In, char *sim, struct SIM *pFullSim)
{
  struct SIM Sim;

  neut_sim_set_zero (&Sim);

  // writing the entity and result definitions of the first simulation
  neut_sim_fscanf (sim, &Sim);
  ut_string_string (In.simdir, &Sim.simdir);
  neut_sim_fprintf (Sim, "W");

  neut_sim_fscanf (In.simdir, pFullSim);

  neut_sim_free (&Sim);

  return;
}

void
nes_merge_restart (struct SIM Sim, char *indir, char *outdir)
{
  int i, fileqty = 0;
  char *dir = ut_string_paste (outdir, "/restart");
  char *prefix = NULL, *infile = NULL, *outfile = NULL, **files = NULL;

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir ("%s", dir);

  ut_string_sprintf (&infile, "%s/restart", indir);
  ut_dir_allfiles (infile, &files, &fileqty);
  ut_array_1d_pchar_sort (files, fileqty);

  // copying the restart files, rst<id>.control and rst<id>.field.core* (FEPX 1 and 2), or
  // restart.rank<r> (FEPX 3)
  ut_string_sprintf (&prefix, "rst%d.", Sim.RestartId);
  for (i = 0; i < fileqty; i++)
    if (!strncmp (files[i], prefix, strlen (prefix)) || !strncmp (files[i], "restart.rank", 12))
    {
      ut_print_message (0, 4, "%s...\n", files[i]);
      ut_string_sprintf (&infile, "%s/restart/%s", indir, files[i]);
      ut_string_sprintf (&outfile, "%s/restart/%s", outdir, files[i]);
      ut_file_cp (infile, outfile);
    }

  ut_free_1d_char (&dir);
  ut_free_1d_char (&prefix);
  ut_free_1d_char (&infile);
  ut_free_1d_char (&outfile);
  ut_free_2d_char (&files, fileqty);

  return;
}
