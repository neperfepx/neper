/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nes_utils_.h"

void
nes_in_set_zero (struct IN_S *pIn)
{
  (*pIn).simdir = NULL;
  (*pIn).mergedir = NULL;
  (*pIn).orispace = NULL;

  (*pIn).entity = NULL;

  (*pIn).entityqty = 0;
  (*pIn).entities = NULL;
  (*pIn).entityresexpr = NULL;


  return;
}

void
nes_in_free (struct IN_S *pIn)
{
  ut_free_1d_char (&(*pIn).simdir);
  ut_free_1d_char (&(*pIn).mergedir);
  ut_free_1d_char (&(*pIn).orispace);

  ut_free_1d_char (&(*pIn).entity);

  ut_free_2d_char (&(*pIn).entities, (*pIn).entityqty);
  ut_free_2d_char (&(*pIn).entityresexpr, (*pIn).entityqty);

  nes_in_set_zero (pIn);

  return;
}

void
nes_loadtess (struct SIM *pSim, struct TESS *pTess, int verbosity)
{
  char *filename = ut_string_paste3 ((*pSim).simdir, "/inputs/", (*pSim).tess);
  FILE *file = NULL;

  if ((*pSim).tess && ut_file_exist (filename))
  {
    if (verbosity)
      ut_print_message (0, verbosity + 1, "%s...\n", (*pSim).tess);

    file = ut_file_open (filename, "R");
    neut_tess_fscanf (file, pTess);
    ut_file_close (file, filename, "R");
  }
  else
    neut_tess_set_zero (pTess);

  (*pTess).pSim = pSim;

  ut_free_1d_char (&filename);

  return;
}

void
nes_loadtesr (struct SIM *pSim, struct TESR *pTesr, int verbosity)
{
  char *filename = ut_string_paste3 ((*pSim).simdir, "/inputs/", (*pSim).tesr);
  char *dirname = ut_string_paste ((*pSim).simdir, "/inputs/");
  FILE *file = NULL;

  if ((*pSim).tesr && ut_file_exist (filename))
  {
    if (verbosity)
      ut_print_message (0, verbosity + 1, "%s...\n", (*pSim).tesr);

    file = ut_file_open (filename, "R");
    neut_tesr_fscanf (file, dirname, pTesr);
    ut_file_close (file, filename, "R");
  }
  else
    neut_tesr_set_zero (pTesr);

  (*pTesr).pSim = pSim;

  ut_free_1d_char (&filename);
  ut_free_1d_char (&dirname);

  return;
}

void
nes_loadmesh (struct SIM *pSim, struct NODES *pNodes, struct MESH *Mesh,
              int verbosity)
{
  int i;
  char *filename = ut_string_paste3 ((*pSim).simdir, "/inputs/", (*pSim).msh);
  FILE *file = NULL;

  if ((*pSim).msh && ut_file_exist (filename))
  {
    if (verbosity)
      ut_print_message (0, verbosity + 1, "%s...\n", (*pSim).msh);

    file = ut_file_open (filename, "R");
    neut_mesh_fscanf_msh (file, pNodes, Mesh, Mesh +1, Mesh + 2, Mesh + 3, NULL, NULL, NULL, NULL);
    ut_file_close (file, filename, "R");
  }

  for (i = 0; i <= 4; i++)
    Mesh[i].pSim = pSim;

  ut_free_1d_char (&filename);

  return;
}

void
nes_loadconfig (struct SIM Sim, char ***pGroupCrySym, int *pGroupQty, int verbosity)
{
  int i, id = 0;
  char *filename = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.cfg);
  char *string = ut_alloc_1d_char (1000);
  FILE *fp = NULL;

  if (Sim.cfg && ut_file_exist (filename))
  {
    if (verbosity)
      ut_print_message (0, verbosity + 1, "%s...\n", Sim.cfg);

    fp = ut_file_open (filename, "R");

    // keywords and values are case-insensitive, as in FEPX, and crystal_type
    // applies to the current phase
    while (fscanf (fp, "%999s", string) == 1)
    {
      // comment: skipping the rest of the line
      if (string[0] == '#')
        ut_file_skip_line (fp, 1);

      else if (!strcasecmp (string, "number_of_phases"))
      {
        if (fscanf (fp, "%d", pGroupQty) != 1 || *pGroupQty < 1)
          ut_print_message (2, 3, "%s: failed to read `number_of_phases'.\n", Sim.cfg);
        (*pGroupCrySym) = ut_alloc_1d_pchar (*pGroupQty);
      }

      else if (!strcasecmp (string, "phase"))
      {
        if (fscanf (fp, "%d", &id) != 1)
          ut_print_message (2, 3, "%s: failed to read `phase' id.\n", Sim.cfg);
        if (id < 1 || id > *pGroupQty)
          ut_print_message (2, 3, "%s: `phase' id %d is not in [1, %d] (`number_of_phases').\n",
                            Sim.cfg, id, *pGroupQty);
      }

      else if (!strcasecmp (string, "crystal_type"))
      {
        if (id == 0)
          ut_print_message (2, 3, "%s: `crystal_type' is not preceded by `phase'.\n", Sim.cfg);
        if (fscanf (fp, "%999s", string) != 1)
          ut_print_message (2, 3, "%s: failed to read `crystal_type'.\n", Sim.cfg);

        if (!strcasecmp (string, "fcc") || !strcasecmp (string, "bcc"))
          ut_string_string ("cubic", (*pGroupCrySym) + id - 1);
        else if (!strcasecmp (string, "hcp"))
          ut_string_string ("hexagonal", (*pGroupCrySym) + id - 1);
        else if (!strcasecmp (string, "bct"))
          ut_string_string ("4/mmm", (*pGroupCrySym) + id - 1);
        else
          ut_print_message (2, 3, "%s: unknown `crystal_type' `%s'.\n", Sim.cfg, string);
      }
    }

    ut_file_close (fp, filename, "R");

    for (i = 0; i < *pGroupQty; i++)
      if (!(*pGroupCrySym)[i])
        ut_print_message (2, 3, "%s: missing `crystal_type' for phase %d.\n", Sim.cfg, i + 1);
  }

  else
    *pGroupQty = 0;

  ut_free_1d_char (&string);
  ut_free_1d_char (&filename);

  return;
}

void
nes_in_addres (char *opt, char *resexpr, struct IN_S *pIn)
{
  int i, pos;
  char *entity = ut_alloc_1d_char (strlen (opt) - 3);

  if (strlen (opt + 4) == 0)
    ut_print_message (2, 3, "Option `-res' needs an entity.\n");

  entity = strncpy (entity, opt + 4, strlen (opt) - 4);
  neut_sim_entity_entity (entity, &entity);

  pos = -1;
  for (i = 0; i < (*pIn).entityqty; i++)
  {
    if (!strcmp (entity, (*pIn).entities[i]))
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    pos = (*pIn).entityqty++;
    (*pIn).entities = ut_realloc_2d_char_addline ((*pIn).entities, (*pIn).entityqty, 0);
    ut_string_string (entity, (*pIn).entities + pos);
    (*pIn).entityresexpr = ut_realloc_2d_char_addline ((*pIn).entityresexpr, (*pIn).entityqty, 0);
  }

  ut_string_string (resexpr, (*pIn).entityresexpr + pos);

  ut_free_1d_char (&entity);

  return;
}
