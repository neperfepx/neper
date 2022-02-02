/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_nlopt_init_.h"

#ifdef HAVE_NLOPT
void
net_tess_opt_comp_nlopt_init_objective (struct TOPT *pTOpt)
{
  nlopt_set_min_objective ((*pTOpt).opt, net_tess_opt_comp_objective, pTOpt);

  return;
}

void
net_tess_opt_comp_nlopt_init_bounds (struct TOPT *pTOpt)
{
  nlopt_set_lower_bounds ((*pTOpt).opt, (*pTOpt).boundl);
  nlopt_set_upper_bounds ((*pTOpt).opt, (*pTOpt).boundu);

  return;
}

void
net_tess_opt_comp_nlopt_init_stop (struct TOPT *pTOpt)
{
  // Stop when an objective value of at least stopval is found: stop
  // minimizing when an objective value ≤ stopval is found, or stop
  // maximizing a value ≥ stopval is found. (Setting stopval to
  // -HUGE_VAL for minimizing or +HUGE_VAL for maximizing disables
  // this stopping criterion.)
  nlopt_set_stopval ((*pTOpt).opt, (*pTOpt).val);

  // Set relative tolerance on function value: stop when an
  // optimization step (or an estimate of the optimum) changes the
  // objective function value by less than tol multiplied by the
  // absolute value of the function value. (If there is any chance
  // that your optimum function value is close to zero, you might
  // want to set an absolute tolerance with nlopt_set_ftol_abs as
  // well.) Criterion is disabled if tol is non-positive.
  nlopt_set_ftol_rel ((*pTOpt).opt, (*pTOpt).nlopt_reps);

  // Set absolute tolerance on function value: stop when an
  // optimization step (or an estimate of the optimum) changes the
  // function value by less than tol. Criterion is disabled if tol is
  // non-positive.
  nlopt_set_ftol_abs ((*pTOpt).opt, (*pTOpt).nlopt_eps);

  // Set relative tolerance on optimization parameters: stop when an
  // optimization step (or an estimate of the optimum) changes every
  // parameter by less than tol multiplied by the absolute value of
  // the parameter. (If there is any chance that an optimal
  // parameter is close to zero, you might want to set an absolute
  // tolerance with nlopt_set_xtol_abs as well.) Criterion is
  // disabled if tol is non-positive.
  nlopt_set_xtol_rel ((*pTOpt).opt, (*pTOpt).xreps);

  // Set absolute tolerances on optimization parameters. tol is a
  // pointer to an array of length n (the dimension from
  // nlopt_create) giving the tolerances: stop when an optimization
  // step (or an estimate of the optimum) changes every parameter
  // x[i] by less than tol[i]. (Note that this function makes a copy
  // of the tol array, so subsequent changes to the caller's tol
  // have no effect on opt.) In nlopt_get_xtol_abs, tol must be an
  // array of length n, which upon successful return contains a copy
  // of the current tolerances.
  // nlopt_set_xtol_abs (opt, const double* tol);
  // For convenience, the following function may be used to set the
  // absolute tolerances in all n optimization parameters to the
  // same value:
  nlopt_set_xtol_abs1 ((*pTOpt).opt, (*pTOpt).xeps);

  // For derivative-free local-optimization algorithms, the
  // optimizer must somehow decide on some initial step size to
  // perturb x by when it begins the optimization. This step size
  // should be big enough that the value of the objective changes
  // significantly, but not too big if you want to find the local
  // optimum nearest to x. By default, NLopt chooses this initial
  // step size heuristically from the bounds, tolerances, and other
  // information, but this may not always be the best choice.
  // You can modify the initial step size by calling:
  // nlopt_set_initial_step (nlopt_opt opt, const double* dx);
  // Here, dx is an array of length n (the dimension of the problem
  // from nlopt_create) containing the (nonzero) initial step size
  // for each component of the optimization parameters x. If you
  // pass NULL for dx, then NLopt will use its heuristics to
  // determine the initial step size. For convenience, if you want
  // to set the step sizes in every direction to be the same value,
  // you can instead call:
  nlopt_set_initial_step1 ((*pTOpt).opt, (*pTOpt).inistep);

  // Stop when the number of function evaluations exceeds maxeval.
  // (This is not a strict maximum: the number of function
  // evaluations may exceed maxeval slightly, depending upon the
  // algorithm.) Criterion is disabled if maxeval is non-positive.
  nlopt_set_maxeval ((*pTOpt).opt, (*pTOpt).itermax);

  // Stop when the optimization time (in seconds) exceeds maxtime.
  // (This is not a strict maximum: the time may exceed maxtime
  // slightly, depending upon the algorithm and on how slow your
  // function evaluation is.) Criterion is disabled if maxtime is
  // non-positive.
  nlopt_set_maxtime ((*pTOpt).opt, (*pTOpt).time);

  return;
}

#endif
