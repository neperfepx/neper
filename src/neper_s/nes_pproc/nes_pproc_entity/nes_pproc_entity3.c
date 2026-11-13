/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_.h"

// getting the steps of files <simfile>.step<step> (*pmaxstep is -1 if there is none)
void
nes_pproc_entity_file_steps (char *simfile, int **pprinted, int *pmaxstep)
{
  int i, step, fileqty = 0;
  char *dir = NULL, *basename = NULL, **files = NULL;
  char *slash = strrchr (simfile, '/');

  ut_string_string (slash ? simfile : ".", &dir);
  if (slash)
    dir[slash - simfile] = '\0';
  ut_string_string (slash ? slash + 1 : simfile, &basename);

  (*pmaxstep) = -1;

  if (ut_sys_isdir ("%s", dir))
  {
    ut_dir_allfiles (dir, &files, &fileqty);

    for (i = 0; i < fileqty; i++)
      if (!neut_sim_fscanf_steps_file (basename, files[i], &step))
      {
        if (step > *pmaxstep)
        {
          (*pprinted) = ut_realloc_1d_int_zero (*pprinted, step + 1, step - *pmaxstep);
          (*pmaxstep) = step;
        }

        (*pprinted)[step] = 1;
      }
  }

  ut_free_1d_char (&dir);
  ut_free_1d_char (&basename);
  ut_free_2d_char (&files, fileqty);

  return;
}

// setting the steps of the simulation directory from those of the files, if it has no results,
// and testing them otherwise
void
nes_pproc_entity_file_setsteps (struct SIM *pSim, char *simfile, int *printed, int maxstep)
{
  int i, step, resqty = 0, insim, infile;

  for (i = 0; i < (*pSim).EntityQty; i++)
    resqty += (*pSim).EntityResQty[i];

  if (!resqty)
  {
    if (!printed[0])
      ut_print_message (2, 5, "\nFiles `%s.step*' have no step 0.\n", simfile);

    (*pSim).StepQty = maxstep;
    ut_free_1d_int (&(*pSim).StepState);
    (*pSim).StepState = ut_alloc_1d_int (maxstep + 1);
    ut_array_1d_int_memcpy (printed, maxstep + 1, (*pSim).StepState);
  }

  else
    for (step = 0; step <= ut_num_max (maxstep, (*pSim).StepQty); step++)
    {
      insim = !neut_sim_setstep (pSim, step);
      infile = step <= maxstep && printed[step];

      if (insim != infile)
        ut_print_message (2, 5, infile ? "\nFiles `%s.step*' have step %d, which is not a step of the simulation directory.\n"
                                       : "\nFiles `%s.step*' do not have step %d of the simulation directory.\n",
                          simfile, step);
    }

  return;
}
