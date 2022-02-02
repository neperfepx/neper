/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_crys_.h"

void
neut_crys_set_zero (struct CRYS *pCrys)
{
  (*pCrys).crysym = NULL;

  return;
}

void
neut_crys_free (struct CRYS *pCrys)
{
  ut_free_1d_char (&(*pCrys).crysym);

  return;
}
