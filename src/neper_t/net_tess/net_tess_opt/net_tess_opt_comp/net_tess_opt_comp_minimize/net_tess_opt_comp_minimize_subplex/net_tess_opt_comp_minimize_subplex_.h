/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/time.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#include"../net_tess_opt_comp_minimize_objective/net_tess_opt_comp_minimize_objective.h"

#include"net_tess_opt_comp_minimize_subplex.h"

// subplex strategy constants: minimum and maximum dimensions of the
// subspaces
#define SUBPLEX_NSMIN 2
#define SUBPLEX_NSMAX 5

// stopping criteria (as NLopt's nlopt_stopping)
struct SUBPLEX_STOP
{
  int n;                        // number of (free) variables
  double minf_max;              // stopval
  double ftol_rel, ftol_abs;
  double xtol_rel, *xtol_abs;
  int nevals, maxeval;
  double maxtime, start;
  struct TOPT *pTOpt;           // for the forced stop
};

// objective function, of the free variables
struct SUBPLEX_FUNC
{
  struct TOPT *pTOpt;
  int n;                        // number of variables
  int *free;                    // indices of the free variables
  int freeqty;                  // number of free variables
  double *x;                    // all variables
};

// a subspace and the total error of the cell of its seed
struct SUBPLEX_SUBERR
{
  int s;
  double err;
};

extern const char *net_tess_opt_comp_minimize_subplex_init (struct TOPT *pTOpt, struct SUBPLEX_FUNC *pF, struct SUBPLEX_STOP *stop, double **px, double **plb, double **pub, double **pdx);

extern void net_tess_opt_comp_minimize_subplex_post (struct TOPT *pTOpt, const char *ret, struct SUBPLEX_FUNC *pF, struct SUBPLEX_STOP *stop, double **px, double **plb, double **pub, double **pdx);

extern double net_tess_opt_comp_minimize_subplex_f (struct SUBPLEX_FUNC *pF, const double *x);

extern void net_tess_opt_comp_minimize_subplex_subspaces_rowan (int n, const double *dx, int *p, int *tmp, int *sub, int *pnsubs);
extern void net_tess_opt_comp_minimize_subplex_subspaces_seed (struct SUBPLEX_FUNC *pF, int *p, int *sub, int *pnsubs);
extern void net_tess_opt_comp_minimize_subplex_subspaces_percellerror (struct SUBPLEX_FUNC *pF, int *p, int *sub, int *pnsubs);

extern const char *net_tess_opt_comp_minimize_subplex_nldrmd (int n, struct SUBPLEX_FUNC *pF, double *xsub, int is, const int *p, const double *lb, const double *ub, double *x, double *minf, const double *xstep, struct SUBPLEX_STOP *stop, double psi, double *scratch, double *fdiff);

extern int net_tess_opt_comp_minimize_subplex_stop_ftol (const struct SUBPLEX_STOP *s, double f, double oldf);

extern int net_tess_opt_comp_minimize_subplex_stop_x (const struct SUBPLEX_STOP *s, const double *x, const double *oldx);

extern int net_tess_opt_comp_minimize_subplex_stop_evals (const struct SUBPLEX_STOP *s);

extern int net_tess_opt_comp_minimize_subplex_stop_time (const struct SUBPLEX_STOP *s);

extern int net_tess_opt_comp_minimize_subplex_stop_forced (const struct SUBPLEX_STOP *s);

extern void net_tess_opt_comp_minimize_subplex_nldrmd_sort (const double *pts, int n, int *order);

extern int net_tess_opt_comp_minimize_subplex_nldrmd_close (double a, double b);

extern int net_tess_opt_comp_minimize_subplex_nldrmd_reflectpt (int n, double *xnew, const double *c, double scale, const double *xold, const double *lb, const double *ub);

extern double net_tess_opt_comp_minimize_subplex_nldrmd_f (int n, const double *xs, struct SUBPLEX_FUNC *pF, double *xsub, int is, const int *p);

extern int net_tess_opt_comp_minimize_subplex_stop_ftol_relstop (double vold, double vnew, double reltol, double abstol);

extern void net_tess_opt_comp_minimize_subplex_subspaces_sort (int *p, int n, const double *dx, int *tmp);

extern double net_tess_opt_comp_minimize_subplex_seconds (void);

extern int net_tess_opt_comp_minimize_subplex_suberr_cmp (const void *a, const void *b);

extern int net_tess_opt_comp_minimize_subplex_nldrmd_compare (const double *pts, int n, int i, int j);
