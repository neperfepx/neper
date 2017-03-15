/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_cube_.h"

/* Domain:
 *
 */
void
net_domain_cube (double **size, struct POLY *pDomain)
{
  /* initialization of the vertices */
  CubeVer (size, pDomain);
  /* initialization of the faces */
  CubeFaces (size, pDomain);

  return;
}
