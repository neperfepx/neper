/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_tess_.h"

/* The parent seeds of the 3 parent faces of the vertex are recorded, with
 * PrevP replaced by PNb.
 */
void
net_tess3d_tess_txnb_tvnb_search_prevver_parfacepoly (struct POLY *Poly, int PNb, int VNb,
                                                      int PrevP, int *PFG)
{
  int i;

  for (i = 0; i <= 2; i++)
    PFG[i] = Poly[PNb].FacePoly[Poly[PNb].VerFace[VNb][i]];

  PFG[ut_array_1d_int_eltpos (PFG, 3, PrevP)] = PNb;

  return;
}

/* We search the vertex in polyhedron PrevP whose parent face parent seeds
 * are PFG.  It is a vertex of the face of PrevP of parent seed PNb (only
 * the vertices of this face are checked).  The lowest such vertex is
 * returned.
 */
int
net_tess3d_tess_txnb_tvnb_search_prevver_search (struct POLY *Poly, int PNb, int PrevP, int *PFG)
{
  int i, j, k, face, ver = 0;

  for (face = 1; face <= Poly[PrevP].FaceQty; face++)
    if (Poly[PrevP].FacePoly[face] == PNb)
      for (k = 1; k <= Poly[PrevP].FaceVerQty[face]; k++)
      {
        i = Poly[PrevP].FaceVerNb[face][k];

        for (j = 0; j < 3; j++)
          if (ut_array_1d_int_eltpos (PFG, 3, Poly[PrevP].FacePoly[Poly[PrevP].VerFace[i][j]])
              == -1)
            break;

        if (j == 3 && (!ver || i < ver))
          ver = i;
      }

  return ver;
}
