/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_poly_geom_.h"

void
neut_poly_bbox (struct POLY Poly, double **bbox)
{
  int i, j;

  bbox[0][0] = bbox[1][0] = bbox[2][0] = DBL_MAX;
  bbox[0][1] = bbox[1][1] = bbox[2][1] = -DBL_MAX;

  for (j = 0; j < 3; j++)
    for (i = 1; i <= Poly.VerQty; i++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Poly.VerCoo[i][j]);
      bbox[j][1] = ut_num_min (bbox[j][1], Poly.VerCoo[i][j]);
    }

  return;
}

int
neut_poly_volume (struct POLY Poly, double *pvol)
{
  int i, ver, f;
  double area, h, proj[3], eq[4], centre[3];

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (Poly.FaceQty == 0)
  {
    (*pvol) = 0;
    return 1;
  }

  neut_poly_vercentroid (Poly, centre);

  (*pvol) = 0;
  for (f = 1; f <= Poly.FaceQty; f++)
  {
    ver = Poly.FaceVerNb[f][1];

    for (i = 1; i < Poly.FaceVerQty[f]; i++)
    {
      area =
        ut_space_triangle_area (Poly.VerCoo[ver],
                                Poly.VerCoo[Poly.FaceVerNb[f][i]],
                                Poly.VerCoo[Poly.FaceVerNb[f][i + 1]]);
      if (!isnan (area))
      {
        ut_array_1d_memcpy (centre, 3, proj);
        ut_space_points_plane (Poly.VerCoo[ver],
                               Poly.VerCoo[Poly.FaceVerNb[f][i]],
                               Poly.VerCoo[Poly.FaceVerNb[f][i + 1]], eq);
        ut_space_point_dir_plane_proj (proj, eq + 1, eq, proj);
        h = ut_space_dist (centre, proj);
        if (!isnan (h))
          (*pvol) += area * h * 0.3333333333333333333333;
      }
    }

    area =
      ut_space_triangle_area (Poly.VerCoo[ver],
                              Poly.VerCoo[Poly.
                                          FaceVerNb[f][Poly.FaceVerQty[f]]],
                              Poly.VerCoo[Poly.FaceVerNb[f][1]]);
    if (!isnan (area))
    {
      ut_array_1d_memcpy (centre, 3, proj);
      ut_space_points_plane (Poly.VerCoo[ver],
                             Poly.VerCoo[Poly.
                                         FaceVerNb[f][Poly.FaceVerQty[f]]],
                             Poly.VerCoo[Poly.FaceVerNb[f][1]], eq);
      ut_space_point_dir_plane_proj (proj, eq + 1, eq, proj);
      h = ut_space_dist (centre, proj);
      if (!isnan (h))
        (*pvol) += area * h * 0.3333333333333333333333;
    }
  }

  return 0;
}

int
neut_poly_volume_2d (struct POLY Poly, double Size, double *pval)
{
  neut_poly_volume (Poly, pval);
  (*pval) /= Size;

  return 0;
}

int
neut_polys_volume (struct POLY *Poly, int *polys, int polyqty, double *pvol)
{
  int i, poly;
  double tmp;

  (*pvol) = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    neut_poly_volume (Poly[poly], &tmp);
    (*pvol) += tmp;
  }

  return 0;
}

int
neut_polys_volume_2d (struct POLY *Poly, int *polys, int polyqty, double Size,
                      double *pval)
{
  neut_polys_volume (Poly, polys, polyqty, pval);
  (*pval) /= Size;

  return 0;
}

void
neut_poly_diameq (struct POLY Poly, double *pval)
{
  neut_poly_volume (Poly, pval);
  ut_space_volume_diameq (*pval, pval);

  return;
}

void
neut_poly_diameq_2d (struct POLY Poly, double h, double *pval)
{
  neut_poly_volume (Poly, pval);
  (*pval) /= h;
  ut_space_area_diameq (*pval, pval);

  return;
}

void
neut_poly_radeq (struct POLY Poly, double *pval)
{
  neut_poly_diameq (Poly, pval);
  (*pval) *= 0.5;

  return;
}

