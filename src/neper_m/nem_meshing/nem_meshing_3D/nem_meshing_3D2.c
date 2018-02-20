/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_3D_.h"

int
nem_meshing_3D_poly (struct IN_M In, double cl, struct MULTIM *pMultim,
		     struct timeval *pctrlc_t, double *pallowed_t,
		     double *pmax_elapsed_t, struct TESS Tess,
		     struct NODES *pNodes, struct MESH *Mesh, int poly)
{
  int a;
  double elapsed_t, mOsize;
  struct NODES N, N2;
  struct MESH M, M2;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);

  (*pMultim).Oalgo[poly] = -1;
  for (a = 0; a < (*pMultim).algoqty; a++)
  {
    // meshing
    nem_meshing_3D_poly_algo (In, cl, pMultim, a, pctrlc_t, pallowed_t,
			      pmax_elapsed_t, Tess, *pNodes, Mesh, poly,
			      &N2, &M2, &mOsize, &elapsed_t);

    // best-quality mesh, recording it
    if ((*pMultim).Oalgo[poly] == a)
    {
      neut_nodes_memcpy (N2, &N);
      neut_mesh_memcpy (M2, &M);
    }

    // minimum quality reached; breaking
    if ((*pMultim).mO[poly][a] > In.meshqualmin)
      break;
  }

  if ((*pMultim).Oalgo[poly] != -1)
    (*pMultim).algohit[(*pMultim).Oalgo[poly]]++;
  else
    ut_print_message (2, 3, "Meshing of poly %d failed\n", poly);

  // Recording poly mesh in global mesh
  nem_meshing_3D_poly_record (Tess, poly, N, M, pNodes, Mesh);

  neut_nodes_free (&N);
  neut_nodes_free (&N2);
  neut_mesh_free (&M);
  neut_mesh_free (&M2);

  return 0;
}

void
nem_meshing_3D_progress (struct MULTIM Multim, int poly, int polyqty,
			 char *message)
{
  int a;
  int *pct = ut_alloc_1d_int (Multim.algoqty);
  char *tmp = ut_alloc_1d_char (128);
  char *format = ut_alloc_1d_char (128);

  sprintf (format, "%%3.0f%%%% (%.2g|%.2g/",
	   (poly > 0) ? ut_array_1d_min (Multim.O + 1, poly) : 0,
	   (poly > 0) ? ut_array_1d_mean (Multim.O + 1, poly) : 0);

  if (poly > 0)
    ut_array_1d_int_percent (Multim.algohit, Multim.algoqty, pct);

  for (a = 0; a < Multim.algoqty; a++)
  {
    sprintf (tmp, "%s%2d%%%c", format, pct[a],
	     (a < Multim.algoqty - 1) ? '|' : ')');
    strcpy (format, tmp);
  }

  ut_print_progress (stdout, poly, polyqty, format, message);

  ut_free_1d_int (pct);
  ut_free_1d_char (format);
  ut_free_1d_char (tmp);

  return;
}
