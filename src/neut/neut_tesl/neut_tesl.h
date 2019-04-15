/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_tesl.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_TESL_H
#define NEUT_TESL_H

  extern void neut_tesl_bbox (struct TESL, double **);
  extern void neut_tesl_domainface (struct TESL, int **);
  extern void neut_tesl_domainfacever (struct TESL, int **);
  extern int neut_tesl_cell_true (struct TESL, int);
  extern int neut_tesl_ver_true (struct TESL, int *, int);
  extern int neut_tesl_edge_true (struct TESL, int *, int);
  extern void neut_tesl_edge_poly (struct TESL, int, int *);
  extern int neut_tesl_face_true (struct TESL, int *, int);

  extern void neut_tesl_set_zero (struct TESL *);
  extern void neut_tesl_free (struct TESL *);
  extern void neut_tesl_init_celltrue (struct TESL *);
  extern int neut_tesl_face_area (struct TESL, int, double *);
  extern int neut_tesl_poly_volume (struct TESL, int, double *);

  extern int neut_tesl_cell_body (struct TESL, int);

  extern void neut_tesl_init_cellbody (struct TESL *);

#include "neut_tesl_tess/neut_tesl_tess.h"

#endif				/* NEUT_TESL_H */

#ifdef __cplusplus
}
#endif
