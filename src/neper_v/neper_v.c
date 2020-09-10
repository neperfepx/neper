/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_v_.h"

int
neper_v (int fargc, char **fargv, int argc, char **argv)
{
  int i, dim, type;
  struct PRINT Print;

  // Simulation
  struct SIM Sim;

  // Tessellation
  struct TESS Tess;
  struct TESR Tesr;

  // Mesh
  struct NODES Nodes;
  struct MESH *Mesh = calloc (5, sizeof (MESH));

  // Point
  struct POINT Point;

  // Datas
  struct DATA *TessData = calloc (5, sizeof (DATA)); // ver, edge, face, poly, seed
  struct DATA TesrData;
  struct DATA NodeData;
  struct DATA *MeshData = calloc (5, sizeof (DATA));
  struct DATA CsysData;
  struct DATA PointData;

  int *expargcs = ut_alloc_1d_int (2);
  char ***expargvs = ut_alloc_1d_ppchar (2);

  ut_arg_expand (fargc, fargv, &(expargcs[0]), &(expargvs[0]));
  ut_arg_expand (argc, argv, &(expargcs[1]), &(expargvs[1]));

  // init general stuff ---
  neut_print_set_default (&Print);
  neut_sim_set_zero (&Sim);
  neut_tess_set_zero (&Tess);
  neut_tesr_set_zero (&Tesr);
  neut_nodes_set_zero (&Nodes);
  neut_point_set_zero (&Point);
  neut_data_set_default (&TesrData);
  for (dim = 0; dim <= 4; dim++)
  {
    neut_mesh_set_zero (Mesh + dim);
    neut_data_set_default (MeshData + dim);
  }
  for (dim = 0; dim <= 4; dim++)
    neut_data_set_default (TessData + dim);

  neut_data_set_default (&NodeData);
  neut_data_set_default (&CsysData);
  neut_data_set_default (&PointData);

  // Printing module header --------------------------------------------
  ut_print_moduleheader ("-V", fargc, fargv, argc, argv);

  for (type = 0; type < 2; type++)
  {
    int expargc = expargcs[type];
    char **expargv = expargvs[type];

    for (i = 1; i < expargc; i++)
    {
      // tessellation / mesh loading -------------------------------------
      if (expargv[i][0] != '-')
        nev_load (expargv[i], &Sim, &Tess, &Tesr, &Nodes, Mesh, &Point,
                  TessData, &TesrData, &NodeData, MeshData, &PointData);

      // data loading and settings ---------------------------------------
      else if (!strncmp (expargv[i], "-data", 5))
        nev_data (expargv, &i, Sim, &Tess, &Tesr, &Nodes, &Mesh, &Point, TessData, &TesrData,
                  &NodeData, MeshData, &CsysData, &PointData);

      // show settings ---------------------------------------------------
      else if (!strncmp (expargv[i], "-show", 5))
        nev_show (expargv, &i, Tess, Tesr, Nodes, Mesh, Point, &Print);

      // simulation settings ---------------------------------------------
      else if (!strncmp (expargv[i], "-sim", 4))
        nev_sim (expargv, &i, &Sim);

      // space settings --------------------------------------------------
      else if (!strncmp (expargv[i], "-space", 6))
        nev_space (expargv, &i, &PointData);

      // output settings -------------------------------------------------
      else if (!strncmp (expargv[i], "-out", 4))
        nev_outdir (expargv, &i, &Print);

      // slicing ---------------------------------------------------------
      else if (!strncmp (expargv[i], "-slice", 5))
        nev_slice (expargv, &i, &Print);

      // camera settings -------------------------------------------------
      else if (!strncmp (expargv[i], "-camera", 7))
        nev_camera (expargv, &i, &Print);

      // image settings --------------------------------------------------
      else if (!strncmp (expargv[i], "-image", 6))
        nev_image (expargv, &i, &Print);

      // include settings ------------------------------------------------
      else if (!strncmp (expargv[i], "-include", 8))
        nev_include (expargv, &i, &Print);

      // povray settings -------------------------------------------------
      else if (!strcmp (expargv[i], "-povray"))
        nev_povray (expargv, &i, &Print);

      // image printing --------------------------------------------------
      else if (!strcmp (expargv[i], "-print"))
        nev_print (expargv, &i, &Print, Sim, Tess, TessData, Tesr, &TesrData, Nodes,
                   Mesh, &NodeData, MeshData, &CsysData, Point, &PointData);

      else
        ut_print_message (2, 0, "Unknown option %s (i = %d)\n", expargv[i],
                          i);
    }
  }

  neut_print_free (&Print);
  neut_sim_free (&Sim);
  neut_tess_free (&Tess);
  neut_nodes_free (&Nodes);
  for (dim = 0; dim <= 4; dim++)
  {
    neut_mesh_free (Mesh + dim);
    neut_data_free (MeshData + dim);
  }
  for (dim = 0; dim <= 4; dim++)
    neut_data_free (TessData + dim);
  neut_data_free (&NodeData);

  // ut_free_2d_char (&expargv, argc);

  return 0;
}