void
neut_poly_radeq_2d (struct POLY Poly, double h, double *pval)
{
  neut_poly_diameq_2d (Poly, h, pval);
  (*pval) *= 0.5;

  return;
}

void
neut_polys_diameq (struct POLY *Poly, int *polys, int polyqty, double *pval)
{
  int i, poly;
  double tmp;

  if (polyqty == 1)
  {
    neut_poly_diameq (Poly[polys[0]], pval);
    return;
  }

  (*pval) = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    neut_poly_volume (Poly[poly], &tmp);
    (*pval) += tmp;
  }
  (*pval) = pow ((6 / M_PI) * (*pval), 0.3333333333333333333333333);

  return;
}

void
neut_polys_diameq_2d (struct POLY *Poly, int *polys, int polyqty, double h,
                      double *pval)
{
  neut_polys_volume (Poly, polys, polyqty, pval);
  (*pval) /= h;
  (*pval) = sqrt ((4 / M_PI) * (*pval));

  return;
}

int
neut_poly_vercentroid (struct POLY Poly, double *coo)
{
  int i;

  ut_array_1d_zero (coo, 3);

  for (i = 1; i <= Poly.VerQty; i++)
    ut_array_1d_add (coo, Poly.VerCoo[i], 3, coo);
  ut_array_1d_scale (coo, 3, 1. / Poly.VerQty);

  return 0;
}

int
neut_poly_centroid (struct POLY Poly, double *coo)
{
  int i, j, ver, f;
  double area;
  double *p0 = NULL, *p1 = NULL, *p2 = NULL, *tmp = NULL, *C = NULL;
  double vol, totvol;

  if (neut_poly_isvoid (Poly))
  {
    ut_array_1d_set (coo, 3, sqrt (-1));
    return -1;
  }

  p0 = ut_alloc_1d (3);
  tmp = ut_alloc_1d (3);
  C = ut_alloc_1d (3);

  for (j = 0; j < 3; j++)
    for (i = 1; i <= Poly.VerQty; i++)
      C[j] += Poly.VerCoo[i][j];
  ut_array_1d_scale (C, 3, 1. / Poly.VerQty);

  totvol = 0;
  ut_array_1d_set (coo, 3, 0);

  for (f = 1; f <= Poly.FaceQty; f++)
  {
    ver = Poly.FaceVerNb[f][1];
    ut_array_1d_memcpy (Poly.VerCoo[ver], 3, p0);

    for (i = 1; i <= Poly.FaceVerQty[f]; i++)
    {
      p1 = Poly.VerCoo[Poly.FaceVerNb[f][i]];
      p2 =
        Poly.VerCoo[Poly.
                    FaceVerNb[f][ut_array_rotpos
                                 (1, Poly.FaceVerQty[f], i, 1)]];

      area = ut_space_triangle_area (p0, p1, p2);

      if (area > 1e-20)
      {
        ut_space_tet_centre (C, p0, p1, p2, tmp);
        vol = ut_space_tet_volume (C, p0, p1, p2);
        ut_array_1d_scale (tmp, 3, vol);
        ut_array_1d_add (coo, tmp, 3, coo);
        totvol += vol;
      }
    }
  }

  ut_array_1d_scale (coo, 3, 1. / totvol);
  ut_free_1d (&p0);
  ut_free_1d (&tmp);
  ut_free_1d (&C);
  return 0;
}

int
neut_poly_point_in (struct POLY Poly, double *coo)
{
  int i;
  int status = 0;
  double *C = ut_alloc_1d (3);

  neut_poly_centroid (Poly, C);

  status = 1;
  for (i = 1; i <= Poly.FaceQty; i++)
    if (ut_space_point_plane_side (coo - 1, Poly.FaceEq[i]) !=
        ut_space_point_plane_side (C - 1, Poly.FaceEq[i]))
    {
      status = 0;
      break;
    }

  ut_free_1d (&C);

  return status;
}

