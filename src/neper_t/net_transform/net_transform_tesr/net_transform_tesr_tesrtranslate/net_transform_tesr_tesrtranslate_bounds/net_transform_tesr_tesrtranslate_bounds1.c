/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_tesrtranslate_bounds_.h"

int count;

struct TROPT
{
  double *origin;
  int cellqty;

  int *cellcooqty;
  int *refcellcooqty;
  double ***cellcoos;
  double ***refcellcoos;

  double *bestx;
  double bestval;

  char *message;
};
typedef struct TROPT TROPT;

struct TROPT
tropt_alloc (int cellqty)
{
  struct TROPT Tropt;

  Tropt.origin = ut_alloc_1d (3);

  Tropt.cellqty = cellqty;
  Tropt.cellcooqty = ut_alloc_1d_int (Tropt.cellqty + 1);
  Tropt.refcellcooqty = ut_alloc_1d_int (Tropt.cellqty + 1);
  Tropt.cellcoos = ut_alloc_1d_ppdouble (Tropt.cellqty + 1);
  Tropt.refcellcoos = ut_alloc_1d_ppdouble (Tropt.cellqty + 1);
  Tropt.bestx = ut_alloc_1d (3);
  Tropt.bestval = DBL_MAX;

  Tropt.message = ut_alloc_1d_char (1000);

  return Tropt;
}

double
myfunc (unsigned n, const double *x, double *grad, void *data)
{
  int i, j;
  double val;
  struct TROPT *pTropt = (struct TROPT *) data;
  char *newmessage = ut_alloc_1d_char (1000);

  (void) n;
  (void) x;
  (void) grad;

  val = 0;

  for (i = 0; i < (*pTropt).cellqty; i++)
  {
#pragma omp parallel for private (j)
    for (j = 0; j < (*pTropt).cellcooqty[i]; j++)
    {
      int k;
      double dist, *coo = ut_alloc_1d (3);

      ut_array_1d_add ((*pTropt).cellcoos[i][j], (double *) x, 3, coo);

      dist = DBL_MAX;
      for (k = 0; k < (*pTropt).refcellcooqty[i]; k++)
        dist = ut_num_min (dist, ut_space_dist (coo, (*pTropt).refcellcoos[i][k]));

#pragma omp critical
      val += dist;

      ut_free_1d (&coo);
    }
  }
  val /= ut_array_1d_int_sum ((*pTropt).cellcooqty, (*pTropt).cellqty);

  if (val < (*pTropt).bestval)
  {
    (*pTropt).bestval = val;
    ut_array_1d_memcpy ((double *) x, 3, (*pTropt).bestx);
  }

  ut_print_clearline (stdout, strlen ((*pTropt).message));

  if (count == 0)
  {
    sprintf ((*pTropt).message, "f   =%.6f", val);

    ut_print_message (0, 4, "Using %.0f voxels per cell (min %d max %d)...\n",
                      ut_array_1d_int_mean ((*pTropt).cellcooqty, (*pTropt).cellqty),
                      ut_array_1d_int_min ((*pTropt).cellcooqty, (*pTropt).cellqty),
                      ut_array_1d_int_max ((*pTropt).cellcooqty, (*pTropt).cellqty));
    ut_print_message (0, 4, "Initial solution: ");
    printf ("%s", (*pTropt).message);
    printf ("\n");
  }

  else
  {
    sprintf ((*pTropt).message, "%6d: fmin=%.6f f = %f (s=%9.6f %9.6f %9.6f)",
             count, (*pTropt).bestval, val, x[0], x[1], x[2]);

    if (count == 1)
      ut_print_message (0, 4, "Iteration ");
    printf ("%s", (*pTropt).message);
    fflush (stdout);
  }

  count++;

  ut_free_1d_char (&newmessage);

  return val;
}

void
net_transform_tesr_tesrtranslate_bounds (struct TESR *pTesr, int *cells, int
                                         *refcells, int cellqty, struct TESR Tesr2)
{
  int i;
  double *s = ut_alloc_1d (3), *tol = ut_alloc_1d (3);
  struct TROPT Tropt = tropt_alloc (cellqty);

  ut_array_1d_memcpy ((*pTesr).Origin, 3, Tropt.origin);

  // minimizing distance between grain boundaries
  for (i = 0; i < cellqty; i++)
  {
    neut_tesr_cell_boundcoos (*pTesr, cells[i], Tropt.cellcoos + i,
                              Tropt.cellcooqty + i, 2, "z");
    neut_tesr_cell_boundcoos (Tesr2, refcells[i], Tropt.refcellcoos + i,
                              Tropt.refcellcooqty + i, 2, "z");
  }

#ifdef HAVE_NLOPT
  double minf;

  nlopt_opt opt = nlopt_create (NLOPT_LN_SBPLX, 3);

  nlopt_set_min_objective (opt, myfunc, &Tropt);

  ut_array_1d_memcpy ((*pTesr).vsize, 3, tol);
  ut_array_1d_scale (tol, 3, 0.1);
  nlopt_set_xtol_abs (opt, tol);

  nlopt_set_initial_step (opt, (*pTesr).vsize);

  if (nlopt_optimize (opt, s, &minf) < 0)
  {
    printf ("nlopt failed!\n");
    fflush (stdout);
    abort ();
  }

  printf ("\n");

  nlopt_destroy (opt);
#else
  ut_print_message (2, 0, "This version of Neper was compiled without NLopt.\n");
#endif

  ut_array_1d_add ((*pTesr).Origin, s, 3, (*pTesr).Origin);

  ut_free_1d (&tol);
  ut_free_1d (&s);

  return;
}
