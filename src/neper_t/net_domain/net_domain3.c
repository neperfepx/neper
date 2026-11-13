/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain_cylinder_planes (double h, double Rad, int qty, double **eq)
{
  int i;
  double s;
  double *n = ut_alloc_1d (3);
  double *rtheta = ol_rtheta_alloc ();
  double **g = ol_g_alloc ();

  eq[0][1] = 0;
  eq[0][2] = 0;
  eq[0][3] = -1;
  eq[0][0] = 0;

  eq[1][1] = 0;
  eq[1][2] = 0;
  eq[1][3] = 1;
  eq[1][0] = h;

  s = Rad * (1 - cos (M_PI / qty));
  Rad *= cos (M_PI / qty);

  n[0] = -1;
  ol_r_set_this (rtheta, 0, 0, -1);

  for (i = 0; i < qty; i++)
  {
    rtheta[3] = 360 * ((double) i) / qty;
    ol_rtheta_g (rtheta, g);
    ol_g_vect_vect (g, n, eq[i + 2] + 1);
    eq[i + 2][0] =
      Rad + (eq[i + 2][1] * (Rad + s) + eq[i + 2][2] * (Rad + s) + eq[i + 2][3] * (Rad + s));
  }

  ol_r_free (&n);
  ol_rtheta_free (&rtheta);
  ol_g_free (&g);

  return;
}

void
net_domain_tesspoly_planes (struct TESS Tess, int id, int *pqty, double **eq)
{
  int i, face;

  (*pqty) = Tess.PolyFaceQty[id];
  for (i = 1; i <= Tess.PolyFaceQty[id]; i++)
  {
    face = Tess.PolyFaceNb[id][i];
    ut_array_1d_memcpy (Tess.FaceEq[face], 4, eq[i - 1]);
    if (Tess.PolyFaceOri[id][i] == -1)
      ut_array_1d_scale (eq[i - 1], 4, -1);
  }

  return;
}

void
net_domain_clip (struct POLY *pPoly, double **eq, int qty)
{
  int i, shift;
  double **cubesize = ut_alloc_2d (3, 2);

  if ((*pPoly).VerQty == 0)
  {
    cubesize[0][0] = -1e6;
    cubesize[0][1] = 1e6;
    cubesize[1][0] = -1e6;
    cubesize[1][1] = 1e6;
    cubesize[2][0] = -1e6;
    cubesize[2][1] = 1e6;

    net_domain_cube (cubesize, pPoly);
  }

  shift =
    ((*pPoly).FaceQty > 0) ? ut_array_1d_int_min ((*pPoly).FacePoly + 1,
                                                  (*pPoly).FaceQty) : 0;

  for (i = 0; i < qty; i++)
    net_tess3d_polycomp_poly_cut (shift - i - 1, eq[i], NULL, pPoly);

  neut_poly_fixverorder (pPoly);

  ut_free_2d (&cubesize, 3);

  return;
}

void
net_domain_sphere_planes (double rad, int qty, double **eq)
{
  int i, j;
  double **pts = ut_alloc_2d (qty, 3);

  ut_space_sphere_points (1, qty, pts);

  for (i = 0; i < qty; i++)
  {
    ut_array_1d_memcpy (pts[i], 3, eq[i] + 1);
    eq[i][0] = rad;
    for (j = 0; j < 3; j++)
      eq[i][0] += rad * eq[i][j + 1];
  }

  ut_free_2d (&pts, qty);

  return;
}

// The hull planes are at a distance (1 + NET_DOMAIN_HULLGAP) * Rad from
// the axis, so that they are strictly outside the cylinder.
void
net_domain_cylinder_hullplanes (double h, double Rad, int qty, double **eq)
{
  int i;

  // end planes and plane normals
  net_domain_cylinder_planes (h, Rad, qty, eq);

  for (i = 2; i < qty + 2; i++)
    eq[i][0] = (1 + NET_DOMAIN_HULLGAP) * Rad
             + Rad * (eq[i][1] + eq[i][2] + eq[i][3]);

  return;
}

