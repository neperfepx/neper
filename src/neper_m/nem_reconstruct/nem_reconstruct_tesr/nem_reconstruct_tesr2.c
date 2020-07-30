/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_tesr_.h"

// Considering that the domain is a filled cube to set VoxCell at the
// boundary.
void
nem_reconstruct_tesr_domain (struct TESR *pTesr)
{
  int i, j, k;

  // x = 0 and x = 1
  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
    {
      (*pTesr).VoxCell[0][j][k] = -1;
      (*pTesr).VoxCell[(*pTesr).size[0] + 1][j][k] = -2;
    }

  // y = 0 and y = 1
  for (i = 1; i <= (*pTesr).size[0]; i++)
    for (k = 1; k <= (*pTesr).size[2]; k++)
    {
      (*pTesr).VoxCell[i][0][k] = -3;
      (*pTesr).VoxCell[i][(*pTesr).size[1] + 1][k] = -4;
    }

  // z = 0 and z = 1
  for (i = 1; i <= (*pTesr).size[0]; i++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
    {
      (*pTesr).VoxCell[i][j][0] = -5;
      (*pTesr).VoxCell[i][j][(*pTesr).size[2] + 1] = -6;
    }

  return;
}

void
nem_reconstruct_tesr_tesr (struct TESR Tesr, struct TESS *pTess,
                           struct NODES *pNodes, struct MESH *Mesh,
                           struct NSET *NSet)
{
  int i, j, k, elt;
  double *dsize = ut_alloc_1d (3);

  for (i = 0; i < Tesr.Dim; i++)
    dsize[i] = Tesr.size[i] * Tesr.vsize[i];

  neut_mesh_str (Tesr.Dim, Tesr.size, pNodes, Mesh + Tesr.Dim,
                 NSet + Tesr.Dim - 1);
  neut_nodes_scale (pNodes, dsize[0], dsize[1], dsize[2]);

  elt = 0;
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        Mesh[Tesr.Dim].EltElset[++elt] = Tesr.VoxCell[i][j][k];

  neut_mesh_init_elsets (Mesh + Tesr.Dim);

  neut_tess_set_zero (pTess);
  nem_reconstruct_mesh ("all", pNodes, Mesh, pTess);

  neut_mesh_reset (Mesh + Tesr.Dim);

  ut_free_1d (&dsize);

  return;
}
