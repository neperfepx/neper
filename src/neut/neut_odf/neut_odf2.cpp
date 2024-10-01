/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_qcloud_struct.hpp"

extern void neut_odf_comp_elts_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id);
extern void neut_odf_comp_elts_neigh (double *q, double cut_fact, QCLOUD qcloud, my_kd_tree_t *nano_index, struct ODF *pOdf, int id);
extern void neut_odf_comp_nodes_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id);
extern void neut_odf_comp_nodes_neigh (double *q, double cut_fact, QCLOUD qcloud, my_kd_tree_t *nano_index,
                           struct ODF *pOdf, int id);
extern void neut_odf_orifield_comp_elts_all (struct OL_SET *pOSet, double *oridata, double *q,
                                 int id, struct ODF *pOdf);
extern void neut_odf_orifield_comp_nodes_all (struct OL_SET *pOSet, double *oridata, double *q,
                                  int id, struct ODF *pOdf);

void
neut_odf_init_eltweight (struct ODF *pOdf)
{
  int i, dim = neut_mesh_array_dim ((*pOdf).Mesh);
  double *coo = ut_alloc_1d (3);
  double *q = ol_q_alloc ();

  if (!(*pOdf).EltWeight)
    (*pOdf).EltWeight = ut_alloc_1d ((*pOdf).odfqty);

  for (i = 0; i < (*pOdf).Mesh[dim].EltQty; i++)
  {
    if (dim != 3)
      abort ();

    neut_mesh_elt_volume_orispace ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, (*pOdf).gridtype, (*pOdf).EltWeight + i);
    neut_mesh_elt_centre ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, coo);
  }

  // we keep the weights as is, so that they represent the orientation volumes
  // ut_array_1d_scale ((*pOdf).EltWeight, (*pOdf).odfqty, 1. / ut_array_1d_mean ((*pOdf).EltWeight, (*pOdf).odfqty));

  ut_free_1d (&coo);
  ol_q_free (q);

  return;
}

void
neut_odf_elt_ori (struct ODF *pOdf, int elt, double *q)
{
  int dim = neut_mesh_array_dim ((*pOdf).Mesh);
  double *coo = ut_alloc_1d (3);

  neut_mesh_elt_centre ((*pOdf).Nodes, (*pOdf).Mesh[dim], elt, coo);

  if (!strncmp ((*pOdf).gridtype, "rodrigues", 9))
    ol_R_q (coo, q);
  else if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
  {
    if (!strcmp ((*pOdf).gridunit, "degree"))
      ol_e_q (coo, q);
    else
      ol_e_q_rad (coo, q);
  }
  else if (!strcmp ((*pOdf).gridtype, "homochoric"))
    ol_homochoric_q (&((*pOdf).hfct), coo, q);
  else
    ut_print_message (2, 2,
                      "Unkown grid type `%s' (option -odfgridtype).\n",
                      (*pOdf).gridtype);

  ut_free_1d (&coo);

  return;
}

