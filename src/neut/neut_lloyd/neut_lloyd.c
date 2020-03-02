/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_lloyd_.h"

void
neut_lloyd_set_zero (struct LLOYD *pLloyd)
{
  (*pLloyd).itermax = 0;
  (*pLloyd).val = 0;
  (*pLloyd).fact = 0;
  (*pLloyd).norm = NULL;

  (*pLloyd).dimqty = 0;
  (*pLloyd).dims = NULL;

  (*pLloyd).iter = 0;
  (*pLloyd).objval = 0;

  return;
}

void
neut_lloyd_free (struct LLOYD *pLloyd)
{
  ut_free_1d_char (&(*pLloyd).norm);
  ut_free_1d_int (&(*pLloyd).dims);

  neut_lloyd_set_zero (pLloyd);

  return;
}