void
net_domain_cylinder_prim (double *parms, struct PRIM **pPrim, int *pPrimQty)
{
  char *expr = ut_alloc_1d_char (1000);
  double rad = parms[1] / 2;

  (*pPrimQty) = 1;
  (*pPrim) = (struct PRIM *) calloc (1, sizeof (struct PRIM));
  neut_prim_set_zero (*pPrim);

  sprintf (expr, "cylinderi(%.17g,%.17g,0,0,0,1,%.17g)", rad, rad, rad);
  if (neut_prim_sscanf (expr, *pPrim))
    ut_print_neperbug ();

  ut_free_1d_char (&expr);

  return;
}

// The hull planes are at a distance (1 + NET_DOMAIN_HULLGAP) * rad from
// the centre, so that they are strictly outside the sphere.
void
net_domain_sphere_hullplanes (double rad, int qty, double **eq)
{
  int i;

  net_domain_sphere_planes (rad, qty, eq);

  for (i = 0; i < qty; i++)
    eq[i][0] += NET_DOMAIN_HULLGAP * rad;

  return;
}

void
net_domain_sphere_prim (double *parms, struct PRIM **pPrim, int *pPrimQty)
{
  char *expr = ut_alloc_1d_char (1000);
  double rad = parms[0] / 2;

  (*pPrimQty) = 1;
  (*pPrim) = (struct PRIM *) calloc (1, sizeof (struct PRIM));
  neut_prim_set_zero (*pPrim);

  sprintf (expr, "spherei(%.17g,%.17g,%.17g,%.17g)", rad, rad, rad, rad);
  if (neut_prim_sscanf (expr, *pPrim))
    ut_print_neperbug ();

  ut_free_1d_char (&expr);

  return;
}

// The lateral surface of the hourglass is the inner part of a torus of
// same centre and axis, whose circular profile goes through the ends
// (radius re) and the centre (radius rc).  For a half-height b and
// d = re - rc, the minor radius a verifies (a - d)^2 + b^2 = a^2, and the
// major radius is rc + a.
void
net_domain_hourglass_prim (double *parms, struct PRIM **pPrim, int *pPrimQty)
{
  char *expr = ut_alloc_1d_char (1000);
  double b = parms[0] / 2, re = parms[1] / 2, rc = parms[3] / 2;
  double d = re - rc, a = (b * b + d * d) / (2 * d);

  (*pPrimQty) = 1;
  (*pPrim) = (struct PRIM *) calloc (1, sizeof (struct PRIM));
  neut_prim_set_zero (*pPrim);

  sprintf (expr, "torus(%.17g,%.17g,%.17g,0,0,1,%.17g,%.17g)", re, re, b,
           rc + a, a);
  if (neut_prim_sscanf (expr, *pPrim))
    ut_print_neperbug ();

  ut_free_1d_char (&expr);

  return;
}

// net_domain_fromtess_bbox returns the bounding box of the vertices and seeds
// of a tessellation (seeds may be out of the domain), and its largest size.
double
net_domain_fromtess_bbox (struct TESS Tess, double **bbox)
{
  int i, j;
  double size = 0;

  for (j = 0; j < 3; j++)
  {
    bbox[j][0] = DBL_MAX;
    bbox[j][1] = -DBL_MAX;
  }

  for (i = 1; i <= Tess.VerQty; i++)
    for (j = 0; j < 3; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Tess.VerCoo[i][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], Tess.VerCoo[i][j]);
    }

  if (Tess.SeedCoo)
    for (i = 1; i <= Tess.CellQty; i++)
      for (j = 0; j < 3; j++)
      {
        bbox[j][0] = ut_num_min (bbox[j][0], Tess.SeedCoo[i][j]);
        bbox[j][1] = ut_num_max (bbox[j][1], Tess.SeedCoo[i][j]);
      }

  for (j = 0; j < 3; j++)
    size = ut_num_max (size, bbox[j][1] - bbox[j][0]);

  return size;
}

