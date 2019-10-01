/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_dis.h"

int
ol_g_g_gdisori (double **g1, double **g2, char* crysym, double **gdis)
{
  int i, nb;
  double theta, mintheta;
  double **g1inv = ol_g_alloc ();
  double **g2rot = ol_g_alloc ();
  double **gtmp  = ol_g_alloc ();
  int qty = ol_lauegroup_qty (crysym);

  ol_g_inverse (g1, g1inv);

  mintheta = 1e3;
  nb = 1;
  for (i = 1; i <= qty; i++)
  {
    ol_g_crysym (g2, crysym, i, g2rot);
    ol_g_g_g_ref (g1inv, g2rot, gtmp);
    ol_g_theta_rad (gtmp, &theta);
    if (theta < mintheta)
    {
      mintheta = theta;
      nb = i;
    }
  }

  ol_g_crysym (g2, crysym, nb, g2rot);
  ol_g_g_g_ref (g1inv, g2rot, gdis);

  ol_g_free (g1inv);
  ol_g_free (g2rot);
  ol_g_free (gtmp);

  return nb;
}

int
ol_g_g_gdisori_ref (double **g1, double **g2, char* crysym, double **gdis)
{
  return ol_g_g_gdisori (g1, g2, crysym, gdis);
}

int
ol_g_g_gdisori_cur (double **g1, double **g2, char* crysym, double **gdis)
{
  int i, nb;
  double theta, mintheta;
  double **g1inv = ol_g_alloc ();
  double **g2g1inv = ol_g_alloc ();
  int qty = ol_lauegroup_qty (crysym);

  ol_g_inverse (g1, g1inv);
  ol_g_g_g (g1inv, g2, g2g1inv);

  mintheta = 1e3;
  nb = 1;
  for (i = 1; i <= qty; i++)
  {
    ol_g_crysym (g2g1inv, crysym, i, gdis);
    ol_g_theta_rad (gdis, &theta);
    if (theta < mintheta)
    {
      mintheta = theta;
      nb = i;
    }
  }

  ol_g_crysym (g2g1inv, crysym, nb, gdis);

  ol_g_free (g1inv);
  ol_g_free (g2g1inv);

  return nb;
}

