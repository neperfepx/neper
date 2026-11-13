/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_geom_.h"

int
neut_tess_cell_size (struct TESS Tess, int cell, double *psize)
{
  if (Tess.Dim == 3)
  {
    neut_tess_poly_volume (Tess, cell, psize);
    if (Tess.PseudoDim == 2)
      (*psize) /= Tess.PseudoSize;
  }
  else if (Tess.Dim == 2)
    neut_tess_face_area (Tess, cell, psize);
  else
    ut_print_neperbug ();

  return 0;
}

int
neut_tess_edge_eq (struct TESS Tess, int edge, double *eq)
{
  if (edge < 1 || edge > Tess.EdgeQty || Tess.Dim != 2)
    ut_print_neperbug ();

  ut_space_points_line (Tess.VerCoo[Tess.EdgeVerNb[edge][0]],
                        Tess.VerCoo[Tess.EdgeVerNb[edge][1]], eq);

  return 0;
}

int
neut_tess_edge_dir (struct TESS Tess, int edge, double *dir)
{
  if (edge < 1 || edge > Tess.EdgeQty)
    ut_print_neperbug ();

  ut_array_1d_sub (Tess.VerCoo[Tess.EdgeVerNb[edge][0]],
                   Tess.VerCoo[Tess.EdgeVerNb[edge][1]], 3, dir);

  ut_array_1d_normalize (dir, 3);

  return 0;
}

int
neut_tess_domedge_eq (struct TESS Tess, int domedge, double *eq)
{
  if (domedge < 1 || domedge > Tess.DomEdgeQty || Tess.Dim != 2)
    ut_print_neperbug ();

  ut_space_points_line (Tess.DomVerCoo[Tess.DomEdgeVerNb[domedge][0]],
                        Tess.DomVerCoo[Tess.DomEdgeVerNb[domedge][1]], eq);

  return 0;
}

int
neut_tess_domedge_length (struct TESS Tess, int dedge, double *pval)
{
  (*pval) =
    ut_space_dist (Tess.DomVerCoo[Tess.DomEdgeVerNb[dedge][0]],
                   Tess.DomVerCoo[Tess.DomEdgeVerNb[dedge][1]]);

  return 0;
}

int
neut_tess_face_area (struct TESS Tess, int face, double *parea)
{
  int i, ver;
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;

  if (face < 1 || face > Tess.FaceQty)
    ut_print_neperbug ();

  if (Tess.FaceVerQty[face] == 0)
  {
    (*parea) = 0;
    return 1;
  }

  p0 = ut_alloc_1d (3);

  if (Tess.FaceState[face] == -1)
  {
    (*parea) = 0;
    return -1;
  }

  if (Tess.FaceState[face] > 0)
  {
    if (Tess.FacePt[face] == 0)
      ut_array_1d_memcpy (Tess.FacePtCoo[face], 3, p0);
    else if (Tess.FacePt[face] > 0)
    {
      ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
      ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
    }
    else
      neut_tess_face_centre (Tess, face, p0);
  }
  else if (Tess.FaceState[face] == 0)
    neut_tess_face_centre (Tess, face, p0);

  (*parea) = 0;
  for (i = 1; i < Tess.FaceVerQty[face]; i++)
  {
    p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = Tess.VerCoo[Tess.FaceVerNb[face][Tess.FaceVerQty[face]]];
  p2 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (&p0);

  return 0;
}

void
neut_tess_face_normal (struct TESS Tess, int face, double *n)
{
  ut_array_1d_memcpy (Tess.FaceEq[face] + 1, 3, n);

  return;
}

int
neut_tess_faces_area (struct TESS Tess, int *faces, int faceqty,
                      double *parea)
{
  int i;
  double facearea;

  (*parea) = 0;
  for (i = 0; i < faceqty; i++)
  {
    neut_tess_face_area (Tess, faces[i], &facearea);
    (*parea) += facearea;
  }

  return 0;
}

int
neut_tess_size (struct TESS Tess, double *psize)
{
  if (Tess.Dim == 3)
  {
    neut_tess_volume (Tess, psize);
    if (Tess.PseudoDim == 2)
      (*psize) /= Tess.PseudoSize;
  }
  else if (Tess.Dim == 2)
    neut_tess_area (Tess, psize);
  else
    abort ();

  return 0;
}

int
neut_tess_diameq (struct TESS Tess, double *pval)
{
  neut_tess_size (Tess, pval);

  if (Tess.Dim == 3)
    (*pval) = sqrt ((4 / M_PI) * (*pval));
  else if (Tess.Dim == 2)
    (*pval) = pow ((6 / M_PI) * (*pval), 0.3333333333333333333333333);

  return 0;
}

int
neut_tess_radeq (struct TESS Tess, double *pval)
{
  neut_tess_diameq (Tess, pval);
  (*pval) *= .5;

  return 0;
}

void
neut_tess_cellavdiameq (struct TESS Tess, int CellQty, double *pdiameq)
{
  double size;

  neut_tess_size (Tess, &size);
  size /= CellQty;

  if (Tess.Dim == 3 && Tess.PseudoDim == -1)
    *pdiameq = pow ((6 / M_PI) * size, 1 / 3.);
  else if (Tess.Dim == 2 || (Tess.Dim == 3 && Tess.PseudoDim == 2))
    *pdiameq = sqrt ((4 / M_PI) * size);
  else
    abort ();

  return;
}

void
neut_tess_cellavdiameq_cellqty (struct TESS Tess, double avdiameq,
                                int *pCellQty, double *pfact)
{
  double tesssize, size;

  neut_tess_size (Tess, &tesssize);

  if (Tess.Dim == 3 && Tess.PseudoDim == -1)
    size = pow (avdiameq, 3) * (M_PI / 6);
  else if (Tess.Dim == 2 || (Tess.Dim == 3 && Tess.PseudoDim == 2))
    size = pow (avdiameq, 2) * (M_PI / 4);
  else
    abort ();

  (*pCellQty) = ut_num_d2ri (tesssize / size);
  if (pfact)
    (*pfact) = (tesssize / size) / (*pCellQty);

  return;
}

void
neut_tess_cellavsize_cellqty (struct TESS Tess, double avsize, int *pCellQty,
                              double *pfact)
{
  double tesssize;

  neut_tess_size (Tess, &tesssize);

  (*pCellQty) = ut_num_d2ri (tesssize / avsize);
  if (pfact)
    (*pfact) = (tesssize / avsize) / (*pCellQty);

  return;
}

void
neut_tess_cellavradeq (struct TESS Tess, int CellQty, double *pradeq)
{
  neut_tess_cellavdiameq (Tess, CellQty, pradeq);
  (*pradeq) *= 0.5;

  return;
}

void
neut_tess_cellavsize (struct TESS Tess, int CellQty, double *psize)
{
  neut_tess_size (Tess, psize);
  *psize /= CellQty;

  return;
}

/*
int
neut_tess_face_area2 (struct TESS *Tess, int face, double *parea)
{
  int i, ver;
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;

  if (face < 1 || face > (*Tess).FaceQty)
    ut_print_neperbug ();

  p0 = ut_alloc_1d (3);

  if ((*Tess).FaceState[face] == -1)
  {
    (*parea) = 0;
    return -1;
  }

  if ((*Tess).FaceState[face] > 0)
  {
    if ((*Tess).FacePt[face] == 0)
      ut_array_1d_memcpy ( (*Tess).FacePtCoo[face], 3,p0);
    else if ((*Tess).FacePt[face] > 0)
    {
      ver = (*Tess).FaceVerNb[face][(*Tess).FacePt[face]];
      ut_array_1d_memcpy ( (*Tess).VerCoo[ver], 3,p0);
    }
    else
      neut_tess_face_centre2 (Tess, face, p0);
  }
  else if ((*Tess).FaceState[face] == 0)
  {
    ver = (*Tess).FaceVerNb[face][1];
    ut_array_1d_memcpy ( (*Tess).VerCoo[ver], 3,p0);
  }

  (*parea) = 0;
  for (i = 1; i < (*Tess).FaceVerQty[face]; i++)
  {
    p1 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][i]];
    p2 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][(*Tess).FaceVerQty[face]]];
  p2 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (&p0);

  return 0;
}
*/

int
neut_tess_volume (struct TESS Tess, double *pvol)
{
  int i;
  double tmp;

  // the domain is the faceted hull cut by the primitives
  if (Tess.DomPrimQty > 0)
    return neut_tess_domprim_volume (Tess, pvol);

  (*pvol) = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    neut_tess_poly_volume (Tess, i, &tmp);
    (*pvol) += tmp;
  }

  return 0;
}

