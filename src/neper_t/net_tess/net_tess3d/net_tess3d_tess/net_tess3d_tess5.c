/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_tess_.h"

/* We search the number of a polyhedron in which the studied vertex has
 * been recorded previously, as the first parent seed of the parent faces
 * of the vertex that is positive and lower than PNb.
 */
int
net_tess3d_tess_txnb_tvnb_search_prevpoly (struct POLY *Poly, int PNb, int VNb)
{
  int i, PrevP = 0;

  for (i = 0; i < 3; i++)
  {
    PrevP = Poly[PNb].FacePoly[Poly[PNb].VerFace[VNb][i]];

    if (PrevP < PNb && PrevP > 0)
      break;
  }

  return PrevP;
}

/* We search the number of the studied vertex in polyhedron PrevP.  It is
 * such that the seeds of its 3 parent faces are the same as those of the
 * vertex of polyhedron PNb, except for PrevP which is replaced by PNb.
 */
int
net_tess3d_tess_txnb_tvnb_search_prevver (struct POLY *Poly, int PNb, int VNb, int PrevP)
{
  int PFG[3];

  net_tess3d_tess_txnb_tvnb_search_prevver_parfacepoly (Poly, PNb, VNb, PrevP, PFG);

  return net_tess3d_tess_txnb_tvnb_search_prevver_search (Poly, PNb, PrevP, PFG);
}
