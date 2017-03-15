/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_m.h"

int *
ol_m_alloc ()
{
  int *m = ut_alloc_1d_int (6);

  m[2] = 1;
  m[3] = 1;

  return m;
}

void
ol_m_free (int *m)
{
  if (!m)
    return;
  else
    free (m);

  return;
}

void
ol_m_set_zero (int *m)
{
  ut_array_1d_int_zero (m, 6);

  return;
}

void
ol_m_set_id (int *m)
{
  ut_array_1d_int_zero (m, 6);
  m[2] = 1;
  m[3] = 1;

  return;
}

void
ol_m_set_this (int *m, int h, int k, int l, int u, int v, int w)
{
  m[0] = h;
  m[1] = k;
  m[2] = l;
  m[3] = u;
  m[4] = v;
  m[5] = w;

  return;
}

void
ol_m_memcpy (int *msrc, int *mdest)
{
  ut_array_1d_int_memcpy (mdest, 6, msrc);

  return;
}

void
ol_m_g (int *m, double **g)
{
  double M, N;

  M =
    sqrt ((double) m[0] * (double) m[0] + (double) m[1] * (double) m[1] +
	  (double) m[2] * (double) m[2]);
  N =
    sqrt ((double) m[3] * (double) m[3] + (double) m[4] * (double) m[4] +
	  (double) m[5] * (double) m[5]);

  g[0][0] = (double) m[3] / N;
  g[1][0] = (double) m[4] / N;
  g[2][0] = (double) m[5] / N;

  g[0][1] =
    ((double) m[1] * (double) m[5] - (double) m[2] * (double) m[4]) / (M * N);
  g[1][1] =
    ((double) m[2] * (double) m[3] - (double) m[0] * (double) m[5]) / (M * N);
  g[2][1] =
    ((double) m[0] * (double) m[4] - (double) m[1] * (double) m[3]) / (M * N);

  g[0][2] = (double) m[0] / M;
  g[1][2] = (double) m[1] / M;
  g[2][2] = (double) m[2] / M;

  return;
}

void
ol_g_m (double **g, char* crysym, int maxid, int maxres, int **allm, double *mstat,
	int *pqty)
{
  ol_g_m_quality (g, crysym, maxid, maxres, 0.5, allm, mstat, pqty);

  return;
}

void
ol_g_m_quality (double **g, char* crysym, int maxid, int maxres, double alpha, int **allm, double *mstat, int *pqty)
{
  int i, j, test, pos, hklpos, uvwpos, ID;
  double A;
  double *v = ut_alloc_1d (3);
  int *vsgn = ut_alloc_1d_int (3);
  double *vp = ut_alloc_1d (3);
  double *a = ut_alloc_1d (3);
  int *qty = ut_alloc_1d_int (2);

  int ***id = ut_alloc_3d_int (2, 5 * maxid, 3);	/* empirical */
  double *theta, *mq;
  int *Sig;
  int *srt;

  int *m = ol_m_alloc ();
  double **g2 = ol_g_alloc ();
  int scalprod, resqty;

  /*** Building a set of potential miller indices. ***/
  for (ID = 0; ID <= 1; ID++)
  {
    if (ID == 0)		/* Searching hkl */
      for (i = 0; i < 3; i++)
      {
	v[i] = fabs (g[i][2]);
	vsgn[i] = ut_num_sgn (g[i][2]);
      }
    else			/* Searching uvw */
      for (i = 0; i < 3; i++)
      {
	v[i] = fabs (g[i][0]);
	vsgn[i] = ut_num_sgn (g[i][0]);
      }

    qty[ID] = 0;
    ut_array_1d_zero (vp, 3);
    do
    {
      for (i = 0; i < 3; i++)
	a[i] = (floor (vp[i] + 0.5) + 0.5) / v[i];
      A = ut_array_1d_min (a, 3);
      /* A is the number by which to multiply v to get the
       * next id (one index incremented) */

      for (i = 0; i < 3; i++)
      {
	vp[i] = A * (1 + 1e-6) * v[i];
	id[ID][qty[ID]][i] = ut_num_d2ri (vp[i]);
      }

      test = 1;			/* if test = 1, recording under conditions; else, no */
      for (j = 0; j < qty[ID]; j++)	/* checking whether not the same as previously */
	if (ut_vector_int_angle (id[ID][j], id[ID][qty[ID]]) < 1e-3)
	{
	  test = 0;
	  break;
	}

      if (test == 1 && ut_array_1d_int_max (id[ID][qty[ID]], 3) <= maxid)
	qty[ID]++;
    }
    while (ut_array_1d_max (vp, 3) <= maxid + .5);

    for (i = 0; i < qty[ID]; i++)
      for (j = 0; j < 3; j++)
	id[ID][i][j] *= vsgn[j];
  }
  /*** Built a set of potential miller indices. ***/

  theta = ut_alloc_1d (qty[0] * qty[1]);
  Sig = ut_alloc_1d_int (qty[0] * qty[1]);
  mq = ut_alloc_1d (qty[0] * qty[1]);
  srt = ut_alloc_1d_int (qty[0] * qty[1]);

  resqty = qty[0] * qty[1];
  for (i = 0; i < qty[0]; i++)
  {
    ut_array_1d_int_memcpy (m, 3, id[0][i]);

    for (j = 0; j < qty[1]; j++)
    {
      pos = i * qty[1] + j;

      ut_array_1d_int_memcpy (m + 3, 3, id[1][j]);

      scalprod = m[0] * m[3] + m[1] * m[4] + m[2] * m[5];
      if (scalprod != 0)
	resqty--;

      ol_m_g (m, g2);
      ol_g_g_disori (g, g2, crysym, &(theta[pos]));

      Sig[pos] = (fabs (m[0]) + fabs (m[1]) + fabs (m[2])) +
	(fabs (m[3]) + fabs (m[4]) + fabs (m[5]));

      mq[pos] =
	(1 - alpha) * (double) Sig[pos] + alpha * theta[pos] +
	(double) (fabs (scalprod) * 1e7);
      /* if scalprod != 0, mq is set to a very high value to reject the
       * set of indices. */
    }
  }

  ut_array_1d_sort_index (mq, qty[0] * qty[1], srt);

  (*pqty) = ut_num_min_int (resqty, maxres);
  for (i = 0; i < (*pqty); i++)
  {
    pos = srt[i];
    hklpos = pos / qty[1];
    uvwpos = pos % qty[1];

    ut_array_1d_int_memcpy (allm[i], 3, id[0][hklpos]);
    ut_array_1d_int_memcpy (allm[i] + 3, 3, id[1][uvwpos]);
    mstat[i] = mq[pos];
  }

  /* freeing memory */
  ut_free_1d (v);
  ut_free_1d_int (vsgn);
  ut_free_1d (vp);
  ut_free_1d (a);
  ut_free_1d_int (qty);

  ut_free_3d_int (id, 2, 5 * maxid);
  ut_free_1d (theta);
  ut_free_1d_int (Sig);
  ut_free_1d (mq);
  ut_free_1d_int (srt);

  ol_m_free (m);
  ol_g_free (g2);

  return;
}

