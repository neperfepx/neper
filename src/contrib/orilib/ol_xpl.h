/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OL_GND_H
#define OL_GND_H

#include "ol_map.h"

struct OL_CRYS
{
  // structure
  char* structure;
  double a;
  double nu;

  // slip systems
  size_t N;
  double** m;
  double** s;
  double** t;

  size_t gnd_size;
  double** gnd_s;
  double** gnd_t;

  char* rho_min;
};
typedef struct OL_CRYS OL_CRYS;

struct OL_GNDMAP
{
  double stepsize;
  size_t xsize;
  size_t ysize;

  unsigned int** id;
  double**** nye;
  double***  rho;
  double**   rhosum;
};
typedef struct OL_GNDMAP OL_GNDMAP;

extern struct OL_CRYS ol_crys_alloc ();
extern void ol_crys_free (struct OL_CRYS);

extern void ol_crys_set_this (struct OL_CRYS*, char*, double, double, char*);
extern int  ol_crys_set_mat (struct OL_CRYS*, char*);

extern struct OL_GNDMAP ol_gndmap_alloc (struct OL_MAP, struct OL_CRYS);
extern void ol_gndmap_free (struct OL_GNDMAP);

extern double** ol_nye_alloc (void);
extern void ol_nye_free (double**);

extern int ol_gnd_map_nye_region_method (struct OL_MAP Map, struct
    OL_CRYS Crys, double thetamin, double thetamax, double** nye,
                       unsigned int *pid, char* method);
extern int  ol_gnd_map_nye_region (struct OL_MAP, struct OL_CRYS, double
    thetamin, double thetamax, double**, unsigned int*);
extern void ol_gnd_map_nye (struct OL_MAP, struct OL_CRYS, double
    thetamin, double thetamax, struct OL_GNDMAP*);
extern void ol_gnd_map_nye_method (struct OL_MAP, struct OL_CRYS, double
    thetamin, double thetamax, struct OL_GNDMAP*, char*);

extern int  ol_gnd_nye_rho (struct OL_CRYS, double**, double*);
extern int  ol_gnd_nye_rho_log (struct OL_CRYS, double**, double*);

extern void ol_gnd_map_nye_rho (struct OL_CRYS, struct OL_GNDMAP*);
extern void ol_gnd_map_nye_rho_log (struct OL_CRYS, struct OL_GNDMAP*);

extern void ol_gnd_map_rho (struct OL_MAP, struct OL_CRYS, double
    thetamin, double thetamax, struct OL_GNDMAP*);
extern void ol_gnd_map_rho_log (struct OL_MAP, struct OL_CRYS, double
    thetamin, double thetamax, struct OL_GNDMAP*);

extern void ol_crys_ss_schmid (struct OL_CRYS Crys, double** g,
    double*** T);
extern void ol_crys_ss_schmids (struct OL_CRYS Crys, double** g,
    double*** M);

#endif /* OL_GND_H */
#ifndef OL_GNDx
#define OL_GNDx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#endif /* OL_GNDx */
#ifndef OL_GNDx
#define OL_GNDx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#include "ol_glo.h"
#include "ol_des.h"
#include "ol_cal.h"
#include "ol_map.h"
#endif /* OL_GNDx */
#ifdef __cplusplus
}
#endif
