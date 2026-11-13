/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_input_.h"

int
nev_input (int argc, char **argv, struct IN_V *pIn, int *ppos)
{
  if (*ppos == 1)
    ut_print_message (0, 2, "Reading arguments...\n");

  nev_input_treatargs (argc, argv, pIn, ppos);

  return (*ppos == -1) ? -1 : 0;
}

void
nev_input_free (struct IN_V *pIn)
{
  ut_free_1d_char (&(*pIn).input);
  ut_free_1d_char (&(*pIn).povray);
  ut_free_1d_char (&(*pIn).asymptote);
  ut_free_1d_char (&(*pIn).step);
  ut_free_1d_char (&(*pIn).crysym);
  ut_free_1d_char (&(*pIn).space);
  ut_free_1d_char (&(*pIn).slice);
  ut_free_1d_char (&(*pIn).cameracoo);
  ut_free_1d_char (&(*pIn).cameralookat);
  ut_free_1d_char (&(*pIn).camerasky);
  ut_free_1d_char (&(*pIn).cameraangle);
  ut_free_1d_char (&(*pIn).cameraprojection);
  ut_free_1d_char (&(*pIn).lightsource);
  ut_free_1d_char (&(*pIn).lightambient);
  ut_free_1d_char (&(*pIn).lightdiffuse);
  ut_free_1d_char (&(*pIn).lightreflection);
  ut_free_1d_char (&(*pIn).scenebackground);
  ut_free_1d_char (&(*pIn).pfpolestring);
  ut_free_1d_char (&(*pIn).pfpolelabel);
  ut_free_1d_char (&(*pIn).pfprojection);
  ut_free_1d_char (&(*pIn).pfprojlabel);
  ut_free_1d_char (&(*pIn).pfmode);
  ut_free_1d_char (&(*pIn).pffont);
  ut_free_1d_char (&(*pIn).pfgridsize);
  ut_free_1d_char (&(*pIn).pfkernel);
  ut_free_1d_char (&(*pIn).pfdirstring);
  ut_free_1d_char (&(*pIn).pfsym);
  ut_free_1d_char (&(*pIn).pfshape);
  ut_free_1d_char (&(*pIn).oricol);
  ut_free_1d_char (&(*pIn).oriedgecol);
  ut_free_1d_char (&(*pIn).oriedgerad);
  ut_free_1d_char (&(*pIn).orimode);
  ut_free_1d_char (&(*pIn).orilayout);
  ut_free_1d_char (&(*pIn).orisym);
  ut_free_1d_char (&(*pIn).orifield);
  ut_free_1d_char (&(*pIn).imagesize);
  ut_free_1d_char (&(*pIn).imageformat);
  ut_free_1d_char (&(*pIn).includepov);
  ut_free_1d_char (&(*pIn).print);

  ut_free_3d_char (&(*pIn).data, (*pIn).dataqty, 2);
  (*pIn).dataqty = 0;
  ut_free_3d_char (&(*pIn).show, (*pIn).showqty, 2);
  (*pIn).showqty = 0;

  return;
}
