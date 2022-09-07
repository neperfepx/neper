/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_scene_.h"

void
nev_scene (char **argv, int *pi, struct PRINT *pPrint)
{
  if (strcmp (argv[*pi], "-scenebackground") == 0)
    sscanf (argv[++(*pi)], "%s", (*pPrint).scenebackground);

  else
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);

  return;
}
