/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_data_tess.h"

extern struct DATA* nev_data_tess_init (struct TESS Tess, struct DATA *TessData,
                                       char **pentity, char *prop, char *argument,
			               int *pid, int *pentityqty,
			               char **ptype, char **pvalue);

extern void nev_data_tess_col (struct SIM Sim,
                                      struct TESS Tess,
                                      char *entity, int id,
				      int entityqty, char *type,
				      char *value,
                                      struct DATA *TessData);

extern void nev_data_tess_col_col (int entityqty, char *value,
                                          struct DATA *TessData);

extern void nev_data_tess_col_scal (char *input, struct TESS *Tess, struct NODES *pNodes,
                                           struct MESH **pMesh, char *entity,
                                           int entityqty, char *value,
                                           struct DATA *TessData);
