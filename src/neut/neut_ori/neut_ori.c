/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_ori_.h"

void
neut_ori_volthetafct (struct FCT *pvolfct)
{
  int i;
  double *ptmp = NULL;

  ut_fct_set_numerical (pvolfct, 0, M_PI, 100000, "bounds");

  for (i = 0; i < 100000; i++)
    (*pvolfct).y[i] = M_PI * ((*pvolfct).x[i] - sin ((*pvolfct).x[i]));
  ptmp = (*pvolfct).x;
  (*pvolfct).x = (*pvolfct).y;
  (*pvolfct).y = ptmp;

  ut_fct_init_interp (pvolfct);

  return;
}

void
neut_ori_vol_theta (struct FCT *pvolfct, double vol, double *ptheta)
{
  (*ptheta) = ut_fct_eval (*pvolfct, vol);

  return;
}

void
neut_ori_n_avthetaeq (struct FCT *pvolfct, int N, char *crysym,
                      double *ptheta)
{
  double vol = pow (M_PI, 2) / (N * ol_crysym_qty (crysym));

  if (pvolfct)
    (*ptheta) = ut_fct_eval (*pvolfct, vol);
  else
  {
    struct FCT thetafct;
    ut_fct_set_zero (&thetafct);
    neut_ori_volthetafct (&thetafct);
    (*ptheta) = ut_fct_eval (thetafct, vol);
    ut_fct_free (&thetafct);
  }

  return;
}

void
neut_ori_n_avradeq (struct FCT *pvolfct, int N, char *crysym, double *pradeq)
{
  neut_ori_n_avthetaeq (pvolfct, N, crysym, pradeq);
  (*pradeq) /= 2;

  return;
}

void
neut_ori_n_avdiameq (struct FCT *pvolfct, int N, char *crysym,
                     double *pdiameq)
{
  neut_ori_n_avthetaeq (pvolfct, N, crysym, pdiameq);

  return;
}

void
neut_ori_f_qdotnorm (double *f, double *pqdotnorm)
{
  (*pqdotnorm) = ut_vector_norm (f);

  return;
}

void
neut_ori_f_qdot (double *f, double *qdot)
{
  double qdotnorm;

  neut_ori_f_qdotnorm (f, &qdotnorm);
  qdot[0] = 0;
  neut_ori_f_r (f, qdot + 1);
  ut_array_1d_scale (qdot + 1, 3, qdotnorm);

  return;
}

void
neut_ori_f_r (double *f, double *r)
{
  if (ut_vector_norm (f) != 0)
  {
    ut_array_1d_memcpy (f, 3, r);
    ut_array_1d_scale (r, 3, 1. / ut_vector_norm (r));
  }
  else
    ut_array_1d_set_3 (r, 1, 0, 0);

  return;
}

void
neut_ori_f_norm (double **f, int N, double *pnorm)
{
  int i;

  (*pnorm) = 0;
#pragma omp parallel for
  for (i = 0; i < N; i++)
#pragma omp atomic
    (*pnorm) += ut_vector_norm_squared (f[i]);
  (*pnorm) = sqrt (*pnorm);

  return;
}

int
neut_ori_expr_desconv (char *expr, char **pdes, char **pconv)
{
  int status = 0, qty;
  char **tmp = NULL;

  ut_free_1d_char (pdes);
  ut_free_1d_char (pconv);

  ut_list_break (expr, NEUT_SEP_DEP, &tmp, &qty);

  if (qty < 1 || qty > 2)
    status = -1;
  else if (qty == 1)
  {
    ut_string_string (tmp[0], pdes);
    ut_string_string ("active", pconv);
  }
  else
  {
    ut_string_string (tmp[0], pdes);
    ut_string_string (tmp[1], pconv);
  }

  if (strcmp (*pconv, "active") && strcmp (*pconv, "passive"))
    status = -1;

  ut_free_2d_char (&tmp, qty);

  return status;
}

int
neut_ori_expr_desconv_fepx (char *expr, char **pdes, char **pconv)
{
  int status;

  status = neut_ori_expr_desconv (expr, pdes, pconv);

  if (!strcmp (*pdes, "e"))
    ut_string_string ("euler-bunge", pdes);
  else if (!strcmp (*pdes, "ek"))
    ut_string_string ("euler-kocks", pdes);
  else if (!strcmp (*pdes, "er"))
    ut_string_string ("euler-roe", pdes);
  else if (!strcmp (*pdes, "g"))
    ut_string_string ("rotation-matrix", pdes);
  else if (!strcmp (*pdes, "rtheta"))
    ut_string_string ("angle-axis", pdes);
  else if (!strcmp (*pdes, "R"))
    ut_string_string ("rodrigues", pdes);
  else if (!strcmp (*pdes, "q"))
    ut_string_string ("quaternion", pdes);
  else
    abort ();

  return status;
}

