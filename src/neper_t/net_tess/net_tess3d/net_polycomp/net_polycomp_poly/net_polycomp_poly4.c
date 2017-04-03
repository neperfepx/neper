/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_poly_.h"

/* VerUseUpdating modifies pPolymod.VerUse: VerUse[i]<--0 if the ith
 * vertex is in the bad side.
 */
int
VerUseUpdating (POLYMOD * pPolymod, int *BadVer)
{
  int i;

  /* VerUse[] takes the value 0 for the bad vertices. */
  for (i = 1; i <= BadVer[0]; i++)
    (*pPolymod).VerUse[BadVer[i]] = 0;

  return ut_array_1d_int_sum ((*pPolymod).VerUse + 1,
			      (*pPolymod).VerQty) > 0 ? 0 : -1;
}

/* FaceUseUpdating modifies pPolymod.FaceUse: FaceUse[i]<--0 if the ith
 * face has to be suppressed.
 */
void
FaceUseUpdating (POLYMOD * pPolymod, int *BadVer)
{
  int face;

  /* Every face is tested successively. */
  for (face = 1; face <= (*pPolymod).FaceQty; face++)
    /* if the face is used, it is tested.   */
    if ((*pPolymod).FaceUse[face] == 1)
      /* tests if vertices belonging to the face is still useful. */
      /* CheckFace returns 1 if the face is still useful          */
      /* (FaceUse=1); 0 else.                                     */
      (*pPolymod).FaceUse[face] = CheckFace (BadVer, *pPolymod, face);

  return;
}

/* CheckFace returns 1 if the face is still useful
 * (FaceUse=1); 0 else.
 */
int
CheckFace (int *BadVer, struct POLYMOD Polymod, int face)
{
  /* mute variable */
  int sum;

  /* for each vertex of the face, one checks if is still
   * belong to the polyhedron or not. If yes, the value 1 is
   * returned. Indeed, whenever a vertex is useful, the tested
   * face is useful. If all vertices are useless, the face is
   * useless: the program returns 0.
   */
  for (sum = 1; sum <= Polymod.FaceVerQty[face]; sum++)
    /* whenever a vertex of the face does not belong to vertices,
     * the face must be kept: the value 1 is returned.
     */
    if (ut_array_1d_int_eltpos
	(BadVer + 1, BadVer[0], Polymod.FaceVerNb[face][sum]) == -1)
      return 1;

  /* Else, 0 is returned. */
  return 0;
}

/* NewFace adds the new face by increasing FaceQty and through the
initializations of FaceEq, FacePoly & FaceUse. */
void
NewFace (int Nei, POLYMOD * pPolymod, double *plane)
{

  /* The quantity of faces (useful or not) is incremented. */
  UpdFaceQty (pPolymod);

  /* The new face is recorded. */
  UpdFaceEq (pPolymod, plane);

  /* The face is set as useful. */
  UpdFaceUse (pPolymod);

  /* The seed of which the new face is associated is recorded. */
  UpdFacePoly (pPolymod, Nei);

  (*pPolymod).FaceVerQty
    = ut_realloc_1d_int ((*pPolymod).FaceVerQty, (*pPolymod).FaceQty + 1);
  (*pPolymod).FaceVerQty[(*pPolymod).FaceQty] = 0;

  (*pPolymod).FaceVerNb
    = ut_realloc_1d_pint ((*pPolymod).FaceVerNb, (*pPolymod).FaceQty + 1);
  (*pPolymod).FaceVerNb[(*pPolymod).FaceQty] = ut_alloc_1d_int (1);

  return;
}

/* FnLFacesDet determines the first and last faces to be modified.
 * These are two neighbor faces cut by the new plane, and are
 * recorded as FnLFaces[0] and FnLFaces[1].
 * To find a face cut by the new plane, the bad vertices are considered
 * successively and, for each of them, the three parent faces are treated.
 * One searches a couple (bad vertex, face) such that the new plane cut the
 * face boundary through a segment composed of the bad vertex (and an other
 * -good- vertex).
 */
