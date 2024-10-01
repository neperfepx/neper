/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_FEPXSIM_OP_H
#define NEUT_FEPXSIM_OP_H

/// \file neut_fepxsim_op.h
/// \brief FSimulation-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Initialize a \c struct \c FEPXSIM
/// \param *pFSim: simulation
extern void neut_fepxsim_set_zero (struct FEPXSIM *pFSim);

/// \brief Free a \c struct \c FEPXSIM
/// \param *pFSim: simulation
extern void neut_fepxsim_free (struct FEPXSIM *pFSim);

#endif /* NEUT_FEPXSIM_OP_H */
