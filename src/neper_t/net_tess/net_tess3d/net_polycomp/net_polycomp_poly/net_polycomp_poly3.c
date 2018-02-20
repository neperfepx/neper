/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_poly_.h"

/* BadVerDet searches the vertices that are on the bad side of the
 * bissecting plane, i.e. that of the neighbour
 */
void
BadVerDet (double *plane, int CenterSide, struct POLYMOD Polymod,
	   int **pBadVer)
{
  /* Mute variables */
  int i;

  int VerSide;

  (*pBadVer) = ut_alloc_1d_int (1);

  /* For every real vertex of the polyhedron, we check if it is
   * in the bad side or not.
   */
  for (i = 1; i <= Polymod.VerQty; i++)
    if (Polymod.VerUse[i] == 1)
    {
      VerSide = ut_space_planeside (plane, Polymod.VerCoo[i] - 1);
      /* the term -1 is added to take into account the fact that Side
       * read coordinates in [1]...[3] while they are stored in
       * [0]...[2] in VerCoo[i]
       *printf("Ver Side=%d\n",VerSide);

       * if the vertex is in the bad side, its existence is taken
       * into account: BadVer[0]++; and its number (i) is stored.
       */
      if (VerSide != CenterSide)
      {
	(*pBadVer)[0]++;
	(*pBadVer) = ut_realloc_1d_int ((*pBadVer), (*pBadVer)[0] + 2);
	(*pBadVer)[(*pBadVer)[0]] = i;
      }
    }

  return;
}

/* PolyhedronModification modifies the polyhedron to take into account
 * the new neighbour.
 */
int
PolyhedronModification (int Nei, double *plane, struct POLYMOD *pPolymod,
			int *BadVer)
{
  int status;
  int *FnLFaces = NULL;		/* first & last face to be modified         */
  int Face;			/* current face to be modified              */
  int pFace;			/* previous face that has been modified     */
  int nFace;			/* next face to modifiy                     */
  int firstVer;			/* first created vertex                     */

  /* VerUseUpdating modifies pPolymod.VerUse: VerUse[i]<--0 if the
   * ith vertex is a bad vertex.
   */
  status = VerUseUpdating (pPolymod, BadVer);
  if (status != 0)
  {
    neut_polymod_free (pPolymod);
    return -1;
  }

  /* FaceUseUpdating modifies pPolymod.FaceUse: FaceUse[i]<--0 if all
   * the vertices of face i are bad vertices.
   */
  FaceUseUpdating (pPolymod, BadVer);

  /* NewFace adds the new face by increasing FaceQty and
   * initializing FaceUse, FacePoly & FaceEq.
   */
  NewFace (Nei, pPolymod, plane);

  /* FnLFacesDet determines the first and last faces to be modified.
   * These are two neighbor faces cut by the new plane, and are
   * recorded as FnLFaces[0] and FnLFaces[1].
   */
  FnLFaces = ut_alloc_1d_int (2);
  FnLFacesDet (*pPolymod, BadVer, FnLFaces);

  /* NewVer adds the vertex which is the intersection between faces
   * first, second and third, i.e. the first and last face to modify and
   * the new face: VerQty is increased, VerUse, VerFace and VerCoo are
   * initiated.. The number of the created vertex is returned.
   */
  firstVer = NewVer (pPolymod, FnLFaces[0], FnLFaces[1], (*pPolymod).FaceQty);

  /* NewFaceAddVer adds the new vertex to the new face.
   */
  NewFaceAddVer (pPolymod);

  nFace = FnLFaces[0];
  pFace = FnLFaces[1];

  while (nFace != FnLFaces[1])
  {
    Face = nFace;

    /* FaceModif modifies Face to take into account the new
     * face.
     */
    nFace = FaceModif (pFace, Face, pPolymod, BadVer, 0);
    /* The new face is updated by adding the last created vertex.
     */
    NewFaceAddVer (pPolymod);
    pFace = Face;
  }

  FaceModif (pFace, FnLFaces[1], pPolymod, BadVer, firstVer);

  ut_free_1d_int (FnLFaces);

  return 0;
}
