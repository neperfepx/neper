/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

/* The functions net_tess_opt_comp_minimize_subplex_subspaces_rowan,
 * net_tess_opt_comp_minimize_subplex_nldrmd and
 * net_tess_opt_comp_minimize_subplex_stop_* are derived from NLopt
 * (contrib/nlopt/src/algs/neldermead/sbplx.c and nldrmd.c, and
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


// Setting the free variables (x), with their bounds (lb and ub) and
// initial steps (dx), the objective function of the free variables (*pF)
// and the stopping criteria (*stop).  Returns "invalid_args" if the
// initial solution is out of bounds, "success" otherwise.
const char *
net_tess_opt_comp_minimize_subplex_init (struct TOPT *pTOpt,
                                         struct SUBPLEX_FUNC *pF,
                                         struct SUBPLEX_STOP *stop,
                                         double **px, double **plb,
                                         double **pub, double **pdx)
{
  int i, j, n = (*pTOpt).xqty, n0;

  ut_free_1d_char (&(*pTOpt).result);

  (*pF).pTOpt = pTOpt;
  (*pF).n = n;
  (*pF).x = ut_alloc_1d (n);
  (*pF).free = ut_alloc_1d_int (n);

  // free variables, and fixed variables set to their bounds
  (*pF).freeqty = 0;
  for (i = 0; i < n; i++)
    if ((*pTOpt).boundl[i] != (*pTOpt).boundu[i])
      (*pF).free[(*pF).freeqty++] = i;
    else
      (*pF).x[i] = (*pTOpt).boundl[i];
  n0 = (*pF).freeqty;

  (*px) = ut_alloc_1d (n0);
  (*plb) = ut_alloc_1d (n0);
  (*pub) = ut_alloc_1d (n0);
  (*pdx) = ut_alloc_1d (n0);
  (*stop).xtol_abs = ut_alloc_1d (n0);
  for (j = 0; j < n0; j++)
  {
    i = (*pF).free[j];
    (*px)[j] = (*pTOpt).x[i];
    (*plb)[j] = (*pTOpt).boundl[i];
    (*pub)[j] = (*pTOpt).boundu[i];
    (*pdx)[j] = (*pTOpt).inistep;
    (*stop).xtol_abs[j] = (*pTOpt).xeps;
  }

  (*stop).n = n0;
  (*stop).minf_max = (*pTOpt).val;
  (*stop).ftol_rel = (*pTOpt).nlopt_reps;
  (*stop).ftol_abs = (*pTOpt).nlopt_eps;
  (*stop).xtol_rel = (*pTOpt).xreps;
  (*stop).nevals = 0;
  (*stop).maxeval = (*pTOpt).itermax;
  (*stop).maxtime = (*pTOpt).time;
  (*stop).start = net_tess_opt_comp_minimize_subplex_seconds ();
  (*stop).pTOpt = pTOpt;

  for (j = 0; j < n0; j++)
    if ((*plb)[j] > (*pub)[j] || (*px)[j] < (*plb)[j] || (*px)[j] > (*pub)[j])
      return "invalid_args";

  return "success";
}

// Setting the result of the optimization (unless it is a forced stop,
// whose reason is already set), setting the variables ((*pTOpt).x) to the
// solution (the free variables to x, the fixed variables to their bounds),
// unless the arguments were invalid, and freeing memory
void
net_tess_opt_comp_minimize_subplex_post (struct TOPT *pTOpt, const char *ret,
                                         struct SUBPLEX_FUNC *pF,
                                         struct SUBPLEX_STOP *stop,
                                         double **px, double **plb,
                                         double **pub, double **pdx)
{
  int i, j;

  if (strcmp (ret, "forced_stop"))
    ut_string_string (ret, &(*pTOpt).result);

  if (strcmp (ret, "invalid_args"))
  {
    for (i = 0; i < (*pF).n; i++)
      if ((*pTOpt).boundl[i] == (*pTOpt).boundu[i])
        (*pTOpt).x[i] = (*pTOpt).boundl[i];
    for (j = 0; j < (*pF).freeqty; j++)
      (*pTOpt).x[(*pF).free[j]] = (*px)[j];
  }

  ut_free_1d (px);
  ut_free_1d (plb);
  ut_free_1d (pub);
  ut_free_1d (pdx);
  ut_free_1d (&(*stop).xtol_abs);
  ut_free_1d (&(*pF).x);
  ut_free_1d_int (&(*pF).free);

  return;
}

// objective function of the free variables x: evaluation, test of the
// stopping criteria, and report
double
net_tess_opt_comp_minimize_subplex_f (struct SUBPLEX_FUNC *pF,
                                      const double *x)
{
  int j;
  const char *stop;
  struct TOPT *pTOpt = (*pF).pTOpt;

  for (j = 0; j < (*pF).freeqty; j++)
    (*pF).x[(*pF).free[j]] = x[j];

  net_tess_opt_comp_minimize_objective ((*pF).x, pTOpt);

  stop = net_tess_opt_comp_minimize_objective_stop (pTOpt);
  if (stop)
    ut_string_string (stop, &(*pTOpt).result);

  net_tess_opt_comp_minimize_objective_report (pTOpt);

  return (*pTOpt).objval;
}

// Setting the subspaces of the n free variables, on which Nelder-Mead is
// run in turn, from the progress vector dx (the change of the variables
// over the last subplex iteration, 0 initially), as in Rowan's subplex
// (and NLopt's sbplx_minimize): the variables are sorted by decreasing
// |dx|, and partitioned in subspaces of SUBPLEX_NSMIN to SUBPLEX_NSMAX
// variables, at sudden drops of the average |dx|.  On output, p holds the
// indices of the variables, by subspace, *pnsubs is the number of
// subspaces, and subspace s is made of variables p[sub[s]...sub[s+1]-1].
// tmp is a scratch array of length n.
void
net_tess_opt_comp_minimize_subplex_subspaces_rowan (int n, const double *dx,
                                                    int *p, int *tmp, int *sub,
                                                    int *pnsubs)
{
  int i, k, nk, ns;
  double normi = 0, normdx = 0, ns_goodness, norm, goodness;

  /* sort indices into the progress vector dx by decreasing
     order of magnitude |dx| */
  for (i = 0; i < n; ++i)
    p[i] = i;
  net_tess_opt_comp_minimize_subplex_subspaces_sort (p, n, dx, tmp);

  for (i = 0; i < n; ++i)
    normdx += fabs (dx[i]);     /* L1 norm */

  (*pnsubs) = 0;
  for (i = 0; i + SUBPLEX_NSMIN < n; i += ns)
  {
    /* find subspace starting at index i */
    ns_goodness = -HUGE_VAL;
    norm = normi;
    nk = i + SUBPLEX_NSMAX > n ? n : i + SUBPLEX_NSMAX; /* max k for this subspace */
    for (k = i; k < i + SUBPLEX_NSMIN - 1; ++k)
      norm += fabs (dx[p[k]]);
    ns = SUBPLEX_NSMIN;
    for (k = i + SUBPLEX_NSMIN - 1; k < nk; ++k)
    {
      norm += fabs (dx[p[k]]);
      /* remaining subspaces must be big enough to partition */
      if (n - (k + 1) < SUBPLEX_NSMIN)
        continue;
      /* maximize figure of merit defined by Rowan thesis:
         look for sudden drops in average |dx| */
      if (k + 1 < n)
        goodness = norm / (k + 1) - (normdx - norm) / (n - (k + 1));
      else
        goodness = normdx / n;
      if (goodness > ns_goodness)
      {
        ns_goodness = goodness;
        ns = (k + 1) - i;
      }
    }
    for (k = i; k < i + ns; ++k)
      normi += fabs (dx[p[k]]);
    sub[(*pnsubs)++] = i;
  }

  /* last subspace */
  sub[(*pnsubs)++] = i;
  sub[*pnsubs] = n;

  return;
}

