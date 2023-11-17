/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_general_domain (struct IN_T In, struct TESS PTess, int cell,
                                  struct TOPT *pTOpt)
{
  neut_tess_poly_tess (PTess, cell, &((*pTOpt).Dom0));
  neut_tess_poly_tess (PTess, cell, &((*pTOpt).Dom));

  if (ut_array_1d_int_sum (In.periodic, 3))
    net_tess_perdomain (In, PTess, cell, &((*pTOpt).DomPer));

  if (PTess.Level == 0)
  {
    ut_string_string (PTess.DomType, &(*pTOpt).DomType);
    char *fct = NULL;
    int i, varqty;
    char **vars = NULL;
    char **vals = NULL;

    ut_string_function (In.domain, &fct, &vars, &vals, &varqty);

    (*pTOpt).DomParms = ut_alloc_1d (varqty);
    for (i = 0; i < varqty; i++)
      sscanf (vals[i], "%lf", (*pTOpt).DomParms + i);

    ut_free_1d_char (&fct);
    ut_free_2d_char (&vars, varqty);
    ut_free_2d_char (&vals, varqty);
  }

  if (!ut_array_1d_int_sum (In.periodic, 3))
    net_tess_poly ((*pTOpt).Dom, 1, &(*pTOpt).DomPoly);
  else
    net_tess_poly ((*pTOpt).DomPer, 1, &(*pTOpt).DomPoly);

  // if -transform cut, we load the primitives

  int i, partqty;
  char **parts = NULL;

  ut_list_break (In.transform, NEUT_SEP_NODEP, &parts, &partqty);

  for (i = 0; i < partqty; i++)
    if (!strncmp (parts[i], "cut", 3))
      net_transform_tess_cut_pre_prim (parts[i], &(*pTOpt).Prim, &(*pTOpt).PrimQty, 0);

  ut_free_2d_char (&parts, partqty);

  return;
}

void
net_tess_opt_init_general_cellqty (struct IN_T In, struct MTESS MTess,
                                   struct TESS *pTess, int poly, int *pCellQty)
{
  int varqty;
  char **vars = NULL;
  double *vals = NULL;
  char *tmp2 = ut_alloc_1d_char (100);
  char *expr = In.n[(*pTess).Level + 1];

  neut_tess_expr_vars_vals (pTess, expr, NULL, NULL, NULL, (char *) "cell",
                            poly, &vars, &vals, NULL, &varqty);

  neut_mtess_tess_poly_mid (MTess, *pTess, poly, &tmp2);
  if (strcmp (expr, "from_morpho"))
  {
    net_multiscale_arg_0d_int_fscanf (expr, tmp2, varqty, vars, vals,
                                      pCellQty);
    (*pCellQty) = ut_num_max (*pCellQty, 1);
  }
  else
    (*pCellQty) = -1;

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);
  ut_free_1d_char (&tmp2);

  return;
}

void
net_tess_opt_init_parms_algo (struct IN_T In, int level, struct MTESS MTess,
                              struct TESS *Tess, int dtess, int dcell,
                              struct TOPT *pTOpt)
{
  char *optialgo = NULL;

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.optialgo[neut_topt_typepos (*pTOpt)][level],
                                           &optialgo);
  ut_list_break (optialgo, NEUT_SEP_NODEP, &(*pTOpt).algoname,
                 &(*pTOpt).algoqty);

#ifdef HAVE_NLOPT
  int i;
  (*pTOpt).algo =
    (nlopt_algorithm *) calloc ((*pTOpt).algoqty, sizeof (nlopt_algorithm));

  for (i = 0; i < (*pTOpt).algoqty; i++)
  {
    if (!strcmp ((*pTOpt).algoname[i], "neldermead"))
      (*pTOpt).algo[i] = NLOPT_LN_NELDERMEAD;
    else if (!strcmp ((*pTOpt).algoname[i], "subplex"))
      (*pTOpt).algo[i] = NLOPT_LN_SBPLX;
    else if (!strcmp ((*pTOpt).algoname[i], "cobyla"))
      (*pTOpt).algo[i] = NLOPT_LN_COBYLA;
    else if (!strcmp ((*pTOpt).algoname[i], "bobyqa"))
      (*pTOpt).algo[i] = NLOPT_LN_BOBYQA;
    else if (!strcmp ((*pTOpt).algoname[i], "newuoa"))
      (*pTOpt).algo[i] = NLOPT_LN_NEWUOA_BOUND;
    else if (!strcmp ((*pTOpt).algoname[i], "praxis"))
      (*pTOpt).algo[i] = NLOPT_LN_PRAXIS;
    else if (!strncmp ((*pTOpt).algoname[i], "random", 5))
      (*pTOpt).algo[i] = NLOPT_LN_PRAXIS;
    else if (!strncmp ((*pTOpt).algoname[i], "lloyd", 5))
      (*pTOpt).algo[i] = NLOPT_LN_PRAXIS;
    else
      abort ();
  }
