/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_per_.h"

void
neut_mesh_fprintf_per_plain (FILE * file, struct NODES Nodes)
{
  int i, id;

  for (i = 1; i <= Nodes.PerNodeQty; i++)
  {
    id = Nodes.PerNodeNb[i];

    fprintf (file, "%d %d ", id, Nodes.PerNodeMaster[id]);
    ut_array_1d_int_fprintf (file, Nodes.PerNodeShift[id], 3, "%d");
  }

  return;
}

void
neut_mesh_fprintf_per_geof (FILE * file, struct NODES Nodes)
{
  int i, j;

  for (i = 1; i <= Nodes.PerNodeQty; i++)
    for (j = 1; j <= 3; j++)
      fprintf (file, "**mpc2 %d U%d %d U%d\n", Nodes.PerNodeMaster[Nodes.PerNodeNb[i]], j, Nodes.PerNodeNb[i], j);

  return;
}
