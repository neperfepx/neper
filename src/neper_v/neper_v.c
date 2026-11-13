/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neper_v_.h"

int
neper_v (int fargc, char **fargv, int argc, char **argv)
{
  int i, j, run_id, pos, expargc, qty, PointQty = 0;
  char **expargv = NULL, **tmp = NULL;
  struct IN_V In;
  struct PRINT Print;
  struct SIM Sim;
  struct TESS Tess;
  struct TESR Tesr;
  struct NODES Nodes;
  struct MESH *Mesh = NULL;
  struct NSET NSet;
  struct POINT *Points = NULL;
  struct DATA *TessData = NULL;
  struct DATA *TesrData = NULL;
  struct DATA NodeData;
  struct DATA **MeshData = NULL;
  struct DATA CsysData;
  struct DATA *PointData = NULL;

  Mesh = calloc (5, sizeof (MESH));
  TessData = calloc (6, sizeof (DATA));
  TesrData = calloc (2, sizeof (DATA));
  MeshData = calloc (5, sizeof (DATA*));
  for (i = 0; i < 5; i++)
    MeshData[i] = calloc (3, sizeof (DATA));

  neut_print_set_default (&Print);
  neut_sim_set_zero (&Sim);
  neut_tess_set_zero (&Tess);
  neut_tesr_set_zero (&Tesr);
  neut_nodes_set_zero (&Nodes);
  neut_nset_set_zero (&NSet);
  for (i = 0; i < 5; i++)
    neut_mesh_set_zero (Mesh + i);
  for (i = 0; i < 6; i++)
    neut_data_set_default (TessData + i);
  for (i = 0; i <= 1; i++)
    neut_data_set_default (TesrData + i);
  for (i = 0; i < 5; i++)
    for (j = 0; j < 3; j++)
      neut_data_set_default (MeshData[i] + j);
  neut_data_set_default (&NodeData);
  neut_data_set_default (&CsysData);

  // expanding and concatenating input arguments
  ut_arg_expand (fargc, fargv, &expargc, &expargv);
  ut_arg_expand (argc, argv, &qty, &tmp);
  for (i = 1; i <= qty; i++)
  {
    expargc++;
    expargv = ut_realloc_1d_pchar_null (expargv, expargc + 1, 1);
    ut_string_string (tmp[i], expargv + expargc);
  }

  // Printing module header --------------------------------------------
  ut_print_moduleheader ("-V", fargc, fargv, argc, argv);

  run_id = 0;
  pos = 1;
  while (!nev_input (expargc, expargv, &In, &pos))
  {
    run_id++;

    if (run_id == 1)
      nev_load (In.input, &Sim, &Tess, &Tesr, &Nodes, Mesh, &NSet, &Points,
                &PointQty, TessData, TesrData, &NodeData, MeshData,
                &PointData);

    nev_print (In, &Print, &Sim, &Tess, TessData, Tesr, TesrData, Nodes,
               Mesh, NSet, &NodeData, MeshData, &CsysData, Points, PointQty, PointData);
  }

  nev_input_free (&In);
  if (Print.showvox)
    ut_free_3d_int (&Print.showvox, Tesr.size[0] + 1, Tesr.size[1] + 1);
  neut_print_free (&Print);
  neut_sim_free (&Sim);
  neut_tess_free (&Tess);
  neut_tesr_free (&Tesr);
  neut_nodes_free (&Nodes);
  neut_nset_free (&NSet);
  for (i = 0; i < 5; i++)
    neut_mesh_free (Mesh + i);
  free (Mesh);
  for (i = 0; i < 5; i++)
  {
    for (j = 0; j < 3; j++)
      neut_data_free (MeshData[i] + j);
    free (MeshData[i]);
  }
  free (MeshData);
  for (i = 0; i < 6; i++)
    neut_data_free (TessData + i);
  free (TessData);
  for (i = 0; i <= 1; i++)
    neut_data_free (TesrData + i);
  free (TesrData);
  neut_data_free (&NodeData);
  neut_data_free (&CsysData);
  for (i = 0; i < PointQty; i++)
  {
    neut_point_free (Points + i);
    neut_data_free (PointData + i);
  }
  free (Points);
  free (PointData);

  ut_free_2d_char (&expargv, expargc + 1);
  ut_free_2d_char (&tmp, qty + 1);

  return 0;
}
