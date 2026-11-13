/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_poly_.h"

int
net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns_neiver (struct POLY Poly, int face, int sum,
                                                       int near)
{
  int NeiPos;

  NeiPos =
    1 + ut_array_1d_int_eltpos (Poly.FaceVerNb[face] + 1,
                                Poly.FaceVerQty[face], sum);
  NeiPos = ut_array_rotpos (1, Poly.FaceVerQty[face], NeiPos, near);

  return Poly.FaceVerNb[face][NeiPos];
}

/* FaceVerNb[Face] is rotated to put all the bad vertices at the end.
 * The bad vertices are the ones not used anymore, see
 * net_tess3d_polycomp_poly_cut_mod_veruse.  The program returns the number
 * of bad vertices.
 */
int
net_tess3d_polycomp_poly_cut_mod_face_vercouple_rot (struct POLY *pPoly, int *VerUse, int *FaceUse,
                                                     int Face)
{
  int i, num = 0, shift = 0;
  int qty = (*pPoly).FaceVerQty[Face];
  int *vers = (*pPoly).FaceVerNb[Face];
  int *tmp = NULL;

  if (FaceUse[Face] == 0)
  {
    printf ("\n");
    ut_print_message (2, 3,
                      "Tessellation failed because of a degenerated configuration.  Randomize to fix.\n");
  }

  // Determining the rotation, as a number of one-step rotations to the right
  // (the last vertex becoming the first), such that the first vertex is
  // a good vertex and the last vertex is a bad vertex.  After rotating
  // by shift, vertex i is the initial vertex 1 + (i - 1 - shift) mod qty.
  // The first loop is a preliminary step that makes the first vertex bad
  // and the last vertex good.
#define VER(i) vers[1 + ((((i) - 1 - shift) % qty) + qty) % qty]
  while (VerUse[VER (1)] || !VerUse[VER (qty)])
    shift++;
  while (VerUse[VER (qty)] || !VerUse[VER (1)])
    shift++;
#undef VER

  shift %= qty;
  if (shift)
  {
    tmp = ut_alloc_1d_int (qty);
    for (i = 0; i < qty; i++)
      tmp[(i + shift) % qty] = vers[i + 1];
    ut_array_1d_int_memcpy (tmp, qty, vers + 1);
    ut_free_1d_int (&tmp);
  }

  for (i = qty; i >= 1 && !VerUse[vers[i]]; i--)
    num++;

  return num;
}
