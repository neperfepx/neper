/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_pre_.h"

void
net_tess_opt_init_sset_pre_randseed (struct MTESS MTess, struct TESS *Tess,
				     int domtess, int dompoly,
				     struct SEEDSET *SSet, int CellQty,
				     struct SEEDSET *pSSet)
{
  int i, tess;
  int levelqty = Tess[domtess].Level + 1;
  int *ns = ut_alloc_1d_int (levelqty + 1);
  int *ids = ut_alloc_1d_int (levelqty + 1);
  int *polys = ut_alloc_1d_int (levelqty + 1);
  int **doms = ut_alloc_2d_int (levelqty + 1, 2);

  neut_mtess_tess_doms (MTess, Tess[domtess], doms);
  ut_array_1d_int_set_2 (doms[levelqty], domtess, dompoly);

  for (i = 1; i < levelqty; i++)
  {
    polys[i] = doms[i][1];
    tess = MTess.DomTess[doms[i - 1][0]][doms[i - 1][1]];
    ids[i] = SSet[tess].Id;
    ns[i] = SSet[tess].N;
  }
  ids[levelqty] = (*pSSet).Id;
  ns[levelqty] = CellQty;
  polys[levelqty] = dompoly;

  (*pSSet).Random = net_tess_opt_init_sset_pre_randseed_rand (ns, ids, polys,
							      Tess[domtess].
							      Level + 1);

  ut_free_1d_int (ns);
  ut_free_1d_int (ids);
  ut_free_1d_int (polys);
  ut_free_2d_int (doms, levelqty + 1);

  return;
}

void
net_tess_opt_init_sset_pre_dim (struct TOPT TOpt, struct SEEDSET *pSSet)
{
  (*pSSet).Dim = TOpt.Dim;

  return;
}

void
net_tess_opt_init_sset_pre_type (struct SEEDSET *pSSet)
{
  ut_string_string ("standard", &(*pSSet).Type);

  return;
}

void
net_tess_opt_init_sset_pre_size (struct TESS *Tess, int dtess,
				 int dcell, struct SEEDSET *pSSet)
{
  struct TESS Cell;

  (*pSSet).Size = ut_alloc_2d (3, 2);
  neut_tess_set_zero (&Cell);
  neut_tess_poly_tess (Tess[dtess], dcell, &Cell);
  neut_tess_bbox (Cell, (*pSSet).Size);
  neut_tess_free (&Cell);

  return;
}

void
net_tess_opt_init_sset_pre_id (struct IN_T In, struct MTESS MTess,
			       struct TESS *Tess, int dtess, int dcell,
			       struct SEEDSET *pSSet)
{
  char *tmp = ut_alloc_1d_char (100);

  neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &tmp);
  net_multiscale_arg_0d_int_fscanf (In.id[Tess[dtess].Level + 1], tmp,
				    0, NULL, NULL, &(*pSSet).Id);

  ut_free_1d_char (tmp);

  return;
}
