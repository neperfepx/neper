/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_ori_.h"

void
neut_ori_des_des (char *des, char **pdes)
{
  if (!strcmp (des, "e"))
    ut_string_string ("euler-bunge", pdes);
  else if (!strcmp (des, "ek"))
    ut_string_string ("euler-kocks", pdes);
  else if (!strcmp (des, "er"))
    ut_string_string ("euler-roe", pdes);
  else if (!strcmp (des, "R"))
    ut_string_string ("rodrigues", pdes);
  else if (!strcmp (des, "rtheta"))
    ut_string_string ("axis-angle", pdes);
  else if (!strcmp (des, "q"))
    ut_string_string ("quaternion", pdes);
  else if (!strcmp (des, "m"))
    ut_string_string ("miller", pdes);
  else
    ut_string_string (des, pdes);

  return;
}

void
neut_ori_des_expand (char *des, char **pdes2)
{
  int qty;
  char **parts = NULL;
  char *part1 = NULL, *part2 = NULL;

  ut_list_break (des, NEUT_SEP_DEP, &parts, &qty);

  if (qty == 0)
    ut_print_message (2, 2, "Failed to process `%s'.\n", des);

  // descriptor

  if (!strcmp (parts[0], "e"))
    ut_string_string ("euler-bunge", &part1);
  else if (!strcmp (parts[0], "ek"))
    ut_string_string ("euler-kocks", &part1);
  else if (!strcmp (parts[0], "er"))
    ut_string_string ("euler-roe", &part1);
  else if (!strcmp (parts[0], "R"))
    ut_string_string ("rodrigues", &part1);
  else if (!strcmp (parts[0], "rtheta"))
    ut_string_string ("axis-angle", &part1);
  else if (!strcmp (parts[0], "q"))
    ut_string_string ("quaternion", &part1);
  else if (!strcmp (parts[0], "m"))
    ut_string_string ("miller", &part1);
  else
    ut_string_string (parts[0], &part1);

  // convention

  if (qty == 2)
  {
    if (strcmp (parts[1], "active") && strcmp (parts[1], "passive"))
      ut_print_message (2, 2, "Failed to process `%s'.\n", des);

    ut_string_string (parts[1], &part2);
  }
  else
    ut_string_string ("active", &part2);

  *pdes2 = ut_alloc_1d_char (strlen (part1) + strlen (part2) + 2);
  sprintf (*pdes2, "%s%s%s", part1, NEUT_SEP_DEP, part2);

  ut_free_2d_char (&parts, qty);
  ut_free_1d_char (&part1);
  ut_free_1d_char (&part2);

  return;
}

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

  neut_ori_des_des (*pdes, pdes);

  ut_free_2d_char (&tmp, qty);

  return status;
}

void
neut_ori_desconv_expr (char *des, char *conv, char **pexpr)
{
  char *tmp = NULL;

  ut_free_1d_char (pexpr);

  if (!conv || strlen (conv) == 0 || !strcmp (conv, "active"))
    ut_string_string (des, pexpr);
  else
  {
    *pexpr = ut_alloc_1d_char (strlen (des) + strlen (conv) + 2);
    sprintf (*pexpr, "%s%s%s", des, NEUT_SEP_DEP, conv);
  }

  ut_free_1d_char (&tmp);

  return;
}

void
neut_ori_desconv_expr_fepx (char *des, char *conv, char **pexpr)
{
  char *tmp0 = ut_alloc_1d_char (10);

  if (!strcmp (des, "euler"))
    sprintf (tmp0, "e");
  else if (!strcmp (des, "euler-bunge"))
    sprintf (tmp0, "e");
  else if (!strcmp (des, "euler-kocks"))
    sprintf (tmp0, "ek");
  else if (!strcmp (des, "euler-roe"))
    sprintf (tmp0, "er");
  else if (!strcmp (des, "rotation-matrix"))
    sprintf (tmp0, "g");
  else if (!strcmp (des, "angle-axis"))
    sprintf (tmp0, "rtheta");
  else if (!strcmp (des, "rodrigues"))
    sprintf (tmp0, "R");
  else if (!strcmp (des, "quaternion"))
    sprintf (tmp0, "q");
  else
    abort ();

  if (!strcmp (conv, "passive"))
  {
    (*pexpr) = ut_realloc_1d_char (*pexpr, strlen (tmp0) + 9);
    sprintf (*pexpr, "%s%s%s", tmp0, NEUT_SEP_DEP, conv);
  }
  else
    ut_string_string (tmp0, pexpr);

  return;
}

