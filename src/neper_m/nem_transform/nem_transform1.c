/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transform_.h"

void
nem_transform (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
               struct MESH *Mesh)

{
  int i, partqty, status;
  double *tmp = ut_alloc_1d (3);
  char **parts = NULL;
  (void) Mesh;

  ut_string_separate (In.transform, NEUT_SEP_NODEP, &parts, &partqty);

  for (i = 0; i < partqty; i++)
  {
    if (!strncmp (parts[i], "scale", 5))
    {
      ut_print_message (0, 3, "Scaling...\n");
      status = sscanf (parts[i], "scale(%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2);
      if (status == 2)
        tmp[2] = 1;
      neut_nodes_scale (pNodes, tmp[0], tmp[1], tmp[2]);
    }

    else if (!strncmp (parts[i], "translate", 5))
    {
      ut_print_message (0, 3, "Translating...\n");
      status = sscanf (parts[i], "translate(%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2);
      if (status == 2)
        tmp[2] = 1;
      neut_nodes_shift (pNodes, tmp[0], tmp[1], tmp[2]);
    }

    else if (!strncmp (parts[i], "smooth", 5))
    {
      ut_print_message (0, 3, "Smoothing...\n");
      nem_transform_smooth (parts[i], Tess, pNodes, Mesh);
    }

    else
      ut_print_message (1, 3, "Skipping `%s'...\n", parts[i]);
  }

  ut_free_1d (tmp);

  return;
}
