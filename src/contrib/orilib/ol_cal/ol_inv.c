/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_inv.h"

void
ol_g_inverse (double **g, double **ginv)
{
  int i, j;
  double **gcpy;

  gcpy = ol_g_alloc ();
  ol_g_memcpy (g, gcpy);

  for (i = 0; i <= 2; i++)
    for (j = 0; j <= 2; j++)
      ginv[i][j] = gcpy[j][i];

  ol_g_free (gcpy);

  return;
}

void
ol_q_inverse (double *q, double *qinv)
{
  qinv[0] = q[0];
  qinv[1] = -q[1];
  qinv[2] = -q[2];
  qinv[3] = -q[3];

  return;
}

void
ol_R_inverse (double *R, double *Rinv)
{
  Rinv[0] = -R[0];
  Rinv[1] = -R[1];
  Rinv[2] = -R[2];

  return;
}
