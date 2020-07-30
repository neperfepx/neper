/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_space_.h"

void
nev_space (char **argv, int *pi, struct DATA *pPointData)
{
  if (!strcmp (argv[*pi], "-space"))
    ut_string_string (argv[++(*pi)], &(*pPointData).Space);
  else
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);

  return;
}
