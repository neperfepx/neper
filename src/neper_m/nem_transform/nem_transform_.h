/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include"structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_transform.h"
#include"../nem_smoothing/nem_smoothing.h"

extern void nem_transform_smooth (char *smooth, struct TESS Tess,
                                  struct NODES *pNodes, struct MESH *Mesh);
