/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern int nem_mesh_3d_gmsh (struct TESS, int, struct NODES, struct MESH *,
			     double, double, double, char *, char *, double, double,
			     struct NODES *, struct MESH *, double *,
			     struct timeval *, double *);

// argument for a worker process calling the Gmsh library
#define NEM_GMSH_WORKER_ARG "--gmsh-worker"

extern int nem_gmsh_worker (void);
// mesh size factors of the face projections (see nem_mesh_gmsh_sizefact)
#define NEM_GMSH_SIZEFACT_NONE 0
#define NEM_GMSH_SIZEFACT_STEREO 1
#define NEM_GMSH_SIZEFACT_TORUS 2
#define NEM_GMSH_SIZEFACT_PARMQTY 9

extern int nem_mesh_gmsh_lib_2d (int algo, int recombine, double rnd, double clmax,
                                 double clgrad, double **coos,
                                 double *cls, int qty, int facttype,
                                 double *factparms, double allowed_t,
                                 struct timeval *pctrlc_t, struct NODES *pN,
                                 struct MESH *pM);
extern double nem_mesh_gmsh_sizefact (int type, double *parms, double *coo);
extern double nem_mesh_gmsh_tor_w (double R, double r, double v);
extern double nem_mesh_gmsh_tor_v (double R, double r, double w);
extern int nem_mesh_gmsh_lib_3d (int algo, int opti, double rnd,
                                 double clmax, double clgrad,
                                 struct NODES SkinNodes,
                                 struct MESH Skin, double allowed_t,
                                 struct timeval *pctrlc_t, struct NODES *pN,
                                 struct MESH *pM);
extern void nem_mesh_gmsh_lib_stop (void);

extern void nem_mesh_3d_gmsh_localskin (struct NODES Nodes, struct MESH Skin,
                                        struct NODES *pSkinNodes,
                                        struct MESH *pSkinLoc);
extern double nem_mesh_gmsh_sizeratio (struct NODES BNodes, double cl,
                                       double clgrad, struct NODES N,
                                       struct MESH M);

// calibration of the scale factor of the mesh size map passed to Gmsh (see
// nem_mesh_gmsh_sizecalib_next)
struct NEM_SIZECALIB
{
  int iter;
  double alpha;                 // current scale factor
  double prevalpha, prevr;      // previous scale factor and size ratio
  double slope;                 // estimated slope (log-log)
  double alphamin, alphamax;    // bracket
  double besterr;               // best error, |r - 1|
};

extern void nem_mesh_gmsh_sizecalib_init (struct NEM_SIZECALIB *pCalib);
extern int nem_mesh_gmsh_sizecalib_next (struct NEM_SIZECALIB *pCalib,
                                         double r, double clreps, int *pbest);
