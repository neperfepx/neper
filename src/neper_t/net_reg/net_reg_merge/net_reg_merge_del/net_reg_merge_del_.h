/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../../../structIn_t.h"
#include"ut.h"
#include"neut_t.h"
#include"../../../net_utils/net_utils.h"

#include"net_reg_merge_del_ff/net_reg_merge_del_ff.h"

#include"net_reg_merge_del.h"

extern void net_reg_merge_del_buffer (struct TESS Tess, int edge,
				      int **buf, struct TESS *pTessBuf);

extern void UpdateEdgeState (struct TESS *, int);
extern void SearchDelNNewVer (struct TESS *, int, int *, int *);
extern void UpdateEdgeVerNb (struct TESS *, int, int, int);
extern void UpdateEdgeLength (struct TESS *, int);
extern int UpdateFaceVerNEdge (struct TESS *, int, int, int, int);
extern void UpdateFaceState (struct TESS *, int, int);
extern void UpdateFaceVer (struct TESS *, int, int, int);
extern int UpdateFaceEdge (struct TESS *, int, int);
extern void net_reg_merge_delFromFace (struct TESS *, int, int, int);
extern void DeleteVerFromFace (struct TESS *, int, int, int);
extern void ReplaceVerInFace (struct TESS *, int, int, int);
extern int DeleteFace (struct TESS *, int, int, int);
extern void DeleteFaceFromItsPoly (struct TESS *, int);
extern void SearchNewNOldEdges (struct TESS *, int, int, int *, int *);
extern void net_edgedel_edgedom_fromverdom (struct TESS *, int);
extern void net_tess_reg_ver_facedom (struct TESS *, int);
extern int UpdateVer (struct TESS *, int, int, int, int);
extern void UpdateVerState (struct TESS *, int, int, int);
extern void UpdateVerEdge (struct TESS *, int, int, int, int);
extern int UpdateVerBound (struct TESS *, int, int);
extern int UpdateVerCoo (struct TESS *, int, int, int);
extern int UpdateVerCooBary (struct TESS *, int, int);
extern int UpdateVerCooMiniFF (struct TESS *, int, int);