/* rapid function on which other quaternion functions are based */
int
ol_q_q_disori_rad (double *q1, double *q2, char* crysym, double *ptheta)
{
  int i, nb;
  double rho, maxrho, crysym_maxrho;
  double *qd = ol_q_alloc ();
  int qty = ol_lauegroup_qty (crysym);

  double Tcubic[25][4] = {
    {8888, 8888, 8888, 8888},	/* phantom */
    {1, 0, 0, 0},
    {OL_IS2, OL_IS2, 0, 0},
    {0, 1, 0, 0},
    {OL_IS2, -OL_IS2, 0, 0},
    {OL_IS2, 0, OL_IS2, 0},
    {0, 0, 1, 0},
    {OL_IS2, 0, -OL_IS2, 0},
    {OL_IS2, 0, 0, OL_IS2},
    {0, 0, 0, 1},
    {OL_IS2, 0, 0, -OL_IS2},
    {0, OL_IS2, OL_IS2, 0},
    {0, 0, OL_IS2, OL_IS2},
    {0, OL_IS2, 0, OL_IS2},
    {0, OL_IS2, -OL_IS2, 0},
    {0, 0, OL_IS2, -OL_IS2},
    {0, OL_IS2, 0, -OL_IS2},
    {.5, .5, .5, .5},
    {.5, -.5, -.5, -.5},
    {.5, -.5, .5, .5},
    {.5, .5, -.5, -.5},
    {.5, .5, -.5, .5},
    {.5, -.5, .5, -.5},
    {.5, .5, .5, -.5},
    {.5, -.5, -.5, .5}
  };

  double Thexa[13][4] = {
    {1.000000000, 0.000000000, 0.000000000 ,0.000000000}, // phantom
    {1.000000000, 0.000000000, 0.000000000 ,0.000000000},
    {0.866025404, 0.000000000, 0.000000000 ,0.500000000},
    {0.500000000, 0.000000000, 0.000000000 ,0.866025404},
    {0.000000000, 0.000000000, 0.000000000 ,1.000000000},
    {0.500000000, 0.000000000, 0.000000000 ,-0.866025404},
    {0.866025404, 0.000000000, 0.000000000 ,-0.500000000},
    {0.000000000, 0.866025404, -0.500000000 ,0.000000000},
    {0.000000000, -0.500000000, 0.866025404 ,0.000000000},
    {0.000000000, 0.000000000, 1.000000000 ,0.000000000},
    {0.000000000, 0.500000000, 0.866025404 ,0.000000000},
    {0.000000000, 0.866025404, 0.500000000 ,0.000000000},
    {0.000000000, 1.000000000, 0.000000000 ,0.000000000}
  };

  /* computation of q1^{-1}.q2 */
  ol_q_inverse (q1, qd);
  ol_q_q_q (qd, q2, qd);

  if (strcmp (crysym, "cubic") == 0)
  {
    crysym_maxrho = .92387953251128675613; /* = cos (45/2) */

    maxrho = 0;
    nb = 1;
    for (i = 1; i <= qty; i++)
    {
      rho = fabs (qd[0] * Tcubic[i][0] - qd[1] * Tcubic[i][1]
		- qd[2] * Tcubic[i][2] - qd[3] * Tcubic[i][3]);
      if (rho > maxrho)
      {
	nb = i;
	maxrho = rho;
	if (maxrho > crysym_maxrho)
	  break;
      }
    }
  }
  else if (strcmp (crysym, "hexagonal") == 0)
  {
    crysym_maxrho = DBL_MAX;

    maxrho = 0;
    nb = 1;
    for (i = 1; i <= qty; i++)
    {
      rho = fabs (qd[0] * Thexa[i][0] - qd[1] * Thexa[i][1]
		- qd[2] * Thexa[i][2] - qd[3] * Thexa[i][3]);
      if (rho > maxrho)
      {
	nb = i;
	maxrho = rho;
	if (maxrho > crysym_maxrho)
	  break;
      }
    }
  }
  else if (strcmp (crysym, "triclinic") == 0)
  {
    maxrho = fabs (qd[0]);
    nb = 1;
  }
  else
    abort ();

  (*ptheta) = 2 * ut_num_acos (maxrho);

  ol_q_free (qd);

  return nb;
}

/* rapid function on which other quaternion functions are based */
int
ol_q_q_disori_fr_rad (double *q1, double *q2, char* crysym, double *ptheta)
{
  int i, nb;
  double rho, maxrho, crysym_maxrho;
  double *qd = ol_q_alloc ();

  double Tcubic[15][4] = {
    {1, 0, 0, 0},		// 1
    {OL_IS2, OL_IS2, 0, 0},     // 2
    {OL_IS2, -OL_IS2, 0, 0},    // 4
    {OL_IS2, 0, OL_IS2, 0},     // 5
    {OL_IS2, 0, -OL_IS2, 0},	// 7
    {OL_IS2, 0, 0, OL_IS2},	// 8
    {OL_IS2, 0, 0, -OL_IS2},	// 10
    {.5, .5, .5, .5},		// 17
    {.5, -.5, -.5, -.5},	// 18
    {.5, -.5, .5, .5},		// 19
    {.5, .5, -.5, -.5},		// 20
    {.5, .5, -.5, .5},		// 21
    {.5, -.5, .5, -.5},		// 22
    {.5, .5, .5, -.5},		// 23
    {.5, -.5, -.5, .5}		// 24
  };

  int Tcubic_number[15] = {1, 2, 4, 5, 7, 8, 10, 17, 18, 19, 20, 21, 22,
    23, 24};

  /* computation of q1^{-1}.q2 */
  ol_q_inverse (q1, qd);
  ol_q_q_q (qd, q2, qd);

  if (strcmp (crysym, "cubic") == 0)
    crysym_maxrho = .92387953251128675613; /* = cos (45/2) */
  else
    crysym_maxrho = 1e3;

  maxrho = 0;
  nb = 1;
  for (i = 0; i < 15; i++)
  {
    rho = fabs (qd[0] * Tcubic[i][0] - qd[1] * Tcubic[i][1]
	      - qd[2] * Tcubic[i][2] - qd[3] * Tcubic[i][3]);
    if (rho > maxrho)
    {
      nb = Tcubic_number[i];
      maxrho = rho;
      if (maxrho > crysym_maxrho)
	break;
    }
  }

  (*ptheta) = 2 * ut_num_acos (maxrho);

  ol_q_free (qd);

  return nb;
}

