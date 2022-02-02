/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_geof_.h"

void
neut_mesh_fprintf_geof (FILE * file, struct NODES Nodes, struct MESH Mesh1D,
                        struct MESH Mesh2D, struct MESH Mesh3D,
                        struct MESH MeshCo, struct NSET NSet0D,
                        struct NSET NSet1D, struct NSET NSet2D, char *nsets,
                        char *fasets, char *dim,
                        struct BOUNDARY Bound)
{
  int i, printdim, printliset = 0;
  int *shift = ut_alloc_1d_int (5);
  char *printdimstring = ut_alloc_1d_char (2);

  printdim = 0;
  for (i = 3; i >= 0; i--)
  {
    sprintf (printdimstring, "%d", i);
    if (ut_list_testelt (dim, NEUT_SEP_NODEP, printdimstring))
    {
      printdim = i;
      break;
    }
  }

  printliset = (printdim > 1 && ut_list_testelt (dim, NEUT_SEP_NODEP, "1"));

  if (strlen (dim) > 1)
    ut_print_message (0, 3,
                      "Only the %dD mesh will be written in the geof file.\n",
                      printdim);

  if (printliset)
    ut_print_message (0, 3, "The 1D mesh will be written as lisets.\n");

  neut_mesh_fprintf_geof_head (file);

  neut_mesh_fprintf_geof_nodes (file, Nodes);

  neut_mesh_fprintf_geof_elts (file, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                               printdimstring, shift);

  neut_mesh_fprintf_geof_sethead (file);

  neut_mesh_fprintf_geof_nsets (file, NSet0D, NSet1D, NSet2D, nsets);

  if (fasets != NULL && strlen (fasets) > 0 && strcmp (fasets, "none"))
  {
    if (Mesh3D.EltQty == 0)
      neut_mesh_fprintf_geof_lisets (file, Mesh2D, Bound, fasets);
    else
      neut_mesh_fprintf_geof_fasets (file, Mesh3D, Bound, fasets);
  }

  if (printliset)
    neut_mesh_fprintf_geof_lisets_all (file, Mesh1D);

  neut_mesh_fprintf_geof_elsets (file, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                                 printdimstring, shift);

  neut_mesh_fprintf_geof_foot (file);

  ut_free_1d_int (&shift);
  ut_free_1d_char (&printdimstring);

  return;
}
