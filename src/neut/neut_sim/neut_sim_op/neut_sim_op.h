/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SIM_OP_H
#define NEUT_SIM_OP_H

extern void neut_sim_set_zero (struct SIM *Sim);
extern void neut_sim_free (struct SIM *Sim);
extern void neut_sim_addres (struct SIM *pSim, char *entity, char *res, int written);
extern void neut_sim_addnoderes (struct SIM *pSim, char *res, int written);
extern void neut_sim_addeltres (struct SIM *pSim, char *res, int written);
extern void neut_sim_rmres (struct SIM *pSim, char *entity, char *res);
extern void neut_sim_rmnoderes (struct SIM *pSim, char *res);
extern void neut_sim_rmeltres (struct SIM *pSim, char *res);
extern int neut_sim_testres (struct SIM Sim, char *entity, char *res);

extern int neut_sim_updatenodes (struct SIM Sim, int step, struct NODES *pNodes);

#endif /* NEUT_SIM_OP_H */
