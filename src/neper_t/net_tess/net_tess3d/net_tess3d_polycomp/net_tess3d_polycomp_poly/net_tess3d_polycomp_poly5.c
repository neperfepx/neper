/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_poly_.h"

/* net_tess3d_polycomp_poly_cut_mod_faceuse_check returns 1 if the face is still useful
 * (FaceUse=1); 0 else.
 */
int
net_tess3d_polycomp_poly_cut_mod_faceuse_check (struct POLY Poly, int *VerUse, int face)
{
  /* mute variable */
  int sum;

  /* for each vertex of the face, one checks if is still
   * belong to the polyhedron or not. If yes, the value 1 is
   * returned. Indeed, whenever a vertex is useful, the tested
   * face is useful. If all vertices are useless, the face is
   * useless: the program returns 0.
   */
  for (sum = 1; sum <= Poly.FaceVerQty[face]; sum++)
    /* whenever a vertex of the face is still used (i.e. does not
     * belong to BadVer), the face must be kept: the value 1 is returned.
     */
    if (VerUse[Poly.FaceVerNb[face][sum]])
      return 1;

  /* Else, 0 is returned. */
  return 0;
}

void
net_tess3d_polycomp_poly_cut_mod_newface_qty (struct POLY *pPoly)
{
  (*pPoly).FaceQty++;

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newface_eq (struct POLY *pPoly, double *plane)
{
  int i;

  (*pPoly).FaceEq =
    ut_realloc_2d_addline ((*pPoly).FaceEq, (*pPoly).FaceQty + 1, 4);

  for (i = 0; i <= 3; i++)
    (*pPoly).FaceEq[(*pPoly).FaceQty][i] = plane ? plane[i] : -1;

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newface_poly (struct POLY *pPoly, int Nei)
{
  (*pPoly).FacePoly =
    ut_realloc_1d_int ((*pPoly).FacePoly, (*pPoly).FaceQty + 1);

  (*pPoly).FacePoly[(*pPoly).FaceQty] = Nei;

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns (struct POLY Poly, int ns, int nf, int *Ver,
                                                int *BadVer)
{
  int face;

  Ver[1] = BadVer[ns];
  face = Poly.VerFace[Ver[1]][nf];
  Ver[0] = net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns_neiver (Poly, face, Ver[1], -1);
  Ver[2] = net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns_neiver (Poly, face, Ver[1], +1);

  return;
}

int
net_tess3d_polycomp_poly_cut_mod_fnlfaces_commonfaces (struct POLY Poly, int first, int second,
                                                       int *res)
{
  int i, j;
  int nb = 0;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      if (Poly.VerFace[first][i] == Poly.VerFace[second][j])
      {
        res[nb] = Poly.VerFace[first][i];
        nb++;
        break;
      }

  return nb;
}

/* net_tess3d_polycomp_poly_cut_mod_face_vercouple rotates FaceVerNb to place
 * the bad vertices at the end, searches couple1, couple2, faces1, faces2 and
 * supplies bel2.
 */
int
net_tess3d_polycomp_poly_cut_mod_face_vercouple (struct POLY *pPoly, int *VerUse, int *FaceUse,
                                                 int Face, int pFace, int *couple1, int *couple2,
                                                 int *faces1, int *faces2, int *pNbBadVer)
{
  /* FaceVerNb[Face] is rotated to put all the bad vertices at the end.
   * The amount of bad vertices is recorded.
   */
  (*pNbBadVer) = net_tess3d_polycomp_poly_cut_mod_face_vercouple_rot (pPoly, VerUse, FaceUse, Face);

  /* The 2 couples of vertices of which segment is cut by the new plane
   * are recorded as couple1 & couple2.
   * The faces associated to these couples of vertices are recorded.
   * faces1[0] and faces1[1] are the two common faces of vertices
   * couple1[0] & couple1[1]. The same for faces2.
   */
  couple1[0] = (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face]];
  couple1[1] = (*pPoly).FaceVerNb[Face][1];
  couple2[0] =
    (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face] - (*pNbBadVer)];
  couple2[1] =
    (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face] - (*pNbBadVer) +
                                1];

  /* Recording of the common faces of couple1 vertices and couple2
   * vertices.
   */
  net_tess3d_polycomp_poly_cut_mod_fnlfaces_commonfaces (*pPoly, couple1[0], couple1[1], faces1);
  net_tess3d_polycomp_poly_cut_mod_fnlfaces_commonfaces (*pPoly, couple2[0], couple2[1], faces2);

  return ut_array_1d_int_eltpos (faces2, 2, pFace);
}

/* Modification of Face propoerties to suppress the bad vertices.
 */
void
net_tess3d_polycomp_poly_cut_mod_face_genpara (struct POLY *pPoly, int Face, int NbBadVer)
{
  int i;

  /* Suppression of the bad vertices */
  for (i = 1; i <= NbBadVer; i++)
    (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face] + 1 - i] = 0;
  (*pPoly).FaceVerQty[Face] -= NbBadVer;

  return;
}