void
ol_e_m (double *e, char* crysym, int maxid, int maxres, int **allm, double *mstat,
	int *pqty)
{
  double **g = ol_g_alloc ();
  ol_e_g (e, g);

  ol_g_m_quality (g, crysym, maxid, maxres, 0.5, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_e_m_quality (double *e, char* crysym, int maxid, int maxres, double alpha, int **allm, double *mstat, int *pqty)
{
  double **g = ol_g_alloc ();
  ol_e_g (e, g);

  ol_g_m_quality (g, crysym, maxid, maxres, alpha, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_rtheta_m (double *r, double theta, char* crysym, int maxid, int maxres, int **allm, double *mstat, int *pqty)
{
  double **g = ol_g_alloc ();
  ol_rtheta_g (r, theta, g);

  ol_g_m_quality (g, crysym, maxid, maxres, 0.5, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_rtheta_m_quality (double *r, double theta, char* crysym, int maxid, int maxres, double alpha, int **allm, double *mstat, int *pqty)
{
  double **g = ol_g_alloc ();
  ol_rtheta_g (r, theta, g);

  ol_g_m_quality (g, crysym, maxid, maxres, alpha, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_R_m (double *R, char* crysym, int maxid, int maxres, int **allm, double *mstat,
	int *pqty)
{
  double **g = ol_g_alloc ();
  ol_R_g (R, g);

  ol_g_m_quality (g, crysym, maxid, maxres, 0.5, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_R_m_quality (double *R, char* crysym, int maxid, int maxres, double alpha, int **allm, double *mstat, int *pqty)
{
  double **g = ol_g_alloc ();
  ol_R_g (R, g);

  ol_g_m_quality (g, crysym, maxid, maxres, alpha, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_q_m (double *q, char* crysym, int maxid, int maxres, int **allm, double *mstat,
	int *pqty)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_m_quality (g, crysym, maxid, maxres, 0.5, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

void
ol_q_m_quality (double *q, char* crysym, int maxid, int maxres, double alpha, int **allm, double *mstat, int *pqty)
{
  double **g = ol_g_alloc ();
  ol_q_g (q, g);

  ol_g_m_quality (g, crysym, maxid, maxres, alpha, allm, mstat, pqty);

  ol_g_free (g);

  return;
}

int
ol_m_fscanf (FILE * file, int *m)
{
  if (fscanf (file, "%d%d%d%d%d%d",
	      &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]) == 6)
    return 1;
  else
    return 0;
}

int
ol_m_fprintf (FILE * file, int *m, char *format)
{
  int i;

  for (i = 0; i <= 5; i++)
  {
    fprintf (file, format, m[i]);
    fprintf (file, " ");
  }
  return fprintf (file, "\n");
}

void
ol_m_e (int *m, double *e)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_e (g, e);

  ol_g_free (g);

  return;
}

void
ol_m_e_rad (int *m, double *e)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_e_rad (g, e);

  ol_g_free (g);

  return;
}

void
ol_m_q (int *m, double *q)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_q (g, q);

  ol_g_free (g);

  return;
}

void
ol_m_rtheta (int *m, double *r, double *ptheta)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_rtheta (g, r, ptheta);

  ol_g_free (g);

  return;
}

void
ol_m_rtheta_rad (int *m, double *r, double *ptheta)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_rtheta_rad (g, r, ptheta);

  ol_g_free (g);

  return;
}

void
ol_m_r (int *m, double *r)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_r (g, r);

  ol_g_free (g);

  return;
}

void
ol_m_theta_rad (int *m, double *ptheta)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_theta_rad (g, ptheta);

  ol_g_free (g);

  return;
}

void
ol_m_theta (int *m, double *ptheta)
{
  ol_m_theta_rad (m, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_m_R (int *m, double *R)
{
  double **g = ol_g_alloc ();

  ol_m_g (m, g);
  ol_g_R (g, R);

  ol_g_free (g);

  return;
}