#endif // HAVE_NLOPT

  return;
}

void
net_tess_opt_init_parms_algomaxiter (struct IN_T In, int level,
                                     struct TOPT *pTOpt)
{
  int i, qty, varqty = 1;
  char **vars = ut_alloc_1d_pchar (varqty);
  double *vals = ut_alloc_1d (varqty);

  (*pTOpt).algomaxiter = ut_alloc_1d_int ((*pTOpt).algoqty);

  ut_list_break ((*pTOpt).dof, NEUT_SEP_NODEP, NULL, &qty);
  qty *= (*pTOpt).seedoptiqty;

  ut_string_string ("varnb", vars);
  vals[0] = qty;

  for (i = 0; i < (*pTOpt).algoqty; i++)
    ut_math_eval_int (In.optialgomaxiter[level], varqty, vars, vals,
                      (*pTOpt).algomaxiter + i);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

void
net_tess_opt_init_parms_objective (char *optiobjective,
                                   struct TOPT *pTOpt)
{
  int i, j, PartQty1, *PartQty2 = NULL;
  char ***parts = NULL;

  ut_string_fnrs (optiobjective, "circularity", "sphericity", INT_MAX);

  ut_list_break2 (optiobjective, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts,
                  &PartQty2, &PartQty1);

  for (i = 0; i < PartQty1; i++)
    if (PartQty2[i] != 2)
      if (strcmp (parts[i][0], "default"))
        ut_print_message (2, 4, "Failed to process expression `%s'.\n",
                          parts[i][0]);

  (*pTOpt).tarobjective = ut_alloc_1d_pchar ((*pTOpt).tarqty);

  int *found = ut_alloc_1d_int (PartQty1);

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    ut_string_string ("default", (*pTOpt).tarobjective + i);

    for (j = 0; j < PartQty1; j++)
      if (!strcmp (parts[j][0], (*pTOpt).tarvar[i]))
      {
        found[j] = 1;
        ut_string_string (parts[j][1], (*pTOpt).tarobjective + i);
      }

    if (!strcmp ((*pTOpt).tarobjective[i], "default"))
      if (!strcmp ((*pTOpt).tarvar[i], "tesr"))
        ut_string_string ("pts(region=surf,res=5)+val(bounddist)",
                          (*pTOpt).tarobjective + i);
  }

  for (i = 0; i < PartQty1; i++)
    if (!found[i] && strcmp (parts[i][0], "default"))
      ut_print_message (1, 4, "Skipped objective function specified for `%s'.\n",
                        parts[i][0]);

  ut_string_string ("L2", &(*pTOpt).objective);
  for (j = 0; j < PartQty1; j++)
    if (!strncmp (parts[j][0], "gen", 3))
      ut_string_string (parts[j][1], &(*pTOpt).objective);

  // ut_free_3d_char (&parts, PartQty1, 2);
  ut_free_1d_int (&PartQty2);
  ut_free_1d_int (&found);

  return;
}

void
net_tess_opt_init_target_bin (double xmin, double xmax, double mean,
                              int binqty, double *binx)
{
  int i, id;
  double min, binwidth = (xmax - xmin) / binqty;
  double *tmp = ut_alloc_1d (binqty);

  for (i = 0; i < binqty; i++)
    binx[i] = xmin + (i + .5) * binwidth;

  ut_array_1d_memcpy (binx, binqty, tmp);
  ut_array_1d_addval (tmp, binqty, -mean, tmp);
  id = ut_array_1d_absmin_index (tmp, binqty);
  min = binx[id] - mean;

  ut_array_1d_addval (binx, binqty, -min, binx);

  ut_free_1d (&tmp);

  return;
}

