/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SIM_FPRINTF_H
#define NEUT_SIM_FPRINTF_H

#include"neut.h"

/// writes the entity and result definitions of a simulation directory
extern void neut_sim_fprintf (struct SIM Sim, char *mode);

/// writes the definition file of an entity, results/<entity>/<entity>.def
extern void neut_sim_fprintf_entity (struct SIM Sim, int entity, char *mode);

/// writes the definition file of a result, results/<entity>/<res>/<res>.def
/// (if the result has an expression)
extern void neut_sim_fprintf_res (struct SIM Sim, int entity, int res, char *mode);

extern void neut_sim_fprintf_asy (char *dir, struct SIM Sim, char *mode);

/// writes at terminal about the simulation
extern void neut_sim_verbose (struct SIM Sim);

#endif /* NEUT_SIM_FPRINTF_H */