// neut_tess_domprim_volume computes the volume of a domain defined as a
// faceted hull cut by a primitive, which must be a sphere (spherei), a
// cylinder (cylinderi) whose ends are hull faces or periodic, or a torus (torus) whose
// inner part bounds the domain between two hull faces normal to its axis
// (hourglass).
int
neut_tess_domprim_volume (struct TESS Tess, double *pvol)
{
  int i;
  double val, min = DBL_MAX, max = -DBL_MAX, R, a, u, F[2];
  struct PRIM Prim;

  if (Tess.DomPrimQty != 1)
    ut_print_neperbug ();

  Prim = Tess.DomPrim[0];

  if (!strcmp (Prim.Type, "spherei"))
    (*pvol) = 4. / 3 * M_PI * pow (Prim.Rad[0], 3);

  else if (!strcmp (Prim.Type, "cylinderi"))
  {
    // periodic along the axis: the cells extend beyond the ends
    for (i = 0; i < 3; i++)
      if (Tess.Type && !strcmp (Tess.Type, "periodic") && Tess.Periodic[i]
          && fabs (fabs (Prim.Dir[i]) - 1) < 1e-6)
      {
        min = 0;
        max = Tess.PeriodicDist[i];
      }

    if (max < min)
      for (i = 1; i <= Tess.VerQty; i++)
      {
        val = ut_array_1d_scalprod (Tess.VerCoo[i], Prim.Dir, 3);
        min = ut_num_min (min, val);
        max = ut_num_max (max, val);
      }

    (*pvol) = M_PI * pow (Prim.Rad[0], 2) * (max - min);
  }

  // integral of pi r(u)^2, with r(u) = R - sqrt (a^2 - u^2) and u the
  // position along the axis, relative to the centre
  else if (!strcmp (Prim.Type, "torus"))
  {
    R = Prim.Rad[0];
    a = Prim.Rad[1];

    for (i = 1; i <= Tess.VerQty; i++)
    {
      val = ut_array_1d_scalprod (Tess.VerCoo[i], Prim.Dir, 3)
          - ut_array_1d_scalprod (Prim.Base, Prim.Dir, 3);
      min = ut_num_min (min, val);
      max = ut_num_max (max, val);
    }

    for (i = 0; i < 2; i++)
    {
      u = (i == 0) ? min : max;
      F[i] = (R * R + a * a) * u - pow (u, 3) / 3
           - R * (u * sqrt (a * a - u * u) + a * a * asin (u / a));
    }

    (*pvol) = M_PI * (F[1] - F[0]);
  }

  else
    ut_print_neperbug ();

  return 0;
}

// neut_tess_point_indomprims returns 1 if a point is on the positive side
// of all domain primitives (see neut_prim_point_side), and 0 otherwise.
int
neut_tess_point_indomprims (struct TESS Tess, double *coo)
{
  int i, side;

  for (i = 0; i < Tess.DomPrimQty; i++)
  {
    neut_prim_point_side (Tess.DomPrim[i], coo, &side);
    if (side == -1)
      return 0;
  }

  return 1;
}

int
neut_tess_poly_volume (struct TESS Tess, int poly, double *pvol)
{
  int i, ver, f, face;
  double area, h;
  double *p1 = NULL;
  double *p2 = NULL;
  double *p0 = NULL;
  double *proj = NULL;
  double *eq = NULL;
  double *centre = NULL;

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (poly < 1 || poly > Tess.PolyQty || Tess.PolyFaceQty[poly] == 0)
  {
    (*pvol) = 0;
    return -1;
  }

  if (Tess.PolyFaceQty[poly] == 0)
  {
    (*pvol) = 0;
    return 1;
  }

  p0 = ut_alloc_1d (3);
  proj = ut_alloc_1d (3);
  eq = ut_alloc_1d (4);
  centre = ut_alloc_1d (3);

  neut_tess_poly_vercentroid (Tess, poly, centre);

  (*pvol) = 0;
  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];

    if (Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
        ut_array_1d_memcpy (Tess.FacePtCoo[face], 3, p0);
      else if (Tess.FacePt[face] == -1)
      {
        ver = Tess.FaceVerNb[face][1];
        neut_tess_face_centre (Tess, face, p0);
      }
      else
      {
        ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
        ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
      }
    }
    else
    {
      ver = Tess.FaceVerNb[face][1];
      ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
    }

    for (i = 1; i < Tess.FaceVerQty[face]; i++)
    {
      p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
      p2 = Tess.VerCoo[Tess.FaceVerNb[face][i + 1]];
      area = ut_space_triangle_area (p0, p1, p2);
      if (!isnan (area))
      {
        ut_array_1d_memcpy (centre, 3, proj);
        ut_space_points_plane (p0, p1, p2, eq);
        ut_space_point_dir_plane_proj (proj, eq + 1, eq, proj);
        h = ut_space_dist (centre, proj);
        if (!isnan (h))
          (*pvol) += area * h * 0.3333333333333333333333;
      }
    }

    p1 = Tess.VerCoo[Tess.FaceVerNb[face][Tess.FaceVerQty[face]]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
    area = ut_space_triangle_area (p0, p1, p2);
    if (!isnan (area))
    {
      ut_array_1d_memcpy (centre, 3, proj);
      ut_space_points_plane (p0, p1, p2, eq);
      ut_space_point_dir_plane_proj (proj, eq + 1, eq, proj);
      h = ut_space_dist (centre, proj);
      if (!isnan (h))
        (*pvol) += area * h * 0.3333333333333333333333;
    }
  }

  // don't free p1, p2 (shortcuts)
  ut_free_1d (&proj);
  ut_free_1d (&eq);
  ut_free_1d (&p0);
  ut_free_1d (&centre);

  return 0;
}

void
neut_tess_poly_aniso (struct TESS Tess, int poly, double **evect, double *eval)
{
  int i, j, k, s, f, ver, face;
  double Vtet;
  double coef, scalprod, V;
  double centre[3];
  double p[4][3];
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;
  double *u = NULL, *v = NULL, *w = NULL, *vect = NULL;
  double *bary = NULL, *M1 = NULL, *mu = NULL;
  double **sum = NULL, **M2 = NULL;
  double **C = NULL;

  p0 = ut_alloc_1d (3);
  u = ut_alloc_1d (3);
  v = ut_alloc_1d (3);
  w = ut_alloc_1d (3);
  vect = ut_alloc_1d (3);
  bary = ut_alloc_1d (3);
  M1 = ut_alloc_1d (3);
  mu = ut_alloc_1d (3);
  sum = ut_alloc_2d (3, 3);
  M2 = ut_alloc_2d (3, 3);
  C = ut_alloc_2d (3, 3);

  neut_tess_poly_centroid (Tess, poly, centre);

  ut_array_1d_zero (mu, 3);
  ut_array_1d_zero (M1, 3);
  ut_array_2d_zero (M2, 3, 3);

  V = 0;
  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];

    if (Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
      {
        ver = -1;
        ut_array_1d_memcpy (Tess.FacePtCoo[face], 3, p0);
      }
      else if (Tess.FacePt[face] == -1)
      {
        ver = Tess.FaceVerNb[face][1];
        neut_tess_face_centre (Tess, face, p0);
      }
      else
      {
        ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
        ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
      }
    }
    else
    {
      ver = Tess.FaceVerNb[face][1];
      ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
    }

    for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    {
      p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
      p2 = Tess.VerCoo[Tess.FaceVerNb[face][ut_array_rotpos (1, Tess.FaceVerQty[face], i, 1)]];

      // 2 vertices are the same, skipping
      if (ver == Tess.FaceVerNb[face][i] || ver == Tess.FaceVerNb[face][ut_array_rotpos (1, Tess.FaceVerQty[face], i, 1)])
        continue;

      ut_array_1d_sub (centre, p0, 3, u);
      ut_array_1d_sub (centre, p1, 3, v);
      ut_array_1d_sub (centre, p2, 3, w);
      ut_vector_vectprod (v, w, vect);
      scalprod = ut_array_1d_scalprod (u, vect, 3);

      Vtet = fabs (scalprod / 6);

      coef = Vtet / 20;

      for (j = 0; j < 3; j++)
      {
        p[0][j] = centre[j];
        p[1][j] = p0[j];
        p[2][j] = p1[j];
        p[3][j] = p2[j];
      }

      ut_array_1d_zero (bary, 3);
      ut_array_2d_zero (sum, 3, 3);
      for (s = 0; s < 4; s++)
      {
        for (j = 0; j < 3; j++)
          bary[j] += p[s][j];

        for (j = 0; j < 3; j++)
          for (k = 0; k < 3; k++)
            sum[j][k] += p[s][j] * p[s][k];
      }

      // First moment of inertia of the tetrahedron
      for (j = 0; j < 3; j++)
        M1[j] += 0.25 * Vtet * bary[j];

      // Second moment of inertia of the tetrahedron
      coef = Vtet / 20;
      for (j = 0; j < 3; j++)
        for (k = 0; k < 3; k++)
          M2[j][k] += coef * (sum[j][k] +  bary[j] * bary[k]);

      V += Vtet;
    }
  }

  for (j = 0; j < 3; j++)
    mu[j] = M1[j] / V;

  for (j = 0; j < 3; j++)
    for (k = 0; k < 3; k++)
      C[j][k] = M2[j][k] / V - mu[j] * mu[k];

  ut_mat_eigen (C, 3, eval, evect);

  // don't free p1, p2 (shortcuts)
  ut_free_1d (&p0);
  ut_free_1d (&u);
  ut_free_1d (&v);
  ut_free_1d (&w);
  ut_free_1d (&vect);
  ut_free_1d (&bary);
  ut_free_1d (&M1);
  ut_free_1d (&mu);
  ut_free_2d (&sum, 3);
  ut_free_2d (&M2, 3);
  ut_free_2d (&C, 3);

  return;
}

void
neut_tess_poly_anisofact (struct TESS Tess, int poly, double *pval)
{
  double eval_max, eval_min;
  double *eval = ut_alloc_1d (3);
  double **evect = ut_alloc_2d (3, 3);

  neut_tess_poly_aniso (Tess, poly, evect, eval);

    //sum_log += log (eval[i]);
    //
  eval_max = ut_array_1d_max (eval, 3);
  eval_min = ut_array_1d_min (eval, 3);

  //tmp = exp (sum_log / 3);
  //*pval = eval_max / tmp;

  (*pval) = sqrt (eval_max / eval_min);

  if (isinf (*pval))
    *pval = -1;

  ut_free_2d (&evect, 3);
  ut_free_1d (&eval);
}

