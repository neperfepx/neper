/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_OOPT_H
#define NEUT_OOPT_H

#include "orilib.h"

extern void neut_oopt_set_zero (struct OOPT *pOOpt);
extern void neut_oopt_time_set_zero (struct OOPT *pOOpt);
extern void neut_oopt_init_neighdist (char *expr, int dim,
                                      struct OL_SET *pOSet,
                                      struct OOPT *pOOpt);
extern void neut_oopt_finalize (struct OOPT *pOOpt);
extern void neut_oopt_free (struct OOPT *pOOpt);

extern void neut_oopt_init_neighs (char *expr, int dim, struct OL_SET *pOSet,
                                   struct OOPT *pOOpt);

extern void neut_oopt_init_thetafct (struct OOPT *pOOpt);

#endif /* NEUT_OOPT_H */
