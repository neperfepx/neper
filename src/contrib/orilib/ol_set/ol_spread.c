/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_spread.h"

void
ol_set_disori (struct OL_SET set, double *qmean, double *pdisori)
{
  unsigned int i;
  double tmp, sumweight;

  (*pdisori) = 0;
  sumweight = 0;
  for (i = 0; i < set.size; i++)
    if (set.id[i] == 1)
    {
      ol_q_q_disori (set.q[i], qmean, set.crysym, &tmp);
      (*pdisori) += set.weight[i] * tmp;
      sumweight += set.weight[i];
    }
  (*pdisori) /= sumweight;

  return;
}

void
ol_set_disoriset (struct OL_SET set1, struct OL_SET *psetd)
{
  unsigned int i;
  double *qm = ol_q_alloc ();

  ol_set_mean_iter (set1, qm);

  (*psetd).size = set1.size;
  for (i = 0; i < set1.size; i++)
  {
    ol_q_q_qdisori (qm, set1.q[i], set1.crysym, (*psetd).q[i]);
    (*psetd).weight[i] = set1.weight[i];
    (*psetd).id[i] = set1.id[i];
  }

  ol_q_free (qm);

  return;
}

void
ol_set_disoriset_cur (struct OL_SET set1, struct OL_SET *psetd)
{
  unsigned int i;
  double *qm = ol_q_alloc ();

  ol_set_mean_iter (set1, qm);

  (*psetd).size = set1.size;
  for (i = 0; i < set1.size; i++)
  {
    ol_q_q_qdisori_cur (qm, set1.q[i], set1.crysym, (*psetd).q[i]);
    (*psetd).weight[i] = set1.weight[i];
    (*psetd).id[i] = set1.id[i];
  }

  ol_q_free (qm);

  return;
}

void
ol_set_mean_disoriset (struct OL_SET set1, double *qm, struct OL_SET *psetd)
{
  unsigned int i;

  (*psetd).size = set1.size;
  for (i = 0; i < set1.size; i++)
  {
    ol_q_q_qdisori (qm, set1.q[i], set1.crysym, (*psetd).q[i]);
    (*psetd).weight[i] = set1.weight[i];
  }

  return;
}

void
ol_set_mean_disoriset_cur (struct OL_SET set1, double *qm,
                           struct OL_SET *psetd)
{
  unsigned int i;

  (*psetd).size = set1.size;
  for (i = 0; i < set1.size; i++)
  {
    ol_q_q_qdisori_cur (qm, set1.q[i], set1.crysym, (*psetd).q[i]);
    (*psetd).weight[i] = set1.weight[i];
  }

  return;
}

int
ol_set_filter (struct OL_SET set1, double rel, struct OL_SET *pset2,
               double *qmean, double *pdis)
{
  unsigned int i;
  double *qm = ol_q_alloc ();
  double mdis;
  double *dis = ut_alloc_1d (set1.size);

  if (set1.size == 0)
  {
    ut_print_message (2, 0, "ol_set_filter: input set of size 0\n");
    abort ();
  }
  else if (set1.size == 1)
  {
    ol_set_memcpy (set1, pset2);
    return 0;
  }

  ol_set_mean (set1, qm);

  mdis = 0;
  for (i = 0; i < set1.size; i++)
  {
    ol_q_q_disori (qm, set1.q[i], set1.crysym, &dis[i]);
    mdis += dis[i];
  }
  mdis *= (rel / set1.size);

  ol_set_memcpy (set1, pset2);
  for (i = 0; i < set1.size; i++)
    if (!(dis[i] < mdis + OL_EPS_DEG))
      (*pset2).id[i] = 0;

  ol_q_free (qm);
  ut_free_1d (&dis);

  if (pdis != NULL)
    (*pdis) = mdis;
  if (qmean != NULL)
    ol_q_memcpy (qm, qmean);

  if ((*pset2).size == 0)
  {
    ol_set_memcpy (set1, pset2);
    return -1;
  }
  else
    return set1.size - (*pset2).size;
}

int
ol_set_filter_iter (struct OL_SET set1, double rel, struct OL_SET *pset2,
                    double *qmean, double *pdis)
{
  ol_set_filter (set1, rel, pset2, qmean, pdis);

  while (ol_set_filter (*pset2, rel, pset2, qmean, pdis) > 0)
    continue;

  if ((*pset2).size == 0)
    return -1;
  else
    return set1.size - (*pset2).size;
}

