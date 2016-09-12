/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_seedset.h
/// \brief Manipulate seed sets
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_SEEDSET_H
#define NEUT_SEEDSET_H

  extern void neut_seedset_set_zero (struct SEEDSET *);
  extern void neut_seedset_free (struct SEEDSET *);
  extern int neut_seedset_seed_master (struct SEEDSET SSet, int seed);

#endif				/* NEUT_SEEDSET_H */

#ifdef __cplusplus
}
#endif
