/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_group_.h"

void
net_group (struct IN_T In, struct TESS *pTess, struct TESR *pTesr)
{
  if (!neut_tess_isvoid (*pTess))
    net_group_tess (In, pTess);

  if (!neut_tesr_isvoid (*pTesr))
    net_group_tesr (In, pTesr);

  return;
}
