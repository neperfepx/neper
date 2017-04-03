/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_inp_.h"

void
neut_mesh_fprintf_inp_mesh2d (FILE *file, struct MESH Mesh2D,
			      int shift_elt, char *type)
{
  int i, j;
  int eltnodeqty2D = neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  int seq2d[6] = { 0, 2, 1, 5, 4, 3 };

  if (!strcmp (Mesh2D.EltType, "tri"))
  {
    fprintf (file, "*Element, type=");
    if (Mesh2D.EltOrder == 1)
      fprintf (file, "CPE3\n");
    else if (Mesh2D.EltOrder == 2)
      fprintf (file, "CPE6\n");

    for (i = 1; i <= Mesh2D.EltQty; i++)
    {
      fprintf (file, "%d, ", i + shift_elt);
      for (j = 0; j < eltnodeqty2D - 1; j++)
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][seq2d[j]]);
      fprintf (file, "%d\n", Mesh2D.EltNodes[i][seq2d[eltnodeqty2D - 1]]);
    }
  }
  else if (!strcmp (Mesh2D.EltType, "quad"))
  {
    fprintf (file, "*Element, type=");

    if (Mesh2D.EltOrder == 1)
    {
      if (!type || !strcmp (type, "standard"))
	fprintf (file, "C2D4\n");
      else if (!strcmp (type, "cohesive"))
	fprintf (file, "COH2D4\n");
      else
	abort ();

      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
	fprintf (file, "%d, ", i + shift_elt);

	fprintf (file, " %d,", Mesh2D.EltNodes[i][0]);
	fprintf (file, " %d,", Mesh2D.EltNodes[i][1]);
	fprintf (file, " %d,", Mesh2D.EltNodes[i][2]);
	fprintf (file, " %d\n", Mesh2D.EltNodes[i][3]);
      }
    }
    else
    {
      if (!type || !strcmp (type, "standard"))
	fprintf (file, "C2D8\n");
      else if (!strcmp (type, "cohesive"))
	fprintf (file, "COH2D8\n");
      else
	abort ();

      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
	fprintf (file, "%d, ", i + shift_elt);

	fprintf (file, "%d, ", Mesh2D.EltNodes[i][0]);
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][1]);
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][2]);
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][3]);
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][4]);
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][5]);
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][6]);
	fprintf (file, "%d\n", Mesh2D.EltNodes[i][7]);
      }
    }
  }

  fprintf (file, "\n");
}

void
neut_mesh_fprintf_inp_mesh3d (FILE *file, struct MESH Mesh3D, int shift_elt, char *type)
{
  int i, j;
  int eltnodeqty3D = neut_elt_nodeqty (Mesh3D.EltType, Mesh3D.Dimension, Mesh3D.EltOrder);
  int seq_tri[10] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 8};
  int seq_quad[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 13, 9, 16, 18,
		      19, 17, 10, 12, 14, 15};
  int seq_triprism[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

  fprintf (file, "*Element, type=");

  if (!strcmp (Mesh3D.EltType, "tri"))
  {
    if (Mesh3D.EltOrder == 1)
      fprintf (file, "C3D4\n");
    else if (Mesh3D.EltOrder == 2)
      fprintf (file, "C3D10\n");

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      fprintf (file, "%d, ", i + shift_elt);
      for (j = 0; j < eltnodeqty3D - 1; j++)
	fprintf (file, "%d, ", Mesh3D.EltNodes[i][seq_tri[j]]);
      fprintf (file, "%d\n", Mesh3D.EltNodes[i][seq_tri[eltnodeqty3D - 1]]);
    }
  }

  else if (!strcmp (Mesh3D.EltType, "quad"))
  {
    if (Mesh3D.EltOrder == 1)
      fprintf (file, "C3D8\n");
    else
      fprintf (file, "C3D20\n");

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      fprintf (file, "%d,", i + shift_elt);
      for (j = 0; j < eltnodeqty3D - 1; j++)
	fprintf (file, "%d, ", Mesh3D.EltNodes[i][seq_quad[j]]);
      fprintf (file, "%d\n", Mesh3D.EltNodes[i][seq_quad[eltnodeqty3D - 1]]);
    }
  }

  else if (!strcmp (Mesh3D.EltType, "triprism"))
  {
    if (Mesh3D.EltOrder == 1)
    {
      if (!type || !strcmp (type, "standard"))
	fprintf (file, "C3D6\n");
      else if (!strcmp (type, "cohesive"))
	fprintf (file, "COH3D6\n");
    }
    else if (Mesh3D.EltOrder == 2)
    {
      if (!type || !strcmp (type, "standard"))
	fprintf (file, "C3D15\n");
      else if (!strcmp (type, "cohesive"))
	fprintf (file, "COH3D15\n");
    }

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      fprintf (file, "%d, ", i + shift_elt);
      for (j = 0; j < eltnodeqty3D - 1; j++)
	fprintf (file, "%d, ", Mesh3D.EltNodes[i][seq_triprism[j]]);
      fprintf (file, "%d\n", Mesh3D.EltNodes[i][seq_triprism[eltnodeqty3D - 1]]);
    }
  }

  else
    ut_error_reportbug ();

  fprintf (file, "\n");

  return;
}