// distance function from the poly boundary
int
neut_poly_point_distfromboundary (struct POLY Poly, double *coo,
                                  double *pdist)
{
  int i, status, edgeqty, **edges = NULL;
  double t, tmp;
  int verbosity = 0;

  (*pdist) = DBL_MAX;
  // testing vertices
  for (i = 1; i <= Poly.VerQty; i++)
  {
    tmp = ut_space_dist (coo, Poly.VerCoo[i]);
    if (tmp < *pdist)
    {
      (*pdist) = tmp;
      if (verbosity)
        printf ("closest ver = %d with dist = %f\n", i, *pdist);
    }
  }

  // testing edges.  We may think that coo may be closest to an edge than
  // to the closest vertex only if the vertex belongs to the edge, but
  // this is not true.  So, testing all edges.
  neut_poly_edges (Poly, &edges, &edgeqty);
  for (i = 0; i < edgeqty; i++)
  {
    ut_space_segment_point_proj (Poly.VerCoo[edges[i][0]],
                                 Poly.VerCoo[edges[i][1]], coo, NULL, &t,
                                 &tmp);
    if (t >= 1e-6 && t <= 1 - 1e-6)
    {
      if (tmp < *pdist)
      {
        (*pdist) = tmp;
        if (verbosity)
          printf ("closest edge = %d-%d with dist = %f\n", edges[i][0],
                  edges[i][1], *pdist);
      }
    }
  }

  // testing all faces (same reason as above)
  for (i = 1; i <= Poly.FaceQty; i++)
  {
    status = neut_poly_point_inface (Poly, coo, i);

    if (status)
    {
      tmp = ut_space_point_plane_dist (coo, Poly.FaceEq[i]);
      if (tmp < *pdist)
      {
        (*pdist) = tmp;
        if (verbosity)
        {
          printf ("closest face = ");
          ut_array_1d_int_fprintf_nonl (stdout, Poly.FaceVerNb[i] + 1,
                                        Poly.FaceVerQty[i], "%d");
          printf (" with dist = %f\n", *pdist);
        }
      }
      (*pdist) = ut_num_min (*pdist, tmp);
    }
  }

  ut_free_2d_int (&edges, edgeqty);

  return 0;
}

// distance function from the poly boundary
int
neut_poly_point_distfromboundary_2d (struct POLY Poly, double *coo,
                                     double *pdist)
{
  int i, status, edgeqty, **edges = NULL;
  double t, tmp;
  int verbosity = 0;

  (*pdist) = DBL_MAX;
  // testing vertices
  for (i = 1; i <= Poly.VerQty; i++)
  {
    tmp = ut_space_dist (coo, Poly.VerCoo[i]);
    if (tmp < *pdist)
    {
      (*pdist) = tmp;
      if (verbosity)
        printf ("closest ver = %d with dist = %f\n", i, *pdist);
    }
  }

  // testing edges.  We may think that coo may be closest to an edge than
  // to the closest vertex only if the vertex belongs to the edge, but
  // this is not true.  So, testing all edges.
  neut_poly_edges (Poly, &edges, &edgeqty);
  for (i = 0; i < edgeqty; i++)
  {
    ut_space_segment_point_proj (Poly.VerCoo[edges[i][0]],
                                 Poly.VerCoo[edges[i][1]], coo, &t, &tmp,
                                 NULL);
    if (t >= 1e-6 && t <= 1 - 1e-6)
    {
      if (tmp < *pdist)
      {
        (*pdist) = tmp;
        if (verbosity)
          printf ("closest edge = %d-%d with dist = %f\n", edges[i][0],
                  edges[i][1], *pdist);
      }
    }
  }

  // testing all faces (same reason as above)
  for (i = 1; i <= Poly.FaceQty; i++)
  {
    if (fabs (Poly.FaceEq[i][3]) > 0.99)
      continue;

    status = neut_poly_point_inface (Poly, coo, i);

    if (status)
    {
      tmp = ut_space_point_plane_dist (coo, Poly.FaceEq[i]);
      if (tmp < *pdist)
      {
        (*pdist) = tmp;
        if (verbosity)
        {
          printf ("closest face = ");
          ut_array_1d_int_fprintf_nonl (stdout, Poly.FaceVerNb[i] + 1,
                                        Poly.FaceVerQty[i], "%d");
          printf (" with dist = %f\n", *pdist);
        }
      }
      (*pdist) = ut_num_min (*pdist, tmp);
    }
  }

  ut_free_2d_int (&edges, edgeqty);

  return 0;
}

