/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_geof_.h"

void
neut_mesh_fprintf_geof_elts_1d (FILE * file, struct MESH Mesh1D, int shift)
{
  int i, eltnodeqty;

  eltnodeqty =
    neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  if (Mesh1D.EltOrder == 1)
    for (i = 1; i <= Mesh1D.EltQty; i++)
    {
      fprintf (file, "%d l2d2 ", shift + i);
      ut_array_1d_int_fprintf (file, Mesh1D.EltNodes[i], eltnodeqty, "%d");
    }

  else if (Mesh1D.EltOrder == 2)
    ut_print_message (1, 3,
                      "2nd order line element not available.  Skipping...\n");

  return;
}

void
neut_mesh_fprintf_geof_elts_2d (FILE * file, struct MESH Mesh2D, int shift)
{
  int i, eltnodeqty;

  eltnodeqty =
    neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  if (!strcmp (Mesh2D.EltType, "tri"))
  {
    if (Mesh2D.EltOrder == 1)
      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
        fprintf (file, "%d c2d3 ", shift + i);
        ut_array_1d_int_fprintf (file, Mesh2D.EltNodes[i], eltnodeqty, "%d");
      }

    else if (Mesh2D.EltOrder == 2)
      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
        fprintf (file, "%d c2d6", shift + i);
        fprintf (file, " %d", Mesh2D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][3]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][5]);
        fprintf (file, "\n");
      }
  }

  else if (!strcmp (Mesh2D.EltType, "quad"))
  {
    if (Mesh2D.EltOrder == 1)
      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
        fprintf (file, "%d c2d4 ", shift + i);
        ut_array_1d_int_fprintf (file, Mesh2D.EltNodes[i], eltnodeqty, "%d");
      }

    else if (Mesh2D.EltOrder == 2)
      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
        fprintf (file, "%d c2d8 ", shift + i);
        fprintf (file, " %d", Mesh2D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][5]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][6]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][3]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][7]);

        fprintf (file, "\n");
      }
  }

  else if (!strcmp (Mesh2D.EltType, "quad6"))
  {
    if (!strcmp (Mesh2D.EltType, "quad6"))
      ut_print_message (1, 4,
                        "Elt type `quad6' undefined in Zset (the mesh will not open).\n");

    if (Mesh2D.EltOrder == 1)
      abort ();

    else if (Mesh2D.EltOrder == 2)
      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
        fprintf (file, "%d c2d4o2 ", shift + i);
        fprintf (file, " %d", Mesh2D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][3]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh2D.EltNodes[i][5]);

        fprintf (file, "\n");
      }
  }

  return;
}

void
neut_mesh_fprintf_geof_elts_3d (FILE * file, struct MESH Mesh3D, int shift)
{
  int i, j;

  if (!strcmp (Mesh3D.EltType, "tri"))
  {
    if (Mesh3D.EltOrder == 1)
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
        fprintf (file, "%d c3d4", shift + i);
        for (j = 0; j < 4; j++)
          fprintf (file, " %d", Mesh3D.EltNodes[i][j]);
        fprintf (file, "\n");
      }
    else if (Mesh3D.EltOrder == 2)
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
        fprintf (file, "%d c3d10", shift + i);
        fprintf (file, " %d", Mesh3D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][5]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][6]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][7]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][9]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][8]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][3]);
        fprintf (file, "\n");
      }
  }

  else if (!strcmp (Mesh3D.EltType, "quad"))
  {
    if (Mesh3D.EltOrder == 1)
    {
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
        fprintf (file, "%d c3d8", i);

        fprintf (file, " %d", Mesh3D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][3]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][5]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][6]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][7]);

        fprintf (file, "\n");
      }
    }
    else
    {
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
        fprintf (file, "%d c3d20", shift + i);

        fprintf (file, " %d", Mesh3D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][8]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][11]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][13]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][3]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][9]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][10]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][12]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][14]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][15]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][16]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][5]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][18]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][6]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][19]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][7]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][17]);

        fprintf (file, "\n");
      }
    }
  }

  else if (!strcmp (Mesh3D.EltType, "triprism"))
  {
    if (Mesh3D.EltOrder == 1)
      ut_print_message (1, 2, "This element type does not exist in Zset.\n");

    else
    {
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
        fprintf (file, "%d c3d15", shift + i);

        fprintf (file, " %d", Mesh3D.EltNodes[i][0]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][6]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][1]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][9]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][2]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][7]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][8]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][10]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][11]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][3]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][12]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][4]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][14]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][5]);
        fprintf (file, " %d", Mesh3D.EltNodes[i][13]);

        fprintf (file, "\n");
      }
    }
  }

  else
    abort ();
}

void
neut_mesh_fprintf_geof_nset (FILE * file, char *name, int NodeQty, int *nset)
{
  int i, Col;

  fprintf (file, "\n**nset %s\n", name);

  Col = 0;
  for (i = 0; i < NodeQty; i++)
    ut_print_wnc_int (file, nset[i], &Col, 72);

  fprintf (file, "\n");

  return;
}
