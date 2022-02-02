/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_MERGE_H
#define NEUT_TESS_MERGE_H

  extern void neut_tess_merge (struct TESS *);
  extern int neut_tess_cellexpr_merge (struct TESS *pTess, char *expr, double coplanar);
  extern int neut_tess_cells_merge_nocompress (struct TESS *pTess, int *cells, int cellqty, double coplanar);
  extern int neut_tess_cells_merge (struct TESS *pTess, int *cells, int cellqty, double coplanar);

#endif				/* NEUT_TESS_MERGE_H */

#ifdef __cplusplus
}
#endif
