/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_tesrobj_.h"

void
net_tess_opt_init_tesrobj_pre (struct TOPT *pTOpt)
{
  (*pTOpt).tarcellpts = ut_alloc_1d_ppdouble ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellptweights =
    ut_alloc_1d_pdouble ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellptqty = ut_alloc_1d_int ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellfact = ut_alloc_1d ((*pTOpt).tartesr.CellQty + 1);
  ut_array_1d_set ((*pTOpt).tarcellfact + 1, (*pTOpt).tartesr.CellQty, 1);

  return;
}

void
net_tess_opt_init_tesrobj_scale (struct TOPT *pTOpt)
{
  int i;

  (*pTOpt).tartesrscale = ut_alloc_1d ((*pTOpt).Dim);
  neut_tesr_cells_anisoxyz ((*pTOpt).tartesr, (*pTOpt).tartesrscale);
  for (i = 0; i < (*pTOpt).Dim; i++)
    (*pTOpt).tartesrscale[i] = 1. / (*pTOpt).tartesrscale[i];
  ut_print_message (0, 4, "Scaling input tesr by %.2f x %.2f x %.2f...\n",
                    (*pTOpt).tartesrscale[0], (*pTOpt).tartesrscale[1],
                    (*pTOpt).tartesrscale[2]);
  neut_tesr_scale (&((*pTOpt).tartesr), (*pTOpt).tartesrscale[0],
                   (*pTOpt).tartesrscale[1], (*pTOpt).tartesrscale[2]);

  return;
}

void
net_tess_opt_init_tesrobj_rasterscale (struct TOPT *pTOpt)
{
  double vsizemax = ut_array_1d_max ((*pTOpt).tartesr.vsize, 3);

  ut_print_message (0, 4,
                    "Scaling input tesr's raster by %.2f x %.2f x %.2f...\n",
                    (*pTOpt).tartesr.vsize[0] / vsizemax,
                    (*pTOpt).tartesr.vsize[1] / vsizemax,
                    (*pTOpt).tartesr.vsize[2] / vsizemax);
  neut_tesr_rasterscale (&((*pTOpt).tartesr),
                         (*pTOpt).tartesr.vsize[0] / vsizemax,
                         (*pTOpt).tartesr.vsize[1] / vsizemax,
                         (*pTOpt).tartesr.vsize[2] / vsizemax);

  return;
}

void
net_tess_opt_init_tesrobj_region (char *region, struct TOPT *pTOpt)
{
  int i, j, **pts = NULL;
  char *message = ut_alloc_1d_char (100);

  ut_print_message (0, 4, "Listing cell voxels... ");

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    ut_print_progress (stdout, i, (*pTOpt).CellQty, "%5.1f%%", message);

    if (!strcmp (region, "surf"))
      neut_tesr_cell_boundpoints ((*pTOpt).tartesr, i, &pts,
                                  &(*pTOpt).tarcellptqty[i], 0, "xyz");
    else if (!strcmp (region, "vol") || !strcmp (region, "all"))
      neut_tesr_cell_points ((*pTOpt).tartesr, i, &pts,
                             &(*pTOpt).tarcellptqty[i]);
    else
      ut_print_message (2, 3, "Could not process expression `%s'.\n", region);

    if ((*pTOpt).tarcellptqty[i] <= 0)
      abort ();

    (*pTOpt).tarcellpts[i] = ut_alloc_2d ((*pTOpt).tarcellptqty[i], 3);
    (*pTOpt).tarcellptweights[i] = ut_alloc_1d ((*pTOpt).tarcellptqty[i]);

    for (j = 0; j < (*pTOpt).tarcellptqty[i]; j++)
      neut_tesr_pos_coo ((*pTOpt).tartesr, pts[j], (*pTOpt).tarcellpts[i][j]);

    ut_free_2d_int (&pts, (*pTOpt).tarcellptqty[i]);
  }

  (*pTOpt).tarcellptqty[0] =
    ut_array_1d_int_sum ((*pTOpt).tarcellptqty + 1, (*pTOpt).tartesr.CellQty);
  (*pTOpt).tavoxqtyini = (*pTOpt).tarcellptqty[0];

  ut_free_1d_char (&message);

  return;
}

void
net_tess_opt_init_tesrobj_res (char *sample, struct TOPT *pTOpt)
{
  int i;
  int *size = ut_alloc_1d_int (3);
  double *vsize = ut_alloc_1d (3);

  net_tess_opt_init_tesrobj_res_size (sample, *pTOpt, size, vsize);

  for (i = 1; i <= (*pTOpt).CellQty; i++)
    net_tess_opt_init_tesrobj_res_cell (size, vsize, i, pTOpt);

  (*pTOpt).tarcellptqty[0] =
    ut_array_1d_int_sum ((*pTOpt).tarcellptqty + 1, (*pTOpt).CellQty);

  ut_free_1d_int (&size);
  ut_free_1d (&vsize);

  return;
}

void
net_tess_opt_init_tesrobj_post (struct TOPT *pTOpt)
{
  int i, j, oldqty = 0, newqty = 0;

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    oldqty += (*pTOpt).tarcellptqty[i] * (*pTOpt).tarcellfact[i];
    newqty += (*pTOpt).tarcellptqty[i];
  }

  // Printing reduction message
  if (newqty > oldqty)
    abort ();

  else if (newqty != oldqty)
    ut_print_message (0, 4,
                      "Number of interface voxels reduced by %7.3f%% (to %d).\n",
                      100.0 * (oldqty - newqty) / oldqty, newqty);
  ut_print_message (0, 4,
                    "Number of voxels per cell: min = %d, avg = %d, max = %d.\n",
                    ut_array_1d_int_min ((*pTOpt).tarcellptqty + 1,
                                         (*pTOpt).CellQty),
                    ut_num_d2ri (ut_array_1d_int_mean
                                 ((*pTOpt).tarcellptqty + 1,
                                  (*pTOpt).CellQty)),
                    ut_array_1d_int_max ((*pTOpt).tarcellptqty + 1,
                                         (*pTOpt).CellQty));

  // Setting tarcellptcells
  (*pTOpt).tarcellptscells = ut_alloc_1d_ppint ((*pTOpt).tartesr.CellQty + 1);
  for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
  {
    (*pTOpt).tarcellptscells[i] =
      ut_alloc_1d_pint ((*pTOpt).tarcellptqty[i] + 1);
    for (j = 0; j < (*pTOpt).tarcellptqty[i]; j++)
    {
      (*pTOpt).tarcellptscells[i][j] =
        ut_alloc_1d_int ((*pTOpt).CellSCellQty[i]);
      ut_array_1d_int_memcpy ((*pTOpt).CellSCellList[i],
                              (*pTOpt).CellSCellQty[i],
                              (*pTOpt).tarcellptscells[i][j]);
    }
  }

  // Setting tarcellptsdist
  (*pTOpt).tarcellptsdist =
    ut_alloc_1d_pdouble ((*pTOpt).tartesr.CellQty + 1);
  for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
    (*pTOpt).tarcellptsdist[i] = ut_alloc_1d ((*pTOpt).tarcellptqty[i] + 1);

  return;
}
