/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_ori_.h"

void
neut_mesh_fprintf_ori (FILE *file, struct MESH Mesh)
{
  if (Mesh.ElsetOri || Mesh.EltOri)
    neut_mesh_fprintf_msh_orientations (file, Mesh);

  return;
}
