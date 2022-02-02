/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_poly_.h"

int
neut_tess_test_polyReciprocityFace (struct TESS Tess, int i, int verbosity)
{
  int j;
  int face;

  if (Tess.PolyFaceQty[i] < 4)
  {
    if (verbosity)
      ut_print_message (2, 3, "number of faces = %d < 4.\n",
                        Tess.PolyFaceQty[i]);

    return 2;
  }

  for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
  {
    face = Tess.PolyFaceNb[i][j];

    if (ut_array_1d_int_eltpos (Tess.FacePoly[face], 2, i) == -1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "based on face %d, but poly is not in face poly list.\n",
                          face);

      return 3;
    }
  }

  return 0;
}

/* this routines tests if several faces belongs to the same domain
 * boundary, which means that there are coplanar. */
int
neut_tess_test_polyCoplaneityFace (struct TESS Tess, int i, int verbosity)
{
  int j, face, res = 0;
  int *tmp = ut_alloc_1d_int (Tess.PolyFaceQty[i] + 1);

  tmp[0] = Tess.PolyFaceQty[i];
  for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
  {
    face = Tess.PolyFaceNb[i][j];
    tmp[j] = (Tess.FaceDom[face][0] == 2) ? Tess.FaceDom[face][1] : -1;
  }

  for (j = 1; j <= Tess.DomFaceQty; j++)
    if (ut_array_1d_int_valnb (tmp + 1, tmp[0], j) > 1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "has several faces which belong to the same domain face.\n");

      res = 3;
      break;
    }

  ut_free_1d_int (&tmp);

  return res;
}

int
neut_tess_test_polyOrientationFace (struct TESS Tess, int i, int verbosity)
{
  int j, face;
  double *eq = ut_alloc_1d (4);
  double *centre = ut_alloc_1d (3);

  neut_tess_poly_centroid (Tess, i, centre);

  for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
  {
    face = Tess.PolyFaceNb[i][j];

    ut_array_1d_memcpy (Tess.FaceEq[face], 4, eq);
    ut_array_1d_scale (eq, 4, Tess.PolyFaceOri[i][j]);

    if (ut_space_point_plane_side (centre - 1, eq) != -1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "(%d) face %d is not properly oriented (centre %f %f %f vs FaceEq*FaceOri %f %f %f %f FaceOri %d.\n",
                          j, face, centre[0], centre[1], centre[2], eq[0],
                          eq[1], eq[2], eq[3], Tess.PolyFaceOri[i][j]);

      return 1;
    }
  }

  ut_free_1d (&eq);
  ut_free_1d (&centre);

  return 0;
}

/*
int
neut_tess_test_polyOrientationFace (struct TESS Tess, int i, int verbosity)
{
  int j, k, face, res = 0;
  int* edge_faces = NULL;
  int edgeqty;
  int* edge = NULL;
  int* face_ori = ut_alloc_1d_int (2);
  int* edge_ori = ut_alloc_1d_int (2);

  neut_tess_poly_edges (Tess, i, &edge, &edgeqty);

  for (j = 0; j < edgeqty; j++)
  {
    neut_tess_poly_edge_faces (Tess, i, edge[j], &edge_faces);

    printf ("j = %d edge = %d\n", j, edge[j]);

    printf ("face = ");
    for (k = 0; k < 2; k++)
    {
      face = edge_faces[k];
      printf ("%d ", face);
      neut_tess_poly_face_ori (Tess, i, face, &(face_ori[k]));
      neut_tess_face_edge_ori (Tess, face, edge[j], &(edge_ori[k]));
    }
    printf ("\n");

    printf ("edge_ori = %d %d face_ori = %d %d\n", edge_ori[0], edge_ori[1], face_ori[0], face_ori[1]);

    if (edge_ori[0] * edge_ori[1] * face_ori[0] * face_ori[1] != -1)
    {
      res = 1;
      break;
    }
  }

  ut_free_1d_int (&edge_faces);
  ut_free_1d_int (&edge);
  ut_free_1d_int (&face_ori);
  ut_free_1d_int (&edge_ori);

  return res;
}
*/

// Why this function is described in neper_math.pdf.
int
neut_tess_test_poly_pinching (struct TESS Tess, int poly, int verbosity)
{
  int i, j, k, l, face, *faces = ut_alloc_1d_int (2);
  int pos, ver, verqty = 0, *vers = NULL;
  int status, verbef, veraft;

  status = 0;

  // testing every pair of faces to see if the intersect elsewhere than
  // along an edge.
  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    faces[0] = Tess.PolyFaceNb[poly][i];

    for (j = i + 1; j <= Tess.PolyFaceQty[poly]; j++)
    {
      faces[1] = Tess.PolyFaceNb[poly][j];

      // if they have more than 2 common vertices, this may happen.
      neut_tess_faces_comvers (Tess, faces, 2, &vers, &verqty);

      // if they have more than 3 common vertices, this does happen.
      if (verqty > 3)
        status = -1;

      // if they have 3 common vertices and if one of them is not a
      // neighbor of the 2 others, this happens.
      else if (verqty == 3)
      {
        for (k = 0; k < 2; k++)
        {
          face = faces[k];

          for (l = 0; l < verqty; l++)
          {
            ver = vers[l];
            pos =
              ut_array_1d_int_eltpos (Tess.FaceVerNb[face] + 1,
                                      Tess.FaceVerQty[face], ver);
            if (pos == -1)
              ut_print_neperbug ();

            pos++;

            verbef =
              Tess.
              FaceVerNb[face][ut_array_rotpos
                              (1, Tess.FaceVerQty[face], pos, -1)];
            veraft =
              Tess.
              FaceVerNb[face][ut_array_rotpos
                              (1, Tess.FaceVerQty[face], pos, 1)];
            if (ut_array_1d_int_eltpos (vers, verqty, verbef) == -1
                && ut_array_1d_int_eltpos (vers, verqty, veraft) == -1)
            {
              status = 1;
              break;
            }
          }

          if (status)
            break;
        }

        if (status)
          break;
      }

      if (status)
        break;
    }

    if (status)
      break;
  }

  if (status && verbosity)
    ut_print_message (2, 3, "number of vers = %d > 2.\n", verqty);

  ut_free_1d_int (&faces);
  ut_free_1d_int (&vers);

  return status;
}
