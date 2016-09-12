/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#include"ut.h"

double
ut_space_dist2d (double *a, double *b)
{
  return sqrt ((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}

double
ut_space_dist_squared (double *a, double *b)
{
  return pow (b[0] - a[0], 2)
       + pow (b[1] - a[1], 2)
       + pow (b[2] - a[2], 2);
}

double
ut_space_dist (double *a, double *b)
{
  return sqrt (ut_space_dist_squared (a, b));
}

double
ut_space_dist_int (int *a, int *b)
{
  return sqrt ((double) (b[0] - a[0]) * (b[0] - a[0])
	     + (double) (b[1] - a[1]) * (b[1] - a[1])
	     + (double) (b[2] - a[2]) * (b[2] - a[2]));
}

int
ut_space_quad_convex (double *A, double *B, double *C, double *D)
{
  int res;
  double *S1 = ut_alloc_1d (3);
  double *S2 = ut_alloc_1d (3);
  double *S3 = ut_alloc_1d (3);
  double *S4 = ut_alloc_1d (3);
  double sc1, sc2, sc3, sc4;

  S1[0] = B[0] - A[0];
  S1[1] = B[1] - A[1];

  S2[0] = C[0] - B[0];
  S2[1] = C[1] - B[1];

  S3[0] = D[0] - C[0];
  S3[1] = D[1] - C[1];

  S4[0] = A[0] - D[0];
  S4[1] = A[1] - D[1];

  sc1 = S1[0] * S2[1] - S1[1] * S2[0];
  sc2 = S2[0] * S3[1] - S2[1] * S3[0];
  sc3 = S3[0] * S4[1] - S3[1] * S4[0];
  sc4 = S4[0] * S1[1] - S4[1] * S1[0];

  if ((ut_num_sgn (sc1) == ut_num_sgn (sc2))
      && (ut_num_sgn (sc2) == ut_num_sgn (sc3))
      && (ut_num_sgn (sc3) == ut_num_sgn (sc4)))
    res = 1;
  else
    res = 0;

  ut_free_1d (S1);
  ut_free_1d (S2);
  ut_free_1d (S3);
  ut_free_1d (S4);

  return res;
}

int
ut_space_quad_convex_int (int *A, int *B, int *C, int *D)
{
  int res;
  int *S1;
  int *S2;
  int *S3;
  int *S4;
  int sc1, sc2, sc3, sc4;

  /*
     if ( (A[0] == B[0] && A[1] == B[1]) || (A[0] == C[0] && A[1] == C[1])
     || (A[0] == D[0] && A[1] == D[1]) || (B[0] == C[0] && B[1] == C[1])
     || (B[0] == D[0] && B[1] == D[1]) || (C[0] == D[0] && C[1] == D[1]))
     return 1;
     else if ( (A[0] == C[0] && A[1] == C[1])
     ||  (B[0] == D[0] && B[1] == D[1]) )
     return 0;
   */

  S1 = ut_alloc_1d_int (3);
  S2 = ut_alloc_1d_int (3);
  S3 = ut_alloc_1d_int (3);
  S4 = ut_alloc_1d_int (3);

  S1[0] = B[0] - A[0];
  S1[1] = B[1] - A[1];

  S2[0] = C[0] - B[0];
  S2[1] = C[1] - B[1];

  S3[0] = D[0] - C[0];
  S3[1] = D[1] - C[1];

  S4[0] = A[0] - D[0];
  S4[1] = A[1] - D[1];

  sc1 = S1[0] * S2[1] - S1[1] * S2[0];
  sc2 = S2[0] * S3[1] - S2[1] * S3[0];
  sc3 = S3[0] * S4[1] - S3[1] * S4[0];
  sc4 = S4[0] * S1[1] - S4[1] * S1[0];

  if ((ut_num_sgn_int (sc1) == ut_num_sgn_int (sc2))
      && (ut_num_sgn_int (sc2) == ut_num_sgn_int (sc3))
      && (ut_num_sgn_int (sc3) == ut_num_sgn_int (sc4)))
    res = 1;
  else
    res = 0;

  ut_free_1d_int (S1);
  ut_free_1d_int (S2);
  ut_free_1d_int (S3);
  ut_free_1d_int (S4);

  return res;
}

void
ut_space_bary2d (double **P, int qty, double *B)
{
  int i, j;
  double tmp;

  for (i = 0; i < 2; i++)
    {
      tmp = 0;
      for (j = 0; j < qty; j++)
	tmp += P[j][i];

      B[i] = tmp / (double) qty;
    }

  return;
}

void
ut_space_bary3d (double **P, int qty, double *B)
{
  int i, j;
  double tmp;

  for (i = 0; i < 3; i++)
    {
      tmp = 0;
      for (j = 0; j < qty; j++)
	tmp += P[j][i];

      B[i] = tmp / (double) qty;
    }

  return;
}

void
ut_space_bary2d_int (int **P, int qty, int *B)
{
  int i, j;
  int tmp;

  for (i = 0; i < 2; i++)
    {
      tmp = 0;
      for (j = 0; j < qty; j++)
	tmp += P[j][i];

      B[i] = ut_num_d2ri ((double) tmp / (double) qty);
    }

  return;
}

int
ut_space_indomain2d (double **P, int qty, double x, double y)
{
  int i, interqty, res;
  double *bary = ut_alloc_1d (2);
  double *A = ut_alloc_1d (2);

  A[0] = x;
  A[1] = y;

  ut_space_bary2d (P, qty, bary);

  interqty = 0;
  for (i = 0; i < qty - 1; i++)
    interqty += ut_space_quad_convex (P[i], bary, P[i + 1], A);
  interqty += ut_space_quad_convex (P[qty - 1], bary, P[0], A);

  if (ut_num_even (interqty))
    res = 1;
  else
    res = 0;

  ut_free_1d (bary);
  ut_free_1d (A);

  return res;
}

int
ut_space_indomain2d_int (int **P, int qty, int *A)
{
  int i, interqty, res;
  int *bary = ut_alloc_1d_int (2);

  ut_space_bary2d_int (P, qty, bary);

  interqty = 0;
  for (i = 0; i < qty - 1; i++)
    interqty += ut_space_quad_convex_int (P[i], bary, P[i + 1], A);
  interqty += ut_space_quad_convex_int (P[qty - 1], bary, P[0], A);

  if (ut_num_even (interqty))
    res = 1;
  else
    res = 0;

  ut_free_1d_int (bary);

  return res;
}

/* indicates the half-space whose a specified point belongs to. */
int
ut_space_planeside (double *P, double *C)
{
  if (P[1] * C[1] + P[2] * C[2] + P[3] * C[3] - P[0] < 0)
    return -1;
  else
    return 1;
}

/* Side indicates the half-space whose a specified point belongs to. */
int
ut_space_planeside_tol (double *P, double *C, double eps)
{
  if (P[1] * C[1] + P[2] * C[2] + P[3] * C[3] - P[0] < eps)
    return -1;
  else
    return 1;
}

/* Side2d indicates the half-space whose a specified point belongs to (2d). */
int
ut_space_planeside_2d (double *P, double *C)
{
  if (P[1] * C[1] + P[2] * C[2] - P[0] < 0)
    return -1;
  else
    return 1;
}


/* ut_space_trianglearea area returns the area of the triangle
 * whose vertex coordinates are specified as input data:
 * vertex i: coo[i][1,2,3] (i=1,2,3)
 * The used formula is:
 * A = || (P2-P1) vect. (P3-P1) || /2
 */
double
ut_space_trianglearea (double *p1, double *p2, double *p3)
{
  int i;
  double res;
  double *tmp1 = ut_alloc_1d (3);
  double *tmp2 = ut_alloc_1d (3);
  double *tmp3 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    {
      tmp1[i] = p2[i] - p1[i];
      tmp2[i] = p3[i] - p1[i];
    }
  ut_vector_vectprod (tmp1, tmp2, tmp3);

  res = 0.5 * ut_vector_norm (tmp3);

  ut_free_1d (tmp1);
  ut_free_1d (tmp2);
  ut_free_1d (tmp3);

  return res;
}

void
ut_space_trianglenormal (double *p1, double *p2, double *p3, double *n)
{
  int i;
  double *a = ut_alloc_1d (4);
  double *b = ut_alloc_1d (4);
  double norm;

  for (i = 0; i < 3; i++)
    {
      a[i] = p2[i] - p1[i];
      b[i] = p3[i] - p2[i];
    }
  ut_vector_vectprod (a, b, n);
  norm = ut_vector_norm (n);

  for (i = 0; i < 3; i++)
    n[i] /= norm;

  ut_free_1d (a);
  ut_free_1d (b);

  return;
}

void
ut_space_polygon_clean (double **coos, int qty, double ***pcoos2,
			int *pqty2, double eps)
{
  int i;

  (*pqty2) = qty;
  if (*pcoos2 != coos)
  {
    (*pcoos2) = ut_alloc_2d (qty, 3);
    ut_array_2d_memcpy (*pcoos2, qty, 3, coos);
  }

  for (i = 0; i < qty; i++)
    while (ut_space_pointaligned ((*pcoos2)[ut_num_rotpos (0, (*pqty2) - 1, i, -1)],
				  (*pcoos2)[i],
				  (*pcoos2)[ut_num_rotpos (0, (*pqty2) - 1, i,  1)],
				  eps))
    {
      ut_array_2d_memcpy ((*pcoos2) + i, (*pqty2) - i - 1, 3, (*pcoos2) + i + 1);
      (*pcoos2) = ut_realloc_2d_delline (*pcoos2, *pqty2, (*pqty2) - 1);
      (*pqty2)--;
    }

  return;
}

int
ut_space_pointaligned (double *p1, double *p2, double *p3, double eps)
{
  int i, res;
  double angle;
  double *v1 = NULL;
  double *v2 = NULL;

  if (ut_array_1d_requal (p1, p2, 3, 1e-6) == 1
   || ut_array_1d_requal (p2, p3, 3, 1e-6) == 1
   || ut_array_1d_requal (p1, p3, 3, 1e-6) == 1)
    return 1;

  v1 = ut_alloc_1d (3);
  v2 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    v1[i] = p2[i] - p1[i];

  for (i = 0; i < 3; i++)
    v2[i] = p3[i] - p1[i];

  angle = ut_vector_angle (v1, v2);

  res = ut_num_equal (angle, 0, eps) || ut_num_equal (angle, 180, eps);

  ut_free_1d (v1);
  ut_free_1d (v2);

  return res;
}

void
ut_space_arrangepoints (double **pts, int qty, double **box, int size,
			int *sequence)
{
  int i, x, y, z;
  double *cl = ut_alloc_1d (3);
  double *BoxCentre = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    cl[i] = (box[i][1] - box[i][0]) / size;

  /* for every cubic box, looking for the elset in it */
  for (z = 0; z < size; z++)
    {
      BoxCentre[2] = box[2][0] + (box[2][1] - box[2][0]) * (z + 0.5) / size;

      for (y = 0; y < size; y++)
	{
	  BoxCentre[1] = box[1][0] +
	    (box[1][1] - box[1][0]) * (y + 0.5) / size;

	  for (x = 0; x < size; x++)
	    {
	      BoxCentre[0] = box[0][0] +
		(box[0][1] - box[0][0]) * (x + 0.5) / size;

	      for (i = 0; i < qty; i++)
		{
		  if (ut_array_1d_int_eltpos (sequence + 1, sequence[0], i) ==
		      -1)
		    if (fabs (pts[i][0] - BoxCentre[0]) <= cl[0]
			&& fabs (pts[i][1] - BoxCentre[1]) <= cl[1]
			&& fabs (pts[i][2] - BoxCentre[2]) <= cl[2])
		      sequence[++sequence[0]] = i;
		}
	    }
	}
    }

  if (sequence[0] != qty)
    {
      printf ("Bad grain sequence for renumbering");
      printf ("sequence[0] = %d != qty = %d\n", sequence[0], qty);
      abort ();
    }

  ut_free_1d (cl);
  ut_free_1d (BoxCentre);

  return;
}

double
ut_space_triangle_area (double *p1, double *p2, double *p3)
{
  double res;
  double *tmp1 = ut_alloc_1d (3);
  double *tmp2 = ut_alloc_1d (3);
  double *tmp3 = ut_alloc_1d (3);

  ut_array_1d_sub (p1, p2, 3, tmp1);
  ut_array_1d_sub (p1, p3, 3, tmp2);
  ut_vector_vectprod (tmp1, tmp2, tmp3);

  res = 0.5 * ut_vector_norm (tmp3);

  ut_free_1d (tmp1);
  ut_free_1d (tmp2);
  ut_free_1d (tmp3);

  return res;
}

double
ut_space_triangle_perimeter (double *p1, double *p2, double *p3)
{
  return ut_space_dist (p1, p2)
       + ut_space_dist (p2, p3)
       + ut_space_dist (p3, p1);
}

int
ut_space_tet_centre (double *p1, double *p2, double *p3, double *p4, double* c)
{
  int i;

  for (i = 0; i < 3; i++)
    c[i] = 0.25 * (p1[i] + p2[i] + p3[i] + p4[i]);

  return 0;
}

int
ut_space_triangle_centre (double *p1, double *p2, double *p3, double* c)
{
  int i;

  for (i = 0; i < 3; i++)
    c[i] = 0.33333333333333333333 * (p1[i] + p2[i] + p3[i]);

  return 0;
}

/* V = norm ( (p1 - p4) . [(p2 - p4) x (p3 - p4)] )/ 6 */
double
ut_space_tet_volume (double *p1, double *p2, double *p3, double *p4)
{
  double V;
  double *a = ut_alloc_1d (3);
  double *b = ut_alloc_1d (3);
  double *c = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3);

  ut_array_1d_sub (p4, p1, 3, a);
  ut_array_1d_sub (p4, p2, 3, b);
  ut_array_1d_sub (p4, p3, 3, c);

  ut_vector_vectprod (b, c, v);
  V = 0.166666666666666666666666667 * ut_vector_scalprod (a, v);

  ut_free_1d (a);
  ut_free_1d (b);
  ut_free_1d (c);
  ut_free_1d (v);

  return (V >= 0) ? V : -V;
}

double
ut_space_hexa_volume (double *p0, double *p1, double *p2, double *p3,
                      double *p4, double *p5, double *p6, double *p7)
{
  double V;
  double* center = ut_alloc_1d (3);

  ut_space_hexa_center (p0, p1, p2, p3, p4, p5, p6, p7, center);

  V = 0;

  V += ut_space_tet_volume (center, p0, p5, p1);
  V += ut_space_tet_volume (center, p0, p5, p4);
  V += ut_space_tet_volume (center, p3, p6, p2);
  V += ut_space_tet_volume (center, p3, p6, p7);

  V += ut_space_tet_volume (center, p1, p6, p2);
  V += ut_space_tet_volume (center, p1, p6, p5);
  V += ut_space_tet_volume (center, p3, p4, p0);
  V += ut_space_tet_volume (center, p3, p4, p7);

  V += ut_space_tet_volume (center, p4, p6, p5);
  V += ut_space_tet_volume (center, p4, p6, p7);
  V += ut_space_tet_volume (center, p0, p2, p1);
  V += ut_space_tet_volume (center, p0, p2, p3);

  ut_free_1d (center);

  return (V >= 0) ? V : -V;
}

int
ut_space_hexa_center (double *p0, double *p1, double *p2, double *p3,
                      double *p4, double *p5, double *p6, double *p7,
		      double* center)
{
  int i;

  for (i = 0; i < 3; i++)
    center[i] = p0[i] + p1[i] + p2[i] + p3[i]
              + p4[i] + p5[i] + p6[i] + p7[i];

  ut_array_1d_scale (center, 3, 0.125);

  return 0;
}

#ifdef HAVE_GSL
#include<gsl/gsl_linalg.h>

/* Projection of a point along a vector onto a plane. */
void
ut_space_projpoint_alongonto (double *coo, double *n, double *plane)
{
  int i;
  int s;
  gsl_matrix *A = gsl_matrix_alloc (4, 4);
  gsl_vector *B = gsl_vector_alloc (4);
  gsl_vector *X = gsl_vector_alloc (4);
  gsl_permutation *p = gsl_permutation_alloc (4);

  /* Projection of any point M (X,Y,Z) along n(a',b',c') onto
   * plane P (ax+by+cz-d=0), denoted as M' (X',Y',Z') is given
   * by the following matrix system:
   * A.X=B with
   * ~ - -
   * A = | 1 0 0 a' |; X = |X'|; B = |X|. (k is a distance parameter.)
   *     | 0 1 0 b' |      |Y'|      |Y|  the 3 first lines means M'M
   *     | 0 0 1 c' |      |Z'|      |Z|  the 4th one means M' belongs to P.
   *     | a b c 0  |      |k |      |d|

   * recording A, which does no depend upon ver, but which is modified by
   * gsl_linalg_LU_... . */
  gsl_matrix_set (A, 0, 0, 1);
  gsl_matrix_set (A, 0, 1, 0);
  gsl_matrix_set (A, 0, 2, 0);
  gsl_matrix_set (A, 0, 3, n[0]);
  gsl_vector_set (B, 0, coo[0]);

  gsl_matrix_set (A, 1, 0, 0);
  gsl_matrix_set (A, 1, 1, 1);
  gsl_matrix_set (A, 1, 2, 0);
  gsl_matrix_set (A, 1, 3, n[1]);
  gsl_vector_set (B, 1, coo[1]);

  gsl_matrix_set (A, 2, 0, 0);
  gsl_matrix_set (A, 2, 1, 0);
  gsl_matrix_set (A, 2, 2, 1);
  gsl_matrix_set (A, 2, 3, n[2]);
  gsl_vector_set (B, 2, coo[2]);

  gsl_matrix_set (A, 3, 3, 0);
  gsl_matrix_set (A, 3, 0, plane[1]);
  gsl_matrix_set (A, 3, 1, plane[2]);
  gsl_matrix_set (A, 3, 2, plane[3]);
  gsl_vector_set (B, 3, plane[0]);

  gsl_linalg_LU_decomp (A, p, &s);
  if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
    {
      printf ("det=%.20f\n", gsl_linalg_LU_det (A, s));
      printf ("ProjectPointAlongOnto: det=0\n");
      abort ();
    }

  gsl_linalg_LU_solve (A, p, B, X);

  for (i = 0; i <= 2; i++)
    coo[i] = gsl_vector_get (X, i);

  gsl_matrix_free (A);
  gsl_vector_free (B);
  gsl_vector_free (X);
  gsl_permutation_free (p);

  return;
}
#endif

void
ut_space_points_plane (double *p1, double *p2, double *p3, double *eq)
{
  int k;
  double norm;
  double *u = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3);

  /* determining normal */
  for (k = 0; k < 3; k++)
    {
      u[k] = p2[k] - p1[k];
      v[k] = p3[k] - p2[k];
    }
  ut_vector_vectprod (u, v, eq + 1);
  norm = ut_vector_norm (eq + 1);
  ut_array_1d_scale (eq + 1, 3, 1. / norm);

  eq[0] = 0;
  /* determining constant */
  for (k = 1; k <= 3; k++)
    eq[0] += eq[k] * p1[k - 1];

  ut_free_1d (u);
  ut_free_1d (v);

  return;
}