void
net_tess_opt_init_ref (struct TOPT *pTOpt, double mean, int id)
{
  int i, j, print = 0;
  double fact = 1;

  print = ((*pTOpt).CellQty == -1);

  if (!strcmp ((*pTOpt).tarvar[id], "size"))
  {
    neut_tess_cellavsize ((*pTOpt).Dom0, (*pTOpt).CellQty,
                          &((*pTOpt).tarrefval[id]));

    if ((*pTOpt).CellQty == -1)
    {
      neut_tess_cellavsize_cellqty ((*pTOpt).Dom0, mean, &(*pTOpt).CellQty,
                                    NULL);
      (*pTOpt).CellQty = ut_num_max ((*pTOpt).CellQty, 1);
      neut_tess_cellavsize ((*pTOpt).Dom0, (*pTOpt).CellQty,
                            &((*pTOpt).tarrefval[id]));
    }
  }

  else if (!strcmp ((*pTOpt).tarvar[id], "diameq")
           || !strcmp ((*pTOpt).tarvar[id], "centroid")
           || !strcmp ((*pTOpt).tarvar[id], "centroidtol")
           || !strcmp ((*pTOpt).tarvar[id], "centroidsize")
           || !strcmp ((*pTOpt).tarvar[id], "centroiddiameq")
           || !strcmp ((*pTOpt).tarvar[id], "tesr"))
  {
    if (!strcmp ((*pTOpt).tartype[id], "stat"))
    {
      fact = 0;
      if (strcmp ((*pTOpt).tarpdf0[id].type, "numerical"))
        abort ();

      for (i = 0; i < (*pTOpt).tarpdf0[id].size; i++)
        fact +=
          (*pTOpt).tarpdf0[id].y[i] * ut_fct_binwidth ((*pTOpt).tarpdf0[id],
                                                       i) *
          pow ((*pTOpt).tarpdf0[id].x[i], (*pTOpt).Dim);
      fact = pow (fact, 1. / (*pTOpt).Dim);
    }
    else
      fact = 1;

    if ((*pTOpt).CellQty != -1)
    {
      neut_tess_cellavdiameq ((*pTOpt).Dom0, (*pTOpt).CellQty,
                              &((*pTOpt).tarrefval[id]));
      (*pTOpt).tarrefval[id] /= fact;

      if (!strcmp ((*pTOpt).tarvar[id], "centroidtol"))
      {
        int qty = 0;
        double norm = 0;
        for (j = 1; j <= (*pTOpt).CellQty; j++)
          if ((*pTOpt).tarcellval[id][j][(*pTOpt).tarcellvalqty[id] - 1] <
              1000)
          {
            qty++;
            norm +=
              pow ((*pTOpt).tarcellval[id][j][(*pTOpt).tarcellvalqty[id] - 1],
                   2);
          }
        norm = sqrt (norm / qty);
        (*pTOpt).tarrefval[id] /= norm;
      }
    }

    else
    {
      if (strcmp ((*pTOpt).tarvar[id], "tesr"))
      {
        neut_tess_cellavdiameq_cellqty ((*pTOpt).Dom0, mean * fact,
                                        &(*pTOpt).CellQty, NULL);
        (*pTOpt).CellQty = ut_num_max ((*pTOpt).CellQty, 1);
        neut_tess_cellavdiameq ((*pTOpt).Dom0, (*pTOpt).CellQty,
                                &((*pTOpt).tarrefval[id]));
        (*pTOpt).tarrefval[id] /= fact;
      }
      else
      {
        (*pTOpt).CellQty = (*pTOpt).tartesr.CellQty;
        neut_tess_cellavdiameq ((*pTOpt).Dom0, (*pTOpt).CellQty,
                                &((*pTOpt).tarrefval[id]));
      }
    }
  }

  else
    (*pTOpt).tarrefval[id] = 1;

  if (print)
    ut_print_message (0, 4, "Number of cells: %d\n", (*pTOpt).CellQty);

  return;
}

