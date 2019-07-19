/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>

#include "orilib.h"
#include "neut.h"
// #include "../../neper_t/net_utils/net_utils.h"

extern void neut_tess_init_domain_domedgevernb (struct TESS *pTess);

extern void neut_tess_domface_tess_general (struct TESS Tess, struct
					    TESS *pT);
extern void neut_tess_domface_tess_seeds (struct TESS Tess, struct TESS *pT);
extern void neut_tess_domface_tess_cells (struct TESS Tess, int *oldface_newface, struct TESS *pT);
extern void neut_tess_domface_tess_faces (struct TESS Tess, int domface,
					  struct TESS *pT,
					  int *oldface_newface);
extern void neut_tess_domface_tess_edges (struct TESS Tess, int domface,
					  struct TESS *pT,
					  int *oldedge_newedge);
extern void neut_tess_domface_tess_vers (struct TESS Tess, int domface,
					 struct TESS *pT, int *oldver_newver);
extern void neut_tess_domface_tess_renumber (struct TESS *pT,
					     int *oldface_newface,
					     int *oldedge_newedge,
					     int *oldver_newver);
extern void neut_tess_domface_tess_domain (struct TESS Tess, int domface,
					   struct TESS *pT,
					   int *oldedge_newedge,
					   int *oldver_newver);

extern void neut_tess_3dcolumnar_2d_faces (struct TESS Tess, int domface,
					   struct TESS *pT);
extern void neut_tess_3dcolumnar_2d_edges (struct TESS Tess, int domface,
					   struct TESS *pT,
					   int *oldedge_newedge);
extern void neut_tess_3dcolumnar_2d_vers (struct TESS Tess, int domface,
					  int *, struct TESS *pT,
					  int *oldver_newver);
extern void neut_tess_3dcolumnar_2d_renumber (struct TESS *pT,
					      int *oldedge_newedge,
					      int *oldver_newver);
