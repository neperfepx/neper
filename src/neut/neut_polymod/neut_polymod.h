/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_polymod.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_POLYMOD_H
#define NEUT_POLYMOD_H

  extern void neut_polymod_set_zero (struct POLYMOD *pPolymod);
  extern void neut_polymod_free (struct POLYMOD *pPolymod);
  extern void neut_polymod_faces_inter (struct POLYMOD Polymod, int p1, int p2, int p3, double *inter);

#endif				/* NEUT_POLYMOD_H */

#ifdef __cplusplus
}
#endif
