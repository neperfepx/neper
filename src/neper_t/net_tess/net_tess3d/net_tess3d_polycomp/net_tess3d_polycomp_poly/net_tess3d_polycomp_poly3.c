/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_poly_.h"

/* net_tess3d_polycomp_poly_cut_badver searches the vertices that are on the bad side of the
 * cutting primitive (negative side, see neut_prim_point_side) or, if pPrim
 * is NULL, of the cutting plane (normal pointing to the bad side, i.e.
 * that of the neighbor).
 */
void
net_tess3d_polycomp_poly_cut_badver (double *plane, struct PRIM *pPrim, struct POLY Poly,
                                     int **pBadVer)
{
  int i, VerSide;

  (*pBadVer) = ut_alloc_1d_int (Poly.VerQty + 1);

  /* For every real vertex of the polyhedron, we check if it is
   * in the bad side or not.
   */
  for (i = 1; i <= Poly.VerQty; i++)
    {
      /* the term -1 is added to take into account the fact that Side
       * read coordinates in [1]...[3] while they are stored in
       * [0]...[2] in VerCoo[i]
       */
      if (!pPrim)
        VerSide = -ut_space_point_plane_side (Poly.VerCoo[i] - 1, plane);
      else
        neut_prim_point_side (*pPrim, Poly.VerCoo[i], &VerSide);

      /* if the vertex is in the bad side, its existence is taken
       * into account: BadVer[0]++; and its number (i) is stored.
       */
      if (VerSide == -1)
        (*pBadVer)[++(*pBadVer)[0]] = i;
    }

  return;
}

/* net_tess3d_polycomp_poly_cut_mod modifies the polyhedron to take into account
 * the new neighbor.  If pPrim is NULL, the new face is plane and the new
 * vertices are computed as the intersections of 3 face planes.  Otherwise,
 * they are computed as the intersections between the initial edges and pPrim
 * plane is the equation of the new face, or NULL if pPrim is curved.  The
 * vertices and faces of the polyhedron that are removed are first recorded as
 * unused (VerUse and FaceUse), and then removed.  If all vertices are removed,
 * the polyhedron is emptied and -1 is returned.
 */
