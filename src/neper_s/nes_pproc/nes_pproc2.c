/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

void
nes_pproc_load (struct SIM *pSim, struct TESS *pTess, struct TESR *pTesr,
                struct NODES *pNodes, struct MESH *Mesh)

{
  int GroupQty = 0;
  char **GroupCrySym = NULL;

  ut_print_message (0, 2, "Loading inputs...\n");

  if ((*pSim).tess)
    nes_loadtess (pSim, pTess, 2);

  if ((*pSim).tesr)
    nes_loadtesr (pSim, pTesr, 2);

  if ((*pSim).msh)
    nes_loadmesh (pSim, pNodes, Mesh, 2);

  if ((*pSim).cfg)
    nes_loadconfig (*pSim, &GroupCrySym, &GroupQty, 2);

  neut_sim_testinputs (*pSim, *pTess, Mesh);

  nes_pproc_load_crysym (pTess, pTesr, pNodes, Mesh, GroupCrySym, GroupQty);

  ut_free_2d_char (&GroupCrySym, GroupQty);

  return;
}
