/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_topt_.h"

void
neut_topt_set_zero (struct TOPT *pTOpt)
{
  (*pTOpt).Dim = 0;
  (*pTOpt).CellQty = 0;
  (*pTOpt).DomType = NULL;
  neut_tess_set_zero (&(*pTOpt).Dom0);
  neut_tess_set_zero (&(*pTOpt).Dom);
  neut_poly_set_zero (&(*pTOpt).DomPoly);
  neut_tess_set_zero (&(*pTOpt).DomPer);
  neut_seedset_set_zero (&(*pTOpt).SSet);
  neut_crys_set_zero (&(*pTOpt).Crys);
  (*pTOpt).Poly = NULL;
  (*pTOpt).CellSize = NULL;
  (*pTOpt).DomParms = NULL;

  (*pTOpt).aspratio = NULL;
  (*pTOpt).activedim = NULL;

  neut_tdyn_set_zero (&(*pTOpt).TDyn);

  (*pTOpt).optitype = NULL;
  (*pTOpt).xqty = 0;
  (*pTOpt).x_pvar = NULL;
  (*pTOpt).x_seed = NULL;
  (*pTOpt).x_var = NULL;

  (*pTOpt).dof = NULL;
  (*pTOpt).tarobjective = NULL;
  (*pTOpt).objective = NULL;
  (*pTOpt).objective_tesrval = NULL;

  (*pTOpt).grid = NULL;
  (*pTOpt).cvlsig = NULL;
  (*pTOpt).cvl = NULL;
  (*pTOpt).cvlmethod = NULL;

  (*pTOpt).curval = NULL;
  (*pTOpt).tarqty = 0;
  (*pTOpt).tarcellval = NULL;
  (*pTOpt).tarcellvalqty = NULL;

  (*pTOpt).tarvar = NULL;
  (*pTOpt).tartype = NULL;
  (*pTOpt).tarexpr = NULL;
  (*pTOpt).tarexprdisqty = NULL;
  (*pTOpt).tarexprdisprop = NULL;

  (*pTOpt).itermax = 0;
  (*pTOpt).nlopt_eps = 0;
  (*pTOpt).xeps = 0;
  (*pTOpt).nlopt_reps = 0;
  (*pTOpt).xreps = 0;
  (*pTOpt).time = 0;
  (*pTOpt).val = 0;
  (*pTOpt).dist = 0;
  (*pTOpt).eps = 0;
  (*pTOpt).reps = 0;
  (*pTOpt).loopmax = 0;

  (*pTOpt).boundl = NULL;
  (*pTOpt).boundu = NULL;

  (*pTOpt).algoname = NULL;
  (*pTOpt).algomaxiter = NULL;

  (*pTOpt).algoid = 0;

#ifdef HAVE_NLOPT
  (*pTOpt).algo = 0;
#endif

  (*pTOpt).inistep = 0;

  (*pTOpt).curcellpenalty = NULL;
  (*pTOpt).curcellval = NULL;
  (*pTOpt).oldcellpenalty = NULL;
  (*pTOpt).oldcellval = NULL;
  (*pTOpt).tarrefval = NULL;

  (*pTOpt).tarpdf0 = NULL;
  (*pTOpt).tarpdf = NULL;
  (*pTOpt).tarcdf0 = NULL;
  (*pTOpt).tarcdf = NULL;
  (*pTOpt).curpdf = NULL;
  (*pTOpt).curcdf = NULL;
  (*pTOpt).curcdf0 = NULL;
  (*pTOpt).curpenalty = NULL;

  (*pTOpt).tarmodeqty = 0;
  (*pTOpt).tarmodecdf0 = NULL;

  neut_tesr_set_zero (&(*pTOpt).tartesr);
  (*pTOpt).tavoxqtyini = 0;
  (*pTOpt).tartesrscale = NULL;
  (*pTOpt).tarcellpts = NULL;
  (*pTOpt).tarcellptweights = NULL;
  (*pTOpt).tarcellptqty = NULL;
  (*pTOpt).tarcellfact = NULL;
  (*pTOpt).tarcellptscells = NULL;

  (*pTOpt).seedopti = NULL;
  (*pTOpt).seedoptiqty = 0;
  (*pTOpt).scellchanged = NULL;
  (*pTOpt).scellchangedqty = 0;
  (*pTOpt).cellchanged = NULL;
  (*pTOpt).cellchangedqty = 0;

  (*pTOpt).CellSCellQty     = NULL;
  (*pTOpt).CellSCellList    = NULL;
  (*pTOpt).SCellCell        = NULL;

  (*pTOpt).objval = DBL_MAX;
  (*pTOpt).objvalmin = NULL;
  (*pTOpt).objval0 = DBL_MAX;
  (*pTOpt).objvalmin0 = NULL;
  (*pTOpt).iter = 0;
  (*pTOpt).loop = 0;
  (*pTOpt).loop_start = NULL;
  (*pTOpt).loop_plateau_length = NULL;
  (*pTOpt).loop_status = NULL;
  (*pTOpt).message = NULL;

  gettimeofday (&((*pTOpt).end_time), NULL);

  (*pTOpt).pnf_cloud = NULL;
  (*pTOpt).pnf_tree = NULL;

  (*pTOpt).ptid_seedid = NULL;
  (*pTOpt).seedid_ptid = NULL;

  return;
}

