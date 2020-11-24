/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef HAVE_GSL
#include<gsl/gsl_interp.h>

#ifndef UT_FCT_H
#define UT_FCT_H
/// \file ut_fct.h
/// \brief Function-related functions
/// \author Romain Quey
/// \bug No known bugs
/// \todo Merge init_interp into alloc?

/// \brief FCT structure
  struct FCT
  {
    char *type;

    double mean;                // mean (or expectation) of distribution x
    double sigma;               // standard deviation x
    double gamma;               // extra parameter for some functions

    // flags for x value restriction (0, 1, 2: none, inclusive, exclusive)
    char type_from;
    char type_to;

    double from;                // lower x value limit
    double to;                  // upper x value limit
    double area;

    double min;
    double max;
    int size;
    double *x;
    double *y;
    gsl_interp *interp;
    gsl_interp_type *interp_type;
    gsl_interp_accel *interp_accel;
  };
  typedef struct FCT FCT;

/// \brief Set a function to 0
/// \param *pFct: function
  extern void ut_fct_set_zero (struct FCT *pFct);

/// \brief Unallocate a function
/// \param *pFct: function
  extern void ut_fct_free (struct FCT *pFct);

/// \brief Initialize interpolation of a function
/// \param *pFct: function
  extern void ut_fct_init_interp (struct FCT *pFct);

/// \brief Copy a function
/// \param Fct: input function
/// \param *pFct2: output function
  extern void ut_fct_memcpy (struct FCT Fct, struct FCT *pFct2);

/// \brief Get the width of a bin
/// \param Fct: input function
/// \param bin: bin
/// \return bin width
  extern double ut_fct_binwidth (struct FCT Fct, int bin);

/// \brief Scale a function
/// \param Fct: input function
/// \param factor: scaling factor
/// \return bin width
  extern void ut_fct_scale (struct FCT *pFct, double factor);

/// \brief Add functions
/// \param Fcts: functions
/// \param fctnb: number of functions
/// \param weights: functions weights
/// \param *pFct2: output function
/// \note \c weights can be \c NULL
/// \return bin width
  extern void ut_fct_add (struct FCT *Fcts, int fctnb, double *weights,
                          struct FCT *pFct2);

/// \brief Evaluate a function
/// \param Fct: function
/// \param x: function variable
/// \return function value
  extern double ut_fct_eval (struct FCT Fct, double x);

/// \brief Set a function from an expression
/// \param expr: expression
/// \param *pFct: function
/// \return 0 on success and -1 on failure
/// \note Aborts on failure
  extern int ut_fct_set (char *expr, struct FCT *pFct);

/// \brief Set a function to a Dirac distribution
/// \param *pFct: function
/// \param x: value where the Dirac is
/// \param area: function area
  extern void ut_fct_set_dirac (struct FCT *pFct, double x, double area);

/// \brief Set a function to a normal distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_normal (struct FCT *pFct, double mean, double sigma,
                                 char type_from, char type_to, double from,
                                 double to);

/// \brief Set a function to a lognormal distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_lognormal (struct FCT *pFct, double mean,
                                    double sigma, char type_from,
                                    char type_to, double from, double to);

/// \brief Set a function to a Lorentzian distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_lorentzian (struct FCT *pFct, double mean,
                                     double sigma, char type_from,
                                     char type_to, double from, double to);

/// \brief Set a function to a studentst distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_studentst (struct FCT *pFct, double mean,
                                    double sigma, char type_from,
                                    char type_to, double from, double to);

/// \brief Set a function to a pseudo-Voigt distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma:
/// \param gamma:
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_pseudovoigt (struct FCT *pFct, double mean,
                                      double sigma, double gamma,
                                      char type_from, char type_to,
                                      double from, double to);

/// \brief Set a function to a Moffat distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma:
/// \param gamma:
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_moffat (struct FCT *pFct, double mean, double sigma,
                                 double gamma, char type_from, char type_to,
                                 double from, double to);

/// \brief Set a function to a Pearson7 distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma:
/// \param gamma:
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_pearson7 (struct FCT *pFct, double mean,
                                   double sigma, double gamma, char type_from,
                                   char type_to, double from, double to);