void
ut_space_points_line (double *p1, double *p2, double *eq)
{
  double Dx, Dy, a, b;

  Dx = p2[0] - p1[0];
  Dy = p2[1] - p1[1];

  if (fabs (Dx) > fabs (Dy))
  {
    a = Dy / Dx;
    b = p1[1] - a * p1[0];

    eq[0] = -b;
    eq[1] =  a;
    eq[2] = -1;
  }
  else
  {
    a = Dx / Dy;
    b = p1[0] - a * p1[1];

    eq[0] = -b;
    eq[1] = -1;
    eq[2] =  a;
  }

  ut_array_1d_scale (eq, 3, 1. / ut_array_1d_norm (eq + 1, 2));

  return;
}

void
ut_space_points_invect_plane (double *p1, double *p2, double *invect,
			      double *eq)
{
  int i;
  double *p3 = ut_alloc_1d (3);
  double dist = ut_space_dist (p1, p2);
  // dist to make the distance between points quite the same

  for (i = 0; i < 3; i++)
    p3[i] = p2[i] + invect[i] * dist;

  ut_space_points_plane (p1, p2, p3, eq);

  ut_free_1d (p3);

  return;
}

void
ut_space_points_invect_plane_new (double *p1, double *p2, double *invect,
				  double *eq)
{
  int i;
  double norm;
  double *v = ut_alloc_1d (3);

  // determining plane normal by cross product of invect and the vector
  // based on p1 and p2
  for (i = 0; i < 3; i++)
    v[i] = p2[i] - p1[i];

  ut_vector_vectprod (invect, v, eq + 1);
  norm = ut_vector_norm (eq + 1);
  ut_array_1d_scale (eq + 1, 3, norm);

  // determining constant by using the first point
  eq[0] = -(p1[0] * eq[0] + p1[1] * eq[1] + p1[2] * eq[2]);

  ut_free_1d (v);

  return;
}

