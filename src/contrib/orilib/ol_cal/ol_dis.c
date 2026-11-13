/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_dis.h"

int
ol_g_g_gdisori (double **g1, double **g2, char *crysym, double **gdis)
{
  int i, nb;
  double theta, mintheta;
  double **g1inv = ol_g_alloc ();
  double **g2rot = ol_g_alloc ();
  double **gtmp = ol_g_alloc ();
  int qty = ol_crysym_qty (crysym);

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

  ol_g_free (&g1inv);
  ol_g_free (&g2rot);
  ol_g_free (&gtmp);

  return nb;
}

int
ol_g_g_gdisori_ref (double **g1, double **g2, char *crysym, double **gdis)
{
  return ol_g_g_gdisori (g1, g2, crysym, gdis);
}

int
ol_g_g_gdisori_cur (double **g1, double **g2, char *crysym, double **gdis)
{
  int i, nb;
  double theta, mintheta;
  double **g1inv = ol_g_alloc ();
  double **g2g1inv = ol_g_alloc ();
  int qty = ol_crysym_qty (crysym);

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

  ol_g_free (&g1inv);
  ol_g_free (&g2g1inv);

  return nb;
}

/* rapid function on which other quaternion functions are based */
int
ol_q_q_disori_rad (double *q1, double *q2, char *crysym, double *ptheta)
{
  int i, nb;
  double rho, maxrho, crysym_maxrho;
  double *qd = ol_q_alloc ();
  int qty = ol_crysym_qty (crysym ? crysym : "triclinic");

  double Tcubic[25][4] = {
    {8888, 8888, 8888, 8888},   /* phantom */
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
    {1.000000000, 0.000000000, 0.000000000, 0.000000000},       // phantom
    {1.000000000, 0.000000000, 0.000000000, 0.000000000},
    {0.866025404, 0.000000000, 0.000000000, 0.500000000},
    {0.500000000, 0.000000000, 0.000000000, 0.866025404},
    {0.000000000, 0.000000000, 0.000000000, 1.000000000},
    {0.500000000, 0.000000000, 0.000000000, -0.866025404},
    {0.866025404, 0.000000000, 0.000000000, -0.500000000},
    {0.000000000, 0.866025404, -0.500000000, 0.000000000},
    {0.000000000, -0.500000000, 0.866025404, 0.000000000},
    {0.000000000, 0.000000000, 1.000000000, 0.000000000},
    {0.000000000, 0.500000000, 0.866025404, 0.000000000},
    {0.000000000, 0.866025404, 0.500000000, 0.000000000},
    {0.000000000, 1.000000000, 0.000000000, 0.000000000}
  };

  /* computation of q1^{-1}.q2 */
  ol_q_inverse (q1, qd);
  ol_q_q_q (qd, q2, qd);

  if (!crysym || !strcmp (crysym, "none") || !strcmp (crysym, "triclinic")
      || !strcmp (crysym, "-1"))
  {
    maxrho = fabs (qd[0]);
    nb = 1;
  }
  else if (!strcmp (crysym, "cubic") || !strcmp (crysym, "m-3m"))
  {
    crysym_maxrho = .92387953251128675613;      /* = cos (45/2) */

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
  else if (!strcmp (crysym, "hexagonal") || !strcmp (crysym, "6/mmm"))
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
  else if (qty > 0)             /* other Laue groups: generic (slower) loop */
  {
    double *qtmp = ol_q_alloc ();

    maxrho = 0;
    nb = 1;
    for (i = 1; i <= qty; i++)
    {
      ol_q_crysym (qd, crysym, i, qtmp);
      rho = fabs (qtmp[0]);
      if (rho > maxrho)
      {
        nb = i;
        maxrho = rho;
      }
    }

    ol_q_free (&qtmp);
  }
  else
  {
    ut_print_message (2, 0, "Unknown crystal symmetry `%s'.\n", crysym);
    abort ();
  }

  (*ptheta) = 2 * ut_num_acos (maxrho);

  ol_q_free (&qd);

  return nb;
}

/* rapid function on which other quaternion functions are based */
int
ol_q_q_disori_fr_rad (double *q1, double *q2, char *crysym, double *ptheta)
{
  int i, nb;
  double rho, maxrho, crysym_maxrho;
  double *qd;

  /* only the cubic symmetry is specialized */
  if (!crysym || (strcmp (crysym, "cubic") && strcmp (crysym, "m-3m")))
    return ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  qd = ol_q_alloc ();

  double Tcubic[15][4] = {
    {1, 0, 0, 0},               // 1
    {OL_IS2, OL_IS2, 0, 0},     // 2
    {OL_IS2, -OL_IS2, 0, 0},    // 4
    {OL_IS2, 0, OL_IS2, 0},     // 5
    {OL_IS2, 0, -OL_IS2, 0},    // 7
    {OL_IS2, 0, 0, OL_IS2},     // 8
    {OL_IS2, 0, 0, -OL_IS2},    // 10
    {.5, .5, .5, .5},           // 17
    {.5, -.5, -.5, -.5},        // 18
    {.5, -.5, .5, .5},          // 19
    {.5, .5, -.5, -.5},         // 20
    {.5, .5, -.5, .5},          // 21
    {.5, -.5, .5, -.5},         // 22
    {.5, .5, .5, -.5},          // 23
    {.5, -.5, -.5, .5}          // 24
  };

  int Tcubic_number[15] = { 1, 2, 4, 5, 7, 8, 10, 17, 18, 19, 20, 21, 22,
    23, 24
  };

  /* computation of q1^{-1}.q2 */
  ol_q_inverse (q1, qd);
  ol_q_q_q (qd, q2, qd);

  crysym_maxrho = .92387953251128675613;        /* = cos (45/2) */

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

  ol_q_free (&qd);

  return nb;
}

int
ol_q_q_disori (double *q1, double *q2, char *crysym, double *ptheta)
{
  int nb;

  nb = ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return nb;
}

int
ol_q_q_qdisori (double *q1, double *q2, char *crysym, double *qd)
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

  ol_q_free (&q1inv);
  ol_q_free (&q2rot);

  return nb;
}

