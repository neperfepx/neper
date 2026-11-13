/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nem_readmesh_.h"

// Returns 1 if the nsets of the mesh are known (recorded in NSet, possibly
// none), and 0 otherwise
int
nem_readmesh (char *filename, struct NODES *pNodes, struct MESH *Mesh, struct NSET *NSet,
              int *ptopology)
{
  int i, nsetstatus = 0;
  struct NSET NSetIn;
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
    // qty = -1 if the file contains no nsets
    neut_nset_set_zero (&NSetIn);
    NSetIn.qty = -1;
    neut_mesh_fscanf_msh (file, pNodes, Mesh, Mesh + 1, Mesh + 2, Mesh + 3,
                          Mesh + 4, Mesh + 5, NSet ? &NSetIn : NULL, ptopology);
    ut_file_close (file, meshname, "r");

    if (NSet && NSetIn.qty >= 0)
    {
      nsetstatus = 1;
      nem_readmesh_nsets (NSetIn, neut_mesh_array_dim (Mesh), NSet);
      neut_nset_rmempty (NSet);
      neut_nset_free (&NSetIn);
    }

    // no nsets in a periodic mesh: no boundary, hence no nsets (a mesh with
    // a boundary has nsets)
    else if (NSet && (*pNodes).PerNodeQty > 0)
      nsetstatus = 1;
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
  ut_free_1d_char (&meshname);
  ut_free_1d_char (&nodename);
  ut_free_2d_char (&list, qty);

  return nsetstatus;
}
