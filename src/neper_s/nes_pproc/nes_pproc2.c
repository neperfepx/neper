/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

void
nes_pproc_load (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes, struct MESH *Mesh)
{
  int GroupQty = 0;
  char **GroupCrySym = NULL;

  ut_print_message (0, 2, "Loading inputs...\n");

  if (Sim.tess)
    nes_loadtess (Sim, pTess, 2);

  if (Sim.msh)
    nes_loadmesh (Sim, pNodes, Mesh, 2);

  if (Sim.config)
    nes_loadconfig (Sim, &GroupCrySym, &GroupQty, 2);

  nes_pproc_load_crysym (pTess, GroupCrySym, GroupQty);

  ut_free_2d_char (&GroupCrySym, GroupQty);

  return;
}
