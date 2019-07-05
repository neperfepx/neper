/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTBOUNDARY_H
#define STRUCTBOUNDARY_H

struct BOUNDARY
{
  int BoundQty;		// number of bounds
  char ***BoundNames;   // names of the bounds
  int **BoundDom;	// [1...][0,1] as in struct TESS
  int **BoundSeed;	// [1...][0,1] cells of bounds, master (lower index) first
  int *BoundEltQty;	// [1...][0,1] number of elts of a bound
  int ***BoundElts;	// [1...][0,1][1...] elts of a bound, master and slave sides
  int ***BoundEltFacets;// [1...][0,1][1...] facets of the elts of BoundElts
  int *BoundNodeQty;	// [1...][0,1] number of nodes of a bound
  int ***BoundNodes;	// [1...][0,1][1...] nodes of a bound, master and slave sides
                        // in 2D: node numbers along a boundary (no duplicates)
			// in 3D: nodes of the elements of the boundary
			// (includes duplicates)
};
typedef struct BOUNDARY BOUNDARY;

#endif /* STRUCTBOUNDARY_H */
