/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_gen.h"

void
ol_set_misorispread (char *distrib, int dim, long int random, struct OL_SET *pOSet)
{
  int i, j;
  int varqty;
  char *fct = NULL;
  char **vars = NULL;
  char **vals = NULL;
  gsl_rng *rand = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rand, random);

  ut_string_function (distrib? distrib : "none", &fct, &vars, &vals, &varqty);

  if (!strcmp (fct, "none"))
  {
    for (i = 0; i < (int) (*pOSet).size; i++)
      ol_q_set_id ((*pOSet).q[i]);
  }

  else if (!strcmp (fct, "normal"))
  {
    double *sig = ut_alloc_1d (dim), *v = ut_alloc_1d (3);

    for (i = 0; i < varqty; i++)
    {
      if (!vars[i] || !strcmp (vars[i], "theta"))
      {
        sscanf (vals[i], "%lf", sig);
        ut_array_1d_set (sig, dim, sig[0]);
      }
      else if (!strcmp (vars[i], "theta1"))
        sscanf (vals[i], "%lf", sig);
      else if (!strcmp (vars[i], "theta2"))
        sscanf (vals[i], "%lf", sig + 1);
      else if (!strcmp (vars[i], "theta3"))
        sscanf (vals[i], "%lf", sig + 2);
      else if (!strcmp (vars[i], "thetam"))
      {
        // Glez and Driver, J. Appl. Cryst., 2001
        // we use sig[0] as a tmp variable
        sscanf (vals[i], "%lf", sig);
        if (dim == 3)
          ut_array_1d_set (sig, dim, sig[0] / (2 * sqrt (2 / M_PI)));
        else if (dim == 2)
          ut_array_1d_set (sig, dim, sig[0] / sqrt (M_PI / 2));
        else if (dim == 1)
          ut_array_1d_set (sig, dim, sig[0] / sqrt (2 / M_PI));
      }
    }

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      for (j = 0; j < dim; j++)
        v[j] = (sig[j] > 0) ? gsl_ran_gaussian (rand, sig[j] / 2) : 0;

      ol_lnq_q (v, (*pOSet).q[i]);
    }

    ut_free_1d (&sig);
    ut_free_1d (&v);
  }

  else if (!strcmp (fct, "flat"))
  {
    double *r = ol_r_alloc ();
    double tmp, theta, thetamax = 0;

    for (i = 0; i < varqty; i++)
    {
      if (!strcmp (vars[i], "theta"))
        thetamax = atof (vals[i]);
    }

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      if (dim == 3)
      ol_nb_max_rtheta (gsl_rng_uniform (rand), gsl_rng_uniform (rand),
                        gsl_rng_uniform (rand), thetamax, r, &theta);
      else if (dim == 2)
      {
        tmp = gsl_rng_uniform (rand) * 2 * M_PI;
        ut_array_1d_set_2 (r, cos (tmp), sin (tmp));
        theta = thetamax * pow (gsl_rng_uniform (rand), 0.5);
      }
      else
        abort ();

      ol_rtheta_q (r, theta, (*pOSet).q[i]);
    }

    ol_r_free (r);
  }

  else
    abort ();

  gsl_rng_free (rand);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&fct);

  return;
}

