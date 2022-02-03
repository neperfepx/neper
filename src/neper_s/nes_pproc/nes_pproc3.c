/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

// Checking consistency between Tess and Sim crysym, if simulation.config exists
void
nes_pproc_load_crysym (struct TESS *pTess, char **GroupCrySym, int GroupQty)
{
  int i, multiple;

  if (GroupQty == 0)
    return;

  // Does simulation.config have multiple crystal symmetries?
  multiple = 0;
  for (i = 1; i < GroupQty; i++)
    if (strcmp (GroupCrySym[0], GroupCrySym[i]))
    {
      multiple = 1;
      break;
    }

  // Not multiple: full check
  if (!multiple)
  {
    if ((*pTess).CellCrySym && strcmp ((*pTess).CellCrySym, GroupCrySym[0]))
    {
      if (!strcmp ((*pTess).CellCrySym, "triclinic"))
        ut_string_string (GroupCrySym[0], &((*pTess).CellCrySym));
      else
        ut_print_message (2, 3, "The crystal symmetries of tessellation and configuration file differ.\n");
    }
    else
      ut_string_string (GroupCrySym[0], &((*pTess).CellCrySym));
  }

  // Multiple, which TESS cannot take at the moment. Letting it slide...
  else
    ut_string_string ("triclinic", &((*pTess).CellCrySym));

  return;
}
