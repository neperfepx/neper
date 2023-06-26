/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_pole.h"

int *
ol_pole_alloc (void)
{
  int *pole = ut_alloc_1d_int (3);

  return pole;
}

int *
ol_hpole_alloc (void)
{
  int *hpole = ut_alloc_1d_int (4);

  return hpole;
}

double *
ol_vect_alloc (void)
{
  double *vect = ut_alloc_1d (3);

  return vect;
}

double *
ol_p_alloc (void)
{
  double *p = ut_alloc_1d (2);

  return p;
}

void
ol_pole_free (int *pole)
{
  if (!pole)
    return;
  else
    ut_free_1d_int (&pole);

  return;
}

void
ol_hpole_free (int *hpole)
{
  if (!hpole)
    return;
  else
    ut_free_1d_int (&hpole);

  return;
}

void
ol_vect_free (double *vect)
{
  if (!vect)
    return;
  else
    ut_free_1d (&vect);

  return;
}

void
ol_p_free (double *p)
{
  if (!p)
    return;
  else
    ut_free_1d (&p);

  return;
}

int
ol_pole_fprintf (FILE * file, int *pole, char *format)
{
  fprintf (file, format, pole[0]);
  fprintf (file, " ");
  fprintf (file, format, pole[1]);
  fprintf (file, " ");
  fprintf (file, format, pole[2]);
  return fprintf (file, "\n");
}

int
ol_pole_fscanf (FILE * file, int *pole)
{
  if (fscanf (file, "%d%d%d", &pole[0], &pole[1], &pole[2]) == 3)
    return 1;
  else
    return 0;
}

int
ol_hpole_fprintf (FILE * file, int *hpole, char *format)
{
  fprintf (file, format, hpole[0]);
  fprintf (file, " ");
  fprintf (file, format, hpole[1]);
  fprintf (file, " ");
  fprintf (file, format, hpole[2]);
  fprintf (file, " ");
  fprintf (file, format, hpole[3]);
  return fprintf (file, "\n");
}

int
ol_hpole_fscanf (FILE * file, int *hpole)
{
  return fscanf (file, "%d%d%d%d", &hpole[0], &hpole[1], &hpole[2],
                 &hpole[3]);
}

int
ol_vect_fprintf (FILE * file, double *vect, char *format)
{
  fprintf (file, format, vect[0]);
  fprintf (file, " ");
  fprintf (file, format, vect[1]);
  fprintf (file, " ");
  fprintf (file, format, vect[2]);
  return fprintf (file, "\n");
}

int
ol_vect_fscanf (FILE * file, double *vect)
{
  int status;

  status = fscanf (file, "%lf%lf%lf", vect, vect + 1, vect + 2);

  return status == 3 ? 1 : -1;
}

int
ol_p_fprintf (FILE * file, double *p, char *format)
{
  fprintf (file, format, p[0]);
  fprintf (file, " ");
  fprintf (file, format, p[1]);
  return fprintf (file, "\n");
}

int
ol_p_fscanf (FILE * file, double *p)
{
  int status;

  status = fscanf (file, "%lf%lf", p, p + 1);

  return status == 2 ? 1 : -1;
}

void
ol_pole_vect (int *m, double *vect)
{
  int i;
  double norm;

  norm = ut_array_1d_int_norm (m, 3);

  for (i = 0; i < 3; i++)       /* cannot copy using ut_; int to double ... */
    vect[i] = (double) m[i];
  ut_array_1d_scale (vect, 3, 1 / norm);

  return;
}

void
ol_hpole_vect (int *hm, double *vect)
{
  int i, j;
  double norm;
  double **a = ut_alloc_2d (4, 3);

  /* setting the non-zero terms only */
  a[0][0] = sqrt (3) / 2;
  a[0][1] = -.5;
  a[1][1] = 1;
  a[2][0] = -sqrt (3) / 2;
  a[2][1] = -.5;
  a[3][2] = 1;

  ut_array_1d_zero (vect, 3);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 4; j++)
      vect[i] += (double) hm[j] * a[j][i];

  norm = ut_array_1d_norm (vect, 3);
  ut_array_1d_scale (vect, 3, 1 / norm);

  ut_free_2d (&a, 4);

  return;
}

void
ol_pole_set_zero (int *pole)
{
  ut_array_1d_int_zero (pole, 3);

  return;
}

