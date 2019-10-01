/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_readmesh_.h"

void
nem_readmesh (char *filename, struct NODES *pNodes, struct MESH *Mesh)
{
  int i;
  FILE *file = NULL, *file2 = NULL, *file3 = NULL;
  char *parms = NULL, *mesh = NULL, *opt = NULL;
  char *filetype = NULL;

  char *meshname = NULL;
  char *nodename = NULL;
  char **list = NULL;
  int qty;

  ut_string_separate (filename, NEUT_SEP_DEP, &list, &qty);

  meshname = ut_alloc_1d_char (strlen (list[0]) + 1);
  strcpy (meshname, list[0]);

  if (qty == 2)
  {
    nodename = ut_alloc_1d_char (strlen (list[1]) + 1);
    strcpy (nodename, list[1]);
  }

  ut_file_format (meshname, &filetype);

  if (strcmp (filetype, "gmsh_msh") == 0)
  {
    file = ut_file_open (meshname, "r");
    neut_mesh_fscanf_msh (file, pNodes, Mesh, Mesh + 1, Mesh + 2, Mesh + 3, Mesh + 4);
    ut_file_close (file, meshname, "r");
  }
  else if (strcmp (filetype, "zebulon_geof") == 0)
  {
    file = ut_file_open (meshname, "r");
    neut_mesh_fscanf_geof (file, pNodes, Mesh + 3);
    ut_file_close (file, meshname, "r");
  }
  else if (strcmp (filetype, "fepx") == 0)
  {
    /* initializing file names */
    if (ut_file_exist ("%s.fepx1", meshname))
      parms = ut_string_paste (meshname, ".fepx1");
    else if (ut_file_exist ("%s.parms", meshname))
      parms = ut_string_paste (meshname, ".parms");

    if (parms == NULL)
    {
      ut_print_message (2, 2, "no .fepx1 or .parms file found.\n");
      abort ();
    }

    if (ut_file_exist ("%s.fepx2", meshname))
      mesh = ut_string_paste (meshname, ".fepx2");
    else if (ut_file_exist ("%s.mesh", meshname))
      mesh = ut_string_paste (meshname, ".mesh");

    if (mesh == NULL)
    {
      ut_print_message (2, 2, "no .fepx2 or .mesh file found.\n");
      abort ();
    }

    if (ut_file_exist ("%s.fepx4", meshname))
      opt = ut_string_paste (meshname, ".fepx4");
    else if (ut_file_exist ("%s.opt", meshname))
      opt = ut_string_paste (meshname, ".opt");

    if (opt == NULL)
    {
      ut_print_message (2, 2, "no .fepx4 or .opt file found.\n");
      abort ();
    }

    file = ut_file_open (parms, "r");
    file2 = ut_file_open (mesh, "r");
    file3 = ut_file_open (opt, "r");
    neut_mesh_fscanf_fepx (file, file2, file3, pNodes, Mesh + 3);
    ut_file_close (file, parms, "r");
    ut_file_close (file2, mesh, "r");
    ut_file_close (file3, opt, "r");

    ut_free_1d_char (parms);
    ut_free_1d_char (mesh);
    ut_free_1d_char (opt);

    neut_mesh_init_nodeelts (Mesh + 3, (*pNodes).NodeQty);
    // neut_mesh3d_mesh2d (*pNodes, *pMesh[3], pMesh[2], &Elsets, &ElsetQty);
  }

  // should not be NodeQty below (should be smaller (RAM))
  for (i = 0; i <= 3; i++)
    neut_mesh_init_nodeelts (Mesh + i, (*pNodes).NodeQty);

  if (nodename)
  {
    double *fact = ut_alloc_1d (3);

    if (sscanf (nodename, "scale(%lf,%lf,%lf)", fact, fact + 1, fact + 2) == 3)
      neut_nodes_scale (pNodes, fact[0], fact[1], fact[2]);
    else
    {
      file = ut_file_open (nodename, "r");
      ut_array_2d_fscanf (file, (*pNodes).NodeCoo + 1, (*pNodes).NodeQty, 3);
      ut_file_close (file, nodename, "r");
    }

    ut_free_1d (fact);
  }

  ut_free_1d_char (filetype);
  ut_free_2d_char (list, qty);

  return;
}