void
neut_topt_free (struct TOPT *pTOpt)
{
  int i, j;

  neut_tdyn_free (&(*pTOpt).TDyn);

  ut_free_1d ((*pTOpt).tartesrscale);
  ut_free_1d ((*pTOpt).tarcellfact);

  ut_free_1d ((*pTOpt).aspratio);
  ut_free_1d_int ((*pTOpt).activedim);

  if ((*pTOpt).tarcellpts)
  {
    for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
      ut_free_2d_double ((*pTOpt).tarcellpts[i], (*pTOpt).tarcellptqty[i]);
    ut_free_1d_ppdouble ((*pTOpt).tarcellpts);
  }

  if ((*pTOpt).tarcellptscells)
  {
    for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
      ut_free_2d_int ((*pTOpt).tarcellptscells[i], (*pTOpt).tarcellptqty[i]);
    ut_free_1d_ppint ((*pTOpt).tarcellptscells);
  }

  ut_free_2d ((*pTOpt).tarcellptweights, (*pTOpt).tartesr.CellQty + 1);
  ut_free_1d_int ((*pTOpt).tarcellptqty);

  neut_tesr_free (&(*pTOpt).tartesr);

  ut_free_1d_pdouble((*pTOpt).x_pvar);
  ut_free_1d_int((*pTOpt).x_seed);
  ut_free_1d_int((*pTOpt).x_var);

  ut_free_2d_char ((*pTOpt).algoname, (*pTOpt).algoqty);
  ut_free_1d_int  ((*pTOpt).algomaxiter);
  ut_free_1d_char ((*pTOpt).dof);

  if ((*pTOpt).tarcellval)
  {
    for (i = 0; i < (*pTOpt).tarqty; i++)
      if ((*pTOpt).tarcellval[i])
	ut_free_2d ((*pTOpt).tarcellval[i], (*pTOpt).CellQty + 1);
    free ((*pTOpt).tarcellval);
  }
  ut_free_1d_int ((*pTOpt).tarcellvalqty);

  ut_free_2d_char ((*pTOpt).tarvar, (*pTOpt).tarqty);
  ut_free_2d_char ((*pTOpt).tartype, (*pTOpt).tarqty);
  ut_free_2d_char ((*pTOpt).tarexpr, (*pTOpt).tarqty);
  ut_free_1d_int ((*pTOpt).tarexprdisqty);
  ut_free_2d ((*pTOpt).tarexprdisprop, (*pTOpt).tarqty);

  ut_free_1d ((*pTOpt).boundl);
  ut_free_1d ((*pTOpt).boundu);

  ut_free_1d ((*pTOpt).curcellpenalty);
  ut_free_3d ((*pTOpt).curcellval, (*pTOpt).tarqty, (*pTOpt).CellQty + 1);
  ut_free_1d ((*pTOpt).oldcellpenalty);
  ut_free_3d ((*pTOpt).oldcellval, (*pTOpt).tarqty, (*pTOpt).CellQty + 1);
  ut_free_1d ((*pTOpt).tarrefval);

  ut_free_1d ((*pTOpt).curpenalty);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).tarpdf0 + i);
  free ((*pTOpt).tarpdf0);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).tarpdf + i);
  free ((*pTOpt).tarpdf);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).tarcdf0 + i);
  free ((*pTOpt).tarcdf0);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).tarcdf + i);
  free ((*pTOpt).tarcdf);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).curpdf + i);
  free ((*pTOpt).curpdf);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).curcdf + i);
  free ((*pTOpt).curcdf);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).curcdf0 + i);
  free ((*pTOpt).curcdf0);

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    for (j = 0; j < (*pTOpt).tarmodeqty[i]; j++)
      ut_fct_free ((*pTOpt).tarmodecdf0[i] + j);
    free ((*pTOpt).tarmodecdf0[i]);
  }
  free ((*pTOpt).tarmodecdf0);

  ut_free_2d ((*pTOpt).tarmodefact, (*pTOpt).tarqty);

  ut_free_1d ((*pTOpt).curval);

  ut_free_1d_int ((*pTOpt).seedopti);
  ut_free_1d_int ((*pTOpt).scellchanged);
  ut_free_1d_int ((*pTOpt).cellchanged);
  ut_free_1d_int ((*pTOpt).CellSCellQty);
  ut_free_2d_int ((*pTOpt).CellSCellList, (*pTOpt).CellQty + 1);
  ut_free_1d_int ((*pTOpt).SCellCell);

  ut_free_1d_int ((*pTOpt).loop_start);
  ut_free_1d_int ((*pTOpt).loop_plateau_length);
  ut_free_1d_int ((*pTOpt).loop_status);

  ut_free_1d_char ((*pTOpt).message);
  ut_free_2d_char ((*pTOpt).tarobjective, (*pTOpt).tarqty);
  ut_free_1d_char ((*pTOpt).objective);
  ut_free_1d_char ((*pTOpt).objective_tesrval);

  neut_topt_set_zero (pTOpt);

  ut_free_1d_char ((*pTOpt).DomType);
  neut_tess_free (&(*pTOpt).Dom0);
  neut_tess_free (&(*pTOpt).Dom);
  neut_poly_free (&(*pTOpt).DomPoly);
  neut_tess_free (&(*pTOpt).DomPer);
  neut_poly_array_free (&(*pTOpt).Poly, (*pTOpt).SSet.N);
  ut_free_1d ((*pTOpt).CellSize);
  ut_free_1d ((*pTOpt).DomParms);
  neut_seedset_free (&(*pTOpt).SSet); // must be freed at the end (used to free others)
  // doing nothing to pTess (pointer only)

  ut_free_2d_char ((*pTOpt).grid, (*pTOpt).tarqty);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).cvl + i);
  free ((*pTOpt).cvl);

  ut_free_1d_char ((*pTOpt).cvlmethod);

  // nlopt_destroy ((*pTOpt).opt);

  return;
}

