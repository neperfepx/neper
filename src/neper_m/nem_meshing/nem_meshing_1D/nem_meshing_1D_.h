/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

#include"../../structIn_m.h"
#include"neut_m.h"
#include"ut.h"
#include"orilib.h"
#include "nem_meshing_1D_useg/nem_meshing_1D_useg.h"

#include"nem_meshing_1D.h"

extern void nem_meshing_1D_pre (struct MESHPARA *, struct TESS,
                                struct NODES RNodes, struct MESH *Mesh,
                                struct NODES, double **);

extern void nem_meshing_1D_edge (struct MESHPARA, struct TESS,
                                 struct NODES RNodes, struct MESH *Mesh,
                                 int, double, double,
                                 struct NODES, double *, struct NODES *,
                                 struct MESH *);

extern void nem_meshing_1D_edge_algo (struct TESS, struct NODES, struct MESH*,
                                      int, double, double, struct NODES,
                                      double *, struct NODES *, struct MESH *);

extern void nem_meshing_1D_edge_algo_mesh (int, int, int, double, double, double,
                                           struct NODES, double *, struct NODES
                                           *, struct MESH *);

extern void nem_meshing_1D_edge_per (struct TESS Tess, struct NODES *N,
                                     struct MESH *M, struct NODES *pN,
                                     struct MESH *pM, int **pmaster_id,
                                     int edge);

extern void nem_mesh_1d_unitsegment (double cl1, double cl2, double clt,
				     double pl, int *pqty, double **pcoo,
				     double **pcl);

extern void nem_meshing_1D_edge_projontomesh (struct TESS Tess, int edge,
                                         struct NODES N0, struct MESH M0,
                                         struct NODES *pN, struct MESH M);

extern void nem_meshing_1D_edge_projontodomain (struct TESS Tess, int edge,
                                                struct NODES *pN, struct MESH M);

extern void nem_meshing_1D_edge_record (struct TESS Tess, int edge, struct NODES N,
                                        struct MESH M, int *master_id,
                                        struct NODES *pNodes, int**,
                                        struct MESH *Mesh);

extern void nem_meshing_1D_edge_record_nodes (struct TESS Tess, int edge,
                                              struct NODES N, int *master_id,
                                              int **N_global_id,
                                              struct NODES *pNodes);

extern void nem_meshing_1D_edge_record_elts (int edge, struct MESH M,
                                             int *N_global_id, struct MESH *Mesh);

extern void nem_meshing_1D_post (struct NODES *pNodes, struct MESH *Mesh,
                                 int Node0DQty, double *NodeCl);

extern void nem_meshing_1d_edge_proj_fixcl (struct MESH M, struct NODES *pN);