// net_domain_fromtess_prim sets the primitive of a curved domain face.
// Returns 0 on success and -1 otherwise, which includes a primitive that does
// not match the tessellation (primitives do not follow all transformations).
int
net_domain_fromtess_prim (struct TESS Tess, int domface, struct PRIM *pPrim)
{
  int i, j, face, status = 0;
  double eps, *proj = ut_alloc_1d (3), **bbox = ut_alloc_2d (3, 2);
  char *expr = ut_alloc_1d_char (100 + 30 * Tess.DomFaceParmQty[domface]);
  // the parameters of an hspacei are those of the hspace, reversed
  double sign = !strcmp (Tess.DomFaceType[domface], "hspacei") ? -1 : 1;

  sprintf (expr, "%s(", Tess.DomFaceType[domface]);
  for (i = 0; i < Tess.DomFaceParmQty[domface]; i++)
    sprintf (expr + strlen (expr), "%s%.17g", i ? "," : "",
             sign * Tess.DomFaceParms[domface][i]);
  strcat (expr, ")");

  status = neut_prim_sscanf (expr, pPrim);

  // (e.g. domain faces that are not primitives, after -transform rmcell)
  if (!(*pPrim).Parms)
    status = -1;

  eps = 1e-3 * net_domain_fromtess_bbox (Tess, bbox);

  for (i = 1; i <= Tess.DomTessFaceQty[domface] && !status; i++)
  {
    face = Tess.DomTessFaceNb[domface][i];
    for (j = 1; j <= Tess.FaceVerQty[face] && !status; j++)
      if (neut_prim_point_proj (*pPrim, Tess.VerCoo[Tess.FaceVerNb[face][j]], proj) > eps)
        status = -1;
  }

  ut_free_1d (&proj);
  ut_free_2d (&bbox, 3);
  ut_free_1d_char (&expr);

  return status ? -1 : 0;
}

void
net_domain_fromtess_addeq (double *eq, double ***peqs, int *peqqty)
{
  (*peqs) = ut_realloc_2d_addline (*peqs, ++(*peqqty), 4);
  ut_array_1d_memcpy (eq, 4, (*peqs)[*peqqty - 1]);

  return;
}

