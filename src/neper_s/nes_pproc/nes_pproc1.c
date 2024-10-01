/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

void
nes_pproc (struct IN_S In, struct SIM *pSim)
{
  int i;
  struct TESS Tess;
  struct TESR Tesr;
  struct NODES Nodes;
  struct MESH *Mesh = malloc (5 * sizeof (struct MESH));

  neut_tess_set_zero (&Tess);
  neut_tesr_set_zero (&Tesr);
  neut_nodes_set_zero (&Nodes);
  for (i = 0; i < 5; i++)
    neut_mesh_set_zero (Mesh + i);

  nes_pproc_load (pSim, &Tess, &Tesr, &Nodes, Mesh);

  for (i = 0; i < In.entityqty; i++)
    nes_pproc_entity (pSim, &Tess, Tesr, &Nodes, Mesh, In.entities[i], In.entityresexpr[i]);

  neut_tess_free (&Tess);
  neut_tesr_free (&Tesr);
  neut_nodes_free (&Nodes);
  for (i = 0; i < 5; i++)
    neut_mesh_free (Mesh + i);

  return;
}
