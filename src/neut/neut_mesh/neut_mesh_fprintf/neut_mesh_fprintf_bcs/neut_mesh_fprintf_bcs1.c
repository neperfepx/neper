/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_bcs_.h"

void
neut_mesh_fprintf_bcs (FILE * file, struct NSET NSet0D,
                       struct NSET NSet1D, struct NSET NSet2D,
                       char *nset)
{
  int i, j, k, status;
  char **name;
  int qty;

  ut_list_break (nset, NEUT_SEP_NODEP, &name, &qty);

  /* first pass, for calculating total number of bcs */
  int bcqty = 0;
  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
        bcqty += NSet0D.NodeQty[j];
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
        bcqty += NSet1D.NodeQty[j];
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
        bcqty += NSet2D.NodeQty[j];
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
        for (k = 0; k < NSet0D.NodeQty[j]; k++)
          fprintf (file, "%d %s\n", NSet0D.nodes[j][k], NSet0D.names[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
        for (k = 0; k < NSet1D.NodeQty[j]; k++)
          fprintf (file, "%d %s\n", NSet1D.nodes[j][k], NSet1D.names[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
        for (k = 0; k < NSet2D.NodeQty[j]; k++)
          fprintf (file, "%d %s\n", NSet2D.nodes[j][k], NSet2D.names[j]);
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
