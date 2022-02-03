/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"
#include"orilib.h"
#include"structIn_s.h"

#include"nes_pproc_entity/nes_pproc_entity.h"
#include"../nes_utils/nes_utils.h"

#include"nes_pproc.h"

extern void nes_pproc_load (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes, struct MESH *Mesh);

extern void nes_pproc_load_crysym (struct TESS *pTess, char **GroupCrySym, int GroupQty);
