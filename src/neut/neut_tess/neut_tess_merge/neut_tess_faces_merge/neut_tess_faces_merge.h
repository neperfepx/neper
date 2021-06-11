/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_MERGECELLS_2D_H
#define NEUT_TESS_MERGECELLS_2D_H

  extern void neut_tess_faces_merge (struct TESS *pTess, int *cells,
				     int cellqty);

  extern void neut_tess_faces_merge_verlists (struct TESS *pTess,
					      int *deledges, int deledgeqty,
					      int **pkeepvers,
					      int *pkeepverqty,
					      int **pdelvers,
					      int *pdelverqty);
  extern void neut_tess_faces_merge_updatevers (struct TESS *pTess,
						int *deledges, int deledgeqty,
						int *keepvers, int keepverqty,
						int *delvers, int delverqty);

#endif				/* NEUT_TESS_MERGECELLS_2D_H */

#ifdef __cplusplus
}
#endif
