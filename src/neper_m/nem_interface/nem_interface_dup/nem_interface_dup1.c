/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
                   struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int faceqty, *faces = NULL;
  int edgeqty, *edges = NULL;
  int verqty, *vers = NULL;

  nem_interface_dup_pre (In, Tess, &vers, &verqty, &edges, &edgeqty, &faces,
                         &faceqty);

  nem_interface_dup_ver (Tess, vers, verqty, pNodes, Mesh);
  nem_interface_dup_edge (Tess, edges, edgeqty, pNodes, Mesh);

  if (Tess.Dim == 3)
    nem_interface_dup_face (Tess, faces, faceqty, pNodes, Mesh);

  neut_nodes_init_dupnodeslave (pNodes);

  nem_interface_dup_per (Tess, pNodes, Mesh);

  nem_interface_dup_renumber_1d (Tess, *pNodes, Mesh);

  neut_mesh_array_init_nodeelts (Mesh, (*pNodes).NodeQty);

  if (Tess.Dim == 2)
    nem_interface_dup_boundelts_2d (Tess, *pNodes, Mesh, pBound);
  else if (Tess.Dim == 3)
  {
    nem_interface_dup_boundelts_3d (Tess, faces, faceqty, *pNodes, Mesh,
                                    pBound);
    nem_interface_dup_renumber_2d (Tess, *pNodes, Mesh);
  }
  else
    ut_print_neperbug ();

  ut_free_1d_int (&faces);
  ut_free_1d_int (&edges);
  ut_free_1d_int (&vers);

  return;
}