void
neut_topt_subcell_add (struct TOPT *pTOpt, int scell, int cell)
{
  (*pTOpt).SCellCell = ut_realloc_1d_int ((*pTOpt).SCellCell, scell + 1);
  (*pTOpt).SCellCell[scell] = cell;

  (*pTOpt).CellSCellQty[cell]++;
  (*pTOpt).CellSCellList[cell]
    = ut_realloc_1d_int ((*pTOpt).CellSCellList[cell], (*pTOpt).CellSCellQty[cell]);
  (*pTOpt).CellSCellList[cell][(*pTOpt).CellSCellQty[cell] - 1] = scell;

  return;
}

void
neut_topt_seed_subcell_add (struct TOPT *pTOpt, double *coo, double rad, int cell)
{
  neut_seedset_addseed (&(*pTOpt).SSet, coo, rad);
  neut_topt_subcell_add (pTOpt, (*pTOpt).SSet.N, cell);

  return;
}

int
neut_topt_eps (struct TOPT TOpt, double *peps)
{
  int status, status2, deltaiter, refiter;
  double alpha = 10;

  if (TOpt.iter - TOpt.loop_start[TOpt.loop - 1] <= TOpt.xqty)
  {
    (*peps) = -1;
    status = -1;
  }

  else
  {
    deltaiter = ut_num_d2ri (alpha * TOpt.xqty);

    status2 = neut_topt_refiter (TOpt, deltaiter, &refiter);

    if (status2)
    {
      deltaiter = TOpt.iter - 1;
      refiter = 1;
    }

    (*peps) =
      (TOpt.objvalmin[refiter] - TOpt.objvalmin[TOpt.iter])
      / deltaiter * TOpt.xqty;
    status = 0;
  }

  return status;
}

