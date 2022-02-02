/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_povray_.h"

void
nev_povray (char **expargv, int *pi, struct PRINT *pPrint)
{
  (*pi)++;

  ut_string_string (expargv[*pi], &((*pPrint).povray));

  return;
}