/// \brief Set a function to a Breit-Wigner distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma:
/// \param gamma:
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_breitwigner (struct FCT *pFct, double mean,
                                      double sigma, double gamma,
                                      char type_from, char type_to,
                                      double from, double to);

/// \brief Set a function to an expnormal distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma:
/// \param gamma:
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_expnormal (struct FCT *pFct, double mean,
                                    double sigma, double gamma,
                                    char type_from, char type_to, double from,
                                    double to);

/// \brief Set a function to a skew-normal distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma:
/// \param gamma:
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_skewnormal (struct FCT *pFct, double mean,
                                     double sigma, double gamma,
                                     char type_from, char type_to,
                                     double from, double to);

/// \brief Set a function to a beta distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_beta (struct FCT *pFct, double mean, double sigma,
                               char type_from, char type_to, double from,
                               double to);

/// \brief Set a function to a Weibull distribution
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
/// \param type_from: 'i' if included and 'e' if excluded
/// \param type_to: 'i' if included and 'e' if excluded
/// \param from: start value
/// \param to: end value
  extern void ut_fct_set_weibull (struct FCT *pFct, double mean, double sigma,
                                  char type_from, char type_to, double from,
                                  double to);

/// \brief Set a function to an erf function
/// \param *pFct: function
/// \param mean: mean
/// \param sigma: standard deviation
  extern void ut_fct_set_erf (struct FCT *pFct, double mean, double sigma);

/// \brief Set a function from an array
/// \param *pFct: function
/// \param array: array
/// \param size: size of the array
/// \param xmin: first value along x
/// \param step: grid step along x
  extern void ut_fct_set_array (struct FCT *pFct, double *array, int size, double xmin, double step);

/// \brief Set a function to the numerical type
/// \param *pFct: function
/// \param min: min
/// \param max: max
/// \param size: size
/// \param sigma: standard deviation
/// \param method: \c center for interprating \c min and \c max as bin centers, and \c bounds for interprating \c min and \c max as bin extreme bounds
  extern void ut_fct_set_numerical (struct FCT *pFct, double min, double
                                    max, int size, char *method);

/// \brief Convert an analytical function into a numerical function
/// \param *pFct: function
/// \param min: min along x-axis (bin center)
/// \param max: max along x-axis (bin center)
/// \param size: size
/// \param *pFct2: numerical function
  extern void ut_fct_numericalfct (struct FCT Fct, double min, double max,
                                   int size, struct FCT *pFct2);

/// \brief Convert an analytical function into a numerical function based on an expression
/// \param *pFct: function
/// \param expr: x-axis expression, of the type \c regular(min,max,size)
/// \param *pFct2: numerical function
  extern void ut_fct_numericalfct_expr (struct FCT Fct, char *expr,
                                        struct FCT *pFct2);

/// \brief Get the convolution of a function
/// \param Fct: function
/// \param sigma: normal convolution kernel standard deviation
/// \param *pFct2: convoluted function
  extern void ut_fct_convolution (struct FCT Fct, double sigma,
                                  struct FCT *pFct2);

/// \brief Get the integral of a function
/// \param Fct: function
/// \param *pFct2: integral function
  extern void ut_fct_integralfct (struct FCT Fct, struct FCT *pFct2);

/// \brief Set the x axis of a numerical function
/// \param *pFct: function
/// \param min: min
/// \param max: max
/// \param size: size
/// \param method: \c center for interprating \c min and \c max as bin centers, and \c bounds for interprating \c min and \c max as bin extreme bounds
  extern void ut_fct_set_xaxis (struct FCT *pFct, double min, double max,
                                int size, char *method);

/// \brief Print debugging information of a function
/// \param file: file
/// \param Fct: function
  extern void ut_fct_debug (FILE * fp, struct FCT Fct);

/// \brief Get the bin of an x coordinate
/// \param Fct: function
/// \param x: min
/// \return position
  extern int ut_fct_x_bin (struct FCT Fct, double x);

#endif                          /* UT_FCT_H */

#endif

#ifdef __cplusplus
}
#endif