// signed distance function from the poly boundary
int
neut_poly_point_distfromboundary_signed (struct POLY Poly, double *coo,
                                         double *pdist)
{
  neut_poly_point_distfromboundary (Poly, coo, pdist);

  if (neut_poly_point_in (Poly, coo))
    (*pdist) *= -1;

  return 0;
}

// signed distance function from the poly boundary
int
neut_poly_point_distfromboundary_2d_signed (struct POLY Poly, double *coo,
                                            double *pdist)
{
  neut_poly_point_distfromboundary_2d (Poly, coo, pdist);

  if (neut_poly_point_in (Poly, coo))
    (*pdist) *= -1;

  return 0;
}

// distance function from the poly
int
neut_poly_point_dist_legacy (struct POLY Poly, double *coo, double *pdist)
{
  neut_poly_point_distfromboundary_signed (Poly, coo, pdist);

  (*pdist) = ut_num_max (0, *pdist);

  return 0;
}

int
neut_poly_point_inface (struct POLY Poly, double *coo, int face)
{
  int i, ver1, ver2, status;
  double *centre = ut_alloc_1d (3);

  neut_poly_face_centre (Poly, face, centre);

  status = 0;
  for (i = 1; i <= Poly.FaceVerQty[face]; i++)
  {
    ver1 = Poly.FaceVerNb[face][i];
    ver2 =
      Poly.FaceVerNb[face][ut_array_rotpos (1, Poly.FaceVerQty[face], i, 1)];

    if (ut_space_triangle_point_in
        (centre, Poly.VerCoo[ver1], Poly.VerCoo[ver2], coo, 1e-6, 0, NULL))
    {
      status = 1;
      break;
    }
  }

  ut_free_1d (&centre);

  return status;
}

