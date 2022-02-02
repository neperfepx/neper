/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_boundary.h
/// \brief Manipulate data associated to tessellations (BOUNDARY)
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_BOUNDARY_H
#define NEUT_BOUNDARY_H

  extern void neut_boundary_set_zero (struct BOUNDARY *);
  extern void neut_boundary_free (struct BOUNDARY *);
  extern void neut_boundary_bound_nodes (struct MESH Mesh, struct BOUNDARY Bound, int id, int side,
					 int **pnodes, int *pnodeqty);
  extern void neut_boundary_bound_nodes_2d (struct MESH Mesh, struct BOUNDARY Bound, int id, int side,
					 int **pnodes, int *pnodeqty);
  extern void neut_boundary_bound_nodes_3d (struct MESH Mesh, struct BOUNDARY Bound, int id, int side,
					 int **pnodes, int *pnodeqty);

#endif				/* NEUT_BOUNDARY_H */

#ifdef __cplusplus
}
#endif
