/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"orilib.h"
#include"neut.h"
#include"structIn_s.h"

#include"nes_pproc_elset.h"

extern void nes_pproc_elset_init_output (struct IN_S In, int elset);

extern void nes_pproc_elset_extract (struct IN_S In, struct SIM Sim, struct
                                     MESH Mesh, int elset);

extern void nes_pproc_elset_compute (int elset, char *var, int step,
                                     char *out, FILE * file);
