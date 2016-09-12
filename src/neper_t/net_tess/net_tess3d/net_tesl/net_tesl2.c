/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tesl_.h"

/* Recording of the tessellation generalities in Tesl, i.e. number
 * of poly, vertices, faces and edges.
 */
void
TessGenDet (struct SEEDSET SSet, struct POLY *Poly, struct TESL *pTesl)
{
  (*pTesl).Id = SSet.Id;

  (*pTesl).PolyQty = TPolyQty (SSet);
  (*pTesl).FaceQty = TFaceQty (SSet, Poly);
  (*pTesl).VerQty = TVerQty (SSet, Poly);
  (*pTesl).EdgeQty = TEdgeQty (SSet, Poly);

  return;
}

void
TessAlloc (struct TESL *pTesl)
{
  (*pTesl).FacePoly = ut_alloc_2d_int ((*pTesl).FaceQty + 1, 2);
  (*pTesl).FaceEq = ut_alloc_2d ((*pTesl).FaceQty + 1, 4);
  (*pTesl).FaceVerQty = ut_alloc_1d_int ((*pTesl).FaceQty + 1);
  (*pTesl).FaceVerNb = ut_alloc_1d_pint ((*pTesl).FaceQty + 1);
  (*pTesl).FaceEdgeNb = ut_alloc_1d_pint ((*pTesl).FaceQty + 1);
  (*pTesl).FaceEdgeOri = ut_alloc_1d_pint ((*pTesl).FaceQty + 1);
  (*pTesl).VerSeed = ut_alloc_2d_int ((*pTesl).VerQty + 1, 4);
  (*pTesl).VerCoo = ut_alloc_2d ((*pTesl).VerQty + 1, 3);
  (*pTesl).SeedCoo = ut_alloc_2d ((*pTesl).PolyQty + 1, 3);
  (*pTesl).SeedWeight = ut_alloc_1d ((*pTesl).PolyQty + 1);
  (*pTesl).PolyVerQty = ut_alloc_1d_int ((*pTesl).PolyQty + 1);
  (*pTesl).PolyFaceQty = ut_alloc_1d_int ((*pTesl).PolyQty + 1);
  (*pTesl).PolyVerNb = ut_alloc_1d_pint ((*pTesl).PolyQty + 1);
  (*pTesl).PolyFaceNb = ut_alloc_1d_pint ((*pTesl).PolyQty + 1);
  (*pTesl).PolyFaceOri = ut_alloc_1d_pint ((*pTesl).PolyQty + 1);
  (*pTesl).PolyEdgeQty = ut_alloc_1d_int ((*pTesl).PolyQty + 1);
  (*pTesl).PolyEdgeNb = ut_alloc_1d_pint ((*pTesl).PolyQty + 1);
  (*pTesl).EdgeVerNb = ut_alloc_1d_pint (1);
  (*pTesl).EdgeVerNb[0] = NULL;

  return;
}

/* Recording of TFNb & TVNb */
void
IniTxNb (struct TESL Tesl, struct POLY *Poly, int **TFNb, int **TVNb)
{
  /* Recording of TFNb */
  IniTFNb (Tesl, Poly, TFNb);
  /* Recording of TVNb */
  IniTVNb (Tesl, Poly, TVNb);

  return;
}

/* For each polyhedron, recording of FacePoly, FaceEq & FaceVerQty */
void
IniTessFaceOr (struct TESL *pTesl, struct POLY *Poly, int **TFNb)
{
  /* Recording of Face seed */
  IniTessFacePoly (pTesl, Poly, TFNb);
  /* Recording of Face eq */
  IniTessFaceEq (pTesl, Poly, TFNb);
  /* Recording of Face ver qty */
  IniTessFaceVerQty (pTesl, Poly, TFNb);

  return;
}

/* For each polyhedron, recording of FaceVerNb */
void
IniTessFaceBound (struct TESL *pTesl, struct POLY *Poly, int **TFNb,
		  int **TVNb)
{
  int i, j, k;
  int FNb;

  /* for every face of every polyhedron */
  for (i = 1; i <= (*pTesl).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      /* if it has not been treated previously */
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
	/* the face tesl number is recorded */
	FNb = TFNb[i][j];

	(*pTesl).FaceVerNb[FNb] =
	  ut_alloc_1d_int ((*pTesl).FaceVerQty[FNb] + 1);

	for (k = 1; k <= (*pTesl).FaceVerQty[FNb]; k++)
	  (*pTesl).FaceVerNb[FNb][k] = TVNb[i][Poly[i].FaceVerNb[j][k]];
      }

  return;
}

/* Recording of vertex properties */
void
IniTessVer (struct TESL *pTesl, struct POLY *Poly, int **TVNb)
{
  /* Recording of the vertex coordinates */
  IniTessVerCoo (pTesl, Poly, TVNb);
  /* Recording of the numbers of the ver parent seeds */
  IniTessVerSeed (pTesl, Poly, TVNb);

  return;
}

/* Recording of the numbers of face and vertex of the polyhedra */
void
IniTessPolyCenNXQty (struct SEEDSET SSet, struct TESL *pTesl,
		     struct POLY *Poly)
{
  int num;
  int i;

  for (num = 1; num <= (*pTesl).PolyQty; num++)
  {
    for (i = 0; i < 3; i++)
      (*pTesl).SeedCoo[num][i] = SSet.SeedCoo[num][i];
    if (!SSet.LamWidth)
    {
      if (SSet.SeedWeight)
	(*pTesl).SeedWeight[num] = SSet.SeedWeight[num];
    }
    else
      (*pTesl).SeedWeight[num] = SSet.LamWidth[num];

    (*pTesl).PolyFaceQty[num] = Poly[num].FaceQty;
    (*pTesl).PolyVerQty[num] = Poly[num].VerQty;
  }

