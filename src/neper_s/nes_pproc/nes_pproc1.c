/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_.h"

void
nes_pproc (struct IN_S In, struct SIM *pSim)
{
  int i;
  struct TESS Tess;
  struct NODES Nodes;
  struct MESH *Mesh = malloc (5 * sizeof (struct MESH));

  neut_tess_set_zero (&Tess);

  neut_nodes_set_zero (&Nodes);

  for (i = 0; i < 5; i++)
    neut_mesh_set_zero (Mesh + i);

  neut_sim_fscanf (In.simdir, pSim, "R");
  neut_sim_fprintf (In.simdir, *pSim, "W");

  nes_pproc_loadmesh (*pSim, &Tess, &Nodes, Mesh);

  if (strcmp (In.noderes, "none") && strcmp (In.noderes, "inputres"))
    nes_pproc_entity (In, pSim, Tess, &Nodes, Mesh, "node");

  if (strcmp (In.eltres, "none") && strcmp (In.eltres, "inputres"))
    nes_pproc_entity (In, pSim, Tess, &Nodes, Mesh, "element");

  if (strcmp (In.elsetres, "none") && strcmp (In.elsetres, "inputres"))
    nes_pproc_entity (In, pSim, Tess, &Nodes, Mesh, "elset");

  neut_tess_free (&Tess);
  neut_nodes_free (&Nodes);

  for (i = 0; i < 5; i++)
    neut_mesh_free (Mesh + i);

  return;
}
