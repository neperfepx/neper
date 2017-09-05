/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include"ut.h"
#include<gsl/gsl_interp.h>

void
ut_fct_set_zero (struct FCT *pFct)
{
  (*pFct).type = NULL;
  (*pFct).expr = NULL;

  (*pFct).mean = 0;
  (*pFct).sig = 0;
  (*pFct).area = 0;

  (*pFct).min = 0;
  (*pFct).max = 0;
  (*pFct).size = 0;
  (*pFct).x = NULL;
  (*pFct).y = NULL;
  (*pFct).interp = NULL;
  (*pFct).interp_accel = NULL;
  (*pFct).interp_type = NULL;

  return;
}

void
ut_fct_set_init_interp (struct FCT *pFct)
{
  if (!(*pFct).interp_type)
    (*pFct).interp_type = (gsl_interp_type*) gsl_interp_cspline;
  (*pFct).interp = gsl_interp_alloc ((*pFct).interp_type, (*pFct).size);
  gsl_interp_init ((*pFct).interp, (*pFct).x, (*pFct).y, (*pFct).size);
  (*pFct).interp_accel = gsl_interp_accel_alloc ();

  return;
}

void
ut_fct_free (struct FCT *pFct)
{
  ut_free_1d_char ((*pFct).expr);
  ut_free_1d_char ((*pFct).type);

  ut_free_1d ((*pFct).x);
  ut_free_1d ((*pFct).y);
  if ((*pFct).interp)
    gsl_interp_free ((*pFct).interp);
  ut_fct_set_zero (pFct);

  return;
}

void
ut_fct_set_dirac (struct FCT *pFct, double x, double area)
{
  ut_string_string ("dirac", &(*pFct).type);
  (*pFct).mean = x;
  (*pFct).sig = 0;
  (*pFct).area = area;

  return;
}

void
ut_fct_set_numerical (struct FCT *pFct, double min, double max, int size)
{
  ut_string_string ("numerical", &(*pFct).type);
  ut_string_string ("none", &(*pFct).expr);
  (*pFct).min = min;
  (*pFct).max = max;
  (*pFct).size = size;
  (*pFct).x = ut_alloc_1d ((*pFct).size);
  (*pFct).y = ut_alloc_1d ((*pFct).size);
  ut_fct_interval_x_linear (min, max, size, pFct);

  return;
}

