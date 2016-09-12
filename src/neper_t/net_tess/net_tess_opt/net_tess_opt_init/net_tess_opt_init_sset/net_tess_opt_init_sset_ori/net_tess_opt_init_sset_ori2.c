/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
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
