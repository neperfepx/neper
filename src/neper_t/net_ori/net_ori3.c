/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_.h"

void
net_orispread_file (char *orispread, struct SEEDSET *pSSet)
{
  int i;
  char *tmp = ut_alloc_1d_char (1000);

  FILE *file = ut_file_open (orispread, "r");

  for (i = 1; i <= (*pSSet).N; i++)
  {
    if (fscanf (file, "%s", tmp) != 1)
      abort ();
    ut_string_string (tmp, (*pSSet).SeedOriDistrib + i);
  }

  ut_file_close (file, orispread, "r");

  ut_free_1d_char (&tmp);

  return;
}
