/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_group_tesr_.h"

void
net_group_tesr (struct IN_T In, struct TESR *pTesr)
{
  FILE *file = NULL;

  (*pTesr).CellGroup = ut_alloc_1d_int ((*pTesr).CellQty + 1);

  if (ut_string_isfilename (In.group))
  {
    file = ut_file_open (In.group, "r");
    ut_array_1d_int_fscanf (file, (*pTesr).CellGroup + 1, (*pTesr).CellQty);
    ut_file_close (file, In.group, "r");
  }

  else
    neut_tesr_entity_expr_val_int (*pTesr, "cell", In.group, (*pTesr).CellGroup);

  return;
}