int
neut_topt_reps (struct TOPT TOpt, double *preps)
{
  int status;

  status = neut_topt_eps (TOpt, preps);

  if (!status)
    (*preps) /= TOpt.objvalmin[TOpt.iter];

  return status;
}

int
neut_topt_plateau (struct TOPT *pTOpt)
{
  int i, status;
  int maxiter = (*pTOpt).algomaxiter[(*pTOpt).algoid];

  // setting loop_plateau_length

  if ((*pTOpt).iter == 1)
      (*pTOpt).loop_plateau_length[(*pTOpt).loop - 1] = 0;
  else if ((*pTOpt).objvalmin[(*pTOpt).iter] < (*pTOpt).objvalmin[(*pTOpt).iter - 1])
      (*pTOpt).loop_plateau_length[(*pTOpt).loop - 1] = 0;
  else
      (*pTOpt).loop_plateau_length[(*pTOpt).loop - 1]++;

  // setting loop_status

  if ((*pTOpt).iter - (*pTOpt).loop_start[(*pTOpt).loop - 1] <= maxiter)
    (*pTOpt).loop_status[(*pTOpt).loop - 1] = 0;
  else
  {
    if ((*pTOpt).objvalmin[(*pTOpt).iter]
     == (*pTOpt).objvalmin[(*pTOpt).loop_start[(*pTOpt).loop - 1]])
      (*pTOpt).loop_status[(*pTOpt).loop - 1] = -2;
    else if ((*pTOpt).objvalmin[(*pTOpt).iter]
	  == (*pTOpt).objvalmin[(*pTOpt).iter - maxiter])
      (*pTOpt).loop_status[(*pTOpt).loop - 1] = -1;
  }

  status = 0;
  for (i = (*pTOpt).loop; i >= 1; i--)
  {
    if ((*pTOpt).loop_status[i - 1] == -2)
      status++;
    else if ((*pTOpt).loop_status[i - 1] == -1)
    {
      status++;
      break;
    }
    else
      break;
  }

  return status;
}

int
neut_topt_refiter (struct TOPT TOpt, int deltaiter, int *prefiter)
{
  int i, loop, status = 0;

  (*prefiter) = TOpt.iter - deltaiter;

  status = neut_topt_iter_loop (TOpt, *prefiter, &loop);

  if (! status)
    for (i = TOpt.loop; i >= loop; i--)
    {
      (*prefiter) -= TOpt.loop_plateau_length[i - 1];

      status = neut_topt_iter_loop (TOpt, *prefiter, &loop);

      if (status)
	break;
    }

  return status;
}

int
neut_topt_iter_loop (struct TOPT TOpt, int iter, int *ploop)
{
  int i;

  (*ploop) = -1;

  if (iter < 1 || iter > TOpt.iter)
    return -1;

  for (i = TOpt.loop; i >= 1; i--)
  {
    if (iter >= TOpt.loop_start[i - 1])
    {
      (*ploop) = i;
      break;
    }
  }

  return 0;
}
