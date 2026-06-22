/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin_nodes (struct SIM *pSim, struct NODES Nodes, char *dir,
                                char *res, char *expr)
{
  int status;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  if (!strcmp (expr, "coo"))
    status = nes_pproc_entity_builtin_nodes_coo (pSim, Nodes, dir, res, &SimRes);

  else if (!strcmp (expr, "disp"))
  {
    neut_sim_simres (*pSim, "node", "coo", &SimRes);
    status = nes_pproc_entity_builtin_nodes_disp (pSim, Nodes, dir, res, &SimRes);
  }

  else
    status = -1;

  neut_simres_free (&SimRes);

  return status;
}

int
nes_pproc_entity_builtin_elsets (struct SIM *pSim, struct TESS *pTess,
                                 struct NODES *pNodes, struct MESH *Mesh,
                                 char *entity, char *res, char *expr)
{
  int memberqty, **members = NULL;
  struct SIMRES SimRes;

  nes_pproc_entity_builtin_elsets_pre (pSim, Mesh, entity, res, &SimRes,
                                       &members, &memberqty);

  if (!strcmp (expr, "ori"))
    nes_pproc_entity_builtin_elsets_ori (pSim, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes);

  else if (!strcmp (expr, "gos"))
    nes_pproc_entity_builtin_elsets_gos (pSim, *pTess, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes);

  else if (!strncmp (expr, "oridis", 6))
    nes_pproc_entity_builtin_elsets_oridis (pSim, *pTess, pNodes, Mesh, entity,
                                            res, expr, members, memberqty, &SimRes);

  else if (!strncmp (expr, "odf", 3))
  {
    // if mesh, we compute the odf over orientation space
    if (!strcmp (entity, "mesh"))
      nes_pproc_entity_builtin_elsets_odf (pSim, pTess, pNodes, Mesh, entity, res,
                                           expr, &SimRes);

    // if elt or elset, we determine the odf value from the mesh odf
    else if (!strcmp (entity, "elset") || !strcmp (entity, "elt"))
      nes_pproc_entity_builtin_elsets_readodf (pSim, *pTess, Mesh, entity, res,
                                               &SimRes);
  }

  else if (!strncmp (expr, "orifield", 8))
  {
    // if mesh, we compute the odf over orientation space
    if (!strcmp (entity, "mesh"))
      nes_pproc_entity_builtin_elsets_orifield (pSim, pTess, pNodes, Mesh,
                                                entity, res, expr, &SimRes);
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
                                struct TESR Tesr, char *entity, char *res,
                                char *expr)
{
  int memberqty, **members = NULL;
  struct SIMRES SimRes;
  char *fct = NULL;

  ut_string_function (expr, &fct, NULL, NULL, NULL);

  nes_pproc_entity_builtin_cells_pre (pSim, *pTess, Tesr, entity, res, &SimRes,
                                      &members, &memberqty);

  neut_sim_simres (*pSim, entity, res, &SimRes);

  if (!strcmp (fct, "ori"))
    nes_pproc_entity_builtin_cells_ori (pSim, *pTess, Tesr, entity, res,
                                        &SimRes);

  else if (!strcmp (fct, "gos"))
    nes_pproc_entity_builtin_cells_gos (pSim, *pTess, Tesr, entity, res,
                                        &SimRes);

  else if (!strcmp (fct, "odf") || !strcmp (fct, "odfn") || !strcmp (fct, "odfindex"))
  {
    if (!strcmp (entity, "tess") || !strcmp (entity, "tesr"))
      nes_pproc_entity_builtin_cells_odf (pSim, pTess, Tesr, entity, res,
                                          expr, &SimRes);

    // if cell, we determine the odf value from the tess/tesr odf
    else if (!strcmp (entity, "cell"))
    {
      if (strstr (fct, "index"))
        ut_print_message (2, 2, "`%s' unavailable for cells.\n", fct);

      nes_pproc_entity_builtin_cells_readodf (pSim, *pTess, Tesr, entity, res,
                                               &SimRes);
    }
  }

  else
    abort ();

  ut_free_2d_int (&members, memberqty + 1);
  neut_simres_free (&SimRes);
  ut_free_1d_char (&fct);

  return 0;
}
