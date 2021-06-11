/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_include_.h"

void
nev_include (char **argv, int *pi, struct PRINT *pPrint)
{
  // load node coordinates ---------------------------------------------
  if (strcmp (argv[(*pi)], "-includepov") == 0)
  {
    (*pi)++;
    (*pPrint).includepov = ut_alloc_1d_char (strlen (argv[*pi]) + 1);
    sscanf (argv[*pi], "%s", (*pPrint).includepov);
  }
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
