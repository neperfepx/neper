/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_pre_.h"

void
net_tess_opt_init_sset_pre_size (struct TESS *Tess, int dtess, int dcell,
                                 struct TOPT *pTOpt)
{
  (*pTOpt).SSet.Size = ut_alloc_2d (3, 2);

  if (pTOpt && !neut_tess_isvoid ((*pTOpt).Dom))
    neut_tess_bbox ((*pTOpt).Dom, (*pTOpt).SSet.Size);

  else
  {
    struct TESS Cell;
    neut_tess_set_zero (&Cell);
    neut_tess_poly_tess (Tess[dtess], dcell, &Cell);
    neut_tess_bbox (Cell, (*pTOpt).SSet.Size);
    neut_tess_free (&Cell);
  }

  return;
}

void
net_tess_opt_init_sset_pre_id (struct IN_T In, struct MTESS MTess,
                               struct TESS *Tess, int dtess, int dcell,
                               struct SEEDSET *pSSet)
{
  char *tmp = ut_alloc_1d_char (100);

  neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &tmp);
  net_multiscale_arg_0d_int_fscanf (In.id[Tess[dtess].Level + 1], tmp, 0,
                                    NULL, NULL, &(*pSSet).Id);

  ut_free_1d_char (&tmp);

  return;
}

void
net_tess_opt_init_sset_pre_default_coo (struct TOPT TOpt, int pos, char *var,
                                        char **pcooexpr)
{
  if (TOpt.tarqty == 0)
    ut_string_string ("random", pcooexpr);
  else if (!strcmp (var, "centroid") || !strcmp (var, "centroidtol"))
  {
    if (strcmp (TOpt.tarexpr[pos], "seed"))
      ut_string_string ("centroid", pcooexpr);
    else
      ut_string_string ("seed", pcooexpr);
  }
  else if (!strcmp (var, "centroiddiameq"))
    ut_string_string ("centroid", pcooexpr);
  else if (!strcmp (var, "tesr"))
    ut_string_string ("centroid", pcooexpr);
  else if (!strcmp (var, "size") || !strcmp (var, "diameq"))
    ut_string_string ("random", pcooexpr);
  else
    ut_string_string ("random", pcooexpr);

  return;
}

void
net_tess_opt_init_sset_pre_default_weight (struct TOPT TOpt, char *var,
                                           char **pweightexpr)
{
  if (TOpt.tarqty == 0)
    ut_string_string ("0", pweightexpr);
  else if (!strcmp (var, "centroid"))
    ut_string_string ("avradeq", pweightexpr);
  else if (!strcmp (var, "centroiddiameq"))
    ut_string_string ("radeq", pweightexpr);
  else if (!strcmp (var, "tesr"))
    ut_string_string ("radeq", pweightexpr);
  else if (!strcmp (var, "size") || !strcmp (var, "diameq"))
    ut_string_string ("avradeq", pweightexpr);
  else
    ut_string_string ("avradeq", pweightexpr);

  return;
}
