/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_data_tesr.h"

extern void nev_data_tesr_cell (struct SIM Sim, struct TESR *pTesr, struct DATA *pTD,
				      char *type, char *argument);
extern void nev_data_tesr_vox (struct SIM Sim, struct TESR *pTesr, struct DATA *pTD,
                                     char *prop, char *argument);

extern void nev_data_tesr_edge (struct DATA *pData, char *attribute, char *datastring);

extern void nev_data_tesr_voidvox (struct DATA *pTD,
                                         char *type, char *argument);
extern void nev_data_tesr_voxedge (struct DATA *pTD,
                                         char *type, char *argument);

extern void nev_data_tesr_cell2vox (struct TESR Tesr, char *type, struct
				   DATA TesrDataPoly,
				   struct DATA *pTD);
