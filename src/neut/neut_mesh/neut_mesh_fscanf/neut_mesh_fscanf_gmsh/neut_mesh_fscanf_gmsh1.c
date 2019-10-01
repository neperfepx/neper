/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_gmsh_.h"

void
neut_mesh_fscanf_msh (FILE * file, struct NODES *pNodes, struct MESH
		      *pMesh0D, struct MESH *pMesh1D, struct MESH *pMesh2D,
		      struct MESH *pMesh3D, struct MESH *pMeshCo)
{
  int *node_nbs = NULL;
  int leftelts;
  struct MESH Trash;
  int status, contiguous;
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
  leftelts -= ReadMeshOfDim (file, mode, pMeshCo ? pMeshCo : &Trash, node_nbs, 3, leftelts);

  neut_mesh_free (&Trash);

  ut_free_1d_int (node_nbs);

  if (leftelts != 0)
    ut_print_message (2, 0, "Reading of mesh file failed! (Element quantity does not match (leftelts = %d.)\n", leftelts);

  ReadEltsFoot (file);

  int i, dim, id, qty;
  char *string = ut_alloc_1d_char (1000);

  if (pMesh0D)
  {
    (*pMesh0D).ElsetLabels = ut_alloc_2d_char ((*pMesh0D).ElsetQty + 1, 100);
    for (i = 1; i <= (*pMesh0D).ElsetQty; i++)
      sprintf ((*pMesh0D).ElsetLabels[i], "ver%d", i);
  }

  if (pMesh1D)
  {
    (*pMesh1D).ElsetLabels = ut_alloc_2d_char ((*pMesh1D).ElsetQty + 1, 100);
    for (i = 1; i <= (*pMesh1D).ElsetQty; i++)
      sprintf ((*pMesh1D).ElsetLabels[i], "edge%d", i);
  }

  if (pMesh2D)
  {
    (*pMesh2D).ElsetLabels = ut_alloc_2d_char ((*pMesh2D).ElsetQty + 1, 100);
    for (i = 1; i <= (*pMesh2D).ElsetQty; i++)
      sprintf ((*pMesh2D).ElsetLabels[i], "face%d", i);
  }

  if (pMesh3D)
  {
    (*pMesh3D).ElsetLabels = ut_alloc_2d_char ((*pMesh3D).ElsetQty + 1, 100);
    for (i = 1; i <= (*pMesh3D).ElsetQty; i++)
      sprintf ((*pMesh3D).ElsetLabels[i], "poly%d", i);
  }

  ut_file_nextstring (file, string);

  if (!strcmp (string, "$PhysicalNames"))
  {
    ut_file_skip (file, 1);
    if (fscanf (file, "%d", &qty) != 1)
      abort ();

    for (i = 1; i <= qty; i++)
    {
      if (fscanf (file, "%d%d", &dim, &id) != 2)
        abort ();

      if (dim == 0)
        status = fscanf (file, "%s", pMesh0D ? (*pMesh0D).ElsetLabels[id] : string);
      else if (dim == 1)
        status = fscanf (file, "%s", pMesh1D ? (*pMesh1D).ElsetLabels[id] : string);
      else if (dim == 2)
        status = fscanf (file, "%s", pMesh2D ? (*pMesh2D).ElsetLabels[id] : string);
      else if (dim == 3)
        status = fscanf (file, "%s", pMesh3D ? (*pMesh3D).ElsetLabels[id] : string);

      if (status != 1)
        abort ();
    }
  }

  ut_free_1d_char (string);
  ut_free_1d_char (mode);

  return;
}

void
neut_mesh_name_fscanf_msh (char *name, struct NODES *pNodes, struct MESH
			   *pMesh0D, struct MESH *pMesh1D,
			   struct MESH *pMesh2D, struct MESH *pMesh3D,
                           struct MESH *pMeshCo)
{
  FILE *file = NULL;
  char **list = NULL;
  int qty;

  ut_string_separate (name, NEUT_SEP_DEP, &list, &qty);

  file = ut_file_open (list[0], "r");

  neut_mesh_fscanf_msh (file, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D, pMeshCo);

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
