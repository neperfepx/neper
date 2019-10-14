/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OL_GND_RHO_H
#define OL_GND_RHO_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"../ut/ut.h"
#include"../ol_xpl.h"
#include"ol_config.h"

#ifdef HAVE_GLPK
#include <glpk.h>
#endif

#ifdef HAVE_NLOPT
#include <nlopt.h>
#endif

extern void ol_gnd_crys_b (struct OL_CRYS, double*);
extern void ol_gnd_nye_rho_sc (struct OL_CRYS, double**, double*);
extern void ol_gnd_nye_rho_l2 (struct OL_CRYS, double**, double*, double*);
#ifdef HAVE_GLPK
extern void ol_gnd_nye_rho_l1 (struct OL_CRYS, double**, double*, double*);
extern void ol_gnd_nye_rho_energy (struct OL_CRYS, double**, double*, double*);
extern void ol_gnd_nye_rho_simplex (struct OL_CRYS, double**, double*, double*, char*);
#endif

#ifdef HAVE_NLOPT
extern void ol_gnd_nye_rho_norm (struct OL_CRYS, double**, double*, double*);
#endif


#endif /* OL_GND_RHO_H */
#ifdef __cplusplus
}
#endif
