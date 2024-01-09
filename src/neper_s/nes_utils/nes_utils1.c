/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_utils_.h"

void
nes_in_set_zero (struct IN_S *pIn)
{
  (*pIn).fepxdir = NULL;
  (*pIn).simdir = NULL;
  (*pIn).mergedir = NULL;
  (*pIn).orispace = NULL;

  (*pIn).entity = NULL;

  (*pIn).entityqty = 0;
  (*pIn).entities = NULL;
  (*pIn).entityresexpr = NULL;

  (*pIn).stepqty = -1; // uninitialized

  return;
}

void
nes_in_free (struct IN_S *pIn)
{
  ut_free_1d_char (&(*pIn).fepxdir);
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
    neut_tesr_fscanf (file, dirname, NULL, NULL, pTesr);
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
    neut_mesh_fscanf_msh (file, pNodes, Mesh, Mesh +1, Mesh + 2, Mesh + 3, NULL, NULL);
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
  int id;
  char *filename = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.cfg);
  char *string = ut_alloc_1d_char (1000);
  FILE *fp = NULL;

  if (Sim.cfg && ut_file_exist (filename))
  {
    if (verbosity)
      ut_print_message (0, verbosity + 1, "%s...\n", Sim.cfg);

    fp = ut_file_open (filename, "R");

    while (fscanf (fp, "%s", string) == 1)
    {
      if (!strcmp (string, "number_of_phases"))
      {
        if (fscanf (fp, "%d", pGroupQty) != 1)
          abort ();
        (*pGroupCrySym) = ut_alloc_1d_pchar (*pGroupQty);
      }

      else if (!strcmp (string, "phase"))
      {
        if (fscanf (fp, "%d", &id) != 1)
          abort ();

        while (fscanf (fp, "%s", string) == 1)
        {
          if (!strcmp (string, "crystal_type"))
          {
            if (fscanf (fp, "%s", string) != 1)
              abort ();
            if (!strcmp (string, "FCC") || !strcmp (string, "BCC"))
              ut_string_string ("cubic", (*pGroupCrySym) + id - 1);
            else if (!strcmp (string, "HCP"))
              ut_string_string ("hexagonal", (*pGroupCrySym) + id - 1);
            else
              abort ();

            break;
          }
        }
      }
    }

    ut_file_close (fp, filename, "R");
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

  entity = strncpy (entity, opt + 4, strlen (opt) - 4);
  neut_sim_entity_entity (entity, &entity);

  pos = -1;
  for (i = 0; i < (*pIn).entityqty; i++)
    if (!strcmp (entity, (*pIn).entities[i]))
    {
      pos = i;
      break;
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
