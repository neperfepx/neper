/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"
#include"../../../net_utils/net_utils.h"

#include"net_reg_merge_del_ff/net_reg_merge_del_ff.h"
#include"net_reg_merge_del_per/net_reg_merge_del_per.h"

#include"net_reg_merge_del.h"

extern int net_reg_merge_del_test (struct TESS Tess, int edge, int delcell,
                                   int verbosity);
extern int net_reg_merge_del_test_link (struct TESS Tess, int edge);
extern int net_reg_merge_del_testfold (struct TESS Tess, int ver, double maxangle);
extern int net_reg_merge_del_test_tet (struct TESS Tess, int poly, int edge);
extern int net_reg_merge_del_delpoly (struct TESS *pTess, int poly, int verbosity);
extern int net_reg_merge_del_isvermaster (struct TESS Tess, int ver, int master);
extern int net_reg_merge_del_testbuf (struct TESS Tess, int **buf);
extern void net_reg_merge_del_domfacefaceqty (struct TESS Tess, int **pqty);
extern int net_reg_merge_del_constraints_compatible (double **constraint,
                                                     int M);
extern void net_reg_merge_del_optimcoo (struct TESS *pTess, int ver,
                                        int *vers, int **offsets, int verqty,
                                        int periodic, double target,
                                        double *pff);
extern void net_reg_merge_del_buffer (struct TESS Tess, int *edges, int edgeqty, int *vers, int verqty, int **buf, struct TESS *pTessBuf);
extern void net_reg_merge_del_buffer_add (int **buffer, int *vals, int valqty);

extern void UpdateEdgeState (struct TESS *, int);
extern void SearchDelNNewVer (struct TESS *, int, int *, int *);
extern void UpdateEdgeVerNb (struct TESS *, int, int, int);
extern void UpdateEdgeLength (struct TESS *, int);
extern int UpdateFaceVerNEdge (struct TESS *, int, int, int, int);
extern void UpdateFaceState (struct TESS *, int, int);
extern int UpdateFaceVer (struct TESS *, int, int, int);
extern int UpdateFaceEdge (struct TESS *, int, int);
extern int net_reg_merge_delFromFace (struct TESS *, int, int, int);
extern int DeleteVerFromFace (struct TESS *, int, int, int);
extern int ReplaceVerInFace (struct TESS *, int, int, int);
extern int DeleteFace (struct TESS *, int, int, int);
extern int DeleteFaceFromItsPoly (struct TESS *, int);
extern void SearchNewNOldEdges (struct TESS *, int, int, int *, int *);
extern void net_edgedel_edgedom_fromverdom (struct TESS *, int);
extern void net_tess_reg_ver_facedom (struct TESS *, int);
extern int UpdateVer (struct TESS *, int, int, int, int);
extern void UpdateVerState (struct TESS *, int, int, int);
extern void UpdateVerEdge (struct TESS *, int, int, int, int);
extern int UpdateVerBound (struct TESS *, int, int);
extern int UpdateVerCoo (struct TESS *, int, int, int *, double **, int, int);
extern int UpdateVerCooBary (struct TESS *, int, int, int);
extern int UpdateVerCooMiniFF (struct TESS *, int, int *, double **, int, int);