void
net_tess_opt_init_target_grid (struct IN_T In, int level, struct MTESS MTess,
                               struct TESS *Tess, int dtess, int dcell,
                               int var, struct TOPT *pTOpt)
{
  int i, status;
  char ***parts = NULL;
  int qty, *qty2 = NULL;
  char *string = NULL;

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.optigrid[level], &string);

  ut_string_fnrs (string, "circularity", "sphericity", INT_MAX);

  ut_list_break2 (string, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty2, &qty);

  status = -1;
  for (i = 0; i < qty; i++)
  {
    if (!strcmp (parts[i][0], (*pTOpt).tarvar[var]))
    {
      ut_string_string (parts[i][1], (*pTOpt).grid + var);
      status = 0;
    }
  }

  if (status == -1)
    ut_print_message (2, 3, "Variable `%s' not found in `-morphooptigrid'.\n",
                      (*pTOpt).tarvar[var]);

  ut_free_1d_char (&string);

  return;
}

void
net_tess_opt_init_target_cvl (struct IN_T In, int level, struct MTESS MTess,
                              struct TESS *Tess, int dtess, int dcell,
                              int var, struct TOPT *pTOpt)
{
  int i;
  double status, min, sum = 0;
  char ***parts = NULL;
  int qty, *qty2 = NULL;
  char *string = NULL;

  ut_string_string ("numerical", &(*pTOpt).cvlmethod);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.optismooth[level],
                                           &string);

  ut_string_fnrs (string, "circularity", "sphericity", INT_MAX);

  ut_list_break2 (string, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty2, &qty);

  status = -1;
  for (i = 0; i < qty; i++)
  {
    if (!strcmp (parts[i][0], "analytical")
        || !strcmp (parts[i][0], "numerical"))
    {
      ut_string_string (parts[i][0], &(*pTOpt).cvlmethod);
      break;
    }

    if (!strcmp (parts[i][0], (*pTOpt).tarvar[var]))
      sscanf (parts[i][1], "%lf", (*pTOpt).cvlsig + var);
    status = 0;
  }

  if (status == -1)
    ut_print_message (2, 3,
                      "Variable `%s' not found in `-morphooptismooth'.\n",
                      (*pTOpt).tarvar[var]);

  if ((*pTOpt).cvlsig[var] > 0)
  {
    ut_fct_set_normal ((*pTOpt).cvl + var, 0, (*pTOpt).cvlsig[var], 0, 0, 0, 0);        // not sure about this
    ut_fct_numericalfct ((*pTOpt).cvl[var], -3 * (*pTOpt).cvlsig[var],
                         3 * (*pTOpt).cvlsig[var], 100, (*pTOpt).cvl + var);

    min = ut_array_1d_min ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size);
    ut_array_1d_addval ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size, -min,
                        (*pTOpt).cvl[var].y);

    for (i = 0; i < (*pTOpt).cvl[var].size; i++)
      sum += ut_fct_binwidth ((*pTOpt).cvl[var], i) * (*pTOpt).cvl[var].y[i];
    ut_array_1d_scale ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size, 1. / sum);
  }

  ut_free_1d_char (&string);

  return;
}

void
net_tess_opt_init_target_scale (struct TOPT *pTOpt, int *pos)
{
  int i;
  double domvol, sumvol, tmp, fact;

  if (!strcmp ((*pTOpt).tarvar[pos[0]], "centroidsize")
      || !strcmp ((*pTOpt).tarvar[pos[0]], "centroiddiameq"))
  {
    neut_tess_size ((*pTOpt).Dom0, &domvol);

    sumvol = 0;
    for (i = 1; i <= (*pTOpt).CellQty; i++)
    {
      ut_space_diameq_size ((*pTOpt).Dim,
                            (*pTOpt).tarcellval[pos[0]][i][pos[1]], &tmp);
      sumvol += tmp;
    }

    fact = pow (domvol / sumvol, 1. / (*pTOpt).Dim);
    for (i = 1; i <= (*pTOpt).CellQty; i++)
      (*pTOpt).tarcellval[pos[0]][i][pos[1]] *= fact;
    if (!ut_num_requal (fact, 1, 1e-3))
      ut_print_message (1, 4, "Cell diameqs scaled by a factor of %f.\n",
                        fact);
  }

  return;
}

