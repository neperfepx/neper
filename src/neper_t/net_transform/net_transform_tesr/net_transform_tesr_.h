/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"structIn_t.h"
#include"neut_t.h"

#include"net_transform_tesr.h"
#include"net_transform_tesr_tesrtranslate/net_transform_tesr_tesrtranslate.h"
#include"net_transform_tesr_tesrmerge/net_transform_tesr_tesrmerge.h"
#include"net_ori/net_ori.h"

extern void net_transform_tesr_tesrrenumber (struct IN_T In, char *string, struct TESR *pTesr);
extern void net_transform_tesr_orirotate (char *string, struct TESR *pTesr);
extern void net_transform_tesr_ori (char *ori, struct TESR *pTesr);
