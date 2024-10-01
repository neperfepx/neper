/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_multim.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_MULTIM_H
#define NEUT_MULTIM_H

  extern void neut_multim_set_zero (struct MULTIM *);
  extern void neut_multim_free (struct MULTIM *, int);
  extern void neut_multim_init (int dim, char *list, int cellqty,
				struct MULTIM *pMultim);

#endif				/* NEUT_MULTIM_H */

#ifdef __cplusplus
}
#endif
