/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_tesr_.h"

void
nev_print_tesr (FILE * file, struct PRINT Print, struct TESR Tesr,
		struct TESRDATA TesrData)
{
  int i, j, k, elt, dim;
  double *size = ut_alloc_1d (3);
  struct TESS Tess;
  struct NODES Nodes;
  struct MESH *Mesh = calloc (4, sizeof (MESH));
  struct NODEDATA NodeData;
  struct MESHDATA *MeshData = calloc (4, sizeof (MESHDATA));
  int **tmp = NULL;

  dim = Tesr.Dim;

  for (i = 0; i < 3; i++)
    size[i] = Tesr.size[i] * Tesr.vsize[i];

  neut_tess_set_zero (&Tess);

  ut_print_message (0, 3, "Converting to mesh...\n");

  neut_nodes_set_zero (&Nodes);
  for (i = 0; i < 4; i++)
  {
    neut_mesh_set_zero (Mesh + i);
    neut_meshdata_set_default (MeshData + i);
  }
  neut_nodedata_set_default (&NodeData);

  neut_mesh_str (dim, Tesr.size, &Nodes, Mesh + dim, NULL);
  neut_nodes_scale (&Nodes, size[0], size[1], size[2]);
  neut_nodes_shift (&Nodes, Tesr.Origin[0], Tesr.Origin[1], Tesr.Origin[2]);

  elt = 0;
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	Mesh[dim].EltElset[++elt]
          = (!Print.showvox || Print.showvox[i][j][k]) ? Tesr.VoxCell[i][j][k] : 0;

  neut_mesh_init_elsets (Mesh + dim);
  neut_mesh_rmelset (Mesh + dim, Nodes, 0);
  neut_nodes_rmorphans (&Nodes, Mesh + dim, NULL);
  neut_mesh_init_nodeelts (Mesh + dim, Nodes.NodeQty);

  if (dim == 1)
  {
    Print.shownode = ut_alloc_1d_int (Nodes.NodeQty + 1);
    ut_array_1d_int_set (Print.shownode + 1, Nodes.NodeQty, 1);
    Print.showelt1d = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
    if (Print.showedge)
#pragma omp parallel for
      for (i = 1; i <= Mesh[dim].EltQty; i++)
	Print.showelt1d[i] = Print.showedge[Mesh[dim].EltElset[i]];
    else
      ut_array_1d_int_set (Print.showelt1d + 1, Mesh[dim].EltQty, 1);
  }
  else if (dim == 2)
  {
    Print.showelt2d = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
    if (Print.showface)
#pragma omp parallel for
      for (i = 1; i <= Mesh[dim].EltQty; i++)
	Print.showelt2d[i] = Print.showface[Mesh[dim].EltElset[i]];
    else
      ut_array_1d_int_set (Print.showelt2d + 1, Mesh[dim].EltQty, 1);
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
  }

  MeshData[dim].EltQty = Mesh[dim].EltQty;
  MeshData[dim].BCol = ut_alloc_1d_int (3);
  MeshData[dim].Col = ut_alloc_2d_int (MeshData[dim].EltQty + 1, 3);
#pragma omp parallel for
  for (i = 1; i <= MeshData[dim].EltQty; i++)
  {
    int id;
    double *coo = ut_alloc_1d (3);
    int *pos = ut_alloc_1d_int (3);
    neut_mesh_elt_centre (Nodes, Mesh[dim], i, coo);
    neut_tesr_coo_pos (Tesr, coo, 0, pos);
    id = pos[0]
      + (pos[1] - 1) * Tesr.size[0]
      + (pos[2] - 1) * (Tesr.size[1] * Tesr.size[0]);

    ut_array_1d_int_memcpy (MeshData[dim].Col[i], 3, TesrData.Col[id]);
    ut_free_1d_int (pos);
    ut_free_1d (coo);
  }

  if (dim == 1)
  {
    NodeData.NodeQty = Nodes.NodeQty;
    NodeData.Col = ut_alloc_2d_int (NodeData.NodeQty + 1, 3);
    MeshData[dim].Rad = ut_alloc_1d (MeshData[dim].EltQty + 1);
    ut_array_1d_set (MeshData[dim].Rad + 1, MeshData[dim].EltQty,
		     Tesr.vsize[0]);
    NodeData.Rad = ut_alloc_1d (NodeData.NodeQty + 1);
    ut_array_1d_set (NodeData.Rad + 1, NodeData.NodeQty, TesrData.BRad);

    NodeData.Col = ut_alloc_2d_int (NodeData.NodeQty + 1, 3);
    for (i = 1; i <= NodeData.NodeQty; i++)
      ut_array_1d_int_memcpy (NodeData.Col[i], 3, TesrData.BCol);
  }

  if (dim >= 2)
  {
    MeshData[dim].BRad = TesrData.BRad;
    ut_array_1d_int_memcpy (MeshData[dim].BCol, 3, TesrData.BCol);
  }

  NodeData.NodeQty = Nodes.NodeQty;
  NodeData.Coo = ut_alloc_2d (Nodes.NodeQty + 1, 3);
  ut_array_2d_memcpy (NodeData.Coo + 1, Nodes.NodeQty, 3, Nodes.NodeCoo + 1);

  if (dim == 1)
  {
    nev_print_mesh_nodes (file, Print, Nodes, Mesh, NodeData);
    nev_print_mesh_1d (file, Print, Tess, Mesh, NodeData, MeshData);
  }
  else if (dim == 2)
    nev_print_mesh_2d (file, Print, Nodes, Mesh, NodeData, MeshData);
  else if (dim == 3)
    nev_print_mesh_3d (file, Print, Nodes, Mesh, NodeData, MeshData, &tmp);

  ut_free_1d (size);

  neut_nodes_free (&Nodes);
  for (i = 0; i < 4; i++)
    neut_mesh_free (Mesh + i);
  free (Mesh);

  neut_nodedata_free (&NodeData);
  for (i = 0; i < 4; i++)
    neut_meshdata_free (MeshData + i);
  free (MeshData);

  return;
}