/* Adding of the first new vertex.
 */
void
net_tess3d_polycomp_poly_cut_mod_face_newver1 (struct POLY *pPoly, int Face)
{
  /* REGISTERING OF THE FIRST NEW SUMMIT, i.e. that created at the previous
   * step.
   */
  (*pPoly).FaceVerQty[Face]++;
  (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face]] =
    (*pPoly).VerQty;

  return;
}

/* Adding of the second new vertex.
 */
void
net_tess3d_polycomp_poly_cut_mod_face_newver2 (struct POLY *pPoly, int Face, int bel2, int next,
                                               double *coo)
{
  /* The FaceVerQty is incremented.
   */
  (*pPoly).FaceVerQty[Face]++;

  /* Creation of the new vertex as the intersection between faces
   * Face, next & (*pPoly).FaceQty.
   */
  net_tess3d_polycomp_poly_cut_mod_newver (pPoly, Face, next, (*pPoly).FaceQty, coo);

  /* Adding of this vertex
   */
  (*pPoly).FaceVerNb[Face] =
    ut_realloc_1d_int ((*pPoly).FaceVerNb[Face],
                       (*pPoly).FaceVerQty[Face] + 1);

  (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face]] =
    (*pPoly).VerQty;

  if (bel2 == -1)
    ut_array_1d_int_switch ((*pPoly).FaceVerNb[Face],
                            (*pPoly).FaceVerQty[Face] - 1,
                            (*pPoly).FaceVerQty[Face]);

  return;
}

/* Else, second vertex of the face is set to the first created vertex.
 */
