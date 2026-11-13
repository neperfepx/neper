/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_poly_.h"

/* net_tess3d_polycomp_poly_cut_mod_veruse modifies VerUse: VerUse[i]<--0 if
 * the ith vertex is in the bad side.  -1 is returned if no vertex is used
 * anymore, 0 else.
 */
int
net_tess3d_polycomp_poly_cut_mod_veruse (int *VerUse, int VerQty, int *BadVer)
{
  int i;

  /* VerUse[] takes the value 0 for the bad vertices. */
  for (i = 1; i <= BadVer[0]; i++)
    VerUse[BadVer[i]] = 0;

  return ut_array_1d_int_sum (VerUse + 1, VerQty) > 0 ? 0 : -1;
}

/* net_tess3d_polycomp_poly_cut_mod_faceuse modifies FaceUse: FaceUse[i]<--0 if
 * the ith face has to be suppressed.
 */
void
net_tess3d_polycomp_poly_cut_mod_faceuse (struct POLY Poly, int *VerUse, int *FaceUse)
{
  int face;

  /* Every face is tested successively. */
  for (face = 1; face <= Poly.FaceQty; face++)
    /* if the face is used, it is tested.   */
    if (FaceUse[face] == 1)
      /* tests if vertices belonging to the face is still useful. */
      /* net_tess3d_polycomp_poly_cut_mod_faceuse_check returns 1 if */
      /* the face is still useful (FaceUse=1); 0 else.             */
      FaceUse[face] = net_tess3d_polycomp_poly_cut_mod_faceuse_check (Poly, VerUse, face);

  return;
}

/* net_tess3d_polycomp_poly_cut_mod_newface adds the new face by increasing FaceQty and through the
initializations of FaceEq & FacePoly. */
void
net_tess3d_polycomp_poly_cut_mod_newface (int Nei, struct POLY *pPoly, double *plane)
{

  /* The quantity of faces (useful or not) is incremented. */
  net_tess3d_polycomp_poly_cut_mod_newface_qty (pPoly);

  /* The new face is recorded. */
  net_tess3d_polycomp_poly_cut_mod_newface_eq (pPoly, plane);

  /* The seed of which the new face is associated is recorded. */
  net_tess3d_polycomp_poly_cut_mod_newface_poly (pPoly, Nei);

  (*pPoly).FaceVerQty =
    ut_realloc_1d_int ((*pPoly).FaceVerQty, (*pPoly).FaceQty + 1);
  (*pPoly).FaceVerQty[(*pPoly).FaceQty] = 0;

  (*pPoly).FaceVerNb =
    ut_realloc_1d_pint ((*pPoly).FaceVerNb, (*pPoly).FaceQty + 1);
  (*pPoly).FaceVerNb[(*pPoly).FaceQty] = ut_alloc_1d_int (1);

  return;
}

/* net_tess3d_polycomp_poly_cut_mod_fnlfaces determines the first and last faces to be modified.
 * These are two neighbor faces cut by the new plane, and are
 * recorded as FnLFaces[0] and FnLFaces[1].
 * To find a face cut by the new plane, the bad vertices are considered
 * successively and, for each of them, the three parent faces are treated.
 * One searches a couple (bad vertex, face) such that the new plane cut the
 * face boundary through a segment composed of the bad vertex (and an other
 * -good- vertex).
 */
