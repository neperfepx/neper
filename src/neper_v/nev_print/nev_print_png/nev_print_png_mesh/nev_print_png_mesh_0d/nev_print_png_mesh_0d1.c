/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_mesh_0d_.h"

void
nev_print_png_mesh_0d (FILE * file, struct PRINT Print, struct MESH *Mesh,
                   struct DATA NodeData, struct DATA *MeshData)
{
  int i;
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;
  char *texture = ut_alloc_1d_char (100);

  if (MeshData[0].Qty > 0)
  {
    int printelt0d_qty = 0;

    for (i = 1; i <= Mesh[0].EltQty; i++)
      if (Print.showelt0d[i] == 1)
      {
        fprintf (file,
                 "#declare elt0d%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
                 i, MeshData[0].Col[i][0] / 255.,
                 MeshData[0].Col[i][1] / 255., MeshData[0].Col[i][2] / 255.,
                 ambient);

        sprintf (texture, "elt0d%d", i);

        int l, print = 0;
        l = 1;
        print = l;

        if (print == 1)
        {
          char *string = ut_alloc_1d_char (100);
          sprintf (string, "%.12f", MeshData[0].Rad[i]);
          nev_print_png_sphere (file, NodeData.Coo[Mesh[0].EltNodes[i][0]],
                            string, texture);

          printelt0d_qty++;
          ut_free_1d_char (&string);
        }
      }

    ut_print_message (0, 4,
                      "Number of 0D elts      reduced by %3.0f%% (to %d).\n",
                      100 * (double) (MeshData[0].Qty -
                                      printelt0d_qty) /
                      MeshData[0].Qty, printelt0d_qty);
  }

  return;
}
