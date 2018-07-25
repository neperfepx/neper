/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_aniso.h"

void
ol_set_aniso (struct OL_SET set, double **evect, double *eval)
{
  int i, j, k;
  int n;
  double **R = ut_alloc_2d (set.size, 3);
  double** evect2 = ol_g_alloc ();

  double **ERR = ut_alloc_2d (3, 3);
  double *ER = ut_alloc_1d (3);
  double **Sb = ut_alloc_2d (3, 3);
  double* theta = ut_alloc_1d (4);
  double sumweight;

  if (set.size == 0)
  {
    ut_print_message (2, 0, "ol_set_aniso: set of size 0");
    abort ();
  }
  else if (set.size == 1)
  {
    ol_g_set_id (evect);
    ut_array_1d_set (eval, 3, 0);
  }
  else
  {
    n = set.size;
    for (i = 0; i < n; i++)
      ol_q_R (set.q[i], R[i]);

    /* calculation of the covariant matrix S */
    if (set.weight != NULL)
    {
      sumweight = ut_array_1d_sum (set.weight, n);
      for (i = 0; i < 3; i++)
	for (j = 0; j < 3; j++)
	{
	  for (k = 0; k < n; k++)
	    if (set.id[k] == 1)
	      ERR[i][j] += set.weight[k] * R[k][i] * R[k][j];

	  ERR[i][j] /= sumweight;
	}

      for (i = 0; i < 3; i++)
      {
	for (k = 0; k < n; k++)
	  if (set.id[k] == 1)
	    ER[i] += set.weight[k] * R[k][i];

	ER[i] /= sumweight;
      }
    }
    else
    {
      for (i = 0; i < 3; i++)
	for (j = 0; j < 3; j++)
	{
	  for (k = 0; k < n; k++)
	    if (set.id[k] == 1)
	      ERR[i][j] += R[k][i] * R[k][j];

	  ERR[i][j] /= (double) n;
	}

      for (i = 0; i < 3; i++)
      {
	for (k = 0; k < n; k++)
	  if (set.id[k] == 1)
	    ER[i] += R[k][i];

	ER[i] /= (double) n;
      }
    }

    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
	Sb[i][j] = ERR[i][j] - ER[i] * ER[j];

    /* calculation of the eigen values and vectors of S */

    ut_mat_eigen (3, Sb, eval, evect);

    /* 114.59... is "(180 / PI) * 2" */
    for (i = 0; i < 3; i++)
      eval[i] = 114.59155902616464175442 * atan (ut_num_sqrt (eval[i]));

    // v2 is recalculated as v0 'vect prod' v1 to get a right-handed
    // csys.

    ut_vector_vectprod (evect[0], evect[1], evect[2]);

    // taking the 4 mmm symmetries of evect, and keeping the less
    // disoriented one.
    /*
    for (i = 1; i <= 4; i++)
    {
      ol_g_crysym (evect, "mmm", i, evect2);
      ol_g_theta (evect2, &(theta[i - 1]));
      printf ("i = %d, theta = %f\n", i, theta[i - 1]);
    }
    min_id = 1 + ut_array_1d_min_index (theta, 4);

    ol_g_crysym (evect, "mmm", min_id, evect);
    */
  }

  ut_free_2d (R, set.size);
  ol_g_free (evect2);
  ut_free_1d (theta);
  ut_free_2d (ERR, 3);
  ut_free_1d (ER);
  ut_free_2d (Sb,  3);

  return;
}

void
ol_q_aniso_theta (double *q, double **evect, double *theta)
{
  int j, k;
  double tmp;
  double *R = ol_R_alloc ();

  ol_q_R (q, R);

  /* Calculation of the thetas */
  for (j = 0; j < 3; j++)
  {
    // printf ("calc theta along ");
    // ut_array_1d_fprintf (stdout, evect[j], 3, "%9.6f");
    tmp = 0;
    for (k = 0; k < 3; k++)
      tmp += evect[j][k] * R[k];

    /* 114.59155902616464175442 is (180 / PI) * 2 */
    theta[j] = 114.59155902616464175442 * atan (tmp);
  }

  ol_R_free (R);

  return;
}

void
ol_set_aniso_thetadistrib (struct OL_SET set, double **evect, double step, double **pdistrib, double *distribfirstx, int *distriblength)
{
  int i, j, n;
  double **theta = ut_alloc_2d (3, set.size);
  double *etheta = ut_alloc_1d (3);
  double* q = ol_q_alloc ();

  /* Calculation of the thetas */
  n = 0;
  for (i = 0; i < (int) set.size; i++)
    if (set.id[i] == 1)
    {
      ol_q_aniso_theta (set.q[i], evect, etheta);

      for (j = 0; j < 3; j++)
	theta[j][n] = etheta[j];
      n++;
    }

  /* Allocation and calculation */
  for (j = 0; j < 3; j++)
  {
    ut_array_distribparam (theta[j], n, step,
			   &(distribfirstx[j]), &(distriblength[j]));

    pdistrib[j] = ut_alloc_1d (distriblength[j]);

    ut_array_distrib (theta[j], n, step, distribfirstx[j],
		      distriblength[j], pdistrib[j]);
  }

  ut_free_2d (theta, 3);
  ut_free_1d (etheta);
  ol_q_free (q);

  return;
}
