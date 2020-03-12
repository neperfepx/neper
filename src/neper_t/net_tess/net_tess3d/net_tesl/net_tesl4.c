/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tesl_.h"

/* TreatVer returns 1 if the ver has not been recorded previously,
 * 0 else. If 1, the vertex number has to be created, else it has
 * to be searched.
 */
int
TreatVer (struct POLY *Poly, int PNb, int VNb)
{
  int status;
  int i;                        /* mute variable */
  int *ParFace = ut_alloc_1d_int (3);   /* 3 parent faces of the vertex */

  for (i = 0; i < 3; i++)
    ParFace[i] = Poly[PNb].VerFace[VNb][i];

  status = 0;
  if (PNb < Poly[PNb].FacePoly[ParFace[0]]
      || Poly[PNb].FacePoly[ParFace[0]] < 0)
    if (PNb < Poly[PNb].FacePoly[ParFace[1]]
        || Poly[PNb].FacePoly[ParFace[1]] < 0)
      if (PNb < Poly[PNb].FacePoly[ParFace[2]]
          || Poly[PNb].FacePoly[ParFace[2]] < 0)
        status = 1;

  ut_free_1d_int (&ParFace);

  return status;
}

int
AddVer (int **TVNb, int PNb, int VNb, int nb)
{
  /* The studied vertex takes the value nb+1
   */
  TVNb[PNb][VNb] = nb + 1;

  return nb + 1;
}

void
SearchVer (struct POLY *Poly, int PNb, int VNb, int **TVNb)
{
  int PrevP;                    /* nb of a polyhedron in which the ver VNb
                                 * has been recorded previously
                                 * (prevP<PNb)
                                 */
  int PrevS;                    /* nb of the vertex in this poyhedron,
                                 * which correspond to this studied.
                                 */

  /* We search the number of a polyhedron in which the studied
   * vertex has been recorded previously. It has a common face
   * with poly PNb.
   */
  PrevP = PrevPoly (Poly, PNb, VNb);

  /* We search now the number of the studied vertex in
   * this polyhedron: PrevS
   */
  PrevS = PrevVer (Poly, PNb, VNb, PrevP);

  /* The new ver corresponding of that of poly PNb and ver VNb
   * takes the value of that from PrevP and PrevS.
   */
  UpdTVNb (TVNb, PNb, VNb, PrevP, PrevS);

  return;
}

/* CommonSeeds searchs the common parent seeds of 2 vertices.
 * It returns their numbers (nb) and their quantity (qty).
 */
int
CommonSeeds (struct TESL Tesl, int first, int second, int *nb)
{
  int i, j;
  int qty = 0;

  for (i = 0; i <= 3; i++)
    for (j = 0; j <= 3; j++)
      if (Tesl.VerSeed[first][i] == Tesl.VerSeed[second][j])
      {
        nb[qty] = Tesl.VerSeed[first][i];
        qty++;
      }

  return qty;
}

/* BissFace returns the number of the bissecting face of segment
 * based on 2 polyhedron centers (S1 & S2).
 * 3 cases can occur:
 * (i) the face exists: its number is returned.
 * (ii) the face does not exist: -1 is returned.
 * (iii) the two vertex numbers are negative: the face cannot
 * exist: 0 is returned.
 *
 */
int
BissFace (struct TESL Tesl, int S1, int S2)
{
  int i;                        /* mute variables       */
  int f;                        /* temporary face nb    */
  int N = Tesl.PolyQty;

  /* S1 & S2 are negative: the bissecting face cannot exist:
   * 0 is returned.
   */
  if ((S1 < 0 || S1 > N) && (S2 < 0 || S2 > N))
    return 0;

  /* Below, the bissecting face is searched using polyhedron S1.
   */

  /* If S1 is negative, it take the (positive) value of S2.
   * This change affects only this routine.
   */
  if (S1 < 0 || S1 > N)
    ut_num_switch_int (&S1, &S2);

  /* The bissecting face is searched.
   * S1 faces are considered successively.
   * If the studied face has for parent seed S2, its number is returned.
   * If it is not the case for any of the faces, S1 & S2 bissecting face
   * does not exist among the tessellation: -1 is returned.
   */
  for (i = 1; i <= Tesl.PolyFaceQty[S1]; i++)
  {
    f = Tesl.PolyFaceNb[S1][i];
    if (ut_array_1d_int_eltpos (Tesl.FacePoly[f], 2, S2) != -1)
      return f;
  }

  return -1;
}

void
RecEdgeVerNb (struct TESL *pTesl, int nb, int S1, int S2)
{

  (*pTesl).EdgeVerNb[nb][0] = S1;
  (*pTesl).EdgeVerNb[nb][1] = S2;

  return;
}

void
RecFaceEdgeNb (struct TESL *pTesl, int FNb, int i, int nb)
{
  (*pTesl).FaceEdgeNb[FNb][i] = nb;

  return;
}

/* Recording of the edge numbers of the polyhedron.
 */
int
RecPolyEdges (struct TESL *pTesl, int PNb)
{
  int i, FNb;
  int qty = 0;

  // initializing alloc
  (*pTesl).PolyEdgeNb[PNb] = ut_alloc_1d_int (1);

  for (i = 1; i <= (*pTesl).PolyFaceQty[PNb]; i++)
  {
    /* The face number is recorded
     */
    FNb = (*pTesl).PolyFaceNb[PNb][i];
    /* The face edges that are not recorded as polyhedron
     * edges yet as so recorded.
     */
    qty = FaceEdges2PolyEdges (pTesl, PNb, FNb, qty);
  }

  return qty;
}
