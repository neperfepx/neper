/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"structIn_t.h"
#include"neut_t.h"

#include"net_transform_tesr_tesrtranslate.h"
#include"net_transform_tesr_tesrtranslate_bounds/net_transform_tesr_tesrtranslate_bounds.h"

extern void net_transform_tesr_tesrtranslate_pre (char *string, struct TESR
    *pTesr, struct TESR *pTesr2, int **pcells, int **prefcells, int *pcellqty,
    char ***palgos, int *palgoqty);

extern void net_transform_tesr_tesrtranslate_centres (struct TESR *pTesr, int
    *cells, int *refcells, int refcellqty, struct TESR Tesr2);
