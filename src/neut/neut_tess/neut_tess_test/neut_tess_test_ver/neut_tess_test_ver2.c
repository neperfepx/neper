/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_ver_.h"

/* we check is the number of edge is >= 3
 * if it does not have two identical edges
 * and if the edges are useful.
 */
int
neut_tess_test_verEdgeQtyNNb (struct TESS Tess, int i, int verbosity)
{
  int j, edge, res, min;

  min = Tess.Dim;
  if (Tess.VerEdgeQty[i] < min)
  {
    if (verbosity)
      ut_print_message (2, 3, "Number of edges = %d < %d.\n",
                        Tess.VerEdgeQty[i], min);

    return 1;
  }

  ut_array_1d_int_uniq (Tess.VerEdgeNb[i], Tess.VerEdgeQty[i] - 1, &res);
  if (res != Tess.VerEdgeQty[i] - 1)
  {
    if (verbosity)
    {
      ut_print_message (2, 3, "2 identical edges in edge list.\n");
      ut_array_1d_int_fprintf (stdout, Tess.VerEdgeNb[i], Tess.VerEdgeQty[i],
                               "%d");
    }

    return 1;
  }

  for (j = 0; j < Tess.VerEdgeQty[i]; j++)
  {
    edge = Tess.VerEdgeNb[i][j];
    if (Tess.EdgeState[edge] == -1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "edge %d is in edge list, but also marked as removed.\n",
                          i, edge);

      return 1;
    }
  }

  return 0;
}

/* we check the reciprocity of each ver edge */
int
neut_tess_test_verEdgeReciprocity (struct TESS Tess, int i, int verbosity)
{
  int j;
  int edge;

  for (j = 0; j <= Tess.VerEdgeQty[i] - 1; j++)
  {
    edge = Tess.VerEdgeNb[i][j];
    if (ut_array_1d_int_eltpos (Tess.EdgeVerNb[edge], 2, i) == -1)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "edge %d is in edge list, but edge not based on ver.\n",
                          edge);

      return 1;
    }
  }

  return 0;
}

// Chacking that the ver is at the right place on the domain
int
neut_tess_test_verBoundNCoo (struct TESS Tess, int ver, int verbosity)
{
  int i;
  int qty;
  int *face = NULL;
  double dist;

  if (Tess.Dim < 3)
    return 0;

  neut_tess_ver_domfaces (Tess, ver, &face, &qty);
  for (i = 0; i < qty; i++)
  {
    dist =
      ut_space_point_plane_dist (Tess.VerCoo[ver], Tess.DomFaceEq[face[i]]);
    if (dist > 1e-8)
    {
      if (verbosity)
        ut_print_message (2, 3,
                          "belongs to domain face %d, but does not lie on the face plane (dist = %.g > 1e-8)\n",
                          face[i], dist);

      return 1;
    }
  }

  ut_free_1d_int (&face);

  return 0;
}

int
neut_tess_test_verFaceCoplaneity (struct TESS Tess, int ver, int verbosity)
{
  int i, j, k;
  int edge1, edge2;
  int face;
  int com;

  for (i = 0; i < Tess.VerEdgeQty[ver] - 1; i++)
  {
    edge1 = Tess.VerEdgeNb[ver][i];

    for (j = i + 1; j < Tess.VerEdgeQty[ver]; j++)
    {
      edge2 = Tess.VerEdgeNb[ver][j];

      com = 0;
      for (k = 0; k < Tess.EdgeFaceQty[edge1]; k++)
      {
        face = Tess.EdgeFaceNb[edge1][k];
        if (ut_array_1d_int_eltpos
            (Tess.EdgeFaceNb[edge2], Tess.EdgeFaceQty[edge2], face) != -1)
          com++;
      }

      if (com > 1)
      {
        if (verbosity)
          ut_print_message (2, 3,
                            "has edges %d and %d, but these edges share more than 1 (= %d) face.\n",
                            edge1, edge2, com);

        return 2;
      }
    }
  }

  return 0;
}
