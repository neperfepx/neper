/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_target_cellqty (struct IN_T In, struct MTESS MTess,
				  struct TESS Tess, int poly, int *pCellQty)
{
  int varqty;
  char **vars = NULL;
  double *vals = NULL;
  char *tmp2 = ut_alloc_1d_char (100);
  double cellavsize;

  varqty = 2;
  vars = ut_alloc_2d_char (varqty, 10);
  vals = ut_alloc_1d (varqty);

  neut_tess_cellavsize (Tess, Tess.CellQty, &cellavsize);
  strcpy (vars[0], "size");
  neut_tess_cell_size (Tess, poly, vals);
  strcpy (vars[1], "sizerel");
  vals[1] = vals[0] / cellavsize;

  neut_mtess_tess_poly_mid (MTess, Tess, poly, &tmp2);
  if (strcmp (In.n[Tess.Level + 1], "from_morpho"))
  {
    net_multiscale_arg_0d_int_fscanf (In.n[Tess.Level + 1],
				      tmp2, varqty, vars, vals, pCellQty);
    (*pCellQty) = ut_num_max (*pCellQty, 1);
  }
  else
    (*pCellQty) = -1;

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (tmp2);

  return;
}

#ifdef HAVE_NLOPT
void
net_tess_opt_init_parms_algo (char *optialgo, struct TOPT *pTOpt)
{
  ut_string_string (optialgo, &(*pTOpt).algoname);

  if (!strcmp (optialgo, "neldermead"))
    (*pTOpt).algo = NLOPT_LN_NELDERMEAD;
  else if (!strcmp (optialgo, "subplex"))
    (*pTOpt).algo = NLOPT_LN_SBPLX;
  else if (!strcmp (optialgo, "cobyla"))
    (*pTOpt).algo = NLOPT_LN_COBYLA;
  else if (!strcmp (optialgo, "bobyqa"))
    (*pTOpt).algo = NLOPT_LN_BOBYQA;
  else if (!strcmp (optialgo, "newuoa"))
    (*pTOpt).algo = NLOPT_LN_NEWUOA_BOUND;
  else if (!strcmp (optialgo, "praxis"))
    (*pTOpt).algo = NLOPT_LN_PRAXIS;
  else if (!strcmp (optialgo, "random"))
    (*pTOpt).algo = NLOPT_LN_PRAXIS;
  else if (!strncmp (optialgo, "lloyd", 5))
    (*pTOpt).algo = NLOPT_LN_PRAXIS;
  else
    abort ();

  return;
}
#endif

void
net_tess_opt_init_parms_objective (char *morphooptiobjective,
				   struct TOPT *pTOpt)
{
  int i, qty;
  char **list = NULL;

  ut_string_separate (morphooptiobjective, NEUT_SEP_DEP, &list, &qty);
  for (i = 0; i < qty; i++)
    ut_string_string (list[i], &(*pTOpt).objective);

  ut_free_2d_char (list, qty);

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strcmp ((*pTOpt).tarvar[i], "tesr"))
    {
      if (!strcmp ((*pTOpt).objective, "default"))
	ut_string_string ("scaled,rasterscaled,surf", &(*pTOpt).objective);
    }
    if (!strcmp ((*pTOpt).tarvar[i], "centroid"))
      ut_string_fnrs ((*pTOpt).objective, "default", "L2", 1);
  }

  return;
}

void
net_tess_opt_init_target_bin (double xmin, double xmax,
			      double mean, int binqty, double *binx)
{
  int i, id;
  double min, binwidth = (xmax - xmin) / binqty;
  double *tmp = ut_alloc_1d (binqty);

  for (i = 0; i < binqty; i++)
    binx[i] = xmin + (i + .5) * binwidth;

  ut_array_1d_memcpy (tmp, binqty, binx);
  ut_array_1d_addval (tmp, binqty, -mean, tmp);
  id = ut_array_1d_absmin_index (tmp, binqty);
  min = binx[id] - mean;

  ut_array_1d_addval (binx, binqty, -min, binx);

  ut_free_1d (tmp);

  return;
}

void
net_tess_opt_init_ref (struct TOPT *pTOpt, double mean, int id)
{
  int i, print = 0;
  double fact = 1, fact2;

  print = ((*pTOpt).CellQty == -1);

  if (!strcmp ((*pTOpt).tarvar[id], "size"))
  {
    neut_tess_cellavsize ((*pTOpt).Dom, (*pTOpt).CellQty,
			  &((*pTOpt).tarrefval[id]));

    if ((*pTOpt).CellQty == -1)
    {
      neut_tess_cellavsize_cellqty ((*pTOpt).Dom, mean, &(*pTOpt).CellQty, &fact2);
      (*pTOpt).tarrefval[id] = mean * fact2;
    }
  }

  else if (!strcmp ((*pTOpt).tarvar[id], "diameq")
	   || !strcmp ((*pTOpt).tarvar[id], "centroid")
	   || !strcmp ((*pTOpt).tarvar[id], "centroidsize")
	   || !strcmp ((*pTOpt).tarvar[id], "centroiddiameq")
	   || !strcmp ((*pTOpt).tarvar[id], "tesr"))
  {
    neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty,
			    &((*pTOpt).tarrefval[id]));

    if (!strcmp ((*pTOpt).tartype[id], "stat"))
    {
      fact = 0;
      if (strcmp ((*pTOpt).tarpdf0[id].type, "numerical"))
	abort ();

      for (i = 0; i < (*pTOpt).tarpdf0[id].size; i++)
	fact +=
	  (*pTOpt).tarpdf0[id].y[i]
	  * ut_fct_binwidth ((*pTOpt).tarpdf0[id], i)
	  * pow ((*pTOpt).tarpdf0[id].x[i], (*pTOpt).Dim);
      fact = pow (fact, 1. / (*pTOpt).Dim);
    }
    else
      fact = 1;

    (*pTOpt).tarrefval[id] /= fact;

    if ((*pTOpt).CellQty == -1)
    {
      neut_tess_cellavdiameq_cellqty ((*pTOpt).Dom, mean * fact, &(*pTOpt).CellQty, &fact2);
      (*pTOpt).tarrefval[id] = mean * fact2;
    }
  }

  else
    (*pTOpt).tarrefval[id] = 1;

  if (print)
  {
    ut_print_message (0, 4, "Number of cells: %d\n", (*pTOpt).CellQty);
    ut_print_message (0, 4, "Average grain size: %f\n", (*pTOpt).tarrefval[id]);
  }

  return;
}

