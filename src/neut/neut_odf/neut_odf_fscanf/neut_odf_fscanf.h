/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_ODF_FSCANF_H
#define NEUT_ODF_FSCANF_H

extern void neut_odf_fnscanf (char *filename, struct ODF *pOdf, char *mode);
extern void neut_odf_fscanf (FILE *file, struct ODF *pOdf);

#endif /* NEUT_ODF_FSCANF_H */

#ifdef __cplusplus
}
#endif
