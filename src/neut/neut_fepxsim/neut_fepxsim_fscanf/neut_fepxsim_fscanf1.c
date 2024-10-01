/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_fepxsim_fscanf_.h"

void
neut_fepxsim_fscanf (char *dir, struct FEPXSIM *pFSim, char *mode)
{
  int i, restart;
  char *filename = NULL, *var = ut_alloc_1d_char (100), *type = NULL;
  char *tmp = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  char *name = NULL;
  char *res = ut_alloc_1d_char (1000);

  neut_fepxsim_free (pFSim);
  neut_fepxsim_set_zero (pFSim);

  neut_sim_name_type (dir, &type, &name, &restart);

  ut_string_string (name, &(*pFSim).fepxdir);
  (*pFSim).RestartId = restart;

  if (!restart)
    filename = ut_string_paste ((*pFSim).fepxdir, "/post.report");
  else
  {
    filename = ut_alloc_1d_char (1000);
    sprintf (filename, "%s/post.report.rst%d", (*pFSim).fepxdir, restart);
  }

  if (ut_file_exist ("%s/rst%d.control", (*pFSim).fepxdir, restart))
    (*pFSim).RestartFiles = 1;

  if (ut_file_exist ("%s/simulation.tess", (*pFSim).fepxdir))
    ut_string_string ("simulation.tess", &(*pFSim).tess);
  if (ut_file_exist ("%s/simulation.tesr", (*pFSim).fepxdir))
    ut_string_string ("simulation.tesr", &(*pFSim).tesr);
  if (ut_file_exist ("%s/simulation.msh", (*pFSim).fepxdir))
    ut_string_string ("simulation.msh", &(*pFSim).msh);

  if (ut_file_exist ("%s/simulation.cfg", (*pFSim).fepxdir))
    ut_string_string ("simulation.cfg", &(*pFSim).cfg);
  else if (ut_file_exist ("%s/simulation.config", (*pFSim).fepxdir))
    ut_string_string ("simulation.config", &(*pFSim).cfg);

  file = ut_file_open (filename, mode);

  while (fscanf (file, "%s", var) != EOF)
  {
    if (!strcmp (var, "number_of_steps"))
    {
      if (fscanf (file, "%d", &(*pFSim).StepQty) != 1)
        abort ();

      (*pFSim).StepState = ut_alloc_1d_int ((*pFSim).StepQty + 1);
      ut_array_1d_int_set ((*pFSim).StepState, (*pFSim).StepQty + 1, 1);
    }
    else if (!strcmp (var, "printed_steps"))
    {
      int qty, step;

      ut_file_nextlinenbwords (file, &qty);

      ut_array_1d_int_set ((*pFSim).StepState + 1, (*pFSim).StepQty, 0);
      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%d", &step) != 1)
          abort ();
        (*pFSim).StepState[step] = 1;
      }
    }
    else if (!strcmp (var, "number_of_partitions"))
    {
      if (fscanf (file, "%d", &(*pFSim).PartQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_nodes"))
    {
      if (fscanf (file, "%d", &(*pFSim).NodeQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_elements"))
    {
      if (fscanf (file, "%d", &(*pFSim).EltQty) != 1)
        abort ();
    }
    // number_of_phases and number_of_slip_systems are for backward compatibility ---
    else if (!strcmp (var, "number_of_phases")
          || !strcmp (var, "number_of_slip_systems"))
      ut_file_skip_line (file, 1);
    else if (!strcmp (var, "number_of_elements_bypartition"))
    {
      (*pFSim).PartEltQty = ut_alloc_1d_int ((*pFSim).PartQty + 1);
      if (ut_array_1d_int_fscanf (file, (*pFSim).PartEltQty + 1, (*pFSim).PartQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_nodes_bypartition"))
    {
      (*pFSim).PartNodeQty = ut_alloc_1d_int ((*pFSim).PartQty + 1);
      if (ut_array_1d_int_fscanf (file, (*pFSim).PartNodeQty + 1, (*pFSim).PartQty) != 1)
          abort ();
    }
    else if (!strcmp (var, "orientation_definition"))
    {
      (*pFSim).OriDes = ut_alloc_1d_char (100);

      if (fscanf (file, "%s", (*pFSim).OriDes) != 1)
        abort ();
    }
    else if (!strcmp (var, "results_nodes"))
    {
      ut_file_nextlinenbwords (file, &(*pFSim).NodeResQty);

      (*pFSim).NodeRes = ut_alloc_1d_pchar ((*pFSim).NodeResQty);

      for (i = 0; i < (*pFSim).NodeResQty; i++)
      {
        if (fscanf (file, "%s", res) != 1)
          abort ();

        ut_string_string (res, (*pFSim).NodeRes + i);
      }
    }
    else if (!strcmp (var, "results_elements"))
    {
      ut_file_nextlinenbwords (file, &(*pFSim).EltResQty);

      (*pFSim).EltRes = ut_alloc_1d_pchar ((*pFSim).EltResQty);

      for (i = 0; i < (*pFSim).EltResQty; i++)
      {
        if (fscanf (file, "%s", res) != 1)
          abort ();

        ut_string_string (res, (*pFSim).EltRes + i);
      }
    }
    else if (!strcmp (var, "restart_id"))
    {
      if (fscanf (file, "%d\n", &(*pFSim).RestartId) != 1)
        abort ();
    }
    else if (!strcmp (var, "restart_files"))
    {
      if (fscanf (file, "%d\n", &(*pFSim).RestartFiles) != 1)
        abort ();
    }
    else
    {
      printf ("var = %s\n", var);
      ut_print_message (2, 3, "Failed to read file.\n");
    }
  }

  ut_file_close (file, filename, mode);

  ut_free_1d_char (&filename);

  filename = ut_string_paste3 (name, "/", (*pFSim).cfg);

  file = ut_file_open (filename, "R");
  while (fscanf (file, "%s", tmp) == 1)
  {
    if (!strcmp (tmp, "read_bcs_from_file"))
      ut_string_string ("simulation.bcs", &(*pFSim).bcs);
    else if (!strcmp (tmp, "read_ori_from_file"))
      ut_string_string ("simulation.ori", &(*pFSim).ori);
    else if (!strcmp (tmp, "read_phase_from_file"))
      ut_string_string ("simulation.phase", &(*pFSim).phase);
  }
  ut_file_close (file, filename, "R");

  ut_free_1d_char (&filename);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&var);
  ut_free_1d_char (&type);
  ut_free_1d_char (&res);
  ut_free_1d_char (&name);

  return;
}
