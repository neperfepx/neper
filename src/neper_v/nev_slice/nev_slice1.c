/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_slice_.h"

void
nev_slice (char **argv, int *pi, struct PRINT *pPrint)
{
  if (strcmp (argv[*pi], "-slicemesh") == 0)
  {
    (*pi)++;
    (*pPrint).slice = ut_alloc_1d_char (strlen (argv[(*pi)]) + 1);
    sscanf (argv[(*pi)], "%s", (*pPrint).slice);
  }
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  if ((*pPrint).showslice)
  {
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showtesr == -1)
      (*pPrint).showtesr = 0;
    if ((*pPrint).showmesh == -1)
      (*pPrint).showmesh = 0;
  }

  sscanf (argv[(*pi)], "%s", (*pPrint).slice);

  return;
}
