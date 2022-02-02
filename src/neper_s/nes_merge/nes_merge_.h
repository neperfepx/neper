/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"
#include"orilib.h"
#include"structIn_s.h"

#include"nes_merge.h"

#include "../nes_convert/nes_convert_write/nes_convert_write.h"

extern void nes_merge_input (struct SIM Sim, char *sim2);
extern void nes_merge_entity_res (struct SIM Sim, char *entity, int cstep, char *sim2);
extern void nes_merge_restart (struct SIM Sim, char *sim1, char *sim2);
extern void nes_merge_post (struct IN_S In, char *sim, int stepqty, struct SIM LastSim, struct SIM *pFullSim);
