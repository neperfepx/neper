/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#ifndef OL_CAL
#define OL_CAL

#include "ol_des.h"
#include "ol_glo.h"

extern void ol_g_g_g_cur (double **, double **, double **);
extern void ol_q_q_q_cur (double *, double *, double *);
extern void ol_R_R_R_cur (double *, double *, double *);
extern void ol_g_g_g (double **, double **, double **);
extern void ol_q_q_q (double *, double *, double *);
extern void ol_R_R_R (double *, double *, double *);
extern void ol_g_g_g_ref (double **, double **, double **);
extern void ol_q_q_q_ref (double *, double *, double *);
extern void ol_R_R_R_ref (double *, double *, double *);
extern void ol_g_vect_vect (double **, double *, double *);
extern void ol_q_vect_vect (double *, double *, double *);

extern void ol_g_refcsys (double **, double **, double **);
extern void ol_q_refcsys (double *, double *, double *);

extern void ol_g_csys (double **, double **, double **);
extern void ol_q_csys (double *, double *, double *);

extern int ol_g_g_gdisori (double **, double **, char*, double **);
extern int ol_g_g_gdisori_cur (double **, double **, char*, double **);
extern int ol_g_g_gdisori_ref (double **, double **, char*, double **);
extern int ol_q_q_disori_rad (double *, double *, char*, double *);
extern int ol_q_q_disori (double *, double *, char*, double *);
extern int ol_q_q_qdisori (double *, double *, char*, double *);
extern int ol_q_q_qdisori_update (double *, double *, char*, int, double *, char *, int *);
extern int ol_q_q_qdisori_cur (double *, double *, char*, double *);
extern int ol_q_q_qdisori_ref (double *, double *, char*, double *);

extern int ol_g_g_gmisori (double **, double **, double **);
extern int ol_g_g_gmisori_cur (double **, double **, double **);
extern int ol_g_g_gmisori_ref (double **, double **, double **);
extern int ol_q_q_misori_rad (double *, double *, double *);
extern int ol_q_q_misori (double *, double *, double *);
extern int ol_g_g_misori_rad (double **, double **, double *);
extern int ol_g_g_misori (double **, double **, double *);
extern int ol_g_g_disori_rad (double **, double **, char*, double *);
extern int ol_g_g_disori (double **, double **, char*, double *);
extern int ol_q_q_qmisori (double *, double *, double *);
extern int ol_q_q_qmisori_cur (double *, double *, double *);
extern int ol_q_q_qmisori_ref (double *, double *, double *);

extern void ol_vect_vect_theta_rad (double *, double *, double *);
extern void ol_vect_vect_theta (double *, double *, double *);
extern void ol_vect_vect_rtheta_rad (double *, double *, double *, double *);
extern void ol_vect_vect_rtheta (double *, double *, double *, double *);
extern void ol_vect_vect_g (double *, double *, double **);
extern void ol_vect_vect_q (double *, double *, double *);

extern int ol_e_e_disori_rad (double *, double *, char*, double *);
extern int ol_e_e_disori (double *, double *, char*, double *);
extern int ol_e_e_gdisori (double *, double *, char*, double **);
extern int ol_e_e_gmisori (double *, double *, double **);
extern int ol_e_e_qdisori (double *, double *, char*, double *);
extern int ol_e_e_qmisori (double *, double *, double *);
extern int ol_e_e_misori_rad (double *, double *, double *);
extern int ol_e_e_misori (double *, double *, double *);
extern int ol_m_m_disori (int *, int *, char*, double *);
extern int ol_m_m_gdisori (int *, int *, char*, double **);
extern int ol_m_m_gmisori (int *, int *, double **);
extern int ol_m_m_qdisori (int *, int *, char*, double *);
extern int ol_m_m_qmisori (int *, int *, double *);
extern int ol_m_m_misori (int *, int *, double *);
extern int ol_R_R_disori_rad (double *, double *, char*, double *);
extern int ol_R_R_disori (double *, double *, char*, double *);
extern int ol_R_R_misori_rad (double *, double *, double *);
extern int ol_R_R_misori (double *, double *, double *);

