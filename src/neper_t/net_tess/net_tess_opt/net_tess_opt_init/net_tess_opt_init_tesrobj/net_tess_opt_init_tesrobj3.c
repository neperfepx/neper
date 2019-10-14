/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_tesrobj_.h"

void
net_tess_opt_init_tesrobj_res_size (char *sample, struct TOPT TOpt,
				     int *size, double *vsize)
{
  int i, status;
  double val, avdiameq, fact;
  struct TESR Tesr = TOpt.tartesr;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (TOpt.Dom, bbox);

  status = sscanf (sample, "%lf", &val);
  if (status != 1)
    ut_print_message (2, 4, "Failed to read `%s'.\n", sample);

  neut_tess_cellavdiameq (TOpt.Dom, TOpt.CellQty, &avdiameq);

  ut_array_1d_set (vsize, 3, 0);
  ut_array_1d_int_set (size, 3, 1);
  for (i = 0; i < Tesr.Dim; i++)
    vsize[i] = avdiameq / val;

  for (i = 0; i < Tesr.Dim; i++)
  {
    size[i] = ceil ((bbox[i][1] - bbox[i][0]) / vsize[i]);
    fact = vsize[i] / Tesr.vsize[i];

    if (fact > 1)
      ut_print_message (0, 4, "Sampling factor along %c = %f.\n",
	  'x' + i, fact);
    else
    {
      ut_print_message (1, 4, "Sampling factor along %c = %f.  Setting to 1...\n",
	  'x' + i, fact);
      size[i] = Tesr.size[i];
      vsize[i] = Tesr.vsize[i];
    }
  }

  ut_free_2d (bbox, 3);

  return;
}

void
net_tess_opt_init_tesrobj_res_cell (int *size, double *vsize, int cell, struct TOPT *pTOpt)
{
  int i, j, k, l, voxpt, newpt, pos[3];
  struct TESR Tesr = (*pTOpt).tartesr;
  int newptqty;
  double **newpts = NULL;
  double *newptweights = NULL;
  int ***voxptqty = ut_alloc_3d_int (size[0] + 1, size[1] + 1, size[2] + 1);
  int ****voxpts = ut_alloc_3d_pint (size[0] + 1, size[1] + 1, size[2] + 1);

  // mapping input control points onto grid
  for (i = 0; i < (*pTOpt).tarcellptqty[cell]; i++)
  {
    ut_array_1d_int_set (pos, 3, 1);
    for (j = 0; j < Tesr.Dim; j++)
    {
      pos[j] = ceil (((*pTOpt).tarcellpts[cell][i][j] - (*pTOpt).tartesr.Origin[j]) / vsize[j] + 1e-6);
      pos[j] = ut_num_min_int (pos[j], size[j]);
    }

    ut_array_1d_int_list_addelt_nocheck (&(voxpts[pos[0]][pos[1]][pos[2]]),
					 &(voxptqty[pos[0]][pos[1]][pos[2]]), i);
  }

  // merging points by grid voxel
  newptqty = 0;
  for (k = 1; k <= size[2]; k++)
    for (j = 1; j <= size[1]; j++)
      for (i = 1; i <= size[0]; i++)
	if (voxptqty[i][j][k] > 0)
	{
	  // averaging positions
	  newptqty++;
	  newpts = ut_realloc_2d_addline (newpts, newptqty, 3);
	  newptweights = ut_realloc_1d (newptweights, newptqty);
	  newpt = newptqty - 1;
	  for (l = 0; l < voxptqty[i][j][k]; l++)
	  {
	    voxpt = voxpts[i][j][k][l];
	    ut_array_1d_add (newpts[newpt], (*pTOpt).tarcellpts[cell][voxpt],
			     3, newpts[newpt]);

	  }
	  ut_array_1d_scale (newpts[newpt], 3, 1. / voxptqty[i][j][k]);
	  newptweights[newpt] = voxptqty[i][j][k];
	}

  (*pTOpt).tarcellfact[cell] = 1;

  ut_free_2d ((*pTOpt).tarcellpts[cell], (*pTOpt).tarcellptqty[cell]);
  ut_free_1d ((*pTOpt).tarcellptweights[cell]);
  (*pTOpt).tarcellptqty[cell] = newptqty;
  (*pTOpt).tarcellpts[cell] = ut_alloc_2d (newptqty, 3);
  ut_array_2d_memcpy ((*pTOpt).tarcellpts[cell], newptqty, 3, newpts);
  (*pTOpt).tarcellptweights[cell] = ut_alloc_1d (newptqty);
  ut_array_1d_memcpy ((*pTOpt).tarcellptweights[cell], newptqty, newptweights);

  ut_free_2d (newpts, newptqty);
  ut_free_1d (newptweights);
  ut_free_3d_int (voxptqty, size[0] + 1, size[1] + 1);
  ut_free_4d_int (voxpts , size[0] + 1, size[1] + 1, size[2] + 1);

  return;
}
