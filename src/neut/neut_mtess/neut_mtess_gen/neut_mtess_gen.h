/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MTESS_GEN_H
#define NEUT_MTESS_GEN_H

  extern int neut_mtess_tess_poly_mid (struct MTESS MTess, struct TESS Tess,
				       int poly, char **pid);

  extern void neut_mtess_argument_process (struct MTESS MTess, struct TESS *Tess, int dtess, int dcell, char *input, char **poutput);

#endif				/* NEUT_MTESS_GEN_H */

#ifdef __cplusplus
}
#endif
