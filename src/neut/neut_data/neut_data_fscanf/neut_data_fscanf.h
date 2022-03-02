/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_DATA_FSCANF_H
#define NEUT_DATA_FSCANF_H

extern void neut_data_fscanf_ori (struct SIM Sim, char *entity, char
    *datavalue, int qty, double **dataembed, double ***pColData, char
    **pColDataType, int *pColDataSize);

extern void neut_data_fscanf_ori_tesr (struct SIM Sim, struct TESR Tesr,
                                      char *datavalue, int qty,
                                      double ****dataembedvox,
                                      double **dataembedcell,
                                      double ***pColData);

/*
extern void neut_data_fscanf_scal (struct SIM Sim, char *value, int qty, double **dataembed,
                                  double ***pColData);
                                  */

extern void neut_data_fscanf_general (struct DATAINPUT DataInput,
                                      char *entity, int dim, int entityqty,
                                      char *attribute, char *type,
                                      char *value, struct DATA *pData);

extern void neut_data_fscanf_length (int entityqty, char *type, char *value, struct DATA *Data);

extern void neut_data_fscanf_label (char *value, struct DATA *pData);

extern void neut_data_fscanf_coo (struct SIM Sim, char *entity, int entityqty, char *type,
                                 char *value, struct DATA *pData);

#endif				/* NEUT_DATA_FSCANF_H */

#ifdef __cplusplus
}
#endif
