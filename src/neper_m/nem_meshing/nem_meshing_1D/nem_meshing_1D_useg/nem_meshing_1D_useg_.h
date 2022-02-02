/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"

#include"nem_meshing_1D_useg.h"

extern int cl1_pl_x_i (double cl1, double pl, double x);
extern double cl1_pl_n_l (double cl1, double pl, double n);
extern double cl1_cl2_pl_I (double cl1, double cl2, double pl);
extern double cl1_pl_i_x (double cl1, double pl, int i);
extern double cl1_pl_i_cl (double cl1, double pl, int i);
