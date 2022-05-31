/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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

// TODO add support for endianness
int
neut_ori_fscanf (FILE *file, char *desconv, char *format, double **q, int *ids, int qty, char *prefix)
{
  int i, j, pos, status;
  double theta;
  double **g = ol_g_alloc ();
  double *tmp = ut_alloc_1d (9);
  char *des = NULL, *conv = NULL;
  int *ids2 = ut_alloc_1d_int (qty), *idinv = NULL;
  int idinput = (ids && ut_array_1d_int_sum (ids, qty) > 0);

  if (strcmp (format, "ascii") && prefix)
    abort ();

  if (ids)
    ut_array_1d_int_inv (ids, qty, &idinv, NULL);

  status = neut_ori_expr_desconv (desconv, &des, &conv);

  if (status)
    ut_print_exprbug (desconv);

  for (i = 0; i < qty; i++)
  {
    if (neut_ori_fscanf_id (file, prefix, i, ids2 + i) != 1)
      return -1;

    pos = idinput ? idinv[ids2[i]] : i;

    if (pos == -1)
      abort ();

    if (!strcmp (des, "quaternion") || !strcmp (des, "q"))
    {
      if (!strcmp (format, "ascii"))
      {
        if (ol_q_fscanf (file, q[pos]) != 1)
          return -1;
      }
      else
      {
        if (fread (q[pos], sizeof (double), 4, file) != 4)
          return -1;
      }
    }
    else if (!strcmp (des, "euler-bunge") || !strcmp (des, "e"))
    {
      if (ol_e_fscanf (file, tmp) != 1)
        return -1;

      ol_e_q (tmp, q[pos]);
    }
    else if (!strcmp (des, "euler-roe") || !strcmp (des, "er"))
    {
      if (!strcmp (format, "ascii"))
      {
        if (ol_e_fscanf (file, tmp) != 1)
          return -1;
      }
      else
      {
        if (fread (tmp, sizeof (double), 3, file) != 3)
          return -1;
      }

      ol_er_e (tmp, tmp);
      ol_e_q (tmp, q[pos]);
    }
    else if (!strcmp (des, "euler-kocks") || !strcmp (des, "ek"))
    {
      if (!strcmp (format, "ascii"))
      {
        if (ol_e_fscanf (file, tmp) != 1)
          return -1;
      }
      else
      {
        if (fread (tmp, sizeof (double), 3, file) != 3)
          return -1;
      }

      ol_ek_e (tmp, tmp);
      ol_e_q (tmp, q[pos]);
    }
    else if (!strcmp (des, "axis-angle") || !strcmp (des, "rtheta"))
    {
      if (!strcmp (format, "ascii"))
      {
        if (ol_rtheta_fscanf (file, tmp, &theta) != 1)
          return -1;
      }
      else
      {
        if (fread (tmp, sizeof (double), 3, file) != 3)
          return -1;
        if (fread (&theta, sizeof (double), 1, file) != 1)
          return -1;
      }

      ol_rtheta_q (tmp, theta, q[pos]);
    }
    else if (!strcmp (des, "rodrigues") || !strcmp (des, "R"))
    {
      if (!strcmp (format, "ascii"))
      {
        if (ol_R_fscanf (file, tmp) != 1)
          return -1;
      }
      else
      {
        if (fread (tmp, sizeof (double), 3, file) != 3)
          return -1;
      }

      ol_R_q (tmp, q[pos]);
    }
    else if (!strcmp (des, "rotmat") || !strcmp (des, "g"))
    {
      if (!strcmp (format, "ascii"))
      {
        if (ol_g_fscanf (file, g) != 1)
          return -1;
      }
      else
      {
        for (j = 0; j < 3; j++)
          if (fread (g[j], sizeof (double), 3, file) != 3)
            return -1;
      }

      ol_g_q (g, q[pos]);
    }
    else
      ut_print_exprbug (desconv);
  }


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
neut_ori_fnscanf (char *filename, char *desconv, char *format, double **q, int *ids, int qty, char *prefix, char *mode)
{
  int status;

  FILE *file = ut_file_open (filename, mode);

  status = neut_ori_fscanf (file, desconv, format, q, ids, qty, prefix);

  ut_file_close (file, filename, mode);

  return status;
}

void
neut_ori_fprintf (FILE *file, char *desconv, char *format, double **q0, int *ids, int *indexed, int qty, char *prefix)
{
  int i, status;
  double **q = ut_alloc_2d (qty, 4);
  char *des = NULL, *conv = NULL;

  if (strcmp (format, "ascii") && prefix)
    abort ();

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
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

        ol_q_e (q[i], e);
        if (!strcmp (format, "ascii"))
          ol_e_fprintf (file, e, "%17.12f");
        else
          fwrite (e, sizeof (double), 3, file);
      }
    ol_e_free (e);
  }
  else if (!strcmp (des, "euler-kocks"))
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

      ol_q_e (q[i], e);
      ol_e_ek (e, e);
      if (!strcmp (format, "ascii"))
        ol_e_fprintf (file, e, "%17.12f");
      else
        fwrite (e, sizeof (double), 3, file);
    }
    ol_e_free (e);
  }
  else if (!strcmp (des, "euler-roe"))
  {
    double *e = ol_e_alloc ();
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

        ol_q_e (q[i], e);
        ol_e_er (e, e);
        if (!strcmp (format, "ascii"))
          ol_e_fprintf (file, e, "%17.12f");
        else
          fwrite (e, sizeof (double), 3, file);
      }
    ol_e_free (e);
  }
  else if (!strcmp (des, "rotmat"))
  {
    double **g = ol_g_alloc ();
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

        ol_q_g (q[i], g);
        if (!strcmp (format, "ascii"))
        {
          ut_array_1d_fprintf_nonl (file, g[0], 3, "%15.12f");
          fprintf (file, " ");
          ut_array_1d_fprintf_nonl (file, g[1], 3, "%15.12f");
          fprintf (file, " ");
          ut_array_1d_fprintf      (file, g[2], 3, "%15.12f");
        }
        else
        {
          fwrite (g[0], sizeof (double), 3, file);
          fwrite (g[1], sizeof (double), 3, file);
          fwrite (g[2], sizeof (double), 3, file);
        }
    }
    ol_g_free (g);
  }
  else if (!strcmp (des, "axis-angle"))
  {
    double *r = ol_r_alloc ();
    double theta;
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

        ol_q_rtheta (q[i], r, &theta);
        if (!strcmp (format, "ascii"))
          ol_rtheta_fprintf (file, r, theta, "%17.12f");
        else
        {
          fwrite (r, sizeof (double), 3, file);
          fwrite (&theta, sizeof (double), 1, file);
        }
      }
    ol_r_free (r);
  }
  else if (strcmp (des, "angle") == 0)
  {
    double theta;
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

        ol_q_theta (q[i], &theta);
        if (!strcmp (format, "ascii"))
          ol_theta_fprintf (file, theta, "%17.12f");
        else
          fwrite (&theta, sizeof (double), 1, file);
      }
  }
  else if (!strcmp (des, "rodrigues"))
  {
    double *R = ol_R_alloc ();
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

        ol_q_R (q[i], R);
        if (!strcmp (format, "ascii"))
          ol_R_fprintf (file, R, "%17.12f");
        else
          fwrite (R, sizeof (double), 3, file);
      }
    ol_R_free (R);
  }
  else if (!strcmp (des, "quaternion"))
  {
    for (i = 0; i < qty; i++)
      if (!indexed ||  indexed[i])
      {
        if (prefix && !strcmp (prefix, "id"))
          fprintf (file, "%d ", ids ? ids[i] : i + 1);

      if (!strcmp (format, "ascii"))
        ol_q_fprintf (file, q[i], "%17.12f");
      else
        fwrite (q[i], sizeof (double), 4, file);
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

void
neut_ori_orispread (double *qmean, char *orispread, int qty, int id, double **q)
{
  int i;

  struct OL_SET OSet = ol_set_alloc (qty, NULL);

  ol_set_misorispread (orispread, 3, id, &OSet);

  for (i = 0; i < qty; i++)
    ol_q_q_q_ref (qmean, OSet.q[i], q[i]);

  ol_set_free (OSet);

  return;
}

void
neut_ori_fiber_sscanf (char *fiber, double *dirc, double *dirs, double *ptheta, char **pspread)
{
  int i, partqty;
  double theta = 0;
  char **parts = NULL;
  char tmp;

  ut_list_break (fiber, NEUT_SEP_DEP, &parts, &partqty);

  if (partqty == 2 && pspread)
    ut_string_string (parts[1], pspread);

  if (sscanf
      (parts[0], "fiber(%lf,%lf,%lf,%lf,%lf,%lf,%lf)", dirc, dirc + 1, dirc + 2,
       dirs, dirs + 1, dirs + 2, &theta) == 7)
  {
    ut_vector_uvect (dirs, dirs);
    ut_vector_uvect (dirc, dirc);
    if (ptheta)
      *ptheta = theta;
  }

  else if (sscanf
      (parts[0], "fiber(%lf,%lf,%lf,%lf,%lf,%lf)", dirc, dirc + 1, dirc + 2,
       dirs, dirs + 1, dirs + 2) == 6)
  {
    ut_vector_uvect (dirs, dirs);
    ut_vector_uvect (dirc, dirc);
  }

  // legacy
  else
    if (sscanf
        (parts[0], "fiber(%c,%lf,%lf,%lf)", &tmp, dirc, dirc + 1, dirc + 2) == 4)
  {
    if (tmp == 'x' || tmp == 'y' || tmp == 'z')
      for (i = 0; i < 3; i++)
        dirs[i] = tmp == 'x' + i;
    else
      abort ();

    ut_vector_uvect (dirc, dirc);
  }

  else
    abort ();

  ut_free_2d_char (&parts, partqty);

  return;
}

void
neut_ori_fiber (double *dirc, double *dirs, long random, int qty, double **q)
{
  int i;
  double theta;
  double *q_align = ol_q_alloc ();
  double *q_rand = ol_q_alloc ();

  gsl_rng *rnd = NULL;
  rnd = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rnd, random - 1);

  // 1st rotation, to get in the fiber
  ol_vect_vect_q (dirc, dirs, q_align);

  // 2nd rotation, about the fiber
  for (i = 0; i < qty; i++)
  {
    theta = 2.0 * M_PI * gsl_rng_uniform (rnd);
    ol_rtheta_q_rad (dirs, theta, q_rand);
    ol_q_q_q_ref (q_align, q_rand, q[i]);
  }

  ol_q_free (q_align);
  ol_q_free (q_rand);
  gsl_rng_free (rnd);

  return;
}

void
neut_ori_fiber_spread (char *spread, double *dirs, long random, int qty, char *crysym, double **q)
{
  int i;
  double *z = ol_vect_alloc ();
  double *q_align = ol_q_alloc ();
  struct OL_SET OSet2 = ol_set_alloc (qty, crysym);

  ol_set_misorispread (spread, 2, random, &OSet2);

  ut_array_1d_set_3 (z, 0, 0, 1);
  ol_vect_vect_q (dirs, z, q_align);
  for (i = 0; i < qty; i++)
  {
    ol_q_csys (OSet2.q[i], q_align, OSet2.q[i]);
    if (!ut_num_equal (ut_vector_scalprod (OSet2.q[i] + 1, dirs), 0, 1e-6))
      abort ();
    ol_q_q_q_ref (q[i], OSet2.q[i], q[i]);
  }

  ut_free_1d (&z);
  ol_set_free (OSet2);
  ol_q_free (q_align);

  return;
}

void
neut_ori_fiber_rodfiber (double *dirc, double *dirs, double *P, double *v)
{
  int i;
  double **q = ut_alloc_2d (2, 4);
  double **R = ut_alloc_2d (2, 3);

  neut_ori_fiber (dirc, dirs, 1, 2, q);
  for (i = 0; i < 2; i++)
    ol_q_R (q[i], R[i]);

  ut_array_1d_memcpy (R[0], 3, P);
  ut_space_points_uvect (R[0], R[1], v);

  ut_free_2d (&q, 2);
  ut_free_2d (&R, 2);

  return;
}

int
neut_ori_fiber_in (double *q, char *crysym, double *dirc, double *dirs, double theta)
{
  int status = 0;
  int i, qty = ol_crysym_qty (crysym);
  double *qc = ol_q_alloc ();
  double tmp;
  double *dirc2 = ol_vect_alloc ();

  for (i = 1; i <= qty; i++)
  {
    ol_q_crysym (q, crysym, i, qc);

    ol_q_inverse (qc, qc);
    ol_q_vect_vect (qc, dirc, dirc2);
    ol_vect_vect_theta (dirc2, dirs, &tmp);

    tmp = ut_num_min (tmp, 180 - tmp);

    if (tmp < theta)
    {
      status = 1;
      break;
    }
  }

  ol_q_free (qc);
  ol_vect_free (dirc2);

  return status;
}

int
neut_ori_des_isvalid (char *des)
{
  int valqty, status;
  char **vals = NULL;

  ut_list_break (des, NEUT_SEP_DEP, &vals, &valqty);

  status = 1;

  if (valqty < 1 || valqty > 2)
    status = 0;

  else if (strcmp (vals[0], "euler-bunge")
   && strcmp (vals[0], "euler-kocks")
   && strcmp (vals[0], "euler-roe")
   && strcmp (vals[0], "rotmat")
   && strcmp (vals[0], "axis-angle")
   && strcmp (vals[0], "rodrigues")
   && strcmp (vals[0], "quaternion"))
   status = 0;

  else if (valqty == 2 && strcmp (vals[1], "active") && strcmp (vals[1], "passive"))
   status = 0;

  ut_free_2d_char (&vals, valqty);

  return status;
}

int
neut_ori_des_ori (double *q0, char *des0, double *ori)
{
  int status;
  double *q = ol_q_alloc ();
  char *des = NULL, *conv = NULL;

  neut_ori_expr_desconv (des0, &des, &conv);

  ol_q_memcpy (q0, q);

  status = 0;

  if (!strcmp (conv, "passive"))
    ol_q_inverse (q, q);

  if (!strcmp (des, "euler-bunge"))
    ol_q_e (q, ori);

  else if (!strcmp (des, "euler-kocks"))
  {
    ol_q_e (q, ori);
    ol_e_ek (ori, ori);
  }
  else if (!strcmp (des, "euler-roe"))
  {
    ol_q_e (q, ori);
    ol_e_er (ori, ori);
  }
  else if (!strcmp (des, "rotmat"))
  {
    double **g = ol_g_alloc ();
    ol_q_g (q, g);
    ut_array_1d_memcpy (g[0], 3, ori);
    ut_array_1d_memcpy (g[1], 3, ori + 3);
    ut_array_1d_memcpy (g[2], 3, ori + 6);
    ol_g_free (g);
  }
  else if (!strcmp (des, "axis-angle"))
    ol_q_rtheta (q, ori, ori + 3);

  else if (!strcmp (des, "rodrigues"))
    ol_q_R (q, ori);

  else if (!strcmp (des, "quaternion"))
    ol_q_q (q, ori);

  else
    status = -1;

  ol_q_free (q);
  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);

  return status;
}
