/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_csysdata.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_CSYSDATA_H
#define NEUT_CSYSDATA_H

  extern void neut_csysdata_set_default (struct CSYSDATA *);
  extern void neut_csysdata_free (struct CSYSDATA *);
  extern void neut_csysdata_coldatatype_size (struct CSYSDATA NodeData,
					      int *psize);

#endif				/* NEUT_CSYSDATA_H */

#ifdef __cplusplus
}
#endif
