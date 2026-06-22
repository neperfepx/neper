/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern double net_tess_opt_comp_objective (unsigned int n, const double
				           *x, double *grad, void *data);

extern void net_tess_opt_forcestop (int sig);
