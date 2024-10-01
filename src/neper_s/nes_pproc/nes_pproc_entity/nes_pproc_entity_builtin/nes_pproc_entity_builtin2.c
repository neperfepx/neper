/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin_nodes (struct SIM *pSim, struct NODES Nodes, char *dir, char *res)
{
  int status;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  neut_sim_simres (*pSim, "node", "coo", &SimRes);

  if (!strcmp (res, "disp"))
    status = nes_pproc_entity_builtin_nodes_disp (pSim, Nodes, dir, res, &SimRes);
  else
    status = -1;

  neut_simres_free (&SimRes);

  return status;
}

int
nes_pproc_entity_builtin_elsets (struct SIM *pSim, struct TESS *pTess,
                                 struct NODES *pNodes, struct MESH *Mesh,
                                 char *entity, char *res)
{
  int memberqty, **members = NULL;
  struct SIMRES SimRes;

  nes_pproc_entity_builtin_elsets_pre (pSim, Mesh, entity, res, &SimRes,
                                       &members, &memberqty);

  if (!strcmp (res, "ori"))
    nes_pproc_entity_builtin_elsets_ori (pSim, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes);

  else if (!strcmp (res, "gos"))
    nes_pproc_entity_builtin_elsets_gos (pSim, *pTess, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes);

  else if (!strncmp (res, "oridis", 6))
    nes_pproc_entity_builtin_elsets_oridis (pSim, *pTess, pNodes, Mesh, entity,
                                            res, members, memberqty, &SimRes);

  else if (!strncmp (res, "odf", 3))
  {
    // if mesh, we compute the odf over orientation space
    if (!strcmp (entity, "mesh"))
      nes_pproc_entity_builtin_elsets_odf (pSim, pTess, pNodes, Mesh, entity, res,
                                           &SimRes);

    // if elt or elset, we determine the odf value from the mesh odf
    else if (!strcmp (entity, "elset") || !strcmp (entity, "elt"))
      nes_pproc_entity_builtin_elsets_readodf (pSim, *pTess, Mesh, entity, res,
                                               &SimRes);
  }

  else if (!strncmp (res, "orifield", 8))
  {
    // if mesh, we compute the odf over orientation space
    if (!strcmp (entity, "mesh"))
      nes_pproc_entity_builtin_elsets_orifield (pSim, pTess, pNodes, Mesh,
                                                entity, res, &SimRes);
  }

  else
    nes_pproc_entity_builtin_elsets_gen (pSim, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes);

  ut_free_2d_int (&members, memberqty);
  neut_simres_free (&SimRes);

  return 0;
}

int
nes_pproc_entity_builtin_cells (struct SIM *pSim, struct TESS *pTess,
                                struct TESR Tesr, char *entity, char *res)
{
  int memberqty, **members = NULL;
  struct SIMRES SimRes;

  nes_pproc_entity_builtin_cells_pre (pSim, *pTess, Tesr, entity, res, &SimRes,
                                      &members, &memberqty);

  neut_sim_simres (*pSim, entity, res, &SimRes);

  if (!strcmp (res, "ori"))
    nes_pproc_entity_builtin_cells_ori (pSim, *pTess, Tesr, entity, res,
                                        &SimRes);

  else if (!strcmp (res, "gos"))
    nes_pproc_entity_builtin_cells_gos (pSim, *pTess, Tesr, entity, res,
                                        &SimRes);

  else if (!strncmp (res, "odf", 3))
  {
    if (!strcmp (entity, "tess") || !strcmp (entity, "tesr"))
      nes_pproc_entity_builtin_cells_odf (pSim, pTess, Tesr, entity, res,
                                          &SimRes);

    // if cell, we determine the odf value from the tess/tesr odf
    else if (!strcmp (entity, "cell"))
      nes_pproc_entity_builtin_cells_readodf (pSim, *pTess, Tesr, entity, res,
                                               &SimRes);
  }

  else
    abort ();

  ut_free_2d_int (&members, memberqty + 1);
  neut_simres_free (&SimRes);

  return 0;
}
