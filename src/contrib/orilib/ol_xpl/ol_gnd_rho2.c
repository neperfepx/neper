/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_gnd_rho.h"

#ifdef HAVE_NLOPT
#include <nlopt.h>
#endif /* HAVE_NLOPT */

void
ol_gnd_crys_b (struct OL_CRYS Crys, double *pb)
{
  if (!strcmp (Crys.structure, "sc"))
    (*pb) = Crys.a;
  else if (!strcmp (Crys.structure, "fcc"))
    (*pb) = Crys.a * sqrt (2) / 2;	// fcc {111}<110>
  else
    abort ();

  return;
}

void
ol_gnd_nye_rho_sc (struct OL_CRYS Crys, double **nye, double *rho)
{
  int i, j;
  double b;
  int id = 0;

  ol_gnd_crys_b (Crys, &b);

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      rho[id++] = nye[i][j] / b;

  return;
}

void
ol_gnd_nye_rho_l2 (struct OL_CRYS Crys, double **A, double *nyev,
		   double *rho)
{
  double **At = ut_alloc_2d (18, 9);
  double **AAt = ut_alloc_2d (9, 9);
  double **AAtm1 = ut_alloc_2d (9, 9);
  double **B = ut_alloc_2d (18, 9);
  double b;

  ol_gnd_crys_b (Crys, &b);

  ut_mat_transpose (A, 9, 18, At);
  ut_mat_product (A, 9, 18, At, 18, 9, AAt);
  ut_mat_inverse (AAt, 9, AAtm1);
  ut_mat_product (At, 18, 9, AAtm1, 9, 9, B);
  ut_mat_vect_product (B, 18, 9, nyev, 9, rho);
  ut_array_1d_scale (rho, 18, 1 / b);

  ut_free_2d (At, 18);
  ut_free_2d (AAt, 9);
  ut_free_2d (AAtm1, 9);
  ut_free_2d (B, 18);

  return;
}

#ifdef HAVE_GLPK
void
ol_gnd_nye_rho_l1 (struct OL_CRYS Crys, double **A, double *nyev, double *rho)
{
  ol_gnd_nye_rho_simplex (Crys, A, nyev, rho, "L1");

  return;
}

void
ol_gnd_nye_rho_energy (struct OL_CRYS Crys, double **A, double *nyev, double *rho)
{
  ol_gnd_nye_rho_simplex (Crys, A, nyev, rho, "energy");

  return;
}

void
ol_gnd_nye_rho_simplex (struct OL_CRYS Crys, double **A, double *nyev, double *rho, char* type)
{
  glp_prob *lp;
  glp_smcp parm;

  int i, j;
  int eqqty, varqty, varqty2;
  double *B = nyev;
  double *x = rho;
  double b;

  glp_term_out (GLP_OFF);

  eqqty = 9;
  varqty  = 18;

  varqty2 = 2 * varqty;

  lp = glp_create_prob ();
  glp_set_prob_name (lp, "gnd");
  glp_set_obj_dir (lp, GLP_MIN);

  glp_add_rows (lp, eqqty);

  for (i = 1; i <= eqqty; i++)
  {
    glp_set_row_name (lp, i, "row");
    glp_set_row_bnds (lp, i, GLP_FX, B[i - 1], B[i - 1]);
  }

  glp_add_cols (lp, varqty2);

  for (i = 1; i <= varqty2; i++)
  {
    glp_set_col_name (lp, i, "varlb");
    glp_set_col_bnds (lp, i, GLP_LO, 0.0, 0.0);
    if (! strcmp (type, "L1"))
      glp_set_obj_coef (lp, i, 1.);	// coef of the objective function.
    else if (! strcmp (type, "energy"))
    {
      if (! strcmp (Crys.structure, "fcc"))
      {
	if ((i >= 1 && i <= 12) || (i >= 19 && i <= 30)) // edge dislocation
	  glp_set_obj_coef (lp, i, 1.);
	else
	  glp_set_obj_coef (lp, i, 1. / (1 - Crys.nu));
      }
      else
	abort ();
    }
    else
      abort ();
  }

  int qty = eqqty * varqty2;
  int *ia = ut_alloc_1d_int (qty + 1);
  int *ja = ut_alloc_1d_int (qty + 1);
  double *ar = ut_alloc_1d (qty + 1);
  int id;

  id = 1;
  for (i = 0; i < eqqty; i++)
  {
    for (j = 0; j < varqty; j++)
    {
      ia[id] = i + 1;
      ja[id] = j + 1;
      ar[id] = A[i][j];
      id++;
    }
    for (j = varqty; j < varqty2; j++)
    {
      ia[id] = i + 1;
      ja[id] = j + 1;
      ar[id] = -A[i][j - varqty];
      id++;
    }
  }

  glp_load_matrix (lp, qty, ia, ja, ar);

  glp_init_smcp (&parm);
  parm.presolve = GLP_OFF;
  glp_simplex (lp, &parm);

  if (glp_get_status (lp) == GLP_NOFEAS)
    abort ();

  for (i = 0; i < varqty; i++)
    if (glp_get_col_prim (lp, i + 1) > 0)
      x[i] = glp_get_col_prim (lp, i + 1);
    else
      x[i] = -glp_get_col_prim (lp, i + varqty + 1);

  ol_gnd_crys_b (Crys, &b);

  ut_array_1d_scale (x, 18, 1 / b);

  glp_delete_prob (lp);

  return;
}
#endif

