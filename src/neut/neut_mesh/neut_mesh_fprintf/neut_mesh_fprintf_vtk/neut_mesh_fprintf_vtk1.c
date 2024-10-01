/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_vtk_.h"

void
neut_mesh_fprintf_vtk (FILE * file, char *dim, struct NODES Nodes,
                       struct MESH Mesh1D, struct MESH Mesh2D,
                       struct MESH Mesh3D)
{
  neut_meshheader_fprintf_vtk (file);

  neut_nodes_fprintf_vtk (file, Nodes);

  neut_elts_fprintf_vtk (file, dim, Nodes, Mesh1D, Mesh2D, Mesh3D);

  return;
}

void
neut_meshheader_fprintf_vtk (FILE * file)
{
  fprintf (file, "# vtk DataFile Version 3.0\n");
  fprintf (file, "mesh\n");
  fprintf (file, "ASCII\n");
  fprintf (file, "DATASET UNSTRUCTURED_GRID\n");

  return;
}

void
neut_elts_fprintf_vtk (FILE * file, char *dim, struct NODES Nodes,
                       struct MESH Mesh1D, struct MESH Mesh2D,
                       struct MESH Mesh3D)
{
  int i, j, elset;
  int eltnodeqty1D, eltnodeqty2D, eltnodeqty3D, elttype_1D, elttype_2D,
    elttype_3D;
  int *shift = ut_alloc_1d_int (4);
  (void) Nodes;

  eltnodeqty1D =
    neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  eltnodeqty2D =
    neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  eltnodeqty3D =
    neut_elt_nodeqty (Mesh3D.EltType, Mesh3D.Dimension, Mesh3D.EltOrder);

  elttype_1D = -1;
  if (Mesh1D.EltOrder == 1)
    elttype_1D = 4;
  else if (Mesh1D.EltOrder == 2)
    elttype_1D = 21;

  elttype_2D = -1;
  if (Mesh2D.EltType && !strcmp (Mesh2D.EltType, "tri")
      && Mesh2D.EltOrder == 1)
    elttype_2D = 5;
  else if (Mesh2D.EltType && !strcmp (Mesh2D.EltType, "tri")
           && Mesh2D.EltOrder == 2)
    elttype_2D = 22;
  else if (Mesh2D.EltType && !strcmp (Mesh2D.EltType, "quad")
           && Mesh2D.EltOrder == 1)
    elttype_2D = 9;
  else if (Mesh2D.EltType && !strcmp (Mesh2D.EltType, "quad")
           && Mesh2D.EltOrder == 2)
    elttype_2D = 23;

  elttype_3D = -1;
  if (Mesh3D.EltType && !strcmp (Mesh3D.EltType, "tri")
      && Mesh3D.EltOrder == 1)
    elttype_3D = 10;
  else if (Mesh3D.EltType && !strcmp (Mesh3D.EltType, "tri")
           && Mesh3D.EltOrder == 2)
    elttype_3D = 24;
  else if (Mesh3D.EltType && !strcmp (Mesh3D.EltType, "quad")
           && Mesh3D.EltOrder == 1)
    elttype_3D = 12;
  else if (Mesh3D.EltType && !strcmp (Mesh3D.EltType, "quad")
           && Mesh3D.EltOrder == 2)
    elttype_3D = 25;

  // 1D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
  {
    fprintf (file, "CELLS %d %d\n", Mesh1D.EltQty,
             (1 + eltnodeqty1D) * Mesh1D.EltQty);

    for (i = 1; i <= Mesh1D.EltQty; i++)
    {
      fprintf (file, "%d ", eltnodeqty1D);
      for (j = 0; j < eltnodeqty1D; j++)
        fprintf (file, "%d ", Mesh1D.EltNodes[i][j] - 1);
      fprintf (file, "\n");
    }

    fprintf (file, "CELL_TYPES %d\n", Mesh1D.EltQty);
    for (i = 1; i <= Mesh1D.EltQty; i++)
      fprintf (file, "%d\n", elttype_1D);

    fprintf (file, "CELL_DATA %d\n", Mesh1D.EltQty);
    fprintf (file, "SCALARS edge double\n");
    fprintf (file, "LOOKUP_TABLE default\n");
    for (i = 1; i <= Mesh1D.EltQty; i++)
    {
      elset =
        (!Mesh1D.ElsetId) ? Mesh1D.EltElset[i] : Mesh1D.ElsetId[Mesh1D.
                                                                EltElset[i]];
      fprintf (file, "%f\n", (double) elset);
    }
  }

  // 2D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
  {
    fprintf (file, "CELLS %d %d\n", Mesh2D.EltQty,
             (1 + eltnodeqty2D) * Mesh2D.EltQty);

    if (!strcmp (Mesh2D.EltType, "tri"))
    {
      if (Mesh2D.EltOrder == 1)
        for (i = 1; i <= Mesh2D.EltQty; i++)
        {
          fprintf (file, "%d ", eltnodeqty2D);
          for (j = 0; j < eltnodeqty2D; j++)
            fprintf (file, "%d ", Mesh2D.EltNodes[i][j] - 1);
          fprintf (file, "\n");
        }
    }
    else if (!strcmp (Mesh2D.EltType, "quad"))
    {
      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
        fprintf (file, "%d ", eltnodeqty2D);
        for (j = 0; j < eltnodeqty2D; j++)
          fprintf (file, "%d ", Mesh2D.EltNodes[i][j] - 1);
        fprintf (file, "\n");
      }
    }

