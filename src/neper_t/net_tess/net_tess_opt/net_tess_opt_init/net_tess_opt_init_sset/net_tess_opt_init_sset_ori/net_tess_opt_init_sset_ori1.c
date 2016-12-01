/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori (struct IN_T In, struct SEEDSET *pSSet)
{
  if (!strcmp (In.oridistrib, "3D"))
    net_tess_opt_init_sset_ori_3d (pSSet);

  else if (!strncmp (In.oridistrib, "fibre", 5))
    net_tess_opt_init_sset_ori_fibre (In.oridistrib, pSSet);

  else
    abort ();

  net_tess_opt_init_sset_ori_crysym (In, pSSet);

  return;
}
