/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_.h"

void
net_transform_tess (struct IN_T In, struct TESS *pTess)
{
  int i, partqty, status;
  char **parts = NULL;
  double theta, *tmp = ol_r_alloc ();
  double **g = ol_g_alloc ();

  ut_string_separate (In.transform, NEUT_SEP_NODEP, &parts, &partqty);

  for (i = 0; i < partqty; i++)
  {
    if (!strncmp (parts[i], "rotate", 6))
    {
      ut_print_message (0, 2, "Rotating...\n");
      sscanf (parts[i], "rotate(%lf,%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2, &theta);
      ol_rtheta_g (tmp, theta, g);
      neut_tess_rotate (pTess, g);
    }
    else if (!strncmp (parts[i], "scale", 5))
    {
      ut_print_message (0, 2, "Scaling...\n");
      status = sscanf (parts[i], "scale(%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2);
      if (status == 2)
	tmp[2] = 1;
      neut_tess_scale (pTess, tmp[0], tmp[1], tmp[2]);
    }
  }

  ol_g_free (g);
  ol_r_free (tmp);
  ut_free_2d_char (parts, partqty);

  return;
}

void
net_transform_tesr (struct IN_T In, struct TESR *pTesr)
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
    else if (!strncmp (parts[i], "crop:", 5))
    {
      ut_print_message (0, 2, "Cropping by %s...\n", parts[i] + 5);
      neut_tesr_crop (pTesr, parts[i] + 5);
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
    else if (!strcmp (parts[i], "2d"))
    {
      ut_print_message (0, 2, "Making tessellation 2D...\n");
      neut_tesr_2d (pTesr);
    }
    else
      ut_print_message (2, 3, "Expression `%s' could not be processed.\n",
			parts[i]);
  }

  ut_free_2d_char (parts, partqty);

  return;
}
