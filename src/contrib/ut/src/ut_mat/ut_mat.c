/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut.h"

#ifdef HAVE_GSL
#include <gsl/gsl_linalg.h>
#endif

int
ut_mat_product (double **a, int a1, int a2, double **b, int b1, int b2,
                double **c)
{
  int i, j, k;
  double **ccpy;

  if (a2 != b1)
    return -1;

  ccpy = ut_alloc_2d (a1, b2);
  for (i = 0; i < a1; i++)
    for (j = 0; j < b2; j++)
      for (k = 0; k < a2; k++)
        ccpy[i][j] += a[i][k] * b[k][j];

  ut_array_2d_memcpy (ccpy, a1, b2, c);

  ut_free_2d (&ccpy, a1);

  return 0;
}

void
ut_mat_product_33 (double **a, double **b, double **c)
{
  int i, j, k;
  double **ccpy;

  ccpy = ut_alloc_2d (3, 3);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
        ccpy[i][j] += a[i][k] * b[k][j];

  ut_array_2d_memcpy (ccpy, 3, 3, c);

  ut_free_2d (&ccpy, 3);

  return;
}


int
ut_mat_vect_product (double **a, int a1, int a2, double *b, int b1, double *c)
{
  int i, j;
  double *ccpy;

  if (a2 != b1)
    return -1;

  ccpy = ut_alloc_1d (a1);
  for (i = 0; i < a1; i++)
    for (j = 0; j < a2; j++)
      ccpy[i] += a[i][j] * b[j];

  ut_array_1d_memcpy (ccpy, a1, c);

  ut_free_1d (&ccpy);

  return 0;
}

double
ut_mat_det_22 (double **m)
{
  return m[0][0] * m[1][1] - m[1][0] * m[0][1];
}

double
ut_mat_det_33 (double **m)
{
  double det = 0;

  det += m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
  det -= m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]);
  det += m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);

  return det;
}

int
ut_mat_inverse_22 (double **m, double **minv)
{
  double det = ut_mat_det_22 (m);

  if (fabs (det) < 1e-6)
    return -1;

  minv[0][0] = m[1][1] / det;
  minv[0][1] = -m[0][1] / det;
  minv[1][0] = -m[1][0] / det;
  minv[1][1] = m[0][0] / det;

  return 0;
}

int
ut_mat_inverse_33 (double **m, double **minv)
{
  double det = ut_mat_det_33 (m);

  minv[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
  minv[0][1] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
  minv[0][2] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]);

  minv[1][0] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
  minv[1][1] = (m[0][0] * m[2][2] - m[2][0] * m[0][2]);
  minv[1][2] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);

  minv[2][0] = (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
  minv[2][1] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
  minv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]);

  ut_mat_transpose (minv, 3, 3, minv);

  ut_array_2d_scale (minv, 3, 3, 1 / det);

  return 0;
}

double
ut_mat_ddot_33 (double **A, double **B)
{
  return A[0][0] * B[0][0] + A[0][1] * B[0][1] + A[0][2] * B[0][2] +
    A[1][0] * B[1][0] + A[1][1] * B[1][1] + A[1][2] * B[1][2] +
    A[2][0] * B[2][0] + A[2][1] * B[2][1] + A[2][2] * B[2][2];
}

/* Eigen decomposition code for symmetric 3x3 matrices, copied from the
 * public domain Java Matrix library JAMA.  RQ modif: code modifications
 * for ansi compliance and generalization to any size n */

#ifdef MAX
#undef MAX
#endif

#define MAX(a, b) ((a)>(b)?(a):(b))

static double hypot2 (double, double);
static void tql2 (int, double **, double *, double *);
static void tred2 (int, double **, double *, double *);

void
ut_mat_eigen (double **A, int size, double *eval, double **evect)
{
  double *e = ut_alloc_1d (size);
  double **evect2 = ut_alloc_2d (size, size);
  double *eval2 = ut_alloc_1d (size);

  ut_array_2d_memcpy (A, size, size, evect2);
  tred2 (size, evect2, eval2, e);
  tql2 (size, evect2, eval2, e);
  ut_free_1d (&e);

  ut_mat_transpose (evect2, size, size, evect2);

  if (eval)
    ut_array_1d_memcpy (eval2, size, eval);
  if (evect)
    ut_array_2d_memcpy (evect2, size, size, evect);

  return;
}

static double
hypot2 (double x, double y)
{
  return sqrt (x * x + y * y);
}