int
ol_set_anifilter (struct OL_SET set1, double rel, struct OL_SET *pset2)
{
  unsigned int i;
  double *qm = ol_q_alloc ();
  double **v = ut_alloc_2d (3, 3);
  double *mdis = ut_alloc_1d (3);
  double *dis = ut_alloc_1d (3);
  struct OL_SET setd = ol_set_alloc (set1.size, set1.crysym);

  if (set1.size == 0)
  {
    ut_print_message (2, 0, "ol_set_filter: input set of size 0\n");
    abort ();
  }

  ol_set_disoriset (set1, &setd);
  ol_set_aniso (setd, v, mdis);
  ut_array_1d_scale (mdis, 3, rel);

  ol_set_memcpy (set1, pset2);
  for (i = 0; i < set1.size; i++)
  {
    ol_q_aniso_theta (setd.q[i], v, dis);
    if (!(fabs (dis[0]) < mdis[0] + OL_EPS_DEG
          && fabs (dis[1]) < mdis[1] + OL_EPS_DEG
          && fabs (dis[2]) < mdis[2] + OL_EPS_DEG))
      (*pset2).id[i] = 0;
  }

  ol_q_free (qm);
  ut_free_2d (&v, 3);
  ut_free_1d (&mdis);
  ut_free_1d (&dis);
  ol_set_free (&setd);

  if ((*pset2).size == 0)
  {
    ol_set_memcpy (set1, pset2);
    return -1;
  }
  else
    return set1.size - (*pset2).size;
}

int
ol_set_mean_anifilter (struct OL_SET set1, double *qm, double rel,
                       struct OL_SET *pset2)
{
  unsigned int i, j;
  double **v = ut_alloc_2d (3, 3);
  double *mdis = ut_alloc_1d (3);
  double *relmdis = ut_alloc_1d (3);
  double *dis = ut_alloc_1d (3);
  struct OL_SET setd = ol_set_alloc (set1.size, set1.crysym);
  double radius;

  if (set1.size == 0)
  {
    ut_print_message (2, 0, "ol_set_filter: input set of size 0\n");
    abort ();
  }

  ol_set_mean_disoriset (set1, qm, &setd);
  ol_set_aniso (setd, v, mdis);

  for (i = 0; i < 3; i++)
    relmdis[i] = rel * mdis[i];

  ol_set_memcpy (set1, pset2);
  for (i = 0; i < set1.size; i++)
  {
    ol_q_aniso_theta (setd.q[i], v, dis);

    radius = 0;
    for (j = 0; j < 3; j++)
      radius += pow (dis[j] / relmdis[j], 2);
    if (radius >= 1)
      (*pset2).id[i] = 0;
  }

  ut_free_2d (&v, 3);
  ut_free_1d (&mdis);
  ut_free_1d (&relmdis);
  ut_free_1d (&dis);
  ol_set_free (&setd);

  if (ut_array_1d_int_eltpos ((*pset2).id, (*pset2).size, 1) == -1)
  {
    ol_set_memcpy (set1, pset2);
    return -1;
  }
  else
    return set1.size - (*pset2).size;
}


int
ol_set_mean_aniso_anifilter (struct OL_SET set1, double *qm, double **v,
                             double *mdis, double rel, struct OL_SET *pset2)
{
  unsigned int i, j;
  double radius;
  double *relmdis = ut_alloc_1d (3);
  double *dis = ut_alloc_1d (3);
  struct OL_SET setd;

  if (set1.size == 0)
  {
    ut_print_message (2, 0, "Input set of size 0\n");
    abort ();
  }

  setd = ol_set_alloc (set1.size, set1.crysym);
  ol_set_mean_disoriset (set1, qm, &setd);

  for (i = 0; i < 3; i++)
    relmdis[i] = rel * mdis[i];

  ol_set_memcpy (set1, pset2);
  for (i = 0; i < set1.size; i++)
  {
    ol_q_aniso_theta (setd.q[i], v, dis);
    radius = 0;
    for (j = 0; j < 3; j++)
      radius += (dis[j] / relmdis[j]) * (dis[j] / relmdis[j]);
    if (radius >= 1)
      (*pset2).id[i] = 0;
  }

  ut_free_1d (&relmdis);
  ut_free_1d (&dis);
  ol_set_free (&setd);

  if ((*pset2).size == 0)
  {
    ol_set_memcpy (set1, pset2);
    return -1;
  }
  else
    return set1.size - (*pset2).size;
}

int
ol_set_mean_aniso_anifilter_iter (struct OL_SET set1, double *qm0,
                                  double **v0, double *mdis0, double rel,
                                  struct OL_SET *pset2, double *qm,
                                  double **v, double *mdis)
{
  return ol_set_mean_aniso_anifilter_iter_itermax (set1, qm0, v0, mdis0,
                                                   rel, OL_ITERMAX2, pset2,
                                                   qm, v, mdis);
}


int
ol_set_anifilter_iter (struct OL_SET set1, double rel,
                       struct OL_SET *pset2, double *qm,
                       double **v, double *mdis)
{
  return ol_set_anifilter_iter_itermax (set1, rel, OL_ITERMAX2,
                                        pset2, qm, v, mdis);
}