void
net_tess3d_polycomp_poly_cut_mod_face_addbegver (struct POLY *pPoly, int Face, int last, int bel2)
{
  (*pPoly).FaceVerQty[Face]++;

  (*pPoly).FaceVerNb[Face] =
    ut_realloc_1d_int ((*pPoly).FaceVerNb[Face],
                       (*pPoly).FaceVerQty[Face] + 1);

  (*pPoly).FaceVerNb[Face][(*pPoly).FaceVerQty[Face]] = last;
  if (bel2 == -1)
    ut_array_1d_int_switch ((*pPoly).FaceVerNb[Face],
                            (*pPoly).FaceVerQty[Face] - 1,
                            (*pPoly).FaceVerQty[Face]);

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newver_qty (struct POLY *pPoly)
{
  (*pPoly).VerQty++;

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newver_face (struct POLY *pPoly, int first, int second, int third)
{
  (*pPoly).VerFace =
    ut_realloc_2d_int_addline ((*pPoly).VerFace, (*pPoly).VerQty + 1,
                               3);

  (*pPoly).VerFace[(*pPoly).VerQty][0] = first;
  (*pPoly).VerFace[(*pPoly).VerQty][1] = second;
  (*pPoly).VerFace[(*pPoly).VerQty][2] = third;

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newver_coo (struct POLY *pPoly, double *coo)
{
  (*pPoly).VerCoo = ut_realloc_2d_addline ((*pPoly).VerCoo, (*pPoly).VerQty + 1, 3);

  ut_array_1d_memcpy (coo, 3, (*pPoly).VerCoo[(*pPoly).VerQty]);

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newfaceaddver_qty (struct POLY *pPoly)
{
  (*pPoly).FaceVerQty[(*pPoly).FaceQty]++;

  return;
}

void
net_tess3d_polycomp_poly_cut_mod_newfaceaddver_nb (struct POLY *pPoly)
{
  (*pPoly).FaceVerNb[(*pPoly).FaceQty] =
    ut_realloc_1d_int ((*pPoly).FaceVerNb[(*pPoly).FaceQty],
                       (*pPoly).FaceVerQty[(*pPoly).FaceQty] + 1);

  (*pPoly).FaceVerNb[(*pPoly).FaceQty][(*pPoly).
                                             FaceVerQty[(*pPoly).
                                                        FaceQty]] =
    (*pPoly).VerQty;

  return;
}

/* Intersection of 3 face planes */
void
net_tess3d_polycomp_poly_cut_mod_inter_planes (struct POLY Poly, int p1, int p2, int p3,
                                               double *inter)
{
  int i;
  double a[3][3], B[3];
  double *A[3] = { a[0], a[1], a[2] };
  double det;

  for (i = 0; i < 3; i++)
  {
    A[0][i] = Poly.FaceEq[p1][i + 1];
    A[1][i] = Poly.FaceEq[p2][i + 1];
    A[2][i] = Poly.FaceEq[p3][i + 1];
  }
  B[0] = Poly.FaceEq[p1][0];
  B[1] = Poly.FaceEq[p2][0];
  B[2] = Poly.FaceEq[p3][0];

  det = ut_mat_det_33 (A);

  if (ut_num_equal (det, 0, 1e-15))     // the 2 first faces must be coplanar
  {
    if (!ut_space_planes_equal (Poly.FaceEq[p1], Poly.FaceEq[p2]))
      ut_print_neperbug ();

    int verqty, *vers = ut_alloc_1d_int (2);

    ut_array_1d_int_inter (Poly.FaceVerNb[p1] + 1, Poly.FaceVerQty[p1],
                           Poly.FaceVerNb[p2] + 1, Poly.FaceVerQty[p2],
                           vers, &verqty);

    // seems that verqty != 2 should not happen...
    if (verqty == 2)
    {
      double *v = ut_alloc_1d (3);
      ut_array_1d_sub (Poly.VerCoo[vers[0]], Poly.VerCoo[vers[1]], 3,
                       v);
      ut_array_1d_normalize (v, 3);

      ut_vector_vectprod (A[0], v, A[1]);

      B[1] = ut_array_1d_scalprod (A[1], Poly.VerCoo[vers[0]], 3);

      if (ut_num_equal (ut_mat_det_33 (A), 0, 1e-12))   // Fix did not work
        ut_print_neperbug ();

      ut_free_1d (&v);
    }

    ut_free_1d_int (&vers);
  }

  ut_linalg_solve_LU (A, B, 3, inter);

  return;
}

/* Intersection between the initial edge common to faces p1 and p2 and the
 * primitive of face p3 */
void
net_tess3d_polycomp_poly_cut_mod_inter_prim (struct POLY Poly, struct PRIM *pPrim,
                                             int **FaceVerNbCpy, int *FaceVerQtyCpy, int p1, int p2,
                                             int p3, double *inter)
{
  int verqty, *vers = ut_alloc_1d_int (2);

  // The last provided face must be the new one (of primitive pPrim)
  if (p3 < p1 || p3 < p2)
    abort ();

  if (FaceVerNbCpy)
    ut_array_1d_int_inter (FaceVerNbCpy[p1] + 1, FaceVerQtyCpy[p1],
                           FaceVerNbCpy[p2] + 1, FaceVerQtyCpy[p2],
                           vers, &verqty);
  else
    ut_array_1d_int_inter (Poly.FaceVerNb[p1] + 1, Poly.FaceVerQty[p1],
                           Poly.FaceVerNb[p2] + 1, Poly.FaceVerQty[p2],
                           vers, &verqty);

  if (verqty != 2)
    abort ();

  if (!strncmp ((*pPrim).Type, "hspace", 6))
    ut_space_segment_plane_intersect (Poly.VerCoo[vers[0]],
                                      Poly.VerCoo[vers[1]],
                                      Poly.FaceEq[p3], inter);

  else if (!strncmp ((*pPrim).Type, "sphere", 6))
    ut_space_segment_sphere_intersect (Poly.VerCoo[vers[0]],
                                       Poly.VerCoo[vers[1]],
                                       (*pPrim).Base, (*pPrim).Rad[0], inter);

  else if (!strncmp ((*pPrim).Type, "cylinder", 8))
    ut_space_segment_cylinder_intersect (Poly.VerCoo[vers[0]],
                                         Poly.VerCoo[vers[1]],
                                         (*pPrim).Base, (*pPrim).Dir, (*pPrim).Rad[0], inter);

  else if (!strncmp ((*pPrim).Type, "torus", 5))
    ut_space_segment_torus_intersect (Poly.VerCoo[vers[0]],
                                      Poly.VerCoo[vers[1]],
                                      (*pPrim).Base, (*pPrim).Dir, (*pPrim).Rad[0], (*pPrim).Rad[1], inter);

  else if (!strncmp ((*pPrim).Type, "ecylinder", 9))
    ut_space_segment_ecylinder_intersect (Poly.VerCoo[vers[0]],
                                          Poly.VerCoo[vers[1]],
                                          (*pPrim).Parms,
                                          (*pPrim).Parms + 3,
                                          (*pPrim).Parms + 6,
                                          (*pPrim).Parms + 9,
                                          (*pPrim).Parms[12],
                                          (*pPrim).Parms[13],
                                          inter);

  else if (!strncmp ((*pPrim).Type, "cube", 4))
    ut_space_segment_cube_intersect (Poly.VerCoo[vers[0]],
                                     Poly.VerCoo[vers[1]],
                                     (*pPrim).Parms[0],
                                     (*pPrim).Parms[1],
                                     (*pPrim).Parms[2],
                                     (*pPrim).Parms[3],
                                     (*pPrim).Parms[4],
                                     (*pPrim).Parms[5],
                                     (*pPrim).Parms[6],
                                     inter);

  else
  {
    printf ("Primitive not supported\n");
    abort ();
  }

  ut_free_1d_int (&vers);

  return;
}