extern int ol_g_g_disori_orthotropic_rad (double **, double **, char*, double *);
extern int ol_g_g_disori_orthotropic (double **, double **, char*, double *);
extern int ol_g_g_misori_orthotropic_rad (double **, double **, double *);
extern int ol_g_g_misori_orthotropic (double **, double **, double *);
extern int ol_q_q_disori_orthotropic_rad (double *, double *, char*, double *);
extern int ol_q_q_disori_orthotropic (double *, double *, char*, double *);
extern int ol_q_q_misori_orthotropic_rad (double *, double *, double *);
extern int ol_q_q_misori_orthotropic (double *, double *, double *);
extern int ol_e_e_disori_orthotropic_rad (double *, double *, char*, double *);
extern int ol_e_e_disori_orthotropic (double *, double *, char*, double *);
extern int ol_m_m_disori_orthotropic (int *, int *, char*, double *);
extern int ol_m_m_misori_orthotropic (int *, int *, double *);
extern int ol_e_e_misori_orthotropic_rad (double *, double *, double *);
extern int ol_e_e_misori_orthotropic (double *, double *, double *);
extern int ol_g_g_gdisori_orthotropic (double **, double **, char*, double **);
extern int ol_g_g_gmisori_orthotropic (double **, double **, double **);
extern int ol_q_q_qdisori_orthotropic (double *, double *, char*, double *);
extern int ol_q_q_qmisori_orthotropic (double *, double *, double *);

extern void ol_g_inverse (double **, double **);
extern void ol_q_inverse (double *, double *);
extern void ol_R_inverse (double *, double *);

extern int  ol_g_crysym (double **, char*, int, double **);
extern void ol_q_crysym (double *, char*, int, double *);
extern void ol_q_crysym_nopos (double *, char*, int, double *);
extern void ol_m_crysym (int *, char*, int, int *);
extern void ol_pole_crysym (int *, char*, int, int *);
extern double ol_g_gcrysym (double **, char*, double **);
extern double ol_q_qcrysym (double *, char*, double *);
extern double ol_rtheta_rthetacrysym (double *, double, char*, double *, double *);
extern double ol_R_Rcrysym (double *, char*, double *);
extern void ol_m_mcrysym (int *, char*, int *);
extern void ol_e_ecrysym_rad (double *, char*, double *);
extern void ol_e_ecrysym (double *, char*, double *);
extern void ol_g_refsym (double **, int, double **);
extern void ol_q_refsym (double *, int, double *);
extern void ol_R_refsym (double *, int, double *);
extern void ol_m_refsym (int *, int, int *);
extern void ol_vect_refsym (double *, int, double *);
extern void ol_e_refsym_rad (double *, int, double *);
extern void ol_e_refsym (double *, int, double *);
extern void ol_g_refsym_g (double **, int, double **);
extern void ol_q_refsym_q (double *, int, double *);
extern void ol_m_refsym_g (int *, int, double **);
extern void ol_g_refsym_monoclinic (double **, double ***, int *);
extern void ol_q_refsym_monoclinic (double *, double **, int *);
extern void ol_R_refsym_monoclinic (double *, double **, int *);
extern void ol_m_refsym_monoclinic (int *, int **, int *);
extern void ol_g_refsym_orthotropic (double **, double ***, int *);
extern void ol_q_refsym_orthotropic (double *, double **, int *);
extern void ol_R_refsym_orthotropic (double *, double **, int *);
extern void ol_e_refsym_orthotropic (double *, double **, int *);
extern void ol_g_refsym_orthotropic_all (double **, double ***);
extern void ol_q_refsym_orthotropic_all (double *, double **);
extern void ol_R_refsym_orthotropic_all (double *, double **);
extern void ol_e_refsym_orthotropic_all (double *, double **);
extern void ol_m_refsym_orthotropic (int *, int **, int *);
extern void ol_e_ecrysym_orthotropic_rad (double *, char*, double **, int *);
extern void ol_e_ecrysym_orthotropic (double *, char*, double **, int *);
extern int ol_q_qrefsym_orthotropic (double *, double *);
extern void ol_polef_polecrysym (int *, char*, int **, int *);
extern void ol_pole_polecrysym (int *, char*, int *);

extern int ol_crysym_qty (char*);
extern int ol_lauegroup_qty (char*);

extern void ol_g_lng (double**, double**);
extern void ol_q_lnq (double*, double*);
extern void ol_lnq_q (double*, double*);

#endif /* OL_CAL */
#ifndef OL_CALx
#define OL_CALx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#endif /* OL_CALx */
#ifndef OL_CALx
#define OL_CALx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#include "ol_glo.h"
#include "ol_des.h"
#endif /* OL_CALx */
