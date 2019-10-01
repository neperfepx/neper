/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_uniform_.h"
using namespace std;

void
net_ori_uniform_opt_forces_ser (struct OL_SET *pOSet, double **f, double *E)
{
  unsigned int i, j, k;

#pragma omp parallel for
  for (i = 0; i < (*pOSet).size; i++)
    E[i] = 0;

#pragma omp parallel for schedule(dynamic) private (j,k)
  for (i = 0; i < (*pOSet).size; i++)
    for (j = i + 1; j < (*pOSet).size; j++)
    {
      double Eij, *fij = ut_alloc_1d (3);
      net_ori_uniform_opt_forces_comp (pOSet, i, j, fij, &Eij);

      Eij *= 0.5;
#pragma omp atomic
      E[i] += Eij;
#pragma omp atomic
      E[j] += Eij;

      for (k = 0; k < 3; k++)
#pragma omp atomic
        f[i][k] += fij[k];

      for (k = 0; k < 3; k++)
#pragma omp atomic
        f[j][k] -= fij[k];
      ut_free_1d (fij);
    }

  return;
}

void
net_ori_uniform_opt_forces_ser_neigh (struct OL_SET *pOSet,
                              double **f, double *E, struct OOPT *pOOpt,
                              struct QCLOUD *pqcloud, my_kd_tree_t *qindex)
{
  unsigned int i, j, k, neighqty;

#pragma omp parallel for
  for (i = 0; i < (*pOSet).size; i++)
    E[i] = 0;

  (*pOOpt).avneighqty = 0;

#pragma omp parallel for private (j,k,neighqty)
  for (i = 0; i < (*pOSet).size; i++)
  {
    int *neighs = ut_alloc_1d_int ((*pqcloud).pts.size ());
    double Eij, *fij = ut_alloc_1d (3);

    nanoflann::SearchParams params;
    //params.sorted = false;

    std::vector < std::pair < size_t, double > >ret_matches;
    neighqty = qindex->radiusSearch ((*pOSet).q[i], (*pOOpt).neighdE, ret_matches, params);
    for (j = 0; j < neighqty; j++)
      neighs[j] = ret_matches[j].first;

    (*pOOpt).avneighqty += neighqty - 1;

    for (j = 1; j < neighqty; j++)
    {
      if (ret_matches[j].second > 3.999999)
        continue;

      net_ori_uniform_opt_forces_comp_neigh (pOSet, i, neighs[j], fij, &Eij, pqcloud);

      Eij *= 0.5;
      E[i] += Eij;

      for (k = 0; k < 3; k++)
        f[i][k] += fij[k];
    }

    ut_free_1d_int (neighs);
    ut_free_1d (fij);
  }

  (*pOOpt).avneighqty /= (*pOSet).size;

  return;
}

void
net_ori_uniform_opt_rot_ori (double **f, double alpha, int i,
                     struct OL_SET *pOSet, double **dq)
{
  double *r = ol_r_alloc ();
  double *q = ol_q_alloc ();

  neut_ori_f_qdot (f[i], q);
  ut_array_1d_scale (q, 4, alpha);

  ut_array_1d_memcpy (dq[i], 3, q + 1);

  ol_q_q_q_ref ((*pOSet).q[i], q, q);

  ut_array_1d_add ((*pOSet).q[i], q, 4, (*pOSet).q[i]);

  ut_array_1d_normalize ((*pOSet).q[i], 4);

  ol_q_q ((*pOSet).q[i], (*pOSet).q[i]);

  ol_r_free (r);
  ol_q_free (q);

  return;
}

void
net_ori_uniform_log_var (struct IN_T In, int iter, struct OL_SET OSet, struct OOPT OOpt)
{
  int i, j, k;
  int varqty;
  char **vars = NULL;
  char *filename = NULL;
  double *q = ol_q_alloc ();
  double **g = ol_g_alloc ();
  double *R = ol_R_alloc ();
  double *tmp = ol_e_alloc ();
  double theta, *r = ol_r_alloc ();

  /*
  if (!ut_string_iter_test (OOpt.logvar, NEUT_SEP_NODEP, OOpt.iter))
    return;
  */

  filename = ut_string_addextension (In.body, ".logorivar");
  OOpt.logvar_fp = ut_file_open (filename, iter == 0 ? "W" : "A");

  ut_string_separate (OOpt.logvar, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 0; i < (int) OSet.size; i++)
  {
    ol_q_qcrysym (OSet.q[i], OSet.crysym, q);

    for (j = 0; j < varqty; j++)
    {
      if (!strncmp (vars[j], "iter", 4))
	fprintf (OOpt.logvar_fp, "%d", iter);
      else if (!strcmp (vars[j], "id"))
	fprintf (OOpt.logvar_fp, "%d", i + 1);
      else if (!strcmp (vars[j], "g"))
      {
        ol_q_g (q, g);
        for (k = 0; k < 3; k++)
        {
          ut_array_1d_fprintf_nonl (OOpt.logvar_fp, g[k], 3, "%.12f");
          if (k < 2)
            fprintf (OOpt.logvar_fp, " ");
        }
      }
      else if (!strcmp (vars[j], "rtheta"))
      {
        ol_q_rtheta (q, r, &theta);
	ut_array_1d_fprintf_nonl (OOpt.logvar_fp, r, 3, "%.12f");
        fprintf (OOpt.logvar_fp, " %.12f", theta);
      }
      else if (!strcmp (vars[j], "R"))
      {
        ol_q_R (q, R);
	ut_array_1d_fprintf_nonl (OOpt.logvar_fp, R, 3, "%.12f");
      }
      else if (!strcmp (vars[j], "q"))
	ut_array_1d_fprintf_nonl (OOpt.logvar_fp, OSet.q[i], 4, "%.12f");
      else if (!strcmp (vars[j], "e"))
      {
        ol_q_e (q, tmp);
	ut_array_1d_fprintf_nonl (OOpt.logvar_fp, tmp, 3, "%.12f");
      }
      else if (!strcmp (vars[j], "ek"))
      {
        ol_q_e (q, tmp);
        ol_e_ek (tmp, tmp);
	ut_array_1d_fprintf_nonl (OOpt.logvar_fp, tmp, 3, "%.12f");
      }
      else if (!strcmp (vars[j], "er"))
      {
        ol_q_e (q, tmp);
        ol_e_er (tmp, tmp);
	ut_array_1d_fprintf_nonl (OOpt.logvar_fp, tmp, 3, "%.12f");
      }
      else
	fprintf (OOpt.logvar_fp, "-1");

      if (j < varqty - 1)
	fprintf (OOpt.logvar_fp, " ");
    }
    fprintf (OOpt.logvar_fp, "\n");
  }

  ut_file_close (OOpt.logvar_fp, filename, iter == 0 ? "W" : "A");

  ut_free_2d_char (vars, varqty);
  ut_free_1d_char (filename);
  ol_q_free (q);
  ol_R_free (R);
  ol_g_free (g);
  ol_r_free (r);
  ol_e_free (tmp);

  return;
}
