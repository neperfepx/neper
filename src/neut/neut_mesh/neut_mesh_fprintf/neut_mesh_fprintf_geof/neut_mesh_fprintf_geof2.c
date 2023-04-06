/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_geof_.h"

void
neut_mesh_fprintf_geof_head (FILE * file)
{
  fprintf (file, "***geometry\n");

  return;
}

void
neut_mesh_fprintf_geof_nodes (FILE * out, struct NODES Nodes)
{
  int i, j, dim;

  // checking the dimension of the mesh from the node coordinates
  dim = 2;
  for (i = 1; i <= Nodes.NodeQty; i++)
    if (Nodes.NodeCoo[i][2] != 0)
    {
      dim = 3;
      break;
    }

  fprintf (out, "**node\n");
  fprintf (out, "%d %d\n", Nodes.NodeQty, dim);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (out, "%d", i);
    for (j = 0; j < dim; j++)
      fprintf (out, "  " REAL_PRINT_FORMAT,
               (fabs (Nodes.NodeCoo[i][j]) <
                1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (out, "\n");
  }

  return;
}

void
neut_mesh_fprintf_geof_elts (FILE * out, struct MESH Mesh1D,
                             struct MESH Mesh2D, struct MESH Mesh3D,
                             struct MESH MeshCo, char *dim, int *shift)
{
  int EltQty;

  EltQty = 0;
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
  {
    shift[1] = 0;
    EltQty += Mesh1D.EltQty;
  }
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
  {
    shift[2] = EltQty;
    EltQty += Mesh2D.EltQty;
  }
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
  {
    shift[3] = EltQty;
    EltQty += Mesh3D.EltQty;
  }
  if (MeshCo.EltQty > 0)
  {
    shift[4] = EltQty;
    EltQty += MeshCo.EltQty;
  }

  fprintf (out, "\n**element\n");
  fprintf (out, "%d\n", EltQty);

  // 1D elts
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1") && Mesh1D.EltQty > 0)
    neut_mesh_fprintf_geof_elts_1d (out, Mesh1D, shift[1]);

  // 2D elts
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2") && Mesh2D.EltQty > 0)
    neut_mesh_fprintf_geof_elts_2d (out, Mesh2D, shift[2]);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3") && Mesh3D.EltQty > 0)
    neut_mesh_fprintf_geof_elts_3d (out, Mesh3D, shift[3]);

  if (MeshCo.EltQty > 0)
  {
    if (MeshCo.Dimension == 2)
      neut_mesh_fprintf_geof_elts_2d (out, MeshCo, shift[4]);
    else if (MeshCo.Dimension == 3)
      neut_mesh_fprintf_geof_elts_3d (out, MeshCo, shift[4]);
  }

  return;
}

void
neut_mesh_fprintf_geof_nsets (FILE * file, struct NSET NSet0D,
                              struct NSET NSet1D, struct NSET NSet2D,
                              char *nset)
{
  int i, j, status;
  char **name;
  int qty;

  ut_list_break (nset, NEUT_SEP_NODEP, &name, &qty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
        neut_mesh_fprintf_geof_nset (file, NSet0D.names[j], NSet0D.NodeQty[j],
                                     NSet0D.nodes[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
        neut_mesh_fprintf_geof_nset (file, NSet1D.names[j], NSet1D.NodeQty[j],
                                     NSet1D.nodes[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
        neut_mesh_fprintf_geof_nset (file, NSet2D.names[j], NSet2D.NodeQty[j],
                                     NSet2D.nodes[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  ut_free_2d_char (&name, qty);

  return;
}

void
neut_mesh_fprintf_geof_sethead (FILE * file)
{
  fprintf (file, "***group\n");

  return;
}

void
neut_mesh_fprintf_geof_lisets (FILE * file, struct MESH Mesh2D,
                               struct BOUNDARY Bound, char *fasets)
{
  int i, j, k;

  if (Mesh2D.Dimension != 2)
    abort ();

  if (strcmp (fasets, "internal"))
    return;

  fprintf (file, "\n");
  for (i = 1; i <= Bound.BoundQty; i++)
    if (Bound.BoundEltQty[i] > 0)
      for (j = 0; j < 2; j++)
      {
        fprintf (file, "**liset %s\n", Bound.BoundNames[i][j]);
        for (k = 1; k < Bound.BoundNodeQty[i]; k++)
          fprintf (file, "line %d %d\n", Bound.BoundNodes[i][j][k],
                   Bound.BoundNodes[i][j][k + 1]);
        fprintf (file, "\n");
      }

  return;
}

void
neut_mesh_fprintf_geof_lisets_all (FILE * file, struct MESH Mesh1D)
{
  int i, j, elt;

  if (Mesh1D.Dimension != 1)
    abort ();

  fprintf (file, "\n");
  for (i = 1; i <= Mesh1D.ElsetQty; i++)
  {
    fprintf (file, "**liset edge%d\n", i);
    for (j = 1; j <= Mesh1D.Elsets[i][0]; j++)
    {
      elt = Mesh1D.Elsets[i][j];
      if (Mesh1D.EltOrder == 1)
        fprintf (file, "line %d %d\n", Mesh1D.EltNodes[elt][0],
                 Mesh1D.EltNodes[elt][1]);
      else
        fprintf (file, "quad %d %d %d\n", Mesh1D.EltNodes[elt][0],
                 Mesh1D.EltNodes[elt][2], Mesh1D.EltNodes[elt][1]);
    }
    fprintf (file, "\n");
  }

  return;
}

void
neut_mesh_fprintf_geof_fasets (FILE * file, struct MESH Mesh3D,
                               struct BOUNDARY Bound, char *fasets)
{
  int i, j, k;

  if (Mesh3D.Dimension != 3)
    abort ();

  if (strcmp (fasets, "internal"))
    return;

  fprintf (file, "\n");
  for (i = 1; i <= Bound.BoundQty; i++)
    if (Bound.BoundEltQty[i] > 0)
      for (j = 0; j < 2; j++)
      {
        fprintf (file, "**faset %s\n", Bound.BoundNames[i][j]);
        for (k = 1; k <= Bound.BoundNodeQty[i]; k += 3)
          fprintf (file, "t3 %d %d %d\n", Bound.BoundNodes[i][j][k],
                   Bound.BoundNodes[i][j][k + 1],
                   Bound.BoundNodes[i][j][k + 2]);
        fprintf (file, "\n");
      }

  return;
}

/*
void
neut_mesh_fprintf_geof_bsets (FILE* file, struct MESH Mesh1D, char* bsets)
{
  int i, j;

  if (bsets && ut_list_testelt (bsets, NEUT_SEP_NODEP, "1"))
    for (i = 1; i <= Mesh1D.ElsetQty; i++)
    {
      fprintf (file, "\n**liset line%d\n", i);

      if (Mesh1D.EltOrder == 1)
	for (j = 1; j <= Mesh1D.Elsets[i][0]; j++)
	  fprintf (file, "line %d %d\n",
	      Mesh1D.EltNodes[Mesh1D.Elsets[i][j]][0],
	      Mesh1D.EltNodes[Mesh1D.Elsets[i][j]][1]);
      else if (Mesh1D.EltOrder == 2)
	for (j = 1; j <= Mesh1D.Elsets[i][0]; j++)
	  fprintf (file, "quad %d %d %d\n",
	      Mesh1D.EltNodes[Mesh1D.Elsets[i][j]][0],
	      Mesh1D.EltNodes[Mesh1D.Elsets[i][j]][2],
	      Mesh1D.EltNodes[Mesh1D.Elsets[i][j]][1]);

      fprintf (file, "\n");
    }

  return;
}
*/

void
neut_mesh_fprintf_geof_elsets (FILE * file, struct MESH Mesh1D,
                               struct MESH Mesh2D, struct MESH Mesh3D,
                               struct MESH MeshCo, char *dim, int *shift)
{
  int i, j, Col;

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1") && Mesh1D.EltOrder == 1)
    for (i = 1; i <= Mesh1D.ElsetQty; i++)
    {
      fprintf (file, "**elset %s\n", Mesh1D.ElsetLabels[i]);
      Col = 0;
      for (j = 1; j <= Mesh1D.Elsets[i][0]; j++)
        ut_print_wnc_int (file, Mesh1D.Elsets[i][j] + shift[1], &Col, 72);

      fprintf (file, "\n");
    }

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    for (i = 1; i <= Mesh2D.ElsetQty; i++)
    {
      fprintf (file, "\n**elset %s\n", Mesh2D.ElsetLabels[i]);
      Col = 0;
      for (j = 1; j <= Mesh2D.Elsets[i][0]; j++)
        ut_print_wnc_int (file, Mesh2D.Elsets[i][j] + shift[2], &Col, 72);

      fprintf (file, "\n");
    }

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      fprintf (file, "\n**elset %s\n", Mesh3D.ElsetLabels[i]);
      Col = 0;
      for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
        ut_print_wnc_int (file, Mesh3D.Elsets[i][j] + shift[3], &Col, 72);

      fprintf (file, "\n");
    }

  if (MeshCo.EltQty > 0)
    for (i = 1; i <= MeshCo.ElsetQty; i++)
    {
      fprintf (file, "\n**elset %s\n", MeshCo.ElsetLabels[i]);
      Col = 0;
      for (j = 1; j <= MeshCo.Elsets[i][0]; j++)
        ut_print_wnc_int (file, MeshCo.Elsets[i][j] + shift[4], &Col, 72);

      fprintf (file, "\n");
    }

  return;
}

void
neut_mesh_fprintf_geof_foot (FILE * out)
{
  fprintf (out, "\n***return\n");

  return;
}
