/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

// default radius factors (see nev_print_defrad)
#ifndef NEV_DEFRAD_VER
#define NEV_DEFRAD_VER        0.02000
#define NEV_DEFRAD_EDGE       0.01414
#define NEV_DEFRAD_SEED       0.04000
#define NEV_DEFRAD_ELT2DEDGE  0.01000
#define NEV_DEFRAD_ELT3DEDGE  0.00707
#define NEV_DEFRAD_NODE       0.1
#define NEV_DEFRAD_DOMAIN     2        // factor on the edge radius
#endif

extern void nev_print_symbol_expr_params (double rad, char *expr, char *symbol, double *params);
extern void nev_print_cube_expr_params (double rad, char *expr, double *params);
extern void nev_print_square_expr_params (double rad, char *expr, double *params);
extern void nev_print_cyl_expr_params (double rad, char *expr, double *params);
extern void nev_print_arr_expr_params (double rad, char *expr, double *params);
extern void nev_print_tor_expr_params (double rad, char *expr, double *params);
extern void nev_print_disc_expr_params (double rad, char *expr, double *params);
extern void nev_print_ellipsoid_expr_params (double rad, char *expr, double *params);
extern void nev_print_generic_expr_params (double rad, char *expr, double *params);

extern void nev_print_scale_ticks (char *scale, double *pmin, double *pmax,
                                   char ***pticks, double **ptickvals, int *ptickqty,
                                   char **ptickformat);

extern double nev_print_defrad (double size, int cellqty, double fact);
extern int nev_print_tess_defrad (struct TESS Tess, int dim, double *prad);
extern void nev_print_mesh_size_cellqty (struct NODES Nodes, struct MESH *Mesh,
                                         double *psize, int *pcellqty);
extern int nev_print_mesh_defrad (double size, int cellqty, int dim, double *prad);
extern int nev_print_mesh_defedgerad (double size, int cellqty, int dim, char *space,
                                      double *prad);
extern double nev_print_nodes_defrad (int nodeqty);
extern double nev_print_domain_defrad (struct TESS Tess);
extern void nev_print_rad_default (char *expr, double defrad, char **pexpr);
extern void nev_print_rad_vars (char *expr, int varqty, char **vars, double *vals,
                                char **pexpr);
