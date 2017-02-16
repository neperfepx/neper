/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport_.h"

void
nem_transport (struct IN_M In, struct TESS Tess, struct NODES RNodes, struct
	       MESH *RMesh, struct NODES Nodes, struct MESH *Mesh)
{
  int i, transportqty, valqty;
  char **transport = NULL, **val = NULL;
  int *oldelt = NULL;
  int dim = Tess.Dim;

  ut_print_message (0, 2, "Transporting data...\n");
  ut_string_separate (In.transportstring, NEUT_SEP_NODEP,
		      &transport, &transportqty);

  for (i = 0; i < transportqty; i++)
  {
    ut_string_separate (transport[i], NEUT_SEP_DEP, &val, &valqty);

    if (valqty != 3)
      ut_print_message (2, 3, "Bad format for transport expression `%s'.\n",
			transport[i]);

    ut_print_message (0, 3, "Transporting `%s'...\n", val[2]);

    if (!strcmp (val[0], "elt"))
      nem_transport_elt (val[1], val[2], RNodes, RMesh[dim], Nodes, Mesh[dim], &oldelt);
    else if (!strcmp (val[0], "node"))
      nem_transport_node (val[1], val[2], RNodes, RMesh[dim], Nodes);
    else
      ut_print_message (2, 3, "Unknown type of data in `%s' (must be `elt')\n", transport[i]);

    ut_free_2d_char (val, valqty);
    val = NULL;
  }

  ut_free_1d_int (oldelt);
  ut_free_2d_char (transport, transportqty);

  return;
}