void
net_tess_opt_init_tesrobj (struct TOPT *pTOpt)
{
  int i, j, count;
  double vsizemax;

  (*pTOpt).tarcellpts = ut_alloc_1d_ppdouble ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellptqty = ut_alloc_1d_int ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellrefval = ut_alloc_1d ((*pTOpt).tartesr.CellQty + 1);

  if (ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "scaled"))
  {
    (*pTOpt).tartesrscale = ut_alloc_1d ((*pTOpt).Dim);
    neut_tesr_cells_anisoxyz ((*pTOpt).tartesr, (*pTOpt).tartesrscale);
    for (i = 0; i < (*pTOpt).Dim; i++)
      (*pTOpt).tartesrscale[i] = 1. / (*pTOpt).tartesrscale[i];
    ut_print_message (0, 4, "Scaling input tesr by %.2f x %.2f x %.2f...\n",
		      (*pTOpt).tartesrscale[0],
		      (*pTOpt).tartesrscale[1],
		      (*pTOpt).tartesrscale[2]);
    neut_tesr_scale (&((*pTOpt).tartesr),
		     (*pTOpt).tartesrscale[0],
		     (*pTOpt).tartesrscale[1],
		     (*pTOpt).tartesrscale[2], NULL);
  }

  if (ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "rasterscaled"))
  {
    vsizemax = ut_array_1d_max ((*pTOpt).tartesr.vsize, 3);
    ut_print_message (0, 4, "Scaling input tesr's raster by %.2f x %.2f x %.2f...\n",
		      (*pTOpt).tartesr.vsize[0] / vsizemax,
		      (*pTOpt).tartesr.vsize[1] / vsizemax,
		      (*pTOpt).tartesr.vsize[2] / vsizemax);
    neut_tesr_rasterscale (&((*pTOpt).tartesr),
			   (*pTOpt).tartesr.vsize[0] / vsizemax,
			   (*pTOpt).tartesr.vsize[1] / vsizemax,
			   (*pTOpt).tartesr.vsize[2] / vsizemax);
  }

  net_tess_opt_init_tesrobj_pts (pTOpt);

  (*pTOpt).tarcellptqty[0] = ut_array_1d_int_sum ((*pTOpt).tarcellptqty + 1,
						  (*pTOpt).tartesr.CellQty);

  if (ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf")
   || ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf0")
   || ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf1")
   || ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf2"))
    ut_print_message (0, 4, "Number of voxels reduced by %.1f%% (to %d).\n",
		      100.0 - (*pTOpt).tarcellptqty[0] * 100.0 /
		      neut_tesr_voxqty ((*pTOpt).tartesr),
		      (*pTOpt).tarcellptqty[0]);

  count =
    ut_array_1d_int_nbofthisval ((*pTOpt).tarcellptqty + 1,
				 (*pTOpt).tartesr.CellQty, 0);
  if (count > 0)
    ut_print_message (1, 4, " %d %s no voxel!\n", count,
		      (count == 1) ? "cell has" : "cells have");

  // Setting tarcellptcells
  (*pTOpt).tarcellptscells = ut_alloc_1d_ppint ((*pTOpt).tartesr.CellQty + 1);
  for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
  {
    (*pTOpt).tarcellptscells[i]
      = ut_alloc_1d_pint ((*pTOpt).tarcellptqty[i] + 1);
    for (j = 0; j < (*pTOpt).tarcellptqty[i]; j++)
    {
      (*pTOpt).tarcellptscells[i][j]
	= ut_alloc_1d_int ((*pTOpt).CellSCellQty[i]);
      ut_array_1d_int_memcpy ((*pTOpt).tarcellptscells[i][j],
			      (*pTOpt).CellSCellQty[i],
			      (*pTOpt).CellSCellList[i]);
    }
  }

  return;
}

void
net_tess_opt_init_target_cvl (struct IN_T In, int level, int var,
			      struct TOPT *pTOpt)
{
  int i;
  double min, sum = 0;

  sscanf (In.morphooptismooth[level], "%lf", &((*pTOpt).dissmoothing));
  ut_fct_set_normal ((*pTOpt).cvl + var, 0, (*pTOpt).dissmoothing);
  ut_fct_numericalfct ((*pTOpt).cvl[var], -3 * (*pTOpt).dissmoothing,
		       3 * (*pTOpt).dissmoothing, 100, (*pTOpt).cvl + var);

  min = ut_array_1d_min ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size);
  ut_array_1d_addval ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size, -min,
		      (*pTOpt).cvl[var].y);

  for (i = 0; i < (*pTOpt).cvl[var].size; i++)
    sum += ut_fct_binwidth ((*pTOpt).cvl[var], i) * (*pTOpt).cvl[var].y[i];
  ut_array_1d_scale ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size, 1. / sum);

  return;
}
