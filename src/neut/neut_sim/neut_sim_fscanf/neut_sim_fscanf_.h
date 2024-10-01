/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

extern void neut_sim_fscanf_version (char *dir, char **pversion);
extern void neut_sim_fscanf_head (FILE *pSim, char **pversion);
extern void neut_sim_fscanf_input (struct SIM *pSim, FILE *file);
extern void neut_sim_fscanf_general (struct SIM *pSim, char *version, FILE *file);
extern void neut_sim_fscanf_foot (FILE *file);
extern void neut_sim_fscanf_entity (struct SIM *pSim, FILE *file);
extern void neut_sim_fscanf_orispace (struct SIM *pSim, FILE *file);
extern void neut_sim_fscanf_step (struct SIM *pSim, FILE *file);
extern void neut_sim_fscanf_restart (struct SIM *pSim, FILE *file);

#include "neut_sim_fscanf_report/neut_sim_fscanf_report.h"
