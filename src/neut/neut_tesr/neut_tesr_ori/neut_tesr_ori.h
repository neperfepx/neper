/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef  NEUT_TESR_ORI_H
#define  NEUT_TESR_ORI_H

  extern void neut_tesr_cell_orianiso (struct TESR Tesr, int cell,
                                       double **evect, double *eval);
  extern void neut_tesr_cell_orianiso_delta (struct TESR Tesr, int cell,
                                            double **evect, double *delta);

  extern void neut_tesr_cells_olset (struct TESR Tesr, double **CellOri, char *crysym,
                         int *cells, int cellqty, struct OL_SET *pOSet);

  extern void neut_tesr_voxels_olset (struct TESR Tesr, struct OL_SET *pOSet);

#include"net_utils/net_utils.h"

#endif				/* NEUT_TESR_ORI_H */

#ifdef __cplusplus
}
#endif
