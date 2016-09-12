/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_.h"

void
nem_meshing_2D (struct IN_M In, struct MESHPARA MeshPara,
		struct TESS Tess, struct NODES RNodes, struct MESH *RMesh,
		struct NODES *pNodes, struct MESH *Mesh)
{
  int i, id, faceqty, *face = NULL;
  double allowed_t, max_elapsed_t = 0;
  char *message = ut_alloc_1d_char (128);
  struct MULTIM Multim;
  struct timeval ctrlc_t = { 0, 0 };

  ut_print_message (0, 2, "2D meshing... ");

  neut_multim_set_zero (&Multim);
  neut_multim_init (2, In.mesh2dalgo, Tess.FaceQty, &Multim);

  Mesh[2] = neut_mesh_alloc (2, "tri", 1, 0, 0);

  // faces to mesh; default is 'all'
  neut_tess_expr_facelist (Tess, In.meshface, &face, &faceqty);

  nem_meshing_2D_progress (Multim, 0, Tess.FaceQty, message);

  allowed_t = In.mesh2dmaxtime;
  for (i = 0; i < faceqty; i++)
    if (Tess.FaceVerQty[face[i]] > 0)
    {
      id = face[i];

      while (Mesh[2].ElsetQty != id - 1)
	neut_mesh_addelset (Mesh + 2, NULL, 0);

      if (Tess.FaceVerQty[id] > 0)
	nem_meshing_2D_face (In, MeshPara, &Multim,
			     &ctrlc_t, &allowed_t, &max_elapsed_t, Tess,
			     RNodes, RMesh, pNodes, Mesh, id);
      else
	neut_mesh_addelset (Mesh + 2, NULL, 0);

      nem_meshing_2D_progress (Multim, i + 1, faceqty, message);
    }

  neut_mesh_init_nodeelts (Mesh + 2, (*pNodes).NodeQty);
  neut_mesh_init_eltelset (Mesh + 2, NULL);

  neut_multim_free (&Multim, Tess.FaceQty);

#ifdef DEVEL_DEBUGGING_TEST
  sprintf (message, "tmp%d.geo", getpid ());
  remove (message);
  sprintf (message, "tmp%d.msh", getpid ());
  remove (message);
#endif

  ut_free_1d_char (message);
  ut_free_1d_int (face);

  return;
}
