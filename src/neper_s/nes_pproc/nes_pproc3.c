/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

// Checking consistency between Tess and Sim crysym, if simulation.cfg exists
void
nes_pproc_load_crysym (struct TESS *pTess, struct TESR *pTesr,
                       struct NODES *pNodes, struct MESH *Mesh,
                       char **GroupCrySym, int GroupQty)
{
  int i, multiple;

  if (GroupQty)
  {
    // Does simulation.cfg have multiple crystal symmetries?
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
      if (!neut_tess_isvoid (*pTess) && (*pTess).CellCrySym && strcmp ((*pTess).CellCrySym, GroupCrySym[0]))
      {
        if (!strcmp ((*pTess).CellCrySym, "triclinic"))
          ut_string_string (GroupCrySym[0], &((*pTess).CellCrySym));
        else
          ut_print_message (2, 3, "The crystal symmetries of tessellation and configuration file differ.\n");
      }
      else if (!neut_tesr_isvoid (*pTesr) && (*pTesr).CellCrySym && strcmp ((*pTesr).CellCrySym, GroupCrySym[0]))
      {
        if (!strcmp ((*pTesr).CellCrySym, "triclinic"))
          ut_string_string (GroupCrySym[0], &((*pTesr).CellCrySym));
        else
          ut_print_message (2, 3, "The crystal symmetries of tessellation and configuration file differ.\n");
      }
      else
        ut_string_string (GroupCrySym[0], &((*pTess).CellCrySym));
    }

    // Multiple, which TESS cannot take at the moment. Letting it slide...
    else
      ut_string_string ("triclinic", &((*pTess).CellCrySym));
  }

  // Pushing Tess crysym to Nodes and Mesh if not defined there

  for (i = 0; i < 5; i++)
    if (!Mesh[i].ElsetCrySym)
      ut_string_string ((*pTess).CellCrySym, &(Mesh[i].ElsetCrySym));

  if (!(*pNodes).crysym)
    ut_string_string ((*pTess).CellCrySym, &((*pNodes).crysym));

  return;
}
