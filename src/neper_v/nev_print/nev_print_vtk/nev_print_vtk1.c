/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_vtk_.h"

void
nev_print_vtk (char *basename, struct PRINT *pPrint,
               struct SIM Sim, struct NODES Nodes, struct MESH *Mesh,
               struct DATA *pNodeData, struct DATA *MeshData)
{
  FILE *file = NULL;
  char *filename = NULL;
  char *outdir = NULL;

  neut_print_outdir (*pPrint, Sim, "vtk", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  ut_print_message (0, 1, "Printing VTK file...\n");

  if (neut_nodes_isvoid (Nodes))
  {
    ut_print_message (1, 3, "Mesh is void.  Skipping...\n");
    return;
  }

  if (!strcmp (outdir, "."))
    filename = ut_string_addextension (basename, "vtk");
  else
    filename = ut_string_paste4 (outdir, "/", basename, ".vtk");

  file = ut_file_open (filename, "w");

  // mesh --------------------------------------------------------------

  ut_print_message (0, 2, "Printing mesh...\n");

  neut_mesh_fprintf_vtk (file, "3", Nodes, Mesh[1], Mesh[2], Mesh[3]);

  ut_print_message (0, 2, "Printing data...\n");

  // node data

  if ((*pNodeData).CooData || (*pNodeData).ColData)
  {
    fprintf (file, "POINT_DATA %d\n", (*pNodeData).Qty);

    if ((*pNodeData).CooData)
      nev_print_vtk_coodata (file, *pNodeData);

    if ((*pNodeData).ColDataType)
      nev_print_vtk_coldata (file, *pNodeData);
   }

  // element data

  if (MeshData[3].ColDataType)
  {
    fprintf (file, "CELL_DATA %d\n", MeshData[3].Qty);
    nev_print_vtk_coldata (file, MeshData[3]);
  }

  ut_file_close (file, filename, "w");

  ut_free_1d_char (&filename);
  ut_free_1d_char (&outdir);

  return;
}
