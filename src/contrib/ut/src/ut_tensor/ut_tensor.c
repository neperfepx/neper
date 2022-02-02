/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<math.h>
#include"ut.h"

void
ut_tensor_vec2leq (double *vect, double *lvect)
{
  lvect[0] = (vect[1] - vect[0]) / sqrt (2.);
  lvect[1] = -sqrt (1.5) * (vect[1] + vect[0]);
  lvect[2] = sqrt (2.) * vect[2];
  lvect[3] = sqrt (2.) * vect[3];
  lvect[4] = sqrt (2.) * vect[4];

  return;
}

void
ut_tensor_leq2vec (double *lvect, double *vect)
{
  vect[0] = -lvect[0] / sqrt (2.) - lvect[1] / sqrt (6.);
  vect[1] = lvect[0] / sqrt (2.) - lvect[1] / sqrt (6.);
  vect[2] = lvect[2] / sqrt (2.);
  vect[3] = lvect[3] / sqrt (2.);
  vect[4] = lvect[4] / sqrt (2.);

  return;
}

void
ut_tensor_ten2leq (double **tens, double *lvect)
{
  lvect[0] = (tens[1][1] - tens[0][0]) / sqrt (2.);
  lvect[1] = -sqrt (1.5) * (tens[0][0] + tens[1][1]);
  lvect[2] = sqrt (2.) * tens[1][2];
  lvect[3] = sqrt (2.) * tens[0][2];
  lvect[4] = sqrt (2.) * tens[0][1];

  return;
}

void
ut_tensor_leq2tens (double *lvect, double **tens)
{
  tens[0][0] = -1 / sqrt (6.) * lvect[1] - 1 / sqrt (2.) * lvect[0];
  tens[2][2] = sqrt (2. / 3.) * lvect[1];
  tens[1][1] = -tens[0][0] - tens[2][2];
  tens[0][1] = tens[1][0] = 1 / sqrt (2.) * lvect[4];
  tens[1][2] = tens[2][1] = 1 / sqrt (2.) * lvect[2];
  tens[2][0] = tens[0][2] = 1 / sqrt (2.) * lvect[3];

  return;
}