void
net_tess3d_polycomp_poly_cut_mod_fnlfaces (struct POLY Poly, int *VerUse, int *BadVer,
                                           int *FnLFaces)
{
  int i;                        /* mute variable                                        */
  int ns, nf;                   /* number of the vertex in BadVer and number of the     */
  /* considered face.                                     */
  int nb;                       /* in fine, to delete                                   */
  int Ver[3];                   /* Ver[1] <-- number of the considered vertex           */
  /* Ver[0] <-- number of the "previous" neighbor        */
  /* Ver[2] <-- number of the "next" neighbor            */

  /* for each bad vertex of the polyhedron... temporarly named Ver[1],
   * successively and while FnLFaces[0]==0
   * the 3 faces whose vertex is intersection are considered
   * successively: VerFace[Ver[1]][nf], nf=0 to 2
   * The face number is recorded as face.
   */
  for (ns = 1; FnLFaces[0] == 0; ns++)
    for (nf = 0; nf <= 2; nf++)
    {
      /* net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns returns the number of
       * the considered vertex Ver[1], and those of its two neighbors: Ver[0]
       * & Ver[2].
       */
      net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns (Poly, ns, nf, Ver, BadVer);

      /* FaceVerNb[face][pPos], i.e. one of the two neighbor vertices
       * of FaceVerNb[face][Pos], is searched out in vertices.
       * If it is not used anymore (i.e. it belongs to BadVer), these two
       * vertices are in the bad side of the new plane. Else (what we want),
       * vertex Pos is in the bad side and vertex pPos is in the good side.
       */

      for (i = 0; i <= 2; i = i + 2)
        if (VerUse[Ver[i]])
        {
          nb = net_tess3d_polycomp_poly_cut_mod_fnlfaces_commonfaces (Poly, Ver[i], Ver[1],
                                                                      FnLFaces);
          /* simple verification... */
          if (nb != 2)
            ut_print_neperbug ();

          return;
        }
    }

  return;
}

/* FaceModification modifies Face to take into account the new
 * face.
 */
int
net_tess3d_polycomp_poly_cut_mod_face (int pFace, int Face, struct POLY *pPoly, int **FaceVerNbCpy,
                                       int *FaceVerQtyCpy, struct PRIM *pPrim, int *VerUse,
                                       int *FaceUse, int last)
{
  int next = 0;                 /* next face to modify */
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
  int couple1[2] = { 0, 0 };
  int couple2[2] = { 0, 0 };
  /* The faces associated to these couples of vertices are recorded.
   * faces1[0] and faces1[1] are the two common faces of vertices
   * couple1[0] & couple1[1]. The same for faces2. That allows to
   * know whose are the parent faces of a new vertex.
   */
  int faces1[2] = { 0, 0 };
  int faces2[2] = { 0, 0 };

  int bel2;                     /* (see above) */

  int NbBadVer;                 /* Number of bad vertices of the face */
  int *pNbBadVer = &NbBadVer;

  double coo[3];

  /* net_tess3d_polycomp_poly_cut_mod_face_vercouple rotates FaceVerNb to
   * place the bad vertices at the end, searches couple1, couple2, faces1,
   * faces2 and supplies bel2.
   */
  bel2 =
    net_tess3d_polycomp_poly_cut_mod_face_vercouple (pPoly, VerUse, FaceUse, Face, pFace, couple1,
                                                     couple2, faces1, faces2, pNbBadVer);

  /* Recording of the next face to modify.
   */
  if (bel2 != -1)
    if (faces1[0] == Face)
      next = faces1[1];
    else
      next = faces1[0];
  else if (faces2[0] == Face)
    next = faces2[1];
  else
    next = faces2[0];

  net_tess3d_polycomp_poly_cut_mod_inter (*pPoly, pPrim, FaceVerNbCpy, FaceVerQtyCpy, Face, next,
                                          (*pPoly).FaceQty, coo);

  /* Modification of Face properties to suppress the bad vertices.
   */
  net_tess3d_polycomp_poly_cut_mod_face_genpara (pPoly, Face, NbBadVer);

  /* Adding of the first new vertex.
   */
  net_tess3d_polycomp_poly_cut_mod_face_newver1 (pPoly, Face);

  /* If this face is not the last face to modify :
   * Adding of the second new vertex.
   * Else, second vertex of the face is set to the first created vertex.
   */
  if (last == 0)
    net_tess3d_polycomp_poly_cut_mod_face_newver2 (pPoly, Face, bel2, next, coo);
  else
    net_tess3d_polycomp_poly_cut_mod_face_addbegver (pPoly, Face, last, bel2);

  return next;
}