int
ol_q_q_disori (double *q1, double *q2, char* crysym, double *ptheta)
{
  int nb;

  nb = ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return nb;
}

int
ol_q_q_qdisori (double *q1, double *q2, char* crysym, double *qd)
{
  int nb;
  double theta;
  double *q1inv = ol_q_alloc ();
  double *q2rot = ol_q_alloc ();

  nb = ol_q_q_disori_rad (q1, q2, crysym, &theta);

  ol_q_inverse (q1, q1inv);
  ol_q_crysym (q2, crysym, nb, q2rot);
  ol_q_q_q_ref (q1inv, q2rot, qd);
  ol_q_q (qd, qd);

  ol_q_free (q1inv);
  ol_q_free (q2rot);

  return nb;
}

int
ol_q_q_qdisori_update (double *q1, double *q2, char* crysym, int prevnb,
		       double *qd, char* flag, int *pstatus)
{
  int nb, status;
  double theta;
  double *q1inv = ol_q_alloc ();
  double *q2rot = ol_q_alloc ();
  double cubic_maxrho = .92387953251128675613; /* = cos (45/2) */
  ol_q_set_zero (qd);

  nb = 0;

  status = 0;

  if (!strcmp (crysym, "cubic") && prevnb >= 1 && prevnb <= 24)
  {
    ol_q_inverse (q1, q1inv);
    ol_q_crysym (q2, crysym, prevnb, q2rot);
    ol_q_q_q_ref (q1inv, q2rot, qd);
    ol_q_q (qd, qd);
    if (qd[0] > cubic_maxrho)
    {
      nb = prevnb;
      status = 1;
    }
  }

  else if (!strcmp (crysym, "triclinic"))
  {
    ol_q_inverse (q1, q1inv);
    ol_q_q_q_ref (q1inv, q2, qd);
    ol_q_q (qd, qd);
    nb = 1;
    status = 1;
  }

  if (! status)
  {
    if (flag && !strcmp (flag, "fr"))
      nb = ol_q_q_disori_fr_rad (q1, q2, crysym, &theta);
    else
      nb = ol_q_q_disori_rad (q1, q2, crysym, &theta);

    ol_q_inverse (q1, q1inv);
    ol_q_crysym (q2, crysym, nb, q2rot);
    ol_q_q_q_ref (q1inv, q2rot, qd);
    ol_q_q (qd, qd);
  }

  if (pstatus)
    (*pstatus) = status;

  ol_q_free (q1inv);
  ol_q_free (q2rot);

  return nb;
}

int
ol_q_q_qdisori_ref (double *q1, double *q2, char* crysym, double *qd)
{
  return ol_q_q_qdisori (q1, q2, crysym, qd);
}

int
ol_q_q_qdisori_cur (double *q1, double *q2, char* crysym, double *qd)
{
  int nb;
  double theta;
  double *q1inv = ol_q_alloc ();
  double *q2rot = ol_q_alloc ();

  nb = ol_q_q_disori_rad (q1, q2, crysym, &theta);

  ol_q_inverse (q1, q1inv);
  ol_q_crysym (q2, crysym, nb, q2rot);
  ol_q_q_q (q1inv, q2rot, qd);
  ol_q_q (qd, qd);

  ol_q_free (q1inv);
  ol_q_free (q2rot);

  return nb;
}

void
ol_vect_vect_theta_rad (double *v1, double *v2, double *ptheta)
{
  double norm1, norm2;

  norm1 = ut_vector_norm (v1);
  norm2 = ut_vector_norm (v2);

  (*ptheta) = acos (ut_vector_scalprod (v1, v2) / (norm1 * norm2));
  if (isNaN ((*ptheta)) == 1)
    (*ptheta) = 0;

  return;
}

