/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
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
      fprintf (out, "  %.12f",
	       (fabs (Nodes.NodeCoo[i][j]) <
		1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (out, "\n");
  }

  return;
}

void
neut_mesh_fprintf_geof_elts (FILE * out, struct MESH Mesh1D,
			     struct MESH Mesh2D, struct MESH Mesh3D,
			     char *dim, int *shift)
{
  int i, j, eltnodeqty;
  int eltqty;

  eltqty = 0;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1"))
  {
    shift[1] = 0;
    eltqty += Mesh1D.EltQty;
  }
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
  {
    shift[2] = eltqty;
    eltqty += Mesh2D.EltQty;
  }
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
  {
    shift[3] = eltqty;
    eltqty += Mesh3D.EltQty;
  }

  fprintf (out, "\n**element\n");
  fprintf (out, "%d\n", eltqty);

  // 2D elts
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1") && Mesh1D.EltQty > 0)
  {
    eltnodeqty =
      neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
    if (Mesh1D.EltOrder == 1)
      for (i = 1; i <= Mesh1D.EltQty; i++)
      {
	fprintf (out, "%d l2d2 ", shift[1] + i);
	ut_array_1d_int_fprintf (out, Mesh1D.EltNodes[i], eltnodeqty, "%d");
      }

    else if (Mesh1D.EltOrder == 2)
      ut_print_message (1, 3,
			"2nd order line element not available.  Skipping...\n");
  }
  // 2D elts
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2") && Mesh2D.EltQty > 0)
  {
    eltnodeqty =
      neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
    if (!strcmp (Mesh2D.EltType, "tri"))
    {
      if (Mesh2D.EltOrder == 1)
	for (i = 1; i <= Mesh2D.EltQty; i++)
	{
	  fprintf (out, "%d c2d3 ", shift[2] + i);
	  ut_array_1d_int_fprintf (out, Mesh2D.EltNodes[i], eltnodeqty, "%d");
	}

      else if (Mesh2D.EltOrder == 2)
	for (i = 1; i <= Mesh2D.EltQty; i++)
	{
	  fprintf (out, "%d c2d6", shift[2] + i);
	  fprintf (out, " %d", Mesh2D.EltNodes[i][0]);
	  fprintf (out, " %d", Mesh2D.EltNodes[i][3]);
	  fprintf (out, " %d", Mesh2D.EltNodes[i][1]);
	  fprintf (out, " %d", Mesh2D.EltNodes[i][4]);
	  fprintf (out, " %d", Mesh2D.EltNodes[i][2]);
	  fprintf (out, " %d", Mesh2D.EltNodes[i][5]);
	  fprintf (out, "\n");
	}
    }

    else if (!strcmp (Mesh2D.EltType, "quad"))
    {
      if (Mesh2D.EltOrder == 1)
	for (i = 1; i <= Mesh2D.EltQty; i++)
	{
	  fprintf (out, "%d c2d4 ", shift[2] + i);
	  ut_array_1d_int_fprintf (out, Mesh2D.EltNodes[i], eltnodeqty, "%d");
	}

      else if (Mesh2D.EltOrder == 2)
	ut_error_reportbug ();	// FIXME
    }
  }

  // 3D elts
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3") && Mesh3D.EltQty > 0)
  {
    if (!strcmp (Mesh3D.EltType, "tri"))
    {
      if (Mesh3D.EltOrder == 1)
	for (i = 1; i <= Mesh3D.EltQty; i++)
	{
	  fprintf (out, "%d c3d4", shift[3] + i);
	  for (j = 0; j < 4; j++)
	    fprintf (out, " %d", Mesh3D.EltNodes[i][j]);
	  fprintf (out, "\n");
	}
      else if (Mesh3D.EltOrder == 2)
	for (i = 1; i <= Mesh3D.EltQty; i++)
	{
	  fprintf (out, "%d c3d10", shift[3] + i);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][0]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][1]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][2]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][4]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][5]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][6]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][7]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][9]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][8]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][3]);
	  fprintf (out, "\n");
	}
    }

    else if (!strcmp (Mesh3D.EltType, "quad"))
    {
      if (Mesh3D.EltOrder == 1)
      {
	for (i = 1; i <= Mesh3D.EltQty; i++)
	{
	  fprintf (out, "%d c3d8", i);

	  fprintf (out, " %d", Mesh3D.EltNodes[i][0]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][1]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][2]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][3]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][4]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][5]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][6]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][7]);

	  fprintf (out, "\n");
	}
      }
      else
      {
	for (i = 1; i <= Mesh3D.EltQty; i++)
	{
	  fprintf (out, "%d c3d20", i);

	  fprintf (out, " %d", Mesh3D.EltNodes[i][0]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][8]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][1]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][11]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][2]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][13]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][3]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][9]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][10]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][12]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][14]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][15]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][4]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][16]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][5]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][18]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][6]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][19]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][7]);
	  fprintf (out, " %d", Mesh3D.EltNodes[i][17]);

	  fprintf (out, "\n");
	}
      }
    }
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

  ut_string_separate (nset, NEUT_SEP_NODEP, &name, &qty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
	neut_mesh_fprintf_geof_nset (file, NSet0D.names[j], NSet0D.nodeqty[j],
				     NSet0D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
	neut_mesh_fprintf_geof_nset (file, NSet1D.names[j], NSet1D.nodeqty[j],
				     NSet1D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
	neut_mesh_fprintf_geof_nset (file, NSet2D.names[j], NSet2D.nodeqty[j],
				     NSet2D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  ut_free_2d_char (name, qty);

  return;
}

void
neut_mesh_fprintf_geof_sethead (FILE * file)
{
  fprintf (file, "***group\n");

  return;
}

void
neut_mesh_fprintf_geof_lisets (FILE* file, struct MESH Mesh2D,
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
neut_mesh_fprintf_geof_fasets (FILE* file, struct MESH Mesh3D,
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

  if (bsets && ut_string_inlist (bsets, NEUT_SEP_NODEP, "1"))
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
			       char *dim, int *shift)
{
  int i, j, Col;

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1") && Mesh1D.EltOrder == 1)
    for (i = 1; i <= Mesh1D.ElsetQty; i++)
    {
      fprintf (file, "**elset line%d\n", (!Mesh1D.ElsetId) ? i
	       : Mesh1D.ElsetId[i]);
      Col = 0;
      for (j = 1; j <= Mesh1D.Elsets[i][0]; j++)
	ut_print_wnc_int (file, Mesh1D.Elsets[i][j] + shift[1], &Col, 72);

      fprintf (file, "\n");
    }

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
    for (i = 1; i <= Mesh2D.ElsetQty; i++)
    {
      fprintf (file, "\n**elset face%d\n", (!Mesh2D.ElsetId) ? i
	       : Mesh2D.ElsetId[i]);
      Col = 0;
      for (j = 1; j <= Mesh2D.Elsets[i][0]; j++)
	ut_print_wnc_int (file, Mesh2D.Elsets[i][j] + shift[2], &Col, 72);

      fprintf (file, "\n");
    }

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      fprintf (file, "\n**elset poly%d\n", (!Mesh3D.ElsetId) ? i
	       : Mesh3D.ElsetId[i]);
      Col = 0;
      for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
	ut_print_wnc_int (file, Mesh3D.Elsets[i][j] + shift[3], &Col, 72);

      fprintf (file, "\n");
    }

  return;
}

void
neut_mesh_fprintf_geof_part (FILE * file, struct PART Part)
{
  neut_mesh_fprintf_geof_part_nset (file, Part);
  neut_mesh_fprintf_geof_part_elset (file, Part);

  return;
}

void
neut_mesh_fprintf_geof_foot (FILE * out)
{
  fprintf (out, "\n***return\n");

  return;
}
