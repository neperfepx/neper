/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_sim_.h"

// setting the mesh data of a simulation (the mesh orientation descriptor
// prevails over the tessellation one)
void
neut_mesh_sim (struct NODES Nodes, struct MESH *Mesh, struct SIM *pSim)
{
  int dim = neut_mesh_array_dim (Mesh);

  (*pSim).NodeQty = Nodes.NodeQty;

  if (dim >= 0)
  {
    (*pSim).EltQty = Mesh[dim].EltQty;
    (*pSim).ElsetQty = Mesh[dim].ElsetQty;

    if (Mesh[dim].ElsetOriDes)
      ut_string_string (Mesh[dim].ElsetOriDes, &((*pSim).OriDes));
  }

  neut_sim_init_memberqty (pSim);

  return;
}
