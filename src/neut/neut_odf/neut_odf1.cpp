/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"

extern double neut_odf_comp_elts (char *neigh, struct OL_SET *pOSet, my_kd_tree_t *nano_index, struct ODF *pOdf);
extern double neut_odf_comp_nodes (char *neigh, struct OL_SET *pOSet, my_kd_tree_t *nano_index, struct ODF *pOdf);

void
neut_odf_set_zero (struct ODF *pOdf)
{
  int i;

  (*pOdf).gridtype = NULL;

  neut_nodes_set_zero (&((*pOdf).Nodes));
  (*pOdf).Mesh = (struct MESH*) calloc (4, sizeof (struct MESH));
  for (i = 0; i < 4; i++)
    neut_mesh_set_zero ((*pOdf).Mesh + i);

  (*pOdf).odfqty = 0;
  (*pOdf).odf = NULL;
  (*pOdf).odfnqty = 0;
  (*pOdf).odfn = NULL;

  (*pOdf).odfmin = 0;
  (*pOdf).odfmax = 0;
  (*pOdf).odfmean = 0;
  (*pOdf).odfsig = 0;

  ut_fct_set_zero (&((*pOdf).hfct));
  ol_homochoric_thetafct (&((*pOdf).hfct));

  return;
}

void
neut_odf_sigma (char *expr, struct OL_SET *pOSet, struct ODF *pOdf)
{
  int status, varqty = 1;
  double *vals = ut_alloc_1d (varqty);
  char **vars = ut_alloc_1d_pchar (varqty);

  ut_string_string ("avthetaeq", vars);
  neut_n_avthetaeq (NULL, (*pOSet).size, (*pOSet).crysym, vals);
  ol_theta_rad2deg (vals[0], vals);

  status = ut_math_eval (expr, varqty, vars, vals, &((*pOdf).sigma));
  if (status == -1)
    ut_print_message (2, 2, "Failed to parse expression `%s'.\n", expr);

  ol_theta_deg2rad ((*pOdf).sigma, &((*pOdf).sigma));

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
neut_odf_comp (char *mode, char *neigh, struct OL_SET *pOSet, struct ODF *pOdf)
{
  my_kd_tree_t *nano_index = nullptr;
  nanoflann::SearchParams params;
  nanoflann_cloud nano_cloud;

  neut_oset_kdtree (pOSet, &nano_cloud, &nano_index);

  if (strstr (mode, "m") || strstr (mode, "n"))
    neut_odf_comp_elts (neigh, pOSet, nano_index, pOdf);

  if (strstr (mode, "n"))
    neut_odf_comp_nodes (neigh, pOSet, nano_index, pOdf);

  delete nano_index;

  return;
}