// Barycentric approach from http://www.blackpawn.com/texts/pointinpoly/
// Check if P is in triangle ABC. tol1 is the relative tolerancy on the position
// wrt the two edges based on A. tol2 is the relative tolerancy on the
// position wrt the edge opposite to A.
int
ut_space_triangle_point_in (double *A, double *B, double *C, double *P,
			    double tol1, double tol2)
{
  double v0[3], v1[3], v2[3], dot00, dot01, dot02, dot11, dot12,
	 u, v, invDenom;

  ut_array_1d_sub (A, C, 3, v0);
  ut_array_1d_sub (A, B, 3, v1);
  ut_array_1d_sub (A, P, 3, v2);

  // Compute dot products
  dot00 = ut_vector_scalprod (v0, v0);
  dot01 = ut_vector_scalprod (v0, v1);
  dot02 = ut_vector_scalprod (v0, v2);
  dot11 = ut_vector_scalprod (v1, v1);
  dot12 = ut_vector_scalprod (v1, v2);

  // Compute barycentric coordinates
  invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
  u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  // Check if point is in triangle
  return (u >= -tol1) && (v >= -tol1) && (u + v <= 1 + tol2);
}

// returns 1 if P is in the tet based on p1, p2, p3 and p4
// all points must be in the same plane.
int
ut_space_tet_point_in (double *p1, double *p2, double *p3, double *p4,
		       double *P)
{
  double res;
  double *bary = ut_alloc_1d (3);
  double **eq = ut_alloc_2d (4, 4);

  res = 0;

  // testing vertices
  if (ut_array_1d_requal (p1, P, 3, 1e-6) == 1
   || ut_array_1d_requal (p2, P, 3, 1e-6) == 1
   || ut_array_1d_requal (p3, P, 3, 1e-6) == 1
   || ut_array_1d_requal (p4, P, 3, 1e-6) == 1)
    res = 1;
  // testing edges
  else if (ut_space_segment_point_in (p1, p2, P) == 1
        || ut_space_segment_point_in (p2, p3, P) == 1
        || ut_space_segment_point_in (p3, p4, P) == 1
        || ut_space_segment_point_in (p1, p4, P) == 1)
    res = 1;
  // testing body
  else
  {
    // calculating plane eqs and bary
    ut_space_points_plane (p1, p2, p3, eq[0]);
    ut_space_points_plane (p2, p1, p4, eq[1]);
    ut_space_points_plane (p3, p4, p2, eq[2]);
    ut_space_points_plane (p4, p3, p1, eq[3]);

    ut_array_1d_zero (bary, 3);
    ut_array_1d_add (bary, p1, 3, bary);
    ut_array_1d_add (bary, p2, 3, bary);
    ut_array_1d_add (bary, p3, 3, bary);
    ut_array_1d_add (bary, p4, 3, bary);
    ut_array_1d_scale (bary, 3, 0.25);

    // checking if the bary and the point are of the same side of each
    // plane
    if ((ut_space_planeside (eq[0], P - 1) ==
	 ut_space_planeside (eq[0], bary - 1))
	&& (ut_space_planeside (eq[1], P - 1) ==
	    ut_space_planeside (eq[1], bary - 1))
	&& (ut_space_planeside (eq[2], P - 1) ==
	    ut_space_planeside (eq[2], bary - 1))
	&& (ut_space_planeside (eq[3], P - 1) ==
	    ut_space_planeside (eq[3], bary - 1)))
      res = 1;
  }

  ut_free_2d (eq, 4);
  ut_free_1d (bary);

  return res;
}

// returns 1 if [AB] and [CD] intersect, 0 otherwise.  the 4 points must
// be in the same plane.
int
ut_space_segments_intersect (double* A, double* B, double* C, double* D)
{
  int res;
  double* P  = ut_alloc_1d (4);
  double* P2 = ut_alloc_1d (4);

  res = -1;

  // Checking for C and/or D in [AB]
  if (ut_space_segment_point_in (A, B, C) == 1
   || ut_space_segment_point_in (A, B, D) == 1)
    res = 1;

  // Checking for A and/or B in [CD] (does not duplicate the previous one)
  else if (ut_space_segment_point_in (C, D, A) == 1
   || ut_space_segment_point_in (C, D, B) == 1)
    res = 1;

  // Checking for C aligned with [AB] (but not in it, due to previous)
  else if (ut_space_points_aligned (A, B, C))
    res = 0;

  // Checking for D aligned with [AB] (but not in it, due to previous)
  else if (ut_space_points_aligned (A, B, D))
    res = 0;

  if (res == -1)
  {
    // At this point, we know the points are not aligned in any way...

    // Checking that the A B C D are coplanar.
    ut_space_points_plane (A, B, C, P);
    ut_space_points_plane (A, B, D, P2);

    if (ut_space_planes_equal (P, P2) == 0)
    {
      printf ("Points are not in the same plane!\n");
      ut_array_1d_fprintf (stdout, A, 3, "%f");
      ut_array_1d_fprintf (stdout, B, 3, "%f");
      ut_array_1d_fprintf (stdout, C, 3, "%f");
      ut_array_1d_fprintf (stdout, D, 3, "%f");
      ut_array_1d_fprintf (stdout, P, 4, "%.12f");
      ut_array_1d_fprintf (stdout, P2, 4, "%.12f");
      abort ();
    }

    // Now, general case.
    // Considering the half spaces build on [AB].  Checking if C
    // and D are in the same. If yes, [AB] and [CD] are not
    // intersecting.  Else, considering the half spaces build on [CD].
    // Checking if A and B are in the same.  If yes
    //
    ut_space_points_invect_plane (A, B, P + 1, P2);
    if (ut_space_planeside (P2, C - 1) ==  ut_space_planeside (P2, D - 1))
      res = 0;
    else
    {
      ut_space_points_invect_plane (C, D, P + 1, P2);
      if (ut_space_planeside (P2, A - 1) ==  ut_space_planeside (P2, B - 1))
	res = 0;
      else
	res = 1;
    }
  }

  if (res == -1)
  {
    printf ("res should not be -1\n");
    abort ();
  }

  ut_free_1d (P);
  ut_free_1d (P2);

  return res;
}

// returns 1 if A B C are aligned, 0 else.
int
ut_space_points_aligned (double* A, double* B, double* C)
{
  if (ut_space_points_aligned2 (A, B, C) != 0)
    return 1;
  else
    return 0;
}

// returns 1 if A B C are aligned and A>B>C, -1 if aligned otherwise, 0 else.
int
ut_space_points_aligned2 (double* A, double* B, double* C)
{
  int i, res;
  double dist, angle, scal;
  double* ab = ut_alloc_1d (3);
  double* ac = ut_alloc_1d (3);

  dist = ut_space_dist (A, B);
  for (i = 0; i < 3; i++)
    ab[i] = (B[i] - A[i]) / dist;

  for (i = 0; i < 3; i++)
    ac[i] = (C[i] - A[i]) / dist;

  angle = ut_vector_angle (ab, ac);
  if (ut_num_equal (angle,   0, 1e-3) != 1
   && ut_num_equal (angle, 180, 1e-3) != 1)
    res = 0;
  else
  {
    scal = ut_vector_scalprod (ab, ac);
    if (scal > 1)
      res = 1;
    else
      res = -1;
  }

  ut_free_1d (ab);
  ut_free_1d (ac);

  return res;
}

