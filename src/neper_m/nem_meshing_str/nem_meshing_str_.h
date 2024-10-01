/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"../structIn_m.h"
#include"ut.h"
#include"neut_m.h"
#include"../nem_reconstruct/nem_reconstruct.h"

#include"nem_meshing_str.h"

extern void nem_meshing_str_tess (struct TESS Tess, struct NODES *,
                                  struct MESH *Mesh);
extern void nem_meshing_str_meshcell (char *, struct TESR *, struct MESH *,
                                      struct NODES *, struct NSET *);
