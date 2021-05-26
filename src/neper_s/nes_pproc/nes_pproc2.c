/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

void
nes_pproc_loadmesh (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes, struct MESH *Mesh)
{
  FILE *file = NULL;
  char *filename = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.msh);

  filename = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.msh);

  file = ut_file_open (filename, "R");
  neut_mesh_fscanf_msh (file, pNodes, Mesh, Mesh +1, Mesh + 2, Mesh + 3, NULL, NULL);
  ut_file_close (file, filename, "R");

  ut_free_1d_char (&filename);

  filename = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.tess);

  file = ut_file_open (filename, "R");
  neut_tess_fscanf (file, pTess);
  ut_file_close (file, filename, "R");

  ut_free_1d_char (&filename);

  return;
}
