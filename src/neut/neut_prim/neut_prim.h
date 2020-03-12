/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_prim.h
/// \brief Manipulate data associated to tessellations (PRIM)
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_PRIM_H
#define NEUT_PRIM_H

extern void neut_prim_set_zero (struct PRIM *);
extern void neut_prim_free (struct PRIM *);
extern int  neut_prim_sscanf (char *expr, struct PRIM *);
extern void neut_prim_point_mirror (struct PRIM Prim, double *coo, double *mirror);
extern void neut_prim_point_side (struct PRIM Prim, double *coo, int *pside);
extern double neut_prim_point_proj (struct PRIM Prim, double *coo, double *proj);
extern double neut_primparms_point_proj (char* type, double *parms, double *coo, double *proj);
extern void neut_primparms_point_tangentplane (char* type, double *parms, double *coo, double *plane);

#endif				/* NEUT_PRIM_H */

#ifdef __cplusplus
}
#endif