/* net_tess3d_polycomp_poly_cut_mod_newver adds the vertex which is the
 * intersection between faces first second and third, i.e. the first and last
 * face to modified and the new face.
 */
int
net_tess3d_polycomp_poly_cut_mod_newver (struct POLY *pPoly, int first, int second, int third,
                                         double *coo)
{
  net_tess3d_polycomp_poly_cut_mod_newver_qty (pPoly);
  net_tess3d_polycomp_poly_cut_mod_newver_face (pPoly, first, second, third);
  net_tess3d_polycomp_poly_cut_mod_newver_coo (pPoly, coo);

  return (*pPoly).VerQty;
}

/* net_tess3d_polycomp_poly_cut_mod_newfaceaddver adds the new vertex to the new face.
 */
void
net_tess3d_polycomp_poly_cut_mod_newfaceaddver (struct POLY *pPoly)
{
  net_tess3d_polycomp_poly_cut_mod_newfaceaddver_qty (pPoly);
  net_tess3d_polycomp_poly_cut_mod_newfaceaddver_nb (pPoly);

  return;
}

/* net_tess3d_polycomp_poly_cut_mod_curvedface sets the equation of the new
 * (curved) face as the best-fitting plane and fixes the order of its
 * vertices.
 */
void
net_tess3d_polycomp_poly_cut_mod_curvedface (struct POLY *pPoly)
{
  int i, newface = (*pPoly).FaceQty;
  int *vers = ut_alloc_1d_int (2);

  ut_array_1d_int_memcpy ((*pPoly).FaceVerNb[newface] + 1, 2, vers);

  // detecting pair of faces for the considered edge (first edge)
  int commonfaceqty, *commonfaces = ut_alloc_1d_int (3);
  ut_array_1d_int_inter ((*pPoly).VerFace[vers[0]], 3, (*pPoly).VerFace[vers[1]], 3, commonfaces, &commonfaceqty);
  if (commonfaceqty != 2)
    abort ();

  int face, shift, ver1pos, *dir = ut_alloc_1d_int (2);
  for (i = 0; i < 2; i++)
  {
    face = commonfaces[i];
    ver1pos = ut_array_1d_int_eltpos ((*pPoly).FaceVerNb[face] + 1, (*pPoly).FaceVerQty[face], vers[0]);
    ver1pos++;
    for (shift = -1; shift <= 1; shift += 2)
      if ((*pPoly).FaceVerNb[face][ut_array_rotpos (1, (*pPoly).FaceVerQty[face], ver1pos, shift)] == vers[1])
      {
        dir[i] = shift;
        break;
      }
    if (dir[i] == 0)
      abort ();
  }

  // if same rotation, we need to reverse the edges of the new face
  if (dir[0] == dir[1])
    ut_array_1d_int_reverseelts ((*pPoly).FaceVerNb[newface] + 1, (*pPoly).FaceVerQty[newface]);

  // best-fitting plane, oriented as the vertices (outwards), so that
  // neut_poly_fixverorder keeps their order
  int ptqty = (*pPoly).FaceVerQty[newface];
  double **pts = ut_alloc_2d (ptqty, 3);
  double *bary = ut_alloc_1d (3);
  double *n = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);
  for (i = 1; i <= ptqty; i++)
  {
    ut_array_1d_memcpy ((*pPoly).VerCoo[(*pPoly).FaceVerNb[newface][i]], 3, pts[i - 1]);
    ut_array_1d_add (bary, pts[i - 1], 3, bary);
  }
  ut_array_1d_scale (bary, 3, 1. / ptqty);
  ut_space_pointset_plane (pts, ptqty, (*pPoly).FaceEq[newface]);

  for (i = 0; i < ptqty; i++)
  {
    ut_space_triangle_normal (bary, pts[i], pts[(i + 1) % ptqty], n0);
    ut_array_1d_add (n, n0, 3, n);
  }
  if (ut_vector_scalprod (n, (*pPoly).FaceEq[newface] + 1) < 0)
    ut_array_1d_scale ((*pPoly).FaceEq[newface], 4, -1);

  ut_free_2d (&pts, ptqty);
  ut_free_1d (&bary);
  ut_free_1d (&n);
  ut_free_1d (&n0);

  ut_free_1d_int (&dir);
  ut_free_1d_int (&commonfaces);
  ut_free_1d_int (&vers);

  return;
}

