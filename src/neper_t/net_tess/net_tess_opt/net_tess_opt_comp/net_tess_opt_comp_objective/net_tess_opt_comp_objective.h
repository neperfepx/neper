/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"structIn_t.h"

extern double net_tess_opt_comp_objective (unsigned int n, const double
				           *x, double *grad, void *data);

extern void net_tess_opt_forcestop (int sig);
