/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TDYN_OP_H
#define NEUT_TDYN_OP_H

/// \brief Set a TDYN structure to zero
///
///
///
  extern void neut_tdyn_set_zero (struct TDYN *pTess);
  extern void neut_tdyn_alloc (struct TDYN *, int N, int Nall);
  extern void neut_tdyn_free (struct TDYN *);
  extern void neut_tdyn_init_otherdur (struct TDYN *);
  extern void neut_tdyn_init_avdiameq (struct TDYN *pTDyn, struct
				       SEEDSET SSet, struct POLY
				       Domain);

#endif				/* NEUT_TDYN_OP_H */

#ifdef __cplusplus
}
#endif
