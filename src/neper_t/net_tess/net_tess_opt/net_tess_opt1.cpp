/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_.hpp"
#include"neut_struct_nfcloud.hpp"
#include"neut_struct_qcloud.hpp"

int
net_tess_opt (struct IN_T In, int level, char *optitype, char *optistring, struct TESS *Tess,
              int dtess, int dcell, int TessId, struct MTESS *pMTess,
              struct SEEDSET *SSet)
{
  int i, reg;
  struct TOPT TOpt;
  NFCLOUD nf_cloud;
  NFTREE *nf_tree = nullptr;
  QCLOUD qcloud;
  my_kd_tree_t *qtree = NULL;

  neut_topt_set_zero (&TOpt);
  TOpt.pnf_cloud = &nf_cloud;
  TOpt.pnf_tree = &nf_tree;

  TOpt.tarOdf.Sp.pqcloud = &qcloud;
  TOpt.tarOdf.Sp.pqtree = &qtree;

  // Initializing optimization
  net_tess_opt_init (In, level, optitype, optistring, *pMTess, Tess, dtess, dcell, TessId, SSet, &TOpt);

  // Running optimization
  reg = -1;
  for (i = 0; i < TOpt.tarqty; i++)
    if (strstr (TOpt.tarvar[i], "sel"))
      reg = i;

  if (reg == -1 || TOpt.tarqty == 1)
  {
    if (!strcmp (TOpt.optitype, "morpho"))
      ut_print_message (0, 2, "Running tessellation...\n");
    else if (!strcmp (TOpt.optitype, "ori"))
      ut_print_message (0, 2, "Optimizing orientations (general minimization)...\n");
    else
      abort ();

    net_tess_opt_comp (&TOpt);
  }

  else
  {
    ut_print_message (0, 2, "Running tessellation (`%s' excluded)...\n", TOpt.tarvar[reg]);
    // excluding [r]sel
    TOpt.taractive[reg] = 0;
    net_tess_opt_comp (&TOpt);
    // including [r]sel
    TOpt.taractive[reg] = 1;
    // adding val termination criterion than corresponds to the previous value corrected by
    // the number of variables
    TOpt.val = TOpt.objvalmin[TOpt.iter] * sqrt ((double) (TOpt.tarqty - 1) / TOpt.tarqty);
    TOpt.iter = 0;
    sprintf (TOpt.message, "\b");
    ut_print_message (0, 2, "Running tessellation (`%s' included)...\n", TOpt.tarvar[reg]);
    ut_print_message (0, 3, "Target solution   ftar=%.9f\n", TOpt.val);
    net_tess_opt_comp (&TOpt);
  }

  // Recording tessellation
  net_tess_opt_post (pMTess, Tess, dtess, dcell, TessId, TOpt, SSet);

  neut_topt_free (&TOpt);

  return 0;
}
