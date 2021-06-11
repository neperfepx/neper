/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_faceproj_.h"

int
nem_meshing_para_faceproj_eq (struct TESS Tess, struct NODES RNodes,
                              struct MESH *RMesh, int face, double *face_eq)
{
  int i;
  int status, iter, res;
  int NodeQty;
  int *nodes = NULL;
  double **nodecoos = NULL;
  double theta;
  double *r = ol_r_alloc ();
  double **g = ol_g_alloc ();
  int itermax = 1000;
  double *n = ut_alloc_1d (3);

  int mesh2d_defined = (RMesh[2].EltQty > 0);

  // Fixing self-intersecting faces ------------------------------------
  srand48 (face);

  ut_array_1d_memcpy (Tess.FaceEq[face], 4, face_eq);

  res = 0;
  status = 0;
  iter = 1;
  do
  {
    if (mesh2d_defined)
      neut_mesh_face_boundnodecoos (RNodes, RMesh[1], Tess, face, &nodecoos,
                                    &NodeQty);
    else
      neut_tess_face_vercoos (Tess, face, &nodecoos, &NodeQty);

    // copying / projecting node coos
    for (i = 0; i < NodeQty; i++)
      ut_space_point_dir_plane_proj (nodecoos[i], face_eq + 1,
                                     Tess.FaceEq[face], nodecoos[i]);

    status = ut_space_contour_selfintersect (nodecoos, NodeQty);

    if (status == 0 && mesh2d_defined)
      for (i = 1; i <= RMesh[2].Elsets[face][0]; i++)
      {
        neut_mesh_elt_normal (RMesh[2], RNodes, RMesh[2].Elsets[face][i], n);
        if (ut_vector_scalprod (n, face_eq + 1) < 0)
        {
          status = 1;
          // elt = RMesh[2].Elsets[face][i];
          break;
        }
      }

    theta = 2 + 88 * pow ((double) iter / (double) itermax, 2);
    if (status == 1)
    {
      ol_nb_r (drand48 (), drand48 (), r);
      ol_rtheta_g (r, theta, g);
      ol_g_vect_vect (g, Tess.FaceEq[face] + 1, face_eq + 1);
    }

    if (status != 0 && iter == 1)
      res = 1;

    /*
       if (status == 0 && iter > 1)
       printf ("face %d ok with angle %f\n", face, theta);
     */
  }
  while (status == 1 && ++iter <= itermax);

  if (iter > itermax)
    res = -1;

  ut_free_1d_int (&nodes);
  ol_r_free (r);
  ol_g_free (g);
  ut_free_1d (&n);
  ut_free_2d (&nodecoos, NodeQty);

  return res;
}
