/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MTESS_FSCANF_H
#define NEUT_MTESS_FSCANF_H

  extern void neut_mtess_fscanf (FILE * file, struct MTESS *pMTess, struct
				 TESS **pTess);
  extern void neut_mtess_name_fscanf (char *filename, struct MTESS *pMTess,
				      struct TESS **pTess);

#endif				/* NEUT_MTESS_FSCANF_H */

#ifdef __cplusplus
}
#endif