void
ol_vect_vect_theta (double *v1, double *v2, double *ptheta)
{
  ol_vect_vect_theta_rad (v1, v2, ptheta);
  ol_theta_rad2deg ((*ptheta), ptheta);

  return;
}

void
ol_vect_vect_rtheta_rad (double *v1, double *v2, double *r, double *ptheta)
{
  double norm;

  ol_vect_vect_theta_rad (v1, v2, ptheta);

  if (*ptheta > OL_EPS_RAD && *ptheta < M_PI - OL_EPS_RAD)
  {
    ut_vector_vectprod (v1, v2, r);
    norm = ut_vector_norm (r);
    ut_array_1d_scale (r, 3, 1 / norm);
  }
  else
    ol_r_set_id (r);

  return;
}

void
ol_vect_vect_rtheta (double *v1, double *v2, double *r, double *ptheta)
{
  ol_vect_vect_rtheta_rad (v1, v2, r, ptheta);
  ol_theta_rad2deg ((*ptheta), ptheta);

  return;
}

void
ol_vect_vect_g (double *v1, double *v2, double **g)
{
  double theta, *r = ol_r_alloc ();

  ol_vect_vect_rtheta (v1, v2, r, &theta);
  ol_rtheta_g (r, theta, g);

  return;
}

void
ol_vect_vect_q (double *v1, double *v2, double *q)
{
  double theta, *r = ol_r_alloc ();

  ol_vect_vect_rtheta (v1, v2, r, &theta);
  ol_rtheta_q (r, theta, q);

  return;
}

int
ol_g_g_gmisori (double **g1, double **g2, double **gmis)
{
  double **g1inv = ol_g_alloc ();

  ol_g_inverse (g1, g1inv);
  ol_g_g_g_ref (g1inv, g2, gmis);

  ol_g_free (g1inv);

  return 1;
}

int
ol_q_q_qmisori (double *q1, double *q2, double *qmis)
{
  double *q1inv = ol_q_alloc ();

  ol_q_inverse (q1, q1inv);
  ol_q_q_q_ref (q1inv, q2, qmis);
  ol_q_q (qmis, qmis);

  ol_q_free (q1inv);

  return 1;
}

int
ol_g_g_gmisori_ref (double **g1, double **g2, double **gdis)
{
  return ol_g_g_gmisori (g1, g2, gdis);
}

int
ol_q_q_qmisori_ref (double *q1, double *q2, double *qdis)
{
  return ol_q_q_qmisori (q1, q2, qdis);
}

int
ol_g_g_gmisori_cur (double **g1, double **g2, double **gmis)
{
  double **g1inv = ol_g_alloc ();

  ol_g_inverse (g1, g1inv);
  ol_g_g_g (g1inv, g2, gmis);

  ol_g_free (g1inv);

  return 1;
}

int
ol_q_q_qmisori_cur (double *q1, double *q2, double *qmis)
{
  double *q1inv = ol_q_alloc ();

  ol_q_inverse (q1, q1inv);
  ol_q_q_q (q1inv, q2, qmis);
  ol_q_q (qmis, qmis);

  ol_q_free (q1inv);

  return 1;
}

int
ol_g_g_misori_rad (double **g1, double **g2, double *ptheta)
{
  /* may be improved */
  double **gd = ol_g_alloc ();

  ol_g_inverse (g1, gd);
  ol_g_g_g (gd, g2, gd);
  ol_g_theta_rad (gd, ptheta);

  ol_g_free (gd);

  return 1;
}