// http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
// t is the relative position from x1 (0 at x1, 1 at x2)
// dist is the distance
// coo is the coordinate vector
void
ut_space_segment_point_proj (double* x1, double* x2, double* x0,
			     double *pt, double *pdist, double *coo)
{
  int i;
  double t;
  double *x0x1 = ut_alloc_1d (3);
  double *x1x2 = ut_alloc_1d (3);
  double *P = ut_alloc_1d (3);

  ut_array_1d_sub (x0, x1, 3, x0x1);
  ut_array_1d_sub (x1, x2, 3, x1x2);

  t = - ut_vector_scalprod (x0x1, x1x2) / ut_vector_scalprod (x1x2, x1x2);

  if (pt)
    (*pt) = t;

  if (coo || pdist)
  {
    for (i = 0; i < 3; i++)
      P[i] = (1 - t) * x1[i] + t * x2[i];

    if (coo)
      ut_array_1d_memcpy (coo, 3, P);

    if (pdist)
      (*pdist) = ut_space_dist (x0, P);
  }

  ut_free_1d (x0x1);
  ut_free_1d (x1x2);
  ut_free_1d (P);

  return;
}

int
ut_space_segment_point_in (double* x1, double* x2, double* x0)
{
  double a, p, t;

  a = ut_space_triangle_area (x1, x2, x0);
  p = ut_space_triangle_perimeter (x1, x2, x0);

  // close to the ratio between the height and basis segment of the
  // triangle x0-x1-x2.
  if (a / (p * p) < 1e-9)
  {
    ut_space_segment_point_proj (x1, x2, x0, &t, NULL, NULL);

    if ((t >= -1e-6 && t <= 1 + 1e-6))
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

int
ut_space_lines_equal (double* L1, double* L2)
{
  int res;
  double angle, norm;
  double* L1cpy = ut_alloc_1d (3);
  double* L2cpy = ut_alloc_1d (3);

  ut_array_1d_memcpy (L1cpy, 3, L1);
  norm = ut_array_1d_norm (L1cpy + 1, 2);
  ut_array_1d_scale (L1cpy, 3, 1. / norm);

  ut_array_1d_memcpy (L2cpy, 3, L2);
  norm = ut_array_1d_norm (L2cpy + 1, 2);
  ut_array_1d_scale (L2cpy, 3, 1. / norm);

  angle = ut_vector_angle_2d (L1cpy + 1, L2cpy + 1);
  if (ut_num_equal (angle, 0, 1e-3))
  {
    if (ut_num_equal (L1cpy[0], L2cpy[0], 1e-6))
      res = 1;
    else
      res = 0;
  }
  else if (ut_num_equal (angle, 180, 1e-3))
  {
    if (ut_num_equal (L1cpy[0], -L2cpy[0], 1e-6))
      res = 1;
    else
      res = 0;
  }
  else
    res = 0;

  ut_free_1d (L1cpy);
  ut_free_1d (L2cpy);

  return res;
}

int
ut_space_planes_equal (double* P1, double* P2)
{
  int res;
  double angle, norm;
  double* P1cpy = ut_alloc_1d (4);
  double* P2cpy = ut_alloc_1d (4);

  ut_array_1d_memcpy (P1cpy, 4, P1);
  norm = ut_array_1d_norm (P1cpy + 1, 3);
  ut_array_1d_scale (P1cpy, 4, 1. / norm);

  ut_array_1d_memcpy (P2cpy, 4, P2);
  norm = ut_array_1d_norm (P2cpy + 1, 3);
  ut_array_1d_scale (P2cpy, 4, 1. / norm);

  angle = ut_vector_angle (P1cpy + 1, P2cpy + 1);
  if (ut_num_equal (angle, 0, 1e-3))
  {
    if (ut_num_equal (P1cpy[0], P2cpy[0], 1e-6))
      res = 1;
    else
      res = 0;
  }
  else if (ut_num_equal (angle, 180, 1e-3))
  {
    if (ut_num_equal (P1cpy[0], -P2cpy[0], 1e-6))
      res = 1;
    else
      res = 0;
  }
  else
    res = 0;

  ut_free_1d (P1cpy);
  ut_free_1d (P2cpy);

  return res;
}

/*
int
ut_space_planes_equal (double* P1, double* P2)
{
  int res;
  int idx;
  double norm;
  double* P1cpy = ut_alloc_1d (4);
  double* P2cpy = ut_alloc_1d (4);

  ut_array_1d_memcpy (P1cpy, 4, P1);
  norm = ut_array_1d_norm (P1cpy, 4);
  ut_array_1d_scale (P1cpy, 4, 1. / norm);
  idx = ut_array_1d_absmax (P1cpy + 1, 3);
  if (P1cpy[idx + 1] < 0)
    ut_array_1d_scale (P1cpy, 4, -1);

  ut_array_1d_memcpy (P2cpy, 4, P2);
  norm = ut_array_1d_norm (P2cpy, 4);
  ut_array_1d_scale (P2cpy, 4, 1. / norm);
  idx = ut_array_1d_absmax (P2cpy + 1, 3);
  if (P2cpy[idx + 1] < 0)
    ut_array_1d_scale (P2cpy, 4, -1);

  res = ut_array_1d_requal (P1cpy, P2cpy, 4, 1e-6);
  printf ("P1 = %2.0f %2.0f %2.0f %2.0f ", P1[0], P1[1], P1[2], P1[3]);
  printf ("P2 = %2.0f %2.0f %2.0f %2.0f ", P2[0], P2[1], P2[2], P2[3]);
  if (res == 0)
    printf ("are not equal\n");
  else
    printf ("are equal\n");

  ut_free_1d (P1cpy);
  ut_free_1d (P2cpy);

  return res;
}
*/

void
ut_space_points_covarmatrix (double** P, double* w, int dim, int size, double** evect, double* eval)
{
  int i, j, k;
  double sumw;
  double** ERR = ut_alloc_2d (dim, dim);
  double*  ER  = ut_alloc_1d (dim);
  double** S   = ut_alloc_2d (dim, dim);

  if (w != NULL)
  {
    sumw = ut_array_1d_sum (w, size);
    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
      {
	for (k = 0; k < size; k++)
	  ERR[i][j] += w[k] * P[k][i] * P[k][j];

	ERR[i][j] /= sumw;
      }

    for (i = 0; i < dim; i++)
    {
      for (k = 0; k < size; k++)
	ER[i] += w[k] * P[k][i];

      ER[i] /= sumw;
    }
  }
  else
  {
    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
      {
	for (k = 0; k < size; k++)
	  ERR[i][j] += P[k][i] * P[k][j];

	ERR[i][j] /= (double) size;
      }

    for (i = 0; i < dim; i++)
    {
      for (k = 0; k < size; k++)
	ER[i] += P[k][i];

      ER[i] /= (double) size;
    }
  }

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      S[i][j] = ERR[i][j] - ER[i] * ER[j];

  /* calculation of the eigen values and vectors of S */

  ut_mat_eigen (dim, S, eval, evect);

  ut_free_2d (ERR, dim);
  ut_free_1d (ER);
  ut_free_2d (S, dim);

  return;
}

int
ut_space_contour_intersect (double** coo, int ptqty)
{
  int i, j, res, last, I, J;

  // Consider there are 6 elements.
  // elt 1: checking 3-4-5 (i + 2)
  // elt 2: checking 4-5-6
  // elt 3: checking 5-6
  // elt 4: checking 6
  // elt 5-6: none (2 lasts)

  res = 0;
  for (i = 0; i < ptqty - 2; i++)
  {
    last = (i == 0) ? ptqty - 1 : ptqty;
    for (j = i + 2; j < last; j++)
    {
      I = (i + 1 != ptqty)? i + 1 : 0;
      J = (j + 1 != ptqty)? j + 1 : 0;

      res = ut_space_segments_intersect (coo[i], coo[I],
	                                 coo[j], coo[J]);

      if (res == 1)
	break;
    }

    if (res == 1)
      break;
  }

  return res;
}

int
ut_space_points_uvect (double* a, double* b, double* uvect)
{
  int i;

  for (i = 0; i < 3; i++)
    uvect[i] = b[i] - a[i];

  ut_array_1d_scale (uvect, 3, 1./ut_array_1d_norm (uvect, 3));

  return 0;
}

int
ut_space_point_line_dist (double* C, double* L, double* pdist)
{
  (*pdist) = fabs (C[0] * L[1] + C[1] * L[2] - L[0])
             / ut_array_1d_norm (L + 1, 2);

  return 0;
}

#ifdef HAVE_GSL
int
ut_space_point_plane_dist (double* C, double* P, double* pdist)
{
  ut_space_point_plane_dist_signed (C, P, pdist);
  (*pdist) = fabs (*pdist);

  return 0;
}

// http://mathworld.wolfram.com/Point-PlaneDistance.html
int
ut_space_point_plane_dist_signed (double* C, double* P, double* pdist)
{
  (*pdist) = (C[0] * P[1] + C[1] * P[2] + C[2] * P[3] - P[0])
	      / ut_array_1d_norm (P + 1, 3);

  return 0;
}

// Provides the intersection point between segment [AB] and plane P.
// The results is recorded in I.
int
ut_space_segment_plane_intersect (double* A, double* B, double* P,
                                  double* I)
{
  int i;
  double* AB = ut_alloc_1d (3);

  ut_array_1d_memcpy (I, 3, A);

  for (i = 0; i < 3; i++)
    AB[i] = B[i] - A[i];

  ut_space_projpoint_alongonto (I, AB, P);

  ut_free_1d (AB);

  return 0;
}
#endif

void
ut_space_string_vect (char* string, double* vect)
{
  double* plane = ut_alloc_1d (4);
  char* string2 = ut_alloc_1d_char (strlen (string) + 3);

  sprintf (string2, "%s=0", string);

  ut_space_string_plane (string2, plane);
  ut_array_1d_memcpy (vect, 3, plane + 1);

  ut_free_1d_char (string2);
  ut_free_1d (plane);

  return;
}

void
ut_space_string_plane (char* string, double* plane)
{
  int i;
  char c;
  double res;
  char* tmp = NULL;
  char* expr = NULL;
  double* val = NULL;
  char** var = NULL;

  // plane[1] * x + plane[2] * y + plane[3] * z = plane[0]
  ut_array_1d_zero (plane, 4);

  if (sscanf (string, "%c=%lf", &c, &res) == 2
      && (c == 'x'|| c == 'y' || c == 'z'))
  {
    plane[0] = res;
    plane[c - 'x' + 1] = 1;
  }

  else
  {
    val = ut_alloc_1d (3);
    var = ut_alloc_2d_char (3, 2);

    ut_string_string (string, &tmp);
    ut_string_fnrs (tmp, "=", "-(", 1);
    expr = ut_alloc_1d_char (strlen (tmp) + 2);
    sprintf (expr, "%s)", tmp);

    for (i = 0; i < 3; i++)
      sprintf (var[i], "%c", 'x' + i);

    // determining d (constant term)
    ut_array_1d_zero (val, 3);
    ut_math_eval (expr, 3, var, val, &res);
    plane[0] = -res;

    // determining factors
    for (i = 0; i < 3; i++)
    {
      ut_array_1d_zero (val, 3);
      if (strstr (expr, var[i]))
      {
	val[i] = 1;
	ut_math_eval (expr, 3, var, val, &res);
	plane[i + 1] = res + plane[0];
      }
    }

    ut_array_1d_scale (plane, 4, 1. / ut_array_1d_norm (plane + 1, 3));

    ut_free_1d_char (tmp);
    ut_free_1d_char (expr);
    ut_free_1d (val);
    ut_free_2d_char (var, 3);
  }

  return;
}

// Implementation of
// http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
int
ut_space_segments_intersectpt (double* P, double* B, double* Q, double* D,
    double* I)
{
  int i, id, status;
  double t, u;
  double* R = ut_alloc_1d (3);
  double* S = ut_alloc_1d (3);
  double* QmP = ut_alloc_1d (3);
  double* QmPxS = ut_alloc_1d (3);
  double* RxS = ut_alloc_1d (3);
  double* QmPxR = ut_alloc_1d (3);

  ut_array_1d_sub (P, B, 3, R);
  ut_array_1d_sub (Q, D, 3, S);
  ut_array_1d_sub (P, Q, 3, QmP);
  ut_vector_vectprod (QmP, S, QmPxS);
  ut_vector_vectprod (QmP, R, QmPxR);

  ut_vector_vectprod (R, S, RxS);

  id = ut_array_1d_absmax_index (RxS, 3);

  t = QmPxS[id] / RxS[id];
  u = QmPxR[id] / RxS[id];

  if (t > 0 && t < 1 && u > 0 && u < 1)
  {
    for (i = 0; i < 3; i++)
      I[i] = P[i] + t * R[i];

    status = 1;
  }
  else
    status = 0;

  ut_free_1d (R);
  ut_free_1d (S);
  ut_free_1d (QmP);
  ut_free_1d (QmPxS);
  ut_free_1d (RxS);
  ut_free_1d (QmPxR);

  return status;
}

int
ut_space_bary3d_constrained (double** ptcoo, double* ptw, int ptqty,
                             double** eqs, int eqqty, double* barycoo)
{
  int i, j, N, M;
  double **A = NULL, *B = NULL, *X = NULL;
  double sumweight;

  N = ptqty;
  M = eqqty;
  A = ut_alloc_2d (3 + M, 3 + M);
  X = ut_alloc_1d (3 + M);
  B = ut_alloc_1d (3 + M);

  // Filling up the 3 x 3 first values of A
  sumweight = ptw ? ut_array_1d_sum (ptw, ptqty) : ptqty;

  for (i = 0; i < 3; i++)
    A[i][i] = sumweight;

  // Filling up the 3 first values of B
  ut_array_1d_zero (B, 3);
  for (i = 0; i < 3; i++)
    for (j = 0; j < N; j++)
      B[i] += (ptw ? ptw[j] : 1) * ptcoo[j][i];

  // Filling up the 3 x M (x 2) constraint values of A and
  // the M constraint values of B.
  double *nprime = NULL;
  double dprime;
  for (i = 0; i < M; i++)
  {
    nprime = eqs[i] + 1;
    dprime = eqs[i][0];
    for (j = 0; j < 3; j++)
    {
      A[i + 3][j] = nprime[j];
      A[j][i + 3] = nprime[j];
      B[i + 3] = dprime;
    }
  }

  int status = ut_linalg_solve_LU (A, B, M + 3, X);
  if (status != 0)
    ut_error_reportbug ();

  ut_array_1d_memcpy (barycoo, 3, X);

  ut_free_2d (A, 3 + M);
  ut_free_1d (B);
  ut_free_1d (X);

  return 0;
}

int
ut_space_bary2d_constrained (double** ptcoo, double* ptw, int ptqty,
                             double** eqs, int eqqty, double* barycoo)
{
  int i, j, N, M;
  double **A = NULL, *B = NULL, *X = NULL;
  double sumweight;

  N = ptqty;
  M = eqqty;
  A = ut_alloc_2d (2 + M, 2 + M);
  X = ut_alloc_1d (2 + M);
  B = ut_alloc_1d (2 + M);

  // Filling up the 2 x 2 first values of A
  sumweight = ptw ? ut_array_1d_sum (ptw, ptqty) : ptqty;

  for (i = 0; i < 2; i++)
    A[i][i] = sumweight;

  // Filling up the 2 first values of B
  ut_array_1d_zero (B, 2);
  for (i = 0; i < 2; i++)
    for (j = 0; j < N; j++)
      B[i] += (ptw ? ptw[j] : 1) * ptcoo[j][i];

  // Filling up the 2 x M (x 2) constraint values of A and
  // the M constraint values of B.
  double *nprime = NULL;
  double dprime;
  for (i = 0; i < M; i++)
  {
    nprime = eqs[i] + 1;
    dprime = eqs[i][0];
    for (j = 0; j < 2; j++)
    {
      A[i + 2][j] = nprime[j];
      A[j][i + 2] = nprime[j];
      B[i + 2] = dprime;
    }
  }

  int status = ut_linalg_solve_LU (A, B, M + 2, X);
  if (status != 0)
    ut_error_reportbug ();

  ut_array_1d_memcpy (barycoo, 2, X);

  ut_free_2d (A, 2 + M);
  ut_free_1d (B);
  ut_free_1d (X);

  return 0;
}

// see pseudo code and implementation at
// http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
int
ut_space_pts_convexhull_2d (double **coo, int qty, int **phids, int *phqty)
{
  int i, Lqty, Uqty;
  int *idx = ut_alloc_1d_int (qty);
  int *L = ut_alloc_1d_int (qty);
  int *U = ut_alloc_1d_int (qty);
  double *tmp = ut_alloc_1d (qty);
  int id;

  for (i = 0; i < qty; i++)
    tmp[i] = coo[i][0];
  ut_array_1d_sort_index (tmp, qty, idx);

  Lqty = 0;
  for (i = 0; i < qty; i++)
  {
    id = idx[i];

    while (Lqty >= 2 && ut_space_pts_ccw (coo[L[Lqty - 2]],
					  coo[L[Lqty - 1]],
					  coo[id]) <= 0)
      Lqty--;
    L[Lqty++] = id;
  }
  Lqty--;

  Uqty = 0;
  for (i = qty - 1; i >= 0; i--)
  {
    id = idx[i];

    while (Uqty >= 2 && ut_space_pts_ccw (coo[U[Uqty - 2]],
					  coo[U[Uqty - 1]],
					  coo[id]) <= 0)
      Uqty--;
    U[Uqty++] = id;
  }
  Uqty--;

  (*phqty) = Lqty + Uqty;
  (*phids) = ut_alloc_1d_int (*phqty);

  ut_array_1d_int_memcpy (*phids, Lqty, L);
  ut_array_1d_int_memcpy (*phids + Lqty, Uqty, U);

  ut_free_1d (tmp);
  ut_free_1d_int (idx);
  ut_free_1d_int (L);
  ut_free_1d_int (U);

  return 0;
}

double
ut_space_pts_ccw (double* p1, double* p2, double* p3)
{
  return (p2[0] - p1[0]) * (p3[1] - p1[1]) - (p2[1] - p1[1]) * (p3[0] - p1[0]);
}

void
ut_space_pts_bbox (double** coos, int qty, double **bbox)
{
  int i, j;

  bbox[0][0] = bbox[1][0] = bbox[2][0] = DBL_MAX;
  bbox[0][1] = bbox[1][1] = bbox[2][1] = -DBL_MAX;

  for (i = 0; i < qty; i++)
    for (j = 0; j < 3; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], coos[i][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], coos[i][j]);
    }

  return;
}