void
neut_ori_fscanf (FILE *file, char *desconv, double **q, int qty)
{
  int i, status;
  double theta;
  double *tmp = ut_alloc_1d (9);
  char *des = NULL, *conv = NULL;

  ut_file_skip (file, 1);

  status = neut_ori_expr_desconv (desconv, &des, &conv);
  if (status)
    ut_print_exprbug (desconv);

  if (!strcmp (des, "q"))
    ut_array_2d_fscanf (file, q, qty, 4);
  else if (!strcmp (des, "e"))
    for (i = 0; i < qty; i++)
    {
      ol_e_fscanf (file, tmp);
      ol_e_q (tmp, q[i]);
    }
  else if (!strcmp (des, "er"))
    for (i = 0; i < qty; i++)
    {
      ol_e_fscanf (file, tmp);
      ol_er_e (tmp, tmp);
      ol_e_q (tmp, q[i]);
    }
  else if (!strcmp (des, "ek"))
    for (i = 0; i < qty; i++)
    {
      ol_e_fscanf (file, tmp);
      ol_ek_e (tmp, tmp);
      ol_e_q (tmp, q[i]);
    }
  else if (!strcmp (des, "rtheta"))
    for (i = 0; i < qty; i++)
    {
      ol_r_fscanf (file, tmp);
      if (fscanf (file, "%lf", &theta) != 0)
        abort ();
      ol_rtheta_q (tmp, theta, q[i]);
    }
  else if (!strcmp (des, "R"))
    for (i = 0; i < qty; i++)
    {
      ol_R_fscanf (file, tmp);
      ol_R_q (tmp, q[i]);
    }
  else
    ut_print_exprbug (desconv);

  if (!strcmp (conv, "passive"))
    for (i = 0; i < qty; i++)
      ol_q_inverse (q[i], q[i]);

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);
  ut_free_1d (&tmp);

  return;
}

void
neut_ori_fprintf (FILE *file, char *desconv, double **q0, int qty)
{
  int i, status;
  double **q = ut_alloc_2d (qty, 4);
  char *des = NULL, *conv = NULL;

  status = neut_ori_expr_desconv (desconv, &des, &conv);
  if (status)
    ut_print_exprbug (desconv);

  // copying CellOri to q, and taking the inverse if passive convention
  ut_array_2d_memcpy (q0, qty, 4, q);
  if (!strcmp (conv, "passive"))
    for (i = 0; i < qty; i++)
      ol_q_inverse (q[i], q[i]);

  if (strcmp (des, "e") == 0)
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
    {
      ol_q_e (q[i], e);
      ol_e_fprintf (file, e, "%17.12f");
    }
    ol_e_free (e);
  }
  else if (strcmp (des, "ek") == 0)
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
    {
      ol_q_e (q[i], e);
      ol_e_ek (e, e);
      ol_e_fprintf (file, e, "%17.12f");
    }
    ol_e_free (e);
  }
  else if (strcmp (des, "er") == 0)
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
    {
      ol_q_e (q[i], e);
      ol_e_er (e, e);
      ol_e_fprintf (file, e, "%17.12f");
    }
    ol_e_free (e);
  }
  else if (strcmp (des, "g") == 0)
  {
    double **g = ol_g_alloc ();
    for (i = 0; i < qty; i++)
    {
      ol_q_g (q[i], g);
      ol_g_fprintf (file, g, "%17.12f");
    }
    ol_g_free (g);
  }
  else if (strcmp (des, "rtheta") == 0)
  {
    double *r = ol_r_alloc ();
    double theta;
    for (i = 0; i < qty; i++)
    {
      ol_q_rtheta (q[i], r, &theta);
      ol_rtheta_fprintf (file, r, theta, "%17.12f");
    }
    ol_r_free (r);
  }
  else if (strcmp (des, "theta") == 0)
  {
    double theta;
    for (i = 0; i < qty; i++)
    {
      ol_q_theta (q[i], &theta);
      ol_theta_fprintf (file, theta, "%17.12f");
    }
  }
  else if (strcmp (des, "R") == 0)
  {
    double *R = ol_R_alloc ();
    for (i = 0; i < qty; i++)
    {
      ol_q_R (q[i], R);
      ol_R_fprintf (file, R, "%17.12f");
    }
    ol_R_free (R);
  }
  else if (strcmp (des, "q") == 0)
  {
    for (i = 0; i < qty; i++)
      ol_q_fprintf (file, q[i], "%17.12f");
  }
  else
    ut_print_message (2, 1, "Format %s not available.\n", des);

  ut_free_2d (&q, qty);
  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);

  return;
}
