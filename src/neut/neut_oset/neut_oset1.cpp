/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_oset_.hpp"

void
neut_oset_kdtree (struct OL_SET *pOSet, struct QCLOUD *pqcloud,
                  my_kd_tree_t ** pqindex)
{
  neut_oset_kdtree_cloud (pOSet, pqcloud);
  neut_oset_kdtree_build (pqcloud, pqindex);

  return;
}
