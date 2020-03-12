/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_image_.h"

void
nev_image (char **argv, int *pi, struct PRINT *pPrint)
{
  if (strcmp (argv[*pi], "-imagesize") == 0)
    strcpy ((*pPrint).imagesize, argv[++(*pi)]);

  else if (strcmp (argv[*pi], "-imageantialias") == 0)
    ut_string_int (argv[++(*pi)], &((*pPrint).imageantialias));

  else if (strcmp (argv[*pi], "-imagebackground") == 0)
    sscanf (argv[++(*pi)], "%s", (*pPrint).imagebackground);

  else if (strcmp (argv[*pi], "-imageformat") == 0)
    sscanf (argv[++(*pi)], "%s", (*pPrint).format);

  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
