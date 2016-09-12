/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"

#include"../../net_utils/net_utils.h"

#include"net_tess_cube.h"

extern void net_tess_cube_general (int TessId, struct TESS *pTess);
extern void net_tess_cube_ids (int *N, int ***verid, int ****edgeid,
			       int ****faceid, int ***polyid);
extern void net_tess_cube_cells (int *N, struct TESS *pTess);
extern void net_tess_cube_sset (struct TESS Tess, struct SEEDSET *pSSet);
extern void net_tess_cube_vers (int *N, struct TESS PTess, int ***verid,
				struct TESS *pTess);
extern void net_tess_cube_edges (int *N, int ***verid, int ****edgeid,
				 struct TESS *pTess);
extern void net_tess_cube_faces (int *N, int ***verid, int ****edgeid,
				 int ****faceid, struct TESS *pTess);
extern void net_tess_cube_polys (int *N, int ****faceid, int ***polyid,
				 struct TESS *pTess);

extern void net_tess_cube_facepoly (int *N, int ****faceid,
				    struct TESS *pTess);
