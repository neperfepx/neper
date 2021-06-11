/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_write_.h"

int
nes_sim_write_results_prop (struct SIM Sim, char *var,
                           int *pstartstep, int *pcolqty)
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
    (*pcolqty) = ut_string_nbwords (string);

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
nes_sim_write_results_entity_step (struct IN_S In, struct SIM Sim, char *res,
                                   char *entity, int startstep, int colqty)
{
  int i, j, part;
  int *qty_part = NULL;
  char *fepxvar = ut_alloc_1d_char (10);
  char *post_filepref = ut_alloc_1d_char (1000);
  char *step_filepref = ut_alloc_1d_char (1000);
  char *post_filename = ut_alloc_1d_char (1000);
  char *step_filename = ut_alloc_1d_char (1000);
  char *tmp = ut_alloc_1d_char (1000);
  FILE *fp1 = NULL, *fp2 = NULL;
  char *message = ut_alloc_1d_char (100);
  char *line = ut_alloc_1d_char (1000);
  double *zeros = ut_alloc_1d (colqty);
  char *dir = ut_alloc_1d_char (100);

  neut_sim_entity_dir (entity, &dir);

  ut_sys_mkdir ("%s/%s/%s", In.simdir, dir, res);

  if (neut_sim_entityisnode (entity))
    qty_part = Sim.PartNodeQty;
  else if (neut_sim_entityiselt (entity))
    qty_part = Sim.PartEltQty;
  else
    abort ();

  sprintf (post_filepref, "%s/post.%s", Sim.fepxdir, res);
  sprintf (step_filepref, "%s/%s/%s/%s.step", In.simdir, dir, res, res);

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

        fp2 = ut_file_open (step_filename, (part == 1) ? "W" : "A");

        if (i >= startstep)
          ut_file_skip_line (fp1, 1);
        for (j = 0; j < qty_part[part]; j++)
        {
          if (i >= startstep)
          {
            line = fgets (line, 1000, fp1);
            fprintf (fp2, "%s", line);
          }
          else
            ut_array_1d_fprintf (fp2, zeros, colqty, "%.0f");
        }
        ut_file_close (fp2, step_filename, "A");
      }

    ut_file_close (fp1, post_filename, "R");
  }

  ut_free_1d_char (&post_filepref);
  ut_free_1d_char (&step_filepref);
  ut_free_1d_char (&post_filename);
  ut_free_1d_char (&fepxvar);
  ut_free_1d_char (&message);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&line);
  ut_free_1d (&zeros);
  ut_free_1d_char (&dir);

  return;
}
