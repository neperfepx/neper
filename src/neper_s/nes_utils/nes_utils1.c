/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_utils_.h"

void
nes_in_set_zero (struct IN_S *pIn)
{
  (*pIn).fepxdir = NULL;
  (*pIn).simdir = NULL;
  (*pIn).noderes = NULL;
  (*pIn).eltres = NULL;
  (*pIn).elsetres = NULL;

  return;
}

void
nes_in_free (struct IN_S In)
{
  ut_free_1d_char (&In.fepxdir);
  ut_free_1d_char (&In.simdir);
  ut_free_1d_char (&In.noderes);
  ut_free_1d_char (&In.eltres);
  ut_free_1d_char (&In.elsetres);

  return;
}

void
nes_loadmesh (struct SIM Sim, struct NODES *pNodes, struct MESH *pMesh)
{
  FILE *file = NULL;
  char *filename = ut_alloc_1d_char (1000);

  neut_nodes_set_zero (pNodes);

  sprintf (filename, "%s/inputs/%s", Sim.simdir, Sim.msh);

  file = ut_file_open (filename, "R");
  neut_mesh_fscanf_msh (file, pNodes, NULL, NULL, NULL, pMesh, NULL, NULL);
  ut_file_close (file, filename, "R");

  ut_free_1d_char (&filename);

  return;
}