int
ol_q_q_qdisori_update (double *q1, double *q2, char *crysym, int prevnb,
                       double *qd, char *flag, int *pstatus)
{
  int nb, status;
  double theta;
  double *q1inv = ol_q_alloc ();
  double *q2rot = ol_q_alloc ();
  double cubic_maxrho = .92387953251128675613;  /* = cos (45/2) */
  ol_q_set_zero (qd);

  nb = 0;

  status = 0;

  if (crysym && (!strcmp (crysym, "cubic") || !strcmp (crysym, "m-3m")) && prevnb >= 1
      && prevnb <= 24)
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

  else if (!crysym || !strcmp (crysym, "none") || !strcmp (crysym, "triclinic")
           || !strcmp (crysym, "-1"))
  {
    ol_q_inverse (q1, q1inv);
    ol_q_q_q_ref (q1inv, q2, qd);
    ol_q_q (qd, qd);
    nb = 1;
    status = 1;
  }

  if (!status)
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

  ol_q_free (&q1inv);
  ol_q_free (&q2rot);

  return nb;
}

int
ol_q_q_qdisori_ref (double *q1, double *q2, char *crysym, double *qd)
{
  return ol_q_q_qdisori (q1, q2, crysym, qd);
}

int
ol_q_q_qdisori_cur (double *q1, double *q2, char *crysym, double *qd)
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

  ol_q_free (&q1inv);
  ol_q_free (&q2rot);

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
ol_vect_vect_rtheta_rad (double *v1, double *v2, double *rtheta)
{
  double norm;

  ol_vect_vect_theta_rad (v1, v2, rtheta + 3);

  if (rtheta[3] > OL_EPS_RAD && rtheta[3] < M_PI - OL_EPS_RAD)
  {
    ut_vector_vectprod (v1, v2, rtheta);
    norm = ut_vector_norm (rtheta);
    ut_array_1d_scale (rtheta, 3, 1 / norm);
  }
  else
    ol_r_set_id (rtheta);

  return;
}

