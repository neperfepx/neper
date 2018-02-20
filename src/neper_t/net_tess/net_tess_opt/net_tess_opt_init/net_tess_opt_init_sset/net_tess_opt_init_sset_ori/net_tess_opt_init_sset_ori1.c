/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori (struct IN_T In, int level, struct MTESS MTess,
                            struct TESS *Tess, int dtess, int dcell,
			    struct SEEDSET *SSet,
                            struct SEEDSET *pSSet)
{
  int status;
  char *ori = NULL;

  net_multiscale_mtess_arg_0d_char_fscanf (MTess, Tess, dtess, dcell,
                                           In.oridistrib[level], &ori);

  if (!strcmp (ori, "3d") || !strcmp (ori, "3D"))
    net_tess_opt_init_sset_ori_3d (pSSet);

  else if (!strcmp (ori, "equal"))
  {
    if (level > 1)
      net_tess_opt_init_sset_ori_equal (SSet, dtess, dcell, pSSet);
    else
      ut_print_message (2, 2, "Option `-ori equal' not available at scale 1.\n");
  }

  else if (!strncmp (ori, "fibre", 5))
    net_tess_opt_init_sset_ori_fibre (ori, pSSet);

  else if (!strncmp (ori, "file(", 5))
    net_tess_opt_init_sset_ori_file (ori, pSSet);

  else
  {
    status = net_tess_opt_init_sset_ori_label (ori, pSSet);

    if (status)
      abort ();
  }

  net_tess_opt_init_sset_ori_crysym (In, MTess, Tess, dtess, dcell, pSSet);

  ut_free_1d_char (ori);

  return;
}
