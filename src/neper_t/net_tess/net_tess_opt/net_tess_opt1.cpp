/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_.hpp"
#include"neut/neut_structs/neut_nanoflann_struct.hpp"

int
net_tess_opt (struct IN_T In, int level, struct TESS *Tess, int dtess,
	      int dcell, int TessId, struct MTESS *pMTess,
	      struct SEEDSET *SSet)
{
  struct TOPT TOpt;
  NFCLOUD nf_cloud;
  NFTREE  *nf_tree = nullptr;

  neut_topt_set_zero (&TOpt);
  TOpt.pnf_cloud = &nf_cloud;
  TOpt.pnf_tree  = &nf_tree;

  if (strcmp (In.morpho[level], "voronoi"))
  {
    printf ("\n");
    ut_print_message (0, 3, "Entering optimization...\n");
  }

  // Initializing optimization
  net_tess_opt_init (In, level, *pMTess, Tess, dtess, dcell, SSet, &TOpt);

  // Running optimization
  net_tess_opt_comp (&TOpt);

  // Recording tessellation
  net_tess_opt_post (pMTess, Tess, dtess, dcell, TessId,
		     TOpt.Poly, TOpt, SSet);

  neut_topt_free (&TOpt);

  return 0;
}
