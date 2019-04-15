/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<math.h>
#include"neut.h"

extern int neut_mesh_elt2d_fod (struct NODES Nodes, struct MESH Mesh,
				int elt, double ***pfodeq, int *pfodqty,
				int *pfod, int *pdir);

extern int neut_mesh_elt1d_eod (struct NODES Nodes, struct MESH Mesh, int elt,
				double ***peodeq, int *peodqty, int *peod, int
				*pdir);
