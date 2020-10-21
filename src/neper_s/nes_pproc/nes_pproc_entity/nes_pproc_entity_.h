/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>

#include"ut.h"
#include"orilib.h"
#include"neut.h"
#include"structIn_s.h"

#include"nes_pproc_entity.h"

extern void nes_pproc_entity_pre (struct IN_S In, struct SIM Sim, char *entity,
                                  int *pentityqty, char **pdir, char ***pinres,
                                  int *pinresqty);

extern void nes_pproc_entity_file (struct SIM *pSim, char *entity, char *dir, char *res, char *expr);

extern void nes_pproc_entity_subres (struct SIM *pSim, char *entity,
                                     int entityqty, char *dir, char *res);

extern void nes_pproc_entity_expr (struct SIM *pSim, struct TESS Tess,
                                   struct NODES *pNodes, struct MESH *Mesh,
                                   char *entity, int entityqty, char *dim,
                                   char *res, char *expr);

extern void nes_pproc_entity_remove (struct SIM *pSim, char *entity, char *dir, char *result);