/* Symmetric tridiagonal QL algorithm. */
static void
tql2 (int n, double **V, double *d, double *e)
{
/*  This is derived from the Algol procedures tql2, by
    Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
    Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
    Fortran subroutine in EISPACK. */

  int i, j, k, l, m, iter;
  double f, tst1, eps, g, p, r, dl1, h, c, c2, c3, el1, s, s2;

  for (i = 1; i < n; i++)
  {
    e[i - 1] = e[i];
  }
  e[n - 1] = 0.0;
  f = 0.0;
  tst1 = 0.0;
  eps = pow (2.0, -52.0);
  for (l = 0; l < n; l++)
  {
    /* Find small subdiagonal element */
    tst1 = MAX (tst1, fabs (d[l]) + fabs (e[l]));
    m = l;
    while (m < n)
    {
      if (fabs (e[m]) <= eps * tst1)
      {
        break;
      }
      m++;
    }
    /* If m == l, d[l] is an eigenvalue,
       otherwise, iterate. */
    if (m > l)
    {
      iter = 0;

      do
      {
        iter = iter + 1;        /* (Could check iteration count here.) */
        /* Compute implicit shift */
        g = d[l];
        p = (d[l + 1] - g) / (2.0 * e[l]);
        r = hypot2 (p, 1.0);
        if (p < 0)
        {
          r = -r;
        }
        d[l] = e[l] / (p + r);
        d[l + 1] = e[l] * (p + r);
        dl1 = d[l + 1];
        h = g - d[l];
        for (i = l + 2; i < n; i++)
        {
          d[i] -= h;
        }
        f = f + h;
        /* Implicit QL transformation. */

        p = d[m];
        c = 1.0;
        c2 = c;
        c3 = c;
        el1 = e[l + 1];
        s = 0.0;
        s2 = 0.0;
        for (i = m - 1; i >= l; i--)
        {
          c3 = c2;
          c2 = c;
          s2 = s;
          g = c * e[i];
          h = c * p;
          r = hypot2 (p, e[i]);
          e[i + 1] = s * r;
          s = e[i] / r;
          c = p / r;
          p = c * d[i] - s * g;
          d[i + 1] = h + s * (c * g + s * d[i]);
          /* Accumulate transformation. */
          for (k = 0; k < n; k++)
          {
            h = V[k][i + 1];
            V[k][i + 1] = s * V[k][i] + c * h;
            V[k][i] = c * V[k][i] - s * h;
          }
        }
        p = -s * s2 * c3 * el1 * e[l] / dl1;
        e[l] = s * p;
        d[l] = c * p;
        /* Check for convergence. */
      }
      while (fabs (e[l]) > eps * tst1);
    }
    d[l] = d[l] + f;
    e[l] = 0.0;
  }
  /* Sort eigenvalues and corresponding vectors. */
  for (i = 0; i < n - 1; i++)
  {
    k = i;
    p = d[i];
    for (j = i + 1; j < n; j++)
    {
      if (d[j] > p)             /* RQ modif to sort by decreasing eigen value */
      {
        k = j;
        p = d[j];
      }
    }
    if (k != i)
    {
      d[k] = d[i];
      d[i] = p;
      for (j = 0; j < n; j++)
      {
        p = V[j][i];
        V[j][i] = V[j][k];
        V[j][k] = p;
      }
    }
  }
}

/* Symmetric Householder reduction to tridiagonal form. */
static void
tred2 (int n, double **V, double *d, double *e)
{
  int i, j, k;
  double scale, f, g, h, hh;

/*  This is derived from the Algol procedures tred2 by
    Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
    Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
    Fortran subroutine in EISPACK. */
  for (j = 0; j < n; j++)
  {
    d[j] = V[n - 1][j];
  }

  /* Householder reduction to tridiagonal form. */
  for (i = n - 1; i > 0; i--)
  {

    /* Scale to avoid under/overflow. */
    scale = 0.0;
    h = 0.0;
    for (k = 0; k < i; k++)
    {
      scale = scale + fabs (d[k]);
    }
    if (scale == 0.0)
    {
      e[i] = d[i - 1];
      for (j = 0; j < i; j++)
      {
        d[j] = V[i - 1][j];
        V[i][j] = 0.0;
        V[j][i] = 0.0;
      }
    }
    else
    {
      /* Generate Householder vector. */
      for (k = 0; k < i; k++)
      {
        d[k] /= scale;
        h += d[k] * d[k];
      }
      f = d[i - 1];
      g = sqrt (h);
      if (f > 0)
      {
        g = -g;
      }
      e[i] = scale * g;
      h = h - f * g;
      d[i - 1] = f - g;
      for (j = 0; j < i; j++)
      {
        e[j] = 0.0;
      }

      /* Apply similarity transformation to remaining columns. */
      for (j = 0; j < i; j++)
      {
        f = d[j];
        V[j][i] = f;
        g = e[j] + V[j][j] * f;
        for (k = j + 1; k <= i - 1; k++)
        {
          g += V[k][j] * d[k];
          e[k] += V[k][j] * f;
        }
        e[j] = g;
      }
      f = 0.0;
      for (j = 0; j < i; j++)
      {
        e[j] /= h;
        f += e[j] * d[j];
      }
      hh = f / (h + h);
      for (j = 0; j < i; j++)
      {
        e[j] -= hh * d[j];
      }
      for (j = 0; j < i; j++)
      {
        f = d[j];
        g = e[j];
        for (k = j; k <= i - 1; k++)
        {
          V[k][j] -= (f * e[k] + g * d[k]);
        }
        d[j] = V[i - 1][j];
        V[i][j] = 0.0;
      }
    }
    d[i] = h;
  }

  /* Accumulate transformations. */
  for (i = 0; i < n - 1; i++)
  {
    V[n - 1][i] = V[i][i];
    V[i][i] = 1.0;
    h = d[i + 1];
    if (h != 0.0)
    {
      for (k = 0; k <= i; k++)
      {
        d[k] = V[k][i + 1] / h;
      }
      for (j = 0; j <= i; j++)
      {
        g = 0.0;
        for (k = 0; k <= i; k++)
        {
          g += V[k][i + 1] * V[k][j];
        }
        for (k = 0; k <= i; k++)
        {
          V[k][j] -= g * d[k];
        }
      }
    }
    for (k = 0; k <= i; k++)
    {
      V[k][i + 1] = 0.0;
    }
  }
  for (j = 0; j < n; j++)
  {
    d[j] = V[n - 1][j];
    V[n - 1][j] = 0.0;
  }
  V[n - 1][n - 1] = 1.0;
  e[0] = 0.0;
}

