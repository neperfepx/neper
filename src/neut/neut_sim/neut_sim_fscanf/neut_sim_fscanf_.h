/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>
#include<stdarg.h>
#include<unistd.h>
#include<sys/stat.h>
#include<orilib.h>

#include<ctype.h>

#include"neut.h"

extern void neut_sim_fscanf_legacy (char *dir);
extern void neut_sim_fscanf_inputs (struct SIM *pSim);
extern void neut_sim_fscanf_steps (struct SIM *pSim);
extern void neut_sim_fscanf_results (struct SIM *pSim);
extern void neut_sim_fscanf_restart (struct SIM *pSim);

extern void neut_sim_fscanf_legacy_head (FILE *file);
extern void neut_sim_fscanf_legacy_foot (FILE *file);
extern void neut_sim_fscanf_legacy_skip (FILE *file);
extern void neut_sim_fscanf_legacy_entity (struct SIM *pSim, FILE *file);
extern void neut_sim_fscanf_inputs_file (char **files, int fileqty, char *ext, char **pfile);
extern void neut_sim_fscanf_subdirs (char *dir, char ***psubdirs, int *psubdirqty);
extern void neut_sim_fscanf_entitydir_entity (char *entitydir, char **pentity);
extern void neut_sim_fscanf_def (struct SIM *pSim, char *entity);
extern void neut_sim_fscanf_resdef (struct SIM Sim, char *entitydir, char *res, char **pexpr, char **ptype);
extern int neut_sim_fscanf_res (struct SIM Sim, char *entitydir, char *res, int *pstepqty);

extern void neut_sim_fscanf_resdef_value (char *name, char *section, char *value, char **pexpr, char **ptype);
extern void neut_sim_fscanf_error (FILE *file, const char *format, ...);
