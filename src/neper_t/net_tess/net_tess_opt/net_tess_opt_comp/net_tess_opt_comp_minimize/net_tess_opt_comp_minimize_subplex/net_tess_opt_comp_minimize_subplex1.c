/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

/* The function net_tess_opt_comp_minimize_subplex is derived from NLopt
 * (contrib/nlopt/src/algs/neldermead/sbplx.c), whose copyright and permission notice follow.
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


/* subplex strategy constants: */
static const double psi = 0.25, omega = 0.1;

// Minimizing the objective function from (*pTOpt).x with the subplex
// algorithm (from NLopt's sbplx_minimize), with the bounds, stopping
// criteria and initial step of *pTOpt.  As nlopt_optimize does for
// NLOPT_LN_SBPLX, the variables of equal lower and upper bounds are fixed
// (eliminated), and the algorithm is run on the other (free) variables.
// The subspaces are set by net_tess_opt_comp_minimize_subplex_subspaces_rowan
// (Rowan's method), net_tess_opt_comp_minimize_subplex_subspaces_seed (one
// per seed) or net_tess_opt_comp_minimize_subplex_subspaces_percellerror
// (one per seed, by decreasing cell error), depending on
// (*pTOpt).subplexsubspaces.
// val, itermax, time, nlopt_[r]eps and x[r]eps are tested by the
// algorithm, and the other stopping criteria by
// net_tess_opt_comp_minimize_objective_stop, after each evaluation of the
// objective function.  On return, (*pTOpt).result is the result of the
// optimization.
void
net_tess_opt_comp_minimize_subplex (struct TOPT *pTOpt)
{
  const char *ret;
  int i, n;
  double minf;
  double *x = NULL, *lb = NULL, *ub = NULL, *xstep0 = NULL;
  double *xstep = NULL, *xprev, *dx, *xs, *lbs, *ubs, *xsstep, *scratch;
  int *p = NULL;                /* indices of x, by subspace */
  int *ptmp = NULL;
  int *sub = NULL;              /* positions of the subspaces in p */
  struct SUBPLEX_FUNC F, *pF = &F;
  struct SUBPLEX_STOP S, *stop = &S;

  /* free variables (x), with their bounds and initial steps, objective
     function and stopping criteria */
  ret =
    net_tess_opt_comp_minimize_subplex_init (pTOpt, pF, stop, &x, &lb,
                                             &ub, &xstep0);
  n = (*pF).freeqty;

  if (strcmp (ret, "success"))
    goto end;

  if (n == 0)
  {
    minf = net_tess_opt_comp_minimize_subplex_f (pF, x);
    goto end;
  }

  minf = net_tess_opt_comp_minimize_subplex_f (pF, x);
  ++(*stop).nevals;
  if (net_tess_opt_comp_minimize_subplex_stop_forced (stop))
  {
    ret = "forced_stop";
    goto end;
  }
  if (minf < (*stop).minf_max)
  {
    ret = "stopval";
    goto end;
  }
  if (net_tess_opt_comp_minimize_subplex_stop_evals (stop))
  {
    ret = "maxeval";
    goto end;
  }
  if (net_tess_opt_comp_minimize_subplex_stop_time (stop))
  {
    ret = "maxtime";
    goto end;
  }

  xstep = (double *) malloc (sizeof (double) * (n * 3 + SUBPLEX_NSMAX * 4
                                                + (SUBPLEX_NSMAX +
                                                   1) * (SUBPLEX_NSMAX + 1) +
                                                2 * SUBPLEX_NSMAX));
  if (!xstep)
  {
    ret = "out_of_memory";
    goto end;
  }
  xprev = xstep + n;
  dx = xprev + n;
  xs = dx + n;
  xsstep = xs + SUBPLEX_NSMAX;
  lbs = xsstep + SUBPLEX_NSMAX;
  ubs = lbs + SUBPLEX_NSMAX;
  scratch = ubs + SUBPLEX_NSMAX;
  p = (int *) malloc (sizeof (int) * n);
  ptmp = (int *) malloc (sizeof (int) * n);
  sub = (int *) malloc (sizeof (int) * (n + 1));
  if (!p || !ptmp || !sub)
  {
    ret = "out_of_memory";
    goto done;
  }

  memcpy (xstep, xstep0, n * sizeof (double));
  memset (dx, 0, n * sizeof (double));

  while (1)
  {
    int s, ns, is, k, nsubs;
    double fdiff, fdiff_max = 0;

    memcpy (xprev, x, n * sizeof (double));

    /* find the subspaces */
    if (!strcmp ((*pTOpt).subplexsubspaces, "perseed"))
      net_tess_opt_comp_minimize_subplex_subspaces_seed (pF, p, sub, &nsubs);
    else if (!strcmp ((*pTOpt).subplexsubspaces, "percellerror"))
      net_tess_opt_comp_minimize_subplex_subspaces_percellerror (pF, p, sub,
                                                                 &nsubs);
    else
      net_tess_opt_comp_minimize_subplex_subspaces_rowan (n, dx, p, ptmp, sub,
                                                          &nsubs);

    /* perform nelder-mead on each subspace, of dimension ns, made of
       variables p[is...is+ns-1] */
    for (s = 0; s < nsubs; ++s)
    {
      is = sub[s];
      ns = sub[s + 1] - sub[s];
      for (k = is; k < is + ns; ++k)
      {
        xs[k - is] = x[p[k]];
        xsstep[k - is] = xstep[p[k]];
        lbs[k - is] = lb[p[k]];
        ubs[k - is] = ub[p[k]];
      }
      ret =
        net_tess_opt_comp_minimize_subplex_nldrmd (ns, pF, x, is,
                                                   p, lbs, ubs,
                                                   xs, &minf,
                                                   xsstep, stop,
                                                   psi, scratch, &fdiff);
      if (fdiff > fdiff_max)
        fdiff_max = fdiff;
      for (k = is; k < is + ns; ++k)
        x[p[k]] = xs[k - is];
      if (!strcmp (ret, "failure"))
      {
        ret = "xtol";
        goto done;
      }
      if (strcmp (ret, "xtol"))
        goto done;
    }

    /* termination tests: */
    if (net_tess_opt_comp_minimize_subplex_stop_ftol
        (stop, minf, minf + fdiff_max))
    {
      ret = "ftol";
      goto done;
    }
    if (net_tess_opt_comp_minimize_subplex_stop_x (stop, x, xprev))
    {
      int j;
      /* as explained in Rowan's thesis, it is important
         to check |xstep| as well as |x-xprev|, since if
         the step size is too large (in early iterations),
         the inner Nelder-Mead may not make much progress */
      for (j = 0; j < n; ++j)
        if (fabs (xstep[j]) * psi >
            ((*stop).xtol_abs ? (*stop).xtol_abs[j] : 0)
            && fabs (xstep[j]) * psi > (*stop).xtol_rel * fabs (x[j]))
          break;
      if (j == n)
      {
        ret = "xtol";
        goto done;
      }
    }

    /* compute change in optimal point */
    for (i = 0; i < n; ++i)
      dx[i] = x[i] - xprev[i];

    /* setting stepsizes */
    {
      double scale;
      if (nsubs == 1)
        scale = psi;
      else
      {
        double stepnorm = 0, dxnorm = 0;
        for (i = 0; i < n; ++i)
        {
          stepnorm += fabs (xstep[i]);
          dxnorm += fabs (dx[i]);
        }
        scale = dxnorm / stepnorm;
        if (scale < omega)
          scale = omega;
        if (scale > 1 / omega)
          scale = 1 / omega;
      }
      for (i = 0; i < n; ++i)
        xstep[i] = (dx[i] == 0) ? -(xstep[i] * scale)
          : copysign (xstep[i] * scale, dx[i]);
    }
  }

done:
  free (p);
  free (ptmp);
  free (sub);
  free (xstep);

end:
  net_tess_opt_comp_minimize_subplex_post (pTOpt, ret, pF, stop, &x, &lb, &ub,
                                           &xstep0);

  return;
}