// in-plane coordinates of point x in the frame (o; e1, e2)
static void
neut_tess_face_aniso_proj (double *x, double *o, double *e1, double *e2,
                           double *q)
{
  int j;

  q[0] = 0;
  q[1] = 0;
  for (j = 0; j < 3; j++)
  {
    q[0] += (x[j] - o[j]) * e1[j];
    q[1] += (x[j] - o[j]) * e2[j];
  }

  return;
}

void
neut_tess_face_aniso (struct TESS Tess, int face, double **evect, double *eval)
{
  int i, j, k, s, ver, status, legacy;
  int verqty = Tess.FaceVerQty[face];
  double coef, At, A, norm;
  double **p = NULL;
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;
  double *x0 = NULL, *x1 = NULL, *x2 = NULL;
  double *q0 = NULL, *q1 = NULL, *q2 = NULL;
  double *n = NULL, *e1 = NULL, *e2 = NULL;
  double *ev = NULL, **evec = NULL;
  double *u = NULL, *v = NULL, *uxv = NULL;
  double *bary = NULL, *M1 = NULL, *mu = NULL;
  double **sum = NULL, **M2 = NULL;
  double **C = NULL;

  p = ut_alloc_2d (3, Tess.Dim);
  p0 = ut_alloc_1d (3);         // neut_tess_face_centre writes 3 coordinates
  q0 = ut_alloc_1d (2);
  q1 = ut_alloc_1d (2);
  q2 = ut_alloc_1d (2);
  n = ut_alloc_1d (3);
  e1 = ut_alloc_1d (3);
  e2 = ut_alloc_1d (3);
  ev = ut_alloc_1d (Tess.Dim);  // local eigensystem (eval, evect may be NULL)
  evec = ut_alloc_2d (Tess.Dim, Tess.Dim);
  u = ut_alloc_1d (3);
  v = ut_alloc_1d (3);
  uxv = ut_alloc_1d (3);
  bary = ut_alloc_1d (Tess.Dim);
  M1 = ut_alloc_1d (Tess.Dim);
  mu = ut_alloc_1d (Tess.Dim);
  sum = ut_alloc_2d (Tess.Dim, Tess.Dim);
  M2 = ut_alloc_2d (Tess.Dim, Tess.Dim);
  C = ut_alloc_2d (Tess.Dim, Tess.Dim);

  status = (verqty >= 3) ? 0 : -1;

  if (Tess.FaceState[face] > 0)
  {
    if (Tess.FacePt[face] == 0)
      ut_array_1d_memcpy (Tess.FacePtCoo[face], 3, p0);
    else if (Tess.FacePt[face] > 0)
    {
      ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
      ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
    }
    else if (!status)
      neut_tess_face_centre (Tess, face, p0);
  }
  else if (Tess.FaceState[face] == 0 && !status)
      neut_tess_face_centre (Tess, face, p0);

  // 2D tessellation: the moments are computed in in-plane coordinates.
  // If the vertex z coordinates are exactly constant, the in-plane
  // coordinates are (x, y), as historically.  Otherwise (e.g., slice of
  // a 3D tessellation), the vertices are projected into an orthonormal
  // frame (p0; e1, e2) of the face plane, of normal given by the Newell
  // vector of the ordered face vertices.
  legacy = 1;
  if (!status && Tess.Dim == 2)
  {
    x0 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
    for (i = 2; i <= verqty; i++)
      if (Tess.VerCoo[Tess.FaceVerNb[face][i]][2] != x0[2])
      {
        legacy = 0;
        break;
      }

    if (!legacy)
    {
      for (i = 2; i < verqty; i++)
      {
        ut_array_1d_sub (x0, Tess.VerCoo[Tess.FaceVerNb[face][i]], 3, u);
        ut_array_1d_sub (x0, Tess.VerCoo[Tess.FaceVerNb[face][i + 1]], 3, v);
        ut_vector_vectprod (u, v, uxv);
        ut_array_1d_add (n, uxv, 3, n);
      }

      // u and v are reused below with Tess.Dim (2) components only: their
      // third components must not keep the values set here
      ut_array_1d_zero (u, 3);
      ut_array_1d_zero (v, 3);

      norm = ut_vector_norm (n);
      if (!(norm > 0) || !isfinite (norm))
        status = -1;
      else
      {
        ut_array_1d_scale (n, 3, 1 / norm);

        // e1 = normalized (a x n), a being the axis least aligned with n
        k = 0;
        for (j = 1; j < 3; j++)
          if (fabs (n[j]) < fabs (n[k]))
            k = j;
        ut_array_1d_zero (e2, 3);
        e2[k] = 1;
        ut_vector_vectprod (e2, n, e1);
        ut_array_1d_scale (e1, 3, 1 / ut_vector_norm (e1));
        ut_vector_vectprod (n, e1, e2);
      }
    }

    if (legacy)
      ut_array_1d_memcpy (p0, 2, q0);
    else
      neut_tess_face_aniso_proj (p0, p0, e1, e2, q0);
  }

  A = 0;
  for (i = 1; !status && i <= verqty; i++)
  {
    p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][ut_array_rotpos (1, verqty, i, 1)]];

    if (Tess.Dim == 2)
    {
      if (legacy)
      {
        ut_array_1d_memcpy (p1, 2, q1);
        ut_array_1d_memcpy (p2, 2, q2);
      }
      else
      {
        neut_tess_face_aniso_proj (p1, p0, e1, e2, q1);
        neut_tess_face_aniso_proj (p2, p0, e1, e2, q2);
      }
      x0 = q0;
      x1 = q1;
      x2 = q2;
    }
    else
    {
      x0 = p0;
      x1 = p1;
      x2 = p2;
    }

    ut_array_1d_sub (x0, x1, Tess.Dim, u);
    ut_array_1d_sub (x0, x2, Tess.Dim, v);
    ut_vector_vectprod (u, v, uxv);
    At = ut_vector_norm (uxv) * 0.5;

    for (j = 0; j < Tess.Dim; j++)
    {
      p[0][j] = x0[j];
      p[1][j] = x1[j];
      p[2][j] = x2[j];
    }

    ut_array_1d_zero (bary, Tess.Dim);
    ut_array_2d_zero (sum, Tess.Dim, Tess.Dim);
    for (s = 0; s < 3; s++)
    {
      ut_array_1d_add (bary, p[s], Tess.Dim, bary);

      for (j = 0; j < Tess.Dim; j++)
        for (k = 0; k < Tess.Dim; k++)
          sum[j][k] += p[s][j] * p[s][k];
    }

    // First moment of inertia of the tetrahedron
    for (j = 0; j < Tess.Dim; j++)
      M1[j] += At * bary[j] * 0.3333333333333333333333;

    // Second moment of inertia of the tetrahedron
    coef = At / 12;
    for (j = 0; j < Tess.Dim; j++)
      for (k = 0; k < Tess.Dim; k++)
        M2[j][k] += coef * (sum[j][k] + bary[j] * bary[k]);

    A += At;
  }

  if (!status && (!(A > 0) || !isfinite (A)))
    status = -1;

  if (!status)
  {
    ut_array_1d_memcpy (M1, Tess.Dim, mu);
    ut_array_1d_scale (mu, Tess.Dim, 1 / A);

    for (j = 0; j < Tess.Dim; j++)
      for (k = 0; k < Tess.Dim; k++)
        C[j][k] = M2[j][k] / A - mu[j] * mu[k];

    ut_mat_eigen (C, Tess.Dim, ev, evec);

    ut_mat_sym (C, Tess.Dim, C);

    for (j = 0; j < Tess.Dim; j++)
      if (!isfinite (ev[j]))
        status = -1;
  }

  // invalid face: eigensystem of the zero tensor
  if (status)
  {
    ut_array_1d_zero (ev, Tess.Dim);
    ut_array_2d_zero (evec, Tess.Dim, Tess.Dim);
    for (j = 0; j < Tess.Dim; j++)
      evec[j][j] = 1;
  }

  if (eval)
    ut_array_1d_memcpy (ev, Tess.Dim, eval);
  if (evect)
    ut_array_2d_memcpy (evec, Tess.Dim, Tess.Dim, evect);

  ut_free_2d (&p, 3);
  ut_free_1d (&ev);
  ut_free_2d (&evec, Tess.Dim);
  ut_free_1d (&p0);
  ut_free_1d (&q0);
  ut_free_1d (&q1);
  ut_free_1d (&q2);
  ut_free_1d (&n);
  ut_free_1d (&e1);
  ut_free_1d (&e2);
  ut_free_1d (&u);
  ut_free_1d (&v);
  ut_free_1d (&uxv);
  ut_free_1d (&bary);
  ut_free_1d (&M1);
  ut_free_1d (&mu);
  ut_free_2d (&sum, Tess.Dim);
  ut_free_2d (&M2, Tess.Dim);
  ut_free_2d (&C, Tess.Dim);

  return;
}

void
neut_tess_face_anisofact (struct TESS Tess, int face, double *pval)
{
  double eval_max, eval_min;
  double *eval = ut_alloc_1d (Tess.Dim);
  double **evect = ut_alloc_2d (Tess.Dim, Tess.Dim);

  neut_tess_face_aniso (Tess, face, evect, eval);

    //sum_log += log (eval[i]);
  eval_max = ut_array_1d_max (eval, 2);
  eval_min = ut_array_1d_min (eval, 2);
  //tmp = exp (sum_log / 2);

  // invalid face (degenerate or non-finite eigenvalues): -1
  if (!isfinite (eval[0]) || !isfinite (eval[1]) || !(eval_min > 0))
    (*pval) = -1;
  else
  {
    (*pval) = sqrt (eval_max / eval_min);

    if (!isfinite (*pval))
      *pval = -1;
  }

  ut_free_2d (&evect, Tess.Dim);
  ut_free_1d (&eval);
}

