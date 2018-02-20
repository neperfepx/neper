/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_cube_.h"

void
CubeVer (double **size, struct POLY *pDomain)
{
  IniVerQty (pDomain);
  IniVerFace (pDomain);
  IniVerCoo (size, pDomain);

  return;
}

void
CubeFaces (double **size, struct POLY *pDomain)
{
  IniFaceQty (pDomain);
  IniFaceEq (size, pDomain);
  IniFacePoly (pDomain);
  IniFaceVerQty (pDomain);
  IniFaceVerNb (pDomain);

  return;
}
