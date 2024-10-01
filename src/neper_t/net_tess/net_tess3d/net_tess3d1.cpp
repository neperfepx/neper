/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess3d_.h"
#include"neut_nfcloud_struct.hpp"

extern void net_polycomp (struct POLY Domain, struct SEEDSET SeedSet,
                          NFCLOUD * pnf_cloud, NFTREE ** pnf_index,
                          int **pptid_seedid, int **pseedid_ptid,
                          struct POLY **pPoly, int *seed_changed,
                          int seed_changedqty, struct TDYN *);

int
net_tess3d (struct TESS PTess, int poly, struct SEEDSET SSet,
            int TessId, struct MTESS *pMTess, struct TESS *pTess)
{
  int i;
  struct POLY DomPoly, *Poly = NULL;
  struct TDYN TD;
  NFTREE *nf_index = nullptr;
  NFCLOUD nf_cloud;
  int *seedid_ptid = NULL, *ptid_seedid = NULL;

  neut_tess_set_zero (pTess);

  neut_tdyn_set_zero (&TD);
  neut_poly_set_zero (&DomPoly);

  net_tess_poly (PTess, poly, &DomPoly);

  // FacePoly must not contain positive polys; quickfix
  if (ut_array_1d_int_max (DomPoly.FacePoly + 1, DomPoly.FaceQty) > 0)
  {
    for (i = 1; i <= DomPoly.FaceQty; i++)
      DomPoly.FacePoly[i] = -i;
  }

  net_polycomp (DomPoly, SSet, &nf_cloud, &nf_index, &ptid_seedid,
                &seedid_ptid, &Poly, NULL, -1, &TD);

  net_polys_tess (PTess.Level + 1, SSet, TessId, Poly, pTess);

  net_tess3d_domain (PTess, poly, TessId, pMTess, pTess);

  if (!strcmp ((*pTess).Type, "periodic")
      || !strncmp ((*pTess).DomType, "rodrigues", 9))
    net_tess3d_periodic (pTess);

  neut_poly_free (&DomPoly);
  neut_poly_array_free (&Poly, SSet.N);
  neut_tdyn_free (&TD);

  delete nf_index;
  ut_free_1d_int (&seedid_ptid);
  ut_free_1d_int (&ptid_seedid);

  return 0;
}
