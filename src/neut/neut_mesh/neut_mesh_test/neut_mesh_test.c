/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_test_.h"

int
neut_mesh_test (struct MESH Mesh)
{
  int i, j, node, elt, eltnodeqty;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  if (!Mesh.NodeElts)
  {
    printf ("neut_mesh_test needs NodeElts!\n");
    ut_error_reportbug ();
  }

  for (i = 1; i <= Mesh.EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
    {
      node = Mesh.EltNodes[i][j];
      if (ut_array_1d_int_eltpos (Mesh.NodeElts[node] + 1,
				  Mesh.NodeElts[node][0], i) == -1)
	return -1;
    }

  for (i = 1; i <= Mesh.NodeQty; i++)
    for (j = 1; j <= Mesh.NodeElts[i][0]; j++)
    {
      elt = Mesh.NodeElts[i][j];
      if (ut_array_1d_int_eltpos (Mesh.EltNodes[elt], eltnodeqty, i) == -1)
	return -1;
    }

  return 0;
}
