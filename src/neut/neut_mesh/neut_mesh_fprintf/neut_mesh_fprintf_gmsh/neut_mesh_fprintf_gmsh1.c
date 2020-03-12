/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_gmsh_.h"

void
neut_mesh_fprintf_gmsh (FILE * file, char *dim, struct TESS Tess,
                        struct NODES Nodes, struct MESH Mesh0D,
                        struct MESH Mesh1D, struct MESH Mesh2D,
                        struct MESH Mesh3D, struct PART Part,
                        struct MESH MeshCo, char *fasetlist, char *numbering,
                        char *mode)
{
  int fasetqty, *fasetids = NULL;
  char **fasets = NULL;

  neut_mesh_fprintf_gmsh_pre (Tess, fasetlist, &fasets, &fasetids, &fasetqty);

  neut_meshheader_fprintf_gmsh (file, mode, Mesh0D, Mesh1D, Mesh2D, Mesh3D);

  neut_nodes_fprintf_gmsh (file, mode, Nodes);

  neut_elts_fprintf_gmsh (file, mode, Tess, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
                          Part, MeshCo, fasets, fasetids, fasetqty, dim,
                          numbering);

  neut_physical_fprintf_gmsh (file, Mesh0D, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                              fasets, fasetids, fasetqty, dim);

  ut_free_1d_int (&fasetids);
  ut_free_2d_char (&fasets, fasetqty);

  return;
}
