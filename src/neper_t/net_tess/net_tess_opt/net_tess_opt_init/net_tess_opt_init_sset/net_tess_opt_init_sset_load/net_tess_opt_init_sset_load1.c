/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_load_.h"

void
net_tess_opt_init_sset_load (struct IN_T In, int level, struct SEEDSET SSet,
			     struct TOPT *pTOpt)
{
  int i, cell, seedQty;
  FILE *fp = NULL;

  fp = ut_file_open (In.morphooptimultiseed[level], "r");

  ut_file_skip_line (fp, 1);

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    if (fscanf (fp, "%d %d", &cell, &seedQty) != 2)
      ut_print_message (2, 0, "Failed to load seedset!\n");

    net_tess_opt_init_sset_load_cell (pTOpt, SSet, cell, seedQty, fp);
  }

  ut_file_close (fp, In.morphooptimultiseed[level], "r");

  ut_string_string ("custom", &((*pTOpt).SSet).weight);

  return;
}

void
net_seed_custom (char *filename, int level, int cell, int dim,
		 struct TESS Dom, struct SEEDSET *pSSet)
{
  int i, qty, weight;
  double coo;
  FILE *file = NULL;
  double **tmp = NULL;
  char flag[100];
  char **parts = NULL;
  int partqty;
  char *filename2 = NULL;

  (*pSSet).Size = ut_alloc_2d (3, 2);
  neut_tess_bbox (Dom, (*pSSet).Size);

  (*pSSet).SeedCoo = ut_alloc_2d ((*pSSet).N + 1, 3);
  (*pSSet).SeedWeight = ut_alloc_1d ((*pSSet).N + 1);
  ut_string_string ("none", &((*pSSet).weight));

  ut_string_separate (filename, NEUT_SEP_DEP, &parts, &partqty);
  ut_string_string (parts[0], &filename2);
  if (partqty == 2)
    sscanf (parts[1], "%d", &dim);

  qty = ut_num_d2ri (ut_file_nbwords (filename2) / (*pSSet).N);
  weight = qty - dim;
  if (weight != 0 && weight != 1)
    ut_print_message (2, 0, "Seed position/weight file format unknown.\n");

  file = ut_file_open (filename2, "R");

  tmp = ut_alloc_2d ((*pSSet).N, qty);

  if (level == 1)
    ut_array_2d_fscanf (file, tmp, (*pSSet).N, qty);
  else
  {
    sprintf (flag, "%d", cell);
    ut_array_2d_fscanf_filter_prefix (file, tmp, (*pSSet).N, qty, flag);
  }

  ut_array_2d_memcpy ((*pSSet).SeedCoo + 1, (*pSSet).N, dim, tmp);

  if (weight)
  {
    ut_string_string ("custom", &((*pSSet).weight));
    for (i = 1; i <= (*pSSet).N; i++)
      (*pSSet).SeedWeight[i] = tmp[i - 1][qty - 1];
  }

  ut_file_close (file, filename2, "R");

  if (dim == 2)
  {
    coo = .5 * ((*pSSet).Size[2][0] + (*pSSet).Size[2][1]);
    for (i = 1; i <= (*pSSet).N; i++)
      (*pSSet).SeedCoo[i][2] = coo;
  }

  ut_free_2d (tmp, (*pSSet).N);
  ut_free_2d_char (parts, partqty);

  return;
}
