/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_faceproj_.h"

void
nem_meshing_para_faceproj (struct TESS Tess,
			   struct NODES RNodes, struct MESH *RMesh,
			   struct MESHPARA *pMeshPara)
{
  int i, j, status;
  int edge_cpy;
  int face_cpy;
  char *message = ut_alloc_1d_char (8);

  if ((*pMeshPara).dim >= 2 && Tess.Dim == 3)
  {
    (*pMeshPara).face_eq = ut_alloc_2d (Tess.FaceQty + 1, 4);
    (*pMeshPara).face_op = ut_alloc_1d_int (Tess.FaceQty + 1);
    (*pMeshPara).edge_op = ut_alloc_1d_int (Tess.EdgeQty + 1);

    /* face projection vector ***************** */

    ut_print_progress (stdout, 0, Tess.FaceQty, "%3.0f%%", message);
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      status = nem_mesh2d_face_nproj (Tess, RNodes, RMesh,
				      i, (*pMeshPara).face_eq[i]);

      if (status != 0)
	(*pMeshPara).face_op[i] = 1;

      ut_print_progress (stdout, i, Tess.FaceQty, "%3.0f%%", message);
      fflush (stdout);
    }

    for (i = 1; i <= Tess.FaceQty; i++)
      if ((*pMeshPara).face_op[i] == 1)
	for (j = 1; j <= Tess.FaceVerQty[i]; j++)
	  (*pMeshPara).edge_op[Tess.FaceEdgeNb[i][j]] = 1;

    edge_cpy = ut_array_1d_int_sum ((*pMeshPara).edge_op + 1, Tess.EdgeQty);
    face_cpy = ut_array_1d_int_sum ((*pMeshPara).face_op + 1, Tess.FaceQty);

    if (edge_cpy != 0 || face_cpy != 0)
    {
      ut_print_message (0, 3,
			"%d face%s (%d edge%s) cannot be meshed (will be copied).\n",
			face_cpy, (face_cpy > 1) ? "s" : "", edge_cpy,
			(edge_cpy > 1) ? "s" : "");
    }

    ut_free_1d_char (message);
  }
  else
    printf ("\n");

  return;
}
