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

#include"nes_convert_write.h"

extern int nes_convert_write_inputs_file (struct IN_S In, struct SIM Sim, char *filename);
extern void nes_convert_write_inputs (struct IN_S In, struct SIM *pSim);

extern void nes_convert_write_results (struct IN_S IN_S, struct SIM *pSim);
extern void nes_convert_write_results_entity (struct IN_S IN_S, struct SIM *pSim, int pos);
extern int  nes_convert_write_results_prop (struct SIM Sim, char *var,
                                            int *pstartstep);
extern void nes_convert_write_results_entity_step (struct IN_S In, struct SIM Sim, char *var,
                              char *entity, int startstep);
