/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesl_.h"

/* neut_tesl_set_zero: set a tesl to 0 / NULL */
void
neut_tesl_set_zero (struct TESL *pTesl)
{
  (*pTesl).Id = 0;
  (*pTesl).VerQty = 0;
  (*pTesl).FaceQty = 0;
  (*pTesl).EdgeQty = 0;
  (*pTesl).PolyQty = 0;

  (*pTesl).morpho = NULL;
  (*pTesl).VerSeed = NULL;
  (*pTesl).VerEdge = NULL;
  (*pTesl).VerCoo = NULL;
  (*pTesl).VerOfDomain = NULL;
  (*pTesl).EdgeVerNb = NULL;
  (*pTesl).EdgeLength = NULL;
  (*pTesl).FacePoly = NULL;
  (*pTesl).FaceEq = NULL;
  (*pTesl).FaceVerQty = NULL;
  (*pTesl).FaceVerNb = NULL;
  (*pTesl).FaceEdgeNb = NULL;
  (*pTesl).FaceEdgeOri = NULL;
  (*pTesl).SeedCoo = NULL;
  (*pTesl).SeedWeight = NULL;
  (*pTesl).PolyVerQty = NULL;
  (*pTesl).PolyVerNb = NULL;
  (*pTesl).PolyEdgeQty = NULL;
  (*pTesl).PolyEdgeNb = NULL;
  (*pTesl).PolyFaceQty = NULL;
  (*pTesl).PolyFaceNb = NULL;
  (*pTesl).PolyFaceOri = NULL;

  return;
}

void
neut_tesl_free (struct TESL *pTesl)
{
  ut_free_1d_char (&(*pTesl).morpho);
  ut_free_2d_int (&(*pTesl).VerSeed, (*pTesl).VerQty + 1);
  ut_free_2d_int (&(*pTesl).VerEdge, (*pTesl).VerQty + 1);
  ut_free_2d (&(*pTesl).VerCoo, (*pTesl).VerQty + 1);
  ut_free_1d_int (&(*pTesl).VerOfDomain);

  ut_free_2d_int (&(*pTesl).EdgeVerNb, (*pTesl).EdgeQty + 1);
  ut_free_1d (&(*pTesl).EdgeLength);
  ut_free_2d_int (&(*pTesl).FacePoly, (*pTesl).FaceQty + 1);
  ut_free_2d (&(*pTesl).FaceEq, (*pTesl).FaceQty + 1);
  ut_free_1d_int (&(*pTesl).FaceVerQty);
  ut_free_2d_int (&(*pTesl).FaceVerNb, (*pTesl).FaceQty + 1);
  ut_free_2d_int (&(*pTesl).FaceEdgeNb, (*pTesl).FaceQty + 1);
  ut_free_2d_int (&(*pTesl).FaceEdgeOri, (*pTesl).FaceQty + 1);
  ut_free_2d (&(*pTesl).SeedCoo, (*pTesl).PolyQty + 1);
  ut_free_1d (&(*pTesl).SeedWeight);
  ut_free_1d_int (&(*pTesl).PolyVerQty);
  ut_free_2d_int (&(*pTesl).PolyVerNb, (*pTesl).PolyQty + 1);
  ut_free_1d_int (&(*pTesl).PolyEdgeQty);
  ut_free_2d_int (&(*pTesl).PolyEdgeNb, (*pTesl).PolyQty + 1);
  ut_free_1d_int (&(*pTesl).PolyFaceQty);
  ut_free_2d_int (&(*pTesl).PolyFaceNb, (*pTesl).PolyQty + 1);
  ut_free_2d_int (&(*pTesl).PolyFaceOri, (*pTesl).PolyQty + 1);

  neut_tesl_set_zero (pTesl);

  return;
}

void
neut_tesl_bbox (struct TESL Tesl, double **size)
{
  int i, j, ver;

  /* Searching the tessellation bounding box */
  size[0][0] = size[1][0] = size[2][0] = DBL_MAX;
  size[0][1] = size[1][1] = size[2][1] = -DBL_MAX;

  for (i = 1; i <= Tesl.VerOfDomain[0]; i++)
  {
    ver = Tesl.VerOfDomain[i];
    for (j = 0; j < 3; j++)
    {
      size[j][0] = ut_num_min (size[j][0], Tesl.VerCoo[ver][j]);
      size[j][1] = ut_num_max (size[j][1], Tesl.VerCoo[ver][j]);
    }
  }

  return;
}

