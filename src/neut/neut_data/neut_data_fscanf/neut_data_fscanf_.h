/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<ctype.h>

#include "neut.h"
#include "neut_data_fscanf.h"

extern void neut_data_fscanf_ori_label (double **g, int qty, double ***pColData, char **pColDataType);

extern void neut_data_fscanf_ori_file (struct SIM Sim, char *datavalue, int qty, double ***pColData);

extern void neut_data_fscanf_col_col (int entityqty, char *value,
                                     struct DATA *pData);

extern void neut_data_fscanf_scal (char *input, struct SIM *pSim,
                                   struct TESS *pTess, struct TESR *pTesr,
                                   struct NODES *pNodes, struct MESH
                                   **pMesh, struct POINT *Point, char
                                   *entity, int entityqty, char *value,
                                   char **pColDataName, int *pColDataSize,
                                   double ***pColData, char **pColDataType);

extern void neut_data_fscanf_col_tensor (struct SIM Sim,
                                         char *entity, int entityqty,
                                         char *value, struct DATA *pData);
