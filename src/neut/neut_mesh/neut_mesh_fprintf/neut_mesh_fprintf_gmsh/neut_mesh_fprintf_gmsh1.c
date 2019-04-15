/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_gmsh_.h"

void
neut_mesh_fprintf_gmsh (FILE * file, char *dim, struct NODES Nodes,
			struct MESH Mesh0D, struct MESH Mesh1D,
			struct MESH Mesh2D, struct MESH Mesh3D,
			struct PART Part, struct MESH MeshCo,
			char *numbering, char* mode)
{
  neut_meshheader_fprintf_gmsh (file, mode, Mesh0D, Mesh1D, Mesh2D, Mesh3D);

  neut_nodes_fprintf_gmsh (file, mode, Nodes);

  neut_elts_fprintf_gmsh (file, mode, Mesh0D, Mesh1D, Mesh2D, Mesh3D, Part,
			  MeshCo, dim, numbering);

  neut_physical_fprintf_gmsh (file, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
			      dim);

  return;
}
