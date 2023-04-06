/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_map_rgb.h"

/* could be improved as ol_g_ipf_crysym_stproj_rad */
void
ol_map_vect_ipfrgb (struct OL_MAP Map, double *Vs, int *nidrgb)
{
  unsigned int i, j, k;
  int test;
  double *q = ol_q_alloc ();
  double **g = ol_g_alloc ();
  double *v = ol_vect_alloc ();
  double *p = ol_p_alloc ();
  double *weight = ol_e_alloc ();

  for (i = 0; i < Map.xsize; i++)
    for (j = 0; j < Map.ysize; j++)
    {
      if (Map.id[i][j] == 1)
      {
        for (k = 1; k <= 24; k++)
        {
          ol_q_crysym (Map.q[i][j], "cubic", k, q);
          ol_q_g (q, g);
          ol_g_ipf_stprojxy (g, Vs, p);
          ol_stprojxy_vect (p, v);
          test = ol_vect_ipfweight (v, Map.crysym, weight);
          if (test == 0)
          {
            ol_ipfweight_rgb (weight, Map.rgb[i][j]);
            break;
          }
        }
      }
      else
        for (k = 0; k < 3; k++)
          Map.rgb[i][j][k] = nidrgb[k];
    }

  ol_q_free (q);
  ol_g_free (g);
  ol_vect_free (v);
  ol_p_free (p);
  ol_e_free (weight);

  return;
}

void
ol_ipfweight_rgb (double *weight, int *rgb)
{
  int i, j, k, max;
  int **rgbp = ut_alloc_2d_int (3, 3);
  double f;
  double slope;
  double *s = ol_e_alloc ();
  int maxrgb;

  f = 0.1;
  slope = 3;
  maxrgb = 255 * 0.8;

  /* adjustment due to the difference in vertex angle */
  s[0] = slope;
  s[1] = slope;
  s[2] = slope;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      if (i == j)
        rgbp[i][j] = (int) 255;
      else
        rgbp[i][j] = (int) (f / (1 + f) * 255.);

  for (k = 0; k < 3; k++)
    weight[k] = (s[k] - 2) * weight[k] * weight[k] * weight[k]
      + (3 - 2 * s[k]) * weight[k] * weight[k] + s[k] * weight[k];

  rgb[0] =
    rgbp[0][0] * weight[0] + rgbp[0][1] * weight[1] + rgbp[0][2] * weight[2];
  rgb[1] =
    rgbp[1][0] * weight[0] + rgbp[1][1] * weight[1] + rgbp[1][2] * weight[2];
  rgb[2] =
    rgbp[2][0] * weight[0] + rgbp[2][1] * weight[1] + rgbp[2][2] * weight[2];

  max = ut_array_1d_int_max (rgb, 3);

  for (i = 0; i < 3; i++)
    rgb[i] = (int) ((double) rgb[i] * maxrgb / (double) max);

  ut_free_2d_int (&rgbp, 3);
  ol_e_free (s);

  return;
}

void
ol_ipfrgb_legend (size_t xsize, size_t * pysize, char *crysym, int ****prgb)
{
  unsigned int i, j;
  int k, test;
  double *p = ol_p_alloc ();
  double *v = ol_vect_alloc ();
  double *weight = ol_e_alloc ();
  double xmax = 0.414213562373;
  double ymax = 0.366025403784;

  (*pysize) = ceil (xsize * (ymax / xmax));

  (*prgb) = ut_alloc_3d_int (xsize, *pysize, 3);

  for (i = 0; i < xsize; i++)
    for (j = 0; j < *pysize; j++)
    {
      ol_p_set_this (p, ymax * (1 - (double) j / (*pysize - 1)),
                     xmax * (double) i / (xsize - 1));
      ol_stprojxy_vect (p, v);
      test = ol_vect_ipfweight (v, crysym, weight);
      if (test == 0)
        ol_ipfweight_rgb (weight, (*prgb)[i][j]);
      else
        for (k = 0; k < 3; k++)
          (*prgb)[i][j][k] = 255;
    }

  ol_p_free (p);
  ol_vect_free (v);
  ol_e_free (weight);

  return;
}
