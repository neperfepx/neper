/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_pf.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_PF_H
#define NEUT_PF_H

  extern void neut_pf_set_zero (struct PF *pPf);
  extern void neut_pf_free (struct PF *pPf);
  extern void neut_pf_init_ipfborder (struct PF *pPf);
  extern void neut_pf_init_grid (struct PF *pPf);
  extern void neut_pf_point_gridpos (struct PF Pf, double *coo, int *pos);

#endif				/* NEUT_PF_H */

#ifdef __cplusplus
}
#endif