int
neut_poly_face_area (struct POLY Poly, int face, double *parea)
{
  int i, ver;
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;

  if (face < 1 || face > Poly.FaceQty)
    ut_print_neperbug ();

  p0 = ut_alloc_1d (3);

  ver = Poly.FaceVerNb[face][1];
  ut_array_1d_memcpy (Poly.VerCoo[ver], 3, p0);

  (*parea) = 0;
  for (i = 1; i < Poly.FaceVerQty[face]; i++)
  {
    p1 = Poly.VerCoo[Poly.FaceVerNb[face][i]];
    p2 = Poly.VerCoo[Poly.FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = Poly.VerCoo[Poly.FaceVerNb[face][Poly.FaceVerQty[face]]];
  p2 = Poly.VerCoo[Poly.FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (&p0);

  return 0;
}

void
neut_poly_face_centre (struct POLY Poly, int face, double *centre)
{
  int i, ver1, ver2;
  double *coo = ut_alloc_1d (3);
  double *tmp = ut_alloc_1d (3);
  double val, totval;

  ut_array_1d_zero (centre, 3);
  totval = 0;
  ut_array_1d_memcpy (Poly.VerCoo[Poly.FaceVerNb[face][1]], 3, coo);

  // i to last-1 because we are using ver 1 as ref pt
  for (i = 2; i < Poly.FaceVerQty[face]; i++)
  {
    ver1 = Poly.FaceVerNb[face][i];
    ver2 =
      Poly.FaceVerNb[face][ut_array_rotpos (1, Poly.FaceVerQty[face], i, 1)];
    ut_space_triangle_centre (Poly.VerCoo[ver1], Poly.VerCoo[ver2], coo, tmp);
    val = ut_space_triangle_area (Poly.VerCoo[ver1], Poly.VerCoo[ver2], coo);

    ut_array_1d_scale (tmp, 3, val);
    ut_array_1d_add (centre, tmp, 3, centre);
    totval += val;
  }
  ut_array_1d_scale (centre, 3, 1 / totval);

  ut_free_1d (&coo);
  ut_free_1d (&tmp);

  return;
}

void
neut_poly_area (struct POLY Poly, double *pval)
{
  int i;
  double tmp;

  (*pval) = 0;
  for (i = 1; i <= Poly.FaceQty; i++)
  {
    neut_poly_face_area (Poly, i, &tmp);
    (*pval) += tmp;
  }

  return;
}

void
neut_polys_area (struct POLY *Poly, int *polys, int polyqty, double *pval)
{
  int i, j, poly;
  double tmp;

  (*pval) = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];

    for (j = 1; j <= Poly[poly].FaceQty; j++)
    {
      if (ut_array_1d_int_eltpos (polys, polyqty, Poly[poly].FacePoly[j]) ==
          -1)
      {
        neut_poly_face_area (Poly[poly], j, &tmp);
        (*pval) += tmp;
      }
    }
  }

  return;
}

int
neut_poly_sphericity (struct POLY Poly, double *pval)
{
  double diameq, tmp;

  if (neut_poly_isvoid (Poly))
  {
    (*pval) = sqrt (-1);
    return -1;
  }

  neut_poly_diameq (Poly, &diameq);
  neut_poly_area (Poly, &tmp);
  (*pval) = M_PI * diameq * diameq / tmp;

  return 0;
}

int
neut_poly_sphericity_2d (struct POLY Poly, double Size, double *pval)
{
  int i, j, ver1, ver2;
  double diameq, tmp;

  if (neut_poly_isvoid (Poly))
  {
    (*pval) = sqrt (-1);
    return -1;
  }

  neut_poly_diameq_2d (Poly, Size, &diameq);

  // this peaks one of the top or bottom surface and then "breaks"
  tmp = 0;
  for (i = 1; i <= Poly.FaceQty; i++)
    if (fabs (Poly.FaceEq[i][3]) > 0.99)
    {
      for (j = 1; j <= Poly.FaceVerQty[i]; j++)
      {
        ver1 = Poly.FaceVerNb[i][j];
        ver2 =
          Poly.FaceVerNb[i][ut_array_rotpos (1, Poly.FaceVerQty[i], j, 1)];
        tmp += ut_space_dist (Poly.VerCoo[ver1], Poly.VerCoo[ver2]);
      }

      break;
    }

  (*pval) = M_PI * diameq / tmp;

  return 0;
}

int
neut_polys_sphericity (struct POLY *Poly, int *polys, int polyqty,
                       double *pval)
{
  double diameq, tmp;

  neut_polys_diameq (Poly, polys, polyqty, &diameq);
  neut_polys_area (Poly, polys, polyqty, &tmp);
  (*pval) = M_PI * diameq * diameq / tmp;

  return 0;
}

int
neut_polys_size_sphericity (struct POLY *Poly, int *polys, int polyqty,
                            double size, double *pval)
{
  double tmp;

  neut_polys_area (Poly, polys, polyqty, &tmp);
  (*pval) = M_PI * pow (6 / M_PI * size, 0.666666666666666666666666) / tmp;

  return 0;
}

int
neut_polys_convexity (struct POLY *Poly, int *polys, int polyqty,
                      double *pval)
{
  double vol, vol2;
  struct NODES N;
  struct MESH M;
  struct MESH M3;

  if (polyqty == 1)
  {
    (*pval) = 1;
    return 0;
  }

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (&M3);

  neut_polys_volume (Poly, polys, polyqty, &vol2);

  neut_polys_convexhull (Poly, polys, polyqty, &N, &M);
  neut_mesh2d_mesh3d (&N, M, &M3);
  neut_mesh_volume (N, M3, &vol);

  (*pval) = vol2 / vol;
  if (*pval > 1 + 1e-6 || *pval < 0)
  {
    printf ("vol  = %.15f\n", vol);
    printf ("vol2 = %.15f\n", vol2);
    printf ("convexity = %.15f\n", *pval);
    ut_print_neperbug ();
  }

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  neut_mesh_free (&M3);

  return 0;
}

int
neut_polys_sphericity_2d (struct POLY *Poly, int *polys, int polyqty,
                          double pseudosize, double *pval)
{
  double size;

  neut_polys_diameq_2d (Poly, polys, polyqty, pseudosize, &size);
  size = M_PI / 4 * size * size;

  return neut_polys_size_sphericity_2d (Poly, polys, polyqty, size, pval);
}

int
neut_polys_size_sphericity_2d (struct POLY *Poly, int *polys, int polyqty,
                               double size, double *pval)
{
  int i, j, k, ver1, ver2, poly;
  double diameq, tmp;
  int polyqty2, *polys2 = NULL;

  diameq = sqrt (4 / M_PI * size);

  tmp = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    for (j = 1; j <= Poly[poly].FaceQty; j++)
      if (fabs (Poly[poly].FaceEq[j][3]) > 0.99)
      {
        for (k = 1; k <= Poly[poly].FaceVerQty[j]; k++)
        {
          ver1 = Poly[poly].FaceVerNb[j][k];
          ver2 =
            Poly[poly].
            FaceVerNb[j][ut_array_rotpos (1, Poly[poly].FaceVerQty[j], k, 1)];
          neut_poly_verpair_compolys (Poly[poly], ver1, ver2, &polys2,
                                      &polyqty2);
          if (polyqty2 > 1)
            abort ();
          if (polyqty2 == 0
              || ut_array_1d_int_eltpos (polys, polyqty, polys2[0]) == -1)
            tmp +=
              ut_space_dist (Poly[poly].VerCoo[ver1],
                             Poly[poly].VerCoo[ver2]);
        }

        break;
      }
  }

  (*pval) = M_PI * diameq / tmp;

  ut_free_1d_int (&polys2);

  return 0;
}

