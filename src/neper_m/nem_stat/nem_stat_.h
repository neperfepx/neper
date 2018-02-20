/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<string.h>

#include"../structIn_m.h"
#include"ut.h"
#include"neut_m.h"
#include"../nem_writemesh/nem_writemesh.h"

#include"nem_stat.h"

extern void nem_stat_nodes (FILE *, char *, struct NODES, struct MESH *,
			    struct PART, struct TESS);
extern void nem_stat_elts (FILE *, int, char *, struct NODES, struct
			   MESH *, struct PART, struct TESS);
extern void nem_stat_elsets (FILE *, int, char *, struct NODES, struct
			     MESH *, struct PART, struct MESHPARA,
			     struct TESS);

extern void nem_stat_point (FILE * file, char *format,
			    struct NODES Nodes, struct MESH *Mesh,
			    struct TESS Tess, struct POINT Point);
