/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

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
