/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef  NEUT_FLATTEN_OP_H
#define  NEUT_FLATTEN_OP_H

/// \brief Set a FLATTEN structure to zero.
///
///
///
  extern void neut_flatten_set_zero (struct FLATTEN *pFlatten);

  extern void neut_flatten_addver (struct FLATTEN *pFlatten, int *dom);
  extern void neut_flatten_addedge (struct FLATTEN *pFlatten, int *dom);
  extern void neut_flatten_addface (struct FLATTEN *pFlatten, int *dom);
  extern void neut_flatten_addpoly (struct FLATTEN *pFlatten, int *dom);
  extern void neut_flatten_init_domstuff_fromver (struct FLATTEN
						  *pFlatten,
						  struct TESS DTess);
  extern void neut_flatten_init_domstuff_fromedge (struct FLATTEN *pFlatten,
						   struct TESS DTess);
  extern void neut_flatten_init_domstuff_fromface (struct FLATTEN *pFlatten);
  extern void neut_flatten_free (struct FLATTEN *pFlatten);

#endif				/* NEUT_FLATTEN_OP_H */

#ifdef __cplusplus
}
#endif
