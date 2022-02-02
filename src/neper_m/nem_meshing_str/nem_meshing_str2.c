/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_meshing_str_.h"

void
nem_meshing_str_tess (struct TESS Tess, struct NODES *pNodes,
                      struct MESH *Mesh)
{
  int i, elt, sum;
  char *progress = ut_alloc_1d_char (100);

  ut_print_progress (stdout, 0, Mesh[Tess.Dim].EltQty, "%3.0f%%", progress);

  sum = 0;
#pragma omp parallel for schedule(dynamic) private(elt)
  for (elt = 1; elt <= Mesh[Tess.Dim].EltQty; elt++)
  {
    int prevpid = 0, *prevpids = NULL, prevpidqty = 0, pid;
    double *coo = ut_alloc_1d (3);

    neut_mesh_elt_centre (*pNodes, Mesh[Tess.Dim], elt, coo);

    Mesh[Tess.Dim].EltElset[elt] = 0;

    // testing prevpid
    if (prevpid > 0 && neut_tess_point_incell (Tess, coo, prevpid) == 1)
      Mesh[Tess.Dim].EltElset[elt] = prevpid;

    // testing prevpids
    if (!Mesh[Tess.Dim].EltElset[elt] && prevpidqty)
    {
      for (i = 0; i < prevpidqty; i++)
        if (neut_tess_point_incell (Tess, coo, prevpids[i]) == 1)
        {
          Mesh[Tess.Dim].EltElset[elt] = prevpids[i];
          prevpid = prevpids[i];
          break;
        }
    }

    // testing other ids
    if (!Mesh[Tess.Dim].EltElset[elt])
    {
      for (pid = 1; pid <= Tess.CellQty; pid++)
        if (pid != prevpid && ut_array_1d_int_eltpos (prevpids, prevpidqty, pid) == -1)
          if (neut_tess_point_incell (Tess, coo, pid) == 1)
          {
            Mesh[Tess.Dim].EltElset[elt] = pid;
            prevpid = pid;
            ut_array_1d_int_list_addval (&prevpids, &prevpidqty, pid);
            break;
          }
    }

#pragma omp critical
    sum++;

#pragma omp critical
    ut_print_progress (stdout, sum, Mesh[Tess.Dim].EltQty, "%3.0f%%",
                       progress);

    ut_free_1d (&coo);
    ut_free_1d_int (&prevpids);
  }

  neut_mesh_init_elsets (Mesh + Tess.Dim);

  neut_mesh_rmelset (Mesh + Tess.Dim, *pNodes, 0);
  neut_nodes_rmorphans (pNodes, Mesh + Tess.Dim, NULL);

  ut_free_1d_char (&progress);

  return;
}

void
nem_meshing_str_meshcell (char *meshcell, struct TESR *pTesr, struct MESH *Mesh,
                          struct NODES *pNodes, struct NSET *NSet)
{
  int i, cellqty;
  int *cell = NULL;

  neut_tesr_expr_cells (pTesr, meshcell, &cell, &cellqty);

  for (i = 1; i <= (*pTesr).CellQty; i++)
    if (ut_array_1d_int_eltpos (cell, cellqty, i) == -1)
      neut_mesh_rmelset (Mesh + 3, *pNodes, i);

  neut_nodes_rmorphans (pNodes, Mesh + 3, NSet + 2);

  ut_free_1d_int (&cell);

  return;
}
