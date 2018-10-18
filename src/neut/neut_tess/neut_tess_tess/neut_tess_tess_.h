/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"neut_tess_tess.h"

extern void neut_tess_tess_seedcoo (struct TESS, struct TESS *);
extern void neut_tess_tess_seedweight (struct TESS, struct TESS *);
extern void neut_tess_tess_verqty (struct TESS, struct TESS *);
extern void neut_tess_tess_vercoo (struct TESS, struct TESS *);
extern void neut_tess_tess_veredge (struct TESS, struct TESS *);
extern void neut_tess_tess_verbound (struct TESS, struct TESS *);
extern void neut_tess_tess_verstate (struct TESS, struct TESS *);
extern void neut_tess_tess_edgeqty (struct TESS, struct TESS *);
extern void neut_tess_tess_edgevernb (struct TESS, struct TESS *);
extern void neut_tess_tess_edgeface (struct TESS, struct TESS *);
extern void neut_tess_tess_edgelength (struct TESS, struct TESS *);
extern void neut_tess_tess_edgestate (struct TESS, struct TESS *);
extern void neut_tess_tess_edgedel (struct TESS, struct TESS *);
extern void neut_tess_tess_edgebound (struct TESS, struct TESS *);
extern void neut_tess_tess_faceqty (struct TESS, struct TESS *);
extern void neut_tess_tess_facepoly (struct TESS, struct TESS *);
extern void neut_tess_tess_facever (struct TESS, struct TESS *);
extern void neut_tess_tess_faceedge (struct TESS, struct TESS *);
extern void neut_tess_tess_faceeq (struct TESS, struct TESS *);
extern void neut_tess_tess_facebound (struct TESS, struct TESS *);
extern void neut_tess_tess_facestate (struct TESS, struct TESS *);
extern void neut_tess_tess_facept (struct TESS, struct TESS *);
extern void neut_tess_tess_poly (struct TESS, struct TESS *);
extern void neut_tess_tess_polyqty (struct TESS, struct TESS *);
extern void neut_tess_tess_polyface (struct TESS, struct TESS *);
extern void neut_tess_tess_polystate (struct TESS, struct TESS *);
extern void neut_tess_tess_celltrue (struct TESS, struct TESS *);
extern void neut_tess_tess_cellbody (struct TESS, struct TESS *);
extern void neut_tess_tess_celllamid (struct TESS, struct TESS *);
extern void neut_tess_tess_cellmodeid (struct TESS, struct TESS *);
extern void neut_tess_tess_domaindata (struct TESS, struct TESS *);
extern void neut_tess_tess_per (struct TESS, struct TESS *);
