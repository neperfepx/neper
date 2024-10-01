/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_sim_.h"

void
neut_mesh_sim (char *simdir, struct TESS Tess, struct NODES Nodes, struct MESH *Mesh, struct SIM *pSim)
{
  neut_sim_set_zero (pSim);
  ut_string_string (simdir, &((*pSim).simdir));
  (*pSim).NodeQty = Nodes.NodeQty;
  (*pSim).EltQty = Mesh[Tess.Dim].EltQty;
  (*pSim).ElsetQty = Mesh[Tess.Dim].ElsetQty;
  ut_string_string (Mesh[Tess.Dim].ElsetOriDes, &((*pSim).OriDes));

  ut_string_string ("simulation.msh", &(*pSim).msh);
  ut_string_string ("simulation.tess", &(*pSim).tess);

  return;
}
