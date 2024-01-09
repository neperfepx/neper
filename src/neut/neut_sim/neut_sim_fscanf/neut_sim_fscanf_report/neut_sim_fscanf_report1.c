/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fscanf_report_.h"

void
neut_sim_fscanf_report (char *dir, struct SIM *pSim, char *mode)
{
  int i, status, restart;
  char *filename = NULL, *var = ut_alloc_1d_char (100), *type = NULL;
  char *tmp = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  char *res = NULL, *expr = NULL;
  char *name = NULL;
  int groupqty; // backward compatibility w versions <= 4.2.0

  neut_sim_free (pSim);
  neut_sim_set_zero (pSim);

  status = neut_sim_name_type (dir, &type, &name, &restart);

  if (status == -1)
    ut_print_message (2, 3, "Failed to parse directory.\n");

  filename = ut_string_paste (name, "/report");
  ut_string_string (name, &(*pSim).simdir);

  ut_string_string ("simulation.tess", &(*pSim).tess);
  ut_string_string ("simulation.tesr", &(*pSim).tesr);
  ut_string_string ("simulation.msh", &(*pSim).msh);
  ut_string_string ("simulation.config", &(*pSim).cfg);

  file = ut_file_open (filename, mode);

  while (fscanf (file, "%s", var) != EOF)
  {
    if (!strcmp (var, "number_of_steps"))
    {
      if (fscanf (file, "%d", &(*pSim).StepQty) != 1)
        abort ();

      (*pSim).StepState = ut_alloc_1d_int ((*pSim).StepQty + 1);
      ut_array_1d_int_set ((*pSim).StepState, (*pSim).StepQty + 1, 1);
    }
    else if (!strcmp (var, "printed_steps"))
    {
      int qty, step;

      ut_file_nextlinenbwords (file, &qty);

      ut_array_1d_int_set ((*pSim).StepState, (*pSim).StepQty + 1, 0);
      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%d", &step) != 1)
          abort ();
        (*pSim).StepState[step] = 1;
      }
    }
    else if (!strcmp (var, "number_of_nodes"))
    {
      if (fscanf (file, "%d", &(*pSim).NodeQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_elements"))
    {
      if (fscanf (file, "%d", &(*pSim).EltQty) != 1)
        abort ();
    }
    // number_of_phases and number_of_slip_systems are for backward compatibility ---
    else if (!strcmp (var, "number_of_phases"))
    {
      if (fscanf (file, "%d", &groupqty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_slip_systems"))
      ut_file_skip (file, ut_num_max (1, groupqty));
    // ------------------------------------------------------------------------------
    else if (!strcmp (var, "number_of_partitions"))
    {
      if (fscanf (file, "%d", &(*pSim).PartQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_cells"))
    {
      if (fscanf (file, "%d", &(*pSim).CellQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_elsets"))
    {
      if (fscanf (file, "%d", &(*pSim).ElsetQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "orientation_definition"))
    {
      (*pSim).OriDes = ut_alloc_1d_char (100);

      if (fscanf (file, "%s", (*pSim).OriDes) != 1)
        abort ();
    }
    else if (!strcmp (var, "orientation_space"))
    {
      (*pSim).OriSpace = ut_alloc_1d_char (1000);

      if (fscanf (file, "%s", (*pSim).OriSpace) != 1)
        abort ();
    }
    else if (!strcmp (var, "entities"))
    {
      int qty;
      char *entityexpr = ut_alloc_1d_char (1000);

      ut_file_nextlinenbwords (file, &qty);

      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%s", entityexpr) != 1)
          abort ();

        neut_sim_addentity (pSim, entityexpr);
      }

      ut_free_1d_char (&entityexpr);
    }
    else if (!strcmp (var, "entity_type"))
    {
      int pos;
      char *entity = ut_alloc_1d_char (1000);
      char *type = ut_alloc_1d_char (1000);

      if (fscanf (file, "%s", entity) != 1)
        abort ();

      neut_sim_entity_pos (*pSim, entity, &pos);
      if (pos == -1)
        abort ();

      if (fscanf (file, "%s", type) != 1)
        abort ();

      ut_string_string (type, (*pSim).EntityType + pos);

      ut_free_1d_char (&entity);
      ut_free_1d_char (&type);
    }
    else if (!strcmp (var, "entity_expr"))
    {
      int qty, pos;
      char *entity = ut_alloc_1d_char (1000);
      char *expr = ut_alloc_1d_char (1000);

      if (fscanf (file, "%s", entity) != 1)
        abort ();

      neut_sim_entity_pos (*pSim, entity, &pos);
      if (pos == -1)
        abort ();

      ut_file_nextlinenbwords (file, &qty);

      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%s", expr) != 1)
          abort ();

        neut_sim_entity_addexpr (pSim, entity, expr);
      }

      ut_free_1d_char (&entity);
      ut_free_1d_char (&expr);
    }
    else if (!strncmp (var, "results_", 8))
    {
      int qty;
      char *entity = NULL;
      char *resexpr = ut_alloc_1d_char (1000);

      ut_string_string (var + 8, &entity);
      neut_sim_entity_entity (entity, &entity);

      if (!neut_sim_entity_exist (*pSim, entity))
        neut_sim_addentity (pSim, entity);

      ut_file_nextlinenbwords (file, &qty);

      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%s", resexpr) != 1)
          abort ();

        neut_sim_addres (pSim, entity, resexpr, NULL);
      }

      ut_free_1d_char (&resexpr);
      ut_free_1d_char (&entity);
    }
    else if (!strcmp (var, "restart_id"))
    {
      if (fscanf (file, "%d\n", &(*pSim).RestartId) != 1)
        abort ();
    }
    else if (!strcmp (var, "restart_files"))
    {
      if (fscanf (file, "%d\n", &(*pSim).RestartFiles) != 1)
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

  if (!strcmp (type, "fepx"))
  {
    filename = ut_string_paste (name, "/simulation.config");

    file = ut_file_open (filename, "R");
    while (fscanf (file, "%s", tmp) == 1)
    {
      if (!strcmp (tmp, "read_bcs_from_file"))
        ut_string_string ("simulation.bcs", &(*pSim).bcs);
      else if (!strcmp (tmp, "read_ori_from_file"))
        ut_string_string ("simulation.ori", &(*pSim).ori);
      else if (!strcmp (tmp, "read_phase_from_file"))
        ut_string_string ("simulation.phase", &(*pSim).phase);
    }
    ut_file_close (file, filename, "R");

    ut_free_1d_char (&filename);
  }

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&var);
  ut_free_1d_char (&type);
  ut_free_1d_char (&res);
  ut_free_1d_char (&expr);
  ut_free_1d_char (&name);

  return;
}
