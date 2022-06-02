/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_gen.h"

void
ol_set_misorispread (char *distrib, int dim, long int random, struct OL_SET *pOSet)
{
  int i, j;
  int varqty;
  char *fct = NULL;
  char **vars = NULL;
  char **vals = NULL;
  gsl_rng *rand = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rand, random);

  ut_string_function (distrib? distrib : "none", &fct, &vars, &vals, &varqty);

  if (!strcmp (fct, "none"))
  {
    for (i = 0; i < (int) (*pOSet).size; i++)
      ol_q_set_id ((*pOSet).q[i]);
  }

  else if (!strcmp (fct, "normal"))
  {
    double *sig = ut_alloc_1d (dim), *v = ut_alloc_1d (3);

    for (i = 0; i < varqty; i++)
    {
      if (!vars[i] || !strcmp (vars[i], "theta"))
      {
        sscanf (vals[i], "%lf", sig);
        ut_array_1d_set (sig, dim, sig[0]);
      }
      else if (!strcmp (vars[i], "theta1"))
        sscanf (vals[i], "%lf", sig);
      else if (!strcmp (vars[i], "theta2"))
        sscanf (vals[i], "%lf", sig + 1);
      else if (!strcmp (vars[i], "theta3"))
        sscanf (vals[i], "%lf", sig + 2);
      else if (!strcmp (vars[i], "thetam"))
      {
        // Glez and Driver, J. Appl. Cryst., 2001
        // we use sig[0] as a tmp variable
        sscanf (vals[i], "%lf", sig);
        if (dim == 3)
          ut_array_1d_set (sig, dim, sig[0] / (2 * sqrt (2 / M_PI)));
        else if (dim == 2)
          ut_array_1d_set (sig, dim, sig[0] / sqrt (M_PI / 2));
        else if (dim == 1)
          ut_array_1d_set (sig, dim, sig[0] / sqrt (2 / M_PI));
      }
    }

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      for (j = 0; j < dim; j++)
        v[j] = (sig[j] > 0) ? gsl_ran_gaussian (rand, sig[j] / 2) : 0;

      ol_lnq_q (v, (*pOSet).q[i]);
    }

    ut_free_1d (&sig);
    ut_free_1d (&v);
  }

  else if (!strcmp (fct, "flat"))
  {
    double *r = ol_r_alloc ();
    double tmp, theta, thetamax = 0;

    for (i = 0; i < varqty; i++)
    {
      if (!strcmp (vars[i], "theta"))
        thetamax = atof (vals[i]);
    }

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      if (dim == 3)
      ol_nb_max_rtheta (gsl_rng_uniform (rand), gsl_rng_uniform (rand),
                        gsl_rng_uniform (rand), thetamax, r, &theta);
      else if (dim == 2)
      {
        tmp = gsl_rng_uniform (rand) * 2 * M_PI;
        ut_array_1d_set_2 (r, cos (tmp), sin (tmp));
        theta = thetamax * pow (gsl_rng_uniform (rand), 0.5);
      }
      else
        abort ();

      ol_rtheta_q (r, theta, (*pOSet).q[i]);
    }

    ol_r_free (r);
  }

  else
    abort ();

  gsl_rng_free (rand);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&fct);

  return;
}