void
ol_vect_vect_rtheta (double *v1, double *v2, double *rtheta)
{
  ol_vect_vect_rtheta_rad (v1, v2, rtheta);
  ol_theta_rad2deg (rtheta[3], rtheta + 3);

  return;
}

void
ol_vect_vect_g (double *v1, double *v2, double **g)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_vect_vect_rtheta (v1, v2, rtheta);
  ol_rtheta_g (rtheta, g);

  ol_rtheta_free (&rtheta);

  return;
}

void
ol_vect_vect_q (double *v1, double *v2, double *q)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_vect_vect_rtheta (v1, v2, rtheta);
  ol_rtheta_q (rtheta, q);

  ol_rtheta_free (&rtheta);

  return;
}

void
ol_g_g_gmisori (double **g1, double **g2, double **gmis)
{
  double **g1inv = ol_g_alloc ();

  ol_g_inverse (g1, g1inv);
  ol_g_g_g_ref (g1inv, g2, gmis);

  ol_g_free (&g1inv);

  return;
}

void
ol_q_q_qmisori (double *q1, double *q2, double *qmis)
{
  double *q1inv = ol_q_alloc ();

  ol_q_inverse (q1, q1inv);
  ol_q_q_q_ref (q1inv, q2, qmis);
  ol_q_q (qmis, qmis);

  ol_q_free (&q1inv);

  return;
}

void
ol_g_g_gmisori_ref (double **g1, double **g2, double **gdis)
{
  ol_g_g_gmisori (g1, g2, gdis);

  return;
}

void
ol_q_q_qmisori_ref (double *q1, double *q2, double *qdis)
{
  ol_q_q_qmisori (q1, q2, qdis);

  return;
}

void
ol_g_g_gmisori_cur (double **g1, double **g2, double **gmis)
{
  double **g1inv = ol_g_alloc ();

  ol_g_inverse (g1, g1inv);
  ol_g_g_g (g1inv, g2, gmis);

  ol_g_free (&g1inv);

  return;
}

void
ol_q_q_qmisori_cur (double *q1, double *q2, double *qmis)
{
  double *q1inv = ol_q_alloc ();

  ol_q_inverse (q1, q1inv);
  ol_q_q_q (q1inv, q2, qmis);
  ol_q_q (qmis, qmis);

  ol_q_free (&q1inv);

  return;
}

void
ol_g_g_misori_rad (double **g1, double **g2, double *ptheta)
{
  /* may be improved */
  double **gd = ol_g_alloc ();

  ol_g_inverse (g1, gd);
  ol_g_g_g (gd, g2, gd);
  ol_g_theta_rad (gd, ptheta);

  ol_g_free (&gd);

  return;
}