void
net_tess_opt_init_bounds_seeds (struct TOPT *pTOpt)
{
  int i, j, k, seed, dim, PartQty;
  char **parts = NULL;

  ut_list_break ((*pTOpt).dof, NEUT_SEP_NODEP, &parts, &PartQty);

  (*pTOpt).boundl = ut_alloc_1d (PartQty * (*pTOpt).seedoptiqty + 1);
  (*pTOpt).boundu = ut_alloc_1d (PartQty * (*pTOpt).seedoptiqty + 1);

  k = 0;
  for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  {
    seed = (*pTOpt).seedopti[j];
    for (i = 0; i < PartQty; i++)
    {
      if (!strcmp (parts[i], "x") || !strcmp (parts[i], "y")
          || !strcmp (parts[i], "z"))
      {
        dim = parts[i][0] - 'x';
        (*pTOpt).boundl[k] =
          ((*pTOpt).SSet).SeedCoo0[seed][dim] - (*pTOpt).dist;
        (*pTOpt).boundu[k] =
          ((*pTOpt).SSet).SeedCoo0[seed][dim] + (*pTOpt).dist;
        k++;
      }
      else if (!strcmp (parts[i], "w"))
      {
        (*pTOpt).boundl[k] = ((*pTOpt).SSet).SeedWeight[seed] - (*pTOpt).dist;
        (*pTOpt).boundu[k] = ((*pTOpt).SSet).SeedWeight[seed] + (*pTOpt).dist;
        (*pTOpt).boundl[k] = ut_num_max (0, (*pTOpt).boundl[k]);
        k++;
      }
      else
        abort ();
    }
  }

  ut_free_2d_char (&parts, PartQty);

  return;
}

void
net_tess_opt_init_bounds_ori (struct TOPT *pTOpt)
{
  int i, j, k, seed, dim, PartQty;
  char **parts = NULL;

  ut_list_break ((*pTOpt).dof, NEUT_SEP_NODEP, &parts, &PartQty);

  (*pTOpt).boundl = ut_alloc_1d (PartQty * (*pTOpt).seedoptiqty + 1);
  (*pTOpt).boundu = ut_alloc_1d (PartQty * (*pTOpt).seedoptiqty + 1);

  k = 0;
  for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  {
    seed = (*pTOpt).seedopti[j];
    for (i = 0; i < PartQty; i++)
    {
      if (!strcmp (parts[i], "r1") || !strcmp (parts[i], "r2")
          || !strcmp (parts[i], "r3"))
      {
        dim = parts[i][1] - '1';
        (*pTOpt).boundl[k] =
          ((*pTOpt).SSet).SeedOriR[seed][dim] - (*pTOpt).dist;
        (*pTOpt).boundu[k] =
          ((*pTOpt).SSet).SeedOriR[seed][dim] + (*pTOpt).dist;
        k++;
      }
      else if (!strcmp (parts[i], "rw"))
      {
        (*pTOpt).boundl[k] = 0;
        (*pTOpt).boundu[k] =
          ((*pTOpt).SSet).SeedOriWeight[seed] + (*pTOpt).dist;
        k++;
      }
      else if (!strcmp (parts[i], "rt"))
      {
        (*pTOpt).boundl[k] = 0;
        (*pTOpt).boundu[k] =
          ((*pTOpt).SSet).SeedOriTheta[seed] + (*pTOpt).dist;
        k++;
      }
      else
        abort ();
    }
  }

  ut_free_2d_char (&parts, PartQty);

  return;
}

// FIXME/EMMC
void
net_tess_opt_init_bounds_crystal (struct TOPT *pTOpt)
{
  int i, PartQty;
  char **parts = NULL;

  ut_list_break ((*pTOpt).dof, NEUT_SEP_NODEP, &parts, &PartQty);

  (*pTOpt).boundl = ut_alloc_1d (PartQty);
  (*pTOpt).boundu = ut_alloc_1d (PartQty);

  for (i = 0; i < PartQty; i++)
  {
    (*pTOpt).boundl[i] =
      ut_num_max ((*pTOpt).Crys.C[i] - (*pTOpt).dist, 1e-6);
    (*pTOpt).boundu[i] = (*pTOpt).Crys.C[i] + (*pTOpt).dist;
  }

  ut_free_2d_char (&parts, PartQty);

  return;
}
