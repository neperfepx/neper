/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_3d_.h"

int
nem_meshing_3d_poly (struct IN_M In, double cl, double clgrad, double clreps,
                     struct MULTIM *pMultim, struct timeval *pctrlc_t,
                     double *pallowed_t, double *pmax_elapsed_t,
                     struct TESS Tess, struct NODES *pNodes,
                     struct MESH *Mesh, struct NODES *pN, struct MESH *pM,
                     int poly)
{
  int a;
  double elapsed_t, mOsize;
  struct NODES N2;
  struct MESH M2;

  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);

  neut_nodes_reset (pN);
  neut_mesh_set_zero (pM);

  (*pMultim).Oalgo[poly] = -1;
  for (a = 0; a < (*pMultim).algoqty; a++)
  {
    // meshing
    nem_meshing_3d_poly_algo (In, cl, clgrad, clreps, pMultim, a, pctrlc_t,
                              pallowed_t, pmax_elapsed_t, Tess, *pNodes, Mesh,
                              poly, &N2, &M2, &mOsize, &elapsed_t);

    // best-quality mesh, recording it
    if ((*pMultim).Oalgo[poly] == a)
    {
      neut_nodes_memcpy (N2, pN);
      neut_mesh_memcpy (M2, pM);
    }

    // minimum quality reached; breaking
    if ((*pMultim).mO[poly][a] > In.meshqualmin)
      break;
  }

  if ((*pMultim).Oalgo[poly] != -1)
#pragma omp atomic
    (*pMultim).algohit[(*pMultim).Oalgo[poly]]++;
  else
  {
    FILE *fp = ut_file_open ("failed_polys", "w");
    fprintf (fp, "%d\n", poly);
    ut_file_close (fp, "failed_polys", "w");
    ut_print_message (2, 3, "Meshing of poly %d (%d) failed\n", poly,
                      neut_tess_cell_id (Tess, poly));
  }

  neut_nodes_free (&N2);
  neut_mesh_free (&M2);

  return 0;
}

// nem_meshing_3d_algos returns the list of 3D meshing algorithms: only dela
// is used, as netg does not account for the mesh size map
void
nem_meshing_3d_algos (struct IN_M In, char **plist)
{
  int i, qty, algoqty;
  char **algos = NULL, **algo = NULL;

  if (!strcmp (In.mesh3dalgo, "default"))
  {
    ut_string_string (In.mesh3dalgo, plist);
    return;
  }

  ut_list_break (In.mesh3dalgo, NEUT_SEP_NODEP, &algos, &algoqty);

  (*plist) = ut_alloc_1d_char (strlen (In.mesh3dalgo) + 1);
  for (i = 0; i < algoqty; i++)
  {
    ut_list_break (algos[i], NEUT_SEP_DEP, &algo, &qty);
    if (!strcmp (algo[0], "dela"))
    {
      if (strlen (*plist) > 0)
        strcat (*plist, NEUT_SEP_NODEP);
      strcat (*plist, algos[i]);
    }
    ut_free_2d_char (&algo, qty);
  }

  if (strlen (*plist) == 0)
  {
    printf ("\n");
    ut_print_message (2, 3, "Option -mesh3dalgo must include dela, as netg does not account for the mesh size map.\n");
    abort ();
  }
  else if (strcmp (*plist, In.mesh3dalgo))
  {
    printf ("\n");
    ut_print_message (1, 3, "netg ignored in option -mesh3dalgo, as it does not account for the mesh size map.\n");
  }

  ut_free_2d_char (&algos, algoqty);

  return;
}

void
nem_meshing_3d_progress (struct MULTIM Multim, int poly, int polyqty,
                         char *message)
{
  int i, a;
  double mean;
  int *pct = ut_alloc_1d_int (Multim.algoqty);
  char *tmp = ut_alloc_1d_char (128);
  char *format = ut_alloc_1d_char (128);

  if (poly > 0)
  {
    mean = ut_array_1d_mean (Multim.O + 1, poly);
    for (i = 1; i <= poly; i++)
      if (Multim.O[i] == 0)
        Multim.O[i] = mean;
  }

  sprintf (format, "%%3.0f%%%% (%.2g|%.2g/",
           (poly > 0) ? ut_array_1d_min (Multim.O + 1, poly) : 0,
           (poly > 0) ? ut_array_1d_mean (Multim.O + 1, poly) : 0);

  if (poly > 0)
    ut_stat_scaletopercent_int (Multim.algohit, Multim.algoqty, pct);

  for (a = 0; a < Multim.algoqty; a++)
  {
    sprintf (tmp, "%s%2d%%%c", format, pct[a],
             (a < Multim.algoqty - 1) ? '|' : ')');
    strcpy (format, tmp);
  }

  ut_print_progress (stdout, poly, polyqty, format, message);

  ut_free_1d_int (&pct);
  ut_free_1d_char (&format);
  ut_free_1d_char (&tmp);

  return;
}

void
nem_meshing_3d_poly_record (struct TESS Tess, int poly, struct NODES N,
                            struct MESH M, struct NODES *pNodes,
                            struct MESH *Mesh)
{
  int *node_nbs = NULL;

  nem_meshing_3d_poly_record_nodes (Tess, poly, N, &node_nbs, pNodes, Mesh);

  nem_meshing_3d_poly_record_elts (poly, M, node_nbs, Mesh);

  ut_free_1d_int (&node_nbs);

  return;
}
