/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>

#include"ut.h"
#include"neut_v.h"
#include"neut_meshpara/neut_meshpara.h"
#include"neper_m/nem_utils/nem_utils.h"
#include"neper_m/nem_input/nem_input.h"
#include"neper_m/nem_meshing_para/nem_meshing_para.h"
#include"neper_m/nem_meshing/nem_meshing.h"
// #include"../../nefm/nefm.h"
#include "../../../nev_print_utils/nev_print_utils.h"
#include "nev_print_real_png_tess.h"
#include "../nev_print_real_png.h"

extern void nev_print_real_png_tess_header (FILE *, struct DATA);

// angle spanned by an elt of the most curved domain face, in degrees (see
// nev_print_real_png_tess_curvedmesh)
#define NEV_CURVEDMESH_ANGLE 5

extern void nem_input_options_default (struct IN_M *);

extern double nev_print_real_png_tess_curvedmesh_rmin (struct TESS Tess);

extern int nev_print_real_png_tess_curvedmesh (struct TESS Tess,
                                               struct NODES *pNodes,
                                               struct MESH *Mesh);
extern int nev_print_real_png_tess_face_iscurved (struct TESS Tess,
                                                  struct MESH *Mesh,
                                                  int face);
extern int nev_print_real_png_tess_edge_iscurved (struct TESS Tess,
                                                  struct MESH *Mesh,
                                                  int edge);
extern void nev_print_real_png_tess_face_mesh (FILE * file, struct NODES Nodes,
                                               struct MESH Mesh2D, int face,
                                               char *texture);
extern void nev_print_real_png_tess_edge_mesh (FILE * file, struct NODES Nodes,
                                               struct MESH Mesh1D, int edge,
                                               double rad, char *texture);
