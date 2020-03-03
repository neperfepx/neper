/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_gmsh_.h"

void
nem_mesh_2d_gmsh_writeface_b (double cl, FILE * file)
{
  fprintf (file, "Merge \"tmp-surf.msh\";\n");
  fprintf (file, "Line Loop (1) = {1};\n");
  fprintf (file, "Surface (1) = {1};\n");

  if (cl > 0)
    fprintf (file, "Mesh.CharacteristicLengthMax = %.12f;\n", cl);

  return;
}

void
nem_mesh_3d_gmsh_writepoly (double cl, char *surf, FILE * file)
{
  fprintf (file, "Merge \"%s\";\n", surf);
  fprintf (file, "Surface Loop (1) = {1};\n");
  fprintf (file, "Volume (1) = {1};\n");

  if (cl > 0)
    fprintf (file, "Mesh.CharacteristicLengthMax = %.12f;\n", cl);

  return;
}
