/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_readmesh_.h"

void
nem_readmesh (char *filename, struct NODES *pNodes, struct MESH *Mesh)
{
  int i;
  FILE *file = NULL;
  char *filetype = NULL;

  char *meshname = NULL;
  char *nodename = NULL;
  char **list = NULL;
  int qty;

  ut_list_break (filename, NEUT_SEP_DEP, &list, &qty);

  meshname = ut_alloc_1d_char (strlen (list[0]) + 1);
  strcpy (meshname, list[0]);

  if (qty == 2)
  {
    nodename = ut_alloc_1d_char (strlen (list[1]) + 1);
    strcpy (nodename, list[1]);
  }

  ut_file_format (meshname, &filetype);

  if (strcmp (filetype, "gmsh:msh") == 0)
  {
    file = ut_file_open (meshname, "r");
    neut_mesh_fscanf_msh (file, pNodes, Mesh, Mesh + 1, Mesh + 2, Mesh + 3,
                          Mesh + 4, NULL);
    ut_file_close (file, meshname, "r");
  }
  else if (strcmp (filetype, "zset:geof") == 0)
  {
    file = ut_file_open (meshname, "r");
    neut_mesh_fscanf_geof (file, pNodes, Mesh + 3);
    ut_file_close (file, meshname, "r");
  }

  // should not be NodeQty below (should be smaller (RAM))
  for (i = 0; i <= 3; i++)
    neut_mesh_init_nodeelts (Mesh + i, (*pNodes).NodeQty);

  if (nodename)
  {
    double *fact = ut_alloc_1d (3);

    if (sscanf (nodename, "scale(%lf,%lf,%lf)", fact, fact + 1, fact + 2) ==
        3)
      neut_nodes_scale (pNodes, fact[0], fact[1], fact[2]);
    else
    {
      file = ut_file_open (nodename, "r");
      ut_array_2d_fscanf (file, (*pNodes).NodeCoo + 1, (*pNodes).NodeQty, 3);
      ut_file_close (file, nodename, "r");
    }

    ut_free_1d (&fact);
  }

  ut_free_1d_char (&filetype);
  ut_free_2d_char (&list, qty);

  return;
}
