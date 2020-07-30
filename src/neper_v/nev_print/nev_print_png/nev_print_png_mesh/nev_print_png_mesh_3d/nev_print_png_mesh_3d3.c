/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_mesh_3d_.h"

void
nev_print_png_mesh_3d_print_faces (FILE * file, struct PRINT Print,
                               struct NODES N, struct MESH M2D,
                               int *elt2delt3d, int *nodes_new_old,
                               struct DATA NodeData,
                               struct DATA *MeshData)
{
  int i, node;
  int **rgb = NULL;
  char *coldatatype = ut_alloc_1d_char (10);

  strcpy (coldatatype, "elt3d");
  if (MeshData[3].ColDataType
      && !strcmp (MeshData[3].ColDataType, "from_nodes"))
    strcpy (coldatatype, "nodes");

  if (!strcmp (coldatatype, "elt3d"))
  {
    rgb = ut_alloc_2d_int (M2D.EltQty + 1, 3);

    for (i = 1; i <= M2D.EltQty; i++)
      ut_array_1d_int_memcpy (MeshData[3].Col[elt2delt3d[i]], 3, rgb[i]);

    for (i = 1; i <= N.NodeQty; i++)
    {
      node = (nodes_new_old) ? nodes_new_old[i] : i;
      ut_array_1d_memcpy (NodeData.Coo[node], 3, N.NodeCoo[i]);
    }

    nev_print_png_mesh2d (file, N, M2D, NULL, rgb, "elt", Print.showshadow);

    ut_free_2d_int (&rgb, M2D.EltQty + 1);
  }

  else
  {
    rgb = ut_alloc_2d_int (N.NodeQty + 1, 3);

    for (i = 1; i <= N.NodeQty; i++)
    {
      node = (nodes_new_old) ? nodes_new_old[i] : i;
      ut_array_1d_int_memcpy (NodeData.Col[node], 3, rgb[i]);
    }

    for (i = 1; i <= N.NodeQty; i++)
    {
      node = (nodes_new_old) ? nodes_new_old[i] : i;
      ut_array_1d_memcpy (NodeData.Coo[node], 3, N.NodeCoo[i]);
    }

    nev_print_png_mesh2d (file, N, M2D, NULL, rgb, "node", Print.showshadow);

    ut_free_2d_int (&rgb, N.NodeQty + 1);
  }

  ut_free_1d_char (&coldatatype);

  return;
}

void
nev_print_png_mesh_3d_print_edges (FILE * file, int showshadow, struct NODES N,
                               struct MESH M1D, struct DATA *MeshData)
{
  int i;
  double Rad;
  double ambient = showshadow ? 0.6 : 1;
  char *texture = ut_alloc_1d_char (100);
  char *string = ut_alloc_1d_char (100);
  int *Col = ut_alloc_1d_int (3);

  if (MeshData[3].Qty > 0)
    ut_array_1d_int_memcpy (MeshData[3].BCol, 3, Col);
  else if (MeshData[2].Qty > 0)
    ut_array_1d_int_memcpy (MeshData[2].BCol, 3, Col);
  else
    ut_array_1d_int_zero (Col, 3);

  if (MeshData[3].Qty > 0)
    Rad = MeshData[3].BRad;
  else if (MeshData[2].Qty > 0)
    Rad = MeshData[2].BRad;
  else
  {
    ut_print_neperbug ();
    abort ();
  }

  fprintf (file,
           "#declare elt3dedge =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
           Col[0] / 255., Col[1] / 255., Col[2] / 255., ambient);

  strcpy (texture, "elt3dedge");
  sprintf (string, "%.12f", Rad);

  for (i = 1; i <= M1D.EltQty; i++)
    nev_print_png_segment_wsph (file, N.NodeCoo[M1D.EltNodes[i][0]],
                            N.NodeCoo[M1D.EltNodes[i][1]], string, texture);

  ut_free_1d_char (&string);
  ut_free_1d_char (&texture);
  ut_free_1d_int (&Col);

  return;
}
