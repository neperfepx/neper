/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_face_.h"

int
neut_tess_test_faceReciprocityEdge (struct TESS Tess, int i, int verbosity)
{
  int j;
  int edge;

  if (Tess.FaceVerQty[i] < 3)
  {
    if (verbosity)
      ut_print_message (2, 3, "Number of vertices/edges = %d < 3.\n",
                        Tess.FaceVerQty[i]);

    return 2;
  }

  for (j = 1; j <= Tess.FaceVerQty[i]; j++)
  {
    edge = Tess.FaceEdgeNb[i][j];
    if (ut_array_1d_int_eltpos
        (Tess.EdgeFaceNb[edge], Tess.EdgeFaceQty[edge], i) == -1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "based on edge %d, but face not in edge face list.\n",
                          edge);

      return 2;
    }
  }

  return 0;
}

int
neut_tess_test_faceReciprocityVer (struct TESS Tess, int i, int verbosity)
{
  int j, status;
  int ver;
  int faceqty;
  int *face = NULL;

  status = 0;
  for (j = 1; j <= Tess.FaceVerQty[i]; j++)
  {
    ver = Tess.FaceVerNb[i][j];
    neut_tess_ver_faces (Tess, ver, &face, &faceqty);
    if (ut_array_1d_int_eltpos (face, faceqty, i) == -1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "based on ver %d, but face not found in ver faces.\n",
                          ver);

      status = 2;
      break;
    }

    ut_free_1d_int (&face);
  }

  ut_free_1d_int (&face);

  return status;
}

int
neut_tess_test_faceReciprocityPoly (struct TESS Tess, int i, int verbosity)
{
  int j;
  int poly;

  if (Tess.Dim == 2)
    return 0;

  if (Tess.FacePoly[i][0] < 0)
  {
    if (verbosity)
      ut_print_message (2, 3, "first poly have a negative id (%d).\n",
                        Tess.FacePoly[i][0]);

    return 2;
  }

  if (Tess.FacePoly[i][0] < 0 && Tess.FacePoly[i][1] < 0)
  {
    if (verbosity)
      ut_print_message (2, 3, "both polys have negative ids (%d, %d).\n",
                        Tess.FacePoly[i][0], Tess.FacePoly[i][1]);

    return 2;
  }

  for (j = 0; j < 2; j++)
  {
    poly = Tess.FacePoly[i][j];

    if (poly >= 1 && poly <= Tess.PolyQty)
      if (ut_array_1d_int_eltpos
          (Tess.PolyFaceNb[poly] + 1, Tess.PolyFaceQty[poly], i) == -1)
      {
        if (verbosity)
          ut_print_message (2, 3,
                            "poly %d in in poly list, but poly not based on face.\n",
                            poly);

        return 2;
      }
  }

  return 0;
}

int
neut_tess_test_faceState (struct TESS Tess, int i, int verbosity)
{
  int j, ver;

  /* if the face has state=0, it is not modified.
   * every of its vertices must be unmodified (state!=0).
   */
  if (Tess.FaceState[i] == 0)
  {
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
    {
      ver = Tess.FaceVerNb[i][j];
      if (Tess.VerState[ver] != 0)
      {
        if (verbosity)
          ut_print_message (2, 3,
                            "has state = 0, but face ver (id = %d) has state = %d != 0.\n",
                            ver, Tess.VerState[ver]);

        return 1;
      }
    }

    return 0;
  }

  /* if the face has state>0, it is modified.
   * at least one of its vertices must be modified (state!=0).
   */
  else if (Tess.FaceState[i] > 0)
  {
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
    {
      ver = Tess.FaceVerNb[i][j];
      if (Tess.VerState[ver] != 0)
        return 0;
    }

    if (verbosity)
      ut_print_message (2, 3,
                        "state = %d, but all of the face vers have state = 0.\n",
                        Tess.FaceState[i]);

    return 1;
  }
  else
    return 0;                   /* del face --> ok (maybe this is useless). */
}