void
FnLFacesDet (POLYMOD Polymod, int *BadVer, int *FnLFaces)
{
  int i;			/* mute variable                                        */
  int ns, nf;			/* number of the vertex in BadVer and number of the     */
  /* considered face.                                     */
  int nb;			/* in fine, to delete                                   */
  int *Ver;			/* Ver[1] <-- number of the considered vertex           */
  /* Ver[0] <-- number of the "previous" neighbour        */
  /* Ver[2] <-- number of the "next" neighbour            */

  Ver = ut_alloc_1d_int (3);

  /* for each bad vertex of the polyhedron... temporarly named Ver[1],
   * successively and while FnLFaces[0]==0
   * the 3 faces whose vertex is intersection are considered
   * successively: VerFace[Ver[1]][nf], nf=0 to 2
   * The face number is recorded as face.
   */
  for (ns = 1; FnLFaces[0] == 0; ns++)
    for (nf = 0; nf <= 2; nf++)
    {
      /* SnNS returns the number of the considered vertex Ver[1], and
       * those of its two neighbours: Ver[0] & Ver[2].
       */
      SnNS (Polymod, ns, nf, Ver, BadVer);

      /* FaceVerNb[face][pPos], i.e. one of the two neighbor vertices
       * of FaceVerNb[face][Pos], is searched out in vertices.
       * If it belongs to BadVer, tmpPos!=-1, and these two vertices are in
       * the bad side of the new plane. Else (what we want), vertex Pos is
       * in the bad side and vertex pPos is in the good side.
       */

      for (i = 0; i <= 2; i = i + 2)
	if (ut_array_1d_int_eltpos (BadVer + 1, BadVer[0], Ver[i]) == -1)
	{
	  nb = CommonFaces (Polymod, Ver[i], Ver[1], FnLFaces);
	  /* simple verification... */
	  if (nb != 2)
	    ut_error_reportbug ();

	  ut_free_1d_int (Ver);
	  return;
	}
    }

  return;
}

/* FaceModification modifies Face to take into account the new
 * face.
 */
int
FaceModif (int pFace, int Face, POLYMOD * pPolymod, int *BadVer, int last)
{
  int next = 0;			/* next face to modify */
  /* The 2 couples of vertices whose segment is cut by the new plane
   * are recorded as couple1 & couple2.
   * Difference between these
   * two couple of vertices only relies on the face recording.
   * couple1 vertices are written as FaceVerNb[1] and FaceVerNb[Max]
   * whereas couple2 vertices are written as FaceVerNb[Nb] and
   * FaceVerNb[Nb+1]. (quickly, see later). This is important for
   * the recording of the two new vertices, and the order they are
   * recorded. This is expressed through bel2.
   * Practically, the new vertices are first written one after one,
   * and are then reversed if necessary.
   */
  int *couple1;
  int *couple2;
  /* The faces associated to these couples of vertices are recorded.
   * faces1[0] and faces1[1] are the two common faces of vertices
   * couple1[0] & couple1[1]. The same for faces2. That allows to
   * know whose are the parent faces of a new vertex.
   */
  int *faces1;
  int *faces2;

  int bel2;			/* (see above) */

  int NbBadVer;			/* Number of bad vertices of the face */
  int *pNbBadVer = &NbBadVer;

  couple1 = (int *) ut_alloc_1d_int (2);
  couple2 = (int *) ut_alloc_1d_int (2);
  faces1 = (int *) ut_alloc_1d_int (2);
  faces2 = (int *) ut_alloc_1d_int (2);

  /* VerCoupleNFaces rotates FaceVerNb to place the bad vertices at the
   * end, searches couple1, couple2, faces1, faces2 and supplies bel2.
   */
  bel2 =
    VerCoupleNFaces (pPolymod, BadVer, Face, pFace, couple1, couple2, faces1,
		     faces2, pNbBadVer);

  /* Modification of Face properties to suppress the bad vertices.
   */
  FaceModifGenPara (pPolymod, Face, NbBadVer);

  /* Adding of the first new vertex.
   */
  FaceModif1stNewVer (pPolymod, Face);

  /* If this face is not the last face to modify :
   * Adding of the second new vertex.
   * Else, second vertex of the face is set to the first created vertex.
   */
  if (last == 0)
    next = FaceModif2ndNewVer (pPolymod, Face, bel2, faces1, faces2);
  else
    FaceModifAddBegVer (pPolymod, Face, last, bel2);

  /* Memory freeing for local temporary arrays.
   */
  ut_free_1d_int (couple1);
  ut_free_1d_int (couple2);
  ut_free_1d_int (faces1);
  ut_free_1d_int (faces2);

  return next;
}

/* NewVer adds the vertex which is the intersection between faces first
 * second and third, i.e. the first and last face to modified and
 * the new face.
 */
int
NewVer (POLYMOD * pPolymod, int first, int second, int third)
{
  UpdVerQty (pPolymod);
  UpdVerUse (pPolymod);
  UpdVerFace (pPolymod, first, second, third);
  UpdVerCoo (pPolymod, first, second, third);

  return (*pPolymod).VerQty;
}

/* NewFaceAddVer adds the new vertex to the new face.
 */
void
NewFaceAddVer (POLYMOD * pPolymod)
{
  UpdFaceVerQty (pPolymod);
  UpdFaceVerNb (pPolymod);

  return;
}