double
neut_odf_comp_elts (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud,
                    my_kd_tree_t *nano_index, struct ODF *pOdf, double *pfact, int verbosity)
{
  int i, dim = neut_mesh_array_dim ((*pOdf).Mesh);
  double avradeq;
  char *prev = ut_alloc_1d_char (100);

  neut_ori_n_avradeq (NULL, (*pOSet).size, (*pOSet).crysym, &avradeq);

  ut_array_1d_zero ((*pOdf).odf, (*pOdf).odfqty);

  neut_odf_init_eltweight (pOdf);

#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pOdf).Mesh[dim].EltQty; i++)
  {
    double *q = ol_q_alloc ();
    double *coo = ut_alloc_1d (3);

    neut_odf_elt_ori (pOdf, i + 1, q);

#pragma omp critical
    if (verbosity > 0)
      ut_print_progress (stdout, i, (*pOdf).Mesh[dim].EltQty - 1, "%3.0f%%", prev);

    if (!strcmp (neigh, "all"))
      neut_odf_comp_elts_all (pOSet, q, pOdf, i);
    else
    {
      double cut_fact = atof (neigh);
      neut_odf_comp_elts_neigh (q, cut_fact, nano_cloud, nano_index, pOdf, i);
    }

    ol_q_free (q);
    ut_free_1d (&coo);
  }

  (*pOdf).odfmean = ut_array_1d_wmean ((*pOdf).odf, (*pOdf).EltWeight, (*pOdf).odfqty);
  ut_array_1d_scale ((*pOdf).odf, (*pOdf).odfqty, 1. / (*pOdf).odfmean);
  (*pfact) = 1. / (*pOdf).odfmean;
  (*pOdf).odfmean = 1;
  (*pOdf).odfsig = ut_array_1d_wstddev ((*pOdf).odf, (*pOdf).EltWeight, 1, (*pOdf).odfqty);
  (*pOdf).odfmin = ut_array_1d_min ((*pOdf).odf, (*pOdf).odfqty);
  (*pOdf).odfmax = ut_array_1d_max ((*pOdf).odf, (*pOdf).odfqty);

  ut_free_1d_char (&prev);

  return 1;
}

void
neut_odf_comp_nodes (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud,
                     my_kd_tree_t *nano_index, struct ODF *pOdf, double fact, int verbosity)
{
  int i;
  double mean;
  char *prev = ut_alloc_1d_char (1000);

  ut_array_1d_zero ((*pOdf).odfn, (*pOdf).odfnqty);

  mean = 0;
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pOdf).odfnqty; i++)
  {
    double *q = ol_q_alloc ();
    double *qs = ol_q_alloc ();

#pragma omp critical
    if (verbosity > 0)
      ut_print_progress (stdout, i, (*pOdf).odfnqty - 1, "%3.0f%%", prev);

    if (!strncmp ((*pOdf).gridtype, "rodrigues", 9))
      ol_R_q ((*pOdf).Nodes.NodeCoo[i + 1], q);
    else if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
    {
      if (!strcmp ((*pOdf).gridunit, "degree"))
        ol_e_q ((*pOdf).Nodes.NodeCoo[i + 1], q);
      else
        ol_e_q_rad ((*pOdf).Nodes.NodeCoo[i + 1], q);
    }
    else if (strcmp ((*pOdf).gridtype, "homochoric"))
      ol_homochoric_q (&((*pOdf).hfct), (*pOdf).Nodes.NodeCoo[i + 1], q);
    else
      ut_print_message (2, 2,
                        "Unkown grid type `%s' (option -odfgridtype).\n",
                        (*pOdf).gridtype);

    if (!strcmp (neigh, "all"))
      neut_odf_comp_nodes_all (pOSet, q, pOdf, i);
    else
    {
      double cut_fact;
      sscanf (neigh, "%lf", &cut_fact);
      neut_odf_comp_nodes_neigh (q, cut_fact, nano_cloud, nano_index, pOdf, i);
    }

#pragma omp atomic
    mean += (*pOdf).odfn[i];

    ol_q_free (q);
    ol_q_free (qs);
  }

  mean /= (*pOdf).odfnqty;
#pragma omp parallel for
  for (i = 0; i < (*pOdf).odfnqty; i++)
    (*pOdf).odfn[i] *= fact;

  ut_free_1d_char (&prev);

  return;
}

