/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fscanf_.h"

// reading the definitions of a legacy .sim file, and writing those that
// are missing from the directory
void
neut_sim_fscanf_legacy (char *dir)
{
  int i, j, level;
  char *string = ut_alloc_1d_char (1000);
  char *filename = ut_string_paste (dir, "/.sim");
  char *entitydir = NULL;
  FILE *file = NULL;
  struct SIM Sim;

  neut_sim_set_zero (&Sim);
  ut_string_string (dir, &Sim.simdir);

  file = ut_file_open (filename, "R");

  neut_sim_fscanf_legacy_head (file);

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 2)
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "**entity"))
      neut_sim_fscanf_legacy_entity (&Sim, file);
    else
      neut_sim_fscanf_legacy_skip (file);
  }

  neut_sim_fscanf_legacy_foot (file);

  ut_file_close (file, filename, "R");

  if (!access (dir, W_OK))
    for (i = 0; i < Sim.EntityQty; i++)
    {
      neut_sim_entity_entitydir (Sim.Entities[i], &entitydir);

      if (!neut_sim_entity_internal (Sim.Entities[i])
          && !ut_file_exist ("%s/results/%s/%s.def", dir, entitydir, entitydir))
      {
        ut_print_message (1, 3, "Entity `%s' is defined in legacy file .sim.  Writing `results/%s/%s.def'...\n",
                          Sim.Entities[i], entitydir, entitydir);
        neut_sim_fprintf_entity (Sim, i, "W");
      }

      for (j = 0; j < Sim.EntityResQty[i]; j++)
        if (!ut_file_exist ("%s/results/%s/%s/%s.def", dir, entitydir, Sim.EntityRes[i][j],
                            Sim.EntityRes[i][j]))
          neut_sim_fprintf_res (Sim, i, j, "W");
    }

  neut_sim_free (&Sim);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&entitydir);
  ut_free_1d_char (&string);

  return;
}

void
neut_sim_fscanf_inputs (struct SIM *pSim)
{
  int fileqty = 0;
  char *dir = ut_string_paste ((*pSim).simdir, "/inputs");
  char **files = NULL;

  if (ut_sys_isdir ("%s", dir))
  {
    ut_dir_allfiles (dir, &files, &fileqty);
    ut_array_1d_pchar_sort (files, fileqty);

    neut_sim_fscanf_inputs_file (files, fileqty, "tess", &(*pSim).tess);
    neut_sim_fscanf_inputs_file (files, fileqty, "tesr", &(*pSim).tesr);
    neut_sim_fscanf_inputs_file (files, fileqty, "msh", &(*pSim).msh);
    neut_sim_fscanf_inputs_file (files, fileqty, "bcs", &(*pSim).bcs);
    neut_sim_fscanf_inputs_file (files, fileqty, "ori", &(*pSim).ori);
    neut_sim_fscanf_inputs_file (files, fileqty, "opt", &(*pSim).opt);
    neut_sim_fscanf_inputs_file (files, fileqty, "phase", &(*pSim).phase);
    neut_sim_fscanf_inputs_file (files, fileqty, "cfg", &(*pSim).cfg);
    if (!(*pSim).cfg)
      neut_sim_fscanf_inputs_file (files, fileqty, "config", &(*pSim).cfg);
  }

  ut_free_1d_char (&dir);
  ut_free_2d_char (&files, fileqty);

  return;
}

// the steps are those of the result files, results/<entity>/<res>/<res>.step<N>
void
neut_sim_fscanf_steps (struct SIM *pSim)
{
  int i, j, k, step, maxstep = -1, entitydirqty = 0, resqty = 0, fileqty = 0;
  int *printed = NULL;
  char *dir = NULL, **entitydirs = NULL, **res = NULL, **files = NULL;

  ut_string_sprintf (&dir, "%s/results", (*pSim).simdir);
  neut_sim_fscanf_subdirs (dir, &entitydirs, &entitydirqty);

  for (i = 0; i < entitydirqty; i++)
  {
    ut_string_sprintf (&dir, "%s/results/%s", (*pSim).simdir, entitydirs[i]);
    ut_free_2d_char (&res, resqty);
    neut_sim_fscanf_subdirs (dir, &res, &resqty);

    for (j = 0; j < resqty; j++)
    {
      ut_string_sprintf (&dir, "%s/results/%s/%s", (*pSim).simdir, entitydirs[i], res[j]);
      ut_free_2d_char (&files, fileqty);
      fileqty = 0;
      ut_dir_allfiles (dir, &files, &fileqty);

      for (k = 0; k < fileqty; k++)
        if (!neut_sim_fscanf_steps_file (res[j], files[k], &step))
        {
          if (step > maxstep)
          {
            printed = ut_realloc_1d_int_zero (printed, step + 1, step - maxstep);
            maxstep = step;
          }

          printed[step] = 1;
        }
    }
  }

  if (maxstep >= 0)
  {
    (*pSim).StepQty = maxstep;
    (*pSim).StepState = printed;
    (*pSim).StepState[0] = 1;
  }
  else
    (*pSim).StepQty = 0;

  ut_free_1d_char (&dir);
  ut_free_2d_char (&entitydirs, entitydirqty);
  ut_free_2d_char (&res, resqty);
  ut_free_2d_char (&files, fileqty);

  return;
}

