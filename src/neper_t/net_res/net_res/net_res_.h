/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../structIn_t.h"
#include"ut.h"
#include"orilib.h"
#include"neut_t.h"

#include"net_res.h"

extern void net_res_tess (FILE *, char *, char *, struct TESS *pTess);
extern void net_res_tesr (FILE * file, char *entity, char *format,
			   struct TESR *pTesr);
extern void net_res_point (FILE *, char *, struct POINT, struct TESS);
