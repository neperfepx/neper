/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_poly.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_POLY_OP_H
#define NEUT_POLY_OP_H

  extern void neut_poly_free (struct POLY *);
  extern void neut_poly_array_free (struct POLY **pPoly, int size);
  extern void neut_poly_set_zero (struct POLY *);

#endif				/* NEUT_POLY_OP_H */

#ifdef __cplusplus
}
#endif