void
ut_space_points_bisplane (double* A, double* B, double* plane)
{
  int i;

  for (i = 0; i < 3; i++)
    plane[i + 1] = 2 * (B[i] - A[i]);

  plane[0] = 0;
  for (i = 0; i < 3; i++)
    plane[0] +=
      (pow (B[i], 2) - pow (A[i], 2));

  ut_array_1d_scale (plane, 4, 1. / ut_vector_norm (plane + 1));

  return;
}

void
ut_space_points_bisplane_power (double* A, double* B, double wA,
                               double wB, double* plane)
{
  int i;

  for (i = 0; i < 3; i++)
    plane[i + 1] = 2 * (B[i] - A[i]);

  plane[0] = 0;
  for (i = 0; i < 3; i++)
    plane[0] += (pow (B[i], 2) - pow (A[i], 2));

  plane[0] -= pow (wB, 2) - pow (wA, 2);

  ut_array_1d_scale (plane, 4, 1. / ut_vector_norm (plane + 1));

  return;
}

void
ut_space_pts_dir_furthestpt (double **coo, int qty, double *dir, int* ppt)
{
  int i;
  double tmp, max;

  (*ppt) = -1;
  max = -DBL_MAX;
  for (i = 0; i < qty; i++)
  {
    tmp = ut_vector_scalprod (coo[i], dir);
    if (tmp > max)
    {
      max = tmp;
      (*ppt) = i;
    }
  }

  return;
}

