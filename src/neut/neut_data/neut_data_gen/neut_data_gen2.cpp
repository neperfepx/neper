/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_gen_.hpp"

void
neut_data_ori_color_rodrigues (double **data, int size, char *scheme, int **Col)
{
  int i, varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme ? scheme : (char *) "rodrigues", &fct, &vars, &vals, &varqty);

#pragma omp parallel for
  for (i = 1; i <= size; i++)
    neut_ori_rodriguescol_int (data[i], (char *) "cubic", Col[i]);

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
  int i, varqty;
  double length;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme, &fct, &vars, &vals, &varqty);

  // 1.09606677025243897430 = 62.8 * pi / 180
  length = !varqty? 1.09606677025243897430 : atof (vals[0]);

#pragma omp parallel for
  for (i = 1; i <= size; i++)
  {
    double theta, *q = ol_q_alloc ();
    ol_q_qcrysym (data[i], (char *) "cubic", q);
    ol_q_theta_rad (q, &theta);
    double val = theta / length;
    tinycolormap::ColormapType type =
      neut_data_colscheme_tinycolormaptype ((char *) NEUT_DEFAULT_COLSCHEME_REAL);
    tinycolormap::Color col = tinycolormap::GetColor(val, type);
    Col[i][0] = ut_num_d2ri (255 * col.r());
    Col[i][1] = ut_num_d2ri (255 * col.g());
    Col[i][2] = ut_num_d2ri (255 * col.b());
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
  int i, j, varqty, absolute;
  double length;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme, &fct, &vars, &vals, &varqty);

  // 1.09606677025243897430 = 62.8 * pi / 180
  absolute = 0;
  length = 1.09606677025243897430;
  for (i = 0; i < varqty; i++)
  {
    if (!strcmp (vals[i], "absolute"))
      absolute = 1;
    else
      length = atof (vals[i]);
  }

#pragma omp parallel for private(i,j)
  for (i = 1; i <= size; i++)
  {
    double theta, *q = ol_q_alloc (), *r = ol_r_alloc ();
    ol_q_qcrysym (data[i], (char *) "cubic", q);
    ol_q_rtheta_rad (q, r, &theta);
    for (j = 0; j < 3; j++)
    {
      Col[i][j] = absolute?
        ut_num_d2ri (255 * fabs (r[j] * theta) / length)
      : ut_num_d2ri (127.5 * (r[j] * theta + length) / length);

      Col[i][j] = ut_num_bound (Col[i][j], 0, 255);
    }

    ol_q_free (q);
    ol_r_free (r);
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
neut_data_ori_color_ipf (double **data, int size, char *crysym, char *scheme, int **Col)
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
    for (j = 1; j <= ol_crysym_qty (crysym); j++)
    {
      ol_q_crysym (data[i], crysym, j, q);
      ol_q_g (q, g);
      ol_g_ipf_stprojxy (g, Vs, p);
      ol_stprojxy_vect (p, v);
      test = ol_vect_ipfweight (v, crysym, weight);
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