int
neut_ori_fscanf (FILE *file, char *desconv, double **q, int qty, char *prefix)
{
  int i, id, status;
  double theta;
  double **g = ol_g_alloc ();
  double *tmp = ut_alloc_1d (9);
  char *des = NULL, *conv = NULL;

  status = neut_ori_expr_desconv (desconv, &des, &conv);

  if (status)
    ut_print_exprbug (desconv);

  if (!strcmp (des, "quaternion") || !strcmp (des, "q"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_q_fscanf (file, q[id - 1]) != 1)
        return -1;
    }
  else if (!strcmp (des, "euler-bunge") || !strcmp (des, "e"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_e_fscanf (file, tmp) != 1)
        return -1;

      ol_e_q (tmp, q[id - 1]);
    }
  else if (!strcmp (des, "euler-roe") || !strcmp (des, "er"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_e_fscanf (file, tmp) != 1)
        return -1;

      ol_er_e (tmp, tmp);
      ol_e_q (tmp, q[id - 1]);
    }
  else if (!strcmp (des, "euler-kocks") || !strcmp (des, "ek"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_e_fscanf (file, tmp) != 1)
        return -1;

      ol_ek_e (tmp, tmp);
      ol_e_q (tmp, q[id - 1]);
    }
  else if (!strcmp (des, "axis-angle") || !strcmp (des, "rtheta"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_rtheta_fscanf (file, tmp, &theta) != 1)
        return -1;

      ol_rtheta_q (tmp, theta, q[id - 1]);
    }
  else if (!strcmp (des, "rodrigues") || !strcmp (des, "R"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_R_fscanf (file, tmp) != 1)
        return -1;

      ol_R_q (tmp, q[id - 1]);
    }
  else if (!strcmp (des, "rotmat") || !strcmp (des, "g"))
    for (i = 0; i < qty; i++)
    {
      if (neut_ori_fscanf_id (file, prefix, i, &id) != 1)
        return -1;

      if (ol_g_fscanf (file, g) != 1)
        return -1;

      ol_g_q (g, q[id - 1]);
    }
  else
    ut_print_exprbug (desconv);

  if (!strcmp (conv, "passive"))
    for (i = 0; i < qty; i++)
      ol_q_inverse (q[i], q[i]);

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);
  ut_free_1d (&tmp);
  ol_g_free (g);

  return 1;
}

int
neut_ori_fnscanf (char *filename, char *desconv, double **q, int qty, char *prefix)
{
  int status;

  FILE *file = ut_file_open (filename, "r");

  status = neut_ori_fscanf (file, desconv, q, qty, prefix);

  ut_file_close (file, filename, "r");

  return status;
}

void
neut_ori_fprintf (FILE *file, char *desconv, double **q0, int qty, char *prefix)
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

  if (!strcmp (des, "euler-bunge"))
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_e (q[i], e);
      ol_e_fprintf (file, e, "%17.12f");
    }
    ol_e_free (e);
  }
  else if (!strcmp (des, "euler-kocks"))
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_e (q[i], e);
      ol_e_ek (e, e);
      ol_e_fprintf (file, e, "%17.12f");
    }
    ol_e_free (e);
  }
  else if (!strcmp (des, "euler-roe"))
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_e (q[i], e);
      ol_e_er (e, e);
      ol_e_fprintf (file, e, "%17.12f");
    }
    ol_e_free (e);
  }
  else if (!strcmp (des, "rotmat"))
  {
    double **g = ol_g_alloc ();
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_g (q[i], g);
      ut_array_1d_fprintf_nonl (file, g[0], 3, "%15.12f");
      fprintf (file, " ");
      ut_array_1d_fprintf_nonl (file, g[1], 3, "%15.12f");
      fprintf (file, " ");
      ut_array_1d_fprintf      (file, g[2], 3, "%15.12f");
    }
    ol_g_free (g);
  }
  else if (!strcmp (des, "axis-angle"))
  {
    double *r = ol_r_alloc ();
    double theta;
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_rtheta (q[i], r, &theta);
      ol_rtheta_fprintf (file, r, theta, "%17.12f");
    }
    ol_r_free (r);
  }
  else if (strcmp (des, "angle") == 0)
  {
    double theta;
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_theta (q[i], &theta);
      ol_theta_fprintf (file, theta, "%17.12f");
    }
  }
  else if (!strcmp (des, "rodrigues"))
  {
    double *R = ol_R_alloc ();
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_R (q[i], R);
      ol_R_fprintf (file, R, "%17.12f");
    }
    ol_R_free (R);
  }
  else if (!strcmp (des, "quaternion"))
  {
    for (i = 0; i < qty; i++)
    {
      if (prefix && !strcmp (prefix, "id"))
        fprintf (file, "%d ", i + 1);

      ol_q_fprintf (file, q[i], "%17.12f");
    }
  }
  else
    ut_print_message (2, 1, "Format %s not available.\n", des);

  ut_free_2d (&q, qty);
  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);

  return;
}

void
neut_ori_rodriguescol_R (double *R, char *crysym, double *col)
{
  int i;

  if (!crysym || !strcmp (crysym, "cubic"))
  {
    if (crysym)
      ol_R_Rcrysym (R, crysym, R);

    for (i = 0; i < 3; i++)
      col[i] =
        ut_num_bound (ut_num_d2ri (127.5 * (R[i] + OL_S2 - 1) / (OL_S2 - 1)), 0,
                      255);
  }

  return;
}

void
neut_ori_rodriguescol_int (double *q, char *crysym, int *col)
{
  int i;
  double *tmp = ut_alloc_1d (3);

  neut_ori_rodriguescol (q, crysym, tmp);

  for (i = 0; i < 3; i++)
    col[i] = ut_num_d2ri (tmp[i]);

  ut_free_1d (&tmp);

  return;
}

void
neut_ori_rodriguescol (double *q, char *crysym, double *col)
{
  double *R = ol_R_alloc ();

  ol_q_R (q, R);

  neut_ori_rodriguescol_R (R, crysym, col);

  ol_R_free (R);

  return;
}
