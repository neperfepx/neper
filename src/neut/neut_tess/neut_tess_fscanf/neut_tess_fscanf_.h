/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

extern void neut_tess_fscanf_head (FILE *, struct TESS *, char **pversion);
extern void neut_tess_fscanf_foot (FILE *);
extern void neut_tess_fscanf_cell (FILE *, struct TESS *, char *version);
extern void neut_tess_fscanf_ver (FILE *, struct TESS *);
extern void neut_tess_fscanf_edge (FILE *, struct TESS *);
extern void neut_tess_fscanf_face (FILE *, struct TESS *);
extern void neut_tess_fscanf_poly (FILE *, struct TESS *);
extern void neut_tess_fscanf_domain (FILE *, struct TESS *, char *version);
extern void neut_tess_fscanf_domain_vers (FILE *, struct TESS *);
extern void neut_tess_fscanf_domain_edges_v2p0 (FILE *, struct TESS *);
extern void neut_tess_fscanf_domain_edges_v3p3 (FILE *, struct TESS *);
extern void neut_tess_fscanf_domain_edges_v3p5 (FILE *, struct TESS *);
extern void neut_tess_fscanf_domain_faces_v2p0 (FILE *, struct TESS *);
extern void neut_tess_fscanf_domain_faces_v3p3 (FILE *, struct TESS *);
extern void neut_tess_fscanf_per (FILE *, struct TESS *);
extern void neut_tess_fscanf_per_gen (FILE *, struct TESS *);
extern void neut_tess_fscanf_per_seed (FILE *, struct TESS *);
extern void neut_tess_fscanf_per_face (FILE *, struct TESS *);
extern void neut_tess_fscanf_per_edge (FILE *, struct TESS *);
extern void neut_tess_fscanf_per_ver (FILE *, struct TESS *);
extern void neut_tess_fscanf_scale (FILE *, struct TESS *);
extern void neut_tess_fscanf_scale_gen (FILE *, struct TESS *);
extern void neut_tess_fscanf_scale_cellid (FILE *, struct TESS *);