void
ut_mat_transpose (double **A, int M, int N, double **At)
{
  int i, j;
  double **Cpy = ut_alloc_2d (M, N);

  ut_array_2d_memcpy (A, M, N, Cpy);

  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
      At[i][j] = Cpy[j][i];

  ut_free_2d (&Cpy, M);

  return;
}

void
ut_mat_trace (double **A, int size, double *ptr)
{
  int i;

  (*ptr) = 0;
  for (i = 0; i < size; i++)
    (*ptr) += A[i][i];

  return;
}

int
ut_mat_inverse (double **mat, int n, double **inv)
{
#ifdef HAVE_GSL
  int i, j, signum;

  gsl_matrix *gsl_mat = gsl_matrix_alloc (n, n);
  gsl_matrix *gsl_inv = gsl_matrix_alloc (n, n);
  gsl_permutation *gsl_p = gsl_permutation_alloc (n);
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      gsl_matrix_set (gsl_mat, i, j, mat[i][j]);

  gsl_linalg_LU_decomp (gsl_mat, gsl_p, &signum);
  gsl_linalg_LU_invert (gsl_mat, gsl_p, gsl_inv);

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      inv[i][j] = gsl_matrix_get (gsl_inv, i, j);

  gsl_matrix_free (gsl_mat);
  gsl_matrix_free (gsl_inv);
  gsl_permutation_free (gsl_p);

#else
  if (n == 2)
    ut_mat_inverse_22 (mat, inv);
  else if (n == 3)
    ut_mat_inverse_33 (mat, inv);
  else
    ut_print_messagewnc (2, 72,
                         "ut_mat_inverse for n>3 not available since ut has been compiled without gsl.\n");
#endif

  return 0;
}

void
ut_mat_svd (double **A, int M, int N, double **U, double **S, double **V)
{
  int i;
  double **AAt = NULL, **AtA = NULL;
  double **At = ut_alloc_2d (N, M);
  double *L = ut_alloc_1d (ut_num_max (M, N));

  ut_mat_transpose (A, M, N, At);

  if (U)
  {
    AAt = ut_alloc_2d (M, M);
    ut_mat_product (A, M, N, At, N, M, AAt);
    ut_mat_eigen (AAt, M, L, U);
    ut_free_2d (&AAt, M);
  }

  if (V)
  {
    AtA = ut_alloc_2d (N, N);
    ut_mat_product (At, N, M, A, M, N, AtA);
    ut_mat_eigen (AtA, N, L, V);
    ut_free_2d (&AtA, N);
  }

  if (S)
  {
    ut_array_2d_zero (S, M, N);
    for (i = 0; i < ut_num_min (M, N); i++)
      S[i][i] = sqrt (L[i]);
  }

  ut_free_2d (&At, M);
  ut_free_1d (&L);

  return;
}

void
ut_mat_sym (double **A, int size, double **S)
{
  int i, j;

  for (i = 0; i < size; i++)
    S[i][i] = A[i][i];

  for (i = 0; i < size; i++)
    for (j = i + 1; j < size; j++)
      S[i][j] = (A[i][j] + A[j][i]) / 2;

  for (i = 0; i < size; i++)
    for (j = 0; j < i; j++)
      S[i][j] = S[j][i];

  return;
}

