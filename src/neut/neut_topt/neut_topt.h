/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TOPT_H
#define NEUT_TOPT_H

extern void neut_topt_set_zero (struct TOPT *pTOpt);
extern void neut_topt_free (struct TOPT *pTOpt);
extern void neut_topt_subcell_add (struct TOPT *pTOpt, int scell, int
    cell);
extern void neut_topt_seed_subcell_add (struct TOPT *pTOpt, double *coo,
    double rad, int cell);
extern int neut_topt_eps (struct TOPT TOpt, double *peps);
extern int neut_topt_reps (struct TOPT TOpt, double *preps);

extern int neut_topt_plateau (struct TOPT *pTOpt);

extern int neut_topt_refiter (struct TOPT TOpt, int deltaiter, int *prefiter);

extern int neut_topt_iter_loop (struct TOPT TOpt, int iter, int *ploop);

extern int neut_topt_typepos (struct TOPT TOpt);

#endif /* NEUT_TOPT_H */
