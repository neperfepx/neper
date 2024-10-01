/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin (struct SIM *pSim, struct TESS *pTess, struct TESR Tesr,
                       struct NODES *pNodes, struct MESH *Mesh, char *entity,
                       struct SIMRES *pSimRes)
{
  int status = -1;
  char *type = NULL;

  neut_sim_entity_type (*pSim, entity, &type);

  if (!strcmp (type, "node"))
    status = nes_pproc_entity_builtin_nodes (pSim, *pNodes, (*pSimRes).dir,
                                          (*pSimRes).res);

  // entity is "elsets", "mesh" or a custom entity
  else if (!strcmp (type, "elt") || !strcmp (type, "elset") || !strcmp (type, "mesh"))
    status = nes_pproc_entity_builtin_elsets (pSim, pTess, pNodes, Mesh, entity,
                                              (*pSimRes).res);

  // entity is cell
  else if (!strcmp (type, "cell"))
    status = nes_pproc_entity_builtin_cells (pSim, pTess, Tesr, entity,
                                             (*pSimRes).res);

  ut_free_1d_char (&type);

  return status;
}
