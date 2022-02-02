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

#include"net_utils/net_utils.h"

#endif				/* NEUT_TESR_ORI_H */

#ifdef __cplusplus
}
#endif
