/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_ODF_H
#define NEUT_ODF_H


extern void neut_odf_set_zero (struct ODF *pOdf);
extern void neut_odf_fnscanf (char *filename, struct ODF *pOdf, char *mode);
extern void neut_odf_space_fnscanf (char *filename, struct ODF *pOdf, char *mode);
extern void neut_odf_setsigma (struct ODF *pOdf, char *expr, int qty, char *crysym);
extern void neut_odf_comp (char *mode, char *neigh, struct OL_SET *pOSet, struct ODF *pOdf, int verbosity);
extern void neut_odf_orides (struct ODF Odf, char **porides);
extern void neut_odf_mesh_olset (struct ODF Odf, struct OL_SET *pOSet);

extern void neut_odf_init_eltweight (struct ODF *pOdf);
extern void neut_odf_convolve (struct ODF *pOdf, char *kernel);
extern void neut_odf_deconvolve (struct ODF *pOdf, char *kernel);

extern void neut_odf_elt_ori (struct ODF Odf, int elt, gsl_rng *r, double *q);
extern void neut_odf_orifield_comp (char *mode, char *neigh, struct OL_SET *pOSet,
                                    double *oridata, struct ODF *pOdf);

extern int neut_odf_normalize (struct ODF *pOdf, double *pfact);

#endif /* NEUT_ODF_H */

#ifdef __cplusplus
}
#endif