void
neut_tesl_domainface (struct TESL Tesl, int **Face)
{
  int i, j;
  int face;

  for (face = 1; face <= 6; face++)
    Face[face][0] = 0;

  for (i = 1; i <= Tesl.FaceQty; i++)
  {
    for (j = 0; j <= 1; j++)
      if (Tesl.FacePoly[i][j] < 0)
      {
        face = -Tesl.FacePoly[i][j];

        Face[face][0]++;
        Face[face][Face[face][0]] = i;

        continue;
      }
  }

  return;
}

void
neut_tesl_domainfacever (struct TESL Tesl, int **Ver)
{
  int i, j, k;
  int face;
  int **DFace = ut_alloc_2d_int (7, Tesl.FaceQty + 1);
  int *FaceVer;

  neut_tesl_domainface (Tesl, DFace);

  for (i = 1; i <= 6; i++)
  {
    FaceVer = ut_alloc_1d_int (DFace[i][0] * 50);

    for (j = 1; j <= DFace[i][0]; j++)
    {
      face = DFace[i][j];
      for (k = 1; k <= Tesl.FaceVerQty[face]; k++)
      {
        FaceVer[0]++;
        FaceVer[FaceVer[0]] = Tesl.FaceVerNb[face][k];
      }
    }

    ut_array_1d_int_sort_uniq (FaceVer + 1, FaceVer[0], &(FaceVer[0]));

    ut_array_1d_int_memcpy (FaceVer, FaceVer[0] + 1, Ver[i]);

    ut_free_1d_int (&FaceVer);
  }

  ut_free_2d_int (&DFace, 7);

  return;
}

void
neut_tesl_edge_poly (struct TESL Tesl, int edge, int *poly)
{
  int i, ver1, ver2;

  ver1 = Tesl.EdgeVerNb[edge][0];
  ver2 = Tesl.EdgeVerNb[edge][1];

  ut_array_1d_int_inter (Tesl.VerSeed[ver1], 4, Tesl.VerSeed[ver2], 4,
                         poly + 1, &(poly[0]));

  ut_array_1d_int_sort (poly + 1, poly[0]);

  for (i = -1; i >= -6; i--)
    poly[0] -= ut_array_1d_int_rmelt (poly + 1, poly[0], i, 1);

  return;
}

int
neut_tesl_face_area (struct TESL Tesl, int face, double *parea)
{
  int i, j, ver;
  double *p0 = ut_alloc_1d (3);
  double *p1 = NULL;
  double *p2 = NULL;

  if (face < 1 || face > Tesl.FaceQty)
  {
    (*parea) = 0;
    return -1;
  }

  // taking barycenter
  ut_array_1d_zero (p0, 3);
  for (i = 1; i <= Tesl.FaceVerQty[face]; i++)
  {
    ver = Tesl.FaceVerNb[face][i];

    for (j = 0; j < 3; j++)
      p0[j] += Tesl.VerCoo[ver][j];
  }
  ut_array_1d_scale (p0, 3, 1. / Tesl.FaceVerQty[face]);

  (*parea) = 0;
  for (i = 1; i < Tesl.FaceVerQty[face]; i++)
  {
    p1 = Tesl.VerCoo[Tesl.FaceVerNb[face][i]];
    p2 = Tesl.VerCoo[Tesl.FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = Tesl.VerCoo[Tesl.FaceVerNb[face][Tesl.FaceVerQty[face]]];
  p2 = Tesl.VerCoo[Tesl.FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (&p0);

  return 0;
}

int
neut_tesl_poly_volume (struct TESL Tesl, int poly, double *pvol)
{
  int f, face;
  double area, h;
  double *proj = ut_alloc_1d (3);

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre.

  if (poly < 1 || poly > Tesl.PolyQty)
  {
    (*pvol) = 0;
    return -1;
  }

  (*pvol) = 0;
  for (f = 1; f <= Tesl.PolyFaceQty[poly]; f++)
  {
    face = Tesl.PolyFaceNb[poly][f];

    neut_tesl_face_area (Tesl, face, &area);

    ut_array_1d_memcpy (Tesl.SeedCoo[poly], 3, proj);
    ut_space_point_dir_plane_proj (proj, Tesl.FaceEq[face] + 1,
                                   Tesl.FaceEq[face], proj);
    h = ut_space_dist (Tesl.SeedCoo[poly], proj);
    (*pvol) += area * h * 0.3333333333333333333333;
  }

  ut_free_1d (&proj);

  return 0;
}

void
neut_tesl_volume (struct TESL Tesl, double *pvol)
{
  int i;
  double poly_vol;

  (*pvol) = 0;
  for (i = 1; i <= Tesl.PolyQty; i++)
  {
    neut_tesl_poly_volume (Tesl, i, &poly_vol);
    (*pvol) += poly_vol;
  }

  return;
}
