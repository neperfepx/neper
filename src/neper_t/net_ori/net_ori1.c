/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_.h"

void
net_ori_pre (struct IN_T In, struct MTESS MTess, struct TESS *Tess,
             struct SEEDSET **pSSet, int *pSSetQty)
{
  int i, cellqty;

  (*pSSet) = calloc (2, sizeof (struct SEEDSET));
  for (i = 0; i < 2; i++)
    neut_seedset_set_zero (*pSSet + i);
  sscanf (In.n[1], "%d", &cellqty);
  net_multiscale_arg_0d_int_fscanf (In.n[1], 0, 0, NULL, NULL,
                                    &(*pSSet)[1].N);
  (*pSSet)[1].Nall = (*pSSet)[1].N;
  net_ori_mtess_id (In, MTess, Tess, 0, 1, *pSSet + 1);
  net_ori_mtess_randseed (MTess, Tess, 0, 1, (*pSSet), cellqty, *pSSet + 1);

  (*pSSetQty) = 1;

  return;
}

void
net_ori (struct IN_T In, int level, struct MTESS MTess, struct TESS *Tess,
         struct SEEDSET *SSet, int dtess, int dcell, struct SEEDSET *pSSet,
         int verbositylevel)
{
  int status;
  char *ori = NULL;
  char *oridistrib = NULL;
  char *crysym = NULL;
  struct OL_SET OSet;

  net_ori_mtess_params (In, level, MTess, Tess, dtess, dcell, &ori, &oridistrib, &crysym);

  OSet = ol_set_alloc ((*pSSet).N, crysym);

  if (!strcmp (ori, "random"))
    net_ori_random ((*pSSet).Random, &OSet);

  else if (!strcmp (ori, "uniform"))
    net_ori_uniform (In, level, MTess, Tess, dtess, dcell, (*pSSet).Random,
                     &OSet, verbositylevel);

  else if (!strcmp (ori, "equal"))
    net_ori_equal (SSet, dtess, dcell, &OSet);

  else if (!strncmp (ori, "spread(", 7))
    net_ori_spread (ori, SSet, dtess, dcell, &OSet);

  else if (!strncmp (ori, "fibre", 5) || !strncmp (ori, "fiber", 5))
    net_ori_fiber ((*pSSet).Random, ori, &OSet);

  else if (!strncmp (ori, "file(", 5))
    net_ori_file (ori, &OSet);

  else
  {
    status = net_ori_label (ori, &OSet);

    if (status)
      abort ();
  }

  net_ori_crysym (&OSet);

  net_ori_memcpy (OSet, pSSet);

  net_oridistrib (oridistrib, pSSet);

  ol_set_free (OSet);
  ut_free_1d_char (&ori);
  ut_free_1d_char (&oridistrib);
  ut_free_1d_char (&crysym);

  return;
}

void
net_ori_post (struct TESR *pTesr)
{
  int i, j;

  (*pTesr).VoxOri = ut_alloc_4d ((*pTesr).size[0] + 1,
                                 (*pTesr).size[1] + 1,
                                 (*pTesr).size[2] + 1, 4);

  for (i = 1; i <= (*pTesr).CellQty; i++)
  {
    int voxqty, **voxs = NULL;
    double **q = NULL;

    neut_tesr_cell_voxs (*pTesr, i, &voxs, &voxqty);

    q = ut_alloc_2d (voxqty, 4);
    neut_ori_oridistrib ((*pTesr).CellOri[i], (*pTesr).CellOriDistrib[i], voxqty, i, q);

    for (j = 0; j < voxqty; j++)
      ol_q_memcpy (q[j], (*pTesr).VoxOri[voxs[j][0]][voxs[j][1]][voxs[j][2]]);

    ut_free_2d_int (&voxs, voxqty);
    ut_free_2d (&q, voxqty);
  }

  return;
}
