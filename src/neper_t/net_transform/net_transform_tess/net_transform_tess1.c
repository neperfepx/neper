/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_.h"

void
net_transform_tess (struct IN_T In, struct TESS *pDom, struct TESS *pTess)
{
  int i, PartQty, status;
  char **parts = NULL;
  double theta, *tmp = ol_r_alloc ();
  double **g = ol_g_alloc ();

  ut_list_break (In.transform, NEUT_SEP_NODEP, &parts, &PartQty);

  for (i = 0; i < PartQty; i++)
  {
    if (!strncmp (parts[i], "scale", 5))
    {
      ut_print_message (0, 2, "Scaling...\n");
      status = sscanf (parts[i], "scale(%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2);
      if (status == 2)
        tmp[2] = 1;
      neut_tess_scale (pTess, tmp[0], tmp[1], tmp[2]);
    }

    else if (!strncmp (parts[i], "rotate", 6))
    {
      ut_print_message (0, 2, "Rotating...\n");
      sscanf (parts[i], "rotate(%lf,%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2,
              &theta);
      ol_r_set_unit (tmp);
      ol_rtheta_g (tmp, theta, g);
      neut_tess_rotate (pTess, g);
    }

    else if (!strncmp (parts[i], "translate", 9))
    {
      ut_print_message (0, 2, "Translating...\n");
      status =
        sscanf (parts[i], "translate(%lf,%lf,%lf)", tmp, tmp + 1, tmp + 2);
      if (status == 2)
        tmp[2] = 1;
      neut_tess_shift (pTess, tmp[0], tmp[1], tmp[2]);
    }

    else if (!strncmp (parts[i], "cut", 3))
    {
      ut_print_message (0, 2, "Cutting (experimental)...\n");
      net_transform_tess_cut (parts[i], *pDom, pTess);
    }

    else if (!strncmp (parts[i], "mergecell", 9))
    {
      ut_print_message (0, 2, "Merging cells...\n");

      int j, exprqty, qty;
      char *fct = NULL, **exprs = NULL;

      ut_string_function_expr (parts[i], &fct, &exprs, &exprqty);

      for (j = 0; j < exprqty; j++)
      {
        qty = neut_tess_cellexpr_merge (pTess, exprs[j], 1);
        ut_print_message (0, 3, "Merged %d cells...\n", qty);
      }

      ut_free_1d_char (&fct);
      ut_free_2d_char (&exprs, exprqty);
    }

    else if (!strncmp (parts[i], "rmcell", 6))
    {
      ut_print_message (0, 2, "Removing cells...\n");

      int j, exprqty, qty;
      char *fct = NULL, **exprs = NULL;

      ut_string_function_expr (parts[i], &fct, &exprs, &exprqty);

      for (j = 0; j < exprqty; j++)
      {
        qty = neut_tess_cellexpr_remove (pTess, exprs[j]);
        ut_print_message (0, 3, "Removed %d cells...\n", qty);
      }

      ut_free_1d_char (&fct);
      ut_free_2d_char (&exprs, exprqty);
    }

    else if (!strcmp (parts[i], "resetcellid"))
    {
      ut_print_message (0, 2, "Resetting cell ids...\n");
      neut_tess_resetcellid (pTess);
    }

    else if (!strncmp (parts[i], "planecut(", 9))
    {
      ut_print_message (0, 2, "Cutting by plane...\n");
      net_tess_clip_expr (pTess, parts[i]);
    }

    else if (!strncmp (parts[i], "crop(", 5))
    {
      ut_print_message (0, 2, "Cropping...\n");
      net_tess_crop_expr (pTess, parts[i]);
    }

    else if (!strncmp (parts[i], "slice(", 6))
    {
      int j, exprqty, domface;
      double *eq = ut_alloc_1d (4);
      char *fct = NULL, **exprs = NULL;
      struct TESS Tess2;

      neut_tess_set_zero (&Tess2);

      ut_print_message (0, 2, "Slicing...\n");

      if (!strcmp ((*pTess).Type, "periodic"))
        ut_print_message (2, 3,
                          "Not available for periodic tessellations.\n");

      ut_string_function_expr (parts[i], &fct, &exprs, &exprqty);
      if (exprqty != 4)
        ut_print_message (2, 2, "Failed to parse expression `%s'.\n",
                          parts[i]);

      for (j = 0; j < 4; j++)
        sscanf (exprs[j], "%lf", eq + j);

      ut_array_1d_scale (eq, 4, 1. / ut_array_1d_norm (eq + 1, 3));
      domface = net_tess_clip (pTess, eq);
      neut_tess_tess (*pTess, &Tess2);
      neut_tess_domface_tess (Tess2, domface, pTess);

      ut_free_1d (&eq);
      ut_free_1d_char (&fct);
      ut_free_2d_char (&exprs, exprqty);
      neut_tess_free (&Tess2);
    }

    else if (!strcmp (parts[i], "resetlabels"))
    {
      ut_print_message (0, 2, "Resetting labels...\n");
      neut_tess_init_domain_label (pTess);
    }

    else if (!strncmp (parts[i], "ori", 3))
    {
      ut_print_message (0, 3, "Overriding orientations...\n");
      net_transform_tess_ori (parts[i], pTess);
    }

    else
      ut_print_message (1, 3, "Skipping `%s'...\n", parts[i]);
  }

  ol_g_free (g);
  ol_r_free (tmp);
  ut_free_2d_char (&parts, PartQty);

  return;
}