// net_domain_fromtess_hull adds the planes of the faceted hull of a primitive,
// as net_domain does.  The hull must be tight, as the direct cut relies on the
// cell edges, which get longer with the hull.  Primitives that do not bound a
// domain (e.g. an exterior sphere) do not need any.
void
net_domain_fromtess_hull (struct TESS Tess, int domface, struct PRIM Prim,
                          double ***peqs, int *peqqty)
{
  int i, j, face, ver, qty = 0, pseudodim;
  double rad = 0, tmp, min = DBL_MAX, max = -DBL_MAX, pseudosize, **eq = NULL;
  double planemin, planemax;
  double *v = ut_alloc_1d (3), *parms = ut_alloc_1d (10);
  double dot, *k = ut_alloc_1d (3);
  char *expr = ut_alloc_1d_char (100), *n = ut_alloc_1d_char (20);

  if (!strcmp (Prim.Type, "spherei"))
  {
    rad = Prim.Rad[0];
    qty = 100;                  // default of net_domain_sphereparms
    eq = ut_alloc_2d (qty, 4);
    net_domain_sphere_hullplanes (rad, qty, eq);
  }

  else if (!strcmp (Prim.Type, "cylinderi") || !strcmp (Prim.Type, "torus"))
  {
    // extent along the axis and maximum radius of the face vertices
    for (i = 1; i <= Tess.DomTessFaceQty[domface]; i++)
    {
      face = Tess.DomTessFaceNb[domface][i];
      for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      {
        ver = Tess.FaceVerNb[face][j];
        ut_array_1d_sub (Prim.Base, Tess.VerCoo[ver], 3, v);
        tmp = ut_vector_scalprod (v, Prim.Dir);
        min = ut_num_min (min, tmp);
        max = ut_num_max (max, tmp);
        rad = ut_num_max (rad, sqrt (ut_num_max (ut_vector_scalprod (v, v) - tmp * tmp, 0)));
      }
    }

    if (!strcmp (Prim.Type, "cylinderi"))
      rad = Prim.Rad[0];

    // the end planes are preferred, as periodic cells extend beyond them
    planemin = DBL_MAX;
    planemax = -DBL_MAX;
    for (i = 1; i <= Tess.DomFaceQty; i++)
      if (!strcmp (Tess.DomFaceType[i], "plane")
          && fabs (ut_vector_scalprod (Tess.DomFaceEq[i] + 1, Prim.Dir)) > 1 - 1e-6)
      {
        tmp = (Tess.DomFaceEq[i][0] - ut_vector_scalprod (Tess.DomFaceEq[i] + 1, Prim.Base))
            * ut_vector_scalprod (Tess.DomFaceEq[i] + 1, Prim.Dir);
        planemin = ut_num_min (planemin, tmp);
        planemax = ut_num_max (planemax, tmp);
      }
    if (planemax > planemin)
    {
      min = planemin;
      max = planemax;
    }

    if (rad > 0 && max > min)
    {
      if (!strcmp (Prim.Type, "torus"))
        qty = NET_DOMAIN_HOURGLASS_FACETQTY;
      else
      {
        // same number of facets as net_domain_cylinderparms
        sprintf (expr, "cylinder(%.17g,%.17g)", max - min, 2 * rad);
        sprintf (n, "%d", Tess.CellQty);
        net_domain_cylinderparms (expr, n, parms, &pseudodim, &pseudosize);
        qty = ut_num_d2ri (parms[2]) - 2;
      }

      eq = ut_alloc_2d (qty + 2, 4);
      net_domain_cylinder_planes (max - min, rad, qty, eq);

      // removing the end planes, and rotating from the z axis to the axis
      for (i = 0; i < qty; i++)
        ut_array_1d_memcpy (eq[i + 2], 4, eq[i]);

      // (smallest rotation, as a domain is typically rotated about a normal
      // to the axis)
      tmp = sqrt (Prim.Dir[0] * Prim.Dir[0] + Prim.Dir[1] * Prim.Dir[1]);
      if (tmp > 1e-12)
      {
        k[0] = -Prim.Dir[1] / tmp;
        k[1] = Prim.Dir[0] / tmp;

        for (i = 0; i < qty; i++)
        {
          ut_array_1d_memcpy (eq[i] + 1, 3, v);
          dot = k[0] * v[0] + k[1] * v[1];
          eq[i][1] = v[0] * Prim.Dir[2] + k[1] * v[2] * tmp + k[0] * dot * (1 - Prim.Dir[2]);
          eq[i][2] = v[1] * Prim.Dir[2] - k[0] * v[2] * tmp + k[1] * dot * (1 - Prim.Dir[2]);
          eq[i][3] = v[2] * Prim.Dir[2] + (k[0] * v[1] - k[1] * v[0]) * tmp;
        }
      }
    }
  }

  // setting the plane distances from the axis or centre (the normals are the
  // only useful information above)
  for (i = 0; i < qty; i++)
  {
    eq[i][0] = (1 + NET_DOMAIN_HULLGAP) * rad + ut_vector_scalprod (eq[i] + 1, Prim.Base);
    net_domain_fromtess_addeq (eq[i], peqs, peqqty);
  }

  if (eq)
    ut_free_2d (&eq, !strcmp (Prim.Type, "spherei") ? qty : qty + 2);
  ut_free_1d (&v);
  ut_free_1d (&parms);
  ut_free_1d (&k);
  ut_free_1d_char (&expr);
  ut_free_1d_char (&n);

  return;
}
