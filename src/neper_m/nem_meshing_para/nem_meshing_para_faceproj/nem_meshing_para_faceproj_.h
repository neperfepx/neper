/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>

#include"ut.h"
#include"orilib.h"
#include"neut_m.h"
// #include"../../nem/nem.h"

#include"nem_meshing_para_faceproj.h"

extern int nem_mesh2d_face_nproj (struct TESS,
				  struct NODES, struct MESH *, int, double *);
