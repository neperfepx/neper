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

#include"nes_pproc_entity_builtin/nes_pproc_entity_builtin.h"

extern void nes_pproc_entity_pre (struct SIM *pSim, char *entity,
                                  char *entityresexpr, int *pentityqty,
                                  char **pdir, char ***presults, int *presultqty);

extern void nes_pproc_entity_remove (struct SIM *pSim, char *entity, char *result);

extern void nes_pproc_entity_file (struct SIM *pSim, char *entity, char *dir, char *res, char *expr);

extern void nes_pproc_entity_subres (struct SIM *pSim, char *entity, struct SIMRES *pSimRes);

extern int nes_pproc_entity_expr (struct SIM *pSim, struct TESS *pTess,
                                  struct TESR Tesr, struct NODES *pNodes,
                                  struct MESH *Mesh, char *entity,
                                  struct SIMRES *pSimRes);
