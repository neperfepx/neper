/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_1d_useg_pieces (double cl1, double cl2, double clt,
                                        double clgrad, double *x, double *a,
                                        double *b, double *F, int *pqty);
extern double nem_meshing_1d_useg_piece_int (double a, double b, double u,
                                             double v);
extern double nem_meshing_1d_useg_piece_intinv (double a, double b, double u,
                                                double dF);
extern double nem_meshing_1d_useg_size (double cl1, double cl2, double clt,
                                        double clgrad, double x);
extern void nem_meshing_1d_useg_place (double *x, double *a, double *b,
                                       double *F, int pieceqty, int n,
                                       double *coo);
extern double nem_meshing_1d_useg_sizeratio (double cl1, double cl2,
                                             double clt, double clgrad,
                                             double *coo, int n);
