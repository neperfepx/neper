/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_.h"

void
nem_meshing_2D_face_proj (struct TESS Tess, struct NODES RNodes,
			  struct MESH *RMesh, int face,
			  double *face_eq, struct NODES *pN)
{
  struct NODES Nint;
  struct MESH Mint;

  neut_nodes_set_zero (&Nint);
  neut_mesh_set_zero (&Mint);

  if (Tess.FaceState[face] > 0 && RNodes.NodeQty == 0)
  {
    neut_tess_face_interpolmesh (Tess, face, &Nint, &Mint);
    neut_nodes_proj_alongontomesh (pN, face_eq, Nint, Mint, 1);
  }

  if (RNodes.NodeQty > 0)
    neut_nodes_proj_alongontomesh (pN, face_eq, RNodes, RMesh[2], face);

  neut_nodes_free (&Nint);
  neut_mesh_free (&Mint);

  return;
}