void
ol_g_g_misori (double **g1, double **g2, double *ptheta)
{
  ol_g_g_misori_rad (g1, g2, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return;
}

void
ol_q_q_misori_rad (double *q1, double *q2, double *ptheta)
{
  /* may be improved */
  double *qd = ol_q_alloc ();

  ol_q_inverse (q1, qd);
  ol_q_q_q (qd, q2, qd);
  ol_q_q (qd, qd);
  (*ptheta) = 2 * ut_num_acos (qd[0]);

  ol_q_free (&qd);

  return;
}

void
ol_q_q_misori (double *q1, double *q2, double *ptheta)
{
  ol_q_q_misori_rad (q1, q2, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return;
}

/* for the sake of convenience and efficiency, quaternions are used. */
int
ol_g_g_disori_rad (double **g1, double **g2, char *crysym, double *ptheta)
{
  int nb;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_g_q (g1, q1);
  ol_g_q (g2, q2);

  nb = ol_q_q_disori_rad (q1, q2, crysym, ptheta);

  ol_q_free (&q1);
  ol_q_free (&q2);

  return nb;
}

int
ol_g_g_disori (double **g1, double **g2, char *crysym, double *ptheta)
{
  int nb;

  nb = ol_g_g_disori_rad (g1, g2, crysym, ptheta);

  (*ptheta) *= 57.29577951308232087721;

  return nb;
}

int
ol_g_g_disori_orthotropic_rad (double **g1, double **g2, char *crysym,
                               double *pdis)
{
  int i, id = 0;
  double dis;
  double ***gsym = ut_alloc_3d (4, 3, 3);

  ol_g_refsym_orthotropic_all (g1, gsym);

  (*pdis) = DBL_MAX;
  for (i = 0; i < 4; i++)
  {
    ol_g_g_disori_rad (gsym[i], g2, crysym, &dis);
    if (dis < (*pdis))
    {
      (*pdis) = dis;
      id = i;
    }
  }

  ut_free_3d (&gsym, 4, 3);

  return id + 1;
}

int
ol_g_g_misori_orthotropic_rad (double **g1, double **g2, double *pdis)
{
  int i, id = 0;
  double dis;
  double ***gsym = ut_alloc_3d (4, 3, 3);

  ol_g_refsym_orthotropic_all (g1, gsym);

  (*pdis) = DBL_MAX;
  for (i = 0; i < 4; i++)
  {
    ol_g_g_misori_rad (gsym[i], g2, &dis);
    if (dis < (*pdis))
    {
      (*pdis) = dis;
      id = i;
    }
  }

  ut_free_3d (&gsym, 4, 3);

  return id + 1;
}

int
ol_g_g_gdisori_orthotropic (double **g1, double **g2, char *crysym,
                            double **gdis)
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

  ut_free_3d (&gsym, 4, 3);
  ut_free_3d (&gd, 4, 3);
  ut_free_1d (&dis);

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

  ut_free_3d (&gsym, 4, 3);
  ut_free_3d (&gd, 4, 3);
  ut_free_1d (&dis);

  return id + 1;
}

int
ol_q_q_qdisori_orthotropic (double *q1, double *q2, char *crysym,
                            double *qdis)
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

  ut_free_2d (&qsym, 4);
  ut_free_2d (&qd, 4);
  ut_free_1d (&dis);

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

  ut_free_2d (&qsym, 4);
  ut_free_2d (&qd, 4);
  ut_free_1d (&dis);

  return id + 1;
}

int
ol_g_g_disori_orthotropic (double **g1, double **g2, char *crysym,
                           double *pdis)
{
  int id;

  id = ol_g_g_disori_orthotropic_rad (g1, g2, crysym, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return id;
}

int
ol_g_g_misori_orthotropic (double **g1, double **g2, double *pdis)
{
  int id;

  id = ol_g_g_misori_orthotropic_rad (g1, g2, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return id;
}

int
ol_q_q_disori_orthotropic_rad (double *q1, double *q2, char *crysym,
                               double *pdis)
{
  int i, id = 0;
  double dis;
  double **qsym = ut_alloc_2d (4, 4);

  ol_q_refsym_orthotropic_all (q1, qsym);

  (*pdis) = DBL_MAX;
  for (i = 0; i < 4; i++)
  {
    ol_q_q_disori_rad (qsym[i], q2, crysym, &dis);
    if (dis < (*pdis))
    {
      (*pdis) = dis;
      id = i;
    }
  }

  ut_free_2d (&qsym, 4);

  return id + 1;
}

int
ol_q_q_disori_orthotropic (double *q1, double *q2, char *crysym, double *pdis)
{
  int id;

  id = ol_q_q_disori_orthotropic_rad (q1, q2, crysym, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return id;
}

int
ol_q_q_misori_orthotropic_rad (double *q1, double *q2, double *pdis)
{
  int i, id = 0;
  double dis;
  double **qsym = ut_alloc_2d (4, 4);

  ol_q_refsym_orthotropic_all (q1, qsym);

  (*pdis) = DBL_MAX;
  for (i = 0; i < 4; i++)
  {
    ol_q_q_misori_rad (qsym[i], q2, &dis);
    if (dis < (*pdis))
    {
      (*pdis) = dis;
      id = i;
    }
  }

  ut_free_2d (&qsym, 4);

  return id + 1;
}

int
ol_q_q_misori_orthotropic (double *q1, double *q2, double *pdis)
{
  int id;

  id = ol_q_q_misori_orthotropic_rad (q1, q2, pdis);
  ol_theta_rad2deg (*pdis, pdis);

  return id;
}
