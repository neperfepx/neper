/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/// \file neut_seedset.h
/// \brief Manipulate seed sets
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_SEEDSET_H
#define NEUT_SEEDSET_H

extern void neut_seedset_set_zero (struct SEEDSET *);
extern void neut_seedset_free (struct SEEDSET *);
extern int neut_seedset_seed_master (struct SEEDSET SSet, int seed);

extern int neut_seedset_addseed (struct SEEDSET *pSSet, double *coo,
				 double w);
extern int neut_seedset_slave_add (struct SEEDSET *pSSet, int master,
				   int *shift);
extern int neut_seedset_slave_setprop (struct SEEDSET *pSSet, int id);
extern int neut_seedset_update_slaves (struct SEEDSET *pSSet);
extern void neut_seedset_init_seedslave (struct SEEDSET *pSSet);

extern int neut_seedset_seedcoo0toseedcoo (struct SEEDSET *pSSet);
extern int neut_seedset_seedcootoseedcoo0 (struct SEEDSET *pSSet);

extern int neut_seedset_seed_update_fromseedcoo0 (struct SEEDSET *pSSet, int id);
extern int neut_seedset_seed_update_fromseedorir (struct SEEDSET *pSSet, int id);
extern void neut_seedset_seed_updateslaves (struct SEEDSET *pSSet, int id);

extern double neut_seedset_voidcell_distance (struct SEEDSET, int seed);

extern void neut_seedset_memcpy (struct SEEDSET SSetA, struct SEEDSET *pSSetB);
extern void neut_seedset_memcpy_general (struct SEEDSET SSetA, struct SEEDSET *pSSetB);
extern void neut_seedset_memcpy_seeds (struct SEEDSET SSetA, struct SEEDSET *pSSetB);
extern void neut_seedset_memcpy_periodic (struct SEEDSET SSetA, struct SEEDSET *pSSetB);
extern void neut_seedset_memcpy_seed_addseed (struct SEEDSET SSetA, int
    seed, struct SEEDSET *pSSetB);
extern void neut_seedset_activedim (struct SEEDSET SSet, char *morpho, int **pdim, int
    *pdimqty);

extern void neut_seedset_seed_randomize (struct SEEDSET *pSet, int seed,
			     gsl_rng *r, int *dims, int dimqty,
			     double mindist, double maxdist);
extern void neut_seedset_bbox_size (struct SEEDSET SSet, double *psize);

extern void neut_seedset_bcc (struct TESS Dom, int n, struct SEEDSET *pSSet);
extern void neut_seedset_bcc_expr (struct TESS Dom, char *cooexpr, struct SEEDSET *pSSet);

#endif /* NEUT_SEEDSET_H */
