/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SIM_FPRINTF_H
#define NEUT_SIM_FPRINTF_H

#include"neut.h"

/// writes a simulation report file
extern void neut_sim_fprintf (char *dir, struct SIM Sim, char *mode);

/// writes at terminal about the simulation
extern void neut_sim_verbose (struct SIM Sim);

#endif /* NEUT_SIM_FPRINTF_H */