int
ol_set_anifilter_iter_itermax (struct OL_SET set1, double rel,
                               int maxiter, struct OL_SET *pset2, double *qm,
                               double **v, double *mdis)
{
  return ol_set_mean_aniso_anifilter_iter_itermax (set1, NULL, NULL,
                                                   NULL, rel, maxiter, pset2,
                                                   qm, v, mdis);
}


int
ol_set_mean_aniso_anifilter_iter_itermax (struct OL_SET set1, double *qmean0,
                                          double **v0, double *mdis0,
                                          double rel, int maxiter,
                                          struct OL_SET *pset2, double *qm,
                                          double **v, double *mdis)
{
  int i;
  double *q = ol_q_alloc ();
  double *qmean = ol_q_alloc ();
  double *r = ol_r_alloc ();
  double theta;
  double **gshake = ol_g_alloc ();
  double *mdis_cur = ut_alloc_1d (3);
  double **v_cur = ol_g_alloc ();
  double *mdis_prev = ut_alloc_1d (3);
  double **v_prev = ol_g_alloc ();
  struct OL_SET Setd;
  struct OL_SET set1cpy = ol_set_alloc (set1.size, set1.crysym);
  ol_set_memcpy (set1, &set1cpy);

  ol_set_memcpy (set1cpy, pset2);
  Setd = ol_set_alloc (set1cpy.size, set1cpy.crysym);

  ut_array_1d_set (mdis_prev, 3, 1e6);
  ol_g_set_id (v_prev);

  for (i = 1; i <= maxiter; i++)
  {
    // calculating mean
    // if first iteration: if mean provided as input, taking it;
    // otherwise, computing it.
    if (i == 1)
    {
      if (qmean0)
        ol_q_memcpy (qmean0, qmean);
      else
        ol_set_mean_iter (*pset2, qmean);
    }
    // if not first increment, computing mean using the previous one as
    // reference orientation.
    else
      ol_set_mean_iter_ref (*pset2, qmean, qmean);

    // mean orientation is now recorded in qmean.

    // computing the disorientation set
    ol_set_mean_disoriset (*pset2, qmean, &Setd);

    // calculating anisotropy properties
    // if first increment and provided as input, taking them;
    // otherwise, computing them
    if (i == 1 && v0 && mdis0)
    {
      ol_g_memcpy (v0, v_cur);
      ol_e_memcpy (mdis, mdis_cur);
    }
    else
      ol_set_aniso (Setd, v_cur, mdis_cur);

    // eigen values / vectors now recorded in mdis_cur / v_cur.

    // general case: filtering orientation set based on mean
    // orientations and anisotropy properties
    if (i % 50 != 0)
      ol_set_mean_aniso_anifilter (set1cpy, qmean, v_cur, mdis_cur,
                                   rel, pset2);

    // once in a while, for speeding up convergence
    else
    {
      // moving sligthly the average orientation
      ol_nb_max_rtheta (drand48 (), drand48 (), drand48 (),
                        2 * 50 / i * mdis_cur[2], r, &theta);
      ol_rtheta_q (r, theta, q);
      ol_q_q_q (qmean, q, qmean);

      // filtering
      ol_set_mean_aniso_anifilter (set1cpy, qmean, v_cur, mdis_cur,
                                   rel, pset2);

      // if the filtered set is void, rotating slightly the eigen
      // vectors until it gets non-void
      int actualsize = ut_array_1d_int_sum ((*pset2).id, (*pset2).size);
      while (actualsize == 0)
      {
        ol_nb_max_rtheta (drand48 (), drand48 (), drand48 (),
                          2 * 50 / i * mdis_cur[2], r, &theta);
        ol_rtheta_g (r, theta, gshake);
        ol_g_g_g (v_cur, gshake, v_cur);
        ol_set_mean_aniso_anifilter (set1cpy, qmean, v_cur, mdis_cur,
                                     rel, pset2);
        actualsize = ut_array_1d_int_sum ((*pset2).id, (*pset2).size);
      }
    }

    // testing convergence
    // theta is the rotation of the eigen basis
    ol_g_g_disori (v_cur, v_prev, "mmm", &theta);
    // if less than 2 degrees, and each of the eigen values have changed
    // by less than 1%, considering that we are converged.
    if (theta < 2)
      if (2 * fabs (mdis_cur[0] - mdis_prev[0]) /
          (mdis_cur[0] + mdis_prev[0]) < 0.01
          && 2 * fabs (mdis_cur[1] - mdis_prev[1]) / (mdis_cur[1] +
                                                      mdis_prev[1]) < 0.01
          && 2 * fabs (mdis_cur[2] - mdis_prev[2]) / (mdis_cur[2] +
                                                      mdis_prev[2]) < 0.01)
        break;

    // if not converged (have not exited the loop), copying eigen
    // decomposition in mdis_prev / v_prev
    ol_r_memcpy (mdis_cur, mdis_prev);
    ol_g_memcpy (v_cur, v_prev);
  }

  // copying mean orientation and eigen decomposition if necessary
  if (qm)
    ol_q_memcpy (qmean, qm);

  // recomputing eigen decomposition to make sure it matches the
  // filtered orientation set

  ol_set_aniso (Setd, v, mdis);

  ol_set_free (&Setd);
  ol_set_free (&set1cpy);

  return (i < maxiter) ? i : -1;
}

