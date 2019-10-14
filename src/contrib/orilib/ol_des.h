/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OL_DES
#define OL_DES

#include "ut.h"
#include "ol_glo.h"
#include "ol_cal.h"

/*
// below (gsl include and FCT) is a hack to get the code to compile
#include<gsl/gsl_interp.h>

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
*/

extern int ol_des_size (char*);

extern double* ol_e_alloc (void);
extern void ol_e_free (double *);
extern void ol_e_set_zero (double *);
extern void ol_e_set_id (double *);
extern void ol_e_set_this (double *, double, double, double);
extern void ol_e_memcpy (double *, double *);
extern void ol_g_e (double **, double *);
extern void ol_e_g (double *, double **);
extern void ol_e_g_rad (double *, double **);
extern void ol_g_e_rad (double **, double *);
extern void ol_e_e (double *, double *);
extern void ol_e_e_rad (double *, double *);
extern void ol_e_deg2rad (double *, double *);
extern void ol_e_rad2deg (double *, double *);
extern int ol_e_fscanf (FILE *, double *);
extern int ol_e_fprintf (FILE *, double *, const char *);

extern void ol_e_R (double *, double *);
extern void ol_e_R_rad (double *, double *);
extern void ol_e_rtheta (double *, double *, double *);
extern void ol_e_rtheta_rad (double *, double *, double *);
extern void ol_e_r (double *, double *);
extern void ol_e_theta (double *, double *);
extern void ol_e_theta_rad (double *, double *);

extern void ol_e_er (double*, double*);
extern void ol_e_ek (double*, double*);
extern void ol_er_e (double*, double*);
extern void ol_ek_e (double*, double*);
extern void ol_e_er_rad (double*, double*);
extern void ol_e_ek_rad (double*, double*);
extern void ol_er_e_rad (double*, double*);
extern void ol_ek_e_rad (double*, double*);

extern double** ol_g_alloc (void);
extern void ol_g_free (double **);
extern void ol_g_set_zero (double **);
extern void ol_g_set_id (double **);
extern void ol_g_set_this (double **, double, double, double, double, double,
			   double, double, double, double);
extern void ol_g_memcpy (double **, double **);
extern int ol_g_fscanf (FILE *, double **);
extern int ol_g_fprintf (FILE *, double **, char *);

extern int* ol_m_alloc (void);
extern void ol_m_free (int *);
extern void ol_m_g (int *, double **);
extern void ol_g_m (double **, char*, int, int, int **, double *, int *);
extern void ol_g_m_quality (double **, char*, int, int, double, int **,
			    double *, int *);
extern void ol_e_m (double *, char*, int, int, int **, double *, int *);
extern void ol_e_m_quality (double *, char*, int, int, double, int **,
			    double *, int *);
extern void ol_rtheta_m (double *, double, char*, int, int, int **, double *, int *);
extern void ol_rtheta_m_quality (double *, double, char*, int, int, double, int **,
				 double *, int *);
extern void ol_R_m (double *, char*, int, int, int **, double *, int *);
extern void ol_R_m_quality (double *, char*, int, int, double, int **,
			    double *, int *);
extern void ol_q_m (double *, char*, int, int, int **, double *, int *);
extern void ol_q_m_quality (double *, char*, int, int, double, int **,
			    double *, int *);
extern void ol_m_memcpy (int *, int *);
extern int ol_m_fscanf (FILE *, int *);
extern int ol_m_fprintf (FILE *, int *, char *);
extern void ol_m_set_zero (int *);
extern void ol_m_set_id (int *);
extern void ol_m_set_this (int *, int, int, int, int, int, int);
extern void ol_m_e (int *, double *);
extern void ol_m_e_rad (int *, double *);
extern void ol_m_q (int *, double *);
extern void ol_m_rtheta (int *, double *, double *);
extern void ol_m_rtheta_rad (int *, double *, double *);
extern void ol_m_r (int *, double *);
extern void ol_m_theta (int *, double *);
extern void ol_m_theta_rad (int *, double *);
extern void ol_m_R (int *, double *);

