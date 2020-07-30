/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_gen.h"

void
ol_set_misoridistrib (char *distrib, struct OL_SET *pOSet)
{
  size_t i, j;
  int varqty;
  char *fct = NULL;
  char **vars = NULL;
  char **vals = NULL;

  ut_string_function (distrib, &fct, &vars, &vals, &varqty);

  if (!strcmp (fct, "none"))
  {
    for (i = 0; i < (*pOSet).size; i++)
      ol_q_set_id ((*pOSet).q[i]);
  }

  else if (!strcmp (fct, "normal"))
  {
    double sig, *v = ut_alloc_1d (3);

    sscanf (vars[0], "%lf", &sig);

    for (i = 0; i < (*pOSet).size; i++)
    {
      gsl_rng *rand = gsl_rng_alloc (gsl_rng_ranlxd2);
      gsl_rng_set (rand, 1);

      for (j = 0; j < 3; j++)
        v[j] = gsl_ran_gaussian (rand, sig / 2);

      ol_lnq_q (v, (*pOSet).q[i]);

      gsl_rng_free (rand);
    }

    ut_free_1d (&v);
  }

  else
    abort ();

  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&fct);

  return;
}
