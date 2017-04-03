/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_gmsh_.h"

void
neut_mesh_fscanf_msh (FILE * file, struct NODES *pNodes, struct MESH
		      *pMesh0D, struct MESH *pMesh1D, struct MESH *pMesh2D,
		      struct MESH *pMesh3D)
{
  int *node_nbs = NULL;
  int leftelts;
  struct MESH Trash;
  int contiguous;
  char *mode = NULL;

  neut_mesh_set_zero (&Trash);
  neut_nodes_free (pNodes);

  neut_mesh_fscanf_gmshHead (file, &contiguous, &mode);
  (*pNodes).NodeQty = ReadNodesHead (file);

  if (contiguous == 0)
  {
    node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
    node_nbs[0] = (*pNodes).NodeQty;
  }
  ReadNodes (file, mode, pNodes, node_nbs);
  // neut_nodes_renumber_continuous (pNodes, node_nbs, &nodes_old_new);

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", &leftelts) != 1)
    abort ();

  char c;
  if (!strcmp (mode, "binary"))
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  leftelts -= ReadMeshOfDim (file, mode, pMesh0D ? pMesh0D : &Trash, node_nbs, 0, leftelts);
  leftelts -= ReadMeshOfDim (file, mode, pMesh1D ? pMesh1D : &Trash, node_nbs, 1, leftelts);
  leftelts -= ReadMeshOfDim (file, mode, pMesh2D ? pMesh2D : &Trash, node_nbs, 2, leftelts);
  leftelts -= ReadMeshOfDim (file, mode, pMesh3D ? pMesh3D : &Trash, node_nbs, 3, leftelts);

  neut_mesh_free (&Trash);

  ut_free_1d_int (node_nbs);

  if (leftelts != 0)
    ut_print_message (2, 0, "Reading of mesh file failed! (Element quantity does not match (leftelts = %d.)\n", leftelts);

  ReadEltsFoot (file);
  ut_free_1d_char (mode);

  return;
}

void
neut_mesh_name_fscanf_msh (char *name, struct NODES *pNodes, struct MESH
			   *pMesh0D, struct MESH *pMesh1D,
			   struct MESH *pMesh2D, struct MESH *pMesh3D)
{
  FILE *file = NULL;
  char **list = NULL;
  int qty;

  ut_string_separate (name, NEUT_SEP_DEP, &list, &qty);

  file = ut_file_open (list[0], "r");

  neut_mesh_fscanf_msh (file, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D);

  ut_file_close (file, list[0], "r");

  if (qty == 2)
  {
    file = ut_file_open (list[1], "r");
    ut_array_2d_fscanf (file, (*pNodes).NodeCoo + 1, (*pNodes).NodeQty, 3);
    ut_file_close (file, list[1], "r");
  }

  ut_free_2d_char (list, qty);

  return;
}
