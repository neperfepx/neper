/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<float.h>

#include"../../structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_meshing_0d.h"

extern void nem_meshing_0d_ver (struct TESS, int, struct MESHPARA,
				struct NODES *, struct MESH *);
extern void nem_meshing_0d_addvermesh (struct NODES N, struct MESH M,
				       int master, int *shift,
				       struct NODES *pNodes,
				       struct MESH *Mesh);
