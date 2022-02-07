/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_write_.h"

int
nes_convert_write_inputs_file (struct IN_S In, struct FEPXSIM FSim, char *filename)
{
  int status = -1;
  char *infile = ut_string_paste3 (FSim.fepxdir, "/", filename);
  char *outfile = ut_string_paste3 (In.simdir, "/inputs/", filename);

  infile = ut_string_paste3 (FSim.fepxdir, "/", filename);

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
    sprintf (command, "cp %s/%s %s/inputs/", FSim.fepxdir, filename, In.simdir);
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
nes_convert_write_results_entity (struct IN_S In, struct FEPXSIM *pFSim, char *entity)
{
  int i, j, startstep, status, resqty;
  char *dir = NULL;
  char **res = NULL;

  dir = ut_string_paste3 (In.simdir, "/results/", entity);
  if (!strcmp (entity, "nodes"))
  {
    resqty = (*pFSim).NodeResQty;
    res = (*pFSim).NodeRes;
  }
  else if (!strcmp (entity, "elts"))
  {
    resqty = (*pFSim).EltResQty;
    res = (*pFSim).EltRes;
  }
  else
    abort ();

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir ("%s", dir);

  for (i = 0; i < resqty; i++)
  {
    status = nes_convert_write_results_prop (*pFSim, res[i], &startstep);

    if (!status)
    {
      ut_print_message (0, 4, "%s ", res[i]);
      for (j = 0; j < 49 - (int) strlen (res[i]); j++)
        printf (".");
      printf (" ");

      nes_convert_write_results_entity_step (In, *pFSim, res[i],
                                             entity, startstep);
    }
  }

  ut_dir_closemessage (dir, "w");

  ut_free_1d_char (&dir);
  // do not free res

  return;
}
