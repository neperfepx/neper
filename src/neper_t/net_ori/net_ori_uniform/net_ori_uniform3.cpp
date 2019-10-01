/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_uniform_.h"

void
net_ori_uniform_opt_forces (struct OL_SET *pOSet,
                    double **f, double *E, struct OOPT *pOOpt,
                    struct QCLOUD *pqcloud, my_kd_tree_t *qindex)
{
  struct timeval beg_time, end_time;

  gettimeofday (&beg_time, NULL);

  ut_array_2d_zero (f, (*pOSet).size, 3);

  if ((*pOOpt).neighd > M_PI - OL_EPS_RAD)
    net_ori_uniform_opt_forces_ser (pOSet, f, E);
  else
    net_ori_uniform_opt_forces_ser_neigh (pOSet, f, E, pOOpt, pqcloud, qindex);

  gettimeofday (&end_time, NULL);

  neut_ori_f_norm (f, (*pOSet).size, &(*pOOpt).avforce);

  if ((*pOOpt).avforce_ini == DBL_MAX)
    (*pOOpt).avforce_ini = (*pOOpt).avforce;

  (*pOOpt).f = ((*pOOpt).f == DBL_MAX) ? 1 : (*pOOpt).avforce / (*pOOpt).avforce_ini;

  (*pOOpt).fmin = ((*pOOpt).fmin == DBL_MAX) ? 1 : ut_num_min ((*pOOpt).fmin, (*pOOpt).f);

  if (pOOpt)
    (*pOOpt).force_dur = ut_time_subtract (&beg_time, &end_time);

  return;
}

int
net_ori_uniform_opt_energy (int iter, struct OL_SET *pOSet,
                    double *E, double **pEtot, struct OOPT *pOOpt)
{
  struct timeval beg_time, end_time;

  (*pEtot) = ut_realloc_1d (*pEtot, iter + 1);

  gettimeofday (&beg_time, NULL);

  (*pEtot)[iter] = ut_array_1d_sum (E, (*pOSet).size);

  gettimeofday (&end_time, NULL);

  if (pOOpt)
    (*pOOpt).potential_dur = ut_time_subtract (&beg_time, &end_time);

  return 0;
}

void
net_ori_uniform_opt_alpha (struct OOPT *pOOpt, struct OL_SET OSet, double **dq,
                   int iter, double **f, double **fc, double *palpha)
{
  unsigned int i;
  double r, tmp1, tmp2;

  neut_ori_n_avradeq (&((*pOOpt).thetafct), OSet.size, OSet.crysym, &r);

  // first iteration
  if (iter == 1 && ((*palpha) == 0 || (*palpha) == DBL_MAX))
  {
    double *qdotnorm = ut_alloc_1d (OSet.size);
#pragma omp parallel for
    for (i = 0; i < OSet.size; i++)
    {
      neut_ori_f_qdotnorm (f[i], qdotnorm + i);
      if (qdotnorm[i] == 0)
        qdotnorm[i] = DBL_MAX;
    }

    (*palpha) = 0.8 * pow (r, 3) / 3.4;
    ut_free_1d (qdotnorm);
  }

  // Barzilai-Borwein
  else
  {
    tmp1 = 0;
    tmp2 = 0;
#pragma omp parallel for
    for (i = 0; i < OSet.size; i++)
    {
      double deltaf[3];
      ut_array_1d_sub (fc[i], f[i], 3, deltaf);
      if (iter % 2 == 0)        // alterning between alpha1 and alpha2
      {
#pragma omp atomic
        tmp1 += ut_array_1d_scalprod (dq[i], dq[i], 3);
#pragma omp atomic
        tmp2 += ut_array_1d_scalprod (dq[i], deltaf, 3);
      }
      else
      {
#pragma omp atomic
        tmp1 += ut_array_1d_scalprod (dq[i], deltaf, 3);
#pragma omp atomic
        tmp2 += ut_array_1d_scalprod (deltaf, deltaf, 3);
      }
    }

    (*palpha) = -tmp1 / tmp2;
  }

  return;
}

void
net_ori_uniform_opt_rot (struct OOPT *pOOpt, double **f, double alpha,
                 struct OL_SET *pOSet, double **dq)
{
  unsigned int i;
  struct timeval beg_time, end_time;

  gettimeofday (&beg_time, NULL);

#pragma omp parallel for
  for (i = 0; i < (*pOSet).size; i++)
    if (!(*pOOpt).fixoriqty || (*pOOpt).fixori[i] == 0)
      net_ori_uniform_opt_rot_ori (f, alpha, i, pOSet, dq);

  gettimeofday (&end_time, NULL);

  if (pOOpt)
    (*pOOpt).rot_dur = ut_time_subtract (&beg_time, &end_time);

  return;
}

void
net_ori_uniform_opt_verbosity (struct OOPT OOpt, int iter,
                               char *prevmessage, char *message,
                               int verbositylevel)
{
  /*
  if (0 && iter == 0)
  {
    ut_print_message (0, verbositylevel, "");
    sprintf (message, "[initial] reps = %.6f", 1.);
    ut_print_progress_nonl (stdout, 100, 100, message, prevmessage);
    printf ("\n");
  }

  else
  {
  */
  if (iter == 1)
  {
    ut_print_message (0, verbositylevel, "Initial solution: f   =%.9f\n", 1.);
  }
  else
  {
    if (iter == 2)
    {
      strcpy (prevmessage, " ");
      ut_print_message (0, verbositylevel, prevmessage);
    }

    sprintf (message, "Iteration %6d: fmin=%.9f, f=%.9f", iter, OOpt.fmin, OOpt.f);
    ut_print_progress (stdout, verbositylevel, INT_MAX, message, prevmessage);
    strcpy (prevmessage, message);
  }

  /*
  }
  */

  fflush (stdout);

  return;
}

int
net_ori_uniform_opt_stop (struct OOPT *pOOpt, int iter)
{
  int res, stop, varqty = 2;
  char **vars = NULL;
  double *vals = NULL;

  vars = ut_alloc_1d_pchar (varqty);
  vals = ut_alloc_1d (varqty);

  ut_string_string ("iter", vars);
  ut_string_string ("reps", vars + 1);

  vals[0] = iter;
  vals[1] = (*pOOpt).f;

  res = ut_math_eval_int ((*pOOpt).orioptistop, varqty, vars, vals, &stop);
  if (res == -1)
    ut_print_message (2, 2, "Could not process expression `%s'.\n",
                      (*pOOpt).orioptistop);

  if ((*pOOpt).n == 1)
    stop = 1;

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return stop;
}

void
net_ori_uniform_log (struct IN_T In, int iter, struct OL_SET OSet, struct OOPT OOpt)
{
  if (strcmp (OOpt.logvar, "none"))
    net_ori_uniform_log_var (In, iter, OSet, OOpt);

  return;
}
