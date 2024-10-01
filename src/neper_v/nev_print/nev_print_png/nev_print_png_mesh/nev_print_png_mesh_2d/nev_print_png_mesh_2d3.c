/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_mesh_2d_.h"

void
nev_print_png_mesh_2d_print_faces (FILE * file, struct PRINT Print,
                               struct NODES N, struct MESH M2D,
                               struct DATA NodeData,
                               struct DATA **MeshData)
{
  int i;
  int **rgb = NULL;
  char *coldatatype = ut_alloc_1d_char (10);
  double **Coo = ut_alloc_2d (N.NodeQty + 1, 3);

  // for some reasons, we have to reset NodeCoo to its original value
  ut_array_2d_memcpy (N.NodeCoo + 1, N.NodeQty, 3, Coo + 1);

  strcpy (coldatatype, "elt2d");
  if (MeshData[2][0].ColDataType
      && !strcmp (MeshData[2][0].ColDataType, "from_nodes"))
    strcpy (coldatatype, "nodes");

  if (!strncmp (coldatatype, "elt", 3))
  {
    rgb = ut_alloc_2d_int (M2D.EltQty + 1, 3);
    double* trs = ut_alloc_1d (M2D.EltQty + 1);

    for (i = 1; i <= M2D.EltQty; i++)
    {
      ut_array_1d_int_memcpy (MeshData[2][0].Col[i], 3, rgb[i]);
      if (MeshData[2][0].Trs)
        trs[i] = MeshData[2][0].Trs[i];
    }

    for (i = 1; i <= N.NodeQty; i++)
      ut_array_1d_memcpy (NodeData.Coo[i], 3, N.NodeCoo[i]);

    nev_print_png_mesh2d (file, N, M2D, Print.showelt2d, rgb, trs, "elt", Print);

    for (i = 1; i <= N.NodeQty; i++)
      ut_array_1d_memcpy (Coo[i], 3, N.NodeCoo[i]);

    ut_free_2d_int (&rgb, M2D.EltQty + 1);
    ut_free_1d (&trs);
  }

  else
  {
    rgb = ut_alloc_2d_int (N.NodeQty + 1, 3);

    for (i = 1; i <= N.NodeQty; i++)
      ut_array_1d_int_memcpy (NodeData.Col[i], 3, rgb[i]);

    for (i = 1; i <= N.NodeQty; i++)
      ut_array_1d_memcpy (NodeData.Coo[i], 3, N.NodeCoo[i]);

    nev_print_png_mesh2d (file, N, M2D, Print.showelt2d, rgb, NULL, "node", Print);

    for (i = 1; i <= N.NodeQty; i++)
      ut_array_1d_memcpy (Coo[i], 3, N.NodeCoo[i]);

    ut_free_2d_int (&rgb, N.NodeQty + 1);
  }

  ut_free_1d_char (&coldatatype);
  ut_free_2d (&Coo, N.NodeQty + 1);

  return;
}

void
nev_print_png_mesh_2d_print_edges (FILE * file, struct PRINT Print, struct NODES N,
                               struct MESH M1D, struct DATA **MeshData)
{
  int i;
  double Rad;
  char *texture = ut_alloc_1d_char (100);
  int *Col = ut_alloc_1d_int (3);

  if (MeshData[2][0].Qty > 0)
    ut_array_1d_int_memcpy (MeshData[2][0].BCol, 3, Col);
  else
    ut_array_1d_int_zero (Col, 3);

  Rad = MeshData[2][0].BRad;

  fprintf (file,
           "#declare elt3dedge =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
           Col[0] / 255., Col[1] / 255., Col[2] / 255., Print.lightambient, Print.lightdiffuse, Print.lightreflection);

  strcpy (texture, "elt3dedge");

  for (i = 1; i <= M1D.EltQty; i++)
    nev_print_png_segment_wsph (file, N.NodeCoo[M1D.EltNodes[i][0]],
                            N.NodeCoo[M1D.EltNodes[i][1]], Rad, texture);

  ut_free_1d_char (&texture);
  ut_free_1d_int (&Col);

  return;
}
