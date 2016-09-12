/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport_.h"

void
nem_transport (struct IN_M In, struct TESS Tess, struct NODES RNodes, struct
	       MESH *RMesh, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, dim, transportqty, valqty;
  char **transport = NULL, **val = NULL;
  int *oldelt = ut_alloc_1d_int (Mesh[3].EltQty + 1);
  FILE *file = NULL;
  char *name = NULL;

  ut_print_message (0, 2, "Transporting element data...\n");
  ut_string_separate (In.transportstring, NEUT_SEP_NODEP,
		      &transport, &transportqty);

  nem_transport_mesh3d_parelt (Tess, RNodes, RMesh, *pNodes, Mesh, oldelt);

  for (i = 0; i < transportqty; i++)
  {
    ut_string_separate (transport[i], NEUT_SEP_DEP, &val, &valqty);

    if (valqty != 3)
      ut_print_message (2, 3, "Bad format for transport expression `%s'.\n",
			transport[i]);

    ut_print_message (0, 3, "Transporting `%s'...\n", val[2]);

    if (strcmp (val[0], "elt") != 0)
      ut_print_message (2, 3,
			"Unknown type of data in `%s' (must be `elt')\n",
			transport[i]);

    if (sscanf (val[1], "real%d", &dim) == 1)
    {
      double **data = ut_alloc_2d (RMesh[3].EltQty + 1, dim);

      file = ut_file_open (val[2], "r");
      for (j = 1; j <= RMesh[3].EltQty; j++)
	ut_array_1d_fscanf (file, data[j], dim);
      ut_file_close (file, val[2], "r");

      name = ut_alloc_1d_char (strlen (val[2]) + 5);
      sprintf (name, "%s.rem", val[2]);

      file = ut_file_open (name, "w");
      for (j = 1; j <= Mesh[3].EltQty; j++)
	ut_array_1d_fprintf (file, data[oldelt[j]], dim, "%11.6f");
      ut_file_close (file, name, "w");
      ut_free_1d_char (name);

      ut_free_2d (data, RMesh[3].EltQty + 1);
    }
    else if (sscanf (val[1], "int%d", &dim) == 1)
    {
      int **data = ut_alloc_2d_int (RMesh[3].EltQty + 1, dim);

      file = ut_file_open (val[2], "r");
      for (j = 1; j <= RMesh[3].EltQty; j++)
	ut_array_1d_int_fscanf (file, data[j], dim);
      ut_file_close (file, val[2], "r");

      name = ut_alloc_1d_char (strlen (val[2]) + 5);
      sprintf (name, "%s.rem", val[2]);

      file = ut_file_open (name, "w");
      for (j = 1; j <= Mesh[3].EltQty; j++)
	ut_array_1d_int_fprintf (file, data[oldelt[j]], dim, "%11.6f");
      ut_file_close (file, name, "w");
      ut_free_1d_char (name);

      ut_free_2d_int (data, RMesh[3].EltQty + 1);
    }

    ut_free_2d_char (val, valqty);
    val = NULL;
  }

  ut_free_1d_int (oldelt);
  ut_free_2d_char (transport, transportqty);

  return;
}
