/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_ODF_H
#define NEUT_ODF_H


extern void neut_odf_set_zero (struct ODF *pOdf);
extern void neut_odf_setsigma (struct ODF *pOdf, char *expr, int qty, char *crysym);
extern void neut_odf_comp (char *mode, char *neigh, struct OL_SET *pOSet, struct ODF *pOdf, int verbosity);
extern void neut_odf_orides (struct ODF Odf, char **porides);

extern void neut_odf_convolve (struct ODF *pOdf, char *kernel);
extern void neut_odf_deconvolve (struct ODF *pOdf, char *kernel);

extern void neut_odf_elt_ori (struct ODF Odf, int elt, gsl_rng *r, double *q);
extern void neut_odf_orifield_comp (char *mode, char *neigh, struct OL_SET *pOSet,
                                    double *oridata, struct ODF *pOdf);

extern int neut_odf_normalize (struct ODF *pOdf, double *pfact);

extern int neut_odf_index (struct ODF *pOdf, double *pindex);
extern int neut_odfn_index (struct ODF *pOdf, double *pindex);

extern double neut_odf_constant (double theta);

extern void neut_odf_crysym (struct ODF ODF, char **pcrysym);
extern void neut_odf_free (struct ODF *pOdf);

#include "neut_odf_fscanf/neut_odf_fscanf.h"

#endif /* NEUT_ODF_H */

#ifdef __cplusplus
}
#endif
