/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_camera.h"

extern void nev_camera_lookat (char *expr, struct TESS Tess,
			       struct NODES Nodes, struct MESH Mesh,
			       struct PRINT *pPrint);

extern void nev_camera_tess_expr_coo (struct TESS Tess, char *expr,
				      double *coo);
extern void nev_camera_mesh_expr_coo (struct NODES Nodes, struct MESH
				      Mesh, char *expr, double *coo);
extern void nev_camera_tesr_expr_coo (struct TESR Tesr, char *expr,
				      double *coo);
extern void nev_camera_point_expr_coo (struct POINT Point, char *expr,
				       double *coo);
extern void nev_camera_expr_coo (double **bbox, double *centre, double *v, char *expr,
				 double *coo);
extern void nev_camera_v (int dim, double *v);

extern void nev_camera_expr_sky (char *expr, double *coo);
