/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_mesh_3d_.h"

void
nev_print_mesh_3d (FILE * file, struct PRINT Print, struct NODES Nodes,
		   struct MESH *Mesh,
		   struct NODEDATA NodeData, struct MESHDATA *MeshData,
		   int ***pprintelt3dface)
{
  struct NODES N;
  struct MESH M1D, M2D;
  int elt3dfaceqty = neut_elt_boundqty (Mesh[3].EltType, 3);
  int *elt2delt3d = NULL;
  int *nodes_new_old = NULL;
  int elt2dnodeqty;
  int **seq2 = NULL;

  if (Mesh[3].EltQty == 0)
    return;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M1D);
  neut_mesh_set_zero (&M2D);
  neut_elt_boundlist (Mesh[3].EltType, 2, &seq2, &elt2dnodeqty, NULL);

  (*pprintelt3dface) = ut_alloc_2d_int (Mesh[3].EltQty + 1, elt3dfaceqty);

  ut_print_message (0, 3, "Reducing data...\n");

  nev_print_mesh_3d_compress (Print, Nodes, Mesh[3],
			      &N, &M2D, pprintelt3dface, &elt2delt3d,
			      &nodes_new_old);
  nev_print_mesh_2d_compress (M2D, NULL, &M1D);

  ut_print_message (0, 4,
		    "Number of 3D elt faces reduced by %2.0f\%% (to %d).\n",
		    100 - ut_num_percent (M2D.EltQty,
					  Print.showelt3d[0] * elt3dfaceqty),
		    M2D.EltQty);

  ut_print_message (0, 4,
		    "Number of 3D elt edges reduced by %2.0f\%% (to %d).\n",
		    100 - ut_num_percent (M1D.EltQty,
					  M2D.EltQty * elt2dnodeqty),
		    M1D.EltQty);

  nev_print_mesh_3d_print (file, Print, N, M1D, M2D,
			   elt2delt3d, nodes_new_old, NodeData, MeshData);

  ut_free_1d_int (nodes_new_old);

  return;
}
