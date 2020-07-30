/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_camera_.h"

void
nev_camera (char **argv, int *pi, struct PRINT *pPrint)
{
  if (!strcmp (argv[*pi], "-cameracoo"))
  {
    (*pi)++;
    (*pPrint).cameracoostring =
      ut_realloc_1d_char ((*pPrint).cameracoostring, strlen (argv[*pi]) + 1);
    strcpy ((*pPrint).cameracoostring, argv[*pi]);
  }

  else if (!strcmp (argv[*pi], "-cameralookat"))
  {
    (*pi)++;
    (*pPrint).cameralookatstring =
      ut_realloc_1d_char ((*pPrint).cameralookatstring,
                          strlen (argv[*pi]) + 1);
    strcpy ((*pPrint).cameralookatstring, argv[*pi]);
  }

  else if (!strcmp (argv[*pi], "-camerasky"))
  {
    (*pi)++;
    (*pPrint).cameraskystring =
      ut_realloc_1d_char ((*pPrint).cameraskystring, strlen (argv[*pi]) + 1);
    strcpy ((*pPrint).cameraskystring, argv[*pi]);
  }

  else if (strcmp (argv[*pi], "-cameraangle") == 0)
    ut_string_real (argv[++(*pi)], &((*pPrint).cameraangle));

  else if (strcmp (argv[*pi], "-cameraprojection") == 0)
    sprintf ((*pPrint).cameraprojection, "%s", argv[++(*pi)]);

  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
