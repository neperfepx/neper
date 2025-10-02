/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_onecell_.h"

int
net_tess_onecell (struct IN_T In, struct MTESS *pMTess,
               struct TESS *Tess,
               int dtess, int dcell, int TessId, struct SEEDSET *SSet)
{
  int i;
  double *O = ut_alloc_1d (4);

  neut_tess_poly_tess (Tess[dtess], dcell, Tess + TessId);

  Tess[TessId].Level = Tess[dtess].Level + 1;

  for (i = 1; i <= Tess[TessId].FaceQty; i++)
    if (ut_space_point_plane_side (O, Tess[TessId].FaceEq[i]) > 0)
    {
      ut_array_1d_int_reverseelts (Tess[TessId].FaceEdgeNb[i] + 2,
                                   Tess[TessId].FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts (Tess[TessId].FaceVerNb[i] + 2,
                                   Tess[TessId].FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts (Tess[TessId].FaceEdgeOri[i] + 2,
                                   Tess[TessId].FaceVerQty[i] - 1);
      ut_array_1d_int_scale (Tess[TessId].FaceEdgeOri[i] + 1,
                             Tess[TessId].FaceVerQty[i], -1);

      neut_tess_face_fixorifromedges (Tess[TessId], i, Tess[TessId].FaceEq[i]);
      Tess[TessId].PolyFaceOri[1][1 + ut_array_1d_int_eltpos (Tess[TessId].PolyFaceNb[1] + 1, Tess[TessId].PolyFaceQty[1], i)] *= -1;
    }

  SSet[TessId].Dim = Tess[TessId].Dim;
  SSet[TessId].N = 1;
  SSet[TessId].Nall = 1;
  SSet[TessId].SeedCoo = ut_alloc_2d (SSet[TessId].N + 1, 3);
  SSet[TessId].SeedWeight = ut_alloc_1d (SSet[TessId].N + 1);
  SSet[TessId].SeedCoo0 = ut_alloc_2d (SSet[TessId].N + 1, 3);
  neut_tess_cell_centroid (Tess[TessId], 1, SSet[TessId].SeedCoo[1]);
  ut_array_1d_memcpy (SSet[TessId].SeedCoo[1], 3, SSet[TessId].SeedCoo0[1]);
  SSet[TessId].Size = ut_alloc_2d (3, 2);
  neut_tess_bbox (Tess[TessId], SSet[TessId].Size);

  // net_tess3d_domain (Tess[dtess], dcell, TessId, pMTess, Tess + TessId);
  neut_tess_init_domain_poly (Tess + TessId, Tess[dtess], dcell, NULL, NULL, NULL);

  net_ori_mtess_id (In, *pMTess, Tess, 0, 1, SSet + 1);

  net_ori_mtess_randseed (*pMTess, Tess, 0, 1, SSet, 1, SSet + 1);

  if (ut_array_1d_int_sum (In.periodic, 3) > 0
   && !strncmp (Tess[TessId].DomType, "rodrigues", 9))
    net_tess3d_periodic (Tess + TessId);

  ut_string_string ("triclinic", &(Tess[TessId].CellCrySym));

  ut_free_1d (&O);

  return 0;
}
