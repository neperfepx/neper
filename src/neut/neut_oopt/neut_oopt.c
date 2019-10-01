/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_oopt_.h"

void
neut_oopt_set_zero (struct OOPT *pOOpt)
{
  gettimeofday (&(*pOOpt).beg_time, NULL);

  (*pOOpt).force_dur = 0;
  (*pOOpt).potential_dur = 0;
  (*pOOpt).rot_dur = 0;
  (*pOOpt).unrot_dur = 0;
  (*pOOpt).total_dur = 0;

  (*pOOpt).nc = 0;
  (*pOOpt).neighd = 0;
  (*pOOpt).neighdE = 0;

  ut_fct_set_zero (&((*pOOpt).thetafct));
  neut_ori_volthetafct (&((*pOOpt).thetafct));

  (*pOOpt).avforce_ini = DBL_MAX;
  (*pOOpt).avforce = DBL_MAX;
  (*pOOpt).f = DBL_MAX;
  (*pOOpt).fmin = DBL_MAX;
  (*pOOpt).avneighqty = INT_MAX;

  (*pOOpt).fixoriqty = 0;
  (*pOOpt).fixori = NULL;

  (*pOOpt).n = 0;
  (*pOOpt).id = 0;
  (*pOOpt).crysym = NULL;
  (*pOOpt).orioptistop = NULL;
  (*pOOpt).orioptineigh = NULL;

  (*pOOpt).logvar = NULL;
  (*pOOpt).logvar_fp = NULL;

  return;
}

void
neut_oopt_time_set_zero (struct OOPT *pOOpt)
{
  gettimeofday (&(*pOOpt).beg_time, NULL);

  (*pOOpt).force_dur = 0;
  (*pOOpt).potential_dur = 0;
  (*pOOpt).rot_dur = 0;
  (*pOOpt).unrot_dur = 0;
  (*pOOpt).total_dur = 0;

  return;
}

void
neut_oopt_finalize (struct OOPT *pOOpt)
{
  gettimeofday (&(*pOOpt).end_time, NULL);

  (*pOOpt).total_dur =
    ut_time_subtract (&(*pOOpt).beg_time, &(*pOOpt).end_time);

  return;
}

void
neut_oopt_free (struct OOPT *pOOpt)
{
  (void) pOOpt;

  return;
}
