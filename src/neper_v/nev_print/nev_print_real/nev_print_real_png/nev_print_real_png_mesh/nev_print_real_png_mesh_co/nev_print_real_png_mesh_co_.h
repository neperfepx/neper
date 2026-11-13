/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_real_png_mesh_co.h"
#include "../../nev_print_real_png.h"

extern void nev_print_real_png_mesh_co_compress (struct PRINT Print, struct MESH MeshCo,
                                                 struct DATA NodeData, struct NODES *pN,
                                                 struct MESH *pM2D, int **pelt2deltco,
                                                 int **pedges, int *pedgeqty);

extern void nev_print_real_png_mesh_co_print_faces (FILE * file, struct PRINT Print,
                                                    struct NODES N, struct MESH M2D,
                                                    int *elt2deltco, struct DATA MeshDataCo);

extern void nev_print_real_png_mesh_co_print_edges (FILE * file, struct PRINT Print,
                                                    struct NODES N, int *edges,
                                                    int edgeqty, struct DATA MeshDataCo);

extern int nev_print_real_png_mesh_co_edge_cmp (const void *a, const void *b);

extern void nev_print_real_png_mesh_co_seq (struct MESH MeshCo, int *pvernodeqty,
                                            int ***pseqtri, int *ptriqty, int ***pseqedge,
                                            int *pedgeqty, int *prefedgeqty);