void
neut_mesh_fprintf_inp_elsets (FILE *file, struct MESH Mesh,
			      char *name, int shift_elt)
{
  int i, j, Col, call;

  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    fprintf (file, "*Elset, elset=%s%d\n", name,
	     (!Mesh.ElsetId) ? i : Mesh.ElsetId[i]);
    Col = 0;
    call = 0;
    for (j = 1; j <= Mesh.Elsets[i][0]; j++)
      ut_print_wnc_wncall (file, &Col, 72, &call, 16,
			   (j < Mesh.Elsets[i][0]) ? "%d," : "%d\n",
			   Mesh.Elsets[i][j] + shift_elt);
    fprintf (file, "\n");
  }

  return;
}

void
neut_mesh_fprintf_inp_nsets (FILE * file, struct NSET NSet0D,
			     struct NSET NSet1D, struct NSET NSet2D,
			     char *nset, char* instance)
{
  int i, j, status;
  char **name = NULL;
  int qty;

  ut_string_separate (nset, NEUT_SEP_NODEP, &name, &qty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
	neut_mesh_fprintf_inp_nset (file, NSet0D.names[j], NSet0D.nodeqty[j],
				    NSet0D.nodes[j], instance);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
	neut_mesh_fprintf_inp_nset (file, NSet1D.names[j], NSet1D.nodeqty[j],
				    NSet1D.nodes[j], instance);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
	neut_mesh_fprintf_inp_nset (file, NSet2D.names[j], NSet2D.nodeqty[j],
				    NSet2D.nodes[j], instance);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  return;
}

void
neut_mesh_fprintf_inp_nset (FILE * file, char *name, int qty,
                            int *nodeset, char* instance)
{
  int i, Col, call;

  fprintf (file, "*Nset, nset=%s", name);
  if (instance)
    fprintf (file, ", instance=%s", instance);
  fprintf (file, "\n");

  Col = 0;
  call = 0;
  for (i = 0; i < qty; i++)
    ut_print_wnc_wncall (file, &Col, 72, &call, 16,
			 (i < qty - 1) ? "%d," : "%d\n", nodeset[i]);
  fprintf (file, "\n");

  return;
}

void
neut_mesh_fprintf_inp_part_nset (FILE * file, struct PART Part)
{
  int p, i, Col, call, nodeqty;
  int *nodes = ut_alloc_1d_int (Part.node_parts[0]);

  for (p = 0; p < Part.qty; p++)
  {
    nodeqty = 0;
    for (i = 1; i <= Part.node_parts[0]; i++)
      if (Part.node_parts[i] == p)
	nodes[nodeqty++] = i;

    fprintf (file, "*Nset, nset=part%d\n", p + 1);
    Col = 0;
    call = 0;
    for (i = 0; i < nodeqty; i++)
      ut_print_wnc_wncall (file, &Col, 72, &call, 16,
			   (i < nodeqty - 1) ? "%d," : "%d\n", nodes[i]);

    fprintf (file, "\n");
  }

  ut_free_1d_int (nodes);

  return;
}

void
neut_mesh_fprintf_inp_part_elset (FILE * file, struct PART Part)
{
  int p, i, Col, eltqty, call;
  int *elts = ut_alloc_1d_int (Part.elt_parts[0]);

  for (p = 0; p < Part.qty; p++)
  {
    eltqty = 0;
    for (i = 1; i <= Part.elt_parts[0]; i++)
      if (Part.elt_parts[i] == p)
	elts[eltqty++] = i;

    fprintf (file, "*Elset, elset=part%d\n", p + 1);
    Col = 0;
    call = 0;
    for (i = 0; i < eltqty; i++)
      ut_print_wnc_wncall (file, &Col, 72, &call, 16,
			   (i < eltqty - 1) ? "%d," : "%d\n", elts[i]);

    fprintf (file, "\n");
  }

  return;
}

void
neut_mesh_fprintf_inp_boundary (FILE* file, struct BOUNDARY Bound)
{
  int i, j, k;

  for (i = 1; i <= Bound.BoundQty; i++)
    if (Bound.BoundDom[i][0] == -1)
      for (j = 0; j < 2; j++)
	if (Bound.BoundNames[i][j] && strlen (Bound.BoundNames[i][j]) > 0)
	{
	  for (k = 1; k <= Bound.BoundEltQty[i]; k++)
	  {
	    fprintf (file, "*Elset, elset=elt%d-S%d, internal, instance=tess\n",
		     Bound.BoundElts[i][j][k],
		     Bound.BoundEltFacets[i][j][k]);
	    fprintf (file, "%d,\n", Bound.BoundElts[i][j][k]);
	  }
	  fprintf (file, "*Surface, type=ELEMENT, name=%s\n",
		   Bound.BoundNames[i][j]);

	  for (k = 1; k <= Bound.BoundEltQty[i]; k++)
	    fprintf (file, "elt%d-S%d, S%d\n", Bound.BoundElts[i][j][k],
		     Bound.BoundEltFacets[i][j][k],
		     Bound.BoundEltFacets[i][j][k]);
	}

  return;
}

void
neut_mesh_fprintf_inp_fasets (FILE *file, struct TESS Tess,
			      struct MESH Mesh2D, struct MESH Mesh3D,
			      struct NODES Nodes, char *fasetlist)
{
  struct BOUNDARY Bound;

  if (!strcmp (fasetlist, "internal"))
  {
    ut_print_message (1, 4, "Skipped `-faset internal'.\n");
    return;
  }

  neut_boundary_set_zero (&Bound);

  neut_mesh_fasets_bound (Tess, Nodes, Mesh2D, Mesh3D, fasetlist, &Bound);

  neut_mesh_fprintf_inp_boundary (file, Bound);

  neut_boundary_free (&Bound);

  return;
}