/* net_tess3d_polycomp_poly_cut_mod_inter computes the intersection between faces p1, p2
 * and p3 of the polyhedron, p3 being the new face, of primitive pPrim (NULL
 * for a plane).  If pPrim is NULL, it is computed as the intersection of the
 * 3 face planes, and otherwise as the intersection between the initial edge
 * common to p1 and p2 (from FaceVerNbCpy and FaceVerQtyCpy) and the
 * primitive.
 */
void
net_tess3d_polycomp_poly_cut_mod_inter (struct POLY Poly, struct PRIM *pPrim, int **FaceVerNbCpy,
                                        int *FaceVerQtyCpy, int p1, int p2, int p3, double *inter)
{
  if (!pPrim)
    net_tess3d_polycomp_poly_cut_mod_inter_planes (Poly, p1, p2, p3, inter);
  else
    net_tess3d_polycomp_poly_cut_mod_inter_prim (Poly, pPrim, FaceVerNbCpy, FaceVerQtyCpy, p1, p2,
                                                 p3, inter);

  return;
}

/* net_tess3d_polycomp_poly_cut_mod_compact removes the unused vertices and faces (see
 * VerUse and FaceUse), keeping the order of the others.
 */
void
net_tess3d_polycomp_poly_cut_mod_compact (struct POLY *pPoly, int *VerUse, int *FaceUse)
{
  int i, j, qty;
  int *VerNew = ut_alloc_1d_int ((*pPoly).VerQty + 1);
  int *FaceNew = ut_alloc_1d_int ((*pPoly).FaceQty + 1);

  qty = 0;
  for (i = 1; i <= (*pPoly).VerQty; i++)
    if (VerUse[i])
    {
      VerNew[i] = ++qty;
      (*pPoly).VerFace[qty] = (*pPoly).VerFace[i];
      (*pPoly).VerCoo[qty] = (*pPoly).VerCoo[i];
    }
    else
    {
      ut_free_1d_int (&(*pPoly).VerFace[i]);
      ut_free_1d (&(*pPoly).VerCoo[i]);
    }
  (*pPoly).VerQty = qty;

  qty = 0;
  for (i = 1; i <= (*pPoly).FaceQty; i++)
    if (FaceUse[i])
    {
      FaceNew[i] = ++qty;
      (*pPoly).FacePoly[qty] = (*pPoly).FacePoly[i];
      (*pPoly).FaceEq[qty] = (*pPoly).FaceEq[i];
      (*pPoly).FaceVerQty[qty] = (*pPoly).FaceVerQty[i];
      (*pPoly).FaceVerNb[qty] = (*pPoly).FaceVerNb[i];
    }
    else
    {
      ut_free_1d (&(*pPoly).FaceEq[i]);
      ut_free_1d_int (&(*pPoly).FaceVerNb[i]);
    }
  (*pPoly).FaceQty = qty;

  for (i = 1; i <= (*pPoly).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pPoly).VerFace[i][j] = FaceNew[(*pPoly).VerFace[i][j]];

  for (i = 1; i <= (*pPoly).FaceQty; i++)
    for (j = 1; j <= (*pPoly).FaceVerQty[i]; j++)
      (*pPoly).FaceVerNb[i][j] = VerNew[(*pPoly).FaceVerNb[i][j]];

  ut_free_1d_int (&VerNew);
  ut_free_1d_int (&FaceNew);

  return;
}
