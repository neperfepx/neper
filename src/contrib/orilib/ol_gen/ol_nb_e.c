/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_nb_e.h"
#define frand() ((double) rand() / (RAND_MAX+1.0))

void
ol_nb_e (double n1, double n2, double n3, double *e)
{
  ol_nb_e_rad (n1, n2, n3, e);
  ol_e_rad2deg (e, e);

  return;
}

void
ol_nb_e_rad (double n1, double n2, double n3, double *e)
{
  e[0] = 2 * OL_PI * n1;
  e[1] = ut_num_acos (2 * n2 - 1);
  e[2] = 2 * OL_PI * n3;

  return;
}

void
ol_srand_e_rad (int seed, int qty, double **e)
{
  int i;
  srand (seed);

  for (i = 0; i < qty; i++)
    ol_nb_e_rad (frand (), frand (), frand (), e[i]);

  return;
}

void
ol_srand_e (int seed, int qty, double **e)
{
  int i;
  srand (seed);

  for (i = 0; i < qty; i++)
    ol_nb_e (frand (), frand (), frand (), e[i]);

  return;
}
