/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"
#include"net_ori/net_ori.h"

#include"../../net_utils/net_utils.h"
#include"../net_tess3d/net_tess3d_periodic/net_tess3d_periodic.h"

#include"net_tess_cube.h"

extern void net_tess_cube_general (struct IN_T In, struct TESS *Tess, int dtess, int dcell,
                                   int TessId, struct TESS *pTess);
extern void net_tess_cube_ids (int *N, int ***verid, int ****edgeid,
			       int ****faceid, int ***polyid);
extern void net_tess_cube_cells (int *N, struct TESS *pTess);
extern void net_tess_cube_sset (struct TESS Tess, struct SEEDSET *pSSet);
extern void net_tess_cube_vers (int *N, double **bbox, int ***verid,
				struct TESS *pTess);
extern void net_tess_cube_edges (int *N, int ***verid, int ****edgeid,
				 struct TESS *pTess);
extern void net_tess_cube_faces (int *N, int ***verid, int ****edgeid,
				 int ****faceid, struct TESS *pTess);
extern void net_tess_cube_polys (int *N, int ****faceid, int ***polyid,
				 struct TESS *pTess);

extern void net_tess_cube_facepoly (int *N, int ****faceid,
				    struct TESS *pTess);

extern void net_tess_cube_vers_per (int *N, int ***verid, struct TESS *pTess);
extern void net_tess_cube_edges_per (int *N, int ****edgeid, struct TESS *pTess);
extern void net_tess_cube_faces_per (int *N, int ****faceid, struct TESS *pTess);
