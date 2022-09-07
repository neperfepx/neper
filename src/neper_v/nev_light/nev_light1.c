/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_light_.h"

void
nev_light (char **argv, int *pi, struct PRINT *pPrint)
{
  if (!strcmp (argv[*pi], "-lightambient"))
    ut_string_string (argv[++(*pi)], &(*pPrint).lightambientstring);

  else if (!strcmp (argv[*pi], "-lightsource"))
    ut_string_string (argv[++(*pi)], &(*pPrint).lightsourcestring);

  else if (!strcmp (argv[*pi], "-lightdiffuse"))
    ut_string_string (argv[++(*pi)], &(*pPrint).lightdiffusestring);

  else if (!strcmp (argv[*pi], "-lightreflection"))
    ut_string_string (argv[++(*pi)], &(*pPrint).lightreflectionstring);

  else
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);

  return;
}
