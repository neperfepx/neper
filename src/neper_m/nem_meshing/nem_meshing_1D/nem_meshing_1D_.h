/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

#include"../../structIn_m.h"
#include"neut_m.h"
#include"ut.h"
#include"orilib.h"

#include"nem_meshing_1D.h"

extern void EdgeMeshing (struct TESS, int, double, double, struct
			 NODES, double *, struct NODES *, struct MESH *);
extern void nem_mesh_1d_unitsegment (double cl1, double cl2, double clt,
				     double pl, int *pqty, double **pcoo,
				     double **pcl);
