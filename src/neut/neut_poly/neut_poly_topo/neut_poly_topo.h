/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_poly.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_POLY_TOPO_H
#define NEUT_POLY_TOPO_H

  extern int neut_poly_isvoid (struct POLY Poly);
  extern int neut_poly_ver_polys (struct POLY Poly, int poly, int ver,
      int **ppoly, int *ppolyqty);
  extern int neut_poly_verpair_compolys (struct POLY Poly, int ver1,
					 int ver2, int **ppoly, int *ppolyqty);

  extern int neut_polys_polypair_neigh (struct POLY *Poly,
					int *PerSeedSlaveQty, int **PerSeedSlaveNb,
					int polyA, int polyB);
  extern void neut_poly_neighpolys (struct POLY Poly, struct SEEDSET, int **ppolys, int *ppolyqty);
  extern void neut_polys_neighpolys (struct POLY *Poly, struct SEEDSET
      SSet, int *polys, int polyqty, int **pneighs, int *pneighqty);
  extern int  neut_polys_contiguous (struct POLY *Poly,
				     int *PerSeedSlaveQty, int **PerSeedSlaveNb,
				     int* polys, int polyqty,
				     int ***ppolys, int** ppolyqty, int *pqty);

  extern int neut_poly_edges (struct POLY Poly, int ***pedges, int
      *pedgeqty);

#endif				/* NEUT_POLY_TOPO_H */

#ifdef __cplusplus
}
#endif