// Setting the subspaces of the free variables, one per seed: the free
// variables of a seed (which are consecutive) form a subspace (split in
// subspaces of SUBPLEX_NSMAX variables, if needed), in the order of the
// variables.  The output is as for
// net_tess_opt_comp_minimize_subplex_subspaces_rowan.
void
net_tess_opt_comp_minimize_subplex_subspaces_seed (struct SUBPLEX_FUNC *pF,
                                                   int *p, int *sub,
                                                   int *pnsubs)
{
  int i, n = (*pF).freeqty, *x_seed = (*(*pF).pTOpt).x_seed;

  (*pnsubs) = 0;
  for (i = 0; i < n; i++)
  {
    p[i] = i;
    if (i == 0 || x_seed[(*pF).free[i]] != x_seed[(*pF).free[i - 1]]
        || i - sub[(*pnsubs) - 1] == SUBPLEX_NSMAX)
      sub[(*pnsubs)++] = i;
  }
  sub[*pnsubs] = n;

  return;
}

// Setting the subspaces of the free variables, one per seed (see
// net_tess_opt_comp_minimize_subplex_subspaces_seed), ordered by decreasing
// total error of the cells of the seeds ((*pTOpt).curcelltotalerror, from
// the last evaluation of the objective function), the subspaces of equal
// errors keeping the order of the seeds.  The output is as for
// net_tess_opt_comp_minimize_subplex_subspaces_rowan.
void
net_tess_opt_comp_minimize_subplex_subspaces_percellerror (struct SUBPLEX_FUNC
                                                           *pF, int *p,
                                                           int *sub,
                                                           int *pnsubs)
{
  int s, i, k, seed, n = (*pF).freeqty;
  struct TOPT *pTOpt = (*pF).pTOpt;
  struct SUBPLEX_SUBERR *suberr = NULL;
  int *p0 = ut_alloc_1d_int (n), *sub0 = NULL;

  net_tess_opt_comp_minimize_subplex_subspaces_seed (pF, p0, sub, pnsubs);
  sub0 = ut_alloc_1d_int (*pnsubs + 1);
  ut_array_1d_int_memcpy (sub, *pnsubs + 1, sub0);

  suberr = (struct SUBPLEX_SUBERR *) calloc (*pnsubs, sizeof (struct SUBPLEX_SUBERR));
  for (s = 0; s < *pnsubs; s++)
  {
    seed = (*pTOpt).x_seed[(*pF).free[p0[sub0[s]]]];
    suberr[s].s = s;
    suberr[s].err = (*pTOpt).curcelltotalerror[(*pTOpt).SCellCell[seed]];
  }

  qsort (suberr, *pnsubs, sizeof (struct SUBPLEX_SUBERR),
         net_tess_opt_comp_minimize_subplex_suberr_cmp);

  k = 0;
  for (s = 0; s < *pnsubs; s++)
  {
    sub[s] = k;
    for (i = sub0[suberr[s].s]; i < sub0[suberr[s].s + 1]; i++)
      p[k++] = p0[i];
  }
  sub[*pnsubs] = n;

  free (suberr);
  ut_free_1d_int (&p0);
  ut_free_1d_int (&sub0);

  return;
}

