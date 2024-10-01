/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_FSCANF_H
#define NEUT_TESS_FSCANF_H

  extern void neut_tess_fscanf (FILE *, struct TESS *);
  extern void neut_tess_fnscanf (char *, struct TESS *);

#include "neut_tess_fscanf_obj/neut_tess_fscanf_obj.h"
#include "neut_tess_fscanf_ovm/neut_tess_fscanf_ovm.h"

#endif				/* NEUT_TESS_FSCANF_H */

#ifdef __cplusplus
}
#endif
