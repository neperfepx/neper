/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_mesh_2d_.h"

void
nev_print_mesh_2d (FILE * file, struct PRINT Print, struct NODES Nodes,
		   struct MESH *Mesh, struct NODEDATA NodeData, struct
		   MESHDATA *MeshData)
{
  struct MESH M1D;

  if (Mesh[2].EltQty == 0)
    return;

  neut_mesh_set_zero (&M1D);

  nev_print_mesh_2d_compress (Mesh[2], Print.showelt2d, &M1D);

  nev_print_mesh_2d_print (file, Print, Nodes, M1D, Mesh[2],
			   NodeData, MeshData);

  neut_mesh_free (&M1D);

  return;
}
