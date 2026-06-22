/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"

extern void neut_odf_comp_exact (struct OL_SET *pOSet, char *entity, struct ODF *pOdf, int verbosity);
extern void neut_odf_comp_neigh (struct OL_SET *pOSet, char *entity, char *neigh, struct ODF *pOdf, int verbosity);
extern void neut_odf_orifield_comp_exact (struct OL_SET *pOSet, char *entity, double *oridata, struct ODF *pOdf);

void
neut_odf_set_zero (struct ODF *pOdf)
{
  (*pOdf).sigma = 0;

  (*pOdf).odfqty = 0;
  (*pOdf).odf = NULL;
  (*pOdf).odfnqty = 0;
  (*pOdf).odfn = NULL;

  (*pOdf).odfmin = 0;
  (*pOdf).odfmax = 0;
  (*pOdf).odfmean = 0;
  (*pOdf).odfsig = 0;

  neut_ospace_set_zero (&((*pOdf).Sp));

  return;
}

void
neut_odf_setsigma (struct ODF *pOdf, char *expr, int qty, char *crysym)
{
  int status, varqty = 1;
  double *vals = ut_alloc_1d (varqty);
  char **vars = ut_alloc_1d_pchar (varqty);

  ut_string_string ("avthetaeq", vars);
  neut_ori_n_avthetaeq (NULL, qty, crysym, vals);
  ol_theta_rad2deg (vals[0], vals);

  status = ut_math_eval (expr, varqty, vars, vals, &((*pOdf).sigma));
  if (status == -1)
    ut_print_message (2, 2, "Failed to parse expression `%s'.\n", expr);

  ol_theta_deg2rad ((*pOdf).sigma, &((*pOdf).sigma));

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

void
neut_odf_comp (char *entity, char *neigh, struct OL_SET *pOSet, struct ODF *pOdf, int verbosity)
{
  struct OL_SET OSet2;

  ol_set_zero (&OSet2);

  if (!strncmp (entity, "elt", 3))
  {
    (*pOdf).odfqty = (*pOdf).Sp.Mesh[3].EltQty;
    if (!(*pOdf).odf)
      (*pOdf).odf = ut_alloc_1d ((*pOdf).odfqty);
  }

  if (!strncmp (entity, "node", 4))
  {
    (*pOdf).odfnqty = (*pOdf).Sp.Nodes.NodeQty;
    if (!(*pOdf).odfn)
      (*pOdf).odfn = ut_alloc_1d ((*pOdf).odfnqty);
  }

  ol_set_expandset (*pOSet, &OSet2);

  for (int i = 0; i < (int) (*pOSet).size; i++)
    if ((*pOSet).weight && isnan ((*pOSet).weight[i]))
      abort ();

  for (int i = 0; i < (int) OSet2.size; i++)
    if (OSet2.weight && isnan (OSet2.weight[i]))
      abort ();

  if (!strcmp (neigh, "all"))
    neut_odf_comp_exact (&OSet2, entity, pOdf, verbosity);
  else
    neut_odf_comp_neigh (&OSet2, entity, (char *) "5", pOdf, verbosity);

  ol_set_free (&OSet2);

  return;
}

void
neut_odf_orides (struct ODF Odf, char **porides)
{
  ut_string_function (Odf.Sp.space, porides, NULL, NULL, NULL);

  return;
}

void
neut_odf_convolve (struct ODF *pOdf, char *kernel)
{
  int i;
  double theta;
  struct OL_SET OSet;
  double *odf_cpy = ut_alloc_1d ((*pOdf).odfqty);

  ol_set_zero (&OSet);
  sscanf (kernel, "normal(%lf)", &theta);

  neut_ospace_mesh_olset (&((*pOdf).Sp), &OSet);

  for (i = 0; i < (*pOdf).odfqty; i++)
    OSet.weight[i] *= (*pOdf).odf[i];

  ut_array_1d_scale (OSet.weight, OSet.size, 1. / ut_array_1d_mean (OSet.weight, OSet.size));

  /*
  for (i = 0; i < (*pOdf).odfqty; i++)
    if (OSet.weight[i] < 1e-2)
      OSet.weight[i] = 0;
  ol_set_clean (&OSet);
  */

  if (theta > 0)
  {
    (*pOdf).sigma = theta * M_PI / 180;
    neut_odf_comp ((char *) "elt", (char *) "5", &OSet, pOdf, 1);
  }

  else
  {
    ut_array_1d_memcpy ((*pOdf).odf, (*pOdf).odfqty, odf_cpy);

    (*pOdf).sigma = theta * M_PI / 180;
    neut_odf_comp ((char *) "elt", (char *) "5", &OSet, pOdf, 1);

    for (i = 0; i < (*pOdf).odfqty; i++)
      (*pOdf).odf[i] = ut_num_max (2 * odf_cpy[i] - (*pOdf).odf[i], 0);
  }

  // setting (potential) negative values to zero
  for (i = 0; i < (*pOdf).odfqty; i++)
    (*pOdf).odf[i] = ut_num_max ((*pOdf).odf[i], 0);

  neut_odf_normalize (pOdf, NULL);

  ol_set_free (&OSet);
  ut_free_1d (&odf_cpy);

  return;
}

int
neut_odf_normalize (struct ODF *pOdf, double *pfact)
{
  int status;

  status = 0;
  if (pfact)
    *pfact = 1;

  if (!(*pOdf).Sp.Mesh[3].EltWeight)
    neut_mesh_init_eltweight ((*pOdf).Sp.Nodes, (*pOdf).Sp.Mesh + 3);

  (*pOdf).odfmean = ut_array_1d_wmean ((*pOdf).odf, (*pOdf).Sp.Mesh[3].EltWeight + 1, (*pOdf).odfqty);

  if (!ut_num_equal ((*pOdf).odfmean, 1, 1e-6))
  {
    ut_array_1d_scale ((*pOdf).odf, (*pOdf).odfqty, 1. / (*pOdf).odfmean);
    if (pfact)
      (*pfact) = 1. / (*pOdf).odfmean;
    (*pOdf).odfmean = 1.;
    status = 1;
  }

  return status;
}

int
neut_odf_index (struct ODF *pOdf, double *pindex)
{
  int status;
  double sumw;

  status = 0;

  if (!(*pOdf).Sp.Mesh[3].EltWeight)
    neut_mesh_init_eltweight ((*pOdf).Sp.Nodes, (*pOdf).Sp.Mesh + 3);

  *pindex = 0;
  sumw = 0;
  for (int i = 0; i < (*pOdf).odfqty; i++)
  {
    (*pindex) += (*pOdf).Sp.Mesh[3].EltWeight[i + 1] * pow ((*pOdf).odf[i] - 1, 2);
    sumw += (*pOdf).Sp.Mesh[3].EltWeight[i + 1];
  }
  (*pindex) /= sumw;
  (*pindex) = sqrt (*pindex);

  return status;
}

int
neut_odfn_index (struct ODF *pOdf, double *pindex)
{
  int status;
  double sumw;

  status = 0;

  if (!(*pOdf).Sp.Nodes.NodeWeight)
    neut_nodes_init_nodeweight (&(*pOdf).Sp.Nodes, (*pOdf).Sp.Mesh + 3);

  *pindex = 0;
  sumw = 0;
  for (int i = 0; i < (*pOdf).odfnqty; i++)
  {
    (*pindex) += (*pOdf).Sp.Nodes.NodeWeight[i + 1] * pow ((*pOdf).odfn[i] - 1, 2);
    sumw += (*pOdf).Sp.Nodes.NodeWeight[i + 1];
  }
  (*pindex) /= sumw;
  (*pindex) = sqrt (*pindex);

  return status;
}

void
neut_odf_deconvolve (struct ODF *pOdf, char *kernel)
{
  double theta;
  char *newkernel = ut_alloc_1d_char (100);

  sscanf (kernel, "normal(%lf)", &theta);
  theta *= -1;
  sprintf (newkernel, "normal(%lf)", theta);

  neut_odf_convolve (pOdf, newkernel);

  ut_free_1d_char (&newkernel);

  return;
}

void
neut_odf_elt_ori (struct ODF Odf, int elt, gsl_rng *r, double *q)
{
  double *coo = ut_alloc_1d (3);

  if (!strncmp (Odf.Sp.space, "rodrigues", 9))
  {
    if (r)
      neut_mesh_elt_randompt (Odf.Sp.Nodes, Odf.Sp.Mesh[3], elt + 1, r, coo);
    else
      neut_mesh_elt_centre (Odf.Sp.Nodes, Odf.Sp.Mesh[3], elt + 1, coo);
    ol_R_q (coo, q);
  }
  else
  {
    printf ("Odf.Sp.space = %s\n", Odf.Sp.space);
    abort ();
  }

  ut_free_1d (&coo);

  return;
}

void
neut_odf_orifield_comp (char *entity, char *neigh, struct OL_SET *pOSet,
                        double *oridata, struct ODF *pOdf)
{
  (void) neigh;

  neut_odf_orifield_comp_exact (pOSet, entity, oridata, pOdf);

  /*
  (*pOdf).odfmean = ut_array_1d_mean ((*pOdf).odf, (*pOdf).odfqty);
  (*pOdf).odfsig = ut_array_1d_wstddev ((*pOdf).odf, vol, 1, (*pOdf).odfqty);
  (*pOdf).odfmin = ut_array_1d_min ((*pOdf).odf, (*pOdf).odfqty);
  (*pOdf).odfmax = ut_array_1d_max ((*pOdf).odf, (*pOdf).odfqty);
  */

  return;
}

void
neut_odf_crysym (struct ODF Odf, char **pcrysym)
{
  int varqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (Odf.Sp.space, &fct, &vars, &vals, &varqty);
  if (varqty > 0)
    ut_string_string (vals[0], pcrysym);

  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&fct);

  return;
}

void
neut_odf_free (struct ODF *pOdf)
{
  int i;

  ut_free_1d_char (&(*pOdf).Sp.space);
  ut_free_1d_char (&(*pOdf).Sp.spaceunit);
  neut_tess_free (&(*pOdf).Sp.Tess);
  neut_nodes_free (&(*pOdf).Sp.Nodes);
  for (i = 0; i <= 3; i++)
    neut_mesh_free ((*pOdf).Sp.Mesh + i);

  ut_free_1d (&((*pOdf).odf));
  ut_free_1d (&((*pOdf).odfn));

  ut_fct_free (&((*pOdf).Sp.hfct));
  ut_fct_free (&((*pOdf).Sp.thetafct));

  return;
}
