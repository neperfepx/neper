/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OL_GEN
#define OL_GEN

#include "ol_des.h"
#include "ol_glo.h"

extern int  ol_label_g (char *, double **);

extern void ol_nb_e (double, double, double, double *);
extern void ol_nb_e_rad (double, double, double, double *);
extern void ol_srand_e_rad (int, int, double **);
extern void ol_srand_e (int, int, double **);

extern void ol_nb_r (double, double, double *);
extern void ol_nb_max_theta_rad (double, double, double *);
extern void ol_nb_max_theta (double, double, double *);
extern void ol_nb_max_theta_mis_rad (double, double, double *);
extern void ol_nb_max_theta_mis (double, double, double *);
extern void ol_nb_max_rtheta (double, double, double, double, double *,
			      double *);
extern void ol_nb_max_rtheta_mis_rad (double, double, double, double,
				      double *, double *);
extern void ol_nb_max_rtheta_mis (double, double, double, double, double *,
				  double *);
extern void ol_nb_max_rtheta_rad (double, double, double, double, double *,
				  double *);

#endif /* OL_GEN */
#ifndef OL_GENx
#define OL_GENx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#endif /* OL_GENx */
#ifndef OL_GENx
#define OL_GENx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#include "ol_glo.h"
#include "ol_des.h"
#endif /* OL_GENx */
#ifdef __cplusplus
}
#endif
