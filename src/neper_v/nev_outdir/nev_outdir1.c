/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_outdir_.h"

void
nev_outdir (char **argv, int *pi, struct PRINT *pPrint)
{
  if (!strcmp (argv[*pi], "-outdir"))
    ut_string_string (argv[++(*pi)], &(*pPrint).outdir);
  else
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);

  return;
}
