/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
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

void
nem_mesh_2d_gmsh_writenodes (struct TESS Tess, struct NODES Nodes,
			     struct MESH *Mesh, int face, double *face_proj,
			     FILE * file)
{
  int i, node;
  int nodeqty;
  int *nodes = NULL;
  double *coo = ut_alloc_1d (3);

  // searching set of nodes of the 0-D an 1-D meshes for this face

  neut_mesh_face_boundnodes (Mesh[1], Tess, face, &nodes, &nodeqty);

  // printing face nodes
  for (i = 0; i < nodeqty; i++)
  {
    node = nodes[i];
    ut_array_1d_memcpy (coo, 3, Nodes.NodeCoo[node]);

    // if modified face, projecting the node into its initial plane
    if (Tess.FaceState[face] > 0 && face_proj)
      ut_space_projpoint_alongonto (coo, face_proj, Tess.FaceEq[face]);

    fprintf (file, "Point(%d) = {%.12f, %.12f, %.12f, %.12f};\n",
	     node, coo[0], coo[1], coo[2], Nodes.NodeCl[node]);
  }

  ut_free_1d_int (nodes);
  ut_free_1d (coo);

  return;
}

void
nem_mesh_2d_gmsh_write1dmesh (struct TESS Tess, struct MESH *Mesh, int face,
			      FILE * file)
{
  int i, j, elt, edge;

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];

    /* writing elts */
    for (j = 1; j <= Mesh[1].Elsets[edge][0]; j++)
    {
      elt = Mesh[1].Elsets[edge][j];
      fprintf (file, "Line(%d) = {%d,%d} ;\n", elt,
	       Mesh[1].EltNodes[elt][0], Mesh[1].EltNodes[elt][1]);
    }
  }

  return;
}