void
ol_set_clean (struct OL_SET *pOSet)
{
  int i, pos, newsize, oldsize;
  double **q = ut_alloc_2d ((*pOSet).size, 4);
  double *weight = ut_alloc_1d ((*pOSet).size);
  double *theta = ut_alloc_1d ((*pOSet).size);
  int *id = ut_alloc_1d_int ((*pOSet).size);

  oldsize = (*pOSet).size;
  ut_array_2d_memcpy     ((*pOSet).q,      oldsize, 4, q);
  ut_array_1d_memcpy     ((*pOSet).weight, oldsize, weight);
  if ((*pOSet).theta)
    ut_array_1d_memcpy     ((*pOSet).theta,  oldsize, theta);
  if ((*pOSet).id)
    ut_array_1d_int_memcpy ((*pOSet).id,     oldsize, id);

  // first pass to determine size
  newsize = 0;
  for (i = 0; i < (int) (*pOSet).size; i++)
    if ((*pOSet).weight[i] > 0 && (!(*pOSet).id || (*pOSet).id[i] == 1))
      newsize++;

  if (newsize < oldsize)
  {
    ut_free_2d (&(*pOSet).q, (*pOSet).size);

    (*pOSet).size = newsize;

    (*pOSet).q = ut_alloc_2d ((*pOSet).size, 4);
    (*pOSet).weight = ut_realloc_1d ((*pOSet).weight, (*pOSet).size);
    if ((*pOSet).theta)
      (*pOSet).theta = ut_realloc_1d ((*pOSet).theta, (*pOSet).size);
    if ((*pOSet).id)
      (*pOSet).id = ut_realloc_1d_int ((*pOSet).id, (*pOSet).size);

    pos = 0;
    for (i = 0; i < oldsize; i++)
      if (weight[i] > 0 && (!(*pOSet).id || id[i] == 1))
      {
        ut_array_1d_memcpy (q[i], 4, (*pOSet).q[pos]);
        (*pOSet).weight[pos] = weight[i];
        if ((*pOSet).theta)
        (*pOSet).theta[pos] = theta[i];
        if ((*pOSet).id)
          (*pOSet).id[pos] = id[i];
        pos++;
      }
  }

  ut_free_2d (&q, oldsize);
  ut_free_1d (&weight);
  ut_free_1d (&theta);
  ut_free_1d_int (&id);

  return;
}

void
ol_set_addori (struct OL_SET *pOSet, double *q, double weight, double theta, int id)
{
  (*pOSet).size++;

  (*pOSet).q = ut_realloc_2d_addline ((*pOSet).q, (*pOSet).size, 4);
  ut_array_1d_memcpy (q, 4, (*pOSet).q[(*pOSet).size - 1]);

  if (weight >= 0) // orientations may have a zero weight
  {
    (*pOSet).weight = ut_realloc_1d ((*pOSet).weight, (*pOSet).size);
    (*pOSet).weight[(*pOSet).size - 1] = weight;
  }

  if (theta >= 0) // orientations may have a zero theta
  {
    (*pOSet).theta = ut_realloc_1d ((*pOSet).theta, (*pOSet).size);
    (*pOSet).theta[(*pOSet).size - 1] = theta;
  }

  if (id >= 0)
  {
    (*pOSet).id = ut_realloc_1d_int ((*pOSet).id, (*pOSet).size);
    (*pOSet).id[(*pOSet).size - 1] = id;
  }

  return;
}

void
ol_set_expandset (struct OL_SET OSet1, struct OL_SET *pOSet2)
{
  int i, j;
  struct OL_SET OSet1c;

  ol_set_zero (&OSet1c);
  ol_set_zero (pOSet2);

  ol_set_memcpy (OSet1, &OSet1c);

  int isspecial = 0;
  for (i = 0; i < (int) OSet1c.size; i++)
    if (ol_q_isspecial (OSet1c.q[i]))
    {
      isspecial = 1;
      break;
    }

  // standard orientation set
  if (!isspecial)
    ol_set_memcpy (OSet1c, pOSet2);

  else
  {
    ut_string_string (OSet1c.crysym, &((*pOSet2).crysym));

    for (i = 0; i < (int) OSet1c.size; i++)
    {
      if (!ol_q_isspecial (OSet1c.q[i]))
      {
        ol_set_addori (pOSet2, OSet1c.q[i],
                       OSet1c.weight ? OSet1c.weight[i] : 1,
                       OSet1c.theta ? OSet1c.theta[i] : 0,
                       OSet1c.id ? OSet1c.id[i] : i);
      }
      else
      {
        struct OL_SET OSet3;
        char *label = ut_alloc_1d_char (100);

        ol_q_label (OSet1c.q[i], label);

        ol_q_special_set (OSet1c.q[i], &OSet3);

        for (int j = 0; j < (int) OSet3.size; j++)
          if (OSet3.weight && isnan (OSet3.weight[j]))
            abort ();

        for (j = 0; j < (int) OSet3.size; j++)
        {
          ol_set_addori (pOSet2, OSet3.q[j],
                         OSet1c.weight ? OSet1c.weight[i] / OSet3.size : 1. / OSet3.size,
                         OSet1c.theta ? OSet1c.theta[i] : 0,
                         OSet1c.id ? OSet1c.id[i] : i);

          if (isnan ((*pOSet2).weight[(*pOSet2).size - 1]))
            abort ();

          if (!strcmp (label, "uniform"))
          {
            if (!(*pOSet2).theta)
              (*pOSet2).theta = ut_alloc_1d ((*pOSet2).size);

            (*pOSet2).theta[j] = DBL_MAX;
          }
        }

        ut_free_1d_char (&label);

        ol_set_free (&OSet3);
      }
    }
  }

  return;
}