/* neut_tess_test_faceBound tests whether the face has 3 aligned vertices (on a domain edge)
 * or not. The case where three of the vertices of a face have 2 com bounds is
 * considered as critical.
 */
int
neut_tess_test_faceBound (struct TESS Tess, int face, int verbosity)
{
  int i, ver, domver;

  int *bound1 = ut_alloc_1d_int (1);

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    if (Tess.VerDom[ver][0] == 1)
    {
      bound1[0]++;
      bound1 = ut_realloc_1d_int (bound1, bound1[0] + 1);
      bound1[bound1[0]] = Tess.VerDom[ver][1];
    }
    else if (Tess.VerDom[ver][0] == 0)
    {
      domver = Tess.VerDom[ver][1];
      bound1 =
        ut_realloc_1d_int (bound1,
                           bound1[0] + 1 + Tess.DomVerEdgeQty[domver]);
      ut_array_1d_int_memcpy (Tess.DomVerEdgeNb[domver],
                              Tess.DomVerEdgeQty[domver],
                              bound1 + bound1[0] + 1);
      bound1[0] += Tess.DomVerEdgeQty[domver];
    }
  }

  int *bound2 = ut_alloc_1d_int (bound1[0] + 1);
  ut_array_1d_int_memcpy (bound1, bound1[0] + 1, bound2);
  ut_array_1d_int_sort_uniq (bound2 + 1, bound2[0], &(bound2[0]));

  for (i = 1; i <= bound2[0]; i++)
    if (ut_array_1d_int_valnb (bound1 + 1, bound1[0], bound2[i]) == 3)
    {
      // 3 face vertices are on the same domain edge
      if (verbosity)
        ut_print_message (2, 3,
                          "has 3 vertices lying on the same domain edge.\n");

      return 2;
    }

  ut_free_1d_int (&bound1);
  ut_free_1d_int (&bound2);

  return 0;
}

int
neut_tess_test_faceSelfIntersect (struct TESS Tess, int face, int verbosity)
{
  int i;
  int res = 0;
  int cooqty = Tess.FaceVerQty[face];
  double **coo = ut_alloc_2d (cooqty, 3);
  double *n = ut_alloc_1d (3);
  double *sum_n = ut_alloc_1d (3);

  for (i = 0; i < Tess.FaceVerQty[face]; i++)
  {
    ut_array_1d_memcpy (Tess.VerCoo[Tess.FaceVerNb[face][i + 1]], 3, coo[i]);
    ut_space_point_dir_plane_proj (coo[i], Tess.FaceEq[face] + 1,
                                   Tess.FaceEq[face], coo[i]);
  }

  res = ut_space_contour_selfintersect (coo, cooqty);

  if (res > 0)
    if (verbosity)
      ut_print_message (2, 3, "has self-intersections (%d).\n", res);

  ut_free_2d (&coo, Tess.FaceVerQty[face]);
  ut_free_1d (&n);
  ut_free_1d (&sum_n);

  return res;
}

int
neut_tess_test_vervsedge (struct TESS Tess, int face, int verbosity)
{
  int i, ver, edge, pos;

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    edge = Tess.FaceEdgeNb[face][i];
    pos = (Tess.FaceEdgeOri[face][i] == 1) ? 0 : 1;
    if (ver != Tess.EdgeVerNb[edge][pos])
    {
      if (verbosity)
        printf ("ver and edge sequences do not match.\n");
      return 1;
    }
  }

  return 0;
}

int
neut_tess_test_face_normal (struct TESS Tess, int face, int verbosity)
{
  int status;
  double *n = ut_alloc_1d (3);

  neut_tess_face_normal_fromver (Tess, face, n);

  status = 0;
  if (ut_vector_scalprod (n, Tess.FaceEq[face] + 1) < 0)
  {
    if (verbosity)
      ut_print_message (2, 3,
                        "normal is not properly oriented (scal = %f < 0).\n",
                        ut_vector_scalprod (Tess.FaceEq[face] + 1, n));

    status = 1;
  }

  ut_free_1d (&n);

  return status;
}
