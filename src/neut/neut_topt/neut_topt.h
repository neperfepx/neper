/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TOPT_H
#define NEUT_TOPT_H

extern void neut_topt_set_zero (struct TOPT *pTOpt);
extern void neut_topt_free (struct TOPT *pTOpt);
extern void neut_topt_subcell_add (struct TOPT *pTOpt, int scell, int
    cell);
extern void neut_topt_seed_subcell_add (struct TOPT *pTOpt, double *coo,
    double rad, int cell);

#endif /* NEUT_TOPT_H */