int
ol_q_special_set (double *qlabel, struct OL_SET *pOSet)
{
  double *qref = ol_q_alloc ();
  double *qrot = ol_q_alloc ();
  double *q = ol_q_alloc ();
  double *r = ol_r_alloc ();
  double *R = ol_R_alloc ();
  double *qnew = ol_q_alloc ();
  double theta, thetamin, thetamax;
  int i, stepqty, status;
  double theta_step = 0.5;

  ol_set_zero (pOSet);

  status = 0;

  // random / uniform
  if (ut_num_equal (qlabel[0], UNIFORM, 1e-6))
  {
    ol_r_set_this (r, 1, 0, 0);
    ol_rtheta_q (r, 0.1, qnew); // any orientation would do.  We avoid (0,0,0) because of a bug in openGJK down the road.
    ol_set_addori (pOSet, qnew, 0, DBL_MAX, 0);
  }

  // fcc_psc_alpha
  else if (ut_num_equal (qlabel[0], FCC_PSC_ALPHA, 1e-6))
  {
    thetamin = -45;
    thetamax = 45;
    stepqty = ut_num_d2ri ((thetamax - thetamin) / theta_step);

    ol_r_set_this (r, 1, 0, 0);
    ol_rtheta_q (r, 45, qref);
    ol_r_set_this (r, 0, 0, 1);

    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, thetamin + (thetamax - thetamin) * ((double) i / stepqty), qrot);
      ol_q_q_q_ref (qref, qrot, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }
  }

  // fcc_psc_alpha2
  else if (ut_num_equal (qlabel[0], FCC_PSC_ALPHA2, 1e-6))
  {
    thetamin = 45;
    thetamax = 135;
    stepqty = ut_num_d2ri ((thetamax - thetamin) / theta_step);

    ol_r_set_this (r, 1, 0, 0);
    ol_rtheta_q (r, 45, qref);
    ol_r_set_this (r, 0, 0, 1);

    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, thetamin + (thetamax - thetamin) * ((double) i / stepqty), qrot);
      ol_q_q_q_ref (qref, qrot, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }
  }

  // fcc_psc_alpha_full
  else if (ut_num_equal (qlabel[0], FCC_PSC_ALPHA_FULL, 1e-6))
  {
    thetamin = -90;
    thetamax = 90;
    stepqty = ut_num_d2ri ((thetamax - thetamin) / theta_step);

    ol_r_set_this (r, 1, 0, 0);
    ol_rtheta_q (r, 45, qref);
    ol_r_set_this (r, 0, 0, 1);

    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, thetamin + (thetamax - thetamin) * ((double) i / stepqty), qrot);
      ol_q_q_q_ref (qref, qrot, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }
  }

  else if (ut_num_equal (qlabel[0], FCC_PSC_BETA, 1e-6))
  {
    double *R1 = ol_R_alloc ();
    double *R2 = ol_R_alloc ();
    double *q1 = ol_q_alloc ();
    double *q2 = ol_q_alloc ();

    ol_R_set_this (R1, 0.414213562373, 0.171572875254, 0.414213562373);
    ol_R_set_this (R2, -0.171572875254, 0.414213562373, 0.414213562373);
    ol_R_q (R1, q1);
    ol_R_q (R2, q2);

    // first segment
    ol_q_q_qmisori (q1, q2, qrot);
    ol_q_rtheta (qrot, r, &theta);

    stepqty = ut_num_d2ri (theta / theta_step);

    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, theta * i / stepqty, qrot);
      ol_q_q_q_ref (q1, qrot, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }

    // second segment
    for (i = 0; i < stepqty + 1; i++)
    {
      ut_array_1d_memcpy ((*pOSet).q[i], 4, qnew);
      ut_array_1d_scale (qnew, 2, -1);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }

    ol_R_free (R1);
    ol_R_free (R2);
    ol_q_free (q1);
    ol_q_free (q2);
  }

  else if (ut_num_equal (qlabel[0], FCC_PSC_BETA_BENT, 1e-6))
  {
    int j;
    double *R1 = ol_R_alloc ();
    double *R2 = ol_R_alloc ();
    double *Rtmp = ol_R_alloc ();
    double *Rc = ol_R_alloc ();
    double *q1 = ol_q_alloc ();
    double *q2 = ol_q_alloc ();
    double *v1 = ut_alloc_1d (3);
    double *v2 = ut_alloc_1d (3);
    double *z  = ut_alloc_1d (3);
    z[2] = 1;
    double dist1, dist;

    ol_R_set_this (R1, 0.414213562373, 0.171572875254, 0.414213562373);
    ol_R_set_this (R2, -0.171572875254, 0.414213562373, 0.414213562373);
    ut_array_1d_add (R1, R2, 3, Rtmp);
    ut_array_1d_scale (Rtmp, 3, 0.5);
    ut_array_1d_sub (R1, R2, 3, v1);
    ut_array_1d_scale (v1, 3, 0.5);
    ut_vector_vectprod (v1, z, v2);
    ut_array_1d_scale (v2, 3, 3); // controls how far is the center / straight is the segment
    ut_array_1d_add (Rtmp, v2, 3, Rc);
    ol_R_q (R1, q1);
    ol_R_q (R2, q2);

    // first segment
    ol_q_q_qmisori (q1, q2, qrot);
    ol_q_rtheta (qrot, r, &theta);

    stepqty = ut_num_d2ri (theta / theta_step);

    dist1 = ut_space_dist (R1, Rc);
    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, theta * i / stepqty, qrot);
      ol_q_q_q_ref (q1, qrot, qnew);
      ol_q_R (qnew, Rtmp);
      dist = ut_space_dist (Rtmp, Rc);
      ut_array_1d_sub (Rc, Rtmp, 3, v1);
      for (j = 0; j < 3; j++)
        Rtmp[j] = Rc[j] + v1[j] * dist1 / dist;
      ol_R_q (Rtmp, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }

    // second segment
    for (i = 0; i < stepqty + 1; i++)
    {
      ut_array_1d_memcpy ((*pOSet).q[i], 4, qnew);
      ut_array_1d_scale (qnew, 2, -1);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }

    ol_R_free (R1);
    ol_R_free (R2);
    ol_R_free (Rtmp);
    ol_R_free (Rc);
    ut_free_1d (&v1);
    ut_free_1d (&v2);
    ut_free_1d (&z);
    ol_q_free (q1);
    ol_q_free (q2);
  }

  // 100_x3
  else if (ut_num_equal (qlabel[0], _100_X3, 1e-6))
  {
    thetamin = -45;
    thetamax = 45;
    stepqty = ut_num_d2ri ((thetamax - thetamin) / theta_step);

    ol_q_set_unit (qref);
    ol_r_set_this (r, 0, 0, 1);

    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, thetamin + (thetamax - thetamin) * ((double) i / stepqty), qrot);
      ol_q_q_q_ref (qref, qrot, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
    }
  }

  // 110_x3
  else if (ut_num_equal (qlabel[0], _110_X3, 1e-6))
  {
    thetamin = -180;
    thetamax = 180;
    stepqty = ut_num_d2ri ((thetamax - thetamin) / theta_step);

    ol_r_set_this (r, 1 / sqrt (2), -1 / sqrt (2), 0);
    ol_rtheta_q_rad (r, acos (1. / sqrt (3)), qref);
    ol_r_set_this (r, 0, 0, 1);

    for (i = 0; i < stepqty + 1; i++)
    {
      ol_rtheta_q (r, thetamin + (thetamax - thetamin) * ((double) i / stepqty), qrot);
      ol_q_q_q_ref (qref, qrot, qnew);
      ol_set_addori (pOSet, qnew, 0, 0, 0);
      double *R = ol_R_alloc ();
      ol_q_R (qnew, R);
      printf ("AA: "); ol_R_fprintf (stdout, R, "%f");
    }
  }

  else
    status = -1;

  ol_q_free (qref);
  ol_q_free (qrot);
  ol_q_free (q);
  ol_r_free (r);
  ol_R_free (R);
  ol_q_free (qnew);

  return status;
}
