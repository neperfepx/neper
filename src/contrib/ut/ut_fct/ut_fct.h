/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_GSL
#include<gsl/gsl_interp.h>

#ifndef UT_FCT_H
#define UT_FCT_H

struct FCT
{
  char* type;

  double mean;
  double sig;
  double area;
  char *expr;

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

extern void ut_fct_set_zero (struct FCT *pFct);
extern void ut_fct_free (struct FCT *pFct);
extern double ut_fct_eval (struct FCT Fct, double x);
extern void ut_fct_set_init_interp (struct FCT *pFct);
extern void ut_fct_set_dirac (struct FCT *pFct, double x, double area);
extern void ut_fct_set_normal (struct FCT *pFct, double mean, double sig);
extern void ut_fct_set_erf (struct FCT *pFct, double mean, double sig);
extern void ut_fct_set_lognormal (struct FCT *pFct, double mean, double
    sig, char *expr);
extern void ut_fct_set_numerical (struct FCT *pFct, double min, double
    max, int size, char *method);
extern void ut_fct_memcpy (struct FCT Fct, struct FCT *pFct2);
extern double ut_fct_binwidth (struct FCT Fct, int bin);
extern void ut_fct_convolution (struct FCT Fct, double sig, struct FCT *pFct2);
extern void ut_fct_integralfct (struct FCT Fct, struct FCT *pFct2);
extern double ut_fct_binwidth (struct FCT Fct, int bin);
extern void ut_fct_numericalfct (struct FCT Fct, double min, double max, int size, struct FCT *pFct2);
extern void ut_fct_numericalfct_expr (struct FCT Fct, char *expr, struct FCT *pFct2);
extern int  ut_fct_set (char *expr, struct FCT *pFct);
extern void ut_fct_interval_x_linear (double min, double max, int size, char *method,
				      struct FCT *pFct);
extern int ut_fct_scale (struct FCT *pFct, double val);
extern int ut_fct_add (struct FCT *Fct1, int qty, double *fact, char *expr, struct FCT *pFct2);
extern void ut_fct_debug (FILE *fp, struct FCT Fct);

extern int ut_fct_x_pos (struct FCT Fct, double x);

#endif /* UT_FCT_H */

#endif

#ifdef __cplusplus
}
#endif
