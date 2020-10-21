/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_write_.h"

int
nes_sim_write_inputs_file (struct IN_S In, struct SIM Sim, char *filename)
{
  int status = -1;
  char *infile = ut_string_paste3 (Sim.fepxdir, "/", filename);
  char *outfile = ut_string_paste3 (In.simdir, "/inputs/", filename);

  if (ut_file_exist (infile))
  {
    ut_print_message (0, 4, "%s...\n", filename);
    ut_file_cp (infile, outfile);

    status = 0;
  }

  else if (strstr (infile, "*"))
  {
    ut_print_message (0, 4, "%s...\n", filename);
    char *command = ut_alloc_1d_char (1000);
    sprintf (command, "cp %s/%s %s/inputs/", Sim.fepxdir, filename, In.simdir);
    status = system (command);
    ut_free_1d_char (&command);
  }

  else
    status = -1;

  ut_free_1d_char (&infile);
  ut_free_1d_char (&outfile);

  return status;
}

void
nes_sim_write_results_entity (struct IN_S In, struct SIM *pSim, char *entity)
{
  int i, j, startstep, colqty, status, resqty, *reswritten = NULL;
  char *dir = NULL;
  char **res = NULL;

  if (neut_sim_entityisnode (entity))
  {
    dir = ut_string_paste (In.simdir, "/results/nodes");
    resqty = (*pSim).NodeResQty;
    res = (*pSim).NodeRes;
    reswritten = (*pSim).NodeResWritten;
  }
  else if (neut_sim_entityiselt (entity))
  {
    dir = ut_string_paste (In.simdir, "/results/elements");
    resqty = (*pSim).EltResQty;
    res = (*pSim).EltRes;
    reswritten = (*pSim).EltResWritten;
  }
  else
    abort ();

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir ("%s", dir);

  for (i = 0; i < resqty; i++)
  {
    status = nes_sim_write_results_prop (*pSim, res[i], &startstep, &colqty);

    if (!status)
    {
      ut_print_message (0, 4, "%s ", res[i]);
      for (j = 0; j < 49 - (int) strlen (res[i]); j++)
        printf (".");
      printf (" ");

      nes_sim_write_results_entity_step (In, *pSim, res[i], entity, startstep, colqty);

      reswritten[i] = 1;
      neut_sim_fprintf (In.simdir, *pSim, "W");
    }
  }

  ut_dir_closemessage (dir, "w");

  ut_free_1d_char (&dir);
  // do not free res
  // do not free reswritten

  return;
}