extern int* ol_pole_alloc (void);
extern int* ol_hpole_alloc (void);
extern double* ol_vect_alloc (void);
extern double* ol_p_alloc (void);
extern void ol_pole_free (int *);
extern void ol_hpole_free (int *);
extern void ol_vect_free (double *);
extern void ol_p_free (double *);
extern int ol_pole_fprintf (FILE *, int *, char *);
extern int ol_pole_fscanf (FILE *, int *);
extern int ol_hpole_fprintf (FILE *, int *, char *);
extern int ol_hpole_fscanf (FILE *, int *);
extern int ol_vect_fprintf (FILE *, double *, char *);
extern int ol_vect_fscanf (FILE *, double *);
extern int ol_p_fprintf (FILE *, double *, char *);
extern int ol_p_fscanf (FILE *, double *);
extern void ol_pole_vect (int *, double *);
extern void ol_hpole_vect (int *, double *);
extern void ol_vect_stproj (double *, double *);
extern void ol_vect_stproj_rad (double *, double *);
extern void ol_vect_stprojxy (double *, double *);
extern void ol_eaprojxy_vect (double *, double *);
extern void ol_vect_eaproj (double *, double *);
extern void ol_vect_eaprojxy (double *, double *);
extern void ol_vect_eaproj_rad (double *, double *);
extern void ol_g_pf_stproj (double **, double *, double *);
extern void ol_g_pf_stproj_rad (double **, double *, double *);
extern void ol_g_pf_stprojxy (double **, double *, double *);
extern void ol_g_pf_eaproj (double **, double *, double *);
extern void ol_g_pf_eaprojxy (double **, double *, double *);
extern void ol_g_pf_eaproj_rad (double **, double *, double *);
extern void ol_g_ipf_stproj (double **, double *, double *);
extern void ol_g_ipf_stproj_rad (double **, double *, double *);
extern void ol_g_ipf_stprojxy (double **, double *, double *);
extern void ol_g_ipf_eaproj (double **, double *, double *);
extern void ol_g_ipf_eaprojxy (double **, double *, double *);
extern void ol_g_ipf_eaproj_rad (double **, double *, double *);
extern void ol_g_ipf_crysym_stproj (double **, double *, char*, double *);
extern void ol_g_ipf_crysym_stproj_rad (double **, double *, char*, double *);
extern void ol_g_ipf_crysym_stprojxy (double **, double *, char*, double *);
extern void ol_g_ipf_crysym_eaproj (double **, double *, char*, double *);
extern void ol_g_ipf_crysym_eaprojxy (double **, double *, char*, double *);
extern void ol_g_ipf_crysym_eaproj_rad (double **, double *, char*, double *);
extern void ol_ipf_g_stprojxy (double *p, double *vect, double **g);

extern void ol_q_pf_stproj (double *, double *, double *);
extern void ol_q_pf_stprojxy (double *, double *, double *);
extern void ol_q_pf_stproj_rad (double *, double *, double *);
extern void ol_q_pf_eaproj (double *, double *, double *);
extern void ol_q_pf_eaprojxy (double *, double *, double *);
extern void ol_q_pf_eaproj_rad (double *, double *, double *);
extern void ol_q_ipf_stproj (double *, double *, double *);
extern void ol_q_ipf_stprojxy (double *, double *, double *);
extern void ol_q_ipf_stproj_rad (double *, double *, double *);
extern void ol_q_ipf_eaproj (double *, double *, double *);
extern void ol_q_ipf_eaprojxy (double *, double *, double *);
extern void ol_q_ipf_eaproj_rad (double *, double *, double *);
extern void ol_q_ipf_crysym_stproj (double *, double *, char*, double *);
extern void ol_q_ipf_crysym_stprojxy (double *, double *, char*, double *);
extern void ol_q_ipf_crysym_stproj_rad (double *, double *, char*, double *);
extern void ol_q_ipf_crysym_eaproj (double *, double *, char*, double *);
extern void ol_q_ipf_crysym_eaprojxy (double *, double *, char*, double *);
extern void ol_q_ipf_crysym_eaproj_rad (double *, double *, char*, double *);
extern void ol_ipf_q_stprojxy (double *p, double *vect, double *q);

extern void ol_pole_set_zero (int *);
extern void ol_pole_set_this (int *, int, int, int);
extern void ol_pole_memcpy (int *, int *);
extern void ol_vect_set_zero (double *);
extern void ol_vect_set_this (double *, double, double, double);
extern void ol_vect_memcpy (double *, double *);
extern void ol_p_set_zero (double *);
extern void ol_p_set_this (double *, double, double);
extern void ol_p_memcpy (double *, double *);

extern void ol_stproj_vect_rad (double *, double *);
extern void ol_eaproj_vect_rad (double *, double *);
extern void ol_stproj_vect (double *, double *);
extern void ol_eaproj_vect (double *, double *);
extern void ol_stprojxy_vect (double *, double *);
extern void ol_eaprojxy_vect (double *, double *);

extern int ol_vect_ipfweight (double *, double *);

extern double* ol_q_alloc (void);
extern void ol_q_free (double *);
extern int ol_q_fscanf (FILE *, double *);
extern int ol_q_fprintf (FILE *, double *, char *);
extern void ol_q_rtheta (double *, double *, double *);
extern void ol_q_rtheta_rad (double *, double *, double *);
extern void ol_q_r (double *, double *);
extern void ol_q_theta (double *, double *);
extern void ol_q_theta_rad (double *, double *);
extern void ol_q_r (double *, double *);
extern void ol_q_theta (double *, double *);
extern void ol_q_theta_rad (double *, double *);
extern void ol_rtheta_q (double *, double, double *);
extern void ol_rtheta_q_rad (double *, double, double *);
extern void ol_q_R (double *, double *);
extern void ol_g_q (double **, double *);
extern void ol_q_g (double *, double **);
extern void ol_e_q (double *, double *);
extern void ol_q_e (double *, double *);
extern void ol_e_q_rad (double *, double *);
extern void ol_q_e_rad (double *, double *);
extern void ol_q_memcpy (double *, double *);
extern void ol_q_q (double *, double *);
extern void ol_q_set_zero (double *);
extern void ol_q_set_id (double *);
extern void ol_q_set_unit (double *);
extern void ol_q_set_this (double *, double, double, double, double);

