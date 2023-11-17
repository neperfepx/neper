/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_pre_.h"

void
net_tess_opt_init_sset_pre (struct IN_T In, int level, struct MTESS MTess,
                            struct TESS *Tess, int dtess, int dcell,
                            struct SEEDSET *SSet,
                            char **pvar, int *ppos, char **pweightexpr,
                            char **pcooexpr, struct TOPT *pTOpt)
{
  int i, qty, *qty1 = NULL;
  char *string = ut_alloc_1d_char (1000);
  char ***parts = NULL;

  // general

  (*pTOpt).SSet.Dim = (*pTOpt).Dim;
  ut_string_string ("standard", &(*pTOpt).SSet.Type);
  (*pTOpt).SSet.N = (*pTOpt).CellQty;
  (*pTOpt).SSet.Nall = (*pTOpt).SSet.N;

  net_ori_mtess_id (In, MTess, Tess, dtess, dcell, &((*pTOpt).SSet));

  net_ori_mtess_randseed (MTess, Tess, dtess, dcell, SSet, (*pTOpt).CellQty, &((*pTOpt).SSet));

  // setting sset.size
  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_init_sset_pre_size (Tess, dtess, dcell, pTOpt);

  (*pTOpt).CellSCellQty = ut_alloc_1d_int ((*pTOpt).CellQty + 1);
  (*pTOpt).CellSCellList = ut_alloc_1d_pint ((*pTOpt).CellQty + 1);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess,
                                           dcell, In.optiini[level],
                                           &string);

  (*ppos) = -1;
  ut_string_string ("none", pvar);

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strncmp ((*pTOpt).tarvar[i], "centroid", 8)
        || !strcmp ((*pTOpt).tarvar[i], "tesr")
        || !strcmp ((*pTOpt).tarvar[i], "size")
        || !strcmp ((*pTOpt).tarvar[i], "diameq"))
    {
      if (*ppos == -1)
      {
        (*ppos) = i;
        ut_string_string ((*pTOpt).tarvar[i], pvar);
      }
      /*
         else
         {
         printf ("\n");
         ut_print_message (1, 4, "Variables `%s' used (not `%s').\n",
         (*pTOpt).tarvar[*ppos], (*pTOpt).tarvar[i]);
         }
       */
    }
  }

  if (!strcmp (string, "default"))
  {
    net_tess_opt_init_sset_pre_default_coo (*pTOpt, *ppos, *pvar, pcooexpr);
    net_tess_opt_init_sset_pre_default_weight (*pTOpt, *pvar, pweightexpr);
  }

  else if (ut_file_exist (In.optiini[level])
           && ut_file_testformat (In.optiini[level], "tess"))
  {
    ut_string_string (In.optiini[level], pcooexpr);
    ut_string_string (In.optiini[level], pweightexpr);
  }

  else
  {
    ut_list_break2 (string, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty1,
                    &qty);

    for (i = 0; i < qty; i++)
      if (!strcmp (parts[i][0], "coo"))
        ut_string_string (parts[i][1], pcooexpr);
      else if (!strcmp (parts[i][0], "weight"))
        ut_string_string (parts[i][1], pweightexpr);
      else
        ut_print_message (2, 3,
                          "Unknown variable `%s' (should be `coo' or `weight').\n",
                          parts[i][0]);

    if (!(*pcooexpr))
      net_tess_opt_init_sset_pre_default_coo (*pTOpt, *ppos, *pvar, pcooexpr);

    if (!(*pweightexpr))
      net_tess_opt_init_sset_pre_default_weight (*pTOpt, *pvar, pweightexpr);
  }

  ut_free_1d_char (&string);

  return;
}
