/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_stdtriangle_.h"

void
net_domain_stdtriangle (char *stdtriangle, struct POLY *pPoly)
{
  struct PF Pf;

  neut_pf_set_zero (&Pf);

  net_domain_stdtriangle_parms (stdtriangle, &Pf);
  net_domain_stdtriangle_compute (Pf, pPoly);

  neut_pf_free (&Pf);

  return;
}