extern double* ol_R_alloc (void);
extern void ol_R_free (double *);
extern int ol_R_fscanf (FILE *, double *);
extern int ol_R_fprintf (FILE *, double *, char *);
extern void ol_g_R (double **, double *);
extern void ol_R_g (double *, double **);
extern void ol_rtheta_R (double *, double, double *);
extern void ol_rtheta_R_rad (double *, double, double *);
extern void ol_R_rtheta (double *, double *, double *);
extern void ol_R_rtheta_rad (double *, double *, double *);
extern void ol_R_r (double *, double *);
extern void ol_R_theta (double *, double *);
extern void ol_R_theta_rad (double *, double *);
extern void ol_R_memcpy (double *, double *);
extern void ol_R_set_zero (double *);
extern void ol_R_set_id (double *);
extern void ol_R_set_this (double *, double, double, double);
extern void ol_R_e (double *, double *);
extern void ol_R_e_rad (double *, double *);
extern void ol_R_q (double *, double *);

extern double* ol_homochoric_alloc (void);
extern void ol_homochoric_free (double *);
extern int ol_homochoric_fscanf (FILE *, double *);
extern int ol_homochoric_fprintf (FILE *, double *, char *);
extern void ol_g_homochoric (double **, double *);

#ifdef HAVE_GSL
extern void ol_homochoric_thetafct (struct FCT *phfct);
extern void ol_homochoric_g (struct FCT *, double *, double **);
extern void ol_homochoric_rtheta (struct FCT *, double *, double *, double *);
extern void ol_homochoric_rtheta_rad (struct FCT *, double *, double *, double *);
extern void ol_homochoric_theta (struct FCT *, double *, double *);
extern void ol_homochoric_theta_rad (struct FCT *, double *, double *);
extern void ol_homochoric_e (struct FCT *, double *, double *);
extern void ol_homochoric_e_rad (struct FCT *, double *, double *);
extern void ol_homochoric_q (struct FCT *, double *, double *);
#endif // HAVE_GSL

extern void ol_rtheta_homochoric (double *, double, double *);
extern void ol_rtheta_homochoric_rad (double *, double, double *);
extern void ol_homochoric_r (double *, double *);
extern void ol_homochoric_memcpy (double *, double *);
extern void ol_homochoric_set_zero (double *);
extern void ol_homochoric_set_id (double *);
extern void ol_homochoric_set_this (double *, double, double, double);

extern double* ol_r_alloc (void);
extern void ol_r_free (double *);
extern int ol_rtheta_fscanf (FILE *, double *, double *);
extern int ol_r_fscanf (FILE *, double *);
extern int ol_theta_fscanf (FILE *, double *);
extern int ol_rtheta_fprintf (FILE *, double *, double, char *);
extern int ol_r_fprintf (FILE *, double *, char *);
extern int ol_theta_fprintf (FILE *, double, char *);
extern int ol_r_fprintf (FILE *, double *, char *);
extern int ol_theta_fprintf (FILE *, double, char *);
extern int ol_g_rtheta (double **, double *, double *);
extern int ol_g_rtheta_rad (double **, double *, double *);
extern int ol_g_r (double **, double *);
extern void ol_g_theta (double **, double *);
extern void ol_g_theta_rad (double **, double *);
extern void ol_rtheta_g (double *, double, double **);
extern void ol_rtheta_g_rad (double *, double, double **);
extern void ol_r_memcpy (double *, double *);
extern void ol_rtheta_memcpy (double *, double, double *, double *);
extern void ol_r_set_zero (double *);
extern void ol_r_set_id (double *);
extern void ol_r_set_unit (double *);
extern void ol_r_set_this (double *, double, double, double);
extern void ol_rtheta_set_zero (double *, double *);
extern void ol_rtheta_set_id (double *, double *);
extern void ol_rtheta_set_this (double *, double *, double, double, double,
				double);
extern void ol_rtheta_rtheta (double *, double, double *, double *);
extern void ol_rtheta_rtheta_rad (double *, double, double *, double *);
extern void ol_theta_deg2rad (double, double *);
extern void ol_theta_rad2deg (double, double *);

extern void ol_rtheta_e (double *, double, double *);
extern void ol_rtheta_e_rad (double *, double, double *);


#endif /* OL_DES */
#ifndef OL_DESx
#define OL_DESx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#endif /* OL_DESx */
#ifndef OL_DESx
#define OL_DESx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#include "ol_glo.h"
#include "ol_cal.h"
#endif /* OL_DESx */
#ifdef __cplusplus
}
#endif