/* Nelder-Mead simplex algorithm, used as a subroutine for the Rowan's
   subplex algorithm.  Modified to handle bound constraints ala
   Richardson and Kuester (1973), as mentioned below. */

/* heuristic "strategy" constants: */
static const double alpha = 1, beta = 0.5, gamm = 2, delta = 0.5;

#define CHECK_EVAL(xc,fc)                                                \
  ++(*stop).nevals;                                                    \
  if (net_tess_opt_comp_minimize_subplex_stop_forced (stop)) \
  { ret = "forced_stop"; goto done; }                               \
  if ((fc) <= *minf)                                                    \
  {                                                                     \
    *minf = (fc); memcpy (x, (xc), n * sizeof (double));                \
    if (*minf < (*stop).minf_max)                                      \
    { ret = "stopval"; goto done; }                        \
  }                                                                     \
  if (net_tess_opt_comp_minimize_subplex_stop_evals (stop)) \
  { ret = "maxeval"; goto done; }                           \
  if (net_tess_opt_comp_minimize_subplex_stop_time (stop))  \
  { ret = "maxtime"; goto done; }

/* Version of Nelder-Mead used as a subroutine for the subplex method,
   on the subspace of dimension n of the variables p[is...is+n-1] of
   xsub (the subplex variables).

   *minf should contain the value of f(x)  (so that we don't have to
   re-evaluate f at the starting x).

   The simplex diameter |xl - xh| must be reduced by a factor of psi.

   scratch should contain an array of length >= (n+1)*(n+1) + 2*n,
   used as scratch workspace.

   On output, *fdiff will contain the difference between the high
   and low function values of the last simplex. */
