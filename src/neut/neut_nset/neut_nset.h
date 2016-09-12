/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_nset.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_NSET_H
#define NEUT_NSET_H

  extern void neut_nset_expand (struct NSET, struct NSET, struct NSET, char *,
				char **);
  extern void neut_nset_set_zero (struct NSET *);
  extern void neut_nset_free (struct NSET *);
  extern void neut_nsets_inter (struct NSET NSet, int id1, int id2,
				char **pname, int **pnodes, int *pnodeqty);

#endif				/* NEUT_NSET_H */

#ifdef __cplusplus
}
#endif
