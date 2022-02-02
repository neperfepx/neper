/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_camera_.h"

void
nev_camera (char **argv, int *pi, struct PRINT *pPrint)
{
  if (!strcmp (argv[*pi], "-cameracoo"))
    ut_string_string (argv[++(*pi)], &(*pPrint).cameracoostring);

  else if (!strcmp (argv[*pi], "-cameralookat"))
    ut_string_string (argv[++(*pi)], &(*pPrint).cameralookatstring);

  else if (!strcmp (argv[*pi], "-camerasky"))
    ut_string_string (argv[++(*pi)], &(*pPrint).cameraskystring);

  else if (strcmp (argv[*pi], "-cameraangle") == 0)
    ut_string_real (argv[++(*pi)], &((*pPrint).cameraangle));

  else if (strcmp (argv[*pi], "-cameraprojection") == 0)
    sprintf ((*pPrint).cameraprojection, "%s", argv[++(*pi)]);

  else
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);

  return;
}