int
neut_tess_area (struct TESS Tess, double *parea)
{
  int i;
  double tmp;

  (*parea) = 0;
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_tess_face_area (Tess, i, &tmp);
    (*parea) += tmp;
  }

  return 0;
}

int
neut_tess_domface_area (struct TESS Tess, int domface, double *parea)
{
  int i;
  double tmp;

  if (domface < 0 || domface > Tess.DomFaceQty)
  {
    printf ("domface = %d < 0 || > Tess.DomFaceQty = %d\n", domface,
            Tess.DomFaceQty);
    ut_print_neperbug ();
  }

  (*parea) = 0;
  for (i = 1; i <= Tess.DomTessFaceQty[domface]; i++)
  {
    neut_tess_face_area (Tess, Tess.DomTessFaceNb[domface][i], &tmp);
    (*parea) += tmp;
  }

  return 0;
}

int
neut_tess_domface_label_area (struct TESS Tess, char *domface, double *parea)
{
  int id;

  neut_tess_domface_label_id (Tess, domface, &id);
  neut_tess_domface_area (Tess, id, parea);

  return 0;
}

int
neut_tess_poly_vercentroid (struct TESS Tess, int poly, double *coo)
{
  int i, verqty;
  int *vers = NULL;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  ut_array_1d_zero (coo, 3);

  for (i = 0; i < verqty; i++)
    ut_array_1d_add (coo, Tess.VerCoo[vers[i]], 3, coo);
  ut_array_1d_scale (coo, 3, 1. / verqty);

  ut_free_1d_int (&vers);

  return 0;
}

int
neut_tess_poly_centroid (struct TESS Tess, int poly, double *coo)
{
  int i, ver, f, face, status;
  double area;
  double *p0 = ut_alloc_1d (3);
  double *p1 = NULL;
  double *p2 = NULL;
  double vol, totvol;
  double *tmp = ut_alloc_1d (3);
  double *verbary = ut_alloc_1d (3);
  int *vers = NULL;
  int verqty;

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (poly < 1 || poly > Tess.PolyQty || Tess.PolyFaceQty[poly] == 0)
    return -1;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  if (verqty > 0)
  {
    for (i = 0; i < verqty; i++)
      ut_array_1d_add (verbary, Tess.VerCoo[vers[i]], 3, verbary);
    ut_array_1d_scale (verbary, 3, 1. / verqty);

    totvol = 0;
    ut_array_1d_set (coo, 3, 0);

    for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
    {
      face = Tess.PolyFaceNb[poly][f];

      if (Tess.FaceState[face] > 0)
      {
        if (Tess.FacePt[face] == 0)
          ut_array_1d_memcpy (Tess.FacePtCoo[face], 3, p0);
        else if (Tess.FacePt[face] == -1)
        {
          ver = Tess.FaceVerNb[face][1];
          neut_tess_face_centre (Tess, face, p0);
        }
        else
        {
          ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
          ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
        }
      }
      else
      {
        ver = Tess.FaceVerNb[face][1];
        ut_array_1d_memcpy (Tess.VerCoo[ver], 3, p0);
      }

      for (i = 1; i <= Tess.FaceVerQty[face]; i++)
      {
        p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
        p2 =
          Tess.VerCoo[Tess.
                      FaceVerNb[face][ut_array_rotpos
                                      (1, Tess.FaceVerQty[face], i, 1)]];

        area = ut_space_triangle_area (p0, p1, p2);

        if (area > 1e-20)
        {
          ut_space_tet_centre (verbary, p0, p1, p2, tmp);
          vol = ut_space_tet_volume (verbary, p0, p1, p2);
          ut_array_1d_scale (tmp, 3, vol);
          ut_array_1d_add (coo, tmp, 3, coo);
          totvol += vol;
        }
      }
    }

    ut_array_1d_scale (coo, 3, 1. / totvol);
    status = 0;
  }
  else
  {
    ut_array_1d_zero (coo, 3);
    status = -1;
  }

  // don't free p1, p2 (shortcuts)
  ut_free_1d (&p0);
  ut_free_1d (&tmp);
  ut_free_1d (&verbary);
  ut_free_1d_int (&vers);

  return status;
}

int
neut_tess_cell_centroid (struct TESS Tess, int cell, double *coo)
{
  int status = 0;

  if (Tess.Dim == 3)
    status = neut_tess_poly_centroid (Tess, cell, coo);
  else if (Tess.Dim == 2)
    neut_tess_face_centre (Tess, cell, coo);
  else
    abort ();

  return status;
}

void
neut_tess_bbox (struct TESS Tess, double **bbox)
{
  int i, j;

  /* Searching the tessellation bounding box */
  bbox[0][0] = bbox[1][0] = bbox[2][0] = DBL_MAX;
  bbox[0][1] = bbox[1][1] = bbox[2][1] = -DBL_MAX;

  if (Tess.DomVerQty == 0)
    for (i = 1; i <= Tess.VerQty; i++)
      for (j = 0; j < 3; j++)
      {
        bbox[j][0] = ut_num_min (bbox[j][0], Tess.VerCoo[i][j]);
        bbox[j][1] = ut_num_max (bbox[j][1], Tess.VerCoo[i][j]);
      }
  else
    for (i = 1; i <= Tess.DomVerQty; i++)
      for (j = 0; j < 3; j++)
      {
        bbox[j][0] = ut_num_min (bbox[j][0], Tess.DomVerCoo[i][j]);
        bbox[j][1] = ut_num_max (bbox[j][1], Tess.DomVerCoo[i][j]);
      }

  return;
}

void
neut_tess_bboxsize (struct TESS Tess, double *bboxsize)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (Tess, bbox);
  for (i = 0; i < 3; i++)
    bboxsize[i] = bbox[i][1] - bbox[i][0];

  ut_free_2d (&bbox, 3);

  return;
}

void
neut_tess_cell_bboxsize (struct TESS Tess, int cell, double *bboxsize)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_cell_bbox (Tess, cell, bbox);
  for (i = 0; i < 3; i++)
    bboxsize[i] = bbox[i][1] - bbox[i][0];

  ut_free_2d (&bbox, 3);

  return;
}

void
neut_tess_vers_bbox (struct TESS Tess, int *vers, int verqty, double **bbox)
{
  int i, j;

  for (i = 0; i < Tess.Dim; i++)
  {
    bbox[i][0] = DBL_MAX;
    bbox[i][1] = -DBL_MAX;
  }

  for (i = 0; i < verqty; i++)
    for (j = 0; j < Tess.Dim; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Tess.VerCoo[vers[i]][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], Tess.VerCoo[vers[i]][j]);
    }

  return;
}

int
neut_tess_cell_bbox (struct TESS Tess, int cell, double **bbox)
{
  if (neut_tess_cell_isvoid (Tess, cell))
  {
    ut_array_2d_zero (bbox, 3, 2);

    return -1;
  }

  else
  {
    int verqty, *vers = NULL;

    neut_tess_cell_vers (Tess, cell, &vers, &verqty);

    neut_tess_vers_bbox (Tess, vers, verqty, bbox);

    ut_free_1d_int (&vers);

    return 0;
  }
}

void
neut_tess_poly_bbox (struct TESS Tess, int poly, double **bbox)
{
  int verqty, *vers = NULL;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  neut_tess_vers_bbox (Tess, vers, verqty, bbox);

  ut_free_1d_int (&vers);

  return;
}

void
neut_tess_face_bbox (struct TESS Tess, int face, double **bbox)
{
  int verqty, *vers = NULL;

  neut_tess_face_vers (Tess, face, &vers, &verqty);

  neut_tess_vers_bbox (Tess, vers, verqty, bbox);

  ut_free_1d_int (&vers);

  return;
}

void
neut_tess_edge_bbox (struct TESS Tess, int edge, double **bbox)
{
  int verqty, *vers = NULL;

  neut_tess_edge_vers (Tess, edge, &vers, &verqty);

  neut_tess_vers_bbox (Tess, vers, verqty, bbox);

  ut_free_1d_int (&vers);

  return;
}

void
neut_tess_face_centre_x (struct TESS Tess, int face, double *px)
{
  double *centre = ut_alloc_1d (3);

  neut_tess_face_centre (Tess, face, centre);
  (*px) = centre[0];

  ut_free_1d (&centre);

  return;
}

void
neut_tess_face_centre_y (struct TESS Tess, int face, double *px)
{
  double *centre = ut_alloc_1d (3);

  neut_tess_face_centre (Tess, face, centre);
  (*px) = centre[1];

  ut_free_1d (&centre);

  return;
}

void
neut_tess_face_centre_z (struct TESS Tess, int face, double *px)
{
  double *centre = ut_alloc_1d (3);

  neut_tess_face_centre (Tess, face, centre);
  (*px) = centre[2];

  ut_free_1d (&centre);

  return;
}

