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

#ifdef __cplusplus
extern "C"
{
#endif

extern int nes_pproc_entity_builtin_nodes (struct SIM *pSim, struct NODES Nodes, char *dir,
                                           char *res);

extern int nes_pproc_entity_builtin_elsets (struct SIM *pSim, struct TESS *pTess,
                                         struct NODES *pNodes, struct MESH
                                         *Mesh, char *entity, char *res);

extern int nes_pproc_entity_builtin_nodes_disp (struct SIM *pSim, struct NODES Nodes,
                                             char *dir, char *res,
                                             struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_elsets_pre (struct SIM *pSim,
                                              struct MESH *Mesh,
                                              char *entity, char *res,
                                              struct SIMRES *pSimRes,
                                              int ***pelsets, int *pelsetqty);

extern void nes_pproc_entity_builtin_elsets_ori (struct SIM *pSim,
                                              struct NODES *pNodes, struct MESH *Mesh,
                                              char *entity, char *res,
                                              int **elsets, int elsetqty,
                                              struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_elsets_oridis (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_elsets_odf (struct SIM *pSim, struct TESS *pTess,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_elsets_readodf (struct SIM *pSim, struct TESS Tess,
                                  struct MESH *Mesh, char *entity, char *res,
                                  struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_cells_readodf (struct SIM *pSim, struct TESS Tess,
                                        struct TESR Tesr, char *entity,
                                        char *res, struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_elsets_gen (struct SIM *pSim,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes);

extern int nes_pproc_entity_builtin_cells (struct SIM *pSim, struct TESS *pTess,
                                            struct TESR Tesr, char *entity, char *res);
extern void nes_pproc_entity_builtin_cells_ori (struct SIM *pSim, struct TESS Tess,
                                    struct TESR Tesr, char *entity, char *res,
                                    struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_cells_gos (struct SIM *pSim, struct TESS Tess,
                                    struct TESR Tesr, char *entity, char *res,
                                    struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_cells_odf (struct SIM *pSim, struct TESS *pTess,
                                     struct TESR Tesr, char *entity, char *res,
                                     struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_cells_pre (struct SIM *pSim, struct TESS Tess,
                                  struct TESR Tesr, char *entity, char *res,
                                  struct SIMRES *pSimRes,
                                  int ***pcells, int *pcellqty);

extern void nes_pproc_entity_builtin_elsets_gos (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes);

extern void nes_pproc_entity_builtin_odf_writeconfig (struct SIMRES *pSimRes, char *name, struct ODF Odf);

#ifdef __cplusplus
}
#endif
