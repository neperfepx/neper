/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_1d_useg (double cl1, double cl2, double clt,
				 double pl, int *pqty, double **pcoo,
				 double **pcl);

extern void nem_meshing_1d_useg_reverse (int ptqty, double *coo, double *cl);