void
ut_fct_set_normal (struct FCT *pFct, double mean, double sig)
{
  ut_string_string ("normal", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;

  return;
}

void
ut_fct_set_erf (struct FCT *pFct, double mean, double sig)
{
  ut_string_string ("erf", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;

  return;
}

void
ut_fct_set_lognormal (struct FCT *pFct, double mean, double sig, char *expr)
{
  ut_string_string ("lognormal", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  ut_string_string (expr, &(*pFct).expr);

  return;
}

double
ut_fct_eval (struct FCT Fct, double x)
{
  double val;

  if (!strcmp (Fct.type, "numerical"))
  {
    if (x < Fct.x[0])
      val = Fct.y[0];
    else if (x > Fct.x[Fct.size - 1])
      val = Fct.y[Fct.size - 1];
    else
      val = gsl_interp_eval (Fct.interp, Fct.x, Fct.y, x, Fct.interp_accel);
  }

  else if (!strcmp (Fct.type, "dirac"))
    val = (x == Fct.mean) ? DBL_MAX : 0;

  else if (!strcmp (Fct.type, "normal"))
    val = exp (-pow (x - Fct.mean, 2) / (2 * pow (Fct.sig, 2))) / (Fct.sig * sqrt (2 * M_PI));

  else if (!strcmp (Fct.type, "lognormal"))
  {
    double ln_sig, ln_mu;
    if (Fct.expr && strstr (Fct.expr, "1-x"))
      x = 1 - x;

    ln_sig = sqrt (log (1 + pow (Fct.sig, 2) / pow (Fct.mean, 2)));
    ln_mu = log (Fct.mean) - .5 * pow (ln_sig, 2);

    val = (x > 0) ?
          exp (-pow (log (x) - ln_mu, 2) / (2 * pow (ln_sig, 2)))
	    / (x * ln_sig * sqrt (2 * M_PI)) : 0;
  }

  else if (!strcmp (Fct.type, "erf"))
    val = 0.5 + 0.5 * erf ((x - Fct.mean) / (sqrt (2) * Fct.sig));

  else
  {
    printf ("Unknown Fct.type = %s\n", Fct.type);
    abort ();
  }

  return val;
}

void
ut_fct_numericalfct_expr (struct FCT Fct, char *expr, struct FCT *pFct2)
{
  int size;
  double min, max;

  sscanf (expr, "regular(%lf,%lf,%d)", &min, &max, &size);
  ut_fct_numericalfct (Fct, min, max, size, pFct2);

  return;
}

void
ut_fct_numericalfct (struct FCT Fct, double min, double max, int size, struct FCT *pFct2)
{
  int i, pos;
  double sum;
  struct FCT FctC;

  ut_fct_set_zero (&FctC);
  ut_fct_memcpy (Fct, &FctC);

  ut_fct_set_numerical (pFct2, min, max, size);
  (*pFct2).mean = FctC.mean;
  (*pFct2).sig = FctC.sig;
  (*pFct2).area = FctC.area;

  ut_fct_interval_x_linear (min, max, size, pFct2);

  if (FctC.type && !strcmp (FctC.type, "dirac"))
  {
    pos = ut_array_1d_eltpos ((*pFct2).x, (*pFct2).size, FctC.mean);
    (*pFct2).y[pos] = 1. / ut_fct_binwidth (*pFct2, pos);
  }
  else
  {
    sum = 0;
    for (i = 0; i < (*pFct2).size; i++)
    {
      (*pFct2).y[i] = ut_fct_eval (FctC, (*pFct2).x[i]);
      sum += ut_fct_binwidth (*pFct2, i) * (*pFct2).y[i];
    }
    ut_array_1d_scale ((*pFct2).y, (*pFct2).size, FctC.area / sum);
  }

  (*pFct2).interp_type = (gsl_interp_type*) gsl_interp_cspline;
  if ((*pFct2).interp)
    gsl_interp_free ((*pFct2).interp);
  (*pFct2).interp = gsl_interp_alloc ((*pFct2).interp_type, (*pFct2).size);
  gsl_interp_init ((*pFct2).interp, (*pFct2).x, (*pFct2).y, (*pFct2).size);
  if ((*pFct2).interp_accel)
    gsl_interp_accel_free ((*pFct2).interp_accel);
  (*pFct2).interp_accel = gsl_interp_accel_alloc ();

  ut_fct_free (&FctC);

  return;
}

void
ut_fct_memcpy (struct FCT Fct, struct FCT *pFct2)
{
  ut_fct_free (pFct2);
  ut_fct_set_zero (pFct2);

  ut_string_string (Fct.type, &(*pFct2).type);
  ut_string_string (Fct.expr, &(*pFct2).expr);

  (*pFct2).mean = Fct.mean;
  (*pFct2).sig = Fct.sig;
  (*pFct2).area = Fct.area;

  (*pFct2).min = Fct.min;
  (*pFct2).max = Fct.max;
  (*pFct2).size = Fct.size;
  (*pFct2).x = ut_alloc_1d ((*pFct2).size);
  (*pFct2).y = ut_alloc_1d ((*pFct2).size);
  ut_array_1d_memcpy ((*pFct2).x, (*pFct2).size, Fct.x);
  ut_array_1d_memcpy ((*pFct2).y, (*pFct2).size, Fct.y);

  (*pFct2).interp_type = Fct.interp_type;
  if ((*pFct2).interp_type)
    ut_fct_set_init_interp (pFct2);
  (*pFct2).interp_accel = Fct.interp_accel;

  return;
}

void
ut_fct_integralfct (struct FCT Fct, struct FCT *pFct2)
{
  int i;

  ut_fct_free (pFct2);

  if (!strcmp (Fct.type, "normal"))
    ut_fct_set_erf (pFct2, Fct.mean, Fct.sig);

  else if (!strcmp (Fct.type, "numerical"))
  {
    ut_fct_set_numerical (pFct2, Fct.min, Fct.max, Fct.size);
    ut_array_1d_memcpy ((*pFct2).x, (*pFct2).size, Fct.x);

    for (i = 0; i < (*pFct2).size; i++)
      if (i == 0)
	(*pFct2).y[i] = 0.5 * ut_fct_binwidth (Fct, i) * Fct.y[i];
      else
	(*pFct2).y[i] = 0.5 * ut_fct_binwidth (Fct, i - 1) * Fct.y[i - 1]
		      + 0.5 * ut_fct_binwidth (Fct, i) * Fct.y[i]
		      + (*pFct2).y[i - 1];
    (*pFct2).interp_type = (gsl_interp_type*) gsl_interp_cspline;
    ut_fct_set_init_interp (pFct2);
  }

  else
    abort ();

  ut_array_1d_scale ((*pFct2).y, (*pFct2).size, 1 / (*pFct2).y[(*pFct2).size - 1]);

  return;
}

double
ut_fct_binwidth (struct FCT Fct, int bin)
{
  if (bin == 0)
    return Fct.x[1] - Fct.x[0];
  else if (bin == Fct.size - 1)
    return Fct.x[Fct.size - 1] - Fct.x[Fct.size - 2];
  else if (bin > 0 && bin < Fct.size - 1)
    return 0.5 * (Fct.x[bin + 1] - Fct.x[bin - 1]);
  else
  {
    printf ("bin = %d out of range [%d,%d]\n", bin, 0, Fct.size - 1);
    abort ();
  }
}

void
ut_fct_convolution (struct FCT Fct, double sig, struct FCT *pFct2)
{
  int i, j;
  struct FCT FctC;

  ut_fct_set_zero (&FctC);
  ut_fct_set_normal (&FctC, 0, sig);

  if (sig == 0)
    ut_fct_memcpy (Fct, pFct2);

  else if (!strcmp (Fct.type, "normal"))
    ut_fct_set_normal (pFct2, Fct.mean, sqrt (pow (Fct.sig, 2) + pow (sig, 2)));

  else if (!strcmp (Fct.type, "dirac"))
    ut_fct_set_normal (pFct2, Fct.mean, sig);

  else if (!strcmp (Fct.type, "numerical"))
  {
    ut_fct_memcpy (Fct, pFct2);
    ut_array_1d_zero ((*pFct2).y, (*pFct2).size);

    for (i = 0; i < Fct.size; i++)
      for (j = 0; j < Fct.size; j++)
	(*pFct2).y[i]
	  += Fct.y[j] * ut_fct_eval (FctC, Fct.x[i] - Fct.x[j])
		      * ut_fct_binwidth (Fct, j);

    (*pFct2).interp_type = (gsl_interp_type*) gsl_interp_cspline;
  }

  else
    abort ();

  ut_fct_free (&FctC);

  return;
}

int
ut_fct_set (char *string, struct FCT *pFct)
{
  int status;
  char *fct = NULL;
  char **vals = NULL;
  int varqty;
  double mean, sig;
  char *expr = NULL;

  ut_string_function_separate (string, &fct, NULL, &vals, &varqty);
  if (varqty >= 1)
    sscanf (vals[0], "%lf", &mean);
  if (varqty >= 2)
    sscanf (vals[1], "%lf", &sig);
  if (varqty >= 3)
    ut_string_string (vals[2], &expr);

  status = 0;
  if (!strcmp (fct, "dirac"))
    ut_fct_set_dirac (pFct, mean, 1);
  else if (!strcmp (fct, "normal"))
    ut_fct_set_normal (pFct, mean, sig);
  else if (!strcmp (fct, "lognormal"))
    ut_fct_set_lognormal (pFct, mean, sig, expr);
  else
  {
    status = -1;
    abort ();
  }

  ut_free_1d_char (fct);
  ut_free_2d_char (vals, varqty);
  ut_free_1d_char (expr);

  return status;
}

void
ut_fct_interval_x_linear (double min, double max, int size,
                          struct FCT *pFct)
{
  int i;

  (*pFct).x = ut_realloc_1d ((*pFct).x, size);
  (*pFct).min = min;
  (*pFct).max = max;
  (*pFct).size = size;

  for (i = 0; i < (*pFct).size; i++)
    (*pFct).x[i] = min + (max - min) * (2 * i + 1) / (2 * (*pFct).size);

  return;
}

int
ut_fct_scale (struct FCT *pFct, double val)
{
  int i;

  (*pFct).area *= val;

  for (i = 0; i < (*pFct).size; i++)
    (*pFct).y[i] *= val;

  return 0;
}

int
ut_fct_add (struct FCT *Fct1, int qty, double *fact, char *expr, struct FCT *pFct2)
{
  int i, j;

  ut_fct_memcpy (Fct1[0], pFct2);
  ut_string_string (expr, &(*pFct2).expr);

  if (qty == 1)
    return 0;

  ut_array_1d_zero ((*pFct2).y, (*pFct2).size);
  (*pFct2).area = 0;
  for (j = 0; j < qty; j++)
  {
    (*pFct2).area += (fact? fact[j] : 1) * Fct1[j].area;
    for (i = 0; i < (*pFct2).size; i++)
      (*pFct2).y[i] += (fact? fact[j] : 1) * Fct1[j].y[i];
  }

  return 0;
}

void
ut_fct_debug (FILE *fp, struct FCT Fct)
{
  int i;

  fprintf (fp, "Fct.type = %s\n", Fct.type);
  fprintf (fp, "Fct.mean = %f\n", Fct.mean);
  fprintf (fp, "Fct.sig = %f\n", Fct.sig);
  fprintf (fp, "Fct.area = %f\n", Fct.area);
  fprintf (fp, "Fct.expr = %s\n", Fct.expr);
  fprintf (fp, "Fct.size = %d\n", Fct.size);
  fprintf (fp, "Fct.min = %f\n", Fct.min);
  fprintf (fp, "Fct.max = %f\n", Fct.max);

  for (i = 0; i < Fct.size; i++)
    fprintf (fp, "%f %f\n", Fct.x[i], Fct.y[i]);

  return;
}

int
ut_fct_x_pos (struct FCT Fct, double x)
{
  int id;
  double binwidth = ut_fct_binwidth (Fct, 0);

  id = (x - Fct.min) / binwidth;
  if (id < 0 && x < Fct.min - binwidth * 1e-6)
    id = -1;
  else if (id > Fct.size - 1 && x > Fct.max + binwidth * 1e-6)
    id = -1;

  return id;
}
