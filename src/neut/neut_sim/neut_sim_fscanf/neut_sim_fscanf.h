/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_SIM_FSCANF_H
#define NEUT_SIM_FSCANF_H

  /// reads a simulation directory from its content
  extern void neut_sim_fscanf (char *dir, struct SIM *pSim);

  /// returns 0 and the step if file is <res>.step<step>, -1 otherwise
  extern int neut_sim_fscanf_steps_file (char *res, char *file, int *pstep);

#endif				/* NEUT_SIM_FSCANF_H */

#ifdef __cplusplus
}
#endif
