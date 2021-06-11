/* This file is part of the Neper software package. */
/* Ctopoyright (C) 2003-2021, Romain Quey. */
/* See the CTOPOYING file in the ttopo-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef  NEUT_FLATTEN_TOPO_H
#define  NEUT_FLATTEN_TOPO_H

  extern void neut_flatten_domface_edges (struct FLATTEN Flatten,
					  struct TESS DTess, int dface,
					  int **pedges, int *pedgeqty);

/// \brief
///
///
///
  extern void neut_flatten_domface_ver_edges (struct FLATTEN Flatten,
					      struct TESS DTess, int dface,
					      struct TESS FTess,
					      int ver, int **pedges,
					      int *pedgeqty);

#endif				/* NEUT_FLATTEN_TOPO_H */

#ifdef __cplusplus
}
#endif
