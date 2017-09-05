/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_1d_useg (double cl1, double cl2, double clt,
				 double pl, int *pqty, double **pcoo,
				 double **pcl);

extern void nem_meshing_1d_useg_reverse (int ptqty, double *coo, double *cl);

extern int cl1_cl_pl_n (double cl1, double cl, double pl);
extern int cl1_pl_x_i (double cl1, double pl, double x);
extern double cl1_pl_n_l (double cl1, double pl, double n);
extern double cl1_cl2_pl_I (double cl1, double cl2, double pl);
extern double cl1_pl_i_x (double cl1, double pl, int i);
extern double cl1_pl_i_cl (double cl1, double pl, int i);
