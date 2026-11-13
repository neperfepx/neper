/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

/// \brief Evaluate the objective function at x
extern double net_tess_opt_comp_minimize_objective (const double *x,
                                           struct TOPT *pTOpt);

/// \brief Test the stopping criteria not handled by the optimization
/// algorithm (NULL if none is met, the reason of the stop otherwise)
extern const char *net_tess_opt_comp_minimize_objective_stop (struct TOPT
                                                              *pTOpt);

/// \brief End the testing of the stopping criteria (restore the default
/// Ctrl+C behavior)
extern void net_tess_opt_comp_minimize_objective_stop_end (void);

/// \brief Report the evaluation of the objective function (print and log)
extern void net_tess_opt_comp_minimize_objective_report (struct TOPT *pTOpt);
