/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_group_tess_.h"

void
net_group_tess (struct IN_T In, struct TESS *pTess)
{
  FILE *file = NULL;

  (*pTess).CellGroup = ut_alloc_1d_int ((*pTess).CellQty + 1);

  if (ut_string_isfilename (In.group))
  {
    file = ut_file_open (In.group, "r");
    ut_array_1d_int_fscanf (file, (*pTess).CellGroup + 1, (*pTess).CellQty);
    ut_file_close (file, In.group, "r");
  }

  else
    neut_tess_entity_expr_val_int (pTess, "cell", In.group, (*pTess).CellGroup);

  return;
}
