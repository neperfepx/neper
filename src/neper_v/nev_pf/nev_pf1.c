/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_pf_.h"

void
nev_pf (char **argv, int *pi, struct PRINT *pPrint)
{
  int *polef = ut_alloc_1d_int (3);

  if (!strcmp (argv[*pi], "-pfpole"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfpolestring);
  else if (!strcmp (argv[*pi], "-pfprojection"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfprojection);
  else if (!strcmp (argv[*pi], "-pfmode"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfmode);
  else if (!strcmp (argv[*pi], "-pffont"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pffont);
  else if (!strcmp (argv[*pi], "-pfgridsize"))
    ut_string_int (argv[++(*pi)], &(*pPrint).pfgridsize);
  else if (!strcmp (argv[*pi], "-pfkernel"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfkernel);
  else if (!strcmp (argv[*pi], "-pfdir"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfdirstring);
  else if (!strcmp (argv[*pi], "-pfsym"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfsym);
  else if (!strcmp (argv[*pi], "-pfclustering"))
    (*pPrint).pfclustering = atoi (argv[++(*pi)]);
  else if (!strcmp (argv[*pi], "-pfshape"))
    ut_string_string (argv[++(*pi)], &(*pPrint).pfshape);
  else
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);

  ut_free_1d_int (&polef);

  return;
}
