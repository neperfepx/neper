/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_tocta_seed_.h"
#include"../../net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_coo/net_tess_opt_init_sset_coo.h"

int
net_tess_tocta_seed_readargs (char *morpho, int *pn)
{
  int varqty;
  char **vars = NULL, **vals = NULL;

  ut_string_function (morpho, NULL, &vars, &vals, &varqty);

  if (varqty != 1)
    abort ();

  sscanf (vals[0], "%d", pn);

  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);

  return 0;
}

int
net_tess_tocta_seed_set (struct IN_T In, int level, struct MTESS MTess,
                         struct TESS *Tess, int dtess, int dcell,
                         struct TESS Dom, int n, struct SEEDSET *SSet,
                         struct SEEDSET *pSSet)
{
  char *ori = NULL, *crysym = NULL;

  ut_print_message (0, 2, "Setting seeds...\n");
  net_tess_opt_init_sset_general (In, MTess, Tess, dtess, dcell, SSet, pSSet,
                                  1, NULL);

  (*pSSet).N = net_tess_opt_init_sset_coo_bcc (Dom, n, &(*pSSet).SeedCoo);

  net_tess_tocta_seed_set_finalize (pSSet);

  ut_print_message (0, 2, "Generating crystal orientations...\n");
  net_ori (In, level, MTess, Tess, SSet, dtess, dcell, pSSet, 3);

  ut_free_1d_char (&ori);
  ut_free_1d_char (&crysym);

  return 0;
}
