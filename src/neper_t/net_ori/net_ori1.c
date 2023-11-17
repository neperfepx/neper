/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
  char *ori = NULL, *orisampling = NULL, *orispread = NULL, *oricrysym = NULL;
  int i, j, partqty, *qty = NULL;
  char **parts = NULL;
  struct OL_SET OSet, *OSets = NULL;

  net_ori_mtess_params (In, level, MTess, Tess, dtess, dcell, &ori, &orisampling, &orispread, &oricrysym);

  OSet = ol_set_alloc ((*pSSet).N, oricrysym);

  ut_list_break (ori, "+", &parts, &partqty);

  net_ori_qty (pSSet, parts, partqty, &qty);

  OSets = malloc (partqty * sizeof (struct OL_SET));
  for (i = 0; i < partqty; i++)
    OSets[i] = ol_set_alloc (qty[i], oricrysym);

  for (i = 0; i < partqty; i++)
  {
    if (!strcmp (parts[i], "random") || strstr (In.orioptiini[level], "ori=random"))
    {
      if (!strcmp (In.orisampling[level], "random"))
        net_ori_random ((*pSSet).Random, OSets + i);

      else if (!strcmp (In.orisampling[level], "uniform"))
        net_ori_uniform (In, level, MTess, Tess, dtess, dcell, (*pSSet).Random,
                         OSets + i, verbositylevel);

      else
        ut_print_message (2, 3, "Failed to process `%s'.\n", In.orisampling[level]);
    }

    else if (!strncmp (parts[i], "fiber", 5))
      net_ori_fiber (SSet, dtess, dcell, (*pSSet).Random, parts[i], OSets + i);

    else if (!strncmp (ori, "file(", 5))
    {
      net_ori_file (ori, OSets + i);
      ut_string_string (oricrysym, &(OSets[i].crysym));
    }

    else if (strstr (In.orioptiini[level], "ori="))
    {
      int qty, *qty1 = NULL;
      char ***parts = NULL;
      ut_list_break2 (In.orioptiini[level], ",", "=", &parts, &qty1, &qty);
      for (j = 0; j < qty; j++)
        if (!strcmp (parts[j][0], "ori"))
          net_ori_file (parts[j][1], OSets + i);
      ut_string_string (oricrysym, &(OSets[i].crysym));

      ut_free_3d_char (&parts, qty, 2);
      ut_free_1d_int (&qty1);
    }

    else if (!strncmp (parts[i], "odf", 3))
      net_ori_odf ((*pSSet).Random, parts[i], OSets + i);

    else
      net_ori_label (parts[i], SSet, dtess, dcell, OSets + i);

    if (strstr (In.orioptiini[level], "weight="))
    {
      int qty, *qty1 = NULL;
      char ***parts = NULL;
      ut_list_break2 (In.orioptiini[level], ",", "=", &parts, &qty1, &qty);

      OSets[i].weight = ut_alloc_1d (OSets[i].size);
      for (j = 0; j < qty; j++)
        if (!strcmp (parts[j][0], "weight"))
          ut_array_1d_fnscanf_wcard (parts[j][1], OSets[i].weight, OSets[i].size, "numeral", "r");
    }

    if (strstr (In.orioptiini[level], "theta="))
    {
      int qty, *qty1 = NULL;
      char ***parts = NULL;
      ut_list_break2 (In.orioptiini[level], ",", "=", &parts, &qty1, &qty);

      OSets[i].theta = ut_alloc_1d (OSets[i].size);
      for (j = 0; j < qty; j++)
        if (!strcmp (parts[j][0], "theta"))
        {
          ut_array_1d_fnscanf_wcard (parts[j][1], OSets[i].theta, OSets[i].size, "numeral", "r");
          ut_array_1d_scale (OSets[i].theta, OSets[i].size, M_PI / 180);
        }
    }
  }

  ol_set_cat (OSets, partqty, &OSet);

  if (partqty > 1)
      ol_set_shuf (&OSet, (*pSSet).Random);

  for (i = 0; i < partqty; i++)
    ol_set_free (OSets + i);
  free (OSets);

  net_ori_oricrysym (&OSet);

  net_ori_memcpy (OSet, pSSet);

  // processing -orispread
  net_ori_orispread (orispread, pSSet);

  ut_free_1d_int (&qty);
  ut_free_2d_char (&parts, partqty);
  ut_free_1d_char (&ori);
  ut_free_1d_char (&orisampling);
  ut_free_1d_char (&orispread);
  ut_free_1d_char (&oricrysym);
  ol_set_free (&OSet);

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
    neut_ori_orispread ((*pTesr).CellOri[i], (*pTesr).CellOriDistrib[i], voxqty, i, q);

    for (j = 0; j < voxqty; j++)
      ol_q_memcpy (q[j], (*pTesr).VoxOri[voxs[j][0]][voxs[j][1]][voxs[j][2]]);

    ut_free_2d_int (&voxs, voxqty);
    ut_free_2d (&q, voxqty);
  }

  return;
}
