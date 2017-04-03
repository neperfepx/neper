/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_.h"

int
net_tess (struct IN_T In, int level, struct TESS *Tess, int dtess,
	  int dcell, struct SEEDSET *SSet, int TessId,
	  struct MTESS *pMTess)

{
  int status = -1, periodic;
  int dim = (In.levelqty > 1 && In.dim == 2) ? 3 : In.dim;
  struct TESS Dom;

  neut_tess_set_zero (&Dom);

  // initializing domain (do not use net_tess_poly_tess for the
  // non-periodic case as it will loose domain information needed for
  // multiscale).
  periodic = ut_array_1d_int_sum (In.periodic, 3) && (level == 1);
  if (periodic)
    net_tess_perdomain (In, Tess[dtess], dcell, &Dom);
  else
    neut_tess_poly_tess (Tess[dtess], dcell, &Dom);

  // regular tessellations: cube and square
  if (!strncmp (In.morpho[level], "cube", 4)
   || !strncmp (In.morpho[level], "square", 6))
    status = net_tess_cube (In.morpho[level], dim, Tess[dtess],
			    dcell, TessId, Tess + TessId, SSet + TessId);

  // standard Voronoi/Laguerre tessellation
  else if (!strncmp (In.morpho[level], "lamellar", 8))
    status = net_tess_lam (In, level, pMTess, Tess, dtess, dcell,
			   Dom, TessId, SSet);

  // other tessellations
  else
    status = net_tess_opt (In, level, Tess, dtess, dcell, TessId,
			   pMTess, SSet);

  // finalizing
  if (!status)
  {
    if (dim == 2 && Tess[TessId].Dim == 3)
      net_tess_3dto2d (Tess + TessId);

    net_tess_finalize (Tess[dtess], SSet[TessId], Tess + TessId);
  }

  neut_tess_free (&Dom);

  return status;
}