#ifdef HAVE_NLOPT
int count;

double
myfunc (unsigned n, const double *x, double *grad, void *my_func_data)
{
  count++;

  n = n;
  grad = grad;
  my_func_data = my_func_data;

  int i;
  double value = 0;

  for (i = 0; i < 18; i++)
    value += x[i] * x[i] * x[i];

  if (grad)
    for (i = 0; i < 18; i++)
      grad[i] = 3 * x[i] * x[i];

  return value;
}

typedef struct
{
  double *A_line;
  double nyev;

} my_constraint_data;

double
myconstraint (unsigned n, const double *x, double *grad, void *data)
{
  my_constraint_data *d = (my_constraint_data *) data;

  n = n;
  grad = grad;

  int i;
  double value = 0;
  for (i = 0; i < 18; i++)
    value += d->A_line[i] * x[i];

  value -= d->nyev;

  // if (grad)
  // {
  // grad[0] = 3 * a * (a * x[0] + b) * (a * x[0] + b);
  // grad[1] = -1.0;
  // }
  return value;
}

void
ol_gnd_nye_rho_norm (struct OL_CRYS Crys, double **A, double *nyev,
		     double *rho)
{
  int i;
  int count = 0;
  double b;
  double A_max;
  double nyev_max;

  int size1, size2;
  size1 = 9;
  size2 = 18;

  A_max = ut_array_2d_absmax (A, size1, size2);
  ut_array_2d_scale (A, size1, size2, 1 / A_max);

  nyev_max = ut_array_1d_absmax (nyev, size1);
  ut_array_1d_scale (nyev, size1, 1 / nyev_max);

  ol_gnd_crys_b (Crys, &b);

  nlopt_opt opt = nlopt_create (NLOPT_LN_COBYLA, size2);	// algorithm and dimensionality

  nlopt_set_min_objective (opt, myfunc, NULL);

  my_constraint_data *data = malloc (size1 * sizeof (my_constraint_data));

  for (i = 0; i < size1; i++)
  {
    data[i].A_line = ut_alloc_1d (size2);
    ut_array_1d_memcpy (data[i].A_line, size2, A[i]);
    data[i].nyev = nyev[i];
  }

  for (i = 0; i < size1; i++)
    nlopt_add_equality_constraint (opt, myconstraint, &data[i], 1e-8);

  nlopt_set_xtol_rel (opt, 1e-4);

  // Setting initial guess
  ut_array_1d_zero (rho, size2);
  // ol_gnd_nye_rho_l2m (Crys, A, nyev, rho);
  // ut_array_1d_scale  (rho, size2, b);
  // for (i = 0; i < size2; i++)
  // rho[i] = ut_num_max (0, rho[i]);

  double minf;			/* the minimum objective value, upon return */

  if (nlopt_optimize (opt, rho, &minf) < 0)
  {
    printf ("nlopt failed!\n");
    abort ();
  }

  ut_array_1d_scale (rho, size2, nyev_max / A_max);
  ut_array_1d_scale (rho, size2, 1 / b);

  ut_array_2d_scale (A, size1, size2, A_max);
  ut_array_1d_scale (nyev, size1, nyev_max);

  printf ("found minimum = %0.10g (%d iters)\n", minf, count);
  double *B = ut_alloc_1d (size1);
  ut_mat_vect_product (A, size1, size2, rho, size2, B);
  for (i = 0; i < 8; i++)
    printf ("%g %g\n", nyev[i], B[i]);

  nlopt_destroy (opt);

  return;
}
#endif
