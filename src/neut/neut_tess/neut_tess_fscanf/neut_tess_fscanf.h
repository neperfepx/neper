/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_FSCANF_H
#define NEUT_TESS_FSCANF_H

  extern void neut_tess_fscanf (FILE *, struct TESS *);
  extern void neut_tess_fscanf_verbosity (FILE *, struct TESS *, int);
  extern void neut_tess_fscanf_head (struct TESS *, FILE *);
  extern void neut_tess_name_fscanf (char *, struct TESS *);

  extern void neut_tess_fscanf_1p9 (FILE *, struct TESS *);
  extern void neut_tess_fscanf_1p10 (FILE *, struct TESS *);

#endif				/* NEUT_TESS_FSCANF_H */

#ifdef __cplusplus
}
#endif
