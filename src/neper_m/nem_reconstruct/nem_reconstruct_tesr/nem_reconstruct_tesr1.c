/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_tesr_.h"

void
nem_reconstruct_tesr (struct TESR *pTesr, struct TESS *pTess,
                      struct NODES *pNodes, struct MESH *Mesh,
                      struct NSET *NSet)
{
  // Setting VoxCell at the domain boundary
  nem_reconstruct_tesr_domain (pTesr);

  nem_reconstruct_tesr_tesr (*pTesr, pTess, pNodes, Mesh, NSet);

  return;
}
