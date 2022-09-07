/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_v_.h"

int
neper_v (int fargc, char **fargv, int argc, char **argv)
{
  int i, type;
  struct PRINT Print;

  // Simulation
  struct SIM Sim;

  // Tessellation
  struct TESS Tess;
  struct TESR Tesr;

  // Mesh
  struct NODES Nodes;
  struct MESH *Mesh = calloc (5, sizeof (MESH));

  // Points
  int PointQty = 0;
  struct POINT *Points = NULL;

  // Datas
  struct DATA *TessData = calloc (6, sizeof (DATA)); // ver, edge, face, poly, seed, crystal
  struct DATA *TesrData = calloc (2, sizeof (DATA)); // cell, vox
  struct DATA NodeData;
  struct DATA *MeshData = calloc (5, sizeof (DATA));
  struct DATA CsysData;
  struct DATA *PointData = NULL;

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
  // neut_point_set_zero (&Points);
  for (i = 0; i <= 4; i++)
  {
    neut_mesh_set_zero (Mesh + i);
    neut_data_set_default (MeshData + i);
  }
  for (i = 0; i <= 5; i++)
    neut_data_set_default (TessData + i);
  for (i = 0; i <= 1; i++)
    neut_data_set_default (TesrData + i);

  neut_data_set_default (&NodeData);
  neut_data_set_default (&CsysData);
  // neut_data_set_default (&PointData);

  // Printing module header --------------------------------------------
  ut_print_moduleheader ("-V", fargc, fargv, argc, argv);

  for (type = 0; type < 2; type++)
  {
    int expargc = expargcs[type];
    char **expargv = expargvs[type];

    for (i = 1; i < expargc; i++)
    {
      nev_fixarg (expargv + i);

      // tessellation / mesh loading -------------------------------------
      if (expargv[i][0] != '-')
        nev_load (expargv[i], &Sim, &Tess, &Tesr, &Nodes, Mesh, &Points,
                  &PointQty, TessData, TesrData, &NodeData, MeshData,
                  &PointData);

      // data loading and settings ---------------------------------------
      else if (!strncmp (expargv[i], "-data", 5))
        nev_data (expargv, &i, Sim, &Tess, &Tesr, &Nodes, &Mesh, &Points,
                  PointQty, TessData, TesrData, &NodeData, MeshData,
                  &CsysData, &PointData);

      // show settings ---------------------------------------------------
      else if (!strncmp (expargv[i], "-show", 5))
        nev_show (expargv, &i, Tess, Tesr, Nodes, Mesh, Points, PointQty, &Print);

      // simulation settings ---------------------------------------------
      else if (!strncmp (expargv[i], "-sim", 4) || !strncmp (expargv[i], "-step", 5))
        nev_sim (expargv, &i, &Sim);

      // space settings --------------------------------------------------
      else if (!strncmp (expargv[i], "-space", 6))
        nev_space (expargv, &i, &Print);

      // pf settings -----------------------------------------------------
      else if (!strncmp (expargv[i], "-pf", 3))
        nev_pf (expargv, &i, &Print);

      // output settings -------------------------------------------------
      else if (!strncmp (expargv[i], "-out", 4))
        nev_outdir (expargv, &i, &Print);

      // slicing ---------------------------------------------------------
      else if (!strncmp (expargv[i], "-slice", 5))
        nev_slice (expargv, &i, &Print);

      // camera settings -------------------------------------------------
      else if (!strncmp (expargv[i], "-camera", 7))
        nev_camera (expargv, &i, &Print);

      // light settings -------------------------------------------------
      else if (!strncmp (expargv[i], "-light", 6))
        nev_light (expargv, &i, &Print);

      // image settings --------------------------------------------------
      else if (!strncmp (expargv[i], "-image", 6))
        nev_image (expargv, &i, &Print);

      // image settings --------------------------------------------------
      else if (!strncmp (expargv[i], "-scene", 6))
        nev_scene (expargv, &i, &Print);

      // include settings ------------------------------------------------
      else if (!strncmp (expargv[i], "-include", 8))
        nev_include (expargv, &i, &Print);

      // povray settings -------------------------------------------------
      else if (!strncmp (expargv[i], "-povray", 7))
        nev_povray (expargv, &i, &Print);

      // povray settings -------------------------------------------------
      else if (!strcmp (expargv[i], "-asymptote"))
        nev_asymptote (expargv, &i, &Print);

      // image printing --------------------------------------------------
      else if (!strcmp (expargv[i], "-print"))
        nev_print (expargv, &i, &Print, Sim, Tess, TessData, Tesr, TesrData, Nodes,
                   Mesh, &NodeData, MeshData, &CsysData, Points, PointQty, PointData);

      else
        ut_print_message (2, 0, "Unknown option %s (i = %d)\n", expargv[i],
                          i);
    }
  }

  neut_print_free (&Print);
  neut_sim_free (&Sim);
  neut_tess_free (&Tess);
  neut_nodes_free (&Nodes);
  for (i = 0; i <= 4; i++)
  {
    neut_mesh_free (Mesh + i);
    neut_data_free (MeshData + i);
  }
  for (i = 0; i <= 5; i++)
    neut_data_free (TessData + i);
  neut_data_free (&NodeData);

  // ut_free_2d_char (&expargv, argc);

  return 0;
}
