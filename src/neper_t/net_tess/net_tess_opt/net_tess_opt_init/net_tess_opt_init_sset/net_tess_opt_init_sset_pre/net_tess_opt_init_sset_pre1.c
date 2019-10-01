/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_pre_.h"

void
net_tess_opt_init_sset_general (struct IN_T In, struct MTESS MTess,
				struct TESS *Tess, int dtess, int dcell,
				struct SEEDSET *SSet, struct SEEDSET
				*pSSet, int CellQty, struct TOPT *pTOpt)
{
  neut_seedset_set_zero (pSSet);

  if (pTOpt)
    net_tess_opt_init_sset_pre_dim (*pTOpt, pSSet);

  net_tess_opt_init_sset_pre_type (pSSet);

  net_tess_opt_init_sset_pre_size (Tess, dtess, dcell, pTOpt, pSSet);

  net_ori_mtess_id (In, MTess, Tess, dtess, dcell, pSSet);

  net_ori_mtess_randseed (MTess, Tess, dtess, dcell, SSet, CellQty, pSSet);

  if (pTOpt)
  {
    (*pTOpt).CellSCellQty = ut_alloc_1d_int (CellQty + 1);
    (*pTOpt).CellSCellList = ut_alloc_1d_pint (CellQty + 1);
  }

  return;
}

void
net_tess_opt_init_sset_pre (struct IN_T In, int level,
			    struct MTESS MTess, struct TESS *Tess,
			    int domtess, int domcell,
			    char **pvar, int *ppos, char **pweightexpr,
			    char **pcooexpr, struct TOPT *pTOpt)
{
  int i, qty, *qty1 = NULL;
  char* string = ut_alloc_1d_char (1000);
  char ***parts = NULL;

  ut_free_1d_char (*pweightexpr);
  ut_free_1d_char (*pcooexpr);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, domtess, domcell,
					   In.morphooptiini[level], &string);

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

  else if (ut_file_exist (In.morphooptiini[level])
        && ut_file_testformat (In.morphooptiini[level], "tess"))
  {
    ut_string_string (In.morphooptiini[level], pcooexpr);
    ut_string_string (In.morphooptiini[level], pweightexpr);
  }

  else
  {
    ut_string_separate2 (string, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty1, &qty);

    for (i = 0; i < qty; i++)
      if (!strcmp (parts[i][0], "coo"))
	ut_string_string (parts[i][1], pcooexpr);
      else if (!strcmp (parts[i][0], "weight"))
	ut_string_string (parts[i][1], pweightexpr);
      else
	ut_print_message (2, 3, "Unknown variable `%s' (should be `coo' or `weight').\n",
	                  parts[i][0]);

    if (!(*pcooexpr))
      net_tess_opt_init_sset_pre_default_coo (*pTOpt, *ppos, *pvar, pcooexpr);

    if (!(*pweightexpr))
      net_tess_opt_init_sset_pre_default_weight (*pTOpt, *pvar, pweightexpr);
  }

  ut_free_1d_char (string);

  return;
}