void
neut_tess_face_centre (struct TESS Tess, int face, double *centre)
{
  int i, ver, ver1, ver2, pos;
  double *coo = NULL;
  double *tmp = NULL;
  double val, totval;

  if (Tess.FaceVerQty[face] == 0)
  {
    ut_array_1d_zero (centre, 3);

    return;
  }

  coo = ut_alloc_1d (3);
  tmp = ut_alloc_1d (3);

  pos = (Tess.Dim == 3 && Tess.FacePt[face] > 0) ? Tess.FacePt[face] : 1;
  ver = Tess.FaceVerNb[face][pos];
  ut_array_1d_memcpy (Tess.VerCoo[ver], 3, coo);

  ut_array_1d_set (centre, 3, 0);
  totval = 0;

  if (Tess.FaceVerQty[face] > 0)
  {
    for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    {
      ver1 = Tess.FaceVerNb[face][i];
      ver2 =
        Tess.
        FaceVerNb[face][ut_array_rotpos (1, Tess.FaceVerQty[face], i, 1)];
      ut_space_triangle_centre (Tess.VerCoo[ver1], Tess.VerCoo[ver2], coo,
                                tmp);
      val =
        ut_space_triangle_area (Tess.VerCoo[ver1], Tess.VerCoo[ver2], coo);

      ut_array_1d_scale (tmp, 3, val);
      ut_array_1d_add (centre, tmp, 3, centre);
      totval += val;
    }
    ut_array_1d_scale (centre, 3, 1 / totval);
  }

  ut_free_1d (&coo);
  ut_free_1d (&tmp);

  return;
}

int
neut_tess_edge_centre_x (struct TESS Tess, int edge, double *px)
{
  (*px) =
    (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][0] +
     Tess.VerCoo[Tess.EdgeVerNb[edge][1]][0]) / 2.;

  return 0;
}

int
neut_tess_edge_centre_y (struct TESS Tess, int edge, double *py)
{
  (*py) =
    (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][1] +
     Tess.VerCoo[Tess.EdgeVerNb[edge][1]][1]) / 2.;

  return 0;
}

int
neut_tess_edge_centre_z (struct TESS Tess, int edge, double *pz)
{
  (*pz) =
    (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][2] +
     Tess.VerCoo[Tess.EdgeVerNb[edge][1]][2]) / 2.;

  return 0;
}

int
neut_tess_edge_centre (struct TESS Tess, int edge, double *pos)
{
  int i;

  for (i = 0; i < 3; i++)
    pos[i] =
      (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][i] +
       Tess.VerCoo[Tess.EdgeVerNb[edge][1]][i]) / 2.;

  return 0;
}

void
neut_tess_centre (struct TESS Tess, double *centre)
{
  int i;
  double totval, val;
  double *coo = ut_alloc_1d (3);

  ut_array_1d_zero (centre, 3);

  if (Tess.Dim == 3)
  {
    totval = 0;
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      neut_tess_poly_centroid (Tess, i, coo);
      neut_tess_poly_volume (Tess, i, &val);
      ut_array_1d_scale (coo, 3, val);
      ut_array_1d_add (centre, coo, 3, centre);
      totval += val;
    }
    ut_array_1d_scale (centre, 3, 1. / totval);
  }
  else if (Tess.Dim == 2)
  {
    totval = 0;
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      neut_tess_face_centre (Tess, i, coo);
      neut_tess_face_area (Tess, i, &val);
      ut_array_1d_scale (coo, 3, val);
      ut_array_1d_add (centre, coo, 3, centre);
      totval += val;
    }
    ut_array_1d_scale (centre, 3, 1. / totval);
  }

  ut_free_1d (&coo);

  return;
}

void
neut_tess_bboxcentre (struct TESS Tess, double *centre)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (Tess, bbox);

  for (i = 0; i < Tess.Dim; i++)
    centre[i] = ut_array_1d_mean (bbox[i], 2);

  ut_free_2d (&bbox, 3);

  return;
}

int
neut_tess_point_inpoly_std (struct TESS Tess, double *coo, int poly)
{
  int i, j, face, domface, percooqty, status, status2, intorus, side;
  double **percoos = NULL;

  neut_tess_coo_percoos (Tess, coo, &percoos, &percooqty);

  status = 0;
  for (i = 0; i < percooqty; i++)
  {
    status2 = 1;

    for (j = 1; j <= Tess.PolyFaceQty[poly]; j++)
    {
      face = Tess.PolyFaceNb[poly][j];
      domface = Tess.FaceDom[face][1];

      if (Tess.FaceDom[face][0] != 2
      || (Tess.FaceDom[face][0] == 2 && !strcmp (Tess.DomFaceType[domface], "plane")))
      {
        if (Tess.PolyFaceOri[poly][j] *
            ut_space_point_plane_side_tol (percoos[i] - 1,
                                           Tess.FaceEq[Tess.PolyFaceNb[poly][j]],
                                           1e-12) > 0)
        {
          status2 = 0;
          break;
        }
      }

      else if (!strcmp (Tess.DomFaceType[domface], "torus"))
      {
        intorus = ut_space_point_intorus (percoos[i], Tess.DomFaceParms[domface],
                                                      Tess.DomFaceParms[domface] + 3,
                                                      Tess.DomFaceParms[domface][6],
                                                      Tess.DomFaceParms[domface][7]);
        if (intorus)
        {
          status2 = 0;
          break;
        }
      }

      else
      {
        neut_primparms_point_side (Tess.DomFaceType[domface],
                                   Tess.DomFaceParms[domface], percoos[i],
                                   &side);
        if (side == -1)
        {
          status2 = 0;
          break;
        }
      }
    }

    if (status2)
    {
      status = 1;
      break;
    }
  }

  ut_free_2d (&percoos, percooqty);

  return status;
}

int
neut_tess_point_inpoly_reg (struct TESS Tess, double *coo, int poly)
{
  int i, elt, status, percooqty;
  double **percoos = NULL;
  struct MESH Mesh;
  struct NODES Nodes;

  neut_mesh_set_zero (&Mesh);
  neut_nodes_set_zero (&Nodes);

  neut_tess_poly_mesh (Tess, poly, &Nodes, &Mesh);

  neut_tess_coo_percoos (Tess, coo, &percoos, &percooqty);

  status = 0;
  for (i = 0; i < percooqty; i++)
    if (neut_mesh_elset_point_elt (Nodes, Mesh, 1, percoos[i], &elt) == 0)
    {
      status = 1;
      break;
    }

  ut_free_2d (&percoos, percooqty);
  neut_mesh_free (&Mesh);
  neut_nodes_free (&Nodes);

  return status;
}

int
neut_tess_point_in (struct TESS Tess, double *coo)
{
  int i;

  for (i = 1; i <= Tess.CellQty; i++)
    if (neut_tess_point_incell (Tess, coo, i))
      return 1;

  return 0;
}

int
neut_tess_point_incell (struct TESS Tess, double *coo, int nb)
{
  if (Tess.Dim == 3)
    return neut_tess_point_inpoly (Tess, coo, nb);
  if (Tess.Dim == 2)
    return neut_tess_point_inface (Tess, coo, nb);
  else
    ut_print_neperbug ();

  return 0;
}

int
neut_tess_point_inpoly (struct TESS Tess, double *coo, int nb)
{
  if (Tess.Dim != 3)
    ut_print_neperbug ();

  if (Tess.PolyFaceQty[nb] == 0)
    return 0;
  else if (neut_tess_poly_std (Tess, nb) == 1)
    return neut_tess_point_inpoly_std (Tess, coo, nb);
  else
    return neut_tess_point_inpoly_reg (Tess, coo, nb);
}

