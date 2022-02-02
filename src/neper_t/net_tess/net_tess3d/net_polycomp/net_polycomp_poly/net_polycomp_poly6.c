/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_poly_.h"

/* FaceVerNb[Face] is rotated to put all the bad vertices at the end.
 * The program returns the number of bad vertices.
 */
int
RotFaceVerNb (struct POLYMOD *pPolymod, int *BadVer, int Face)
{
  int i, num = 0;               /* Mute variables */
  int pos, posb, tmp;           /* NbBadVer;      */

  /* While the first vertex of the face is a bad  vertex
   *    or the last  vertex of the face is a good vertex,
   * FaceVerNb[Face] is rotated.
   */

  if ((*pPolymod).FaceUse[Face] == 0)
  {
    printf ("\n");
    ut_print_message (2, 3,
                      "Tessellation failed because of a degenerated configuration.  Randomize to fix.\n");
  }

  while (ut_array_1d_int_eltpos
         (BadVer + 1, BadVer[0], (*pPolymod).FaceVerNb[Face][1]) == -1
         || ut_array_1d_int_eltpos (BadVer + 1, BadVer[0],
                                    (*pPolymod).FaceVerNb[Face][(*pPolymod).
                                                                FaceVerQty
                                                                [Face]]) !=
         -1)
    ut_array_1d_int_rotate ((*pPolymod).FaceVerNb[Face] + 1,
                            (*pPolymod).FaceVerQty[Face], -1);

  pos =
    ut_array_1d_int_eltpos (BadVer + 1, BadVer[0],
                            (*pPolymod).FaceVerNb[Face][(*pPolymod).
                                                        FaceVerQty[Face]]);
  posb =
    ut_array_1d_int_eltpos (BadVer + 1, BadVer[0],
                            (*pPolymod).FaceVerNb[Face][1]);
  while (pos == -1 || posb != -1)
  {
    tmp = (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]];
    for (i = (*pPolymod).FaceVerQty[Face]; i >= 2; i--)
      (*pPolymod).FaceVerNb[Face][i] = (*pPolymod).FaceVerNb[Face][i - 1];

    (*pPolymod).FaceVerNb[Face][1] = tmp;
    pos =
      ut_array_1d_int_eltpos (BadVer + 1, BadVer[0],
                              (*pPolymod).FaceVerNb[Face][(*pPolymod).
                                                          FaceVerQty[Face]]);
    posb =
      ut_array_1d_int_eltpos (BadVer + 1, BadVer[0],
                              (*pPolymod).FaceVerNb[Face][1]);
  }

  for (i = (*pPolymod).FaceVerQty[Face]; i >= 0; i--)
    if (ut_array_1d_int_eltpos
        (BadVer + 1, BadVer[0], (*pPolymod).FaceVerNb[Face][i]) != -1)
      num++;
    else
      break;

  return num;
}
