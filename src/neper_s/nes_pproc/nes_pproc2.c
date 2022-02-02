/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

void
nes_pproc_load (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, GroupQty, differ;
  char **GroupCrySym = NULL;

  ut_print_message (0, 2, "Loading inputs...\n");

  nes_loadtess (Sim, pTess, 2);

  nes_loadmesh (Sim, pNodes, Mesh, 2);

  nes_loadconfig (Sim, &GroupCrySym, &GroupQty, 2);

  // checking consistency between Tess and Sim crysym

  differ = 0;
  for (i = 1; i < GroupQty; i++)
    if (strcmp (GroupCrySym[0], GroupCrySym[i]))
    {
      differ = 1;
      break;
    }

  if (!differ)
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

  else
    ut_string_string ("triclinic", &((*pTess).CellCrySym));

  ut_free_2d_char (&GroupCrySym, GroupQty);

  return;
}
