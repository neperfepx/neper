/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_print_init.h"

#include "nev_camera/nev_camera.h"

extern void nev_print_init_data (struct PRINT, struct TESS, struct DATA *,
                                 struct TESR, struct DATA *,
                                 struct NODES, struct MESH *, struct POINT,
                                 struct DATA *, struct DATA *,
                                 struct DATA *, struct DATA *);

extern void nev_print_init_data_csys (struct PRINT Print, struct DATA *);
extern void nev_print_init_data_nodes (struct PRINT Print, struct NODES Nodes, int Qty, struct DATA *pData);
extern void nev_print_init_data_mesh (struct PRINT Print, struct MESH, double size, int Qty,
                                      int dim, struct DATA *);
extern void nev_print_init_data_tesr (struct PRINT Print, struct TESR, struct DATA *);
extern void nev_print_init_data_tess (struct PRINT Print, struct TESS, struct DATA *);
extern void nev_print_init_data_points (struct PRINT Print, struct POINT, struct DATA *);
extern void nev_print_init_data_crystal (struct PRINT Print, struct POINT, struct DATA *);

extern void nev_print_init_show (struct TESS Tess, struct TESR Tesr,
                                 struct NODES Nodes, struct MESH *Mesh,
                                 int SQty, struct POINT Point,
                                 struct PRINT *pPrint);

extern void nev_print_init_camera (struct TESS, struct TESR, struct NODES,
                                   struct MESH *, struct POINT, struct
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
