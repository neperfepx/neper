/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include"orilib.h"

#include"../nev_data_utils/nev_data_utils.h"
#include "nev_tessdata.h"

extern void nev_tessdata_fscanf_init (struct TESS Tess,
                          char *entity, char *prop, char *argument,
			  int *pid, int *pentityqty,
			  char **ptype, char **pvalue);

extern void nev_tessdata_fscanf_col (struct TESS Tess,
                                     struct TESSDATA *pTessData,
                                     char *entity, int id,
				     int entityqty, char *type,
				     char *value);

extern void nev_tessdata_fscanf_trs (struct TESSDATA *pTessData,
                                     int id, int entityqty, char *type,
				     char *value);

extern void nev_tessdata_fscanf_rad (struct TESSDATA *pTessData,
                                     int id, int entityqty, char *type,
				     char *value);

extern void nev_tessdata_fscanf_colscheme (struct TESSDATA *pTessData,
					   int id, char *type);

extern void nev_tessdata_fscanf_scale (struct TESSDATA *pTessData,
				       int id, char *type);
extern void nev_tessdata_fscanf_scaletitle (struct TESSDATA *pTessData,
					    int id, char *type);