int
neut_tess_point_inface (struct TESS Tess, double *coo, int face)
{
  int i, j, ver1, ver2, status;
  double *centre = ut_alloc_1d (3);
  int *per = ut_alloc_1d_int (3);
  double *coob = ut_alloc_1d (3);
  int *Periodic = ut_alloc_1d_int (3);
  double *PeriodicDist = ut_alloc_1d (3);

  if (Tess.Periodic)
    ut_array_1d_int_memcpy (Tess.Periodic, 3, Periodic);
  if (Tess.PeriodicDist)
    ut_array_1d_memcpy (Tess.PeriodicDist, 3, PeriodicDist);

  neut_tess_face_centre (Tess, face, centre);

  status = 0;
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver1 = Tess.FaceVerNb[face][i];
    ver2 =
      Tess.FaceVerNb[face][ut_array_rotpos (1, Tess.FaceVerQty[face], i, 1)];

    for (per[0] = -Periodic[0]; per[0] <= Periodic[0]; per[0]++)
    {
      for (per[1] = -Periodic[1]; per[1] <= Periodic[1]; per[1]++)
      {
        for (j = 0; j < Tess.Dim; j++)
          coob[j] = coo[j] + per[j] * PeriodicDist[j];

        if (ut_space_triangle_point_in
            (centre, Tess.VerCoo[ver1], Tess.VerCoo[ver2], coob, 1e-6, 1e-6, NULL))
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

  ut_free_1d (&centre);
  ut_free_1d (&coob);
  ut_free_1d_int (&per);
  ut_free_1d_int (&Periodic);
  ut_free_1d (&PeriodicDist);

  return status;
}

int
neut_tess_point_inedge (struct TESS Tess, double *coo, int edge)
{
  int ver1, ver2;

  ver1 = Tess.EdgeVerNb[edge][0];
  ver2 = Tess.EdgeVerNb[edge][1];

  return ut_space_segment_point_in (Tess.VerCoo[ver1], Tess.VerCoo[ver2],
                                    coo);
}

/* rsel2sel sets sel from rsel */
void
rsel2sel (double rsel, int dim, double vol, int CellQty, double *psel)
{
  if (dim == 3)
    (*psel) =
      0.5 * rsel * 0.5 * pow (vol / CellQty, 0.3333333333333333333333333333);
  else if (dim == 2)
  {
    (*psel) = 0.5 * rsel * 0.5 * pow (vol / CellQty, 0.5);
    (*psel) *= 0.5;
  }

  return;
}

/* rcl2cl sets cl from rcl */
void
rcl2cl (double rcl, int dim, double size, int CellQty, char *elttype,
        double *pl)
{
  if (dim == 3)
  {
    if (elttype == NULL || !strcmp (elttype, "tri"))
      (*pl) =
        rcl * 0.5 * pow (size / CellQty, 0.3333333333333333333333333333);
    else if (!strcmp (elttype, "quad"))
    {
      (*pl) =
        rcl * 0.5 * pow (size / CellQty, 0.3333333333333333333333333333);
      (*pl) *= .43084877208099956915;
    }
  }
  else if (dim == 2)
  {
    if (elttype == NULL || !strcmp (elttype, "tri"))
    {
      (*pl) = rcl * 0.5 * pow (size / CellQty, 0.5);
      (*pl) *= 0.5;
    }
    else if (!strncmp (elttype, "quad", 4))
    {
      (*pl) =
        rcl * 0.5 * pow (size / CellQty, 0.3333333333333333333333333333);
      (*pl) *= .43084877208099956915;
    }
    else
      ut_print_neperbug ();
  }
  else if (dim == 1)
  {
    (*pl) = rcl * 0.2 * size / CellQty;
  }

  return;
}

void
neut_tess_face_normal_fromver (struct TESS Tess, int face, double *n)
{
  int i, cooqty;
  double **coo = NULL;
  double *bary = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);

  cooqty = Tess.FaceVerQty[face];
  coo = ut_alloc_2d (cooqty, 3);

  for (i = 0; i < Tess.FaceVerQty[face]; i++)
  {
    ut_array_1d_memcpy (Tess.VerCoo[Tess.FaceVerNb[face][i + 1]], 3, coo[i]);
    ut_array_1d_add (bary, coo[i], 3, bary);
  }
  ut_array_1d_scale (bary, 3, 1. / cooqty);

  ut_array_1d_zero (n, 3);
  for (i = 0; i < Tess.FaceVerQty[face]; i++)
  {
    ut_space_triangle_normal (bary, coo[i],
                              coo[ut_array_rotpos
                                  (0, Tess.FaceVerQty[face] - 1, i, 1)], n0);
    ut_array_1d_add (n, n0, 3, n);
  }
  ut_array_1d_scale (n, 3, 1. / ut_array_1d_norm (n, 3));

  ut_free_2d (&coo, cooqty);
  ut_free_1d (&bary);
  ut_free_1d (&n0);

  return;
}

void
neut_tess_face_dihangle (struct TESS Tess, int face, int id, double *pangle)
{
  int id_prev, id_next;
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);

  id_prev = ut_array_rotpos (1, Tess.FaceVerQty[face], id, -1);
  id_next = ut_array_rotpos (1, Tess.FaceVerQty[face], id, 1);

  ut_array_1d_sub (Tess.VerCoo[Tess.FaceVerNb[face][id_prev]],
                   Tess.VerCoo[Tess.FaceVerNb[face][id]], 3, v1);
  ut_array_1d_sub (Tess.VerCoo[Tess.FaceVerNb[face][id_next]],
                   Tess.VerCoo[Tess.FaceVerNb[face][id]], 3, v2);

  (*pangle) = ut_vector_angle (v1, v2);

  ut_free_1d (&v1);
  ut_free_1d (&v2);

  return;
}

void
neut_tess_face_dihangles (struct TESS Tess, int face, double **pangle,
                          int *pqty)
{
  int i;

  (*pangle) = ut_alloc_1d (Tess.FaceVerQty[face]);
  (*pqty) = Tess.FaceVerQty[face];

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    neut_tess_face_dihangle (Tess, face, i, &((*pangle)[i - 1]));

  return;
}

void
neut_tess_poly_dihangles (struct TESS Tess, int poly, double **pangle,
                          int *pqty)
{
  int *edges = NULL;
  int i, j, edgeqty, ori;
  int *faces = NULL;
  double **n = ut_alloc_2d (2, 3);

  neut_tess_poly_edges (Tess, poly, &edges, &edgeqty);

  (*pangle) = ut_alloc_1d (edgeqty);
  (*pqty) = edgeqty;

  for (i = 0; i < edgeqty; i++)
  {
    neut_tess_poly_edge_faces (Tess, poly, edges[i], &faces);

    for (j = 0; j < 2; j++)
    {
      ut_array_1d_memcpy (Tess.FaceEq[faces[j]] + 1, 3, n[j]);
      neut_tess_poly_face_ori (Tess, poly, faces[j], &ori);
      ut_array_1d_scale (n[j], 3, ori);
    }

    (*pangle)[i] = 180 - ut_vector_angle (n[0], n[1]);

    ut_free_1d_int (&faces);
  }

  ut_free_1d_int (&faces);
  ut_free_2d (&n, 2);

  return;
}

void
neut_tess_face_radeq (struct TESS Tess, int face, double *pval)
{
  neut_tess_face_diameq (Tess, face, pval);
  (*pval) *= .5;

  return;
}

void
neut_tess_face_diameq (struct TESS Tess, int face, double *pval)
{
  neut_tess_face_area (Tess, face, pval);
  (*pval) = sqrt ((4 / M_PI) * (*pval));

  return;
}

void
neut_tess_poly_radeq (struct TESS Tess, int poly, double *pval)
{
  neut_tess_poly_diameq (Tess, poly, pval);
  (*pval) *= .5;

  return;
}

void
neut_tess_poly_diameq (struct TESS Tess, int poly, double *pval)
{
  neut_tess_poly_volume (Tess, poly, pval);
  (*pval) = pow ((6 / M_PI) * (*pval), 0.3333333333333333333333333);

  return;
}

void
neut_tess_cell_radeq (struct TESS Tess, int cell, double *pval)
{
  neut_tess_cell_diameq (Tess, cell, pval);
  (*pval) *= .5;

  return;
}

void
neut_tess_cell_diameq (struct TESS Tess, int cell, double *pval)
{
  if (Tess.Dim == 2)
    neut_tess_face_diameq (Tess, cell, pval);

  else if (Tess.Dim == 3)
    neut_tess_poly_diameq (Tess, cell, pval);

  else
    abort ();

  return;
}

void
neut_tess_face_perimeter (struct TESS Tess, int face, double *pval)
{
  int i, edge;

  (*pval) = 0;
  for (i = 1; i <= Tess.FaceEdgeQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];
    (*pval) += Tess.EdgeLength[edge];

  }

  return;
}

void
neut_tess_poly_area (struct TESS Tess, int poly, double *pval)
{
  int i, face;
  double tmp;

  (*pval) = 0;
  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];
    neut_tess_face_area (Tess, face, &tmp);
    (*pval) += tmp;
  }

  return;
}

int
neut_tess_face_circularity (struct TESS Tess, int face, double *pval)
{
  double diameq, tmp;

  if (Tess.FaceVerQty[face] == 0)
  {
    (*pval) = 0;
    return -1;
  }

  neut_tess_face_diameq (Tess, face, &diameq);
  neut_tess_face_perimeter (Tess, face, &tmp);
  (*pval) = M_PI * diameq / tmp;

  return 0;
}

int
neut_tess_poly_sphericity (struct TESS Tess, int poly, double *pval)
{
  double diameq, tmp;

  if (Tess.PolyFaceQty[poly] == 0)
  {
    (*pval) = 0;
    return -1;
  }

  neut_tess_poly_diameq (Tess, poly, &diameq);
  neut_tess_poly_area (Tess, poly, &tmp);
  (*pval) = M_PI * diameq * diameq / tmp;

  return 0;
}

int
neut_tess_cell_sphericity (struct TESS Tess, int cell, double *pval)
{
  int status;

  if (Tess.Dim == 2)
    status = neut_tess_face_circularity (Tess, cell, pval);
  else if (Tess.Dim == 3)
    status = neut_tess_poly_sphericity (Tess, cell, pval);
  else
    abort ();

  return status;
}

int
neut_tess_cell_planeside (struct TESS Tess, int cell, double *plane,
                          int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_point_plane_side (Tess.VerCoo[vers[i]] - 1, plane);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (&vers);
  ut_free_1d_int (&side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_sphereside (struct TESS Tess, int cell, double *C, double rad,
                           int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_point_sphere_side (Tess.VerCoo[vers[i]], C, rad);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  (*pside) = (min == max) ? min : 0;

  ut_free_1d_int (&vers);
  ut_free_1d_int (&side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_cubeside (struct TESS Tess, int cell, double *parms, int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_point_cube_side (Tess.VerCoo[vers[i]], parms, NULL);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  (*pside) = (min == max) ? min : 0;

  ut_free_1d_int (&vers);
  ut_free_1d_int (&side);

  return 0;
}

int
neut_tess_cell_primside (struct TESS Tess, int cell, struct PRIM Prim,
                         int *pside)
{
  if (!strcmp (Prim.Type, "hspace"))
    return -neut_tess_cell_planeside (Tess, cell, Prim.Eq, pside);
  else if (!strcmp (Prim.Type, "hspacei"))
    return neut_tess_cell_planeside (Tess, cell, Prim.Eq, pside);
  else if (!strcmp (Prim.Type, "sphere"))
    return neut_tess_cell_sphereside (Tess, cell, Prim.Base, Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "spherei"))
    return -neut_tess_cell_sphereside (Tess, cell, Prim.Base, Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "cube"))
    return neut_tess_cell_cubeside (Tess, cell, Prim.Parms, pside);
  else if (!strcmp (Prim.Type, "cubei"))
    return -neut_tess_cell_cubeside (Tess, cell, Prim.Parms, pside);
  else if (!strcmp (Prim.Type, "cylinder"))
    return neut_tess_cell_cylside (Tess, cell, Prim.Base, Prim.Dir,
                                   Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "cylinderi"))
    return -neut_tess_cell_cylside (Tess, cell, Prim.Base, Prim.Dir,
                                    Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "ecylinder"))
    return neut_tess_cell_cyl2side (Tess, cell, Prim.Base, Prim.Dir,
                                    Prim.Dir + 3, Prim.Dir + 6, Prim.Rad[0],
                                    Prim.Rad[1], pside);
  else if (!strcmp (Prim.Type, "ecylinderi"))
    return -neut_tess_cell_cyl2side (Tess, cell, Prim.Base, Prim.Dir,
                                     Prim.Dir + 3, Prim.Dir + 6, Prim.Rad[0],
                                     Prim.Rad[1], pside);
  else if (!strcmp (Prim.Type, "torus"))
    return neut_tess_cell_torusside (Tess, cell, Prim.Base, Prim.Dir,
                                     Prim.Rad[0], Prim.Rad[1], pside);
  else if (!strcmp (Prim.Type, "torusi"))
    return -neut_tess_cell_torusside (Tess, cell, Prim.Base, Prim.Dir,
                                      Prim.Rad[0], Prim.Rad[1], pside);
  else
    abort ();
}

