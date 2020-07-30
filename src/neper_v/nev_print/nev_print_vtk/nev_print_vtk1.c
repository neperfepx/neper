/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_vtk_.h"

void
nev_print_vtk (char *basename, struct PRINT *pPrint,
               struct SIM Sim, struct NODES Nodes, struct MESH *Mesh,
               struct DATA *pNodeData, struct DATA *MeshData)
{
  int i;
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

  if ((*pNodeData).CooData)
  {
    fprintf (file, "POINT_DATA %d\n", (*pNodeData).Qty);
    fprintf (file, "VECTORS %s double\n", (*pNodeData).CooDataName);
    ut_array_2d_fprintf (file, (*pNodeData).Coo + 1, (*pNodeData).Qty, 3, "%g");
  }

  if (MeshData[3].ColDataType)
    fprintf (file, "CELL_DATA %d\n", MeshData[3].Qty);

  if (!ut_string_strcmp (MeshData[3].ColDataType, "int"))
  {
    fprintf (file, "SCALARS %s int\n", MeshData[3].ColDataName);
    fprintf (file, "LOOKUP_TABLE default\n");
    ut_array_2d_fprintf (file, MeshData[3].ColData + 1, MeshData[3].Qty, 1, "%.0f");
  }
  else if (!ut_string_strcmp (MeshData[3].ColDataType, "real"))
  {
    fprintf (file, "SCALARS %s double\n", MeshData[3].ColDataName);
    fprintf (file, "LOOKUP_TABLE default\n");
    ut_array_2d_fprintf (file, MeshData[3].ColData + 1, MeshData[3].Qty, 1, "%g");
  }
  else if (!ut_string_strcmp (MeshData[3].ColDataType, "vector"))
  {
    if (MeshData[3].ColDataSize == 3)
    {
      fprintf (file, "VECTORS %s double\n", MeshData[3].ColDataName);

      ut_array_2d_fprintf (file, MeshData[3].ColData + 1, MeshData[3].Qty, MeshData[3].ColDataSize, "%g");
    }
    else
    {
      fprintf (file, "FIELD %s 1\n", MeshData[3].ColDataName);
      fprintf (file, "%s %d %d double\n", MeshData[3].ColDataName,
                                          MeshData[3].ColDataSize,
                                          MeshData[3].Qty);

      ut_array_2d_fprintf (file, MeshData[3].ColData + 1, MeshData[3].Qty, MeshData[3].ColDataSize, "%g");
    }
  }
  else if (!ut_string_strcmp (MeshData[3].ColDataType, "tensor"))
  {
    fprintf (file, "TENSORS %s double\n", MeshData[3].ColDataName);

    if (MeshData[3].ColDataSize == 9) // all components
      ut_array_2d_fprintf (file, MeshData[3].ColData + 1, MeshData[3].Qty, MeshData[3].ColDataSize, "%g");

    else if (MeshData[3].ColDataSize == 6) // Voigt convention
      for (i = 1; i <= MeshData[3].Qty; i++)
        fprintf (file, "%g %g %g %g %g %g %g %g %g\n",
                 MeshData[3].ColData[i][0], MeshData[3].ColData[i][5],
                 MeshData[3].ColData[i][4], MeshData[3].ColData[i][5],
                 MeshData[3].ColData[i][1], MeshData[3].ColData[i][3],
                 MeshData[3].ColData[i][4], MeshData[3].ColData[i][3],
                 MeshData[3].ColData[i][2]);
  }

  ut_file_close (file, filename, "w");

  ut_free_1d_char (&filename);
  ut_free_1d_char (&outdir);

  return;
}
