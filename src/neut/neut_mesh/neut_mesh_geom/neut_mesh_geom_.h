/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include"neut.h"
#include"neut_struct_nfcloud.hpp"

extern int neut_mesh_elset_points_closestelts_search (struct MESH Mesh, int
    elset, double *coo, NFTREE*, int qty, int *pelt);