double
neut_odf_orifield_comp_elts (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud,
                             my_kd_tree_t *nano_index, double *oridata, struct ODF *pOdf)
{
  int i, dim = neut_mesh_array_dim ((*pOdf).Mesh);
  double *vol = ut_alloc_1d ((*pOdf).odfqty);
  double avradeq;
  neut_ori_n_avradeq (NULL, (*pOSet).size, (*pOSet).crysym, &avradeq);

  ut_array_1d_zero ((*pOdf).odf, (*pOdf).odfqty);

#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pOdf).Mesh[dim].EltQty; i++)
  {
    double *q = ol_q_alloc ();
    double *coo = ut_alloc_1d (3);

    if (dim == 3)
      neut_mesh_elt_volume ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, vol + i);
    else
      neut_mesh_elt_area ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, vol + i);
    neut_mesh_elt_centre ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, coo);

    if (!strncmp ((*pOdf).gridtype, "rodrigues", 9))
    {
      vol[i] *= pow (M_PI * (1 + ut_vector_scalprod (coo, coo)), -2);
      ol_R_q (coo, q);
    }
    else if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
    {
      if (!strcmp ((*pOdf).gridunit, "degree"))
      {
        vol[i] *= sin (coo[1] * M_PI / 180);
        ol_e_q (coo, q);
      }
      else
      {
        vol[i] *= sin (coo[1]);
        ol_e_q_rad (coo, q);
      }
    }
    else if (!strcmp ((*pOdf).gridtype, "homochoric"))
      ol_homochoric_q (&((*pOdf).hfct), coo, q);
    else
      ut_print_message (2, 2,
                        "Unkown grid type `%s' (option -odfgridtype).\n",
                        (*pOdf).gridtype);

    if (1 || !strcmp (neigh, "all"))
      neut_odf_orifield_comp_elts_all (pOSet, oridata, q, i, pOdf);
    else
    {
      double cut_fact = atof (neigh);
      neut_odf_comp_elts_neigh (q, cut_fact, nano_cloud, nano_index, pOdf, i);
    }

    ol_q_free (q);
    ut_free_1d (&coo);
  }

  (*pOdf).odfmean = ut_array_1d_mean ((*pOdf).odf, (*pOdf).odfqty);
  (*pOdf).odfsig = ut_array_1d_wstddev ((*pOdf).odf, vol, 1, (*pOdf).odfqty);
  (*pOdf).odfmin = ut_array_1d_min ((*pOdf).odf, (*pOdf).odfqty);
  (*pOdf).odfmax = ut_array_1d_max ((*pOdf).odf, (*pOdf).odfqty);

  ut_free_1d (&vol);

  return 1;
}

void
neut_odf_orifield_comp_nodes (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud,
                     my_kd_tree_t *nano_index, double *oridata, struct ODF *pOdf)
{
  int i;
  double mean;

  ut_array_1d_zero ((*pOdf).odfn, (*pOdf).odfnqty);

  mean = 0;
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pOdf).odfnqty; i++)
  {
    double *q = ol_q_alloc ();
    double *qs = ol_q_alloc ();

    if (!strncmp ((*pOdf).gridtype, "rodrigues", 9))
      ol_R_q ((*pOdf).Nodes.NodeCoo[i + 1], q);
    else if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
    {
      if (!strcmp ((*pOdf).gridunit, "degree"))
        ol_e_q ((*pOdf).Nodes.NodeCoo[i + 1], q);
      else
        ol_e_q_rad ((*pOdf).Nodes.NodeCoo[i + 1], q);
    }
    else if (strcmp ((*pOdf).gridtype, "homochoric"))
      ol_homochoric_q (&((*pOdf).hfct), (*pOdf).Nodes.NodeCoo[i + 1], q);
    else
      ut_print_message (2, 2,
                        "Unkown grid type `%s' (option -odfgridtype).\n",
                        (*pOdf).gridtype);

    if (1 || !strcmp (neigh, "all"))
      neut_odf_orifield_comp_nodes_all (pOSet, oridata, q, i, pOdf);
    else
    {
      double cut_fact;
      sscanf (neigh, "%lf", &cut_fact);
      neut_odf_comp_nodes_neigh (q, cut_fact, nano_cloud, nano_index, pOdf, i);
    }

#pragma omp atomic
    mean += (*pOdf).odfn[i];

    ol_q_free (q);
    ol_q_free (qs);
  }

  mean /= (*pOdf).odfnqty;

  return;
}