    fprintf (file, "CELL_TYPES %d\n", Mesh2D.EltQty);
    for (i = 1; i <= Mesh2D.EltQty; i++)
      fprintf (file, "%d\n", elttype_2D);

    fprintf (file, "CELL_DATA %d\n", Mesh2D.EltQty);
    fprintf (file, "SCALARS face double\n");
    fprintf (file, "LOOKUP_TABLE default\n");
    for (i = 1; i <= Mesh2D.EltQty; i++)
    {
      elset =
        (!Mesh2D.ElsetId) ? Mesh2D.EltElset[i] : Mesh2D.ElsetId[Mesh2D.
                                                                EltElset[i]];
      fprintf (file, "%f\n", (double) elset);
    }
  }

  // 3D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
  {
    fprintf (file, "CELLS %d %d\n", Mesh3D.EltQty,
             (1 + eltnodeqty3D) * Mesh3D.EltQty);

    if (!strcmp (Mesh3D.EltType, "tri"))
    {
      if (Mesh3D.EltOrder == 1)
        for (i = 1; i <= Mesh3D.EltQty; i++)
        {
          fprintf (file, "%d ", eltnodeqty3D);
          for (j = 0; j < eltnodeqty3D; j++)
            fprintf (file, "%d ", Mesh3D.EltNodes[i][j] - 1);
          fprintf (file, "\n");
        }
      else if (Mesh3D.EltOrder == 2)
        for (i = 1; i <= Mesh3D.EltQty; i++)
        {
          fprintf (file, "%d ", eltnodeqty3D);
          for (j = 0; j < 8; j++)
            fprintf (file, "%d ", Mesh3D.EltNodes[i][j] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][9] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][8] - 1);
          fprintf (file, "\n");
        }
    }

    else if (!strcmp (Mesh3D.EltType, "quad"))
    {
      if (Mesh3D.EltOrder == 1)
        for (i = 1; i <= Mesh3D.EltQty; i++)
        {
          fprintf (file, "%d ", eltnodeqty3D);
          for (j = 0; j < eltnodeqty3D; j++)
            fprintf (file, "%d ", Mesh3D.EltNodes[i][j] - 1);
          fprintf (file, "\n");
        }
      if (Mesh3D.EltOrder == 2)
        for (i = 1; i <= Mesh3D.EltQty; i++)
        {
          fprintf (file, "%d ", eltnodeqty3D);
          for (j = 0; j < 8; j++)
            fprintf (file, "%d ", Mesh3D.EltNodes[i][j] - 1);

          fprintf (file, "%d ", Mesh3D.EltNodes[i][8] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][11] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][13] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][9] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][16] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][18] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][19] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][17] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][10] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][12] - 1);
          fprintf (file, "%d ", Mesh3D.EltNodes[i][14] - 1);
          fprintf (file, "%d\n", Mesh3D.EltNodes[i][15] - 1);
        }
    }

    fprintf (file, "CELL_TYPES %d\n", Mesh3D.EltQty);
    for (i = 1; i <= Mesh3D.EltQty; i++)
      fprintf (file, "%d\n", elttype_3D);

    fprintf (file, "CELL_DATA %d\n", Mesh3D.EltQty);
    fprintf (file, "SCALARS poly double\n");
    fprintf (file, "LOOKUP_TABLE default\n");
    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      elset =
        (!Mesh3D.ElsetId) ? Mesh3D.EltElset[i] : Mesh3D.ElsetId[Mesh3D.
                                                                EltElset[i]];
      fprintf (file, "%f\n", (double) elset);
    }
  }

  if (Mesh3D.PartQty > 0)
  {
    fprintf (file, "SCALARS part double\n");
    fprintf (file, "LOOKUP_TABLE default\n");
    if (Mesh3D.EltQty > 0)
      for (i = 1; i <= Mesh3D.EltQty; i++)
        fprintf (file, "%f\n", (double) Mesh3D.EltPart[i]);
    else if (Mesh2D.EltQty > 0)
      for (i = 1; i <= Mesh2D.EltQty; i++)
        fprintf (file, "%f\n", (double) Mesh3D.EltPart[i]);
    else if (Mesh1D.EltQty > 0)
      for (i = 1; i <= Mesh1D.EltQty; i++)
        fprintf (file, "%f\n", (double) Mesh3D.EltPart[i]);
    else
      abort ();
  }

  ut_free_1d_int (&shift);

  return;
}

void
neut_nodes_fprintf_vtk (FILE * file, struct NODES Nodes)
{
  int i, j;

  fprintf (file, "POINTS %d double\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    for (j = 0; j < 3; j++)
      fprintf (file, " " REAL_PRINT_FORMAT,
               (fabs (Nodes.NodeCoo[i][j]) <
                1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }

  return;
}