int
neut_polys_centroid (struct POLY *Poly, int *polys, int polyqty, double *coo)
{
  int i, poly, status;
  double **coos = NULL, *vols = NULL;

  status = -1;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    if (!neut_poly_isvoid (Poly[poly]))
    {
      status = 0;
      break;
    }
  }

  if (status == -1)
  {
    ut_array_1d_set (coo, 3, sqrt (-1));
    return -1;
  }

  coos = ut_alloc_2d (polyqty, 3);
  vols = ut_alloc_1d (polyqty);

  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    neut_poly_centroid (Poly[poly], coos[i]);
    neut_poly_volume (Poly[poly], vols + i);
  }

  ut_array_1d_zero (coo, 3);
  for (i = 0; i < polyqty; i++)
  {
    ut_array_1d_scale (coos[i], 3, vols[i]);
    ut_array_1d_add (coo, coos[i], 3, coo);
  }
  ut_array_1d_scale (coo, 3, 1. / ut_array_1d_sum (vols, polyqty));

  ut_free_2d (&coos, polyqty);
  ut_free_1d (&vols);

  return 0;
}

int
neut_polys_point_dist (struct POLY *Poly, int *polys, int polyqty,
                       double *coo, double *pdist)
{
  int i, poly, scanqty;
  double *tmp = ut_alloc_1d (polyqty);
  int *polys2 = ut_alloc_1d_int (polyqty);
  int *ids = ut_alloc_1d_int (polyqty);

  ut_array_1d_int_memcpy (polys, polyqty, polys2);

  (*pdist) = DBL_MAX;
  scanqty = 0;
  for (i = 0; i < polyqty; i++)
  {
    scanqty++;
    poly = polys[i];
    neut_poly_point_dist (Poly[poly], coo, tmp + i);
    if (tmp[i] == 0)
      break;
  }

  ut_array_1d_sort_index (tmp, scanqty, ids);

  for (i = 0; i < scanqty; i++)
    polys[i] = polys2[ids[i]];

  (*pdist) = tmp[ids[0]];

  ut_free_1d (&tmp);
  ut_free_1d_int (&ids);
  ut_free_1d_int (&polys2);

  return 0;
}

int
neut_polys_point_dist_legacy (struct POLY *Poly, int *polys, int polyqty,
                              double *coo, double *pdist)
{
  int i, poly;
  double tmp;

  (*pdist) = DBL_MAX;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    neut_poly_point_dist_legacy (Poly[poly], coo, &tmp);
    (*pdist) = ut_num_min (*pdist, tmp);
    if (*pdist == 0)
      break;
  }

  return 0;
}

