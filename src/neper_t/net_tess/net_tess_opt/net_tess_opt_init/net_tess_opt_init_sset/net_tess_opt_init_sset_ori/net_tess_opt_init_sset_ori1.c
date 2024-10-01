/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori (int TessId, struct SEEDSET *SSet, char* oriexpr, struct TOPT *pTOpt)
{
  int i;
  FILE *fp = NULL;
  struct TESS Tmp;

  if (ut_string_isfilename (oriexpr))
  {
    if (ut_file_testformat (oriexpr, "tess"))
    {
      neut_tess_set_zero (&Tmp);
      fp = ut_file_open (oriexpr, "R");
      neut_tess_fscanf (fp, &Tmp);
      (*pTOpt).SSet.SeedOri = ut_alloc_2d ((*pTOpt).CellQty + 1, 4);
      ut_array_2d_memcpy (Tmp.CellOri + 1, (*pTOpt).CellQty, 4, (*pTOpt).SSet.SeedOri + 1);
      ut_file_close (fp, oriexpr, "R");
      neut_tess_free (&Tmp);
    }
    else
    {
      struct OL_SET OSet;
      ol_set_zero (&OSet);
      net_ori_file (oriexpr, &OSet);
      net_ori_memcpy (OSet, &(*pTOpt).SSet);
      ol_set_free (&OSet);
    }
  }

  else
  {
    neut_seedset_memcpy (SSet[TessId], &(*pTOpt).SSet);

    (*pTOpt).SSet.SeedOriR = ut_alloc_2d ((*pTOpt).SSet.N + 1, 3);
    for (i = 1; i <= (*pTOpt).SSet.N; i++)
      ol_q_R ((*pTOpt).SSet.SeedOri[i], (*pTOpt).SSet.SeedOriR[i]);
  }

  if (SSet[TessId].SeedOriTheta || strstr ((*pTOpt).dof, "rt"))
  {
    (*pTOpt).SSet.SeedOriTheta = ut_alloc_1d ((*pTOpt).SSet.N + 1);
    if (SSet[TessId].SeedOriTheta)
      ut_array_1d_memcpy (SSet[TessId].SeedOriTheta + 1, (*pTOpt).SSet.N, (*pTOpt).SSet.SeedOriTheta + 1);
  }

  if (SSet[TessId].SeedOriWeight)
    ut_array_1d_memcpy (SSet[TessId].SeedOriWeight + 1, (*pTOpt).SSet.N, (*pTOpt).SSet.SeedOriWeight + 1);

  return;
}
