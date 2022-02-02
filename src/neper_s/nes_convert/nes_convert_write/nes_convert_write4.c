/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_write_.h"

int
nes_convert_write_results_prop (struct SIM Sim, char *var,
                           int *pstartstep)
{
  int status;
  char *filename = ut_alloc_1d_char (1000);
  char *string = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  neut_sim_res_fepxfile (Sim, var, 1, filename);

  if (ut_file_exist (filename))
  {
    file = ut_file_open (filename, "R");

    ut_file_skip (file, 1);
    if (fscanf (file, "%d", pstartstep) != 1)
      abort ();

    ut_file_skip_line (file, 1);
    string = fgets (string, 1000, file);

    ut_file_close (file, filename, "R");

    status = 0;
  }

  else
    status = -1;

  ut_free_1d_char (&string);
  ut_free_1d_char (&filename);

  return status;
}

void
nes_convert_write_results_entity_step (struct IN_S In, struct SIM Sim, char *res,
                                       char *entity, int startstep)
{
  int i, j, k, part;
  int *qty_part = NULL;
  char *fepxvar = ut_alloc_1d_char (10);
  char *post_filepref = ut_alloc_1d_char (1000);
  char *step_filepref = ut_alloc_1d_char (1000);
  char *post_filename = ut_alloc_1d_char (1000);
  char *step_filename = ut_alloc_1d_char (1000);
  char *step_filename0 = ut_alloc_1d_char (1000);
  char *tmp = ut_alloc_1d_char (1000);
  FILE *fp1 = NULL, *fp2 = NULL, *fp3 = NULL;
  char *message = ut_alloc_1d_char (100);
  char *line = ut_alloc_1d_char (1000);
  char *entitydir = NULL;

  neut_sim_entity_entitydir (entity, &entitydir);

  ut_sys_mkdir ("%s/results/%s/%s", In.simdir, entitydir, res);

  if (neut_sim_entityisnode (entity))
    qty_part = Sim.PartNodeQty;
  else if (neut_sim_entityiselt (entity))
    qty_part = Sim.PartEltQty;
  else
    abort ();

  sprintf (post_filepref, "%s/post.%s", Sim.fepxdir, res);
  sprintf (step_filepref, "%s/results/%s/%s/%s.step", In.simdir, entitydir, res, res);

  for (part = 1; part <= Sim.PartQty; part++)
  {
    ut_print_progress (stdout, part, Sim.PartQty, "%3.0f%%",
                       message);

    neut_sim_res_fepxfile (Sim, res, part, post_filename);

    fp1 = ut_file_open (post_filename, "R");

    for (i = 0; i <= Sim.StepQty; ++i)
      if (!Sim.StepState[i])
      {
        sprintf (step_filename, "%s%d", step_filepref, i);
        sprintf (step_filename0, "%s%d", step_filepref, 0);

        fp2 = ut_file_open (step_filename, (part == 1) ? "W" : "A");
        fp3 = ut_file_open (step_filename0, (part == 1) ? "W" : "A");

        if (i >= startstep)
          ut_file_skip_line (fp1, 1);
        for (j = 0; j < qty_part[part]; j++)
        {
          if (i >= startstep)
          {
            line = fgets (line, 1000, fp1);
            if (!line)
              ut_print_message (2, 5, "\nFile `%s': failed to read data for step %d\n", post_filename, i);
            fprintf (fp2, "%s", line);
          }

          // writing the initial state with the same number of data
          if (i > 0 && i == startstep)
          {
            int colqty = ut_string_nbwords (line);
            for (k = 0; k < colqty - 1; k++)
              fprintf (fp3, "0 ");
            fprintf (fp3, "0\n");
          }
        }
        ut_file_close (fp2, step_filename, "A");
        ut_file_close (fp3, step_filename0, "A");
      }

    ut_file_close (fp1, post_filename, "R");
  }

  ut_free_1d_char (&post_filepref);
  ut_free_1d_char (&step_filepref);
  ut_free_1d_char (&post_filename);
  ut_free_1d_char (&step_filename);
  ut_free_1d_char (&step_filename0);
  ut_free_1d_char (&fepxvar);
  ut_free_1d_char (&message);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&line);
  ut_free_1d_char (&entitydir);

  return;
}
