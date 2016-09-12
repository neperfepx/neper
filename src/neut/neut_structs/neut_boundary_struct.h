/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTBOUNDARY_H
#define STRUCTBOUNDARY_H

struct BOUNDARY
{
  int BoundQty;		// number of bounds
  char ***BoundNames;   // names of the bounds
  int **BoundDom;	// [1...][0,1] as in struct TESS
  int **BoundCell;	// [1...][0,1] cells of bounds, master (lower index) first
  int *BoundEltQty;	// [1...][0,1] number of elts of a bound
  int ***BoundElts;	// [1...][0,1][1...] elts of a bound, master and slave sides
  int ***BoundEltFacets;// [1...][0,1][1...] bounds of the elts of BoundElts
};
typedef struct BOUNDARY BOUNDARY;

#endif /* STRUCTBOUNDARY_H */
