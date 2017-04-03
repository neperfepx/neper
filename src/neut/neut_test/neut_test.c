/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_test_.h"

void
neut_test_set_zero (struct TEST *pTest)
{
  (*pTest).Qty = 0;
  (*pTest).Id = NULL;
  (*pTest).ModPreCom = NULL;
  (*pTest).PreCom = NULL;
  (*pTest).Com = NULL;
  (*pTest).ComDep = NULL;

  return;
}

void
neut_test_free (struct TEST *pTest)
{
  ut_free_1d_int ((*pTest).Id);
  ut_free_1d_char ((*pTest).ModPreCom);
  ut_free_2d_char ((*pTest).PreCom, (*pTest).Qty);
  ut_free_2d_char ((*pTest).ComDep, (*pTest).Qty);
  ut_free_2d_char ((*pTest).Com, (*pTest).Qty);
  (*pTest).Qty = 0;

  return;
}
