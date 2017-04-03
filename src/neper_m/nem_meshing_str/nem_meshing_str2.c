/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_meshing_str_.h"

void
nem_meshing_str_tess (struct TESS Tess, struct NODES *pNodes,
		      struct MESH *Mesh)
{
  int elt, pid, prevpid;
  char *progress = ut_alloc_1d_char (100);
  double *coo = ut_alloc_1d (3);

  ut_print_progress (stdout, 0, Mesh[Tess.Dim].EltQty, "%3.0f%%", progress);
  prevpid = 1;
  for (elt = 1; elt <= Mesh[Tess.Dim].EltQty; elt++)
  {
    neut_mesh_elt_centre (*pNodes, Mesh[Tess.Dim], elt, coo);
    if (neut_tess_point_incell (Tess, coo, prevpid) == 1)
      Mesh[Tess.Dim].EltElset[elt] = prevpid;
    else
      for (pid = 1; pid <= Tess.CellQty; pid++)
      {
	if (pid == prevpid)
	  continue;

	if (neut_tess_point_incell (Tess, coo, pid) == 1)
	{
	  Mesh[Tess.Dim].EltElset[elt] = pid;
	  prevpid = pid;
	  break;
	}
      }

    /* print percent evolution in case of verbosity */
    ut_print_progress (stdout, elt, Mesh[Tess.Dim].EltQty, "%3.0f%%",
		       progress);
  }

  neut_mesh_init_elsets (Mesh + Tess.Dim);

  neut_mesh_rmelset (Mesh + Tess.Dim, *pNodes, 0);
  neut_nodes_rmorphans (pNodes, Mesh + Tess.Dim, NULL);

  ut_free_1d_char (progress);
  ut_free_1d (coo);

  return;
}

void
nem_meshing_str_meshcell (char *meshcell, struct TESR Tesr, struct MESH *Mesh,
			  struct NODES *pNodes, struct NSET *NSet)
{
  int i, cellqty;
  int *cell = NULL;

  neut_tesr_expr_cells (Tesr, meshcell, &cell, &cellqty);

  for (i = 1; i <= Tesr.CellQty; i++)
    if (ut_array_1d_int_eltpos (cell, cellqty, i) == -1)
      neut_mesh_rmelset (Mesh + 3, *pNodes, i);

  neut_nodes_rmorphans (pNodes, Mesh + 3, NSet + 2);

  ut_free_1d_int (cell);

  return;
}
