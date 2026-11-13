/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

/// \brief Minimize the objective function using the subplex algorithm
/// (derived from NLopt's NLOPT_LN_SBPLX), with the bounds, stopping
/// criteria and initial step of *pTOpt.
extern void net_tess_opt_comp_minimize_subplex (struct TOPT *pTOpt);