int
neut_poly_point_dist (struct POLY Poly, double *ptcoo, double *pdist)
{
  int status;

  status =
    ut_space_polypts_point_dist (Poly.VerCoo + 1, Poly.VerQty, ptcoo, pdist);
  if (status == -1)
#ifdef DEVEL_DEBUGGING_TEST
    abort ();
#else
    status = neut_poly_point_dist_legacy (Poly, ptcoo, pdist);
#endif

  return status;
}

void
neut_poly_convexity (struct POLY Poly, double *pval)
{
  double vol, vol2;
  struct NODES N;
  struct MESH M;
  struct MESH M3;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (&M3);

  neut_poly_volume (Poly, &vol2);

  neut_poly_convexhull (Poly, &N, &M);
  neut_mesh2d_mesh3d (&N, M, &M3);
  neut_mesh_volume (N, M3, &vol);

  (*pval) = vol2 / vol;
  if (*pval > 1 + 1e-6 || *pval < 0)
  {
    printf ("vol  = %.15f\n", vol);
    printf ("vol2 = %.15f\n", vol2);
    printf ("convexity = %.15f\n", *pval);
    ut_print_neperbug ();
  }

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  neut_mesh_free (&M3);

  return;
}

void
neut_poly_convexhull (struct POLY Poly, struct NODES *pN, struct MESH *pM)
{
  int i, j, verqty;
  double eps = 1e-9;
  double **vercoos = NULL;
  double distmin, dist;
  int distminid;
  int *pt = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  srand48 (1);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  vercoos = ut_alloc_2d (Poly.VerQty, 3);
  verqty = Poly.VerQty;
  for (i = 0; i < verqty; i++)
    ut_array_1d_memcpy (Poly.VerCoo[i + 1], 3, vercoos[i]);

  for (i = 0; i < verqty; i++)
    for (j = 0; j < 3; j++)
      vercoos[i][j] += eps * drand48 ();

  net_pts_convexhull (vercoos, verqty, 3, pN, pM);

  for (i = 0; i < verqty; i++)
  {
    distmin = DBL_MAX;
    distminid = -1;
    for (j = 1; j <= (*pN).NodeQty; j++)
    {
      dist = ut_space_dist (vercoos[i], (*pN).NodeCoo[j]);
      if (dist < distmin)
      {
        distmin = dist;
        distminid = j;
      }
    }

    if (distmin < eps)
      ut_array_1d_memcpy (Poly.VerCoo[i + 1], 3, (*pN).NodeCoo[distminid]);
  }

  ut_free_1d (&coo);
  ut_free_1d_int (&pt);

  ut_free_2d (&vercoos, verqty);

  return;
}

void
neut_polys_convexhull (struct POLY *Poly, int *polys, int polyqty,
                       struct NODES *pN, struct MESH *pM)
{
  int i, j, verqty, poly, id;
  double eps = 1e-6;
  double **vercoos = NULL;
  double **vercoos0 = NULL;
  double distmin, dist;
  int distminid;
  int *pt = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  srand48 (1);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  verqty = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    verqty += Poly[poly].VerQty;
  }

  vercoos0 = ut_alloc_2d (verqty, 3);
  id = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    for (j = 1; j <= Poly[poly].VerQty; j++)
      ut_array_1d_memcpy (Poly[poly].VerCoo[j], 3, vercoos0[id++]);
  }

  vercoos = ut_alloc_2d (verqty, 3);
  ut_array_2d_memcpy (vercoos0, verqty, 3, vercoos);
  for (i = 0; i < verqty; i++)
    for (j = 0; j < 3; j++)
      vercoos[i][j] += eps * drand48 ();

  net_pts_convexhull (vercoos, verqty, 3, pN, pM);

  for (i = 0; i < verqty; i++)
  {
    distmin = DBL_MAX;
    distminid = -1;
    for (j = 1; j <= (*pN).NodeQty; j++)
    {
      dist = ut_space_dist (vercoos[i], (*pN).NodeCoo[j]);
      if (dist < distmin)
      {
        distmin = dist;
        distminid = j;
      }
    }

    if (distmin < eps)
      ut_array_1d_memcpy (vercoos0[i], 3, (*pN).NodeCoo[distminid]);
  }

  ut_free_1d (&coo);
  ut_free_1d_int (&pt);

  ut_free_2d (&vercoos, verqty);

  return;
}
