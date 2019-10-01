/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_point.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_POINT_H
#define NEUT_POINT_H

  extern void neut_point_set_zero (struct POINT *);
  extern void neut_point_free (struct POINT *);
  extern void neut_point_centre (struct POINT Point, double *centre);

  extern void neut_point_var_list (char ***pvar, int *pvarqty);
  extern int neut_point_var_val (struct POINT Point, int id, struct TESS
				 Tess, struct NODES Nodes, struct MESH Mesh,
				 char *var, double **pvals, int *pvalqty, char **ptype);
  extern int neut_point_var_val_one (struct POINT Point, int id, struct TESS
                                     Tess, struct NODES Nodes, struct MESH Mesh,
                                     char *var, double *pval, char **ptype);
  extern void neut_point_addpoint (struct POINT *pPoint, double *coo, double rad);

  extern void neut_point_pt_size (struct POINT Point, int pt, double
      *psize);
  extern void neut_point_shift (struct POINT *pPoint, double *s);
  extern void neut_point_shifttocentre (struct POINT *pPoint);
  extern void neut_point_bbox (struct POINT Point, double **bbox);

#include "neut_point_fscanf/neut_point_fscanf.h"

#endif				/* NEUT_POINT_H */

#ifdef __cplusplus
}
#endif
