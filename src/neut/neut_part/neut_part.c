/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_part_.h"

void
neut_part_set_zero (struct PART *pPart)
{
  (*pPart).qty = 0;
  (*pPart).node_parts = NULL;
  (*pPart).elt_parts = NULL;
  (*pPart).nodeqty = NULL;
  (*pPart).eltqty = NULL;
  (*pPart).targeteltqty = NULL;

  return;
}

void
neut_part_free (struct PART Part)
{
  ut_free_1d_int (Part.node_parts);
  ut_free_1d_int (Part.elt_parts);
  ut_free_1d_int (Part.nodeqty);
  ut_free_1d_int (Part.eltqty);
  ut_free_1d_int (Part.targeteltqty);

  return;
}