const char *
net_tess_opt_comp_minimize_subplex_nldrmd (int n,
                                           struct SUBPLEX_FUNC
                                           *pF, double *xsub,
                                           int is, const int *p,
                                           const double *lb,
                                           const double *ub,
                                           double *x,
                                           double *minf,
                                           const double *xstep,
                                           struct SUBPLEX_STOP
                                           *stop, double psi,
                                           double *scratch, double *fdiff)
{
  double *pts;                  /* (n+1) x (n+1) array of n+1 points plus function val [0] */
  double *c;                    /* centroid * n */
  double *xcur;                 /* current point */
  int order[SUBPLEX_NSMAX + 1]; /* simplex points sorted by f(x) */
  int i, j;
  double ninv = 1.0 / n;
  const char *ret = "success";
  double init_diam = 0;

  pts = scratch;
  c = scratch + (n + 1) * (n + 1);
  xcur = c + n;

  *fdiff = HUGE_VAL;

  /* initialize the simplex based on the starting xstep */
  memcpy (pts + 1, x, sizeof (double) * n);
  pts[0] = *minf;
  if (*minf < (*stop).minf_max)
  {
    ret = "stopval";
    goto done;
  }
  for (i = 0; i < n; ++i)
  {
    double *pt = pts + (i + 1) * (n + 1);
    memcpy (pt + 1, x, sizeof (double) * n);
    pt[1 + i] += xstep[i];
    if (pt[1 + i] > ub[i])
    {
      if (ub[i] - x[i] > fabs (xstep[i]) * 0.1)
        pt[1 + i] = ub[i];
      else                      /* ub is too close to pt, go in other direction */
        pt[1 + i] = x[i] - fabs (xstep[i]);
    }
    if (pt[1 + i] < lb[i])
    {
      if (x[i] - lb[i] > fabs (xstep[i]) * 0.1)
        pt[1 + i] = lb[i];
      else
      {                         /* lb is too close to pt, go in other direction */
        pt[1 + i] = x[i] + fabs (xstep[i]);
        if (pt[1 + i] > ub[i])  /* go towards further of lb, ub */
          pt[1 + i] =
            0.5 * ((ub[i] - x[i] > x[i] - lb[i] ? ub[i] : lb[i]) + x[i]);
      }
    }
    if (net_tess_opt_comp_minimize_subplex_nldrmd_close (pt[1 + i], x[i]))
    {
      ret = "failure";
      goto done;
    }
    pt[0] =
      net_tess_opt_comp_minimize_subplex_nldrmd_f (n, pt + 1, pF,
                                                   xsub, is, p);
    CHECK_EVAL (pt + 1, pt[0]);
  }

restart:
  for (i = 0; i < n + 1; ++i)
    order[i] = i;
  net_tess_opt_comp_minimize_subplex_nldrmd_sort (pts, n, order);

  while (1)
  {
    int low = order[0], high = order[n];
    double fl = pts[low * (n + 1)], *xl = pts + low * (n + 1) + 1;
    double fh = pts[high * (n + 1)], *xh = pts + high * (n + 1) + 1;
    double fr;

    *fdiff = fh - fl;

    if (init_diam == 0)         /* initialize diam. for psi convergence test */
      for (i = 0; i < n; ++i)
        init_diam += fabs (xl[i] - xh[i]);

    /* compute centroid ... if we cared about the performance of this,
       we could do it iteratively by updating the centroid on
       each step, but then we would have to be more careful about
       accumulation of rounding errors... anyway n is unlikely to
       be very large for Nelder-Mead in practical cases */
    ut_array_1d_zero (c, n);
    for (i = 0; i < n + 1; ++i)
    {
      double *xi = pts + i * (n + 1) + 1;
      if (xi != xh)
        for (j = 0; j < n; ++j)
          c[j] += xi[j];
    }
    for (i = 0; i < n; ++i)
      c[i] *= ninv;

    /* x convergence check: find xcur = max radius from centroid */
    ut_array_1d_zero (xcur, n);
    for (i = 0; i < n + 1; ++i)
    {
      double *xi = pts + i * (n + 1) + 1;
      for (j = 0; j < n; ++j)
      {
        double dx = fabs (xi[j] - c[j]);
        if (dx > xcur[j])
          xcur[j] = dx;
      }
    }
    for (i = 0; i < n; ++i)
      xcur[i] += c[i];
    {
      double diam = 0;
      for (i = 0; i < n; ++i)
        diam += fabs (xl[i] - xh[i]);
      if (diam < psi * init_diam)
      {
        ret = "xtol";
        goto done;
      }
    }

    /* reflection */
    if (!net_tess_opt_comp_minimize_subplex_nldrmd_reflectpt
        (n, xcur, c, alpha, xh, lb, ub))
    {
      ret = "xtol";
      goto done;
    }
    fr =
      net_tess_opt_comp_minimize_subplex_nldrmd_f (n, xcur, pF, xsub, is, p);
    CHECK_EVAL (xcur, fr);

    if (fr < fl)
    {                           /* new best point, expand simplex */
      if (!net_tess_opt_comp_minimize_subplex_nldrmd_reflectpt
          (n, xh, c, gamm, xh, lb, ub))
      {
        ret = "xtol";
        goto done;
      }
      fh =
        net_tess_opt_comp_minimize_subplex_nldrmd_f (n, xh, pF, xsub, is, p);
      CHECK_EVAL (xh, fh);
      if (fh >= fr)
      {                         /* expanding didn't improve */
        fh = fr;
        memcpy (xh, xcur, sizeof (double) * n);
      }
    }
    else if (fr < pts[order[n - 1] * (n + 1)])
    {                           /* accept new point */
      memcpy (xh, xcur, sizeof (double) * n);
      fh = fr;
    }
    else
    {                           /* new worst point, contract */
      double fc;
      if (!net_tess_opt_comp_minimize_subplex_nldrmd_reflectpt
          (n, xcur, c, fh <= fr ? -beta : beta, xh, lb, ub))
      {
        ret = "xtol";
        goto done;
      }
      fc =
        net_tess_opt_comp_minimize_subplex_nldrmd_f (n, xcur, pF,
                                                     xsub, is, p);
      CHECK_EVAL (xcur, fc);
      if (fc < fr && fc < fh)
      {                         /* successful contraction */
        memcpy (xh, xcur, sizeof (double) * n);
        fh = fc;
      }
      else
      {                         /* failed contraction, shrink simplex */
        for (i = 0; i < n + 1; ++i)
        {
          double *pt = pts + i * (n + 1);
          if (pt + 1 != xl)
          {
            if (!net_tess_opt_comp_minimize_subplex_nldrmd_reflectpt
                (n, pt + 1, xl, -delta, pt + 1, lb, ub))
            {
              ret = "xtol";
              goto done;
            }
            pt[0] =
              net_tess_opt_comp_minimize_subplex_nldrmd_f (n,
                                                           pt + 1,
                                                           pF, xsub, is, p);
            CHECK_EVAL (pt + 1, pt[0]);
          }
        }
        goto restart;
      }
    }

    pts[high * (n + 1)] = fh;
    net_tess_opt_comp_minimize_subplex_nldrmd_sort (pts, n, order);
  }

done:
  return ret;
}

