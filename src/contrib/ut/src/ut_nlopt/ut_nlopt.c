/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef HAVE_NLOPT
#include<nlopt.h>
#endif // HAVE_NLOPT

#include "ut.h"
#include "ut_nlopt.h"

#ifdef HAVE_NLOPT
void
ut_nlopt_algo (char *optialgo, nlopt_algorithm * palgo)
{

  if (!strcmp (optialgo, "neldermead"))
    (*palgo) = NLOPT_LN_NELDERMEAD;
  else if (!strcmp (optialgo, "subplex"))
    (*palgo) = NLOPT_LN_SBPLX;
  else if (!strcmp (optialgo, "cobyla"))
    (*palgo) = NLOPT_LN_COBYLA;
  else if (!strcmp (optialgo, "bobyqa"))
    (*palgo) = NLOPT_LN_BOBYQA;
  else if (!strcmp (optialgo, "newuoa"))
    (*palgo) = NLOPT_LN_NEWUOA_BOUND;
  else if (!strcmp (optialgo, "praxis"))
    (*palgo) = NLOPT_LN_PRAXIS;
  else if (!strcmp (optialgo, "random"))
    (*palgo) = NLOPT_LN_PRAXIS;
  else if (!strncmp (optialgo, "lloyd", 5))
    (*palgo) = NLOPT_LN_PRAXIS;
  else
    abort ();

  return;
}
#endif // HAVE_NLOPT
