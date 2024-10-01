/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_png_mesh_1d.h"
#include "../../nev_print_png.h"

extern void nev_print_png_mesh_1d_compress (struct PRINT Print, struct TESS Tess,
                                            struct MESH *Mesh, int *hidden);

extern void nev_print_png_mesh_1d_print (FILE * file, struct PRINT Print, struct NODES Nodes, struct MESH *Mesh,
                                         struct DATA NodeData, struct DATA **MeshData, int *hidden);
