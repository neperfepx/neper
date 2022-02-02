/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin_nodes (struct SIM *pSim, char *dir, char *res)
{
  int status;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  neut_sim_simres (*pSim, "node", "coo", &SimRes);

  if (!strcmp (res, "disp"))
    status = nes_pproc_entity_builtin_nodes_disp (pSim, dir, res, &SimRes);
  else
    status = -1;

  neut_simres_free (&SimRes);

  return status;
}

int
nes_pproc_entity_builtin_elsets (struct SIM *pSim, struct TESS Tess,
                                 struct NODES *pNodes, struct MESH *Mesh,
                                 char *entity, char *res)
{
  int memberqty, **members = NULL;
  struct SIMRES SimRes, SimRes2;

  nes_pproc_entity_builtin_elsets_pre (pSim, Tess, Mesh, entity, res, &SimRes,
                                       &SimRes2, &members, &memberqty);

  if (!strcmp (res, "ori"))
    nes_pproc_entity_builtin_elsets_ori (pSim, Tess, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes, &SimRes2);

  else if (!strncmp (res, "oridis", 6))
    nes_pproc_entity_builtin_elsets_oridis (pSim, Tess, pNodes, Mesh, entity,
                                            res, members, memberqty, &SimRes,
                                            &SimRes2);

  else if (!strncmp (res, "odf", 3))
    nes_pproc_entity_builtin_elsets_odf (pSim, Tess, pNodes, Mesh, entity, res,
                                         &SimRes, &SimRes2);

  else
    nes_pproc_entity_builtin_elsets_gen (pSim, Tess, pNodes, Mesh, entity, res,
                                         members, memberqty, &SimRes, &SimRes2);

  ut_free_2d_int (&members, memberqty);
  neut_simres_free (&SimRes);
  neut_simres_free (&SimRes2);

  return 0;
}