void
ut_space_pts_dir_furthestpts (double **coo, int qty, double *dir,
                              double eps, int **ppts, int *pqty)
{
  int i, id;
  double max, *tmp = ut_alloc_1d (qty);

  id = 0;
  max = -DBL_MAX;
  for (i = 0; i < qty; i++)
  {
    tmp[i] = ut_vector_scalprod (coo[i], dir);
    if (tmp[i] > max)
    {
      max = tmp[i];
      id = i;
    }
  }

  ut_free_1d_int (*ppts);
  (*ppts) = NULL;
  ut_array_1d_int_list_addelt (ppts, pqty, id);

  for (i = 0; i < qty; i++)
    if (i != id && tmp[i] > max - eps)
      ut_array_1d_int_list_addelt (ppts, pqty, i);

  ut_free_1d (tmp);

  return;
}

int
ut_space_vectpair_samedir (double *v1, double *v2)
{
  if (ut_vector_scalprod (v1, v2) >= 0)
    return 1;
  else
    return 0;
}

int
ut_space_vectpair_samedir_tol (double *v1, double *v2, double tol)
{
  if (ut_vector_scalprod (v1, v2) >= - tol * tol)
    return 1;
  else
    return 0;
}

// Guy with the cap's method... http://mollyrocket.com/849
int
ut_space_polypts_point_dist (double** ptcoos0, int ptqty,
			     double* ptcoo, double *pdist)
{
  return ut_space_polypts_point_dist_verbosity (ptcoos0, ptqty, ptcoo,
						pdist, 0);
}

int
ut_space_polypts_point_dist_verbosity (double** ptcoos0, int ptqty,
    double* ptcoo, double *pdist, int verbosity)
{
  int i, A, status, qty, startpt, iter, error = 0;
  double **coos = NULL;
  int *list = ut_alloc_1d_int (4);
  double *dir = ut_alloc_1d (3);
  double *O = ut_alloc_1d (3);
  double *tmp = ut_alloc_1d (3);
  double dist, eps = 1e-9, tmp0, tmp1;
  FILE *fp = NULL;

  // computing the Minkowski difference between the pt and the poly
  coos = ut_alloc_2d (ptqty, 3);
  for (i = 0; i < ptqty; i++)
    ut_array_1d_sub (ptcoo, ptcoos0[i], 3, coos[i]);

  if (verbosity > 0)
  {
    fp = ut_file_open ("poly.geo", "w");
    for (i = 0; i < ptqty; i++)
      fprintf (fp, "Point (%d) = {%.12f, %.12f, %.12f};\n",
	       i, ptcoos0[i][0], ptcoos0[i][1], ptcoos0[i][2]);
    ut_file_close (fp, "ut_space_polypts_point_dist_verbosity.geo", "w");
  }

  if (verbosity)
    fp = ut_file_open ("ut_space_polypts_point_dist_verbosity.debug", "w");

  if (verbosity)
  {
    fprintf (fp, "ptcoos0 =\n");
    ut_array_2d_fprintf (fp, ptcoos0, ptqty, 3, "%f");
    fprintf (fp, "ptcoo = ");
    ut_array_1d_fprintf (fp, ptcoo, 3, "%f");
  }

  startpt = 0;
  tmp0 = ut_vector_norm_squared (coos[0]);
  for (i = 1; i < ptqty; i++)
  {
    tmp1 = ut_vector_norm_squared (coos[i]);
    if (tmp1 < tmp0)
    {
      tmp0 = tmp1;
      startpt = i;
    }
  }

  // Initializing simplex with a single vertex - taking the first one.
  // Setting the search direction toward the origin.
  qty = 1;
  list[0] = startpt;
  ut_array_1d_sub (coos[list[0]], O, 3, dir);
  ut_array_1d_scale (dir, 3, 1 / ut_vector_norm (dir));
  status = 1;

  // testing if the point is on the vertex.  If not, entering the loop
  // below.
  dist = ut_vector_norm (coos[list[0]]);
  if (dist < eps)
    status = 0;

  iter = 0;
  while (status == 1 && iter++ < 100)
  {
    if (verbosity)
    {
      fprintf (fp, "\niter %d\n", iter);
      fprintf (fp, "starting iteration\n");

      fprintf (fp, "simplex = ");
      ut_array_1d_int_fprintf (fp, list, qty, "%d");
      fprintf (fp, "dir = ");
      ut_array_1d_fprintf (fp, dir, 3, "%f");
    }

    // searching the furthest point along dir (called A)
    int *fpts = NULL, fptqty = 0;
    ut_space_pts_dir_furthestpts (coos, ptqty, dir, 1e-15, &fpts, &fptqty);
    if (verbosity)
    {
      fprintf (fp, "furthest point is/are ");
      ut_array_1d_int_fprintf (fp, fpts, fptqty, "%d");
    }
    A = fpts[0];

    status = 0;

    if (!pdist)
    {
      if (verbosity)
	fprintf (fp, "passing the origin?\n");
      // if OA.dir < 0, we did not pass the origin by travelling along dir,
      // which means no intersection
      if (ut_vector_scalprod (coos[A], dir) < 0)
      {
	qty = 1;
	list[0] = A;
	status = -1;
	if (verbosity)
	  fprintf (fp, "no\n");
      }
    }

    for (i = 0; i < fptqty; i++)
      if (ut_array_1d_int_eltpos (list, qty, fpts[i]) != -1)
      {
	status = -1;
	break;
      }

    // otherwise, there may be an intersection
    if (status != -1)
    {
      /*
      if (verbosity)
        fprintf (fp, "yes\n");
      */

      if (verbosity)
        fprintf (fp, "adding %d to simplex\n", A);

      // adding A to the list of points
      list[qty++] = A;

      if (verbosity)
      {
	fprintf (fp, "entering ut_polypts_point_dist_dosimplex with list =");
	ut_array_1d_int_fprintf (fp, list, qty, "%d");
	fprintf (fp, "dir = "); ut_array_1d_fprintf (fp, dir, 3, "%f");
      }
      // searching for the new points and search direction
      status = ut_space_polypts_point_dist_dosimplex (coos, list, &qty, dir,
						      eps, verbosity, fp);
      if (verbosity)
      {
	fprintf (fp, "exiting ut_polypts_point_dist_dosimplex with list =");
	ut_array_1d_int_fprintf (fp, list, qty, "%d");
	fprintf (fp, "dir = "); ut_array_1d_fprintf (fp, dir, 3, "%f");
	fprintf (fp, "status = %d\n", status);
      }
    }

    ut_free_1d_int (fpts);
  }

  if (pdist)
  {
    if (status != -1)
      (*pdist) = 0;
    else if (qty == 1)
      (*pdist) = ut_vector_norm (coos[list[0]]);
    else if (qty >= 2)
    {
      ut_array_1d_scale (dir, 3, 1. / ut_array_1d_norm (dir, 3));
      (*pdist) = ut_vector_scalprod (coos[list[0]], dir);
      (*pdist) = fabs (*pdist);
    }
    else
      abort ();
  }

  if (status == 1 || (pdist && isnan (*pdist)))
  {
    if (verbosity == 0)
      ut_space_polypts_point_dist_verbosity (ptcoos0, ptqty, ptcoo, pdist, 1);
    error = 1;
  }

  if (verbosity)
  {
    fprintf (fp, "simplex = ");
    ut_array_1d_int_fprintf (fp, list, qty, "%d");
    fprintf (fp, "dir = ");
    ut_array_1d_fprintf (fp, dir, 3, "%f");

    if (status == -1)
      fprintf (fp, "no intersection\n");
    else
      fprintf (fp, "intersection\n");
  }

  ut_free_2d (coos, ptqty);
  ut_free_1d_int (list);
  ut_free_1d (dir);
  ut_free_1d (O);
  ut_free_1d (tmp);

  if (verbosity)
    ut_file_close (fp, "ut_space_polypts_point_dist_verbosity.debug", "w");

  if (error)
    return -1;
  else
  {
    if (status == -1)
      return 0;
    else
      return 1;
  }
}

