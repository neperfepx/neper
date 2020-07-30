/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_part_.h"

void
neut_part_set_zero (struct PART *pPart)
{
  (*pPart).qty = 0;
  (*pPart).node_parts = NULL;
  (*pPart).elt_parts = NULL;
  (*pPart).NodeQty = NULL;
  (*pPart).EltQty = NULL;
  (*pPart).targeteltqty = NULL;

  return;
}

void
neut_part_free (struct PART Part)
{
  ut_free_1d_int (&Part.node_parts);
  ut_free_1d_int (&Part.elt_parts);
  ut_free_1d_int (&Part.NodeQty);
  ut_free_1d_int (&Part.EltQty);
  ut_free_1d_int (&Part.targeteltqty);

  return;
}