int
net_tess3d_polycomp_poly_cut_mod (int Nei, double *plane, struct PRIM *pPrim, struct POLY *pPoly,
                                  int *BadVer)
{
  int i, status;
  int FnLFaces[2] = { 0, 0 };   /* first & last face to be modified         */
  int Face;                     /* current face to be modified              */
  int pFace;                    /* previous face that has been modified     */
  int nFace;                    /* next face to modifiy                     */
  int firstVer;                 /* first created vertex                     */
  int faceqty = (*pPoly).FaceQty;
  int *FaceVerQtyCpy = NULL;
  int **FaceVerNbCpy = NULL;
  /* VerUse[i] and FaceUse[i] are 1 if vertex / face i is used, 0 else.  A
   * cut adds one face and at most one vertex per initial face. */
  int *VerUse = ut_alloc_1d_int ((*pPoly).VerQty + faceqty + 1);
  int *FaceUse = ut_alloc_1d_int (faceqty + 2);

  ut_array_1d_int_set (VerUse, (*pPoly).VerQty + faceqty + 1, 1);
  ut_array_1d_int_set (FaceUse, faceqty + 2, 1);

  /* If the new vertices are computed as the intersections between the
   * initial edges and the new face, the initial face vertices are recorded.
   */
  if (pPrim)
  {
    FaceVerQtyCpy = ut_alloc_1d_int (faceqty + 1);
    FaceVerNbCpy = ut_alloc_1d_pint (faceqty + 1);

    for (i = 1; i <= faceqty; i++)
    {
      FaceVerQtyCpy[i] = (*pPoly).FaceVerQty[i];
      FaceVerNbCpy[i] = ut_alloc_1d_int ((*pPoly).FaceVerQty[i] + 1);
      ut_array_1d_int_memcpy ((*pPoly).FaceVerNb[i] + 1,
                              (*pPoly).FaceVerQty[i], FaceVerNbCpy[i] + 1);
    }
  }

  /* net_tess3d_polycomp_poly_cut_mod_veruse modifies VerUse: VerUse[i]<--0 if
   * the ith vertex is a bad vertex.
   */
  status = net_tess3d_polycomp_poly_cut_mod_veruse (VerUse, (*pPoly).VerQty, BadVer);
  if (status != 0)
  {
    int pseudodim = (*pPoly).PseudoDim;
    double pseudosize = (*pPoly).PseudoSize;

    neut_poly_free (pPoly);
    (*pPoly).PseudoDim = pseudodim;
    (*pPoly).PseudoSize = pseudosize;

    ut_free_2d_int (&FaceVerNbCpy, faceqty + 1);
    ut_free_1d_int (&FaceVerQtyCpy);
    ut_free_1d_int (&VerUse);
    ut_free_1d_int (&FaceUse);
    return -1;
  }

  /* net_tess3d_polycomp_poly_cut_mod_faceuse modifies FaceUse: FaceUse[i]<--0
   * if all the vertices of face i are bad vertices.
   */
  net_tess3d_polycomp_poly_cut_mod_faceuse (*pPoly, VerUse, FaceUse);

  /* net_tess3d_polycomp_poly_cut_mod_newface adds the new face by increasing
   * FaceQty and initializing FacePoly & FaceEq.
   */
  net_tess3d_polycomp_poly_cut_mod_newface (Nei, pPoly, plane);

  /* net_tess3d_polycomp_poly_cut_mod_fnlfaces determines the first and last faces to be modified.
   * These are two neighbor faces cut by the new plane, and are
   * recorded as FnLFaces[0] and FnLFaces[1].
   */
  net_tess3d_polycomp_poly_cut_mod_fnlfaces (*pPoly, VerUse, BadVer, FnLFaces);

  /* net_tess3d_polycomp_poly_cut_mod_newver adds the vertex which is the intersection between faces
   * first, second and third, i.e. the first and last face to modify and
   * the new face: VerQty is increased, VerFace and VerCoo are
   * initiated.. The number of the created vertex is returned.
   */
  double coo[3];
  net_tess3d_polycomp_poly_cut_mod_inter (*pPoly, pPrim, FaceVerNbCpy, FaceVerQtyCpy, FnLFaces[0],
                                          FnLFaces[1], (*pPoly).FaceQty, coo);
  firstVer =
    net_tess3d_polycomp_poly_cut_mod_newver (pPoly, FnLFaces[0], FnLFaces[1], (*pPoly).FaceQty,
                                             coo);

  /* net_tess3d_polycomp_poly_cut_mod_newfaceaddver adds the new vertex to the new face.
   */
  net_tess3d_polycomp_poly_cut_mod_newfaceaddver (pPoly);

  nFace = FnLFaces[0];
  pFace = FnLFaces[1];

  while (nFace != FnLFaces[1])
  {
    Face = nFace;

    /* net_tess3d_polycomp_poly_cut_mod_face modifies Face to take into account the new
     * face.
     */
    nFace =
      net_tess3d_polycomp_poly_cut_mod_face (pFace, Face, pPoly, FaceVerNbCpy, FaceVerQtyCpy, pPrim,
                                             VerUse, FaceUse, 0);
    /* The new face is updated by adding the last created vertex.
     */
    net_tess3d_polycomp_poly_cut_mod_newfaceaddver (pPoly);
    pFace = Face;
  }

  net_tess3d_polycomp_poly_cut_mod_face (pFace, FnLFaces[1], pPoly, FaceVerNbCpy, FaceVerQtyCpy,
                                         pPrim, VerUse, FaceUse, firstVer);

  if (!plane)
    net_tess3d_polycomp_poly_cut_mod_curvedface (pPoly);

  /* The unused vertices and faces are removed. */
  net_tess3d_polycomp_poly_cut_mod_compact (pPoly, VerUse, FaceUse);

  ut_free_2d_int (&FaceVerNbCpy, faceqty + 1);
  ut_free_1d_int (&FaceVerQtyCpy);
  ut_free_1d_int (&VerUse);
  ut_free_1d_int (&FaceUse);

  return 0;
}
