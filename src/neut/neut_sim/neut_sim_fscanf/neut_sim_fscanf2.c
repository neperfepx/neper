/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fscanf_.h"

void
neut_sim_fscanf_version (char *dir, char **pversion)
{
  char *tmp1 = ut_string_paste (dir, "/report");
  char *tmp2 = ut_string_paste (dir, "/.sim");

  if (ut_file_exist (tmp1))
    ut_string_string ("report", pversion);

  else if (ut_file_exist (tmp2))
    ut_string_string ("sim", pversion);

  else
    abort ();

  ut_free_1d_char (&tmp1);
  ut_free_1d_char (&tmp2);

  return;
}

/* Simulation exportation: head */
void
neut_sim_fscanf_head (FILE *file, char **pversion)
{
  if (!ut_file_string_scanandtest (file, "***sim")
      || !ut_file_string_scanandtest (file, "**format"))
    ut_print_message (2, 0, "Input directory is not a valid simulation directory.\n");

  (*pversion) = ut_alloc_1d_char (100);
  if (fscanf (file, "%s", *pversion) != 1)
    ut_print_message (2, 0, "Input directory is not a valid simulation directory.\n");

  if (strcmp (*pversion, "1.0"))
    ut_print_message (2, 0, "Input directory is not a valid simulation directory.\n");

  return;
}

/* Simulation exportation: foot */
void
neut_sim_fscanf_foot (FILE *file)
{
  char *string = ut_alloc_1d_char (1000);
  ut_file_nextstring (file, string);

  if (!ut_file_string_scanandtest (file, "***end"))
    abort ();

  return;
}

/* Simulation exportation: vertex */
void
neut_sim_fscanf_input (struct SIM *pSim, FILE *file)
{
  int level;
  char *string = ut_alloc_1d_char (1000);

  if (!ut_file_string_scanandtest (file, "**input"))
    abort ();

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "*tess"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).tess);
    }

    else if (!strcmp (string, "*tesr"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).tesr);
    }

    else if (!strcmp (string, "*msh"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).msh);
    }

    else if (!strcmp (string, "*bcs"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).bcs);
    }

    else if (!strcmp (string, "*ori"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).ori);
    }

    else if (!strcmp (string, "*phase"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).phase);
    }

    else if (!strcmp (string, "*config"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).config);
    }

    else
      ut_print_message (2, 2, "Could not read field `%s'.\n", string);
  }

  ut_free_1d_char (&string);

  return;
}

void
neut_sim_fscanf_general (struct SIM *pSim, FILE *file)
{
  int level;
  char *string = ut_alloc_1d_char (1000);

  if (!ut_file_string_scanandtest (file, "**general"))
    abort ();

  if (fscanf (file, "%d%d%d%d%d\n",
              &(*pSim).CellQty,
              &(*pSim).NodeQty,
              &(*pSim).EltQty,
              &(*pSim).ElsetQty,
              &(*pSim).PartQty) != 5)
    abort ();

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "*orides"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, &(*pSim).OriDes);
    }

    else
      ut_print_message (2, 2, "Could not read field `%s'.\n", string);
  }

  ut_free_1d_char (&string);

  return;
}

void
neut_sim_fscanf_entity (struct SIM *pSim, FILE *file)
{
  int i, level, pos, qty;
  char *entity = ut_alloc_1d_char (1000);
  char *string = ut_alloc_1d_char (1000);

  ut_file_skip (file, 1);

  if (fscanf (file, "%s", entity) != 1)
    abort ();

  neut_sim_addentity (pSim, entity);
  pos = (*pSim).EntityQty - 1;

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    int type = 0, member = 0;

    if (fscanf (file, "%s", string) != 1)
      abort ();

    if (!strcmp (string, "*type"))
    {
      type = 1;
      if (fscanf (file, "%s", string) != 1)
        abort ();
      ut_string_string (string, (*pSim).EntityType + pos);
    }

    else if (!strcmp (string, "*member"))
    {
      member = 1;
      if (fscanf (file, "%d", (*pSim).EntityMemberQty + pos) != 1)
        abort ();
      (*pSim).EntityMemberExpr[pos] = ut_alloc_1d_pchar ((*pSim).EntityMemberQty[pos]);

      for (i = 0; i < (*pSim).EntityMemberQty[pos]; i++)
      {
        if (fscanf (file, "%s", string) != 1)
          abort ();
        ut_string_string (string, (*pSim).EntityMemberExpr[pos] + i);
      }
    }

    else if (!strcmp (string, "*result"))
    {
      if (fscanf (file, "%d", &qty) != 1)
        abort ();

      for (i = 0; i < qty; i++)
      {
        if (fscanf (file, "%s", string) != 1)
          abort ();
        neut_sim_addres (pSim, entity, string, NULL);
      }
    }

    else
      ut_print_message (2, 2, "Could not read field `%s'.\n", string);

    if (!type)
      neut_sim_entity_init_type (pSim, entity);
    if (!member)
      neut_sim_entity_init_memberqty (pSim, entity);
  }

  ut_free_1d_char (&entity);
  ut_free_1d_char (&string);

  return;
}

void
neut_sim_fscanf_orispace (struct SIM *pSim, FILE *file)
{
  char *tmp = ut_alloc_1d_char (1000);

  ut_file_skip (file, 1);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  ut_string_string (tmp, &(*pSim).OriSpace);

  ut_free_1d_char (&tmp);

  return;
}

void
neut_sim_fscanf_step (struct SIM *pSim, FILE *file)
{
  int i, id, level, printedqty;
  char *string = ut_alloc_1d_char (1000);

  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &(*pSim).StepQty) != 1)
      abort ();

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "*printed"))
    {
      (*pSim).StepState = ut_alloc_1d_int ((*pSim).StepQty + 1);

      ut_file_skip (file, 1);
      if (fscanf (file, "%d", &printedqty) != 1)
        abort ();
      for (i = 1; i <= printedqty; i++)
      {
        if (fscanf (file, "%d", &id) != 1)
          abort ();
        (*pSim).StepState[id] = 1;
      }
    }

    else
      ut_print_message (2, 2, "Could not read field `%s'.\n", string);
  }

  if (!(*pSim).StepState)
  {
    (*pSim).StepState = ut_alloc_1d_int ((*pSim).StepQty + 1);
    ut_array_1d_int_set ((*pSim).StepState, (*pSim).StepQty + 1, 1);
  }

  ut_free_1d_char (&string);

  return;
}
