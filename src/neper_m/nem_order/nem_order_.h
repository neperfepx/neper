/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>

#include"structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_order.h"

extern void nem_order_init_3d (char *elttype, int ***pfir3, int ***psec3,
			       int *pnodeqty_3d1, int *pnodeqty_3d);
extern void nem_order_init_2d (char *elttype, int ***pfir2, int ***psec2,
			       int *pnodeqty_2d1, int *pnodeqty_2d);
extern void nem_order_init_1d (char *elttype, int ***pfir1, int ***psec1,
			       int *pnodeqty_1d1, int *pnodeqty_1d);