int
ol_g_g_misori (double **g1, double **g2, double *ptheta)
{
  ol_g_g_misori_rad (g1, g2, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return 1;
}

int
ol_q_q_misori_rad (double *q1, double *q2, double *ptheta)
{
  /* may be improved */
  double *qd = ol_q_alloc ();

  ol_q_inverse (q1, qd);
  ol_q_q_q (qd, q2, qd);
  ol_q_q (qd, qd);
  (*ptheta) = 2 * ut_num_acos (qd[0]);

  ol_q_free (qd);

  return 1;
}

int
ol_q_q_misori (double *q1, double *q2, double *ptheta)
{
  ol_q_q_misori_rad (q1, q2, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return 1;
}

/* for the sake of convenience and efficiency, quaternions are used. */
int
ol_g_g_disori_rad (double **g1, double **g2, char* crysym, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_g_q (g1, q1);
  ol_g_q (g2, q2);

  ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_g_g_disori (double **g1, double **g2, char* crysym, double *ptheta)
{
  ol_g_g_disori_rad (g1, g2, crysym, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return 1;
}

int
ol_e_e_disori_rad (double *e1, double *e2, char* crysym, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_e_e_disori (double *e1, double *e2, char* crysym, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  ol_q_q_disori (q1, q2, crysym, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_e_e_gdisori (double *e1, double *e2, char* crysym, double **g)
{
  int res;
  double **g1 = ol_g_alloc ();
  double **g2 = ol_g_alloc ();

  ol_e_g (e1, g1);
  ol_e_g (e2, g2);

  res = ol_g_g_gdisori (g1, g2, crysym, g);

  ol_g_free (g1);
  ol_g_free (g2);

  return res;
}

int
ol_e_e_gmisori (double *e1, double *e2, double **g)
{
  int res;
  double **g1 = ol_g_alloc ();
  double **g2 = ol_g_alloc ();

  ol_e_g (e1, g1);
  ol_e_g (e2, g2);

  res = ol_g_g_gmisori (g1, g2, g);

  ol_g_free (g1);
  ol_g_free (g2);

  return res;
}

int
ol_e_e_qdisori (double *e1, double *e2, char* crysym, double *q)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  res = ol_q_q_qdisori (q1, q2, crysym, q);
  ol_q_q (q, q);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_e_e_qmisori (double *e1, double *e2, double *q)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  res = ol_q_q_qmisori (q1, q2, q);
  ol_q_q (q, q);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_e_e_misori_rad (double *e1, double *e2, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  ol_q_q_misori_rad (q1, q2, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_e_e_misori (double *e1, double *e2, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  ol_q_q_misori (q1, q2, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_m_m_disori (int *m1, int *m2, char* crysym, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_m_q (m1, q1);
  ol_m_q (m2, q2);

  ol_q_q_disori (q1, q2, crysym, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_m_m_gdisori (int *m1, int *m2, char* crysym, double **g)
{
  int res;
  double **g1 = ol_g_alloc ();
  double **g2 = ol_g_alloc ();

  ol_m_g (m1, g1);
  ol_m_g (m2, g2);

  res = ol_g_g_gdisori (g1, g2, crysym, g);

  ol_g_free (g1);
  ol_g_free (g2);

  return res;
}

int
ol_m_m_gmisori (int *m1, int *m2, double **g)
{
  int res;
  double **g1 = ol_g_alloc ();
  double **g2 = ol_g_alloc ();

  ol_m_g (m1, g1);
  ol_m_g (m2, g2);

  res = ol_g_g_gmisori (g1, g2, g);

  ol_g_free (g1);
  ol_g_free (g2);

  return res;
}

int
ol_m_m_qdisori (int *m1, int *m2, char* crysym, double *q)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_m_q (m1, q1);
  ol_m_q (m2, q2);

  res = ol_q_q_qdisori (q1, q2, crysym, q);
  ol_q_q (q, q);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_m_m_qmisori (int *m1, int *m2, double *q)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_m_q (m1, q1);
  ol_m_q (m2, q2);

  res = ol_q_q_qmisori (q1, q2, q);
  ol_q_q (q, q);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_m_m_misori (int *m1, int *m2, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_m_q (m1, q1);
  ol_m_q (m2, q2);

  ol_q_q_misori (q1, q2, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_R_R_disori_rad (double *R1, double *R2, char* crysym, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_R_q (R1, q1);
  ol_R_q (R2, q2);

  ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_R_R_disori (double *R1, double *R2, char* crysym, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_R_q (R1, q1);
  ol_R_q (R2, q2);

  ol_q_q_disori (q1, q2, crysym, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_R_R_misori_rad (double *R1, double *R2, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_R_q (R1, q1);
  ol_R_q (R2, q2);

  ol_q_q_misori_rad (q1, q2, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_R_R_misori (double *R1, double *R2, double *ptheta)
{
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_R_q (R1, q1);
  ol_R_q (R2, q2);

  ol_q_q_misori (q1, q2, ptheta);

  ol_q_free (q1);
  ol_q_free (q2);

  return 1;
}

int
ol_g_g_disori_orthotropic_rad (double **g1, double **g2, char* crysym, double *pdis)
{
  double distmp;
  int i, qty;
  double ***gsym = ut_alloc_3d (4, 3, 3);

  ol_g_refsym_orthotropic (g1, gsym, &qty);

  (*pdis) = 180;
  for (i = 0; i < qty; i++)
  {
    ol_g_g_disori_rad (gsym[i], g2, crysym, &distmp);
    if (distmp < (*pdis))
      (*pdis) = distmp;
  }

  ut_free_3d (gsym, 3, 3);

  return 1;
}

int
ol_g_g_misori_orthotropic_rad (double **g1, double **g2, double *pdis)
{
  double distmp;
  int i, qty;
  double ***gsym = ut_alloc_3d (4, 3, 3);

  ol_g_refsym_orthotropic (g1, gsym, &qty);

  (*pdis) = 180;
  for (i = 0; i < qty; i++)
  {
    ol_g_g_misori_rad (gsym[i], g2, &distmp);
    if (distmp < (*pdis))
      (*pdis) = distmp;
  }

  ut_free_3d (gsym, 3, 3);

  return 1;
}

int
ol_g_g_gdisori_orthotropic (double **g1, double **g2, char* crysym, double **gdis)
{
  int i, id;
  double ***gsym = ut_alloc_3d (4, 3, 3);
  double ***gd = ut_alloc_3d (4, 3, 3);
  double *dis = ut_alloc_1d (4);

  ol_g_refsym_orthotropic_all (g1, gsym);

  for (i = 0; i < 4; i++)
  {
    ol_g_g_gdisori (gsym[i], g2, crysym, gd[i]);
    ol_g_theta (gd[i], &(dis[i]));
  }

  id = ut_array_1d_min_index (dis, 4);
  ol_g_memcpy (gd[id], gdis);

  ut_free_3d (gsym, 3, 3);
  ut_free_3d (gd, 3, 3);
  ut_free_1d (dis);

  return id + 1;
}

int
ol_g_g_gmisori_orthotropic (double **g1, double **g2, double **gdis)
{
  int i, id;
  double ***gsym = ut_alloc_3d (4, 3, 3);
  double ***gd = ut_alloc_3d (4, 3, 3);
  double *dis = ut_alloc_1d (4);

  ol_g_refsym_orthotropic_all (g1, gsym);

  for (i = 0; i < 4; i++)
  {
    ol_g_g_gmisori (gsym[i], g2, gd[i]);
    ol_g_theta (gd[i], &(dis[i]));
  }

  id = ut_array_1d_min_index (dis, 4);
  ol_g_memcpy (gd[id], gdis);

  ut_free_3d (gsym, 3, 3);
  ut_free_3d (gd, 3, 3);
  ut_free_1d (dis);

  return id + 1;
}

int
ol_q_q_qdisori_orthotropic (double *q1, double *q2, char* crysym, double *qdis)
{
  int i, id;
  double **qsym = ut_alloc_2d (4, 4);
  double **qd = ut_alloc_2d (4, 4);
  double *dis = ut_alloc_1d (4);

  ol_q_refsym_orthotropic_all (q1, qsym);

  for (i = 0; i < 4; i++)
  {
    ol_q_q_qdisori (qsym[i], q2, crysym, qd[i]);
    ol_q_theta (qd[i], &(dis[i]));
  }

  id = ut_array_1d_min_index (dis, 4);
  ol_q_memcpy (qd[id], qdis);

  ut_free_2d (qsym, 4);
  ut_free_2d (qd, 4);
  ut_free_1d (dis);

  return id + 1;
}

int
ol_q_q_qmisori_orthotropic (double *q1, double *q2, double *qdis)
{
  int i, id;
  double **qsym = ut_alloc_2d (4, 4);
  double **qd = ut_alloc_2d (4, 4);
  double *dis = ut_alloc_1d (4);

  ol_q_refsym_orthotropic_all (q1, qsym);

  for (i = 0; i < 4; i++)
  {
    ol_q_q_qmisori (qsym[i], q2, qd[i]);
    ol_q_theta (qd[i], &(dis[i]));
  }

  id = ut_array_1d_min_index (dis, 4);
  ol_q_memcpy (qd[id], qdis);

  ut_free_2d (qsym, 4);
  ut_free_2d (qd, 4);
  ut_free_1d (dis);

  return id + 1;
}

int
ol_g_g_disori_orthotropic (double **g1, double **g2, char* crysym, double *pdis)
{
  ol_g_g_disori_orthotropic_rad (g1, g2, crysym, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return 1;
}

int
ol_g_g_misori_orthotropic (double **g1, double **g2, double *pdis)
{
  ol_g_g_misori_orthotropic_rad (g1, g2, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return 1;
}

int
ol_q_q_disori_orthotropic_rad (double *q1, double *q2, char* crysym, double *pdis)
{
  double distmp;
  int i, qty;
  double **qsym = ut_alloc_2d (4, 4);

  ol_q_refsym_orthotropic (q1, qsym, &qty);

  (*pdis) = 180;
  for (i = 0; i < qty; i++)
  {
    ol_q_q_disori_rad (qsym[i], q2, crysym, &distmp);
    if (distmp < (*pdis))
      (*pdis) = distmp;
  }

  ut_free_2d (qsym, 4);

  return 1;
}

int
ol_q_q_disori_orthotropic (double *q1, double *q2, char* crysym, double *pdis)
{
  ol_q_q_disori_orthotropic_rad (q1, q2, crysym, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return 1;
}

int
ol_q_q_misori_orthotropic_rad (double *q1, double *q2, double *pdis)
{
  double distmp;
  int i, qty;
  double **qsym = ut_alloc_2d (4, 4);

  ol_q_refsym_orthotropic (q1, qsym, &qty);

  (*pdis) = 180;
  for (i = 0; i < qty; i++)
  {
    ol_q_q_misori_rad (qsym[i], q2, &distmp);
    if (distmp < (*pdis))
      (*pdis) = distmp;
  }

  ut_free_2d (qsym, 4);

  return 1;
}

int
ol_q_q_misori_orthotropic (double *q1, double *q2, double *pdis)
{
  ol_q_q_misori_orthotropic_rad (q1, q2, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return 1;
}

int
ol_e_e_disori_orthotropic_rad (double *e1, double *e2, char* crysym, double *pdis)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q_rad (e1, q1);
  ol_e_q_rad (e2, q2);

  res = ol_q_q_disori_orthotropic_rad (q1, q2, crysym, pdis);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_e_e_disori_orthotropic (double *e1, double *e2, char* crysym, double *pdis)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  res = ol_q_q_disori_orthotropic (q1, q2, crysym, pdis);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_m_m_disori_orthotropic (int *m1, int *m2, char* crysym, double *pdis)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_m_q (m1, q1);
  ol_m_q (m2, q2);

  res = ol_q_q_disori_orthotropic (q1, q2, crysym, pdis);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_m_m_misori_orthotropic (int *m1, int *m2, double *pdis)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_m_q (m1, q1);
  ol_m_q (m2, q2);

  res = ol_q_q_misori_orthotropic (q1, q2, pdis);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_e_e_misori_orthotropic_rad (double *e1, double *e2, double *pdis)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q_rad (e1, q1);
  ol_e_q_rad (e2, q2);

  res = ol_q_q_misori_orthotropic_rad (q1, q2, pdis);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}

int
ol_e_e_misori_orthotropic (double *e1, double *e2, double *pdis)
{
  int res;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e1, q1);
  ol_e_q (e2, q2);

  res = ol_q_q_misori_orthotropic (q1, q2, pdis);

  ol_q_free (q1);
  ol_q_free (q2);

  return res;
}
