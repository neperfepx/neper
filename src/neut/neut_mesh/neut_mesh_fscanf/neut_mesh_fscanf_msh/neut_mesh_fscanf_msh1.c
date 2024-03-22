/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_msh_.h"

int
neut_mesh_fscanf_msh (FILE * file, struct NODES *pNodes, struct MESH *pMesh0D,
                      struct MESH *pMesh1D, struct MESH *pMesh2D,
                      struct MESH *pMesh3D, struct MESH *pMeshCo,
                      int *ptopology)
{
  int status;
  int *node_nbs = NULL;
  char *mode = NULL, *domain = NULL;
  struct MESH *pMesh = NULL;

  neut_nodes_reset (pNodes);
  neut_mesh_free (pMesh0D);
  neut_mesh_free (pMesh1D);
  neut_mesh_free (pMesh2D);
  neut_mesh_free (pMesh3D);
  neut_mesh_free (pMeshCo);

  while (ut_file_nextstring (file, NULL) == 1)
  {
    if (ut_file_nextstring_test (file, "$MeshFormat"))
      neut_mesh_fscanf_msh_head (file, &mode, &domain, ptopology);

    else if (ut_file_nextstring_test (file, "$MeshVersion"))
      neut_mesh_fscanf_msh_version (file);

    else if (ut_file_nextstring_test (file, "$Nodes"))
      neut_mesh_fscanf_msh_nodes (file, mode, pNodes, &node_nbs);

    else if (ut_file_nextstring_test (file, "$Elements"))
    {
      status = neut_mesh_fscanf_msh_elts (file, mode, domain, node_nbs, pMesh0D, pMesh1D, pMesh2D,
                                 pMesh3D, pMeshCo, &pMesh);
      if (status)
        return -1;
    }

    else if (ut_file_nextstring_test (file, "$Periodicity"))
      neut_mesh_fscanf_msh_periodicity (file, pNodes);

    else if (ut_file_nextstring_test (file, "$PhysicalNames"))
      neut_mesh_fscanf_msh_physical (file, pMesh0D, pMesh1D, pMesh2D, pMesh3D);

    else if (ut_file_nextstring_test (file, "$ElsetCrySym"))
      neut_mesh_fscanf_msh_crysym (file, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D, pMeshCo);

    else if (ut_file_nextstring_test (file, "$ElsetOrientations"))
      neut_mesh_fscanf_msh_orientations (file, "elset", pMesh);

    else if (ut_file_nextstring_test (file, "$ElementOrientations"))
      neut_mesh_fscanf_msh_orientations (file, "elt", pMesh);

    else if (ut_file_nextstring_test (file, "$Groups"))
      neut_mesh_fscanf_msh_groups (file, pMesh);

    else if (ut_file_nextstring_test (file, "$NSets"))
      neut_mesh_fscanf_msh_nsets (file);

    else if (ut_file_nextstring_test (file, "$Fasets"))
      neut_mesh_fscanf_msh_fasets (file, pMesh);

    else if (ut_file_nextstring_test (file, "$NodePartitions"))
      neut_mesh_fscanf_msh_nodeparts (file, pNodes);

    else
      ut_print_message (2, 2, "Failed to read msh file\n");
  }

  ut_free_1d_char (&mode);

  return 0;
}

void
neut_mesh_fnscanf_msh (char *name, struct NODES *pNodes, struct MESH *pMesh0D,
                       struct MESH *pMesh1D, struct MESH *pMesh2D,
                       struct MESH *pMesh3D, struct MESH *pMeshCo,
                       int *ptopology, char *mode)
{
  FILE *file = NULL;
  char **list = NULL;
  int qty;

  ut_list_break (name, NEUT_SEP_DEP, &list, &qty);

  file = ut_file_open (list[0], mode ? mode : "r");

  neut_mesh_fscanf_msh (file, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D,
                        pMeshCo, ptopology);

  ut_file_close (file, list[0], mode ? mode : "r");

  if (qty == 2)
  {
    file = ut_file_open (list[1], mode ? mode : "r");
    ut_array_2d_fscanf (file, (*pNodes).NodeCoo + 1, (*pNodes).NodeQty, 3);
    ut_file_close (file, list[1], mode ? mode : "r");
  }

  ut_free_2d_char (&list, qty);

  return;
}
