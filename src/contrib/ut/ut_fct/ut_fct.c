/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include"ut.h"

#ifdef HAVE_GSL
#include<gsl/gsl_interp.h>

void
ut_fct_set_zero (struct FCT *pFct)
{
  (*pFct).type = NULL;
  (*pFct).expr = NULL;

  (*pFct).mean = 0;
  (*pFct).sig = 0;
  (*pFct).gamma = 0;
  (*pFct).type_from = 0;
  (*pFct).type_to = 0;
  (*pFct).from = 0;
  (*pFct).to = 0;
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

// sets parameters of the Dirac delta function
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
ut_fct_set_numerical (struct FCT *pFct, double min, double max, int size,
                      char *method)
{
  ut_string_string ("numerical", &(*pFct).type);
  ut_string_string ("none", &(*pFct).expr);
  (*pFct).min = min;
  (*pFct).max = max;
  (*pFct).size = size;
  (*pFct).x = ut_alloc_1d ((*pFct).size);
  (*pFct).y = ut_alloc_1d ((*pFct).size);
  ut_fct_interval_x_linear (min, max, size, method, pFct);

  return;
}

// sets parameters of the Normal (Gaussian) distribution
void
ut_fct_set_normal (struct FCT *pFct, double mean, double sig, int type_from,
		   int type_to, double from, double to)
{
  ut_string_string ("normal", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

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

// sets parameters of the Log-normal distribution
void
ut_fct_set_lognormal (struct FCT *pFct, double mean, double sig, char *expr,
		      int type_from, int type_to, double from, double to)
{
  ut_string_string ("lognormal", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  ut_string_string (expr, &(*pFct).expr);
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Lorentzian distribution
void
ut_fct_set_lorentzian (struct FCT *pFct, double mean, double sig,
		       int type_from, int type_to, double from, double to)
{
  ut_string_string ("lorentzian", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Student's t distribution
void
ut_fct_set_studentst (struct FCT *pFct, double mean, double sig,
		      int type_from, int type_to, double from, double to)
{
  ut_string_string ("studentst", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Pseudo-Voigt distribution
void
ut_fct_set_pseudovoigt (struct FCT *pFct, double mean, double sig,
			double gamma, int type_from, int type_to, double from,
			double to)
{
  ut_string_string ("pseudovoigt", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  if (gamma <= 1 && gamma >= 0)	// weighting fraction: (1-gamma)*normal + gamma*lorentzian
    (*pFct).gamma = gamma;
  else
  {
    ut_print_message (2, 0,
		      "The distribution function 'pseudovoigt' requires 0 <= gamma <= 1 (default: gamma = 0.5).\n");
    abort ();
  }
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Moffat distribution
void
ut_fct_set_moffat (struct FCT *pFct, double mean, double sig, double gamma,
		   int type_from, int type_to, double from, double to)
{
  ut_string_string ("moffat", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  if (gamma > 0)
    (*pFct).gamma = gamma;	// exponent (often called beta): pdf = A*[...]^(-gamma)
  else
  {
    ut_print_message (2, 0,
		      "The distribution function 'moffat' requires gamma > 0 (default: gamma = 1).\n");
    abort ();
  }
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Pearson type VII distribution
void
ut_fct_set_pearson7 (struct FCT *pFct, double mean, double sig, double gamma,
		     int type_from, int type_to, double from, double to)
{
  ut_string_string ("pearson7", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).gamma = gamma;	// exponent (m)
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Breit-Wigner-Fano function
void
ut_fct_set_breitwigner (struct FCT *pFct, double mean, double sig,
			double gamma, int type_from, int type_to, double from,
			double to)
{
  ut_string_string ("breitwigner", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  if (gamma >= 0)
    (*pFct).gamma = gamma;	// q, the Fano parameter
  else
  {
    ut_print_message (2, 0,
		      "The distribution function 'breitwigner' requires gamma >= 0 (default: gamma = 1).\n");
    abort ();
  }
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Exponential Gaussian (normal) distribution
void
ut_fct_set_expnormal (struct FCT *pFct, double mean, double sig, double gamma,
		      int type_from, int type_to, double from, double to)
{
  ut_string_string ("expnormal", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  if (gamma > 0)
    (*pFct).gamma = gamma;
  else
  {
    ut_print_message (2, 0,
		      "The distribution function 'expnormal' requires gamma > 0 (default: gamma = sigma).\n");
    abort ();
  }
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Skewed Gaussian (normal) distribution
void
ut_fct_set_skewnormal (struct FCT *pFct, double mean, double sig,
		       double gamma, int type_from, int type_to, double from,
		       double to)
{
  ut_string_string ("skewnormal", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).gamma = gamma;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}
/* DOES NOT WORK
// sets parameters of the distribution
void
ut_fct_set_donaich (struct FCT *pFct, double mean, double sig, double gamma,
		    int type_from, int type_to, double from, double to)
{
  ut_string_string ("donaich", &(*pFct).type);
  (*pFct).mean = mean;
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).gamma = gamma;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}
*/
// sets parameters of the Beta distribution
void
ut_fct_set_beta (struct FCT *pFct, double mean, double sig, int type_from,
		 int type_to, double from, double to)
{
  ut_string_string ("beta", &(*pFct).type);
  if (mean > 0)
    (*pFct).mean = mean;
  else
  {
    ut_print_message (2, 0,
		      "The distribution function 'beta' requires x, y > 0.\n");
    abort ();
  }
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

// sets parameters of the Weibull distribution
void
ut_fct_set_weibull (struct FCT *pFct, double mean, double sig,
		    int type_from, int type_to, double from, double to)
{
  ut_string_string ("weibull", &(*pFct).type);
  if (mean > 0)
    (*pFct).mean = mean;
  else
  {
    ut_print_message (2, 0,
		      "The distribution function 'weibull' requires mean, sigma > 0.\n");
    abort ();
  }
  (*pFct).sig = sig;
  (*pFct).area = 1;
  (*pFct).type_from = type_from;
  (*pFct).type_to = type_to;
  (*pFct).from = from;
  (*pFct).to = to;

  return;
}

double
ut_fct_eval (struct FCT Fct, double x)
{
  double val;
  char skip = '0';

  // makes sure x is within limits, if limits given
  skip = Fct.type_from == 0 ? skip
    : Fct.type_from == 1 ? (x < Fct.from ? '1' : skip)
    : Fct.type_from == 2 ? (x <= Fct.from ? '1' : skip) : skip;
  skip = Fct.type_from == 0 ? skip
    : Fct.type_to == 1 ? (x > Fct.to ? '1' : skip)
    : Fct.type_to == 2 ? (x >= Fct.to ? '1' : skip) : skip;
  if (skip == '0')
  {
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
      val =	exp (-pow (x - Fct.mean, 2) / (2 * pow (Fct.sig, 2)))
                / (Fct.sig * sqrt (2 *M_PI));

    else if (!strcmp (Fct.type, "lognormal"))
    {
      double ln_sig, ln_mu, c;
      if (Fct.expr && sscanf (Fct.expr, "%lf", &c))
      {
      if (c <= 0)
      {
        ut_print_message (2, 0, "Please enter an expr > 0!\n");
        abort ();
      }
        x = c - x;
      }

      ln_sig = sqrt (log (1 + pow (Fct.sig, 2) / pow (Fct.mean, 2)));
      ln_mu = log (Fct.mean) - .5 * pow (ln_sig, 2);

      val = (x > 0) ?
            exp (-pow (log (x) - ln_mu, 2) / (2 * pow (ln_sig, 2)))
              / (x * ln_sig * sqrt (2 * M_PI)) : 0;
    }

    else if (!strcmp (Fct.type, "lorentzian"))
      val = Fct.sig / ((pow (x - Fct.mean, 2) + pow (Fct.sig, 2)) * M_PI);

    else if (!strcmp (Fct.type, "studentst"))
    {
      const double halfSig = Fct.sig * 0.5;

      val = pow (1. + pow (x - Fct.mean, 2) / Fct.sig, -halfSig - 0.5)
              / (tgamma (halfSig) * sqrt (Fct.sig * M_PI))
              * tgamma (halfSig + 0.5);
    }

    else if (!strcmp (Fct.type, "pseudovoigt"))
    {
      double sig_g = Fct.sig / sqrt (2. * log (2.)),
              dist = pow (x - Fct.mean, 2);

      val = (Fct.gamma * Fct.sig / ((dist + pow (Fct.sig, 2)) * M_PI))
              + ((1 - Fct.gamma) * exp (-dist * 0.5 / pow (sig_g, 2))
              / (sig_g * sqrt (2. * M_PI)));
    }

    else if (!strcmp (Fct.type, "moffat"))
      val = (x > 0) ?
              pow (pow ((x - Fct.mean) / Fct.sig, 2) + 1., -Fct.gamma) : 0;

    else if (!strcmp (Fct.type, "pearson7"))
    {
      val = (tgamma (Fct.gamma)
              * pow (1 + pow ((x - Fct.mean) / Fct.sig, 2), -Fct.gamma))
              / (Fct.sig * tgamma (Fct.gamma - 0.5) * tgamma (0.5));
    }

    else if (!strcmp (Fct.type, "breitwigner"))
    {
      const double sum = x - Fct.mean, fac = Fct.sig * 0.5;
      val = pow (Fct.gamma * fac + sum, 2) / (pow (fac, 2) + pow (sum, 2));
    }
    else if (!strcmp (Fct.type, "expnormal"))
    {
      const double prod = Fct.gamma * pow (Fct.sig, 2), sum = Fct.mean - x;

      val = 0.5 * Fct.gamma * exp (Fct.gamma * (0.5 * prod + sum))
              * erfc ((sum + prod) / (Fct.sig * sqrt (2.)));
    }

    else if (!strcmp (Fct.type, "skewnormal"))
    {
      const double factor = (x - Fct.mean) / (Fct.sig * sqrt (2.));

      val = (1. + erf (factor * Fct.gamma)) * exp (-pow (factor, 2))
              / (Fct.sig * sqrt (2. * M_PI));
    }
/*
    else if (!strcmp (Fct.type, "donaich"))
    {
      const double fac0 = (x - Fct.mean) / Fct.sig, fac1 = 1. - Fct.gamma;

      val = cos (0.5 * M_PI * Fct.gamma + fac1 * atan (fac0))
              / (pow (1 + fac0, fac1 * 0.5) * pow (Fct.sig, fac1));
    }
*/
    else if (!strcmp (Fct.type, "beta"))
    {
      val = (x > 0 && x < 1) ?
              tgamma (Fct.mean + Fct.sig) * pow (x, Fct.mean - 1.)
              * pow (1. - x, Fct.sig - 1.)
              / (tgamma (Fct.mean) * tgamma (Fct.sig)) : 0;
    }

    else if (!strcmp (Fct.type, "weibull"))
    {
      val = (x > 0) ?
              Fct.mean * pow (x, Fct.mean - 1)
              * exp (-pow ((x / Fct.sig), Fct.mean)) / pow (Fct.sig, Fct.mean)
              :	0;
    }
    else if (!strcmp (Fct.type, "erf"))
      val = 0.5 + 0.5 * erf ((x - Fct.mean) / (sqrt (2) * Fct.sig));

    else
    {
      printf ("Unknown Fct.type = %s\n", Fct.type);
      abort ();
    }
  }				// if (skip == '1')
  else
    val = 0;			// x is out of limits

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
ut_fct_numericalfct (struct FCT Fct, double min, double max, int size,
		     struct FCT *pFct2)
{
  int i, pos;
  double sum;
  struct FCT FctC;

  ut_fct_set_zero (&FctC);
  ut_fct_memcpy (Fct, &FctC);

  ut_fct_set_numerical (pFct2, min, max, size, "center");
  (*pFct2).mean = FctC.mean;
  (*pFct2).sig = FctC.sig;
  (*pFct2).area = FctC.area;

  ut_fct_interval_x_linear (min, max, size, "center", pFct2);

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
  (*pFct2).gamma = Fct.gamma;
  (*pFct2).type_from = Fct.type_from;
  (*pFct2).type_to = Fct.type_to;
  (*pFct2).from = Fct.from;
  (*pFct2).to = Fct.to;
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
    ut_fct_set_numerical (pFct2, Fct.min, Fct.max, Fct.size, "center");
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

  ut_array_1d_scale ((*pFct2).y, (*pFct2).size,
		     1 / (*pFct2).y[(*pFct2).size - 1]);

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
  ut_fct_set_normal (&FctC, 0, sig, 0, 0, 0, 0);

  if (sig == 0)
    ut_fct_memcpy (Fct, pFct2);

  else if (!strcmp (Fct.type, "normal"))
    ut_fct_set_normal (pFct2, Fct.mean,
		       sqrt (pow (Fct.sig, 2) + pow (sig, 2)), Fct.type_from,
		       Fct.type_to, Fct.from, Fct.to);

  else if (!strcmp (Fct.type, "dirac"))
    ut_fct_set_normal (pFct2, Fct.mean, sig, Fct.type_from, Fct.type_to,
		       Fct.from, Fct.to);

  else if (!strcmp (Fct.type, "numerical"))
  {
    ut_fct_memcpy (Fct, pFct2);
    ut_array_1d_zero ((*pFct2).y, (*pFct2).size);

    for (i = 0; i < Fct.size; i++)
      for (j = 0; j < Fct.size; j++)
        (*pFct2).y[i] += Fct.y[j] * ut_fct_eval (FctC, Fct.x[i] - Fct.x[j])
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
  int i;
  int status;			// return value

  int inputqty;			// number of input parameters
  char *fct = NULL;		// holds name of the input function
  char **vars = NULL;		// holds input parameter keywords
  char **vals = NULL;		// holds input parameter values

  int varqty;			// number of parameter keywords
  char **varlist = NULL;	// list of parameter keywords
  char *param = NULL;

  // parameter variables
  double mean, sig, gam, from = 0, to = 0;
  char *expr = NULL;

  int input_switch[3] = { 0, 0, 0 };	// mean, sig, gam
  int given_boundary_type[2] ={ 0, 0 };


  // Parameter list ------------------------------------------------------------
  varlist = ut_alloc_2d_char (15, 15);
  varqty = 0;
  strcpy (varlist[++varqty], "mean");
  strcpy (varlist[++varqty], "x");
  strcpy (varlist[++varqty], "k");
  strcpy (varlist[++varqty], "sigma");
  strcpy (varlist[++varqty], "y");
  strcpy (varlist[++varqty], "expression");
  strcpy (varlist[++varqty], "gamma");
  strcpy (varlist[++varqty], "frominclusive");
  strcpy (varlist[++varqty], "toinclusive");
  strcpy (varlist[++varqty], "fromexclusive");
  strcpy (varlist[++varqty], "toexclusive");

  ut_string_function_separate (string, &fct, &vars, &vals, &inputqty);

  // read in parameters
  for (i = 0; i < inputqty; ++i)
  {
    if (vars[i])
    {
      status = ut_string_comp (vars[i], varlist, varqty, &param);
    }
    else
      status = -1;

    if (status == 0)	// read parameters by keyword
    {
      if (!strcmp (param, "mean") || !strcmp (param, "x")
          || !strcmp (param, "k"))
      {
        sscanf (vals[i], "%lf", &mean);
        input_switch[0] = 1;
      }

      else if (!strcmp (param, "sigma") || !strcmp (param, "y"))
      {
        sscanf (vals[i], "%lf", &sig);
        input_switch[1] = 1;
      }

      else if (!strcmp (param, "expression"))
        ut_string_string (vals[i], &expr);

      else if (!strcmp (param, "gamma"))
      {
        sscanf (vals[i], "%lf", &gam);
        input_switch[2] = 1;
      }
      else if (!strcmp (param, "frominclusive"))
      {
        sscanf (vals[i], "%lf", &from);
        given_boundary_type[0] = 1;
      }
      else if (!strcmp (param, "toinclusive"))
      {
        sscanf (vals[i], "%lf", &to);
        given_boundary_type[1] = 1;
      }
      else if (!strcmp (param, "fromexclusive"))
      {
        sscanf (vals[i], "%lf", &from);
        given_boundary_type[0] = 2;
      }
      else if (!strcmp (param, "toexclusive"))
      {
        sscanf (vals[i], "%lf", &to);
        given_boundary_type[1] = 2;
      }
    }		// if(status != 0), no keyword recognised
    else	// read parameter by position
      /* order of arguments:
       * for fcts with a gamma: [mean,sigma,gamma,frominclusive,toinclusive,fromtype,totype]
       * for lognormal:         [mean,sigma,frominclusive,toinclusive,fromtype,totype,expr]
       * fo all others:         [mean,sigma,frominclusive,toinclusive,fromtype,totype]
       */
    {
      switch (i)
      {
      case 0:	// mean
        sscanf (vals[0], "%lf", &mean);
        input_switch[0] = 1;
        break;
      case 1:	// sigma
        sscanf (vals[1], "%lf", &sig);
        input_switch[1] = 1;
        break;
      case 2:	// frominclrusive OR gamma
        if (!strcmp (fct, "dirac") || !strcmp (fct, "normal")
        || !strcmp (fct, "beta") || !strcmp (fct, "lognormal")
        || !strcmp (fct, "lorentzian") || !strcmp (fct, "studentst")
        || !strcmp (fct, "weibull"))
        {
          sscanf (vals[2], "%lf", &from);
          given_boundary_type[0] = 1;
        }
        else
        {
          sscanf (vals[2], "%lf", &gam);
          input_switch[2] = 1;
        }
        break;
      case 3:	// toinclusive OR frominclusive
        if (!strcmp (fct, "dirac") || !strcmp (fct, "normal")
        || !strcmp (fct, "beta") || !strcmp (fct, "lognormal")
        || !strcmp (fct, "lorentzian") || !strcmp (fct, "studentst")
        || !strcmp (fct, "weibull"))
        {
          sscanf (vals[3], "%lf", &to);
          given_boundary_type[1] = 1;
        }
        else
        {
          sscanf (vals[3], "%lf", &from);
          given_boundary_type[0] = 1;
        }
        break;
      case 4:	// fromtype OR toinclusive
        if (!strcmp (fct, "dirac") || !strcmp (fct, "normal")
        || !strcmp (fct, "beta") || !strcmp (fct, "lognormal")
        || !strcmp (fct, "lorentzian") || !strcmp (fct, "studentst")
        || !strcmp (fct, "weibull"))
          sscanf (vals[4], "%d", &given_boundary_type[0]);
        else
        {
          sscanf (vals[4], "%lf", &to);
          given_boundary_type[1] = 1;
        }
        break;
      case 5:	// totype OR fromtype
        if (!strcmp (fct, "dirac") || !strcmp (fct, "normal")
        || !strcmp (fct, "beta") || !strcmp (fct, "lognormal")
        || !strcmp (fct, "lorentzian") || !strcmp (fct, "studentst")
        || !strcmp (fct, "weibull"))
          sscanf (vals[5], "%d", &given_boundary_type[1]);
        else
          sscanf (vals[5], "%d", &given_boundary_type[0]);
        break;
      case 6:	// expr OR totype
        if (!strcmp (fct, "lognormal"))
          ut_string_string (vals[6], &expr);
        else
          sscanf (vals[6], "%d", &given_boundary_type[1]);
      }
    }
  }

  // Check whether input correct ---------------------------------------------

  // dirac function only requires mean to be provided
  if (!strcmp (fct, "dirac"))
  {
    if (input_switch[0] > 0)	// if mean given
      input_switch[1] = 1;	// mark sigma as "given"
    else
    {
      ut_print_message (2, 0,
			"Check distribution function input: mean must be provided!\n");
      abort ();
    }
  }

  // makes sure that at least the required parameters were given (mean & sigma)
  if (input_switch[0] < 1 || input_switch[1] < 1)	// if mean or sigma NOT given
  {
    ut_print_message (2, 0,
		      "Check distribution function input: mean and sigma must be provided!\n");
    abort ();
  }

  // sigma must be > 0
  if (sig <= 0)
  {
    ut_print_message (2, 0, "Please enter a sigma > 0!\n");
    abort ();
  }

  // makes sure that gamma has a value assigned
  if (input_switch[2] == 0)	// if gamma yet unassigned
    gam = !strcmp (fct, "pseudovoigt") ? 0.5
      : !strcmp (fct, "moffat") || !strcmp (fct, "breitwigner") ? 1
      : !strcmp (fct, "pearson7") ? 1.5 : sig;

  status = 0;
  if (!strcmp (fct, "dirac"))
    ut_fct_set_dirac (pFct, mean, 1);
  else if (!strcmp (fct, "normal"))
    ut_fct_set_normal (pFct, mean, sig, given_boundary_type[0],
		       given_boundary_type[1], from, to);
  else if (!strcmp (fct, "lognormal"))
    ut_fct_set_lognormal (pFct, mean, sig, expr, given_boundary_type[0],
			  given_boundary_type[1], from, to);
  else if (!strcmp (fct, "lorentzian"))
    ut_fct_set_lorentzian (pFct, mean, sig, given_boundary_type[0],
			   given_boundary_type[1], from, to);
  else if (!strcmp (fct, "studentst"))
    ut_fct_set_studentst (pFct, mean, sig, given_boundary_type[0],
			  given_boundary_type[1], from, to);
  else if (!strcmp (fct, "pseudovoigt"))
    ut_fct_set_pseudovoigt (pFct, mean, sig, gam, given_boundary_type[0],
			    given_boundary_type[1], from, to);
  else if (!strcmp (fct, "moffat"))
    ut_fct_set_moffat (pFct, mean, sig, gam, given_boundary_type[0],
		       given_boundary_type[1], from, to);
  else if (!strcmp (fct, "pearson7"))
    ut_fct_set_pearson7 (pFct, mean, sig, gam, given_boundary_type[0],
			 given_boundary_type[1], from, to);
  else if (!strcmp (fct, "breitwigner"))
    ut_fct_set_breitwigner (pFct, mean, sig, gam, given_boundary_type[0],
			    given_boundary_type[1], from, to);
  else if (!strcmp (fct, "expnormal"))
    ut_fct_set_expnormal (pFct, mean, sig, gam, given_boundary_type[0],
			  given_boundary_type[1], from, to);
  else if (!strcmp (fct, "skewnormal"))
    ut_fct_set_skewnormal (pFct, mean, sig, gam, given_boundary_type[0],
			   given_boundary_type[1], from, to);
/*  else if (!strcmp (fct, "donaich"))
    ut_fct_set_donaich (pFct, mean, sig, gam, given_boundary_type[0],
			given_boundary_type[1], from, to);
*/  else if (!strcmp (fct, "beta"))
    ut_fct_set_beta (pFct, mean, sig, given_boundary_type[0],
		     given_boundary_type[1], from, to);
  else if (!strcmp (fct, "weibull"))
    ut_fct_set_beta (pFct, mean, sig, given_boundary_type[0],
		     given_boundary_type[1], from, to);
  else
  {
    status = -1;
    abort ();
  }

  ut_free_1d_char (fct);
  ut_free_1d_char (expr);
  ut_free_1d_char (param);
  ut_free_2d_char (vars, inputqty);
  ut_free_2d_char (vals, inputqty);
  ut_free_2d_char (varlist, varqty);

  return status;
}

void
ut_fct_interval_x_linear (double min, double max, int size,
                          char* method, struct FCT *pFct)
{
  int i;

  (*pFct).x = ut_realloc_1d ((*pFct).x, size);
  (*pFct).min = min;
  (*pFct).max = max;
  (*pFct).size = size;

  if (!method || !strcmp (method, "center"))
    for (i = 0; i < (*pFct).size; i++)
      (*pFct).x[i] = min + (max - min) * (2 * i + 1) / (2 * (*pFct).size);
  else if (!strcmp (method, "bounds"))
    for (i = 0; i < (*pFct).size; i++)
      (*pFct).x[i] = min + i * (max - min) / ((*pFct).size - 1);
  else
    abort ();

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
ut_fct_add (struct FCT *Fct1, int qty, double *fact, char *expr,
	    struct FCT *pFct2)
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
  fprintf (fp, "Fct.gamma = %f\n", Fct.gamma);
  fprintf (fp, "Fct.type_from = %d\n", Fct.type_from);
  fprintf (fp, "Fct.type_to = %d\n", Fct.type_to);
  fprintf (fp, "Fct.from = %f\n", Fct.from);
  fprintf (fp, "Fct.to = %f\n", Fct.to);
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

#endif
