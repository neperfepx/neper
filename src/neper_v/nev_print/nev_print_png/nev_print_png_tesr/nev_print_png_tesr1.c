/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_tesr_.h"

void
nev_print_png_tesr (FILE * file, struct PRINT Print, struct TESR Tesr,
                struct DATA TesrData)
{
  int i, j, k, elt, dim, voidelset = Tesr.CellQty + 1;
  double *size = ut_alloc_1d (3);
  struct TESS Tess;
  struct NODES Nodes;
  struct MESH *Mesh = calloc (5, sizeof (MESH));
  struct DATA NodeData;
  struct DATA *MeshData = calloc (5, sizeof (DATA));
  int **tmp = NULL;

  dim = Tesr.Dim;

  for (i = 0; i < 3; i++)
    size[i] = Tesr.size[i] * Tesr.vsize[i];

  neut_tess_set_zero (&Tess);

  ut_print_message (0, 3, "Converting to mesh...\n");

  neut_nodes_set_zero (&Nodes);
  for (i = 0; i < 5; i++)
  {
    neut_mesh_set_zero (Mesh + i);
    neut_data_set_default (MeshData + i);
  }
  neut_data_set_default (&NodeData);

  neut_mesh_str (dim, Tesr.size, &Nodes, Mesh + dim, NULL);
  neut_nodes_scale (&Nodes, size[0], size[1], size[2]);
  neut_nodes_shift (&Nodes, Tesr.Origin[0], Tesr.Origin[1], Tesr.Origin[2]);

  elt = 0;
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
      {
        elt++;
        if (Tesr.VoxCell[i][j][k]
         && (!Print.showvoxstring || Print.showvox[i][j][k]))
          Mesh[dim].EltElset[elt] = Tesr.VoxCell[i][j][k];
        else if (Print.showvoidvoxstring && Print.showvox[i][j][k])
          Mesh[dim].EltElset[elt] = voidelset;
      }

  neut_mesh_init_elsets (Mesh + dim);

  neut_mesh_rmelset (Mesh + dim, Nodes, 0);

  neut_nodes_rmorphans (&Nodes, Mesh + dim, NULL);
  neut_mesh_init_nodeelts (Mesh + dim, Nodes.NodeQty);

  if (Print.showedgestring && strcmp (Print.showedgestring, "0"))
  {
    neut_mesh_init_eltelset (Mesh + 2, NULL);
    neut_mesh2d_mesh1d (Nodes, Mesh[2], Mesh + 1, NULL, NULL, NULL, 0);
    Print.showelt1d = ut_alloc_1d_int (Mesh[1].EltQty + 1);

    neut_mesh_entity_expr_val_int (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                                   Mesh[4], Tess, NULL, NULL, NULL, NULL,
                                   "elt1d", Print.showedgestring, Print.showelt1d);
  }

  if (dim == 2)
  {
    Print.showelt2d = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
    if (Print.showface)
#pragma omp parallel for
      for (i = 1; i <= Mesh[dim].EltQty; i++)
        Print.showelt2d[i] = Print.showface[Mesh[dim].EltElset[i]];
    else
      ut_array_1d_int_set (Print.showelt2d + 1, Mesh[dim].EltQty, 1);

    for (i = 1; i <= Mesh[dim].EltQty; i++)
      if (Mesh[dim].EltElset[i] == voidelset)
        Print.showelt2d[i] = 1;
  }
  else if (dim == 3)
  {
    Print.showelt3d = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
    if (Print.showpoly)
#pragma omp parallel for
      for (i = 1; i <= Mesh[dim].EltQty; i++)
        Print.showelt3d[i] = Print.showpoly[Mesh[dim].EltElset[i]];
    else
      ut_array_1d_int_set (Print.showelt3d + 1, Mesh[dim].EltQty, 1);

    for (i = 1; i <= Mesh[dim].EltQty; i++)
      if (Mesh[dim].EltElset[i] == voidelset)
        Print.showelt3d[i] = 1;
  }

  MeshData[dim].Qty = Mesh[dim].EltQty;
  MeshData[dim].BCol = ut_alloc_1d_int (3);
  MeshData[dim].Col = ut_alloc_2d_int (MeshData[dim].Qty + 1, 3);
#pragma omp parallel for
  for (i = 1; i <= MeshData[dim].Qty; i++)
  {
    int id;
    double *coo = ut_alloc_1d (3);
    int *pos = ut_alloc_1d_int (3);
    neut_mesh_elt_centre (Nodes, Mesh[dim], i, coo);
    neut_tesr_coo_pos (Tesr, coo, 0, pos);
    id =
      pos[0] + (pos[1] - 1) * Tesr.size[0] + (pos[2] -
                                              1) * (Tesr.size[1] *
                                                    Tesr.size[0]);

    if (Tesr.VoxCell[pos[0]][pos[1]][pos[2]])
      ut_array_1d_int_memcpy (TesrData.Col[id], 3, MeshData[dim].Col[i]);
    else
      ut_array_1d_int_memcpy (TesrData.VoidCol, 3, MeshData[dim].Col[i]);

    ut_free_1d_int (&pos);
    ut_free_1d (&coo);
  }

  MeshData[dim].BRad = TesrData.BRad;
  ut_array_1d_int_memcpy (TesrData.BCol, 3, MeshData[dim].BCol);

  NodeData.Qty = Nodes.NodeQty;
  NodeData.Coo = ut_alloc_2d (Nodes.NodeQty + 1, 3);
  ut_array_2d_memcpy (Nodes.NodeCoo + 1, Nodes.NodeQty, 3, NodeData.Coo + 1);

  if (dim == 2)
  {
    // printing edges and subedges
    MeshData[1].Col = ut_alloc_2d_int (Mesh[1].EltQty + 1, 3);
    MeshData[1].Rad = ut_alloc_1d (Mesh[1].EltQty + 1);

    neut_mesh_entity_expr_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                               Mesh[4], Tess, NULL, NULL, NULL, NULL,
                               "elt1d", TesrData.RadDataName, MeshData[1].Rad, NULL);

    nev_print_png_mesh_1d (file, Print, Tess, Mesh, NodeData, MeshData);

    // printing tesr
    nev_print_png_mesh_2d (file, Print, Nodes, Mesh, NodeData, MeshData);
  }
  else if (dim == 3)
    nev_print_png_mesh_3d (file, Print, Nodes, Mesh, NodeData, MeshData, &tmp);

  ut_free_1d (&size);

  neut_nodes_free (&Nodes);
  neut_data_free (&NodeData);

  for (i = 0; i < 5; i++)
  {
    neut_mesh_free (Mesh + i);
    neut_data_free (MeshData + i);
  }
  free (Mesh);
  free (MeshData);

  neut_tess_free (&Tess);

  return;
}
