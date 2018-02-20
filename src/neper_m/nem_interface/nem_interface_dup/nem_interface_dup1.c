/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup (struct TESS Tess, struct NODES *pNodes,
		   struct MESH *Mesh, struct BOUNDARY *pBound)
{
  nem_interface_dup_ver  (Tess, pNodes, Mesh);
  nem_interface_dup_edge (Tess, pNodes, Mesh);
  if (Tess.Dim == 3)
    nem_interface_dup_face (Tess, pNodes, Mesh);

  neut_nodes_init_dupnodeslave (pNodes);

  neut_mesh_array_init_nodeelts (Mesh, Tess.Dim, (*pNodes).NodeQty);

  nem_interface_dup_per (Tess, pNodes, Mesh);

  neut_mesh_array_init_nodeelts (Mesh, Tess.Dim, (*pNodes).NodeQty);

  nem_interface_dup_renumber_1d (Tess, *pNodes, Mesh);

  if (Tess.Dim == 2)
    nem_interface_dup_boundelts_2d (Tess, *pNodes, Mesh, pBound);
  else if (Tess.Dim == 3)
  {
    nem_interface_dup_boundelts_3d (Tess, *pNodes, Mesh, pBound);
    nem_interface_dup_renumber_2d (Tess, *pNodes, Mesh);
  }
  else
    ut_error_reportbug ();

  return;
}
