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

#include"nes_sim_fepxparse/nes_sim_fepxparse.h"
#include"nes_sim_write/nes_sim_write.h"

extern void nes_sim_modify (struct IN_S In, struct SIM *pSim);
extern void nes_sim_modify_entity (struct IN_S In, struct SIM *pSim, char *entity);

#include"nes_sim.h"
