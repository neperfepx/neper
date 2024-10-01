/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_lloyd.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_LLOYD_H
#define NEUT_LLOYD_H

  extern void neut_lloyd_set_zero (struct LLOYD *pLloyd);
  extern void neut_lloyd_free (struct LLOYD *pLloyd);

#endif				/* NEUT_LLOYD_H */

#ifdef __cplusplus
}
#endif
