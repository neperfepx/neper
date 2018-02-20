/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../structIn_t.h"
#include"ut.h"
#include"orilib.h"
#include"neut_t.h"

#include"net_stat.h"

extern void net_stat_tess (FILE *, char *, char *, struct TESS);
extern void net_stat_tesr (FILE * file, char *entity, char *format,
			   struct TESR *pTesr);
extern void net_stat_point (FILE *, char *, struct POINT, struct TESS);
