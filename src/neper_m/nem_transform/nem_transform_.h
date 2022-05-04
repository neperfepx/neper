/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include"structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_transform.h"
#include"../nem_smoothing/nem_smoothing.h"
#include"neper_t/net_ori/net_ori.h"

extern void nem_transform_smooth (char *smooth, struct TESS Tess,
                                  struct NODES *pNodes, struct MESH *Mesh);

extern void nem_transform_explode (char *explode, struct NODES *pNodes,
                                   struct MESH *Mesh);

extern void nem_transform_slice (char *explode, struct NODES *pNodes,
                                 struct MESH *Mesh);

extern void nem_transform_node (char *node, struct NODES *pNodes);
extern void nem_transform_ori (char *ori, struct TESS Tess, struct NODES Nodes, struct MESH *Mesh);
