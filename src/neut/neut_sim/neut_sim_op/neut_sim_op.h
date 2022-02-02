/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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

/// \brief Add an entity to a \c struct \c SIM
/// \param *pSim: simulation
/// \param entity: entity = node, element, etc.
/// \note <name> or <name>:<expr> can be passed as argument
extern void neut_sim_addentity (struct SIM *pSim, char *entity);

extern void neut_sim_entity_addexpr (struct SIM *pSim, char *entity, char *expr);

/// \brief Add a result to a \c struct \c SIM
/// \param *pSim: simulation
/// \param entity: result entity = node, element, etc.
/// \param resexpr: result and expression (expression optional)
/// \param expr: expression (expression optional)
extern void neut_sim_addres (struct SIM *pSim, char *entity, char *resexpr, char *expr);

/// \brief Remove a result from a \c struct \c SIM
/// \param *pSim: simulation
/// \param entity: result entity = node, element, etc.
/// \param res: result
extern void neut_sim_rmres (struct SIM *pSim, char *entity, char *res);

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

/// \brief Initialize a \c struct \c SIMRES
/// \param *pSimRes: simulation result
extern void neut_simres_set_zero (struct SIMRES *pSimRes);

/// \brief Free a \c struct \c SIMRES
/// \param *pSimRes: simulation result
extern void neut_simres_free (struct SIMRES *pSimRes);

// set step and update members
extern void neut_simres_setstep (struct SIMRES *pSimRes, int step);

extern void neut_sim_entity_init_members (struct SIM *pSim, struct TESS Tess,
                                          struct NODES Nodes, struct MESH *Mesh,
                                          char *entity);

#endif /* NEUT_SIM_OP_H */
