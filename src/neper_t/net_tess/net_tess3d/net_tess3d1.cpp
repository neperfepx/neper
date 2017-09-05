/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess3d_.h"
#include<ANN/ANN.h>

extern void net_polycomp (struct POLY Domain, struct SEEDSET SeedSet,
			  ANNkd_tree ** pkdTree, struct POLY **pPoly,
			  int *seed_changed, int seed_changedqty,
			  struct TDYN *);

int
net_tess3d (struct TESS PTess, int poly, struct SEEDSET SSet,
	    char *algoneigh, int TessId, struct MTESS *pMTess,
	    struct TESS *pTess)
{
  struct POLY DomPoly, *Poly = NULL;
  struct TDYN TD;
  ANNkd_tree *kdTree = NULL;

  neut_tess_set_zero (pTess);

  neut_tdyn_set_zero (&TD);
  ut_string_string (algoneigh, &TD.algoneigh);
  neut_poly_set_zero (&DomPoly);

  net_tess_poly (PTess, poly, &DomPoly);

  net_polycomp (DomPoly, SSet, &kdTree, &Poly, NULL, -1, &TD);

  net_polys_tess (PTess.Level + 1, SSet, TessId, Poly, pTess);

  net_tess3d_domain (PTess, poly, TessId, pMTess, pTess);

  if (!strncmp ((*pTess).Type, "periodic", 8))
    net_tess3d_periodic (pTess);

  neut_poly_free (&DomPoly);
  neut_poly_array_free (&Poly, SSet.N);
  neut_tdyn_free (&TD);

  delete kdTree;
  annClose ();

  return 0;
}
