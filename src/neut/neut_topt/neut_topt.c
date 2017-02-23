/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_topt_.h"

void
neut_topt_set_zero (struct TOPT *pTOpt)
{
  (*pTOpt).Dim = 0;
  (*pTOpt).CellQty = 0;
  neut_tess_set_zero (&(*pTOpt).Dom);
  neut_tess_set_zero (&(*pTOpt).DomPer);
  neut_seedset_set_zero (&(*pTOpt).SSet);
  (*pTOpt).Poly = NULL;

  neut_tdyn_set_zero (&(*pTOpt).TDyn);

  (*pTOpt).xqty = 0;        
  (*pTOpt).x_pvar = NULL;
  (*pTOpt).x_seed = NULL;
  (*pTOpt).x_var = NULL;

  (*pTOpt).dof = NULL;
  (*pTOpt).objective = NULL;

  (*pTOpt).disgrid = NULL;
  (*pTOpt).dissmoothing = 0;
  (*pTOpt).cvl = NULL;

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
  (*pTOpt).eps = 0;
  (*pTOpt).xeps = 0;
  (*pTOpt).reps = 0;
  (*pTOpt).xreps = 0;
  (*pTOpt).time = 0;
  (*pTOpt).val = 0;
  (*pTOpt).dvalditer = -1;
  (*pTOpt).dist = 0;

  (*pTOpt).boundl = NULL;
  (*pTOpt).boundu = NULL;

  (*pTOpt).algoname = NULL;

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
  (*pTOpt).tartesrscale = NULL;
  (*pTOpt).tarcellpts = NULL;
  (*pTOpt).tarcellptqty = NULL;
  (*pTOpt).tarcellrefval = NULL;
  (*pTOpt).tarcellptscells = NULL;
  (*pTOpt).faceconn = 1;

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
  (*pTOpt).iter = 0;
  (*pTOpt).message = NULL;

  gettimeofday (&((*pTOpt).end_time), NULL);

  return;
}

void
neut_topt_free (struct TOPT *pTOpt)
{
  int i, j;

  neut_tdyn_free (&(*pTOpt).TDyn);

  ut_free_1d ((*pTOpt).tartesrscale);
  ut_free_1d ((*pTOpt).tarcellrefval);

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

  ut_free_1d_int ((*pTOpt).tarcellptqty);

  neut_tesr_free (&(*pTOpt).tartesr);

  ut_free_1d_pdouble((*pTOpt).x_pvar);
  ut_free_1d_int((*pTOpt).x_seed);
  ut_free_1d_int((*pTOpt).x_var);

  ut_free_1d_char ((*pTOpt).algoname);
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

  ut_free_1d_char ((*pTOpt).message);
  ut_free_1d_char ((*pTOpt).objective);

  neut_topt_set_zero (pTOpt);

  neut_tess_free (&(*pTOpt).Dom);
  neut_tess_free (&(*pTOpt).DomPer);
  neut_poly_array_free (&(*pTOpt).Poly, (*pTOpt).SSet.N);
  neut_seedset_free (&(*pTOpt).SSet); // must be freed at the end (used to free others)
  // doing nothing to pTess (pointer only)

  ut_free_2d_char ((*pTOpt).disgrid, (*pTOpt).tarqty);

  for (i = 0; i < (*pTOpt).tarqty; i++)
    ut_fct_free ((*pTOpt).cvl + i);
  free ((*pTOpt).cvl);

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
