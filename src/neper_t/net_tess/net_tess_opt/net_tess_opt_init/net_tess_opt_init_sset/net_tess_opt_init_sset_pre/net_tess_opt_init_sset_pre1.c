/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_pre_.h"

void
net_tess_opt_init_sset_general (struct IN_T In, struct MTESS MTess,
				struct TESS *Tess, int dtess, int dcell,
				struct SEEDSET *SSet, struct SEEDSET 
				*pSSet, int CellQty, struct TOPT *pTOpt)
{
  neut_seedset_set_zero (pSSet);

  net_tess_opt_init_sset_pre_type (pSSet);

  net_tess_opt_init_sset_pre_size (Tess, dtess, dcell, pSSet);

  net_tess_opt_init_sset_pre_id (In, MTess, Tess, dtess, dcell,
				 pSSet);

  net_tess_opt_init_sset_pre_randseed (MTess, Tess, dtess, dcell, SSet,
				       CellQty, pSSet);

  if (pTOpt)
  {
    (*pTOpt).CellSCellQty = ut_alloc_1d_int (CellQty + 1);
    (*pTOpt).CellSCellList = ut_alloc_1d_pint (CellQty + 1);
  }

  return;
}

void
net_tess_opt_init_sset_pre (struct IN_T In, int level,
			    char **pvar, int *ppos, char **pweightexpr,
			    char **pcooexpr, struct TOPT *pTOpt)
{
  int i, qty;

  (*ppos) = -1;
  ut_string_string ("none", pvar);

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strncmp ((*pTOpt).tarvar[i], "centroid", 8)
	|| !strcmp ((*pTOpt).tarvar[i], "tesr")
	|| !strcmp ((*pTOpt).tarvar[i], "size")
	|| !strcmp ((*pTOpt).tarvar[i], "diameq"))
    {
      ut_string_string ((*pTOpt).tarvar[i], pvar);
      if (*ppos != -1)
	abort ();
      (*ppos) = i;
    }
  }

  if (!strcmp (In.morphooptiini[level], "default"))
  {
    if ((*pTOpt).tarqty == 0)
    {
      ut_string_string ("random", pcooexpr);
      ut_string_string ("0", pweightexpr);
    }
    else if (!strcmp (*pvar, "centroid"))
    {
      if (strcmp ((*pTOpt).tarexpr[*ppos], "seed"))
	ut_string_string ("centroid", pcooexpr);
      else
	ut_string_string ("seed", pcooexpr);
      ut_string_string ("avradeq", pweightexpr);
    }
    else if (!strcmp (*pvar, "centroidsize")
	     || !strcmp (*pvar, "centroiddiameq"))
    {
      ut_string_string ("centroid", pcooexpr);
      ut_string_string ("radeq", pweightexpr);
    }
    else if (!strcmp (*pvar, "tesr"))
    {
      ut_string_string ("centroid", pcooexpr);
      ut_string_string ("radeq", pweightexpr);
    }
    else if (!strcmp (*pvar, "size") || !strcmp (*pvar, "diameq"))
    {
      ut_string_string ("random", pcooexpr);
      ut_string_string ("avradeq", pweightexpr);
    }
    else
    {
      ut_string_string ("random", pcooexpr);
      ut_string_string ("avradeq", pweightexpr);
    }
  }

  else
  {
    char **tmp = NULL;
    ut_string_separate (In.morphooptiini[level], NEUT_SEP_NODEP, &tmp, &qty);
    if (qty != 2)
      abort ();
    for (i = 0; i < 2; i++)
      if (!strncmp (tmp[i], "coo:", 4))
	ut_string_string (tmp[i] + 4, pcooexpr);
      else if (!strncmp (tmp[i], "weight:", 7))
	ut_string_string (tmp[i] + 7, pweightexpr);
      else
	abort ();
    ut_free_2d_char (tmp, qty);
  }

  return;
}
