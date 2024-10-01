/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_tesr_.h"

void
nev_print_png_tesr (FILE * file, struct PRINT Print, struct TESR Tesr,
                struct DATA* TesrData)
{
  int i, j, k, elt, dim = Tesr.Dim, voidelset = Tesr.CellQty + 1;
  double *size = ut_alloc_1d (3);
  struct TESS Tess;
  struct NODES Nodes;
  struct MESH *Mesh = calloc (5, sizeof (MESH));
  struct DATA NodeData;
  struct DATA **MeshData = calloc (5, sizeof (DATA *));
  for (i = 0; i < 5; i++)
      MeshData[i] = calloc (3, sizeof (DATA));
  int **tmp = NULL;

  for (i = 0; i < 3; i++)
    size[i] = Tesr.size[i] * Tesr.vsize[i];

  neut_tess_set_zero (&Tess);

  ut_print_message (0, 3, "Converting to mesh...\n");

  neut_nodes_set_zero (&Nodes);
  for (i = 0; i < 5; i++)
    neut_mesh_set_zero (Mesh + i);
  for (i = 0; i < 5; i++)
    for (j = 0; j < 3; j++)
    neut_data_set_default (MeshData[i] + j);
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
        if (Tesr.VoxCell && Tesr.VoxCell[i][j][k]
             && (!Print.showvoxstring || Print.showvox[i][j][k]))
          Mesh[dim].EltElset[elt] = Tesr.VoxCell[i][j][k];
        else if (Print.showvoidvoxstring && Print.showvox[i][j][k])
          Mesh[dim].EltElset[elt] = voidelset;
      }

  neut_mesh_init_elsets (Mesh + dim);

  // we want to be able to show voxels not assigned to cells
  // neut_mesh_rmelset (Mesh + dim, Nodes, 0);

  neut_nodes_rmorphans (&Nodes, Mesh + dim, NULL);
  neut_mesh_init_nodeelts (Mesh + dim, Nodes.NodeQty);

  if (Print.showedgestring && strcmp (Print.showedgestring, "0"))
  {
    neut_mesh_init_eltelset (Mesh + 2, NULL);
    neut_mesh2d_mesh1d (Nodes, Mesh[2], Mesh + 1, NULL, NULL, NULL, 0);
    Print.showelt1d = ut_alloc_1d_int (Mesh[1].EltQty + 1);

    neut_mesh_entity_expr_val_int (Nodes, Mesh, &Tess, NULL, NULL, NULL, NULL,
                                   "elt1d", Print.showedgestring, Print.showelt1d);
  }

  if (dim == 2)
  {
    Print.showelt2d = ut_alloc_1d_int (Mesh[dim].EltQty + 1);

    if (Print.showvox)
    {
      int id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
          {
            id++;
            Print.showelt2d[id] = Print.showvox[i][j][k];
          }
    }

    else
      ut_array_1d_int_set (Print.showelt2d + 1, Mesh[dim].EltQty, 1);

    for (i = 1; i <= Mesh[dim].EltQty; i++)
      if (Mesh[dim].EltElset[i] == voidelset)
        Print.showelt2d[i] = 1;
  }
  else if (dim == 3)
  {
    Print.showelt3d = ut_alloc_1d_int (Mesh[dim].EltQty + 1);

    if (Print.showvox)
    {
      int id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
          {
            id++;
            Print.showelt3d[id] = Print.showvox[i][j][k];
          }
    }

    else
      ut_array_1d_int_set (Print.showelt3d + 1, Mesh[dim].EltQty, 1);

    for (i = 1; i <= Mesh[dim].EltQty; i++)
      if (Mesh[dim].EltElset[i] == voidelset)
        Print.showelt3d[i] = 1;
  }

  MeshData[dim][0].Qty = Mesh[dim].EltQty;
  MeshData[dim][0].BCol = ut_alloc_1d_int (3);
  MeshData[dim][0].Col = ut_alloc_2d_int (MeshData[dim][0].Qty + 1, 3);
#pragma omp parallel for
  for (i = 1; i <= MeshData[dim][0].Qty; i++)
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

    ut_array_1d_int_memcpy (TesrData[1].Col[id], 3, MeshData[dim][0].Col[i]);

    ut_free_1d_int (&pos);
    ut_free_1d (&coo);
  }

  MeshData[dim][0].BRad = TesrData[1].BRad;
  ut_array_1d_int_memcpy (TesrData[1].BCol, 3, MeshData[dim][0].BCol);

  NodeData.Qty = Nodes.NodeQty;
  NodeData.Coo = ut_alloc_2d (Nodes.NodeQty + 1, 3);
  ut_array_2d_memcpy (Nodes.NodeCoo + 1, Nodes.NodeQty, 3, NodeData.Coo + 1);

  if (dim == 2)
  {
    // printing edges and subedges
    MeshData[1][0].Col = ut_alloc_2d_int (Mesh[1].EltQty + 1, 3);
    MeshData[1][0].Rad = ut_alloc_1d (Mesh[1].EltQty + 1);

    neut_mesh_entity_expr_val (Nodes, Mesh, &Tess, NULL, NULL, NULL, NULL,
                               "elt1d", TesrData[1].RadDataName, MeshData[1][0].Rad, NULL);

    nev_print_png_mesh_1d (file, Print, Tess, Nodes, Mesh, NodeData, MeshData);

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
    for (j = 0; j < 3; j++)
      neut_data_free (MeshData[i] + j);
    free (MeshData[i]);
  }
  free (Mesh);
  free (MeshData);

  neut_tess_free (&Tess);

  return;
}