  return;
}

/* Recording of the face and vertex numbers */
void
IniTessPolyXNb (struct TESL *pTesl, struct POLY *Poly, int **TVNb, int **TFNb)
{
  int num, i;

  for (num = 1; num <= (*pTesl).PolyQty; num++)
  {
    (*pTesl).PolyVerNb[num] = ut_alloc_1d_int (Poly[num].VerQty + 1);
    for (i = 1; i <= Poly[num].VerQty; i++)
      (*pTesl).PolyVerNb[num][i] = TVNb[num][i];

    (*pTesl).PolyFaceNb[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pTesl).PolyFaceNb[num][i] = TFNb[num][i];

    (*pTesl).PolyFaceOri[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pTesl).PolyFaceOri[num][i] = IniPolyFaceOri (pTesl, num, i);
  }

  return;
}

/* Recording of the edges */
int
IniTessEdge (struct TESL *pTesl)
{
  int i, j;			/* mute variable.                                           */
  int nb = 0;			/* nb of the currently added edge.                          */
  int S1, S2;			/* numbers of the two vertices on which studied segment is  */
  /* based.                                                   */
  int PrevF;			/* number of the previous face, in which the studied        */
  /* edge has been recorded.                                  */

  /* For every face, for every couple of neighbor vertices, we search
   * if the edge has already been treated.
   * We first search the three common seeds of these vertices.
   * These 3 neighboring seeds have generated 3 faces whose studied
   * edge belongs to boundary.
   * If the number of the current studied face is the lowest, a new edge
   * has to be recorded. Else, we must found the number of the
   * previously-defined edge which corresponds to this one.
   * To do that, we should consider the lowest number of the 3 faces.
   * A first approach would be to search in VerNb the number of the edge
   * whose extremities are the two considered vertices.
   * Another one would be to write (VerNei &) VerEdge and to access these
   * information more rapidly
   */

  for (i = 1; i <= (*pTesl).FaceQty; i++)
  {
    (*pTesl).FaceEdgeNb[i] = ut_alloc_1d_int ((*pTesl).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTesl).FaceVerQty[i]; j++)
    {
      /* Recording of the 2 vertex numbers */
      S1 = (*pTesl).FaceVerNb[i][j];
      if (j != (*pTesl).FaceVerQty[i])
	S2 = (*pTesl).FaceVerNb[i][j + 1];
      else
	S2 = (*pTesl).FaceVerNb[i][1];

      /* TreatFaceVer returns a (strictly) positive value if the
       * ver been recorded previously, 0 else.
       * If 0, the vertex number has to be created, else it has
       * to be searched among the edges of face whose number has
       * been returned.
       */
      PrevF = TreatFaceVer (pTesl, i, S1, S2);
      if (PrevF == 0)
	/* Creation of new edge nb through the incrementation of nb
	 * and the writing of:
	 * EdgeVerNb[nb][0,1]
	 * FaceEdgeNb[i][S1]
	 */
	nb = AddEdge (pTesl, i, j, S1, S2, nb);
      else if (PrevF > 0)
	/* Search of the previously-given number of the edge
	 * to record it in FaceEdgeNb.
	 */
	SearchEdge (pTesl, i, PrevF, S1, S2, j);
      else
	return -1;
    }
  }

  /* Recording of FaceEdgeOri */
  IniTessFaceEdgeOri (pTesl);

  /* Recording of Tesl.PolyEdgeX */
  IniTessPolyEdge (pTesl);

  /* Correction of (*pTesl).EdgeQty */
  (*pTesl).EdgeQty = nb;

  return 0;
}

/* ************************************************************************
 * Recording of the edge lengths from the edge properties, and of the
 * vertex parent edges.
 */
void
IniTessVernEdge (struct TESL *pTesl)
{
  int i, j;			/* mute variables */
  int ver;			/* tmp variable = ver nb */
  int *nb;			/* tmp variable = id of the ver parent edge to be recorded */

  (*pTesl).VerOfDomain = ut_alloc_1d_int (1);
  (*pTesl).VerEdge = ut_alloc_2d_int ((*pTesl).VerQty + 1, 4);

  nb = ut_alloc_1d_int ((*pTesl).VerQty + 1);

  /* for every edge of the tessellation, its number (id) is assigned to
   * its two vertices.
   */
  for (i = 1; i <= (*pTesl).EdgeQty; i++)
    for (j = 0; j <= 1; j++)
    {
      ver = (*pTesl).EdgeVerNb[i][j];
      (*pTesl).VerEdge[ver][nb[ver]] = i;
      nb[ver]++;
    }

  /* The number of the vertices which are vertex of the cubic domain are
   * recorded.
   */
  for (i = 1; i <= (*pTesl).VerQty; i++)
  {
    /*printf("%d: %d %d %d %d\n",i,(*pTesl).VerEdge[i][0],(*pTesl).VerEdge[i][1],(*pTesl).VerEdge[i][2],(*pTesl).VerEdge[i][3]); */
    if (nb[i] == 3)
    {
      (*pTesl).VerOfDomain[0]++;
      (*pTesl).VerOfDomain =
	ut_realloc_1d_int ((*pTesl).VerOfDomain, (*pTesl).VerOfDomain[0] + 1);
      (*pTesl).VerOfDomain[(*pTesl).VerOfDomain[0]] = i;
    }
  }

  ut_free_1d_int (nb);

  return;
}
