/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_tess_.h"

/* net_tess3d_tess_txnb_tvnb_treat returns 1 if the ver has not been
 * recorded previously, 0 else, i.e. if its 3 parent faces have not been
 * recorded previously.
 */
int
net_tess3d_tess_txnb_tvnb_treat (struct POLY *Poly, int PNb, int VNb)
{
  int i, FacePoly;

  for (i = 0; i < 3; i++)
  {
    FacePoly = Poly[PNb].FacePoly[Poly[PNb].VerFace[VNb][i]];
    if (PNb >= FacePoly && FacePoly >= 0)
      return 0;
  }

  return 1;
}

/* Search of the number of a previously-recorded vertex */
void
net_tess3d_tess_txnb_tvnb_search (struct POLY *Poly, int PNb, int VNb, int **TVNb)
{
  int PrevP;                    /* polyhedron in which the vertex has been
                                 * recorded previously (PrevP < PNb) */
  int PrevS;                    /* number of the vertex in this polyhedron */

  PrevP = net_tess3d_tess_txnb_tvnb_search_prevpoly (Poly, PNb, VNb);

  PrevS = net_tess3d_tess_txnb_tvnb_search_prevver (Poly, PNb, VNb, PrevP);

  TVNb[PNb][VNb] = TVNb[PrevP][PrevS];

  return;
}