// returns 0 if point projects inside the simplex or is in it.
// otherwise, returns 1 and updates the simplex and the search direction
int
ut_space_polypts_point_dist_dosimplex (double **vercoo,
				       int *list, int *pqty, double *dir,
				       double eps, int verbosity, FILE* fp)
{
  int i, id, status = -1;
  double norm, dist;
  double *AB = ut_alloc_1d (3);
  double *AC = ut_alloc_1d (3);
  double *AD = ut_alloc_1d (3);
  double *ABC = ut_alloc_1d (3);
  double *ACD = ut_alloc_1d (3);
  double *ADB = ut_alloc_1d (3);
  double *AO = ut_alloc_1d (3);
  double *tmp = ut_alloc_1d (3);
  double *A = NULL, *B = NULL, *C = NULL, *D = NULL;
  double *O = ut_alloc_1d (3);
  int* listb = ut_alloc_1d_int (4);

  // below, O denotes the origin, and the "simplex" denotes the entity
  // closest to O.

  if (verbosity)
  {
    fprintf (fp, "entering ut_poly_point_dist_dosimplex\n");
    fprintf (fp, "*pqty = %d\n", *pqty);
  }

  // line case
  if ((*pqty) == 2)
  {
    // Determining the simplex, which may be AB or A.  Setting the new
    // search direction accordingly.

    // Computing AB.AO to determine if the simplex is AB or A.
    A = vercoo[list[1]];
    B = vercoo[list[0]];
    ut_array_1d_sub (A, B, 3, AB);
    ut_array_1d_sub (A, O, 3, AO);

    // if AB.AO > 0, O the simplex is AB
    // setting the new search direction as ABxAOxAB
    if (ut_space_vectpair_samedir (AB, AO))
    {
      if (verbosity)
        fprintf (fp, "AB is closest\n");
      ut_vector_vectprod (AB, AO, tmp);
      ut_vector_vectprod (tmp, AB, dir);
    }
    // otherwise, the simplex is A. Removing B from the simplex and
    // setting the new search direction as AO
    else
    {
      if (verbosity)
	fprintf (fp, "A is closest\n");

      list[0] = list[1];
      (*pqty) = 1;
      ut_array_1d_memcpy (dir, 3, AO);
    }

    status = 1;
  }

  // triangle case
  else if ((*pqty) == 3)
  {
    double *ABCAC = ut_alloc_1d (3);
    double *ABABC = ut_alloc_1d (3);

    A = vercoo[list[2]];
    B = vercoo[list[1]];
    C = vercoo[list[0]];

    ut_array_1d_sub (A, O, 3, AO);
    ut_array_1d_sub (A, B, 3, AB);
    ut_array_1d_sub (A, C, 3, AC);

    ut_vector_vectprod (AB, AC, ABC);
    ut_vector_vectprod (ABC, AC, ABCAC);
    ut_vector_vectprod (AB, ABC, ABABC);

    // Determining which entity O is closest to. It may be A, AB,
    // AC, ABC-up or ABC-down.  Setting the new search direction.
    if (ut_space_vectpair_samedir (ABCAC, AO))
    {
      // the simplex is AC
      if (ut_space_vectpair_samedir (AC, AO))
      {
	if (verbosity)
	  fprintf (fp, "AC is closest\n");
	list[1] = list[2];
	(*pqty) = 2;
	ut_vector_vectprod (AC, AO, tmp);
	ut_vector_vectprod (tmp, AC, dir);
      }

      // the simplex may be A or AB
      else
      {
	// star case ---------------------------------------------------
	// the simplex is AB
	if (ut_space_vectpair_samedir (AB, AO))
	{
	  if (verbosity)
	    fprintf (fp, "AB is closest 1\n");
	  list[0] = list[1];
	  list[1] = list[2];
	  (*pqty) = 2;
	  ut_vector_vectprod (AB, AO, tmp);
	  ut_vector_vectprod (tmp, AB, dir);
	}

	// the simplex is A
	else
	{
	  if (verbosity)
	    fprintf (fp, "A is closest 1\n");
	  list[0] = list[2];
	  (*pqty) = 1;
	  ut_array_1d_memcpy (dir, 3, AO);
	}
	// end of star case --------------------------------------------
      }
    }

    // other side of AC
    else
    {
      if (ut_space_vectpair_samedir (ABABC, AO))
      {
	// star case ---------------------------------------------------
	// the simplex is AB
	if (ut_space_vectpair_samedir (AB, AO))
	{
	  if (verbosity)
	    fprintf (fp, "AB is closest 2\n");

	  list[0] = list[1];
	  list[1] = list[2];
	  (*pqty) = 2;
	  ut_vector_vectprod (AB, AO, tmp);
	  ut_vector_vectprod (tmp, AB, dir);
	}

	// the simplex is A
	else
	{
	  if (verbosity)
	    fprintf (fp, "A is closest 2\n");
	  list[0] = list[2];
	  (*pqty) = 1;
	  ut_array_1d_memcpy (dir, 3, AO);
	}
	// end of star case --------------------------------------------
      }

      // may be ABC-up or ABC-down
      else
      {
	// ABC
	if (ut_space_vectpair_samedir (ABC, AO))
	{
	  if (verbosity)
	    fprintf (fp, "ABC+ is closest\n");
	  ut_array_1d_memcpy (dir, 3, ABC);
	}
	else
	{
	  if (verbosity)
	    fprintf (fp, "ABC- is closest\n");
	  ut_array_1d_int_switch (list, 0, 1);
	  ut_array_1d_memcpy (dir, 3, ABC);
	  ut_array_1d_scale (dir, 3, -1);
	}
      }
    }

    ut_free_1d (ABCAC);
    ut_free_1d (ABABC);

    status = 1;
  }

  else if ((*pqty) == 4)
  {
    A = vercoo[list[3]];
    B = vercoo[list[2]];
    C = vercoo[list[1]];
    D = vercoo[list[0]];

    ut_array_1d_sub (A, B, 3, AB);
    ut_array_1d_sub (A, C, 3, AC);
    ut_array_1d_sub (A, D, 3, AD);
    ut_array_1d_sub (A, O, 3, AO);

    ut_vector_vectprod (AB, AC, ABC);
    ut_vector_vectprod (AC, AD, ACD);
    ut_vector_vectprod (AD, AB, ADB);

    int *samedir = ut_alloc_1d_int (3);
    samedir[0] = ut_space_vectpair_samedir (ABC, AO);
    samedir[1] = ut_space_vectpair_samedir (ACD, AO);
    samedir[2] = ut_space_vectpair_samedir (ADB, AO);
    int samedir_sum = ut_array_1d_int_sum (samedir, 3);

    if (verbosity)
    {
      fprintf (fp, "AO = "); ut_array_1d_fprintf (fp, AO, 3, "%f");
      fprintf (fp, "ABC = "); ut_array_1d_fprintf (fp, ABC, 3, "%f");
      fprintf (fp, "ABC.AO = %f\n", ut_vector_scalprod (ABC, AO));
      fprintf (fp, "ut_space_vectpair_samedir (ABC, AO) = %d\n",
	       ut_space_vectpair_samedir (ABC, AO));
      fprintf (fp, "ACD = "); ut_array_1d_fprintf (fp, ACD, 3, "%f");
      fprintf (fp, "ACD.AO = %f\n", ut_vector_scalprod (ACD, AO));
      fprintf (fp, "ut_space_vectpair_samedir (ACD, AO) = %d\n",
	       ut_space_vectpair_samedir (ACD, AO));
      fprintf (fp, "ADB = "); ut_array_1d_fprintf (fp, ADB, 3, "%f");
      fprintf (fp, "ADB.AO = %f\n", ut_vector_scalprod (ADB, AO));
      fprintf (fp, "ut_space_vectpair_samedir (ADB, AO) = %d\n",
	       ut_space_vectpair_samedir (ADB, AO));
      fprintf (fp, "samedir_sum = %d\n", samedir_sum);
    }

    ut_array_1d_int_memcpy (listb, 4, list);

    // the point is in the tet
    if (samedir_sum == 0)
      status = 0;

    // the simplex is a face
    else if (samedir_sum == 1)
    {
      if (samedir[0])
      {
	list[0] = listb[1];
	list[1] = listb[2];
	list[2] = listb[3];
	(*pqty) = 3;
	ut_array_1d_memcpy (dir, 3, ABC);
      }

      else if (samedir[1])
      {
	list[0] = listb[0];
	list[1] = listb[1];
	list[2] = listb[3];
	(*pqty) = 3;
	ut_array_1d_memcpy (dir, 3, ACD);
      }

      else if (samedir[2])
      {
	list[0] = listb[2];
	list[1] = listb[0];
	list[2] = listb[3];
	(*pqty) = 3;
	ut_array_1d_memcpy (dir, 3, ADB);
      }

      status = 1;
    }

    else if (samedir_sum == 2)
    {
      // ABC & ACD - intersection = AC
      if (samedir[0] && samedir[1])
      {
	double* ABCAC = ut_alloc_1d (3);
	double* ACACD = ut_alloc_1d (3);
	int* samedir2 = ut_alloc_1d_int (2);

	ut_vector_vectprod (ABC, AC, ABCAC);
	ut_vector_vectprod (AC, ACD, ACACD);

	samedir2[0] = ut_space_vectpair_samedir (ABCAC, AO);
	samedir2[1] = ut_space_vectpair_samedir (ACACD, AO);
	if (verbosity)
	{
	  fprintf (fp, "ABCAC.AO = %d\n", samedir2[0]);
	  fprintf (fp, "ACACD.AO = %d\n", samedir2[1]);
	}

	// AC
	if (samedir2[0] && samedir2[1])
	{
	  list[0] = listb[1];
	  list[1] = listb[3];
	  (*pqty) = 2;
	  ut_vector_vectprod (AC, AO, tmp);
	  ut_vector_vectprod (tmp, AC, dir);
	}
	// ABC
	else if (samedir2[0] == 0)
	{
	  list[0] = listb[1];
	  list[1] = listb[2];
	  list[2] = listb[3];
	  (*pqty) = 3;
	  ut_array_1d_memcpy (dir, 3, ABC);
	}
	// ACD
	else if (samedir2[1] == 0)
	{
	  list[0] = listb[0];
	  list[1] = listb[1];
	  list[2] = listb[3];
	  (*pqty) = 3;
	  ut_array_1d_memcpy (dir, 3, ACD);
	}
	else
	  abort ();

	ut_free_1d (ABCAC);
	ut_free_1d (ACACD);
	ut_free_1d_int (samedir2);
      }

      // ACD & ADB - intersection = AD
      else if (samedir[1] && samedir[2])
      {
	double* ACDAD = ut_alloc_1d (3);
	double* ADADB = ut_alloc_1d (3);
	int* samedir2 = ut_alloc_1d_int (2);

	ut_vector_vectprod (ACD, AD, ACDAD);
	ut_vector_vectprod (AD, ADB, ADADB);

	samedir2[0] = ut_space_vectpair_samedir (ACDAD, AO);
	samedir2[1] = ut_space_vectpair_samedir (ADADB, AO);
	if (verbosity)
	{
	  fprintf (fp, "ACDAD.AO = %d\n", samedir2[0]);
	  fprintf (fp, "ADADB.AO = %d\n", samedir2[1]);
	}

	// AD
	if (samedir2[0] && samedir2[1])
	{
	  list[0] = listb[0];
	  list[1] = listb[3];
	  (*pqty) = 2;
	  ut_vector_vectprod (AD, AO, tmp);
	  ut_vector_vectprod (tmp, AD, dir);
	}
	// ACD
	else if (samedir2[0] == 0)
	{
	  list[0] = listb[0];
	  list[1] = listb[1];
	  list[2] = listb[3];
	  (*pqty) = 3;
	  ut_array_1d_memcpy (dir, 3, ACD);
	}
	// ADB
	else if (samedir2[1] == 0)
	{
	  list[0] = listb[2];
	  list[1] = listb[0];
	  list[2] = listb[3];
	  (*pqty) = 3;
	  ut_array_1d_memcpy (dir, 3, ADB);
	}
	else
	  abort ();

	ut_free_1d (ACDAD);
	ut_free_1d (ADADB);
	ut_free_1d_int (samedir2);
      }

      // ADB & ABC - intersection = AB
      else if (samedir[2] && samedir[0])
      {
	double* ADBAB = ut_alloc_1d (3);
	double* ABABC = ut_alloc_1d (3);
	int* samedir2 = ut_alloc_1d_int (2);

	ut_vector_vectprod (ADB, AB, ADBAB);
	ut_vector_vectprod (AB, ABC, ABABC);

	samedir2[0] = ut_space_vectpair_samedir (ADBAB, AO);
	samedir2[1] = ut_space_vectpair_samedir (ABABC, AO);
	if (verbosity)
	{
	  fprintf (fp, "ADBAB.AO = %d\n", samedir2[0]);
	  fprintf (fp, "ABABC.AO = %d\n", samedir2[1]);
	}

	// AB
	if (samedir2[0] && samedir2[1])
	{
	  list[0] = listb[2];
	  list[1] = listb[3];
	  (*pqty) = 2;
	  ut_vector_vectprod (AB, AO, tmp);
	  ut_vector_vectprod (tmp, AB, dir);
	}
	// ADB
	else if (samedir2[0] == 0)
	{
	  list[0] = listb[2];
	  list[1] = listb[0];
	  list[2] = listb[3];
	  (*pqty) = 3;
	  ut_array_1d_memcpy (dir, 3, ADB);
	}
	// ABC
	else if (samedir2[1] == 0)
	{
	  list[0] = listb[1];
	  list[1] = listb[2];
	  list[2] = listb[3];
	  (*pqty) = 3;
	  ut_array_1d_memcpy (dir, 3, ABC);
	}
	else
	  abort ();

	ut_free_1d (ADBAB);
	ut_free_1d (ABABC);
	ut_free_1d_int (samedir2);
      }

      else
	abort ();

      status = 1;
    }

    else if (samedir_sum == 3)
    {
      int **list2 = ut_alloc_2d_int (3, 4);
      int *qty2 = ut_alloc_1d_int (3);
      double **dir2 = ut_alloc_2d (3, 3);

      ut_array_1d_int_set (qty2, 3, 3);
      ut_array_1d_int_set_3 (list2[0], list[1], list[2], list[3]); // ABC
      ut_array_1d_int_set_3 (list2[1], list[0], list[1], list[3]); // ACD
      ut_array_1d_int_set_3 (list2[2], list[2], list[0], list[3]); // ADB
      ut_array_1d_memcpy (dir2[0], 3, ABC);
      ut_array_1d_memcpy (dir2[1], 3, ACD);
      ut_array_1d_memcpy (dir2[2], 3, ADB);

      // computing simplices for all faces
      // if one is a face, it is the global simplex and we can therefore quit
      // early.  The face is marked and the (uninitialized) qty2/list2
      // of the other faces are not used later.
      id = -1;
      for (i = 0; i < 3; i++)
      {
	ut_space_polypts_point_dist_dosimplex (vercoo, list2[i], &(qty2[i]),
					       dir2[i], eps, verbosity, fp);
	if (verbosity)
	  fprintf (fp, "qty2[%d] = %d\n", i, qty2[i]);

	if (qty2[i] == 3)
	{
	  id = i;
	  break;
	}
      }

      // one of the simplices is a face
      if (id != -1)
      {
	ut_array_1d_int_memcpy (list, qty2[id], list2[id]);
	(*pqty) = 3;
	ut_array_1d_memcpy (dir, qty2[id], dir2[id]);
      }

      // all simplices are A (qty2 == 1 1 1)
      else if (ut_array_1d_int_sum (qty2, 3) == 3)
      {
	list[0] = list[3];
	(*pqty) = 1;
	ut_array_1d_memcpy (dir, 3, AO);
      }

      // at least two of the simplices are an edge, and we must be able
      // to find one edge in common between them
      else
      {
	// ABC & ACD - intersection = AC
	if (qty2[0] == qty2[1] && list2[0][0] == list2[1][0])
	{
	  list[0] = listb[1];
	  list[1] = listb[3];
	  (*pqty) = 2;
	  ut_vector_vectprod (AC, AO, tmp);
	  ut_vector_vectprod (tmp, AC, dir);
	}

	// ACD & ADB - intersection = AD
	else if (qty2[1] == qty2[2] && list2[1][0] == list2[2][0])
	{
	  list[0] = listb[0];
	  list[1] = listb[3];
	  (*pqty) = 2;
	  ut_vector_vectprod (AD, AO, tmp);
	  ut_vector_vectprod (tmp, AD, dir);
	}

	// ADB & ABC - intersection = AB
	else if (qty2[2] == qty2[0] && list2[2][0] == list2[0][0])
	{
	  list[0] = listb[2];
	  list[1] = listb[3];
	  (*pqty) = 2;
	  ut_vector_vectprod (AB, AO, tmp);
	  ut_vector_vectprod (tmp, AB, dir);
	}

	else
	  abort ();
      }

      status = 1;

      ut_free_2d_int (list2, 3);
      ut_free_1d_int (qty2);
      ut_free_2d (dir2, 3);
    }

    else
      abort ();

    ut_free_1d_int (samedir);
  }


  else
  {
    fprintf (fp, "ut_space_polypts_point_dist_dosimplex run with *pqty = %d\n",
	*pqty);
    abort ();
  }

  norm = ut_array_1d_norm (dir, 3);
  if (norm > 0)
    ut_array_1d_scale (dir, 3, 1 / norm);
  else
    ut_array_1d_set_3 (dir, 1, 0, 0);

  dist = ut_vector_scalprod (AO, dir);
  if (dist < eps)
    status = 0;

  ut_free_1d (AB);
  ut_free_1d (AC);
  ut_free_1d (AD);
  ut_free_1d (ABC);
  ut_free_1d (ACD);
  ut_free_1d (ADB);
  ut_free_1d (AO);
  ut_free_1d (O);
  ut_free_1d (tmp);
  ut_free_1d_int (listb);

  return status;
}

