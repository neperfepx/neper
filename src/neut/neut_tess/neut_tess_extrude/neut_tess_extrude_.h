/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"neut_tess_extrude.h"

extern void neut_tess_extrude_gen (struct TESS Tess, struct TESS *pT);
extern void neut_tess_extrude_cell (struct TESS Tess, double d,
				    struct TESS *pT);
extern void neut_tess_extrude_ver (struct TESS Tess, double d,
				   struct TESS *pT);
extern void neut_tess_extrude_edge (struct TESS Tess, struct TESS *pT);
extern void neut_tess_extrude_face (struct TESS Tess, double d,
				    struct TESS *pT);
extern void neut_tess_extrude_poly (struct TESS Tess, struct TESS *pT);
extern void neut_tess_extrude_domain (struct TESS Tess, double d, struct
				      TESS *pT);
