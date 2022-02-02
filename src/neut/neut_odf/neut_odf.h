/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_ODF_H
#define NEUT_ODF_H


extern void neut_odf_set_zero (struct ODF *pOdf);
extern void neut_odf_space_fnscanf (char *filename, struct ODF *pOdf, char *mode);
extern void neut_odf_sigma (char *expr, struct OL_SET *pOSet,
                            struct ODF *pOdf);
extern void neut_odf_comp (char *mode, char *neigh, struct OL_SET *pOSet, struct ODF *pOdf);

#endif /* NEUT_ODF_H */

#ifdef __cplusplus
}
#endif