void
ol_pole_set_this (int *pole, int x, int y, int z)
{
  pole[0] = x;
  pole[1] = y;
  pole[2] = z;

  return;
}

void
ol_pole_memcpy (int *polesrc, int *poledest)
{
  ut_array_1d_int_memcpy (polesrc, 3, poledest);

  return;
}

void
ol_vect_set_zero (double *vect)
{
  ut_array_1d_zero (vect, 3);

  return;
}

void
ol_vect_set_this (double *vect, double x, double y, double z)
{
  vect[0] = x;
  vect[1] = y;
  vect[2] = z;

  return;
}

void
ol_vect_memcpy (double *vectsrc, double *vectdest)
{
  ut_array_1d_memcpy (vectsrc, 3, vectdest);

  return;
}

void
ol_p_set_zero (double *p)
{
  ut_array_1d_zero (p, 2);

  return;
}

void
ol_p_set_this (double *p, double x, double y)
{
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_p_memcpy (double *psrc, double *pdest)
{
  ut_array_1d_memcpy (psrc, 2, pdest);

  return;
}

void
ol_vect_stproj_rad (double *vect, double *p)
{
  double alpha, beta;

  /* Calculation of alpha and beta */

  alpha = ut_num_acos (vect[2]);

  if (fabs (alpha) < 1e-6)      /* beta is undefined; choosing 0 */
    beta = 0;
  else
    beta = atan2 (vect[1] / sin (alpha), vect[0] / sin (alpha));

  if (alpha > OL_PI / 2)
  {
    alpha = OL_PI - alpha;
    beta += OL_PI;
  }

  /* calculation of p
   * 0: Op; 1: beta (angle from RD)
   */
  p[0] = tan (alpha / 2);
  p[1] = beta;

  return;
}

void
ol_vect_stproj (double *vect, double *p)
{
  ol_vect_stproj_rad (vect, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_vect_stprojxy (double *vect, double *p)
{
  double x, y;

  ol_vect_stproj_rad (vect, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_vect_eaproj (double *vect, double *p)
{
  ol_vect_eaproj_rad (vect, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_vect_eaprojxy (double *vect, double *p)
{
  double x, y;

  ol_vect_eaproj_rad (vect, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_vect_eaproj_rad (double *vect, double *p)
{
  ol_vect_stproj_rad (vect, p);

  p[0] = sqrt (2) * sin (atan (p[0]));

  return;
}

void
ol_g_pf_stproj (double **g, double *vect, double *p)
{
  ol_g_pf_stproj_rad (g, vect, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_g_pf_stprojxy (double **g, double *vect, double *p)
{
  double x, y;

  ol_g_pf_stproj_rad (g, vect, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_g_pf_stproj_rad (double **g, double *vect, double *p)
{
  int i, j;
  double *tmp = ol_vect_alloc ();

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp[i] += g[j][i] * vect[j];      /* [j][i] == using the inverse of g */

  ol_vect_stproj_rad (tmp, p);

  ol_vect_free (tmp);

  return;
}

void
ol_g_pf_eaproj (double **g, double *vect, double *p)
{
  ol_g_pf_eaproj_rad (g, vect, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_g_pf_eaprojxy (double **g, double *vect, double *p)
{
  double x, y;

  ol_g_pf_eaproj_rad (g, vect, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}


void
ol_g_pf_eaproj_rad (double **g, double *vect, double *p)
{
  ol_g_pf_stproj_rad (g, vect, p);

  p[0] = sqrt (2) * sin (atan (p[0]));

  return;
}

void
ol_g_ipf_stproj (double **g, double *vect, double *p)
{
  ol_g_ipf_stproj_rad (g, vect, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_g_ipf_stprojxy (double **g, double *vect, double *p)
{
  double x, y;

  ol_g_ipf_stproj_rad (g, vect, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_g_ipf_stproj_rad (double **g, double *vect, double *p)
{
  int i, j;
  double *tmp = ol_vect_alloc ();

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp[i] += g[i][j] * vect[j];

  ol_vect_stproj_rad (tmp, p);

  ol_vect_free (tmp);

  return;
}

void
ol_g_ipf_eaproj (double **g, double *vect, double *p)
{
  ol_g_ipf_eaproj_rad (g, vect, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_g_ipf_eaprojxy (double **g, double *vect, double *p)
{
  double x, y;

  ol_g_ipf_eaproj_rad (g, vect, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_g_ipf_eaproj_rad (double **g, double *vect, double *p)
{
  ol_g_ipf_stproj_rad (g, vect, p);

  p[0] = sqrt (2) * sin (atan (p[0]));

  return;
}

void
ol_g_ipf_crysym_stproj (double **g, double *vect, char *crysym, double *p)
{
  ol_g_ipf_crysym_stproj_rad (g, vect, crysym, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_g_ipf_crysym_stprojxy (double **g, double *vect, char *crysym, double *p)
{
  double x, y;

  ol_g_ipf_crysym_stproj_rad (g, vect, crysym, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

/* could really be improved for cpu efficiency */
/* Does this work for crystals other than cubic??????? */
void
ol_g_ipf_crysym_stproj_rad (double **g, double *vect, char *crysym, double *p)
{
  int i, j;
  double alpha, beta;
  double *tmp = ut_alloc_1d (3);
  double **gsym = ol_g_alloc ();
  double *v = ol_vect_alloc ();
  double *weight = ol_e_alloc ();
  int qty = ol_crysym_qty (crysym);

  /* Calculation of alpha and beta */

  /* Choosing the right symmetry */
  for (i = 1; i <= qty; i++)
  {
    ol_g_crysym (g, crysym, i, gsym);
    ol_g_ipf_stprojxy (gsym, vect, p);
    ol_stprojxy_vect (p, v);
    if (ol_vect_ipfweight (v, crysym, weight) == 0)
      break;
  }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp[i] += gsym[i][j] * vect[j];

  alpha = ut_num_acos (tmp[2]);

  if (fabs (alpha) < 1e-6)      /* beta is undefined; choosing 0 */
    beta = 0;
  else
    beta = atan2 (tmp[1] / sin (alpha), tmp[0] / sin (alpha));

  if (alpha > OL_PI / 2)
  {
    alpha = OL_PI - alpha;
    beta += OL_PI;
  }

  /* calculation of p
   * 0: Op; 1: beta (angle from RD)
   */
  p[0] = tan (alpha / 2);
  p[1] = beta;

  ut_free_1d (&tmp);
  ol_g_free (gsym);
  ol_vect_free (v);
  ol_e_free (weight);

  return;
}

void
ol_g_ipf_crysym_eaproj (double **g, double *vect, char *crysym, double *p)
{
  ol_g_ipf_crysym_eaproj_rad (g, vect, crysym, p);
  ol_theta_rad2deg (p[1], &(p[1]));

  return;
}

void
ol_g_ipf_crysym_eaprojxy (double **g, double *vect, char *crysym, double *p)
{
  double x, y;

  ol_g_ipf_crysym_eaproj_rad (g, vect, crysym, p);

  x = p[0] * cos (p[1]);
  y = p[0] * sin (p[1]);
  p[0] = x;
  p[1] = y;

  return;
}

void
ol_g_ipf_crysym_eaproj_rad (double **g, double *vect, char *crysym, double *p)
{
  ol_g_ipf_crysym_stproj_rad (g, vect, crysym, p);

  p[0] = sqrt (2) * sin (atan (p[0]));

  return;
}

void
ol_stproj_vect_rad (double *p, double *vect)
{
  double alpha = 2 * atan (p[0]);

  vect[0] = sin (alpha) * cos (p[1]);
  vect[1] = sin (alpha) * sin (p[1]);
  vect[2] = cos (alpha);

  return;
}

void
ol_eaproj_vect_rad (double *p, double *vect)
{
  double alpha = 2 * asin (p[0] / sqrt (2));

  vect[0] = sin (alpha) * cos (p[1]);
  vect[1] = sin (alpha) * sin (p[1]);
  vect[2] = cos (alpha);

  return;
}

void
ol_stproj_vect (double *p, double *vect)
{
  double *pcpy = ol_p_alloc ();

  ol_p_memcpy (p, pcpy);
  ol_theta_deg2rad (pcpy[1], &(pcpy[1]));
  ol_stproj_vect_rad (pcpy, vect);

  ol_p_free (pcpy);

  return;
}

void
ol_eaproj_vect (double *p, double *vect)
{
  double *pcpy = ol_p_alloc ();

  ol_p_memcpy (p, pcpy);
  ol_theta_deg2rad (pcpy[1], &(pcpy[1]));
  ol_eaproj_vect_rad (pcpy, vect);

  ol_p_free (pcpy);

  return;
}

void
ol_stprojxy_vect (double *p, double *vect)
{
  double *psh = ol_p_alloc ();

  psh[0] = sqrt (p[0] * p[0] + p[1] * p[1]);

  if (psh[0] > 1e-12)
    psh[1] = atan2 (p[1] / psh[0], p[0] / psh[0]);
  else
    psh[1] = 0;

  ol_stproj_vect_rad (psh, vect);

  ol_p_free (psh);

  return;
}

void
ol_eaprojxy_vect (double *p, double *vect)
{
  double *psh = ol_p_alloc ();

  psh[0] = sqrt (p[0] * p[0] + p[1] * p[1]);

  if (psh[0] > 1e-12)
    psh[1] = atan2 (p[1] / psh[0], p[0] / psh[0]);
  else
    psh[1] = 0;

  ol_eaproj_vect_rad (psh, vect);

  ol_p_free (psh);

  return;
}

int
ol_vect_ipfweight (double *v, char *crysym, double *weight)
{
  double *v1 = ol_vect_alloc ();
  double *v2 = ol_vect_alloc ();
  double *v3 = ol_vect_alloc ();
  double *r = ol_r_alloc ();
  double **g = ol_g_alloc ();
  double t, theta;
  int test = 0;

  ol_e_set_zero (weight);

  if (!strcmp (crysym, "cubic"))
  {
    if (v[0] > v[1] && v[0] < v[1] + sqrt (OL_EPS))
    {
      v[0] = v[1];
      ut_array_1d_normalize (v, 3);
    }

    if (v[0] > v[1])
      test = -1;

    if (test != -1)
    {
      /* calculating v1 = rotated vector to (001)-(011) about (01-1) */
      ol_r_set_this (r, 0, 1 / sqrt (2), -1 / sqrt (2));

      ol_theta_rad2deg (atan2 (sqrt (2) * v[0], (v[1] + v[2])), &theta);

      ol_rtheta_g (r, theta, g);
      ol_g_vect_vect (g, v, v1);
  /* computing theta3 = angle to reach segment (001)-(111) by rotation
     * about r (theta3 has a negative value)

    * theta3 is the solution of equation,
     * cos(theta3).v0 - sin(theta3).(v1+v2)/sqrt(2)
     = cos(theta3).(v1+v2)/2 + sin(theta3).v0/sqrt(2) + (v1-v2)/2
     which is equivalent to,
     t^2 .2(v2-v0) + t (-2sqrt(2).(v0+v1+v2)) + 2(v0-v1) = 0
     with t = tan (theta3 / 2)
     (( using cos (theta3) = (1 - t^2) / (1 + t^2) ))
     ((       sin (theta3) =     2t    / (1 + t^2) ))

    a = 2 * (v[2] - v[0]);
    b = -2 * sqrt(2) * (v[0] + v[1] + v[2]);
    c = 2 * (v[0] - v[1]);

    if (fabs (a) < 1e-6)
      t = - c / b;
    else
    {
      delta = b*b - 4 * a * c;
      t = (-b - sqrt(delta)) / (2 * a);
    }

    ol_theta_rad2deg (2 * atan (t), &thetab);

    ol_rtheta_g (r, thetab, g);
    ol_g_vect_vect (g, v, v1b);

    w = 1 - acos (v1[2]) / (OL_PI / 4);
    wb = 1 - acos(v1b[2]) / acos(1/sqrt(3));
    ratio = theta / (theta + thetab);
    weight[0] = w; */
      weight[0] = 1 - acos (v1[2]) / (OL_PI / 4);

      if (weight[0] < 0 && weight[0] > -OL_EPS)
        weight[0] = 0;
      if (weight[0] > 1 && weight[0] < 1 + OL_EPS)
        weight[0] = 1;
      if (weight[0] < 0 || weight[0] > 1)
        test = -1;
    }

    if (test != -1)
    {
      /* calculating v2 = rotated vector to (001)-(011) about (-110) */

      ol_r_set_this (r, -1 / sqrt (2), 1 / sqrt (2), 0);

      if (fabs (v[1]) < OL_EPS)
        t = -v[0] / (sqrt (2) * v[2]);
      else
        t =
          (sqrt (2) * v[2] -
           sqrt (2 * v[2] * v[2] + 4 * v[0] * v[1])) / (2 * v[1]);

      ol_theta_rad2deg (2 * atan (t), &theta);

      ol_rtheta_g (r, -theta, g);
      ol_g_vect_vect (g, v, v2);
      weight[1] = ut_num_acos (v2[2]) / (OL_PI / 4);
      if (weight[1] < 0 && weight[1] > -OL_EPS)
        weight[1] = 0;
      if (weight[1] > 1 && weight[1] < 1 + OL_EPS)
        weight[1] = 1;
      if (weight[1] < 0 || weight[1] > 1)
        test = -1;
    }

    if (test != -1)
    {
      /* rotation to (001)--(111) */
      /* calculating v3 = rotated vector to (011)-(111) about (-100) */

      ol_r_set_this (r, -1, 0, 0);

      ol_theta_rad2deg (atan2 (v[1] - v[2], v[1] + v[2]), &theta);

      ol_rtheta_g (r, theta, g);
      ol_g_vect_vect (g, v, v3);

      if (ut_array_1d_norm (weight, 2) < OL_EPS)
        weight[2] = 1;
      else
      {
        weight[2] =
          1 - acos ((v3[0] + v3[1] + v3[2]) / sqrt (3)) / acos (2 / sqrt (6));
        if (weight[2] < 0 && weight[2] > -OL_EPS)
          weight[2] = 0;
        if (weight[2] > 1 && weight[2] < 1 + OL_EPS)
          weight[2] = 1;
        if (weight[2] < 0 || weight[2] > 1)
          test = -1;
      }
    }
  }

  else if (!strcmp (crysym, "hexagonal"))
  {
    int i;
    double **g = ol_g_alloc ();
    double *p = ol_p_alloc ();

    int found = 0;
    for (i = 1; i <= 12; i++)
    {
      ol_g_crysym (NULL, "hexagonal", i, g);
      ol_g_vect_vect (g, v, v1);

      ol_vect_stproj (v1, p);

      if (p[1] >= 0 && p[1] <= 30 + OL_EPS)
      {
        found = 1;
        break;
      }
    }

    if (!found)
      abort ();

    weight[0] = 1 - p[0];
    weight[1] = p[0] * p[1] / 30;
    weight[2] = p[0] * (1 - p[1] / 30);

    ol_g_free (g);
    ol_p_free (p);
  }

  else
  {
    printf ("Crystal symmetry `%s' not supported.\n", crysym);
    abort ();
  }

  ol_g_free (g);
  ol_vect_free (v1);
  ol_vect_free (v2);
  ol_vect_free (v3);
  ol_r_free (r);

  return test;
}

void
ol_q_pf_stproj (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_pf_stproj (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_pf_stprojxy (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_pf_stprojxy (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_pf_stproj_rad (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_pf_stproj_rad (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_pf_eaproj (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_pf_eaproj (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_pf_eaprojxy (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_pf_eaprojxy (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_pf_eaproj_rad (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_pf_eaproj_rad (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_stproj (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_stproj (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_stprojxy (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_stprojxy (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_stproj_rad (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_stproj_rad (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_eaproj (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_eaproj (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_eaprojxy (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_eaprojxy (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_eaproj_rad (double *q, double *vect, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_eaproj_rad (g, vect, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_crysym_stproj (double *q, double *vect, char *crysym, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_crysym_stproj (g, vect, crysym, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_crysym_stprojxy (double *q, double *vect, char *crysym, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_crysym_stprojxy (g, vect, crysym, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_crysym_stproj_rad (double *q, double *vect, char *crysym, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_crysym_stproj_rad (g, vect, crysym, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_crysym_eaproj (double *q, double *vect, char *crysym, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_crysym_eaproj (g, vect, crysym, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_crysym_eaprojxy (double *q, double *vect, char *crysym, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_crysym_eaprojxy (g, vect, crysym, p);

  ol_g_free (g);

  return;
}

void
ol_q_ipf_crysym_eaproj_rad (double *q, double *vect, char *crysym, double *p)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_ipf_crysym_eaproj_rad (g, vect, crysym, p);

  ol_g_free (g);

  return;
}

void
ol_ipf_g_stprojxy (double *p, double *vect, double **g)
{
  double *OP = ol_vect_alloc ();

  ol_stprojxy_vect (p, OP);

  ol_vect_vect_g (OP, vect, g);

  ol_vect_free (OP);

  return;
}

void
ol_ipf_q_stprojxy (double *p, double *vect, double *q)
{
  double **g = ol_g_alloc ();

  ol_ipf_g_stprojxy (p, vect, g);
  ol_g_q (g, q);

  return;
}
