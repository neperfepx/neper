/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_.h"

void
nem_meshing_2d (struct IN_M In, struct MESHPARA MeshPara, struct TESS *pTess,
                struct NODES RNodes, struct MESH *RMesh, struct NODES *pNodes,
                struct MESH *Mesh)
{
  int i, faceqty, face, *faces = NULL, qty;
  double allowed_t, max_elapsed_t = 0;
  char *message = ut_alloc_1d_char (128);
  struct MULTIM Multim;
  struct timeval ctrlc_t = { 0, 0 };
  struct NODES *N =
    (struct NODES *) calloc ((*pTess).FaceQty + 1, sizeof (struct NODES));
  struct MESH *M =
    (struct MESH *) calloc ((*pTess).FaceQty + 1, sizeof (struct MESH));
  int **bnodes = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  int **lbnodes = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  int *bnodeqty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  int **master_id = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  int **N_global_id = ut_alloc_1d_pint ((*pTess).FaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    neut_nodes_set_zero (N + i);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    neut_mesh_set_zero (M + i);

  ut_print_message (0, 2, "2D meshing... ");

  neut_multim_set_zero (&Multim);
  neut_multim_init (2, In.mesh2dalgo, (*pTess).FaceQty, &Multim);

  Mesh[2] = neut_mesh_alloc (2, "tri", 1, 0, 0);

  // faces to mesh; default is 'all'
  neut_tess_expr_faces (pTess, In.meshface, &faces, &faceqty);

  nem_meshing_2d_progress (Multim, 0, faceqty, message);

  allowed_t = In.mesh2dmaxtime;
  qty = 0;
#pragma omp parallel for schedule(dynamic) private(i,face)
  for (i = 0; i < faceqty; i++)
    if ((*pTess).FaceVerQty[faces[i]] > 0)
    {
      face = faces[i];

      if ((*pTess).Dim == 2 || strcmp ((*pTess).Type, "periodic")
          || !(*pTess).PerFaceMaster[face])
        nem_meshing_2d_face (In, MeshPara, &Multim, &ctrlc_t, &allowed_t,
                             &max_elapsed_t, *pTess, RNodes, RMesh, *pNodes,
                             Mesh, N + face, M + face, bnodes + face,
                             lbnodes + face, bnodeqty + face, face);
#pragma omp critical
      nem_meshing_2d_progress (Multim, ++qty, faceqty, message);
    }

  // slave parallel faces, if any
#pragma omp parallel for schedule(dynamic) private(i,face)
  for (i = 0; i < faceqty; i++)
    if ((*pTess).FaceVerQty[faces[i]] > 0)
    {
      face = faces[i];

      if ((*pTess).Dim != 2 && !strcmp ((*pTess).Type, "periodic")
          && (*pTess).PerFaceMaster[face])
        nem_meshing_2d_face_per ((*pTess), *pNodes, N, M, N + face, M + face,
                                 master_id + face, &bnodes, &lbnodes,
                                 &bnodeqty, face);
    }

  // Recording face meshes in global mesh
  // do not multithread (periodicity)
  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    while (Mesh[2].ElsetQty != face - 1)
      neut_mesh_addelset (Mesh + 2, NULL, 0);

    if ((*pTess).FaceVerQty[face] > 0)
      nem_meshing_2d_face_record ((*pTess), face, N[face], M[face], bnodes[face],
                                  lbnodes[face], bnodeqty[face],
                                  master_id[face], pNodes, N_global_id, Mesh);
    else
      neut_mesh_addelset (Mesh + 2, NULL, 0);
  }

  neut_mesh_init_nodeelts (Mesh + 2, (*pNodes).NodeQty);
  neut_mesh_init_eltelset (Mesh + 2, NULL);
  neut_mesh_init_elsetlabels (Mesh + 2);
  if (!strcmp ((*pTess).Type, "periodic"))
    neut_nodes_init_nodeslave (pNodes);

  neut_multim_free (&Multim, (*pTess).FaceQty);

#ifdef DEVEL_DEBUGGING_TEST
  sprintf (message, "tmp%d.geo", getpid ());
  remove (message);
  sprintf (message, "tmp%d.msh", getpid ());
  remove (message);
#endif

  ut_free_1d_char (&message);
  ut_free_1d_int (&faces);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    neut_nodes_free (N + i);
  free (N);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    neut_mesh_free (M + i);
  free (M);

  ut_free_1d_int (&bnodeqty);
  ut_free_2d_int (&bnodes, (*pTess).FaceQty + 1);
  ut_free_2d_int (&lbnodes, (*pTess).FaceQty + 1);
  ut_free_2d_int (&N_global_id, (*pTess).FaceQty + 1);
  ut_free_2d_int (&master_id, (*pTess).FaceQty + 1);

  return;
}
