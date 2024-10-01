/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the CSIMYING file in the top-level directory. */

#include"neut_fepxsim_sim_.h"

void
neut_fepxsim_sim (struct FEPXSIM FSim, struct SIM *pSim)
{
  int i;
  char *filename = ut_string_paste3 (FSim.fepxdir, "/", FSim.msh);
  struct NODES Nodes;
  struct MESH Mesh3D;

  neut_nodes_set_zero (&Nodes);
  neut_mesh_set_zero (&Mesh3D);

  neut_sim_set_zero (pSim);

  // (*pFSim).simdir

  (*pSim).NodeQty = FSim.NodeQty;
  (*pSim).EltQty = FSim.EltQty;
  (*pSim).PartQty = FSim.PartQty;

  neut_mesh_fnscanf_msh (filename, &Nodes, NULL, NULL, NULL, &Mesh3D, NULL, NULL, "R");
  (*pSim).ElsetQty = Mesh3D.ElsetQty;

  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh3D);

  if (FSim.NodeResQty > 0)
  {
    neut_sim_addentity (pSim, "node");
    for (i = 0; i < FSim.NodeResQty; i++)
      neut_sim_addres (pSim, "node", FSim.NodeRes[i], NULL);
  }

  if (FSim.EltResQty > 0)
  {
    neut_sim_addentity (pSim, "elt");
    for (i = 0; i < FSim.EltResQty; i++)
      neut_sim_addres (pSim, "elt", FSim.EltRes[i], NULL);
  }

  ut_string_string (FSim.OriDes, &(*pSim).OriDes);

  (*pSim).StepQty = FSim.StepQty;
  if (FSim.StepState)
  {
    (*pSim).StepState = ut_alloc_1d_int ((*pSim).StepQty + 1);
    ut_array_1d_int_memcpy (FSim.StepState, (*pSim).StepQty + 1, (*pSim).StepState);
  }

  ut_string_string (FSim.tess, &(*pSim).tess);
  ut_string_string (FSim.tesr, &(*pSim).tesr);
  ut_string_string (FSim.msh, &(*pSim).msh);
  ut_string_string (FSim.bcs, &(*pSim).bcs);
  ut_string_string (FSim.ori, &(*pSim).ori);
  ut_string_string (FSim.phase, &(*pSim).phase);
  ut_string_string (FSim.cfg, &(*pSim).cfg);

  (*pSim).RestartId = FSim.RestartId;
  (*pSim).RestartFiles = FSim.RestartFiles;

  ut_free_1d_char (&filename);

  return;
}