void
neut_tess_face_convexhull (struct TESS Tess, int face, struct NODES *pN,
                           struct MESH *pM)
{
  int i, j, verqty, *vers = NULL;
  double eps = 1e-9;
  double **vercoos = NULL;
  double distmin, dist;
  int distminid;
  int *pt = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  srand48 (1);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  neut_tess_face_vers (Tess, face, &vers, &verqty);

  vercoos = ut_alloc_2d (verqty, 3);
  for (i = 0; i < verqty; i++)
  {
    ut_array_1d_memcpy (Tess.VerCoo[vers[i]], 3, vercoos[i]);
    if (Tess.VerCoo[vers[i]][2] != 0)
    {
      printf ("FIXME (works only if the face is normal to z)\n");
      abort ();
    }
  }

  for (i = 0; i < verqty; i++)
    for (j = 0; j < Tess.Dim; j++)
      vercoos[i][j] += eps * drand48 ();

  net_pts_convexhull (vercoos, verqty, 2, pN, pM);

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
      ut_array_1d_memcpy (Tess.VerCoo[vers[i]], 3, (*pN).NodeCoo[distminid]);
  }

  ut_free_1d (&coo);
  ut_free_1d_int (&pt);
  ut_free_1d_int (&vers);
  ut_free_2d (&vercoos, verqty);

  return;
}

void
neut_tess_poly_convexhull (struct TESS Tess, int poly, struct NODES *pN,
                           struct MESH *pM)
{
  int i, j, verqty, *vers = NULL;
  double eps = 1e-9;
  double **vercoos = NULL;
  double distmin, dist;
  int distminid;
  int *pt = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  srand48 (1);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  vercoos = ut_alloc_2d (verqty, 3);
  for (i = 0; i < verqty; i++)
    ut_array_1d_memcpy (Tess.VerCoo[vers[i]], 3, vercoos[i]);

  for (i = 0; i < verqty; i++)
    for (j = 0; j < Tess.Dim; j++)
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
      ut_array_1d_memcpy (Tess.VerCoo[vers[i]], 3, (*pN).NodeCoo[distminid]);
  }

  ut_free_1d (&coo);
  ut_free_1d_int (&pt);

  ut_free_1d_int (&vers);
  ut_free_2d (&vercoos, verqty);

  return;
}

