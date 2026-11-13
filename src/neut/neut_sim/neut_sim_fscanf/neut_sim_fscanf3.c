/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fscanf_.h"

void
neut_sim_fscanf_legacy_head (FILE *file)
{
  char *version = ut_alloc_1d_char (100);

  if (!ut_file_string_scanandtest (file, "***sim")
      || !ut_file_string_scanandtest (file, "**format"))
    neut_sim_fscanf_error (file, "expected `***sim' and `**format'.");

  if (fscanf (file, "%99s", version) != 1)
    neut_sim_fscanf_error (file, "expected a format version.");

  if (strcmp (version, "1.0") && strcmp (version, "1.1"))
    neut_sim_fscanf_error (file, "unknown format `%s' (1.0 or 1.1 expected).", version);

  ut_free_1d_char (&version);

  return;
}

void
neut_sim_fscanf_legacy_foot (FILE *file)
{
  if (!ut_file_string_scanandtest (file, "***end"))
    neut_sim_fscanf_error (file, "expected `***end'.");

  return;
}

// skipping a section, up to the next section or the end
void
neut_sim_fscanf_legacy_skip (FILE *file)
{
  int level;

  ut_file_skip (file, 1);

  while (!ut_file_nextstring_sectionlevel (file, &level) && level < 2)
    ut_file_skip (file, 1);

  return;
}

void
neut_sim_fscanf_legacy_entity (struct SIM *pSim, FILE *file)
{
  int i, level, pos, qty;
  char *entity = ut_alloc_1d_char (1000);
  char *string = ut_alloc_1d_char (1000);

  ut_file_skip (file, 1);

  if (fscanf (file, "%s", entity) != 1)
    neut_sim_fscanf_error (file, "expected an entity name after `**entity'.");

  neut_sim_addentity (pSim, entity);
  pos = (*pSim).EntityQty - 1;

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    if (fscanf (file, "%s", string) != 1)
      neut_sim_fscanf_error (file, "expected a field in section `**entity %s'.", entity);

    if (!strcmp (string, "*type"))
    {
      if (fscanf (file, "%s", string) != 1)
        neut_sim_fscanf_error (file, "expected a member type after `*type'.");
      ut_string_string (string, (*pSim).EntityType + pos);
    }

    else if (!strcmp (string, "*member"))
    {
      if (fscanf (file, "%d", &qty) != 1)
        neut_sim_fscanf_error (file, "expected a number of members after `*member'.");

      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%s", string) != 1 || string[0] == '*')
          neut_sim_fscanf_error (file, "expected %d member expression(s) after `*member'.", qty);
        neut_sim_entity_addexpr (pSim, entity, string);
      }
    }

    else if (!strcmp (string, "*result"))
    {
      if (fscanf (file, "%d", &qty) != 1)
        neut_sim_fscanf_error (file, "expected a number of results after `*result'.");

      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%s", string) != 1 || string[0] == '*')
          neut_sim_fscanf_error (file, "expected %d result(s) after `*result'.", qty);
        neut_sim_addres (pSim, entity, string, NULL, NULL);
      }
    }

    else
      neut_sim_fscanf_error (file, "unknown field `%s' in section `**entity %s'.", string, entity);
  }

  if (!strcmp ((*pSim).EntityType[pos], "unknown"))
    ut_string_string ("elset", (*pSim).EntityType + pos);

  ut_free_1d_char (&entity);
  ut_free_1d_char (&string);

  return;
}

// input file of a given extension: the first file named *.<ext> (in
// alphabetical order), simulation.<ext> prevailing (FEPX)
void
neut_sim_fscanf_inputs_file (char **files, int fileqty, char *ext, char **pfile)
{
  int i;
  char *name = NULL;

  ut_string_sprintf (&name, "simulation.%s", ext);

  if (ut_array_1d_string_eltpos (files, fileqty, name) != -1)
    ut_string_string (name, pfile);

  else
    for (i = 0; i < fileqty; i++)
      if (files[i][0] != '.' && strrchr (files[i], '.')
          && !strcmp (strrchr (files[i], '.') + 1, ext))
      {
        ut_string_string (files[i], pfile);
        break;
      }

  ut_free_1d_char (&name);

  return;
}

// sorted list of the (non-hidden) subdirectories of a directory
void
neut_sim_fscanf_subdirs (char *dir, char ***psubdirs, int *psubdirqty)
{
  int i, fileqty = 0;
  char **files = NULL;

  *psubdirqty = 0;
  *psubdirs = NULL;

  if (!ut_sys_isdir ("%s", dir))
    return;

  ut_dir_allfiles (dir, &files, &fileqty);

  for (i = 0; i < fileqty; i++)
    if (files[i][0] != '.' && ut_sys_isdir ("%s/%s", dir, files[i]))
    {
      (*psubdirqty)++;
      (*psubdirs) = ut_realloc_1d_pchar_null (*psubdirs, *psubdirqty, 1);
      ut_string_string (files[i], (*psubdirs) + (*psubdirqty) - 1);
    }

  ut_array_1d_pchar_sort (*psubdirs, *psubdirqty);

  ut_free_2d_char (&files, fileqty);

  return;
}

