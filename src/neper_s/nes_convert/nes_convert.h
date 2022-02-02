/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/// \file nes_convert.h
/// \brief FEPX simulation functions

/// \brief Transform an FEPX raw result directory into a simulation directory
/// \param In: neper -S's In
/// \param *pSim: simulation
extern void nes_convert (struct IN_S In, struct SIM *pSim);
