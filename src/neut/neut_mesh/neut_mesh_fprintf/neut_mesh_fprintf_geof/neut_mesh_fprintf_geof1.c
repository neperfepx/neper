/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_geof_.h"

void
neut_mesh_fprintf_geof (FILE * file, struct NODES Nodes, struct MESH
			Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
			struct MESH Mesh3D, struct NSET NSet0D,
			struct NSET NSet1D, struct NSET NSet2D,
			char *nsets, char *fasets, char *dim,
			struct PART Part, struct BOUNDARY Bound)
{
  Mesh0D.EltQty = Mesh0D.EltQty;
  Mesh1D.EltQty = Mesh1D.EltQty;
  int *shift = ut_alloc_1d_int (4);

  if (strlen (dim) > 1)
    ut_print_message (1, 3,
		      "Multiple-dimension meshes not well supported by Zmaster.\n");

  neut_mesh_fprintf_geof_head (file);

  neut_mesh_fprintf_geof_nodes (file, Nodes);

  neut_mesh_fprintf_geof_elts (file, Mesh1D, Mesh2D, Mesh3D, dim, shift);

  neut_mesh_fprintf_geof_sethead (file);

  neut_mesh_fprintf_geof_nsets (file, NSet0D, NSet1D, NSet2D, nsets);

  if (fasets != NULL && strcmp (fasets, "none"))
  {
    if (Mesh3D.EltQty == 0)
      neut_mesh_fprintf_geof_lisets (file, Mesh2D, Bound, fasets);
    else
      neut_mesh_fprintf_geof_fasets (file, Mesh3D, Bound, fasets);
  }

  neut_mesh_fprintf_geof_elsets (file, Mesh1D, Mesh2D, Mesh3D, dim, shift);

  if (Part.qty > 0)
    neut_mesh_fprintf_geof_part (file, Part);

  neut_mesh_fprintf_geof_foot (file);
  ut_free_1d_int (shift);

  return;
}
