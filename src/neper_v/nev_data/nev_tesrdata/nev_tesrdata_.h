/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include"orilib.h"

#include"../nev_data_utils/nev_data_utils.h"
#include "nev_tesrdata.h"

extern void nev_tesrdata_fscanf_cell (struct TESR, struct TESRDATA *pTD,
				      char *type, char *argument);
extern void nev_tesrdata_fscanf_rptb (struct TESRDATA *pTD,
				      char *type, char *argument);

extern void nev_tesrdata_cell2rpt (struct TESR Tesr, char *type, struct
				   TESRDATA TesrDataPoly,
				   struct TESRDATA *pTD);
