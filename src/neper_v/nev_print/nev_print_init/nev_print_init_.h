/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include"ut.h"
#include"../../structIn_v.h"
#include"neut_v.h"

#include "nev_print_init.h"

extern void nev_print_init_step (struct PRINT Print, struct SIM *pSim);

extern void nev_print_init_data (struct IN_V In, struct PRINT, struct TESS, struct DATA *,
                                 struct TESR, struct DATA *,
                                 struct NODES, struct MESH *, struct POINT *,
                                 int, struct DATA *, struct DATA **,
                                 struct DATA *, struct DATA *, char **);

extern void nev_print_init_data_csys (struct IN_V In, struct DATA *);
extern void nev_print_init_data_nodes (struct IN_V In, struct NODES Nodes, char *crysym, int Qty, struct DATA *pData);
extern void nev_print_init_data_mesh (struct IN_V In, struct NODES Nodes, struct MESH, char *crysym, double size, int Qty,
                                      char *entity, int dim, struct DATA *);
extern void nev_print_init_data_tesr (struct IN_V In, struct TESR, struct DATA *);
extern void nev_print_init_data_tess (struct IN_V In, struct TESS, struct DATA *);
extern void nev_print_init_data_points (struct IN_V In, struct POINT, char *crysym, struct DATA *);
extern void nev_print_init_data_crystal (struct PRINT Print, struct POINT, struct DATA *);

extern void nev_print_init_show (struct TESS Tess, struct TESR Tesr,
                                 struct NODES Nodes, struct MESH *Mesh,
                                 int SQty, struct POINT *Point, int,
                                 struct PRINT *pPrint);

extern void nev_print_init_camera (struct IN_V In, struct TESS, struct TESR, struct NODES,
                                   struct MESH *, struct POINT *, int, struct
                                   DATA, struct PRINT *);
extern void nev_print_init_camera_coo (double **bbox, double *centre, double *v, char *expr,
                                       double *coo);
extern void nev_print_init_camera_coo_tess (struct TESS Tess, char *expr,
                                            double *coo);
extern void nev_print_init_camera_coo_mesh (struct NODES Nodes, struct MESH
				      Mesh, char *expr, double *coo);
extern void nev_print_init_camera_coo_tesr (struct TESR Tesr, char *expr,
                                            double *coo);
extern void nev_print_init_camera_coo_points (struct POINT Point, char *expr,
                                              double *coo);
extern void nev_print_init_camera_lookat (char *expr, struct TESS Tess,
                                          struct NODES Nodes, struct MESH Mesh,
                                          struct PRINT *pPrint);
extern void nev_print_init_camera_v (int dim, double *v);
extern void nev_print_init_camera_sky (char *expr, int dim, double *coo);

extern void nev_print_init_light (struct IN_V In, struct TESS Tess, struct TESR Tesr, struct MESH *Mesh, struct PRINT *pPrint);

extern void nev_print_init_pf (struct IN_V In, struct TESS Tess, struct TESR Tesr, struct POINT *Points, int PointQty, struct PF *pPf);
