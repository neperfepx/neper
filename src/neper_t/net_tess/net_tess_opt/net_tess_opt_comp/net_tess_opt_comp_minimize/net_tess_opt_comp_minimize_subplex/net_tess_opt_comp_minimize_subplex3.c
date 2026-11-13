/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

/* The functions of this file, except
 * net_tess_opt_comp_minimize_subplex_seconds and
 * net_tess_opt_comp_minimize_subplex_suberr_cmp, are derived from NLopt
 * (contrib/nlopt/src/algs/neldermead/nldrmd.c and sbplx.c, and
 * contrib/nlopt/src/util/stop.c), whose copyright and permission notice follow.
 *
 * Copyright (c) 2007-2014 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "net_tess_opt_comp_minimize_subplex_.h"


/* The simplex points are sorted by f(x), the tie-breaker being their
   position in pts (as in NLopt, where they are stored in a red-black
   tree).  order[0] is the best point and order[n] the worst one. */
void
net_tess_opt_comp_minimize_subplex_nldrmd_sort (const double *pts,
                                                int n, int *order)
{
  int i, j, tmp;

  for (i = 1; i < n + 1; i++)
    for (j = i;
         j > 0
         &&
         net_tess_opt_comp_minimize_subplex_nldrmd_compare (pts,
                                                            n,
                                                            order
                                                            [j -
                                                             1],
                                                            order
                                                            [j]) > 0; j--)
    {
      tmp = order[j];
      order[j] = order[j - 1];
      order[j - 1] = tmp;
    }

  return;
}

/* return 1 if a and b are approximately equal relative to floating-point
   precision, 0 otherwise */
int
net_tess_opt_comp_minimize_subplex_nldrmd_close (double a, double b)
{
  return (fabs (a - b) <= 1e-13 * (fabs (a) + fabs (b)));
}

/* Perform the reflection xnew = c + scale * (c - xold),
   returning 0 if xnew == c or xnew == xold (coincident points), 1 otherwise.

   The reflected point xnew is "pinned" to the lower and upper bounds
   (lb and ub), as suggested by J. A. Richardson and J. L. Kuester,
   "The complex method for constrained optimization," Commun. ACM
   16(8), 487-489 (1973).  This is probably a suboptimal way to handle
   bound constraints, but I don't know a better way.  The main danger
   with this is that the simplex might collapse into a
   lower-dimensional hyperplane; this danger can be ameliorated by
   restarting (as in subplex), however. */
int
net_tess_opt_comp_minimize_subplex_nldrmd_reflectpt (int n,
                                                     double *xnew,
                                                     const double
                                                     *c,
                                                     double scale,
                                                     const double
                                                     *xold,
                                                     const double
                                                     *lb, const double *ub)
{
  int equalc = 1, equalold = 1, i;
  for (i = 0; i < n; ++i)
  {
    double newx = c[i] + scale * (c[i] - xold[i]);
    if (newx < lb[i])
      newx = lb[i];
    if (newx > ub[i])
      newx = ub[i];
    equalc = equalc
      && net_tess_opt_comp_minimize_subplex_nldrmd_close (newx, c[i]);
    equalold = equalold
      && net_tess_opt_comp_minimize_subplex_nldrmd_close (newx, xold[i]);
    xnew[i] = newx;
  }
  return !(equalc || equalold);
}

/* objective function of the subspace variables xs */
double
net_tess_opt_comp_minimize_subplex_nldrmd_f (int n,
                                             const double *xs,
                                             struct SUBPLEX_FUNC
                                             *pF, double *xsub,
                                             int is, const int *p)
{
  int i;

  for (i = is; i < is + n; ++i)
    xsub[p[i]] = xs[i - is];

  return net_tess_opt_comp_minimize_subplex_f (pF, xsub);
}

int
net_tess_opt_comp_minimize_subplex_stop_ftol_relstop (double vold,
                                                      double vnew,
                                                      double
                                                      reltol, double abstol)
{
  if (isinf (vold))
    return 0;
  return (fabs (vnew - vold) < abstol
          || fabs (vnew - vold) < reltol * (fabs (vnew) + fabs (vold)) * 0.5
          || (reltol > 0 && vnew == vold));
}

/* sorting the indices p into the delta-x array by decreasing |dx|; the
   sort is stable (merge sort), so that indices of equal |dx| keep their
   order, whatever the platform */
void
net_tess_opt_comp_minimize_subplex_subspaces_sort (int *p, int n,
                                                   const double *dx, int *tmp)
{
  int i, j, k, m;

  if (n < 2)
    return;

  m = n / 2;
  net_tess_opt_comp_minimize_subplex_subspaces_sort (p, m, dx, tmp);
  net_tess_opt_comp_minimize_subplex_subspaces_sort (p + m, n - m, dx, tmp);

  i = 0;
  j = m;
  k = 0;
  while (i < m && j < n)
    tmp[k++] = (fabs (dx[p[j]]) > fabs (dx[p[i]])) ? p[j++] : p[i++];
  while (i < m)
    tmp[k++] = p[i++];
  while (j < n)
    tmp[k++] = p[j++];

  memcpy (p, tmp, n * sizeof (int));

  return;
}

double
net_tess_opt_comp_minimize_subplex_seconds (void)
{
  struct timeval tv;

  gettimeofday (&tv, NULL);

  return tv.tv_sec + 1.e-6 * tv.tv_usec;
}

// by decreasing error, and by increasing index for equal errors
int
net_tess_opt_comp_minimize_subplex_suberr_cmp (const void *a, const void *b)
{
  const struct SUBPLEX_SUBERR *A = (const struct SUBPLEX_SUBERR *) a;
  const struct SUBPLEX_SUBERR *B = (const struct SUBPLEX_SUBERR *) b;

  if ((*A).err != (*B).err)
    return ((*A).err < (*B).err) ? 1 : -1;
  else
    return (*A).s - (*B).s;
}
