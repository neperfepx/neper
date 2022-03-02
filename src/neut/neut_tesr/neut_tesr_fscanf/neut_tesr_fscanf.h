/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESR_FSCANF_H
#define NEUT_TESR_FSCANF_H

  extern void neut_tesr_fnscanf (char *, struct TESR *);
  extern void neut_tesr_fscanf (FILE *, char *, double *, double *, struct TESR *);

#endif				/* NEUT_TESR_FSCANF_H */

#ifdef __cplusplus
}
#endif
