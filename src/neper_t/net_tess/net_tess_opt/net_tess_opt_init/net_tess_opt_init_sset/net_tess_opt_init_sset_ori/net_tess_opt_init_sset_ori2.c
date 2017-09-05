/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori_3d (struct SEEDSET *pSSet)
{
  int i;
  double *e = ol_e_alloc ();
  gsl_rng *r = NULL;
  double n1, n2, n3;

  (*pSSet).q = ut_alloc_2d ((*pSSet).N + 1, 4);

  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, (*pSSet).Random - 1);

  for (i = 1; i <= (*pSSet).N; i++)
  {
    // keep the n1 n2 n3 stuff for backward compatibility.
    // Calling the gsl as an argument switches n1 and n3.
    n1 = gsl_rng_uniform (r);
    n2 = gsl_rng_uniform (r);
    n3 = gsl_rng_uniform (r);
    ol_nb_e (n1, n2, n3, e);
    ol_e_q (e, (*pSSet).q[i]);
  }

  gsl_rng_free (r);
  ol_e_free (e);

  return;
}

// Author: L. Renversade.
void
net_tess_opt_init_sset_ori_fibre (char *distrib, struct SEEDSET *pSSet)
{
  int i;
  double eps = 1e-9;
  char axis_c;
  double axis[3];
  double dir[3];

  double *r = ol_r_alloc ();
  double theta;

  double *q_align = ol_q_alloc ();
  double *q_rand = ol_q_alloc ();

  gsl_rng *rnd = NULL;

  // FIBRE
  if (sscanf (distrib, "fibre(%c,%lf,%lf,%lf)", &axis_c, dir, dir + 1,
	      dir + 2) != 4 || (axis_c != 'x' && axis_c != 'y'
				&& axis_c != 'z'))
    ut_print_message (2, 2, "Option -ori: failed to read fibre.\n");

  axis[0] = (axis_c == 'x') ? 1 : 0;
  axis[1] = (axis_c == 'y') ? 1 : 0;
  axis[2] = (axis_c == 'z') ? 1 : 0;

  ut_vector_uvect (dir, dir);

  (*pSSet).q = ut_alloc_2d ((*pSSet).N + 1, 4);

  rnd = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rnd, (*pSSet).Random - 1);

  /*------ 1st rotation: aligning DIR along AXIS -------------*/
  theta = ut_vector_angle_rad (dir, axis);	// the angle to align DIR with AXIS (DIR -> AXIS): > 0

  if (ut_num_equal (theta, 0, eps) == 1
      || ut_num_equal (theta, M_PI, eps) == 1)
    ol_q_set_id (q_align);	// identity if DIR == AXIS
  else
  {
    ut_vector_vectprod (dir, axis, r);	// the axis to align DIR with AXIS (DIR -> AXIS) : unit vector
    ol_rtheta_q_rad (r, theta, q_align);
  }

  /*------- 2nd rotation: random rotation around AXIS --------*/
  for (i = 1; i <= (*pSSet).N; i++)
  {
    theta = 2.0 * M_PI * gsl_rng_uniform (rnd);
    ol_rtheta_q_rad (axis, theta, q_rand);
    ol_q_q_q_ref (q_align, q_rand, (*pSSet).q[i]);	// rotations in the reference csys
  }

  gsl_rng_free (rnd);
  ol_q_free (q_align);
  ol_q_free (q_rand);
  ol_r_free (r);

  return;
}

void
net_tess_opt_init_sset_ori_crysym (struct IN_T In,
                                   struct MTESS MTess, struct TESS *Tess,
				   int dtess, int dcell,
				   struct SEEDSET *pSSet)
{
  int i;

  net_multiscale_mtess_arg_0d_char_fscanf (MTess, Tess, dtess, dcell,
                                           In.oricrysym,
					   &(*pSSet).crysym);

  for (i = 1; i <= (*pSSet).N; i++)
    ol_q_qcrysym ((*pSSet).q[i], (*pSSet).crysym, (*pSSet).q[i]);

  return;
}

void
net_tess_opt_init_sset_ori_equal (struct SEEDSET *SSet, int dtess,
				  int dcell, struct SEEDSET *pSSet)
{
  int i;

  (*pSSet).q = ut_alloc_2d ((*pSSet).N + 1, 4);

  for (i = 1; i <= (*pSSet).N; i++)
    ut_array_1d_memcpy ((*pSSet).q[i], 4, SSet[dtess].q[dcell]);

  return;
}

int
net_tess_opt_init_sset_ori_label (char *label, struct SEEDSET *pSSet)
{
  int status, i;
  double **g = ol_g_alloc ();

  (*pSSet).q = ut_alloc_2d ((*pSSet).N + 1, 4);
  status = ol_label_g (label, g);

  if (!status)
  {
    ol_g_q (g, (*pSSet).q[1]);
    for (i = 1; i <= (*pSSet).N; i++)
      ut_array_1d_memcpy ((*pSSet).q[i], 4, (*pSSet).q[1]);
  }

  ol_g_free (g);

  return status;
}

void
net_tess_opt_init_sset_ori_file (char *filename, struct SEEDSET *pSSet)
{
  int i, partqty;
  char **parts = NULL;
  char *des = NULL;
  double *vect = ut_alloc_1d (4);
  double **g = ol_g_alloc ();
  FILE *fp = NULL;

  ut_string_separate (filename, NEUT_SEP_DEP, &parts, &partqty);
  if (partqty == 1)
    ut_string_string ("e", &des);
  else
    ut_string_string (parts[1], &des);

  printf ("\n");
  fp = ut_file_open (parts[0], "r");

  (*pSSet).q = ut_alloc_2d ((*pSSet).N + 1, 4);

  for (i = 1; i <= (*pSSet).N; i++)
  {
    if (!strcmp (des, "e"))
    {
      ol_e_fscanf (fp, vect);
      ol_e_q (vect, (*pSSet).q[i]);
    }
    else if (!strcmp (des, "ek"))
    {
      ol_e_fscanf (fp, vect);
      ol_ek_e (vect, vect);
      ol_e_q (vect, (*pSSet).q[i]);
    }
    else if (!strcmp (des, "er"))
    {
      ol_e_fscanf (fp, vect);
      ol_er_e (vect, vect);
      ol_e_q (vect, (*pSSet).q[i]);
    }
    else if (!strcmp (des, "rtheta"))
    {
      ol_rtheta_fscanf (fp, vect, vect + 3);
      ol_rtheta_q (vect, vect[3], (*pSSet).q[i]);
    }
    else if (!strcmp (des, "q"))
    {
      ol_q_fscanf (fp, vect);
      ol_q_q (vect, (*pSSet).q[i]);
    }
    else if (!strcmp (des, "R"))
    {
      ol_R_fscanf (fp, vect);
      ol_R_q (vect, (*pSSet).q[i]);
    }
    else if (!strcmp (des, "g"))
    {
      ol_g_fscanf (fp, g);
      ol_g_q (g, (*pSSet).q[i]);
    }
    else
      abort ();
  }

  ut_file_close (fp, filename, "r");
  ut_print_message (0, 0, "");

  ut_free_2d_char (parts, partqty);
  ut_free_1d (vect);
  ol_g_free (g);

  return;
}
