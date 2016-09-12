/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_.h"

void
net_transform (struct IN_T In, struct TESR *pTesr)
{
  int i, partqty;
  char **parts = NULL;
  double *val = ut_alloc_1d (3);

  ut_string_separate (In.transform, NEUT_SEP_NODEP, &parts, &partqty);

  for (i = 0; i < partqty; i++)
  {
    if (!strcmp (parts[i], "autocrop"))
    {
      ut_print_message (0, 2, "Autocropping...\n");
      neut_tesr_autocrop (pTesr);
    }
    else if (!strncmp (parts[i], "rasterscale", 11))
    {
      ut_print_message (0, 2, "Scaling raster...\n");
      sscanf (parts[i], "rasterscale(%lf,%lf,%lf)", val, val + 1, val + 2);
      neut_tesr_rasterscale (pTesr, val[0], val[1], val[2]);
    }
    else if (!strncmp (parts[i], "rmsat", 5))
    {
      ut_print_message (0, 2, "Removing satellites...\n");
      neut_tesr_rmsat (pTesr, parts[i], 3);
    }
    else if (!strncmp (parts[i], "grow", 4))
    {
      ut_print_message (0, 2, "Applying growth...\n");
      neut_tesr_grow (pTesr, parts[i], 3);
    }
    else if (!strncmp (parts[i], "tessinter", 9))
    {
      ut_print_message (0, 2, "Intersecting with tessellation...\n");
      neut_tesr_tessinter (pTesr, parts[i], 3);
    }
    else
      ut_print_message (2, 3, "Expression `%s' could not be processed.\n",
			parts[i]);
  }

  return;
}
