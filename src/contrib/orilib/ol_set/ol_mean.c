/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_mean.h"

// single pass: mean orientation of set, with quaternions expressed with
// respect to qref
static void
ol_set_mean_ref_pass (struct OL_SET set, double *qref, double *qm)
{
  unsigned int i, j;
  double qnorm;
  double *qb = ol_q_alloc ();

  ut_array_1d_zero (qm, 4);

  for (i = 0; i < set.size; i++)
    if (!set.id || set.id[i] == 1)
    {
      ol_q_q_qdisori_cur (qref, set.q[i], set.crysym, qb);

      for (j = 0; j < 4; j++)
        qm[j] += qb[j] * set.weight[i];
    }

  qnorm = ut_array_1d_norm (qm, 4);
  ut_array_1d_scale (qm, 4, 1 / qnorm);

  ol_q_q_q (qref, qm, qm);

  ol_q_free (&qb);

  return;
}

int
ol_set_mean_ref (struct OL_SET set, double *qref, int itermax, double *qm)
{
  int it;
  double theta;
  double *qb = ol_q_alloc ();

  ol_q_memcpy (qref, qb);

  for (it = 1; ; it++)
  {
    ol_set_mean_ref_pass (set, qb, qm);

    if (itermax == 1)
      break;

    // convergence: the mean orientation no longer changes
    ol_q_q_disori (qb, qm, set.crysym, &theta);
    ol_q_memcpy (qm, qb);

    if (theta <= 1e-1)
      break;
    else if (it == itermax)
    {
      it = -1;
      break;
    }
  }

  ol_q_free (&qb);

  return it;
}

int
ol_set_mean (struct OL_SET set, int itermax, double *qm)
{
  size_t i;

  /* reference: first defined orientation */
  for (i = 0; i < set.size; i++)
    if (!set.id || set.id[i] == 1)
      break;

  if (set.size == 0)
    return 0;

  return ol_set_mean_ref (set, set.q[i < set.size ? i : 0], itermax, qm);
}
