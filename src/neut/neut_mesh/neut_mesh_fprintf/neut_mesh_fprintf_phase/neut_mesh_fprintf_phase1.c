/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_phase_.h"

void
neut_mesh_fprintf_phase (FILE *file, struct MESH Mesh)
{
  if (Mesh.ElsetGroup)
    neut_mesh_fprintf_msh_elsetgroups (file, Mesh);

  return;
}
