/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../structIn_m.h"
#include"ut.h"
#include"neut_m.h"
#include"../nem/nem.h"
#include "nem_writemesh.h"

extern void nem_writemesh_msh (struct IN_M In, struct TESS Tess, struct NODES Nodes,
                   struct MESH *Mesh, struct NSET *NSet, char *nsetlist,
                   char *fasetlist, char* format);

extern void nem_writemesh_sim (struct IN_M In, struct TESS Tess, struct NODES Nodes,
                   struct MESH *Mesh, struct NSET *NSet, char *nsetlist,
                   char *fasetlist);