void
ut_space_sphere_points (int n, double r, double **pts)
{
  int i;
  double phi, ga, beta, alpha;

  phi = (sqrt (5) + 1) / 2 - 1;
  ga = phi * 2 * M_PI;

  for (i = 1; i <= n; i++)
  {
    beta = ga * (double) i;
    alpha = M_PI * 0.5 - asin (-1 + 2 * (double) i / n);

    pts[i - 1][0] = r * sin (alpha) * cos (beta);
    pts[i - 1][1] = r * sin (alpha) * sin (beta);
    pts[i - 1][2] = r * cos (alpha);
  }

  return;
}

void
ut_space_volume_diameq (double vol, double *pdiameq)
{
  (*pdiameq) = pow ((6 / M_PI) * vol, 1. / 3);

  return;
}

void
ut_space_area_diameq (double area, double *pdiameq)
{
  (*pdiameq) = sqrt ((4 / M_PI) * area);

  return;
}

void
ut_space_volume_radeq (double vol, double *pradeq)
{
  ut_space_volume_diameq (vol, pradeq);
  (*pradeq) *= 0.5;

  return;
}

void
ut_space_area_radeq (double area, double *pradeq)
{
  ut_space_area_diameq (area, pradeq);
  (*pradeq) *= 0.5;

  return;
}

void
ut_space_random (gsl_rng *r, int *dims, int dimqty, double mindist,
		 double maxdist, double *v)
{
  int i, size;
  double norm;

  size = 1 + ut_array_1d_int_max (dims, dimqty);

  ut_array_1d_zero (v, size);
  do
  {
    for (i = 0; i < dimqty; i++)
      v[dims[i]] = 2 * gsl_rng_uniform (r) - 1;
    norm = ut_array_1d_norm (v, size);
  }
  while (norm > 1);

  if (mindist == 0)
    ut_array_1d_scale (v, size, maxdist);
  else if (mindist == maxdist)
    ut_array_1d_scale (v, size, maxdist / norm);
  else
    ut_error_reportbug ();

  return;
}