void
ol_set_aniso_modes (struct OL_SET Set, double *qmean,
                    struct OL_SET *pSet1, struct OL_SET *pSet2)
{
  unsigned int i;

  struct OL_SET Setd = ol_set_alloc (Set.size, Set.crysym);

  double **v = ol_g_alloc ();
  double *theta = ol_e_alloc ();

  (*pSet1) = ol_set_alloc (Set.size, Set.crysym);
  (*pSet2) = ol_set_alloc (Set.size, Set.crysym);

  for (i = 0; i < Set.size; i++)
    ol_q_q_qdisori (qmean, Set.q[i], Set.crysym, Setd.q[i]);

  ol_set_aniso (Setd, v, theta);

  (*pSet1).size = 0;
  (*pSet2).size = 0;
  for (i = 0; i < Setd.size; i++)
  {
    ol_q_aniso_theta (Setd.q[i], v, theta);

    if (theta[0] < 0)
    {
      ol_q_memcpy (Set.q[i], (*pSet1).q[(*pSet1).size]);
      (*pSet1).weight[(*pSet1).size] = Set.weight[i];
      (*pSet1).id[(*pSet1).size] = Set.id[i];
      (*pSet1).size++;
    }
    else
    {
      ol_q_memcpy (Set.q[i], (*pSet2).q[(*pSet2).size]);
      (*pSet2).weight[(*pSet2).size] = Set.weight[i];
      (*pSet2).id[(*pSet2).size] = Set.id[i];
      (*pSet2).size++;
    }
  }

  ol_set_free (&Setd);
  ol_g_free (v);
  ol_r_free (theta);

  return;
}

void
ol_set_aniso_v_modes (struct OL_SET Set, double *qmean, double **v,
                      struct OL_SET *pSet1, struct OL_SET *pSet2)
{
  ol_set_aniso_v_thetatr_modes (Set, qmean, v, 0, pSet1, pSet2);

  return;
}

void
ol_set_aniso_v_thetatr_modes (struct OL_SET Set, double *qmean, double **v,
                              double thetatr, struct OL_SET *pSet1,
                              struct OL_SET *pSet2)
{
  unsigned int i;
  int *id = ut_alloc_1d_int (Set.size);
  int size1, size2;

  ol_set_aniso_v_thetatr_modes_id (Set, qmean, v, thetatr, id);

  size1 = ut_array_1d_int_valnb (id, Set.size, 1);
  size2 = ut_array_1d_int_valnb (id, Set.size, 2);
  (*pSet1) = ol_set_alloc (size1, Set.crysym);
  (*pSet2) = ol_set_alloc (size2, Set.crysym);

  (*pSet1).size = 0;
  (*pSet2).size = 0;
  for (i = 0; i < Set.size; i++)
  {
    if (id[i] == 1)
    {
      ol_q_memcpy (Set.q[i], (*pSet1).q[(*pSet1).size]);
      (*pSet1).weight[(*pSet1).size] = Set.weight[i];
      (*pSet1).id[(*pSet1).size] = Set.id[i];
      (*pSet1).size++;
    }
    else
    {
      ol_q_memcpy (Set.q[i], (*pSet2).q[(*pSet2).size]);
      (*pSet2).weight[(*pSet2).size] = Set.weight[i];
      (*pSet2).id[(*pSet2).size] = Set.id[i];
      (*pSet2).size++;
    }
  }

  ut_free_1d_int (&id);

  return;
}

void
ol_set_aniso_v_thetatr_modes_id (struct OL_SET Set, double *qmean,
                                 double **v, double thetatr, int *id)
{
  unsigned int i;
  struct OL_SET Setd = ol_set_alloc (Set.size, Set.crysym);
  double *theta = ol_e_alloc ();

  for (i = 0; i < Set.size; i++)
    ol_q_q_qdisori (qmean, Set.q[i], Set.crysym, Setd.q[i]);

  for (i = 0; i < Setd.size; i++)
  {
    ol_q_aniso_theta (Setd.q[i], v, theta);

    id[i] = (theta[0] < thetatr) ? 1 : 2;
  }

  ol_set_free (&Setd);
  ol_r_free (theta);

  return;
}
