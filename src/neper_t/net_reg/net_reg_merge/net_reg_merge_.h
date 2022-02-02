/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>
#include<unistd.h>

#include"../../structIn_t.h"
#include"ut.h"
#include"neut_t.h"

#include"net_reg_merge_del/net_reg_merge_del.h"
#include"net_reg_merge.h"

extern int net_reg_merge_nextedge (struct TESS, struct REG, double *);
extern void net_reg_merge_print (int, double, int, int, int, double, double);
