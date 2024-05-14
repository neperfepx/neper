/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_mesh_1d_.h"

void
nev_print_png_mesh_1d_compress (struct PRINT Print, struct TESS Tess,
                                struct MESH *Mesh, int *hidden)
{
  int i, j, elset, elt3dqty;
  int *elts3d = NULL;

  // if 3D mesh is here, checking if it is hidden. If so, won't print it.
  if (Mesh[3].EltQty > 0)
  {
    ut_array_1d_int_set (hidden + 1, Mesh[1].EltQty, 1);
    for (i = 1; i <= Mesh[1].EltQty; i++)
      if (Print.showelt1d[i])
      {
        elset = Mesh[1].EltElset[i];

        if (Tess.EdgeQty == 0)
          hidden[i] = 0;
        else if (Tess.EdgeQty > 0 && Tess.EdgeDom[elset][0] >= 1)
          hidden[i] = 0;
        else if (!neut_mesh_elt1d_isembedded (Mesh[3], Mesh[1], i))
          hidden[i] = 0;
        else
        {
          neut_mesh_elt1d_elts3d (Mesh[1], i, Mesh[2], Mesh[3], &elts3d,
                                  &elt3dqty);

          for (j = 0; j < elt3dqty; j++)
            if (Print.showelt3d[elts3d[j]] == 0)
            {
              hidden[i] = 0;
              break;
            }
        }

        ut_free_1d_int (&elts3d);
      }
    hidden[0] = ut_array_1d_int_sum (hidden + 1, Mesh[1].EltQty);
  }

  ut_print_message (0, 4,
                    "Number of 1D elts      reduced by %3.0f%% (to %d).\n",
                    100 * (double) hidden[0] / ut_array_1d_int_sum (Print.showelt1d + 1, Mesh[1].EltQty),
                    ut_array_1d_int_sum (Print.showelt1d + 1, Mesh[1].EltQty) - hidden[0]);

  return;
}

void
nev_print_png_mesh_1d_print (FILE *file, struct PRINT Print, struct NODES Nodes,
                             struct MESH *Mesh, struct DATA NodeData,
                             struct DATA **MeshData, int *hidden)
{
  int i, *showelt = ut_alloc_1d_int (Mesh[1].EltQty + 1);
  double **Coo = ut_alloc_2d (Nodes.NodeQty + 1, 3);
  int **Col = ut_alloc_2d_int (Mesh[1].EltQty + 1, 3);
  double *Rad = ut_alloc_1d (Mesh[1].EltQty + 1);

  ut_array_2d_int_memcpy (MeshData[1][0].Col + 1, Mesh[1].EltQty, 3, Col + 1);
  ut_array_1d_memcpy (MeshData[1][0].Rad + 1, Mesh[1].EltQty, Rad + 1);

  for (i = 1; i <= Mesh[1].EltQty; i++)
    if (Print.showelt1d[i] == 1 && (!hidden || !hidden[i]))
      showelt[i] = 1;

  for (i = 1; i <= Nodes.NodeQty; i++)
    ut_array_1d_memcpy (NodeData.Coo[i], 3, Nodes.NodeCoo[i]);

  nev_print_png_mesh1d (file, Nodes, Mesh[1], showelt, Col, Rad, Print);

  for (i = 1; i <= Nodes.NodeQty; i++)
    ut_array_1d_memcpy (Coo[i], 3, Nodes.NodeCoo[i]);

  ut_free_1d_int (&showelt);
  ut_free_2d (&Coo, Nodes.NodeQty + 1);
  ut_free_2d_int (&Col, Mesh[1].EltQty + 1);
  ut_free_1d (&Rad);

  return;
}
