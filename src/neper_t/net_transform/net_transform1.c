/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_.h"

void
net_transform (struct IN_T In, struct TESS *pDom, struct TESS *pTess, struct TESR *pTesr)
{
  if ((*pTess).Dim > 0)
    net_transform_tess (In, pDom, pTess);

  if ((*pTesr).Dim > 0)
    net_transform_tesr (In, pTesr);

  return;
}
