/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup (struct TESS Tess, struct NODES *pNodes,
		   struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int i;

  nem_interface_dup_ver  (Tess, pNodes, Mesh);
  nem_interface_dup_edge (Tess, pNodes, Mesh);
  if (Tess.Dim == 3)
    nem_interface_dup_face (Tess, pNodes, Mesh);

  neut_nodes_init_dupnodeslave (pNodes);

  for (i = 0; i <= 3; i++)
    neut_mesh_init_nodeelts (Mesh + i, (*pNodes).NodeQty);

  if (Tess.Dim == 2)
    nem_interface_dup_boundelts_2d (Tess, *pNodes, Mesh, pBound);
  else if (Tess.Dim == 3)
    nem_interface_dup_boundelts_3d (Tess, *pNodes, Mesh, pBound);
  else
    ut_error_reportbug ();

  if (Tess.Dim == 3)
    nem_interface_dup_renumber_2d (Tess, *pNodes, Mesh);

  nem_interface_dup_renumber_1d (Tess, *pNodes, Mesh);

  return;
}