void
neut_tess_poly_convexity (struct TESS Tess, int poly, double *pval)
{
  double vol, vol2;
  struct NODES N;
  struct MESH M;
  struct MESH M3;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (&M3);

  neut_tess_poly_volume (Tess, poly, &vol2);

  neut_tess_poly_convexhull (Tess, poly, &N, &M);
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
neut_tess_face_convexity (struct TESS Tess, int face, double *pval)
{
  double area, area2;
  struct NODES N;
  struct MESH M;
  struct MESH M2;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (&M2);

  neut_tess_face_area (Tess, face, &area2);

  neut_tess_face_convexhull (Tess, face, &N, &M);
  neut_mesh1d_mesh2d (&N, M, &M2);
  neut_mesh_area (N, M2, &area);
  (*pval) = area2 / area;

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  neut_mesh_free (&M2);

  return;
}

void
neut_tess_cell_convexity (struct TESS Tess, int cell, double *pval)
{
  if (Tess.Dim == 2)
    neut_tess_face_convexity (Tess, cell, pval);
  else if (Tess.Dim == 3)
    neut_tess_poly_convexity (Tess, cell, pval);
  else
    abort ();

  return;
}

void
neut_tess_edgepair_angle (struct TESS Tess, int edge1, int edge2,
                          double *pangle)
{
  int comver;
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);

  neut_tess_edge_dir (Tess, edge1, v1);
  neut_tess_edge_dir (Tess, edge2, v2);

  *pangle = ut_vector_angle (v1, v2);

  if (!neut_tess_edgepair_comver (Tess, edge1, edge2, &comver))
  {
    if (ut_array_1d_int_eltpos (Tess.EdgeVerNb[edge1], 2, comver) ==
        ut_array_1d_int_eltpos (Tess.EdgeVerNb[edge2], 2, comver))
      (*pangle) -= 180;
  }

  ut_free_1d (&v1);
  ut_free_1d (&v2);

  return;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_cylside (struct TESS Tess, int cell, double *C, double *v,
                        double rad, int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_point_cyl_side (Tess.VerCoo[vers[i]], C, v, rad);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (&vers);
  ut_free_1d_int (&side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_cyl2side (struct TESS Tess, int cell, double *C, double *v,
                         double *ell1, double *ell2, double rad1, double rad2,
                         int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] =
      ut_space_point_ecyl_side (Tess.VerCoo[vers[i]], C, v, ell1, ell2, rad1,
                                rad2);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (&vers);
  ut_free_1d_int (&side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_torusside (struct TESS Tess, int cell, double *C, double *v,
                          double rad, double rad2, int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;
  double *P = NULL;
  double *K = ut_alloc_1d (3);
  double dist;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
  {
    P = Tess.VerCoo[vers[i]];
    ut_space_point_circle_dist (P, C, v, rad, &dist, K);
    side[i] = (dist < rad2) ? -1 : 1;
  }

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (&vers);
  ut_free_1d_int (&side);
  ut_free_1d (&K);

  return 0;
}

void
neut_tess_face_eq_fromvers (struct TESS Tess, int face, double *eq)
{
  int verqty;
  double **vercoos = NULL;

  neut_tess_face_vercoos (Tess, face, &vercoos, &verqty);

  // returns an equation, but only based on disordered vertices
  ut_space_pointset_plane (vercoos, verqty, eq);
  // fixes to match the order of the vertices
  neut_tess_face_fixorifromedges (Tess, face, eq);

  ut_free_2d (&vercoos, verqty);

  return;
}

// Computing the equation of a face: the tangent plane at the projection
// of its centroid for a face on a curved domain face, and the plane
// fitted to its vertices otherwise, oriented from the vertex order
void
neut_tess_face_eq (struct TESS Tess, int face, double *eq)
{
  int domface = Tess.FaceDom[face][1];
  double *coo = NULL;

  if (Tess.FaceDom[face][0] == 2
      && strncmp (Tess.DomFaceType[domface], "plane", 5))
  {
    coo = ut_alloc_1d (3);

    neut_tess_face_centre (Tess, face, coo);
    neut_primparms_point_proj (Tess.DomFaceType[domface],
                               Tess.DomFaceParms[domface], coo, coo);
    neut_primparms_point_tangentplane (Tess.DomFaceType[domface],
                                       Tess.DomFaceParms[domface], coo, eq);
    neut_tess_face_fixorifromedges (Tess, face, eq);

    ut_free_1d (&coo);
  }
  else
    neut_tess_face_eq_fromvers (Tess, face, eq);

  return;
}

void
neut_tess_faceinter_fromdomface (struct TESS *pTess, int face)
{
  int domface = (*pTess).FaceDom[face][1];

  if (domface <= 0)
    abort ();

  neut_tess_face_centre (*pTess, face, (*pTess).FacePtCoo[face]);

  neut_primparms_point_proj ((*pTess).DomFaceType[domface],
                             (*pTess).DomFaceParms[domface],
                             (*pTess).FacePtCoo[face],
                             (*pTess).FacePtCoo[face]);

  neut_primparms_point_tangentplane ((*pTess).DomFaceType[domface],
                                     (*pTess).DomFaceParms[domface],
                                     (*pTess).FacePtCoo[face],
                                     (*pTess).FaceEq[face]);
  double *p1 = ut_alloc_1d (3);
  double *p2 = ut_alloc_1d (3);
  double *p3 = ut_alloc_1d (3);
  double *eq = ut_alloc_1d (4);

  ut_array_1d_memcpy ((*pTess).VerCoo[(*pTess).FaceVerNb[face][1]], 3, p1);
  ut_array_1d_memcpy ((*pTess).VerCoo[(*pTess).FaceVerNb[face][2]], 3, p2);
  ut_array_1d_memcpy ((*pTess).VerCoo[(*pTess).FaceVerNb[face][3]], 3, p3);
  ut_space_points_plane (p1, p2, p3, eq);

  if (ut_vector_scalprod (eq + 1, (*pTess).FaceEq[face] + 1) > 0)
    ut_array_1d_memcpy (eq, 4, (*pTess).FaceEq[face]);
  else
  {
    ut_array_1d_memcpy (eq, 4, (*pTess).FaceEq[face]);
    ut_array_1d_scale ((*pTess).FaceEq[face], 4, -1);
  }

  ut_free_1d (&p1);
  ut_free_1d (&p2);
  ut_free_1d (&p3);
  ut_free_1d (&eq);

  (*pTess).FaceState[face] = 0;
  (*pTess).FacePt[face] = 0;

  return;
}

// neut_tess_domfaces_point_proj projects a point onto the intersection of
// domain faces, by Newton iterations: at each iteration, the point is
// projected onto each domain face and moved to the closest point of the
// intersection of the tangent planes (at the projections).  Only the planes
// whose normals are independent are considered (at most 3), taken by
// decreasing distance, so that a face that is the same as another one is
// skipped and that tangent faces are considered in turn (as with
// alternating projections).  Returns 0 on success (distance to all faces
// lower than 1e-6) and -1 otherwise.
int
neut_tess_domfaces_point_proj (struct TESS Tess, int *domfaces,
                               int domfaceqty, double *point, double *proj)
{
  int i, j, k, iter, domface, qty, *id = ut_alloc_1d_int (domfaceqty);
  double maxdist = DBL_MAX, norm;
  double *dist = ut_alloc_1d (domfaceqty), *tmp = ut_alloc_1d (3);
  double **plane = ut_alloc_2d (domfaceqty, 4);
  double **n = ut_alloc_2d (3, 3);       // orthonormalized kept normals
  double **G = ut_alloc_2d (3, 3);
  double *B = ut_alloc_1d (3), *lambda = ut_alloc_1d (3);
  int *kept = ut_alloc_1d_int (3);

  ut_array_1d_memcpy (point, 3, proj);

  for (iter = 0; iter < 1000; iter++)
  {
    maxdist = 0;
    for (i = 0; i < domfaceqty; i++)
    {
      domface = domfaces[i];
      dist[i] = neut_primparms_point_proj (Tess.DomFaceType[domface],
                                           Tess.DomFaceParms[domface], proj,
                                           tmp);
      neut_primparms_point_tangentplane (Tess.DomFaceType[domface],
                                         Tess.DomFaceParms[domface], tmp,
                                         plane[i]);
      ut_array_1d_normalize (plane[i] + 1, 3);
      plane[i][0] = ut_array_1d_scalprod (plane[i] + 1, tmp, 3);
      maxdist = ut_num_max (maxdist, dist[i]);
    }

    if (maxdist < 1e-14 * (1 + ut_array_1d_norm (proj, 3)))
      break;

    // keeping the planes of independent normals, by decreasing distance
    ut_array_1d_sort_index (dist, domfaceqty, id);
    ut_array_1d_int_reverseelts (id, domfaceqty);

    qty = 0;
    for (i = 0; i < domfaceqty && qty < 3; i++)
    {
      ut_array_1d_memcpy (plane[id[i]] + 1, 3, n[qty]);
      for (j = 0; j < qty; j++)
        for (k = 0; k < 3; k++)
          n[qty][k] -= ut_array_1d_scalprod (plane[id[i]] + 1, n[j], 3)
                     * n[j][k];

      norm = ut_array_1d_norm (n[qty], 3);
      if (norm > 1e-3)
      {
        ut_array_1d_scale (n[qty], 3, 1. / norm);
        kept[qty++] = id[i];
      }
    }

    // proj + sum_j lambda_j n_j verifies n_i . x = d_i for the kept planes
    for (i = 0; i < qty; i++)
    {
      for (j = 0; j < qty; j++)
        G[i][j] = ut_array_1d_scalprod (plane[kept[i]] + 1,
                                        plane[kept[j]] + 1, 3);
      B[i] = plane[kept[i]][0]
           - ut_array_1d_scalprod (plane[kept[i]] + 1, proj, 3);
    }

    if (ut_linalg_solve_LU (G, B, qty, lambda))
      ut_print_neperbug ();

    ut_array_1d_zero (tmp, 3);
    for (j = 0; j < qty; j++)
      for (k = 0; k < 3; k++)
        tmp[k] += lambda[j] * plane[kept[j]][k + 1];
    ut_array_1d_add (proj, tmp, 3, proj);

    // the next projections would not change the point significantly
    if (ut_array_1d_norm (tmp, 3) < 1e-14 * (1 + ut_array_1d_norm (proj, 3))
        && qty == domfaceqty)
      break;
  }

  ut_free_1d_int (&id);
  ut_free_1d (&dist);
  ut_free_1d (&tmp);
  ut_free_2d (&plane, domfaceqty);
  ut_free_2d (&n, 3);
  ut_free_2d (&G, 3);
  ut_free_1d (&B);
  ut_free_1d (&lambda);
  ut_free_1d_int (&kept);

  return (maxdist < 1e-6) ? 0 : -1;
}

void
neut_tess_poly_faces_aveq (struct TESS Tess, int poly, int *faces,
                           int faceqty, double *eq)
{
  int i, ori, face;
  double area, *tmp = ut_alloc_1d (4);

  ut_array_1d_zero (eq, 4);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    ut_array_1d_memcpy (Tess.FaceEq[face], 4, tmp);

    neut_tess_poly_face_ori (Tess, poly, face, &ori);

    neut_tess_face_area (Tess, face, &area);

    ut_array_1d_scale (tmp, 4, ori * area);

    ut_array_1d_add (eq, tmp, 4, eq);
  }

  ut_array_1d_scale (eq, 4, 1 / ut_array_1d_norm (eq + 1, 3));

  ut_free_1d (&tmp);

  return;
}

void
neut_tess_coo_percoos (struct TESS Tess, double *coo, double ***ppercoos,
                       int *ppercooqty)
{
  int i, j, k, l, id, qty = ut_array_1d_int_sum (Tess.Periodic, 3);
  int *per = ut_alloc_1d_int (3);

  (*ppercooqty) = pow (3, qty);

  (*ppercoos) = ut_alloc_2d (*ppercooqty, 3);

  ut_array_1d_memcpy (coo, 3, *ppercoos[0]);
  for (i = 0; i < 3; i++)
  {
    while (Tess.Periodic[i] && (*ppercoos)[0][i] < 0)
      (*ppercoos)[0][i] += Tess.PeriodicDist[i];
    while (Tess.Periodic[i] && (*ppercoos)[0][i] > Tess.PeriodicDist[i])
      (*ppercoos)[0][i] -= Tess.PeriodicDist[i];
  }

  id = 0;
  for (k = -Tess.Periodic[2]; k <= Tess.Periodic[2]; k++)
    for (j = -Tess.Periodic[1]; j <= Tess.Periodic[1]; j++)
      for (i = -Tess.Periodic[0]; i <= Tess.Periodic[0]; i++)
        if (i != 0 || j != 0 || k != 0)
        {
          ut_array_1d_int_set_3 (per, i, j, k);

          id++;
          for (l = 0; l < 3; l++)
            (*ppercoos)[id][l] =
              (*ppercoos)[0][l] + per[l] * Tess.PeriodicDist[l];
        }

  ut_free_1d_int (&per);

  return;
}

void
neut_tess_group_vol (struct TESS Tess, int group, double *pvol)
{
  int i;
  double tmp;

  (*pvol) = 0;
  for (i = 1; i <= Tess.CellQty; i++)
    if (Tess.CellGroup && Tess.CellGroup[i] == group)
    {
      neut_tess_poly_volume (Tess, i, &tmp);
      (*pvol) += tmp;
    }

  return;
}

void
neut_tess_group_area (struct TESS Tess, int group, double *parea)
{
  int i;
  double tmp;

  (*parea) = 0;
  for (i = 1; i <= Tess.CellQty; i++)
    if (Tess.CellGroup && Tess.CellGroup[i] == group)
    {
      neut_tess_face_area (Tess, i, &tmp);
      (*parea) += tmp;
    }

  return;
}

void
neut_tess_group_size (struct TESS Tess, int group, double *psize)
{
  (*psize) = 0;

  if (Tess.Dim == 2)
    neut_tess_group_area (Tess, group, psize);
  else if (Tess.Dim == 3)
    neut_tess_group_vol (Tess, group, psize);

  return;
}

void
neut_tess_edge_dir_length (struct TESS Tess, int edge, double *dir, double *plength)
{
  *plength = fabs (ut_vector_scalprod (Tess.VerCoo[Tess.EdgeVerNb[edge][0]], dir)
                 - ut_vector_scalprod (Tess.VerCoo[Tess.EdgeVerNb[edge][1]], dir));

  return;
}

void
neut_tess_face_dir_length (struct TESS Tess, int face, double *dir, double *plength)
{
  int i, ver;
  double *val = ut_alloc_1d (Tess.FaceVerQty[face] + 1);

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    val[i] = ut_vector_scalprod (Tess.VerCoo[ver], dir);
  }

  *plength = ut_array_1d_max (val + 1, Tess.FaceVerQty[face])
           - ut_array_1d_min (val + 1, Tess.FaceVerQty[face]);

  ut_free_1d (&val);

  return;
}

void
neut_tess_poly_dir_length (struct TESS Tess, int poly, double *dir, double *plength)
{
  int i, *vers = NULL, verqty, ver;
  double *val = NULL;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  val = ut_alloc_1d (verqty + 1);

  for (i = 1; i <= verqty; i++)
  {
    ver = vers[i - 1];
    val[i] = ut_vector_scalprod (Tess.VerCoo[ver], dir);
  }

  *plength = ut_array_1d_max (val + 1, verqty)
           - ut_array_1d_min (val + 1, verqty);

  ut_free_1d (&val);
  ut_free_1d_int (&vers);

  return;
}
