/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_uniform_.h"

void
net_ori_uniform (struct IN_T In, int level, struct MTESS MTess,
                 struct TESS *Tess, int dtess, int dcell, long random,
                 struct OL_SET *pOSet, int verbositylevel)
{
  struct OOPT OOpt;

  neut_oopt_set_zero (&OOpt);

  net_ori_uniform_init (In, level, MTess, Tess, dtess, dcell, random, pOSet,
                        &OOpt, verbositylevel);

  net_ori_uniform_opt (In, &OOpt, pOSet, verbositylevel);

  neut_oopt_free (&OOpt);

  return;
}
