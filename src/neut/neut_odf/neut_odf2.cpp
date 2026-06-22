/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_struct_qcloud.hpp"

void
neut_odf_elt_ori (struct ODF *pOdf, int elt, double *q)
{
  int dim = neut_mesh_array_dim ((*pOdf).Sp.Mesh);
  double *coo = ut_alloc_1d (3);

  neut_mesh_elt_centre ((*pOdf).Sp.Nodes, (*pOdf).Sp.Mesh[dim], elt, coo);

  if (!strcmp ((*pOdf).Sp.space, "rodrigues"))
    ol_R_q (coo, q);
  else if (!strcmp ((*pOdf).Sp.space, "euler-bunge"))
  {
    if (!strcmp ((*pOdf).Sp.spaceunit, "degree"))
      ol_e_q (coo, q);
    else
      ol_e_q_rad (coo, q);
  }
  else if (!strcmp ((*pOdf).Sp.space, "homochoric"))
    ol_homochoric_q (&((*pOdf).Sp.hfct), coo, q);
  else
    ut_print_message (2, 2,
                      "Unkown space `%s' (option -odfspace).\n",
                      (*pOdf).Sp.space);

  ut_free_1d (&coo);

  return;
}

void
neut_odf_comp_exact (struct OL_SET *pOSet, char *entity, struct ODF *pOdf, int verbosity)
{
  int i, j, k, n = ol_crysym_qty ((*pOSet).crysym);
  double fact, *qs = ol_q_alloc ();
  double *oriodf = NULL;
  double sigma_theta, sigma_theta_2;
  char *prev = ut_alloc_1d_char (100);

  int odfqty;
  double **podf = NULL;
  double ***pori = NULL;
  double **pweight = NULL;

  if (!strncmp (entity, "elt", 3))
  {
    odfqty = (*pOdf).odfqty;
    podf = &((*pOdf).odf);
    pori = &((*pOdf).Sp.Mesh[3].EltOri);
    pweight = &((*pOdf).Sp.Mesh[3].EltWeight);
  }
  else if (!strncmp (entity, "node", 4))
  {
    odfqty = (*pOdf).odfnqty;
    podf = &((*pOdf).odfn);
    pori = &((*pOdf).Sp.Nodes.NodeOri);
    pweight = &((*pOdf).Sp.Nodes.NodeWeight);
  }

  if (!(*pori))
    abort ();

  if (!(*podf))
    (*podf) = ut_alloc_1d (odfqty);

  ut_array_1d_zero (*podf, odfqty);

  oriodf = ut_alloc_1d (odfqty);

  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    if (verbosity > 0)
      ut_print_progress (stdout, i, (int) (*pOSet).size, "%3.0f%%", prev);

    ut_array_1d_zero (oriodf, odfqty);

    sigma_theta_2 = pow ((*pOdf).sigma, 2) + pow ((*pOSet).theta ? (*pOSet).theta[i] : 0, 2);
    sigma_theta = sqrt (sigma_theta_2);

    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);

#pragma omp parallel for schedule(dynamic)
      for (k = 0; k < odfqty; k++)
      {
        double theta;

        ol_q_q_misori_rad ((*pori)[k + 1], qs, &theta);
        oriodf[k] += exp (-theta * theta / (2 * sigma_theta_2));
      }
    }

    fact = 1 / ut_fct_eval ((*pOdf).Sp.thetafct, sigma_theta) / ol_crysym_qty ((*pOSet).crysym);
    ut_array_1d_scale (oriodf, odfqty, fact);

    for (k = 0; k < odfqty; k++)
      (*podf)[k] += (*pOSet).weight[i] * oriodf[k];
  }

  // ut_array_1d_scale (*podf, odfqty, 1 / ut_array_1d_sum ((*pOSet).weight, (*pOSet).size));
  ut_array_1d_scale (*podf, odfqty, 1. / ut_array_1d_wmean (*podf, *pweight, odfqty));

  (*pOdf).odfmean = 1;

  ut_free_1d (&oriodf);
  ol_q_free (qs);
  ut_free_1d_char (&prev);

  return;
}

