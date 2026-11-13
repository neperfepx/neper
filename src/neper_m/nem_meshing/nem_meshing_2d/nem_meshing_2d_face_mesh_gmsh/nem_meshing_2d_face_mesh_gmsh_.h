/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include"neper_config.h"

#ifdef HAVE_OPENMP
#include<omp.h>
#endif // HAVE_OPENMP

#include"ut.h"
#include"neut_m.h"
#include"../../../nem/nem.h"

#include"nem_meshing_2d_face_mesh_gmsh.h"

extern void nem_meshing_2d_face_mesh_gmsh_proj (struct TESS Tess, struct NODES Nodes,
                                                int face, struct MESHPARA MeshPara,
                                                int *bnodes, double ***pbnodecoos,
                                                int bnodeqty, int *pfacttype,
                                                double *factparms);



extern void nem_meshing_2d_face_mesh_gmsh_bnodes (struct NODES N, struct MESH *pM,
                                                  double **bnodecoos, int bnodeqty,
                                                  int *lbnodes);

extern void nem_meshing_2d_face_mesh_gmsh_backproj (struct TESS Tess,
                                          struct NODES RNodes,
                                          struct MESH *RMesh, int face,
                                          struct MESHPARA MeshPara,
                                          struct NODES Nodes,
                                          int *bnodes, int *lbnodes,
                                          int bnodeqty, struct NODES *pN,
                                          struct MESH M);

extern void nem_meshing_2d_face_mesh_gmsh_cyl_unroll (struct TESS Tess, int face,
                                                      struct NODES Nodes, int *bnodes,
                                                      int bnodeqty, double **bnodecoos);
extern void nem_meshing_2d_face_mesh_gmsh_cyl_roll (struct TESS Tess, int face,
                                                    struct NODES Nodes, int *bnodes,
                                                    int bnodeqty, double *point,
                                                    double *rolled);
extern void nem_meshing_2d_face_mesh_gmsh_cyl_basis (struct TESS Tess, int face,
                                                     struct NODES Nodes, int *bnodes,
                                                     int bnodeqty, double *c, double *a,
                                                     double *e1, double *e2, double *pr);
extern void nem_meshing_2d_face_mesh_gmsh_sph_stereo (struct TESS Tess, int face,
                                                      struct NODES Nodes, int *bnodes,
                                                      int bnodeqty, double **bnodecoos,
                                                      double *factparms);
extern void nem_meshing_2d_face_mesh_gmsh_sph_unstereo (struct TESS Tess, int face,
                                                        struct NODES Nodes, int *bnodes,
                                                        int bnodeqty, struct NODES *pN);
extern void nem_meshing_2d_face_mesh_gmsh_sph_basis (struct TESS Tess, int face,
                                                     struct NODES Nodes, int *bnodes,
                                                     int bnodeqty, double *c, double *e,
                                                     double *pr);
extern void nem_meshing_2d_face_mesh_gmsh_tor_unroll (struct TESS Tess, int face,
                                                      struct NODES Nodes, int *bnodes,
                                                      int bnodeqty, double **bnodecoos,
                                                      double *factparms);
extern void nem_meshing_2d_face_mesh_gmsh_tor_roll (struct TESS Tess, int face,
                                                    struct NODES Nodes, int *bnodes,
                                                    int bnodeqty, struct NODES *pN);
extern void nem_meshing_2d_face_mesh_gmsh_tor_basis (struct TESS Tess, int face,
                                                     struct NODES Nodes, int *bnodes,
                                                     int bnodeqty, double *c, double *a,
                                                     double *e1, double *e2, double *pR,
                                                     double *pr, double *prho0);
extern void nem_meshing_2d_face_mesh_gmsh_backproj_fixboundary (struct NODES Nodes,
                                                 int *bnodes, int *lbnodes,
                                                 int bnodeqty, struct NODES *pN);

extern void nem_meshing_2d_face_mesh_gmsh_coarse (struct TESS Tess, struct MESH *Mesh,
                                                  int face, int *bnodes, double **bnodecoos,
                                                  double *cls, int bnodeqty, int *poffset,
                                                  double ***pgcoos, double **pgcls,
                                                  int *pgqty);
extern void nem_meshing_2d_face_mesh_gmsh_split (double **gcoos, int gqty,
                                                 double **bnodecoos, int bnodeqty,
                                                 int offset, struct NODES *pN,
                                                 struct MESH *pM);
extern int nem_meshing_2d_face_mesh_gmsh_split_edgenode (int *gpos, int gqty,
                                                         double **bnodecoos, int bnodeqty,
                                                         int offset, int a, int b,
                                                         int **edgenodes, struct NODES *pN);
extern void nem_meshing_2d_face_mesh_gmsh_smooth (struct MESH M, int *lbnodes,
                                                  int bnodeqty, struct NODES *pN);
extern double nem_meshing_2d_face_mesh_gmsh_sjmin (struct NODES N, struct MESH M);
