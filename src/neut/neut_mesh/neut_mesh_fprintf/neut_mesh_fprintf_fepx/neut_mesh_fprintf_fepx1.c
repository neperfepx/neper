/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_fepx_.h"

void
neut_mesh_fprintf_fepx_name (char *body, struct TESS Tess,
			     struct NODES Nodes, struct MESH Mesh2D,
			     struct MESH Mesh3D, struct NSET NSet0D,
			     struct NSET NSet1D, struct NSET NSet2D,
			     char *nset, char *faset, char *version)
{
  FILE *file = NULL;
  int printparms, printmesh, printsurf, printgrain, printbcs, printkocks;
  char *parms, *mesh, *surf, *grain, *bcs, *kocks;

  if (!version || !strcmp (version, "current"))
  {
    printparms = 0;
    printmesh = 1;
    printsurf = 0;
    printgrain = 1;
    printbcs = 1;
    printkocks = 0;
  }
  else if (!strcmp (version, "legacy"))
  {
    printparms = 1;
    printmesh = 1;
    printsurf = 1;
    printgrain = 1;
    printbcs = 1;
    printkocks = 1;
  }
  else
  {
    ut_error_reportbug ();
    abort ();
  }

  parms = ut_string_addextension (body, ".parms");
  mesh = ut_string_addextension (body, ".mesh");
  surf = ut_string_addextension (body, ".surf");
  if (!version || !strcmp (version, "current"))
    grain = ut_string_addextension (body, ".grain");
  else
    grain = ut_string_addextension (body, ".opt");
  bcs = ut_string_addextension (body, ".bcs");
  kocks = ut_string_addextension (body, ".kocks");

  /* fepx1, parms = general parameters */
  if (printparms)
  {
    file = ut_file_open (parms, "w");
    neut_mesh_fprintf_fepx_parms (file, Nodes, Mesh3D);
    ut_file_close (file, parms, "w");
  }

  /* fepx2, mesh = connectivity and geometry */
  if (printmesh)
  {
    file = ut_file_open (mesh, "w");
    if (version && !strcmp (version, "legacy"))
    {
      neut_mesh_fprintf_fepx_elts (file, Tess, NSet2D, Mesh3D);
      fprintf (file, "  1.0   1.0   1.0\n");
      neut_mesh_fprintf_fepx_nodes (file, Nodes);
    }
    else
    {
      char *fasetexpand = NULL;
      neut_nset_expand (NSet0D, NSet1D, NSet2D, faset, &fasetexpand);
      neut_mesh_fprintf_fepx_parms (file, Nodes, Mesh3D);
      neut_mesh_fprintf_fepx_elts (file, Tess, NSet2D, Mesh3D);
      neut_mesh_fprintf_fepx_nodes (file, Nodes);
      neut_mesh_fprintf_fepx_skinelts (file, Tess, Mesh2D, Mesh3D, Nodes,
				       fasetexpand);
      ut_free_1d_char (fasetexpand);
    }

    ut_file_close (file, mesh, "w");
  }

  /* fepx3, surf = connectivity at the surface (2D elements) */
  if (printsurf)
  {
    char *fasetexpand = NULL;
    neut_nset_expand (NSet0D, NSet1D, NSet2D, faset, &fasetexpand);
    file = ut_file_open (surf, "w");
    neut_mesh_fprintf_fepx_skinelts (file, Tess, Mesh2D, Mesh3D, Nodes,
				     fasetexpand);
    ut_file_close (file, surf, "w");
    ut_free_1d_char (fasetexpand);
  }

  /* fepx4, grain = element sets */
  if (printgrain)
  {
    file = ut_file_open (grain, "w");
    neut_mesh_fprintf_fepx_elsets (file, Mesh3D, version);
    ut_file_close (file, grain, "w");
  }

  /* fepx5, bcs = node sets */
  if (printbcs)
  {
    file = ut_file_open (bcs, "w");
    neut_mesh_fprintf_fepx_nsets (file, NSet0D, NSet1D, NSet2D, nset, version);
    ut_file_close (file, bcs, "w");
  }

  /* kocks = node sets */
  if (printkocks)
  {
    if (Tess.CellOri)
    {
      file = ut_file_open (kocks, "w");
      neut_mesh_fprintf_fepx_kocks (file, Tess, version);
      ut_file_close (file, kocks, "w");
    }
    else
      ut_print_message (1, 3, "Skipping kocks file (CellOri not defined)...\n");
  }

  return;
}
