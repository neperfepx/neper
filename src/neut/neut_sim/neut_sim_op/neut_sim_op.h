/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SIM_OP_H
#define NEUT_SIM_OP_H

/// \file neut_sim_op.h
/// \brief Simulation-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Initialize a \c struct \c SIM
/// \param *pSim: simulation
extern void neut_sim_set_zero (struct SIM *pSim);

/// \brief Free a \c struct \c SIM
/// \param *pSim: simulation
extern void neut_sim_free (struct SIM *pSim);

/// \brief Add a result to a \c struct \c SIM
/// \param *pSim: simulation
/// \param entity: result entity = node, element, etc.
/// \param res: result
extern void neut_sim_addres (struct SIM *pSim, char *entity, char *res, char *expr, int written);

/// \brief Add a node result to a \c struct \c SIM
/// \param *pSim: simulation
/// \param res: result
/// \param written: is the result written? 0: no, 1 yes
extern void neut_sim_addnoderes (struct SIM *pSim, char *res, char *expr, int written);

/// \brief Add an element result to a \c struct \c SIM
/// \param *pSim: simulation
/// \param res: result
/// \param written: is the result written? 0: no, 1 yes
extern void neut_sim_addeltres (struct SIM *pSim, char *res, char *expr, int written);

/// \brief Add an elset result to a \c struct \c SIM
/// \param *pSim: simulation
/// \param res: result
/// \param written: is the result written? 0: no, 1 yes
extern void neut_sim_addelsetres (struct SIM *pSim, char *res, char *expr, int written);

/// \brief Remove a result from a \c struct \c SIM
/// \param *pSim: simulation
/// \param entity: result entity = node, element, etc.
/// \param res: result
extern void neut_sim_rmres (struct SIM *pSim, char *entity, char *res);

/// \brief Remove a node result from a \c struct \c SIM
/// \param *pSim: simulation
/// \param res: result
extern void neut_sim_rmnoderes (struct SIM *pSim, char *res);

/// \brief Remove an element result from a \c struct \c SIM
/// \param *pSim: simulation
/// \param res: result
extern void neut_sim_rmeltres (struct SIM *pSim, char *res);

/// \brief Remove an elset result from a \c struct \c SIM
/// \param *pSim: simulation
/// \param res: result
extern void neut_sim_rmelsetres (struct SIM *pSim, char *res);

/// \brief Set the step of a simulation
/// \param *pSim: simulation
/// \param step: simulation step
extern int neut_sim_setstep (struct SIM *pSim, int step);

/// \brief Update the node coordinates to match those at a given step
/// \param Sim: simulation
/// \param step: simulation step
/// \param *pNodes: nodes
extern int neut_sim_updatenodes (struct SIM Sim, int step, struct NODES *pNodes);

extern void neut_sim_init_elsetqty (struct SIM *pSim);

#endif /* NEUT_SIM_OP_H */