void
neut_sim_fscanf_results (struct SIM *pSim)
{
  int i, j, pos, stepqty, missingqty, entitydirqty = 0, resqty = 0;
  char *dir = NULL, *entity = NULL, *expr = NULL, *type = NULL, **entitydirs = NULL, **res = NULL;

  ut_string_sprintf (&dir, "%s/results", (*pSim).simdir);
  neut_sim_fscanf_subdirs (dir, &entitydirs, &entitydirqty);

  for (i = 0; i < entitydirqty; i++)
  {
    neut_sim_fscanf_entitydir_entity (entitydirs[i], &entity);

    pos = -1;

    ut_string_sprintf (&dir, "%s/results/%s", (*pSim).simdir, entitydirs[i]);
    ut_free_2d_char (&res, resqty);
    neut_sim_fscanf_subdirs (dir, &res, &resqty);

    // user-defined entity
    if (ut_file_exist ("%s/results/%s/%s.def", (*pSim).simdir, entitydirs[i], entitydirs[i]))
    {
      neut_sim_fscanf_def (pSim, entitydirs[i]);
      pos = (*pSim).EntityQty - 1;
    }

    else if (!neut_sim_entity_internal (entity))
    {
      // not an entity directory (no result directories, e.g. FEPX's forces)
      if (!resqty)
        continue;

      ut_print_message (1, 3, "Directory `results/%s' has no definition file (%s.def).  Ignoring it...\n",
                        entitydirs[i], entitydirs[i]);
      continue;
    }

    for (j = 0; j < resqty; j++)
    {
      missingqty = neut_sim_fscanf_res (*pSim, entitydirs[i], res[j], &stepqty);

      // not a result directory
      if (missingqty == stepqty)
        continue;

      else if (missingqty > 0)
        ut_print_message (1, 3, "Result `%s' of entity `%s' is incomplete (%d step file(s) missing).\n",
                          res[j], entity, missingqty);

      if (pos == -1)
      {
        neut_sim_addentity (pSim, entity);
        pos = (*pSim).EntityQty - 1;
      }

      neut_sim_fscanf_resdef (*pSim, entitydirs[i], res[j], &expr, &type);
      neut_sim_addres (pSim, entity, res[j], expr, type);
    }
  }

  ut_free_1d_char (&dir);
  ut_free_1d_char (&entity);
  ut_free_1d_char (&expr);
  ut_free_1d_char (&type);
  ut_free_2d_char (&entitydirs, entitydirqty);
  ut_free_2d_char (&res, resqty);

  return;
}

// the restart is that of the restart control file, restart/rst<N>.control (FEPX 1 and 2), or
// the restart point, restart/restart.rank<r> (FEPX 3)
void
neut_sim_fscanf_restart (struct SIM *pSim)
{
  int i, id, length, fileqty = 0;
  char *dir = ut_string_paste ((*pSim).simdir, "/restart");
  char **files = NULL;

  if (ut_sys_isdir ("%s", dir))
  {
    ut_dir_allfiles (dir, &files, &fileqty);

    for (i = 0; i < fileqty; i++)
    {
      length = -1;
      if (sscanf (files[i], "rst%d.control%n", &id, &length) == 1
          && length == (int) strlen (files[i]))
      {
        (*pSim).RestartId = id;
        (*pSim).RestartFiles = 1;
      }

      else if (sscanf (files[i], "restart.rank%d%n", &id, &length) == 1
               && length == (int) strlen (files[i]))
        (*pSim).RestartFiles = 1;
    }
  }

  ut_free_1d_char (&dir);
  ut_free_2d_char (&files, fileqty);

  return;
}