// stopping criteria (from NLopt, contrib/nlopt/src/util/stop.c)

int
net_tess_opt_comp_minimize_subplex_stop_ftol (const struct
                                              SUBPLEX_STOP *s,
                                              double f, double oldf)
{
  return net_tess_opt_comp_minimize_subplex_stop_ftol_relstop
    (oldf, f, (*s).ftol_rel, (*s).ftol_abs);
}

int
net_tess_opt_comp_minimize_subplex_stop_x (const struct
                                           SUBPLEX_STOP *s,
                                           const double *x,
                                           const double *oldx)
{
  int i;
  double diffnorm = 0, norm = 0;

  for (i = 0; i < (*s).n; i++)
  {
    diffnorm += fabs (x[i] - oldx[i]);
    norm += fabs (x[i]);
  }

  if (diffnorm < (*s).xtol_rel * norm)
    return 1;
  if (!(*s).xtol_abs)
    return 0;
  for (i = 0; i < (*s).n; ++i)
    if (fabs (x[i] - oldx[i]) >= (*s).xtol_abs[i])
      return 0;
  return 1;
}

int
net_tess_opt_comp_minimize_subplex_stop_evals (const struct SUBPLEX_STOP *s)
{
  return ((*s).maxeval > 0 && (*s).nevals >= (*s).maxeval);
}

int
net_tess_opt_comp_minimize_subplex_stop_time (const struct SUBPLEX_STOP *s)
{
  return ((*s).maxtime > 0
          && net_tess_opt_comp_minimize_subplex_seconds () - (*s).start >=
          (*s).maxtime);
}

// stop requested by net_tess_opt_comp_minimize_objective_stop
int
net_tess_opt_comp_minimize_subplex_stop_forced (const struct SUBPLEX_STOP *s)
{
  return (*(*s).pTOpt).result != NULL;
}
