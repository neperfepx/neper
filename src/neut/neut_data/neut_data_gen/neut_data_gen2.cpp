/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_gen_.hpp"

void
neut_data_ori_color_rodrigues (double **data, int size, char *scheme, int **Col)
{
  int i, j, varqty = 0;
  double length;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme ? scheme : (char *) "rodrigues", &fct, &vars, &vals, &varqty);

  length = !scheme? OL_S2 - 1 : !varqty? OL_S2 - 1 : atof (vals[0]);

#pragma omp parallel for private(j)
  for (i = 1; i <= size; i++)
  {
    double *R = ol_R_alloc ();
    ol_q_R (data[i], R);
    ol_R_Rcrysym (R, (char *) "cubic", R);
    for (j = 0; j < 3; j++)
      Col[i][j] =
        ut_num_bound (ut_num_d2ri (127.5 * (R[j] + length) / length), 0,
                      255);
    ol_R_free (R);
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
neut_data_ori_color_axis (double **data, int size, char *scheme, int **Col)
{
  int i, j;
  (void) scheme;

#pragma omp parallel for private(j)
  for (i = 1; i <= size; i++)
  {
    double *q = ol_q_alloc ();
    double *r = ol_r_alloc ();
    ol_q_qcrysym (data[i], (char *) "cubic", q);
    ol_q_r (q, r);
    for (j = 0; j < 3; j++)
      Col[i][j] = ut_num_bound (ut_num_d2ri (127.5 * (r[j] + 1)), 0, 255);
    ol_q_free (q);
    ol_r_free (r);
  }

  return;
}

void
neut_data_ori_color_angle (double **data, int size, char *scheme, int **Col)
{
  int i, j, varqty;
  double length;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme, &fct, &vars, &vals, &varqty);

  // 1.09606677025243897430 = 62.8 * pi / 180
  length = !varqty? 1.09606677025243897430 : atof (vals[0]);

#pragma omp parallel for private(j)
  for (i = 1; i <= size; i++)
  {
    double theta, *q = ol_q_alloc ();
    ol_q_qcrysym (data[i], (char *) "cubic", q);
    ol_q_theta_rad (q, &theta);
    for (j = 0; j < 3; j++)
      Col[i][j] = ut_num_bound (ut_num_d2ri (225 * theta / length), 0, 255);
    ol_q_free (q);
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
neut_data_ori_color_axisangle (double **data, int size, char *scheme, int **Col)
{
  int i, j, varqty;
  double length;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme, &fct, &vars, &vals, &varqty);

  // 1.09606677025243897430 = 62.8 * pi / 180
  length = !varqty? 1.09606677025243897430 : atof (vals[0]);

#pragma omp parallel for private(i,j)
  for (i = 1; i <= size; i++)
  {
    double theta, *q = ol_q_alloc (), *r = ol_r_alloc ();
    ol_q_qcrysym (data[i], (char *) "cubic", q);
    ol_q_rtheta_rad (q, r, &theta);
    for (j = 0; j < 3; j++)
      Col[i][j] =
        ut_num_bound (ut_num_d2ri
                      (127.5 * (r[j] * theta + length) / length), 0, 255);
    ol_q_free (q);
    ol_r_free (r);
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
neut_data_ori_color_ipf (double **data, int size, char *scheme, int **Col)
{
  int i, j, varqty, test;
  double *v = ut_alloc_1d (3), *Vs = ut_alloc_1d (3), *p = ut_alloc_1d (2);
  double **g = ol_g_alloc ();
  double *q = ol_q_alloc ();
  double *weight = ut_alloc_1d (3);
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme, &fct, &vars, &vals, &varqty);

  if (varqty == 0)
    Vs[2] = 1;
  else if (varqty == 1)
  {
    if (!strcmp (vals[0], "x") || !strcmp (vals[0], "X"))
      Vs[0] = 1;
    else if (!strcmp (vals[0], "y") || !strcmp (vals[0], "Y"))
      Vs[1] = 1;
    else if (!strcmp (vals[0], "z") || !strcmp (vals[0], "Z"))
      Vs[2] = 1;
    else
      ut_print_exprbug (scheme);
  }
  else
    ut_print_exprbug (scheme);

  for (i = 1; i <= size; i++)
    for (j = 1; j <= 24; j++)
    {
      ol_q_crysym (data[i], (char *) "cubic", j, q);
      ol_q_g (q, g);
      ol_g_ipf_stprojxy (g, Vs, p);
      ol_stprojxy_vect (p, v);
      test = ol_vect_ipfweight (v, weight);
      if (test == 0)
      {
        ol_ipfweight_rgb (weight, Col[i]);
        break;
      }
    }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&v);
  ut_free_1d (&Vs);
  ut_free_1d (&p);
  ol_g_free (g);
  ol_q_free (q);
  ut_free_1d (&weight);

  return;
}