// returns 0 and the step if file is <res>.step<step>, -1 otherwise
int
neut_sim_fscanf_steps_file (char *res, char *file, int *pstep)
{
  int length = -1, reslength = strlen (res);

  if (strncmp (file, res, reslength))
    return -1;

  if (sscanf (file + reslength, ".step%d%n", pstep, &length) != 1
      || length != (int) strlen (file + reslength) || *pstep < 0)
    return -1;

  return 0;
}

void
neut_sim_fscanf_entitydir_entity (char *entitydir, char **pentity)
{
  if (!strcmp (entitydir, "cells"))
    ut_string_string ("cell", pentity);
  else
    neut_sim_entity_entity (entitydir, pentity);

  return;
}

// reading the definition file of an entity, results/<entity>/<entity>.def
void
neut_sim_fscanf_def (struct SIM *pSim, char *entity)
{
  int pos, level;
  char *filename = NULL, *string = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_string_sprintf (&filename, "%s/results/%s/%s.def", (*pSim).simdir, entity, entity);

  neut_sim_addentity (pSim, entity);
  pos = (*pSim).EntityQty - 1;

  file = ut_file_open (filename, "R");

  while (fscanf (file, "%999s", string) == 1)
  {
    if (!strcmp (string, "*type"))
    {
      if (fscanf (file, "%999s", string) != 1)
        ut_print_message (2, 3, "Failed to read `%s': expected a member type after `*type'.\n",
                          filename);
      ut_string_string (string, (*pSim).EntityType + pos);
    }

    else if (!strcmp (string, "*member"))
      while (!ut_file_nextstring_sectionlevel (file, &level) && level == 0
             && fscanf (file, "%999s", string) == 1)
        neut_sim_entity_addexpr (pSim, (*pSim).Entities[pos], string);

    else
      ut_print_message (2, 3, "Failed to read `%s': unknown field `%s'.\n", filename, string);
  }

  ut_file_close (file, filename, "R");

  ut_free_1d_char (&filename);
  ut_free_1d_char (&string);

  return;
}

// reading the definition file of a result, results/<entity>/<res>/<res>.def
// (*pexpr and *ptype are NULL if there is no such file or section)
void
neut_sim_fscanf_resdef (struct SIM Sim, char *entitydir, char *res, char **pexpr, char **ptype)
{
  int c, pos, length;
  char *filename = NULL, *name = NULL, *line = ut_alloc_1d_char (1), *section = NULL, *value = NULL;
  FILE *file = NULL;

  ut_free_1d_char (pexpr);
  ut_free_1d_char (ptype);

  ut_string_sprintf (&filename, "%s/results/%s/%s/%s.def", Sim.simdir, entitydir, res, res);
  ut_string_sprintf (&name, "results/%s/%s/%s.def", entitydir, res, res);

  if (ut_file_exist (filename))
  {
    file = ut_file_open (filename, "R");

    do
    {
      length = 0;
      line[0] = '\0';
      while ((c = fgetc (file)) != EOF && c != '\n')
      {
        line = ut_realloc_1d_char (line, length + 2);
        line[length++] = c;
        line[length] = '\0';
      }

      while (length > 0 && isspace (line[length - 1]))
        line[--length] = '\0';

      value = line + strspn (line, " \t");

      // new section, whose value can be on the same line
      if (value[0] == '*')
      {
        pos = strcspn (value, " \t");
        ut_string_string (value, &section);
        section[pos] = '\0';

        if (strcmp (section, "*type") && strcmp (section, "*expr"))
        {
          ut_print_message (1, 3, "File `%s': unknown section `%s'.  Ignoring it...\n", name, section);
          section[0] = '\0';
        }

        value += pos;
        value += strspn (value, " \t");
      }

      if (strlen (value))
        neut_sim_fscanf_resdef_value (name, section, value, pexpr, ptype);
    }
    while (c != EOF);

    ut_file_close (file, filename, "R");
  }

  ut_free_1d_char (&filename);
  ut_free_1d_char (&name);
  ut_free_1d_char (&line);
  ut_free_1d_char (&section);

  return;
}

// returns the number of missing step files of a result, out of the
// number of written steps (*pstepqty)
int
neut_sim_fscanf_res (struct SIM Sim, char *entitydir, char *res, int *pstepqty)
{
  int step, missingqty = 0;

  (*pstepqty) = 0;

  for (step = 0; step <= Sim.StepQty; step++)
    if (step == 0 || !Sim.StepState || Sim.StepState[step])
    {
      (*pstepqty)++;

      if (!ut_file_exist ("%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir, res, res, step))
        missingqty++;
    }

  return missingqty;
}
