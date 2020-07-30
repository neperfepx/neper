/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_gen.h"

void
ol_set_misoridistrib (char *distrib, long int random, struct OL_SET *pOSet)
{
  int i, j;
  int varqty;
  char *fct = NULL;
  char **vars = NULL;
  char **vals = NULL;
  gsl_rng *rand = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rand, random);

  ut_string_function (distrib, &fct, &vars, &vals, &varqty);

  if (!strcmp (fct, "none"))
  {
    for (i = 0; i < (int) (*pOSet).size; i++)
      ol_q_set_id ((*pOSet).q[i]);
  }

  else if (!strcmp (fct, "normal"))
  {
    double mean, sig, *v = ut_alloc_1d (3);

    sscanf (vals[0], "%lf", &mean);
    sig = mean / (2 * sqrt (2 / M_PI)); // Glez and Driver, J. Appl. Cryst., 2001

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      for (j = 0; j < 3; j++)
        v[j] = gsl_ran_gaussian (rand, sig / 2);

      ol_lnq_q (v, (*pOSet).q[i]);
    }

    ut_free_1d (&v);
  }

  else
    abort ();

  gsl_rng_free (rand);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&fct);

  return;
}
