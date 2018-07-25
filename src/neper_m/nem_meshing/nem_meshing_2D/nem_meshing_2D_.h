/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>
#include<ctype.h>
#include<unistd.h>

#ifdef HAVE_muparser
#include<muparser.h>
#endif

#include"ut.h"
#include"neut_m.h"
#include"../../structIn_m.h"
#include"../../nem/nem.h"

#include"nem_meshing_2D.h"

extern void nem_meshing_2D_progress (struct MULTIM Multim, int face, int
				     faceqty, char *message);
extern int nem_meshing_2D_face (struct IN_M, struct MESHPARA, struct MULTIM *,
				struct timeval *, double *,
				double *, struct TESS, struct NODES,
                                struct MESH *, struct NODES *, struct MESH *,
                                struct NODES *, struct MESH *, int);

extern int nem_meshing_2D_face_per (struct TESS Tess, struct NODES *pNodes,
                                    struct MESH *Mesh, struct NODES *pN,
                                    struct MESH *pM, int **pmaster_id,
                                    int face);

extern void nem_meshing_2D_face_algo (struct IN_M In, struct MESHPARA,
				      struct MULTIM *pMultim, int algo,
				      struct timeval *, double *pallowed_t,
				      double *pmax_elapsed_t,
				      struct TESS Tess, struct NODES RNodes,
				      struct MESH *RMesh, struct NODES Nodes,
				      struct MESH *Mesh, int face,
				      struct NODES *pN, struct MESH *pM,
				      double *pmOsize, double *pelapsed_t);

extern void nem_meshing_2D_face_proj (struct TESS Tess, struct NODES RNodes,
				      struct MESH *RMesh, int face,
				      double *face_eq, struct NODES *pN);

extern void nem_meshing_2D_face_record (struct TESS Tess, int face,
					struct NODES N, struct MESH M,
					int *master_id, struct NODES *pNodes,
					struct MESH *Mesh, struct MESHPARA);

extern void nem_meshing_2D_face_record_nodes (struct TESS Tess, int face,
					      struct NODES N, struct MESH M,
					      int *master_id, int **pnode_nbs,
					      struct NODES *pNodes,
					      struct MESH *Mesh,
					      struct MESHPARA);
extern void nem_meshing_2D_face_record_elts (int face, struct MESH M,
					     int *node_nbs,
					     struct MESH *Mesh);