void
ut_mat_skew (double **A, int size, double **S)
{
  int i, j;

  for (i = 0; i < size; i++)
    S[i][i] = 0;

  for (i = 0; i < size; i++)
    for (j = i + 1; j < size; j++)
      S[i][j] = (A[i][j] - A[j][i]) / 2;

  for (i = 0; i < size; i++)
    for (j = 0; j < i; j++)
      S[i][j] = -S[j][i];

  return;
}

void
ut_mat_sqrt (double **A, int size, double **B)
{
  int i;
  double *eval = ut_alloc_1d (size);
  double **evect = ut_alloc_2d (size, size);
  double **Lambda = ut_alloc_2d (size, size);
  double **tmp = ut_alloc_2d (size, size);
  double **Q = ut_alloc_2d (size, size);
  double **Qinv = ut_alloc_2d (size, size);

  ut_mat_eigen (A, size, eval, evect);

  ut_mat_transpose (evect, size, size, Q);
  for (i = 0; i < size; i++)
    Lambda[i][i] = sqrt (eval[i]);

  ut_mat_inverse (Q, size, Qinv);

  ut_mat_product (Q, size, size, Lambda, size, size, tmp);
  ut_mat_product (tmp, size, size, Qinv, size, size, B);

  ut_free_1d (&eval);
  ut_free_2d (&evect, size);
  ut_free_2d (&Q, size);
  ut_free_2d (&Qinv, size);
  ut_free_2d (&Lambda, size);
  ut_free_2d (&tmp, size);

  return;
}

void
ut_mat_log (double **A, int size, double **B)
{
  int i;
  double *eval = ut_alloc_1d (size);
  double **evect = ut_alloc_2d (size, size);
  double **Lambda = ut_alloc_2d (size, size);
  double **tmp = ut_alloc_2d (size, size);
  double **Q = ut_alloc_2d (size, size);
  double **Qinv = ut_alloc_2d (size, size);

  ut_mat_eigen (A, size, eval, evect);

  ut_mat_transpose (evect, size, size, Q);
  for (i = 0; i < size; i++)
    Lambda[i][i] = log (eval[i]);

  ut_mat_inverse (Q, size, Qinv);

  ut_mat_product (Q, size, size, Lambda, size, size, tmp);
  ut_mat_product (tmp, size, size, Qinv, size, size, B);

  ut_free_1d (&eval);
  ut_free_2d (&evect, size);
  ut_free_2d (&Q, size);
  ut_free_2d (&Qinv, size);
  ut_free_2d (&Lambda, size);
  ut_free_2d (&tmp, size);

  return;
}

void
ut_mat_exp (double **A, int size, double **B)
{
  int i;
  double *eval = ut_alloc_1d (size);
  double **evect = ut_alloc_2d (size, size);
  double **Lambda = ut_alloc_2d (size, size);
  double **tmp = ut_alloc_2d (size, size);
  double **Q = ut_alloc_2d (size, size);
  double **Qinv = ut_alloc_2d (size, size);

  ut_mat_eigen (A, size, eval, evect);

  ut_mat_transpose (evect, size, size, Q);
  for (i = 0; i < size; i++)
    Lambda[i][i] = exp (eval[i]);

  ut_mat_inverse (Q, size, Qinv);

  ut_mat_product (Q, size, size, Lambda, size, size, tmp);
  ut_mat_product (tmp, size, size, Qinv, size, size, B);

  ut_free_1d (&eval);
  ut_free_2d (&evect, size);
  ut_free_2d (&Q, size);
  ut_free_2d (&Qinv, size);
  ut_free_2d (&Lambda, size);
  ut_free_2d (&tmp, size);

  return;
}

void
ut_mat_polar (double **A, int size1, int size2, double **Q, double **S)
{
  double **At = ut_alloc_2d (size2, size1);
  double **AtA = ut_alloc_2d (size2, size2);
  double **Sinv = ut_alloc_2d (size2, size2);

  ut_mat_transpose (A, size1, size2, At);
  ut_mat_product (At, size2, size1, A, size1, size2, AtA);

  ut_mat_sqrt (AtA, size2, S);

  ut_mat_inverse (S, size2, Sinv);

  ut_mat_product (A, size1, size2, Sinv, size2, size2, Q);

  ut_free_2d (&At, size2);
  ut_free_2d (&AtA, size2);

  return;
}

void
ut_mat_dev (double **A, int size, double **B)
{
  int i;
  double tr;

  ut_mat_trace (A, size, &tr);

  ut_array_2d_memcpy (A, size, size, B);

  for (i = 0; i < size; i++)
    B[i][i] -= tr / size;

  return;
}
