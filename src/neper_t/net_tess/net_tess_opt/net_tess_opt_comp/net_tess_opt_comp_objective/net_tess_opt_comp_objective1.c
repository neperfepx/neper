/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_.h"

#ifdef HAVE_NLOPT
static int force_stop = 0;
#endif

double
net_tess_opt_comp_objective (unsigned int n, const double *x, double *grad,
                             void *data)
{
  int i, status;
  double val;
  struct TOPT *pTOpt = (struct TOPT *) data;
  char *message = ut_alloc_1d_char (1000);
  struct timeval t1, t2, t3, t4, t5;
  char *fmin = ut_alloc_1d_char (1000);
  char *f = ut_alloc_1d_char (1000);
  (void) grad;
  char *loopnotice = ut_alloc_1d_char (1000);

  if ((*pTOpt).loop > 1)
    sprintf (loopnotice, " (%d %s)", (*pTOpt).loop,
             (*pTOpt).algoname[(*pTOpt).algoid]);
  else
    strcpy (loopnotice, "");

  gettimeofday (&t1, NULL);

  ((*pTOpt).iter)++;

  // In certain cases, the caller may wish to force the optimization to
  // halt, for some reason unknown to NLopt. For example, if the user
  // presses Ctrl-C, or there is an error of some sort in the objective
  // function. (This is used to implement exception handling in the
  // NLopt wrappers for C++ and other languages.) In this case, it is
  // possible to tell NLopt to halt the optimization gracefully,
  // returning the best point found so far, by calling the following
  // function from within your objective or constraint functions:
  // objective_force_stop (opt);
  // This causes objective_optimize to halt, returning the NLOPT_FORCED_STOP
  // error code (below). It has no effect if not called during
  // objective_optimize.
  // If you want to provide a bit more information, you can call
  // objective_set_force_stop (opt, int val)
#ifdef HAVE_NLOPT
  signal (SIGINT, net_tess_opt_forcestop);

  if (force_stop)
  {
    signal (SIGINT, SIG_DFL);
    force_stop = 0;
    nlopt_set_force_stop ((*pTOpt).opt, -5);
  }
#endif

  gettimeofday (&t2, NULL);

  if (!strcmp ((*pTOpt).optitype, "seeds"))
    net_tess_opt_comp_objective_x_seedset (x, pTOpt);
  else if (!strcmp ((*pTOpt).optitype, "crystal"))
    net_tess_opt_comp_objective_x_crystal (x, pTOpt);
  else if (!strcmp ((*pTOpt).optitype, "domain"))
    net_tess_opt_comp_objective_x_domain (x, pTOpt);
  else
    abort ();

  if ((*pTOpt).tarqty > 0 && !strcmp ((*pTOpt).tarvar[0], "centroid")
      && !strcmp ((*pTOpt).tarexpr[0], "seed"))
    net_tess_opt_comp_objective_centroidal_update (pTOpt);

  gettimeofday (&t3, NULL);

  if (!strcmp ((*pTOpt).optitype, "seeds")
      || !strcmp ((*pTOpt).optitype, "domain"))
    status = net_tess_opt_comp_objective_poly (pTOpt);
  else
    status = 0;

  gettimeofday (&t4, NULL);

  if (status == 0)
    net_tess_opt_comp_objective_fval (pTOpt);
  else
    (*pTOpt).objval = 10;

  gettimeofday (&t5, NULL);

  // must be before neut_topt_[r]eps
  if (n > 0 && neut_topt_plateau (pTOpt))
  {
#ifdef HAVE_NLOPT
    nlopt_set_force_stop ((*pTOpt).opt, -100 - neut_topt_plateau (pTOpt));
#endif
    printf ("\n");
    strcpy ((*pTOpt).message, "");
    ut_print_message (0, 3, "");
  }

  if (n > 0 && (*pTOpt).eps >= 0)
  {
    status = neut_topt_eps (*pTOpt, &val);
    if (status == 0 && val < (*pTOpt).eps)
    {
#ifdef HAVE_NLOPT
      nlopt_set_force_stop ((*pTOpt).opt, -6);
#endif
    }
  }

  if (n > 0 && (*pTOpt).reps >= 0)
  {
    status = neut_topt_reps (*pTOpt, &val);
    if (status == 0 && val < (*pTOpt).reps)
    {
#ifdef HAVE_NLOPT
      nlopt_set_force_stop ((*pTOpt).opt, -7);
#endif
    }
  }

  if (n > 0 && (*pTOpt).loopmax >= 0)
  {
    if ((*pTOpt).loop > (*pTOpt).loopmax)
    {
#ifdef HAVE_NLOPT
      nlopt_set_force_stop ((*pTOpt).opt, -8);
#endif
    }
  }

  if ((*pTOpt).tarqty > 0 && (*pTOpt).iter == 1)
  {
    sprintf (message, "Initial solution: f   =%.9f", (*pTOpt).objval);
    ut_print_progress (stdout, 1, INT_MAX, message, (*pTOpt).message);
    if ((*pTOpt).itermax != 1)
    {
      printf ("\n");
      strcpy ((*pTOpt).message, "");
      ut_print_message (0, 3, "");
    }
  }

  if ((*pTOpt).iter <= (*pTOpt).itermax)
  {
    char *blank = ut_alloc_1d_char (10);

    if ((*pTOpt).objvalmin[(*pTOpt).iter] < 1e12)
      sprintf (fmin, "=%.9f", (*pTOpt).objvalmin[(*pTOpt).iter]);
    else
      sprintf (fmin, ">1e12");

    if ((*pTOpt).objval < 0.1 * 1e12)
      sprintf (f, "=%.9f", (*pTOpt).objval);
    else
      sprintf (f, ">1e12");

    if ((*pTOpt).iter < 1e6)
    {
      for (i = 0; i < 6 - ut_num_tenlen_int ((*pTOpt).iter); i++)
        blank = strcat (blank, " ");

      sprintf (message, "Iteration %s%d: fmin%s f%s%s", blank, (*pTOpt).iter,
               fmin, f, loopnotice);
    }
    else
    {
      for (i = 0; i < 11 - ut_num_tenlen_int ((*pTOpt).iter); i++)
        blank = strcat (blank, " ");
      sprintf (message, "Iter %s%d: fmin=%s f=%s%s", blank, (*pTOpt).iter,
               fmin, f, loopnotice);
    }
    ut_free_1d_char (&blank);
  }

  else
  {
    printf ("\n");
    ut_print_message (0, 3, "");
    sprintf (message, "Final   solution: f   =%.9f", (*pTOpt).objval);
    strcpy ((*pTOpt).message, "");
  }

  if ((*pTOpt).tarqty > 0)
    ut_print_progress (stdout, 1, INT_MAX, message, (*pTOpt).message);

  (*pTOpt).TDyn.var_dur = ut_time_subtract (&((*pTOpt).end_time), &t1);
  (*pTOpt).TDyn.seed_dur = ut_time_subtract (&t2, &t3);
  (*pTOpt).TDyn.val_dur = ut_time_subtract (&t4, &t5);
  (*pTOpt).TDyn.total_dur = ut_time_subtract (&(*pTOpt).end_time, &t5);
  (*pTOpt).TDyn.cumtotal_dur += (*pTOpt).TDyn.total_dur;

  net_tess_opt_comp_objective_log (*pTOpt);

  ut_free_1d_char (&message);

  // Setting t5 of the next iteration
  gettimeofday (&(*pTOpt).end_time, NULL);

#ifdef DEVEL_DEBUGGING_TEST
  net_tess_opt_comp_objective_debugtest (*pTOpt);
#endif

  ut_free_1d_char (&fmin);
  ut_free_1d_char (&f);
  ut_free_1d_char (&loopnotice);

  return (*pTOpt).objval;
}

#ifdef HAVE_NLOPT

void
net_tess_opt_forcestop (int sig)
{
  if (sig != SIGINT)
    abort ();

  force_stop = 1;

  return;
}

#endif
