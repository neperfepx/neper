/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>

#include"../structIn_t.h"
#include"ut.h"
#include"neut_t.h"
#include"net_reg_merge/net_reg_merge.h"

#include"net_reg.h"

extern void net_reg_init (struct IN_T In, struct TESS *pTess, struct REG *pReg);
