/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup (struct TESS Tess, int *intertypes, struct NODES *pNodes,
                   struct MESH *Mesh, struct BOUNDARY *pBound, int ***pnodeseeds)
{
  int faceqty, *faces = NULL;
  int edgeqty, *edges = NULL;
  int verqty, *vers = NULL;
  int interqty, *inters = NULL;

  nem_interface_dup_pre (Tess, intertypes, &vers, &verqty, &edges, &edgeqty, &faces,
                         &faceqty);

  // interfaces: faces in 3D, edges in 2D
  inters = Tess.Dim == 3 ? faces : edges;
  interqty = Tess.Dim == 3 ? faceqty : edgeqty;

  nem_interface_dup_ver (Tess, vers, verqty, inters, interqty, pNodes, Mesh, pnodeseeds);
  nem_interface_dup_edge (Tess, edges, edgeqty, inters, interqty, pNodes, Mesh, pnodeseeds);

  if (Tess.Dim == 3)
    nem_interface_dup_face (Tess, faces, faceqty, inters, interqty, pNodes, Mesh, pnodeseeds);

  neut_nodes_init_dupnodeslave (pNodes);

  if ((*pNodes).PerNodeQty > 0)
    nem_interface_dup_nodeseeds_complete (Tess, Mesh, *pnodeseeds);

  nem_interface_dup_per (Tess, pNodes, *pnodeseeds);

  nem_interface_dup_renumber_1d (Tess, *pNodes, *pnodeseeds, Mesh);

  neut_mesh_array_init_nodeelts (Mesh, (*pNodes).NodeQty);

  if (Tess.Dim == 2)
    nem_interface_dup_boundelts_2d (Tess, edges, edgeqty, *pNodes, *pnodeseeds, Mesh, pBound);
  else if (Tess.Dim == 3)
  {
    nem_interface_dup_boundelts_3d (Tess, faces, faceqty, *pNodes, *pnodeseeds, Mesh,
                                    pBound);
    nem_interface_dup_renumber_2d (Tess, *pNodes, *pnodeseeds, Mesh);
    // (the 2D elt nodes have changed)
    neut_mesh_init_nodeelts (Mesh + 2, (*pNodes).NodeQty);
  }
  else
    ut_print_neperbug ();

  ut_free_1d_int (&faces);
  ut_free_1d_int (&edges);
  ut_free_1d_int (&vers);

  return;
}
