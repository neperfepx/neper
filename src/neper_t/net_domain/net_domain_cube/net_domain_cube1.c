/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_cube_.h"

/* Domain:
 *
 */
void
net_domain_cube_string (char *domain, struct POLY *pDomain)
{
  int pseudodim;
  double pseudosize;
  double **size = ut_alloc_2d (3, 2);

  net_domain_cubeparms (domain, size, &pseudodim, &pseudosize);
  net_domain_cube (size, pDomain);

  if (pseudodim != -1)
  {
    (*pDomain).PseudoDim = pseudodim;
    (*pDomain).PseudoSize = pseudosize;
  }

  ut_free_2d (&size, 3);

  return;
}

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
