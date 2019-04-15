/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>

#include"ut.h"
#include"neut_m.h"
#include"../../structIn_m.h"
#include"../../nem/nem.h"

#include"nem_meshing_3D.h"

extern void nem_meshing_3D_progress (struct MULTIM Multim, int poly, int
				     polyqty, char *message);
extern void nem_meshing_3D_report_poly (struct MULTIM Multim, int poly);
extern int nem_meshing_3D_poly (struct IN_M, double, double, struct MULTIM *,
				struct timeval *pctrlc_t, double *,
				double *, struct TESS, struct NODES *,
				struct MESH *, struct NODES *pN, struct MESH *pM, int);
extern void nem_meshing_3D_report_poly_algo (int poly, int a,
					     struct NODES N, struct MESH M);

extern void nem_meshing_3D_poly_algo (struct IN_M In, double cl, double mesh3dclreps,
				      struct MULTIM *pMultim, int algo,
				      struct timeval *pctrlc_t,
				      double *pallowed_t,
				      double *pmax_elapsed_t,
				      struct TESS Tess, struct NODES Nodes,
				      struct MESH *Mesh, int poly,
				      struct NODES *pN, struct MESH *pM,
				      double *pmOsize, double *pelapsed_t);

extern void nem_meshing_3D_poly_record (struct TESS Tess, int poly,
					struct NODES N, struct MESH M,
					struct NODES *pNodes,
					struct MESH *Mesh);

extern void nem_meshing_3D_poly_record_nodes (struct TESS Tess, int poly,
					      struct NODES N, int **pnode_nbs,
					      struct NODES *pNodes,
					      struct MESH *Mesh);
extern void nem_meshing_3D_poly_record_elts (int poly, struct MESH M,
					     int *node_nbs,
					     struct MESH *Mesh);
