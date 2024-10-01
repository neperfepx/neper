/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<float.h>

#include"neut_t.h"

#include"net_transform_tesr_tesrmerge.h"

extern void net_transform_tesr_tesrmerge_init (char *string, struct TESR *pTesr, struct TESR **pTesrs,
                                               int **pCellId, int ***pCellState, int *pCellQty,
                                               char **pstrategy);
extern void net_transform_tesr_tesrmerge_init_alloc (struct TESR *Tesrs, struct TESR *pTesr);
extern void net_transform_tesr_tesrmerge_copycell (struct TESR Tesr2, int i, struct TESR *pTesr);
extern void net_transform_tesr_tesrmerge_mergecell (struct TESR *Tesrs, int *cells,
                                                    struct TESR *pTesr);

extern int net_transform_tesr_tesrmerge_inonetesr (struct TESR *Tesrs, int *CellId, int CellQty,
                                                   int **CellState, struct TESR *pTesr);
extern int net_transform_tesr_tesrmerge_inbothtesrs (struct TESR *Tesrs, int *CellId, int CellQty,
                                                    int **CellState, struct TESR *pTesr);

extern void net_transform_tesr_tesrmerge_selectcells (struct TESR *Tesrs,
                                          char *string, int *CellIds,
                                          int **CellStates, int CellQty,
                                          int **pcells, int **ptesrs,
                                          int *pcellqty);

extern void net_transform_tesr_tesrmerge_copycells (struct TESR *Tesrs, int *cells,
                                        int *tesrs, int cellqty, struct TESR *pTesr);
extern void net_transform_tesr_tesrmerge_mergecells (struct TESR *Tesrs, int *cells,
                                        int *tesrs, int cellqty, struct TESR *pTesr);

extern void net_transform_tesr_tesrmerge_printcellfound (int cellqty);

extern void net_transform_tesr_tesrmerge_direct (struct TESR *Tesrs, int *CellIds,
                                     int **CellStates, int CellQty,
                                     struct TESR *pTesr);

extern void net_transform_tesr_tesrmerge_smart (struct TESR *Tesrs, int *CellIds,
                                     int **CellStates, int CellQty,
                                     struct TESR *pTesr);
