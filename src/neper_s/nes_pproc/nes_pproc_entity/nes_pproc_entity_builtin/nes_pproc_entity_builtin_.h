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

#include"nes_pproc_entity_builtin.h"

extern int nes_pproc_entity_builtin_nodes (struct SIM *pSim, char *dir, char *res);

extern int nes_pproc_entity_builtin_elsets (struct SIM *pSim, struct TESS Tess,
                                         struct NODES *pNodes, struct MESH
                                         *Mesh, char *entity, char *res);

extern int nes_pproc_entity_builtin_nodes_disp (struct SIM *pSim, char *dir, char *res,
                                             struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_elsets_pre (struct SIM *pSim, struct TESS Tess,
                                              struct MESH *Mesh,
                                              char *entity, char *res,
                                              struct SIMRES *pSimRes,
                                              struct SIMRES *pSimRes2,
                                              int ***pelsets, int *pelsetqty);

extern void nes_pproc_entity_builtin_elsets_ori (struct SIM *pSim, struct TESS Tess,
                                              struct NODES *pNodes, struct MESH *Mesh,
                                              char *entity, char *res,
                                              int **elsets, int elsetqty,
                                              struct SIMRES *pSimRes,
                                              struct SIMRES *pSimRes2);

extern void nes_pproc_entity_builtin_elsets_oridis (struct SIM *pSim, struct TESS
    Tess, struct NODES *pNodes, struct MESH *Mesh, char *entity, char *res, int
    **elsets, int elsetqty, struct SIMRES *pSimRes, struct SIMRES *pSimRes2);

extern void nes_pproc_entity_builtin_elsets_odf (struct SIM *pSim, struct TESS Tess,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, struct SIMRES *pSimRes,
                                  struct SIMRES *pSimRes2);

extern void nes_pproc_entity_builtin_elsets_gen (struct SIM *pSim, struct TESS Tess,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes,
                                  struct SIMRES *pSimRes2);