void
neut_odf_comp_neigh (struct OL_SET *pOSet, char *entity, char *neigh, struct ODF *pOdf, int verbosity)
{
  int i;
  char *prev = ut_alloc_1d_char (100);
  double cut_fact = atof (neigh);

  int odfqty;
  double **podf = NULL;
  double **pweight = NULL;
  char *data = NULL;

  if (neut_sys_data (&data, 0))
    abort ();

  if (!strncmp (entity, "elt", 3))
  {
    if (!(*pOdf).Sp.Mesh[3].EltWeight)
      neut_mesh_init_eltweight ((*pOdf).Sp.Nodes, (*pOdf).Sp.Mesh + 3);

    odfqty = (*pOdf).odfqty;
    podf = &((*pOdf).odf);
    pweight = &((*pOdf).Sp.Mesh[3].EltWeight);
  }
  else if (!strncmp (entity, "node", 4))
  {
    if (!(*pOdf).Sp.Nodes.NodeWeight)
      neut_nodes_init_nodeweight (&(*pOdf).Sp.Nodes, (*pOdf).Sp.Mesh + 3);

    odfqty = (*pOdf).odfnqty;
    podf = &((*pOdf).odfn);
    pweight = &((*pOdf).Sp.Nodes.NodeWeight);
  }
  else
    abort ();

  QCLOUD qcloud;
  QCLOUD *pqcloud = &qcloud;
  my_kd_tree_t *qtree = NULL;
  my_kd_tree_t **pqtree = &qtree;
  nanoflann::SearchParams params;

  if ((*pOdf).Sp.pqcloud && (*pOdf).Sp.pqtree)
  {
    if (!strncmp (entity, "elt", 3))
    {
      pqcloud = (QCLOUD *) (*pOdf).Sp.pqcloud;
      pqtree = (my_kd_tree_t **) (*pOdf).Sp.pqtree;
    }
    else if (!strncmp (entity, "node", 4))
    {
      pqcloud = (QCLOUD *) (*pOdf).Sp.pqcloud;
      pqtree = (my_kd_tree_t **) (*pOdf).Sp.pqtree;
    }
    else
      abort ();
  }
  else
  {
    struct OL_SET Grid;
    if (!strncmp (entity, "elt", 3))
      neut_ospace_mesh_olset (&((*pOdf).Sp), &Grid);
    else
      neut_ospace_mesh_olset_allnodes (&((*pOdf).Sp), &Grid);

    neut_oset_kdtree_cloud (&Grid, pqcloud);

    char *filename = ut_alloc_1d_char (1000);
    if (!strncmp (entity, "elt", 3))
      sprintf (filename, "%s/orispace/%s/%s.etree", data, (*pOdf).Sp.crysym, (*pOdf).Sp.fr);
    else
      sprintf (filename, "%s/orispace/%s/%s.ntree", data, (*pOdf).Sp.crysym, (*pOdf).Sp.fr);

    if (ut_file_exist (filename))
    {
      FILE *fp = ut_file_open (filename, "RB");
      (*pqtree) = new my_kd_tree_t (4, *pqcloud);
      // KDTreeSingleIndexAdaptorParams (10 /* max leaf */ ));
      (*pqtree)->loadIndex (fp);
      ut_file_close (fp, filename, "RB");
    }
    else
      neut_oset_kdtree_build (pqcloud, pqtree);
    ol_set_free (&Grid);
  }

  if (!(*podf))
    (*podf) = ut_alloc_1d (odfqty);

  ut_array_1d_zero (*podf, odfqty);

#pragma omp parallel for schedule(dynamic) private (i)
  for (i = 0; i < (int) (*pOSet).size; i++)
    if (!(*pOSet).weight || (*pOSet).weight[i] > 0)
    {
      if (verbosity > 0)
        ut_print_progress (stdout, i, (int) (*pOSet).size, "%3.0f%%", prev);

      int elt, j, k, neighqty;
      double theta, *thetamis_2 = ut_alloc_1d (3), *theta3 = ut_alloc_1d (3), cut_dist, fact;
      double *sigma_theta3 = ut_alloc_1d (3), *sigma_theta3_2 = ut_alloc_1d (3);
      double *q = ol_q_alloc ();
      double *r = ol_r_alloc ();

      if ((*pOSet).theta3 && (*pOSet).theta3[i])
        ut_array_1d_memcpy ((*pOSet).theta3[i], 3, theta3);
      else if ((*pOSet).theta && (*pOSet).theta[i] >= 0)
        ut_array_1d_set (theta3, 3, (*pOSet).theta[i]);
      else
        ut_array_1d_zero (theta3, 3);

      for (j = 0; j < 3; j++)
        sigma_theta3_2[j] = pow ((*pOdf).sigma, 2) + pow (theta3[j], 2);

      for (j = 0; j < 3; j++)
        sigma_theta3[j] = sqrt (sigma_theta3_2[j]);

      double max_sigma_theta3 = ut_array_1d_max (sigma_theta3, 3);

      double *oriodf = ut_alloc_1d (odfqty);

      if (max_sigma_theta3 > 1e3)
        ut_array_1d_set (oriodf, odfqty, 1);

      else
      {
        cut_dist = (cut_fact * max_sigma_theta3 < 2 * M_PI) ? pow (2 * sin (0.25 * cut_fact * max_sigma_theta3), 2) : 4;
        if (cut_dist == 0)
          cut_dist = 0.1;

        nanoflann::SearchParams params;
        std::vector < std::pair < size_t, double > >ret_matches;
        neighqty = (*pqtree)->radiusSearch ((*pOSet).q[i], cut_dist, ret_matches, params);

        ut_array_1d_zero (oriodf, odfqty);
        // smooth distribution
        if (max_sigma_theta3 > 0)
          for (j = 0; j < neighqty; j++)
          {
            elt = ret_matches[j].first % odfqty;
            theta = 4 * asin (sqrt (ret_matches[j].second) * 0.5);
            ol_q_q_qmisori_ref ((*pOSet).q[i], (*pqcloud).pts[ret_matches[j].first].q, q);
            ol_q_r (q, r);
            for (k = 0; k < 3; k++)
              thetamis_2[k] = theta * theta * r[k] * r[k];

#pragma omp atomic
            oriodf[elt] += exp (- 0.5 * (thetamis_2[0] / sigma_theta3_2[0]
                                       + thetamis_2[1] / sigma_theta3_2[1]
                                       + thetamis_2[2] / sigma_theta3_2[2]));
            // oriodf[elt] += exp (- 0.5 * (theta / sigma_theta3_2[0]));
            // oriodf[elt] += exp (- theta * theta / (2 * sigma_theta3_2));
          }
        // dirac distribution
        else
        {
          elt = ret_matches[0].first % odfqty;
          oriodf[elt] = 1;
        }

        /*
        // normalizing with 1F1 and the fraction that we missed due to the cutoff
        fact = ut_fct_eval ((*pOdf).Sp.thetafct, sigma_theta) / ol_crysym_qty ((*pOSet).crysym)
             * gsl_sf_erf (cut_fact / sqrt (2));
        */
        fact = ut_array_1d_wmean (oriodf, *pweight + 1, odfqty);

        ut_array_1d_scale (oriodf, odfqty, 1 / fact);
      }

      for (j = 0; j < odfqty; j++)
        if (isnan (oriodf[j]))
          abort ();

#pragma omp critical
      for (j = 0; j < odfqty; j++)
        (*podf)[j] += (*pOSet).weight[i] * oriodf[j];

      ut_free_1d (&oriodf);
      ut_free_1d (&sigma_theta3);
      ut_free_1d (&sigma_theta3_2);
      ut_free_1d (&theta3);
      ut_free_1d (&thetamis_2);
      ol_q_free (q);
      ol_r_free (r);
    }

  ut_array_1d_scale (*podf, odfqty, 1. / ut_array_1d_wmean (*podf, *pweight + 1, odfqty));

  for (i = 0; i < odfqty; i++)
    if (isnan ((*podf)[i]))
      abort ();

  (*pOdf).odfmean = 1;

  if (verbosity)
    printf ("\n");

  ut_free_1d_char (&prev);
  ut_free_1d_char (&data);

  return;
}
