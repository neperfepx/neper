/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_.h"

void
nev_print_real_png_meshslices (FILE *file, struct PRINT Print, struct DATA **MeshData,
                               int SQty, struct NODES *SNodes,
                               struct MESH **SMesh, struct DATA *SNodeData,
                               struct DATA ***SMeshData, int **SElt2dElt3d)
{
  int i;

  for (i = 0; i < SQty; i++)
  {
    int j, *showelt2d = ut_alloc_1d_int (SMesh[i][2].EltQty + 1);

    if (Print.showelt3d[0] != -1)
      for (j = 1; j <= SMesh[i][2].EltQty; j++)
        showelt2d[j] = Print.showelt3d[SElt2dElt3d[i][j]];
    else
      ut_array_1d_int_set (showelt2d + 1, SMesh[i][2].EltQty, 1);

    if (ut_string_strcmp (SMeshData[i][2][0].ColDataType, "from_nodes"))
      nev_print_real_png_mesh2d (file, SNodes[i], SMesh[i][2], showelt2d,
                        SMeshData[i][2][0].Col, NULL, "elt", Print);
    else
      nev_print_real_png_mesh2d (file, SNodes[i], SMesh[i][2], showelt2d,
                            SNodeData[i].Col, NULL, "node", Print);

    if (Print.showsliceelt1d)
    {
      double *Rad = ut_alloc_1d (SMesh[i][1].EltQty + 1);
      ut_array_1d_set (Rad + 1, SMesh[i][1].EltQty, MeshData[1][0].Rad[1]);
      int **Col = ut_alloc_2d_int (SMesh[i][1].EltQty + 1, 3);
      for (j = 1; j <= SMesh[i][1].EltQty; j++)
        ut_array_1d_int_memcpy (MeshData[1][0].Col[1], 3, Col[j]);

      nev_print_real_png_mesh1d (file, SNodes[i], SMesh[i][1], NULL,
                            Col, Rad, Print);
      ut_free_1d (&Rad);
      ut_free_2d_int (&Col, SMesh[i][1].EltQty + 1);
    }

    ut_free_1d_int (&showelt2d);
  }

  return;
}
