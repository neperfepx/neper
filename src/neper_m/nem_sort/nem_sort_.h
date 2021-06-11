/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include"structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_sort.h"

extern void nem_sort_nodes (struct IN_M In, struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh);
extern void nem_sort_elts (struct IN_M In, struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh);
