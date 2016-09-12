/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_domain_.h"

int
net_tess3d_domain (struct TESS PTess, int poly, int TessId,
		   struct MTESS *pMTess, struct TESS *pTess)
{
  if (!pMTess)
    return 0;

  (*pMTess).TessDomVerNb
    = ut_realloc_1d_pint ((*pMTess).TessDomVerNb, TessId + 1);
  (*pMTess).TessDomEdgeNb
    = ut_realloc_1d_pint ((*pMTess).TessDomEdgeNb, TessId + 1);
  (*pMTess).TessDomFaceNb
    = ut_realloc_1d_pint ((*pMTess).TessDomFaceNb, TessId + 1);

  if (TessId == 1)
  {
    (*pMTess).TessDomVerNb[0] = NULL;
    (*pMTess).TessDomEdgeNb[0] = NULL;
    (*pMTess).TessDomFaceNb[0] = NULL;
  }

  neut_tess_init_domain_poly (pTess, PTess, poly,
			      &((*pMTess).TessDomVerNb[TessId]),
			      &((*pMTess).TessDomEdgeNb[TessId]),
			      &((*pMTess).TessDomFaceNb[TessId]));

  int i, domface, pos;
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    domface = (*pMTess).TessDomFaceNb[(*pTess).TessId][i];

    pos = ut_array_1d_int_eltpos (PTess.FacePoly[domface], 2, poly);
    if (pos == -1)
      ut_error_reportbug ();

    (*pMTess).DomTessFaceNb[PTess.TessId][domface][pos] = i;
  }

  return 0;
}
