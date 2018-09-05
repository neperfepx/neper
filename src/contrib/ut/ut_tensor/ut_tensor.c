/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<math.h>
#include"ut.h"

void
ut_tensor_vec2leq (double *vec, double *leq)
{
  leq[0] = (vec[1] - vec[0]) / sqrt (2.);
  leq[1] = -sqrt (1.5) * (vec[1] + vec[0]);
  leq[2] = sqrt (2.) * vec[2];
  leq[3] = sqrt (2.) * vec[3];
  leq[4] = sqrt (2.) * vec[4];

  return;
}

void
ut_tensor_leq2vec (double *leq, double *vec)
{
  vec[0] = -leq[0] / sqrt (2.) - leq[1] / sqrt (6.);
  vec[1] = leq[0] / sqrt (2.) - leq[1] / sqrt (6.);
  vec[2] = leq[2] / sqrt (2.);
  vec[3] = leq[3] / sqrt (2.);
  vec[4] = leq[4] / sqrt (2.);

  return;
}

void
ut_tensor_mat2leq (double **mat, double *leq)
{
  leq[0] = (mat[1][1] - mat[0][0]) / sqrt (2.);
  leq[1] = -sqrt (1.5) * (mat[0][0] + mat[1][1]);
  leq[2] = sqrt (2.) * mat[1][2];
  leq[3] = sqrt (2.) * mat[0][2];
  leq[4] = sqrt (2.) * mat[0][1];

  return;
}

void
ut_tensor_leq2mat (double *leq, double **mat)
{
  mat[0][0] = -1 / sqrt (6.) * leq[1] - 1 / sqrt (2.) * leq[0];
  mat[2][2] = sqrt (2. / 3.) * leq[1];
  mat[1][1] = -mat[0][0] - mat[2][2];
  mat[0][1] = mat[1][0] = 1 / sqrt (2.) * leq[4];
  mat[1][2] = mat[2][1] = 1 / sqrt (2.) * leq[2];
  mat[2][0] = mat[0][2] = 1 / sqrt (2.) * leq[3];

  return;
}
