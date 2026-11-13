/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_.h"

// Evaluating the objective function at x (the iteration number is
// incremented)
double
net_tess_opt_comp_minimize_objective (const double *x, struct TOPT *pTOpt)
{
  int status;
  struct timeval t1, t2, t3, t4, t5;

  gettimeofday (&t1, NULL);

  ((*pTOpt).iter)++;

  gettimeofday (&t2, NULL);

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_comp_minimize_objective_x_morpho (x, pTOpt);
  else if (!strcmp ((*pTOpt).optitype, "ori"))
    net_tess_opt_comp_minimize_objective_x_ori (x, pTOpt);
  else
    abort ();

  if ((*pTOpt).tarqty > 0 && !strcmp ((*pTOpt).tarvar[0], "centroid")
      && !strcmp ((*pTOpt).tarexpr[0], "seed"))
    net_tess_opt_comp_minimize_objective_centroidal_update (pTOpt);

  gettimeofday (&t3, NULL);

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    status = net_tess_opt_comp_minimize_objective_poly (pTOpt);
  // nothing to do for ori
  else
    status = 0;

  gettimeofday (&t4, NULL);

  if (status == 0)
    net_tess_opt_comp_minimize_objective_fval (pTOpt);
  else
    (*pTOpt).objval = 10;

  gettimeofday (&t5, NULL);

  (*pTOpt).TDyn.var_dur = ut_time_subtract (&((*pTOpt).end_time), &t1);
  (*pTOpt).TDyn.seed_dur = ut_time_subtract (&t2, &t3);
  (*pTOpt).TDyn.val_dur = ut_time_subtract (&t4, &t5);
  (*pTOpt).TDyn.total_dur = ut_time_subtract (&(*pTOpt).end_time, &t5);
  (*pTOpt).TDyn.cumtotal_dur += (*pTOpt).TDyn.total_dur;
  if (!strcmp ((*pTOpt).optitype, "ori"))
    (*pTOpt).TDyn.iter++;

  return (*pTOpt).objval;
}

// Testing the stopping criteria that are not handled by the optimization
// algorithm (Ctrl+C, eps and reps), after an evaluation of the objective
// function.  Returns NULL if the optimization should go on, or the reason
// of the stop otherwise ("ctrlc", "eps" or "reps"; if several criteria
// are met, the last one).
const char *
net_tess_opt_comp_minimize_objective_stop (struct TOPT *pTOpt)
{
  int status;
  const char *stop = NULL;
  double val;

  if (net_tess_opt_comp_minimize_objective_stop_ctrlc ())
    stop = "ctrlc";

  if ((*pTOpt).eps >= 0)
  {
    status = neut_topt_eps (*pTOpt, &val);
    if (status == 0 && val < (*pTOpt).eps)
      stop = "eps";
  }

  if ((*pTOpt).reps >= 0)
  {
    status = neut_topt_reps (*pTOpt, &val);
    if (status == 0 && val < (*pTOpt).reps)
      stop = "reps";
  }

  return stop;
}

// Reporting the evaluation of the objective function (printing and
// logging)
void
net_tess_opt_comp_minimize_objective_report (struct TOPT *pTOpt)
{
  int i;
  char *message = ut_alloc_1d_char (1000);
  char *fmin = ut_alloc_1d_char (1000);
  char *f = ut_alloc_1d_char (1000);

  if ((*pTOpt).tarqty > 0 && (*pTOpt).iter == 0)
  {
    if ((*pTOpt).curpenalty > 0)
      sprintf (message, "Initial solution :  f   =%.9f (penalized)", (*pTOpt).objval);
    else
      sprintf (message, "Initial solution:   f   =%.9f", (*pTOpt).objval);

    ut_print_progress (stdout, 1, INT_MAX, message, (*pTOpt).message);
    printf ("\n");
    strcpy ((*pTOpt).message, "");
    ut_print_message (0, 4, "");
  }

  else if ((*pTOpt).iter <= (*pTOpt).itermax)
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

      if ((*pTOpt).curcellpenaltyqty == 0 || (*pTOpt).mincellpenaltyqty == 0)
        sprintf (message, "Iteration %s%d: fmin%s f%s", blank, (*pTOpt).iter,
                 fmin, f);
      else
      {
        sprintf (message, "Iter %s%d: fmin=%s f=%s ((%d min) %d void cell%s)", blank, (*pTOpt).iter,
                 fmin, f, (*pTOpt).mincellpenaltyqty,
                 (*pTOpt).curcellpenaltyqty, (*pTOpt).curcellpenaltyqty > 1 ? "s" : "");
      }
    }
    else
    {
      for (i = 0; i < 11 - ut_num_tenlen_int ((*pTOpt).iter); i++)
        blank = strcat (blank, " ");

      if ((*pTOpt).curcellpenaltyqty == 0 || (*pTOpt).mincellpenaltyqty == 0)
        sprintf (message, "Iter %s%d: fmin=%s f=%s", blank, (*pTOpt).iter,
                 fmin, f);
      else
      {
        sprintf (message, "Iter %s%d: fmin=%s f=%s ((%d min) %d void cell%s)", blank, (*pTOpt).iter,
                 fmin, f, (*pTOpt).mincellpenaltyqty,
                 (*pTOpt).curcellpenaltyqty, (*pTOpt).curcellpenaltyqty > 1 ? "s" : "");
      }
    }
    ut_free_1d_char (&blank);
  }

  else
    strcpy ((*pTOpt).message, "");

  if ((*pTOpt).tarqty > 0)
    ut_print_progress (stdout, 1, INT_MAX, message, (*pTOpt).message);

  net_tess_opt_comp_minimize_objective_log (*pTOpt);

  ut_free_1d_char (&message);

  // Setting t5 of the next iteration
  gettimeofday (&(*pTOpt).end_time, NULL);

#ifdef DEVEL_DEBUGGING_TEST
  net_tess_opt_comp_minimize_objective_debugtest (*pTOpt);
#endif

  ut_free_1d_char (&fmin);
  ut_free_1d_char (&f);

  return;
}

// Ending the testing of the stopping criteria, at the end of the
// optimization
void
net_tess_opt_comp_minimize_objective_stop_end (void)
{
  net_tess_opt_comp_minimize_objective_stop_ctrlc_end ();

  return;
}
