/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_gmsh_.h"

/* MeshingOptions writes the meshing options into file. */
void
nem_mesh_gmsh_options (FILE * file, int algo2d, int algo3d, int opti,
                       double rnd)
{
  fprintf (file, "/* Options for the meshing: */\n");
  fprintf (file, "General.Verbosity   = 0;\n");
  fprintf (file, "Mesh.MshFileVersion = 2;\n");
  fprintf (file, "Mesh.Algorithm      = %d;\n", algo2d);
  fprintf (file, "Mesh.Algorithm3D    = %d;\n", algo3d);
  fprintf (file, "Mesh.Binary         = 1;\n");
  fprintf (file, "Mesh.SaveAll = 1;\n");
  if (opti == 1)
    fprintf (file, "Mesh.Optimize       = 1;\n");
  else if (opti == 2)
    fprintf (file, "Mesh.OptimizeNetgen = 1;\n");
  else if (opti == 3)
  {
    fprintf (file, "Mesh.Optimize       = 1;\n");
    fprintf (file, "Mesh.OptimizeNetgen = 1;\n");
  }

  if (rnd > 0)
    fprintf (file, "Mesh.RandomFactor   = %g;\n", rnd);
  fprintf (file, "\n");

  return;
}

int
nem_mesh_2d_gmsh_writeboundary (struct TESS Tess, struct NODES Nodes,
                                struct MESH *Mesh, int face, double *faceproj,
                                int **pbnodes, double ***pbnodecoos,
                                int *pbnodeqty, FILE * file)
{
  int i, node;

  // searching set of nodes of the 0-D an 1-D meshes for this face

  neut_mesh_face_boundnodes (Mesh[1], Tess, face, pbnodes, pbnodeqty);
  (*pbnodecoos) = ut_alloc_2d (*pbnodeqty, 3);

  // printing face nodes
  for (i = 0; i < *pbnodeqty; i++)
  {
    node = (*pbnodes)[i];
    ut_array_1d_memcpy (Nodes.NodeCoo[node], 3, (*pbnodecoos)[i]);

    // if modified face, projecting the node into its initial plane
    if (Tess.FaceState[face] > 0 && faceproj)
      ut_space_point_dir_plane_proj ((*pbnodecoos)[i], faceproj + 1, faceproj,
                                     (*pbnodecoos)[i]);

    fprintf (file, "Point(%d) = {" REAL_PRINT_FORMAT ", " REAL_PRINT_FORMAT ", " REAL_PRINT_FORMAT ", " REAL_PRINT_FORMAT "};\n", i + 1,
             (*pbnodecoos)[i][0], (*pbnodecoos)[i][1], (*pbnodecoos)[i][2],
             Nodes.NodeCl[node]);
  }

  for (i = 1; i <= *pbnodeqty; i++)
    fprintf (file, "Line(%d) = {%d,%d} ;\n", i, i,
             ut_array_rotpos (1, *pbnodeqty, i, 1));

  fprintf (file, "Line Loop(1) = {");

  for (i = 0; i < *pbnodeqty; i++)
    fprintf (file, "%d%s", i + 1, i < *pbnodeqty - 1 ? "," : "};\n");

  fprintf (file, "Plane Surface(1) = {1}; ");
  fprintf (file, "Physical Surface(1) = {1};\n");

  // fprintf (file, "Mesh.CharacteristicLengthMax = %f;\n", cl);
  // above does not work on all architecture.

  return *pbnodeqty;
}
