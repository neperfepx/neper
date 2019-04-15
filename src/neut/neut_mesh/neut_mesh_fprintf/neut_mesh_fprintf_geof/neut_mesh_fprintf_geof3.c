/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_geof_.h"

void
neut_mesh_fprintf_geof_nset (FILE * file, char *name, int nodeqty, int *nset)
{
  int i, Col;

  fprintf (file, "\n**nset %s\n", name);

  Col = 0;
  for (i = 0; i < nodeqty; i++)
    ut_print_wnc_int (file, nset[i], &Col, 72);

  fprintf (file, "\n");

  return;
}

void
neut_mesh_fprintf_geof_part_nset (FILE * file, struct PART Part)
{
  int p, i, Col, nodeqty;
  int *nodes = ut_alloc_1d_int (Part.node_parts[0]);

  for (p = 0; p < Part.qty; p++)
  {
    nodeqty = 0;
    for (i = 1; i <= Part.node_parts[0]; i++)
      if (Part.node_parts[i] == p)
	nodes[nodeqty++] = i;

    fprintf (file, "\n**nset part%d\n", p + 1);
    Col = 0;
    for (i = 0; i < nodeqty; i++)
      ut_print_wnc_int (file, nodes[i], &Col, 72);

    fprintf (file, "\n");
  }

  ut_free_1d_int (nodes);

  return;
}

void
neut_mesh_fprintf_geof_part_elset (FILE * file, struct PART Part)
{
  int p, i, Col, eltqty;
  int *elts = ut_alloc_1d_int (Part.elt_parts[0]);

  for (p = 0; p < Part.qty; p++)
  {
    eltqty = 0;
    for (i = 1; i <= Part.elt_parts[0]; i++)
      if (Part.elt_parts[i] == p)
	elts[eltqty++] = i;

    fprintf (file, "\n**elset part%d\n", p + 1);
    Col = 0;
    for (i = 0; i < eltqty; i++)
      ut_print_wnc_int (file, elts[i], &Col, 72);

    fprintf (file, "\n");
  }

  return;
}
