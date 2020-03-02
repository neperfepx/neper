/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_mesh_2d_.h"

void
nev_print_mesh_2d_print (FILE * file, struct PRINT Print, struct NODES N,
                         struct MESH M1D, struct MESH M2D,
                         struct NODEDATA NodeData, struct MESHDATA *MeshData)
{
  nev_print_mesh_2d_print_faces (file, Print, N, M2D, NodeData, MeshData);

  nev_print_mesh_2d_print_edges (file, Print.showshadow, N, M1D, MeshData);

  return;
}

void
nev_print_mesh_2d_compress (struct MESH Mesh2D, int *showelt,
                            struct MESH *pM1D)
{
  int i, j, k, qty, elt_min;
  int *elts_tmp = NULL;
  int elt2dnodeqty = neut_elt_nodeqty (Mesh2D.EltType, 2, 1);
  int *tmp = ut_alloc_1d_int (2);
  int **seq2 = NULL;

  neut_elt_boundlist (Mesh2D.EltType, 2, &seq2, &elt2dnodeqty, NULL);

  (*pM1D).Dimension = 1;
  (*pM1D).EltOrder = 1;
  ut_string_string (Mesh2D.EltType, &((*pM1D).EltType));

  for (i = 1; i <= Mesh2D.EltQty; i++)
    if (!showelt || showelt[i])
      for (j = 0; j < elt2dnodeqty; j++)
      {
        for (k = 0; k < 2; k++)
          tmp[k] = Mesh2D.EltNodes[i][seq2[j][k]];

        neut_mesh_nodes_comelts (Mesh2D, tmp, 2, &elts_tmp, &qty);
        elt_min = ut_array_1d_int_min (elts_tmp, qty);
        if (elt_min == i || (showelt && showelt[elt_min] == 0))
          neut_mesh_addelt (pM1D, tmp);
      }

  ut_free_1d_int (&elts_tmp);
  ut_free_1d_int (&tmp);

  return;
}
