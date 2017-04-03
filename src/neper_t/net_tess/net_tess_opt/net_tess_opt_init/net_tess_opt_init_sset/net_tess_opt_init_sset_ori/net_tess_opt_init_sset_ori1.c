/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori (struct IN_T In, int level, struct SEEDSET *SSet,
                            int dtess, int dcell, struct SEEDSET *pSSet)
{
  int status;

  if (!strcmp (In.oridistrib[level], "3d")
   || !strcmp (In.oridistrib[level], "3D"))
    net_tess_opt_init_sset_ori_3d (pSSet);

  else if (!strcmp (In.oridistrib[level], "equal"))
  {
    if (level > 1)
      net_tess_opt_init_sset_ori_equal (SSet, dtess, dcell, pSSet);
    else
      ut_print_message (2, 2, "Option `-ori equal' not available at scale 1.\n");
  }

  else if (!strncmp (In.oridistrib[level], "fibre", 5))
    net_tess_opt_init_sset_ori_fibre (In.oridistrib[level], pSSet);

  else if (!strncmp (In.oridistrib[level], "file(", 5))
    net_tess_opt_init_sset_ori_file (In.oridistrib[level], pSSet);

  else
  {
    status = net_tess_opt_init_sset_ori_label (In.oridistrib[level], pSSet);

    if (status)
      abort ();
  }

  net_tess_opt_init_sset_ori_crysym (In, pSSet);

  return;
}
