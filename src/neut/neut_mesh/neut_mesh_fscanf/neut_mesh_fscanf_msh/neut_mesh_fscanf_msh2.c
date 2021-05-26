/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_msh_.h"

void
neut_mesh_fscanf_msh_head (FILE * file, char **pmode, int *ptopology)
{
  int status, type;
  char string[1000];

  if (ptopology)
    (*ptopology) = 1;

  /* Reading of the 2 first strings, which must be "$MeshFormat" and "2". */
  if (fscanf (file, "%s", string) != 1 || strcmp (string, "$MeshFormat") != 0)
  {
    ut_print_message (2, 0, "Input file is not a msh file!\n");
    ut_print_message (2, 0, "(start word does not match.)\n");
    abort ();
  }

  if (fscanf (file, "%s", string) != 1
      || (strcmp (string, "2") != 0 && strcmp (string, "2.2") != 0))
    ut_print_message (2, 0, "Bad msh file format version detected!\n");

  /* skipping the 2 next ones. */
  status = fscanf (file, "%d", &type);
  if (status != 1)
    abort ();

  if (type == 0)
    ut_string_string ("ascii", pmode);
  else if (type == 1)
    ut_string_string ("binary", pmode);
  else
    abort ();

  ut_file_skip (file, 1);

  // trashing 1 value
  if (!strcmp (*pmode, "binary"))
  {
    status = fread (&type, sizeof (int), 1, file);
    if (status != 1)
      abort ();
    status = fscanf (file, "%s", string);
    if (status != 1)
      abort ();
  }

  status = fscanf (file, "%s", string);
  if (status != 1)
    abort ();

  if (strcmp (string, "$Comments") == 0)
  {
    do
    {
      if (fscanf (file, "%s", string) != 1)
        abort ();
    }
    while (strcmp (string, "$EndComments") != 0);

    do
    {
      if (fscanf (file, "%s", string) != 1)
        abort ();
    }
    while (strcmp (string, "$EndMeshFormat") != 0);
  }
  else if (strcmp (string, "$EndMeshFormat"))
    abort ();

  if (ut_file_nextstring_test (file, "$MeshVersion"))
    ut_file_skip (file, 3);

  if (ut_file_nextstring_test (file, "$Topology"))
  {
    ut_file_skip (file, 1);

    if (ptopology)
      fscanf (file, "%d", ptopology);
    else
      ut_file_skip (file, 1);

    ut_file_skip (file, 1);
  }

  return;
}

void
neut_mesh_fscanf_msh_nodes (FILE * file, char *mode,
                            struct NODES *pNodes, int **pnode_nbs)
{
  neut_mesh_fscanf_msh_nodes_head (file, &(*pNodes).NodeQty);

  (*pnode_nbs) = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pnode_nbs)[0] = (*pNodes).NodeQty;

  neut_mesh_fscanf_msh_nodes_prop (file, mode, pNodes, *pnode_nbs);

  neut_mesh_fscanf_msh_nodes_foot (file);

  return;
}

void
neut_mesh_fscanf_msh_elts (FILE * file, char *mode, int *node_nbs,
                           struct MESH *pMesh0D, struct MESH *pMesh1D,
                           struct MESH *pMesh2D, struct MESH *pMesh3D,
                           struct MESH *pMeshCo, struct MESH **ppMesh)
{
  int leftelts;
  struct MESH Trash;

  neut_mesh_set_zero (&Trash);

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", &leftelts) != 1)
    abort ();

  char c;
  if (!strcmp (mode, "binary"))
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  leftelts -=
    neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh0D ? pMesh0D : &Trash, node_nbs, 0,
                   leftelts);
  leftelts -=
    neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh1D ? pMesh1D : &Trash, node_nbs, 1,
                   leftelts);
  leftelts -=
    neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh2D ? pMesh2D : &Trash, node_nbs, 2,
                   leftelts);
  leftelts -=
    neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh3D ? pMesh3D : &Trash, node_nbs, 3,
                   leftelts);
  leftelts -=
    neut_mesh_fscanf_msh_elts_dim (file, mode, pMeshCo ? pMeshCo : &Trash, node_nbs, 3,
                   leftelts);

  ut_free_1d_int (&node_nbs);

  if (leftelts != 0)
    ut_print_message (2, 0,
                      "Reading of mesh file failed! (Element quantity does not match (leftelts = %d.)\n",
                      leftelts);

  neut_mesh_fscanf_msh_elts_foot (file);

  if (pMesh0D)
    neut_mesh_init_elsetlabels (pMesh0D);
  if (pMesh1D)
    neut_mesh_init_elsetlabels (pMesh1D);
  if (pMesh2D)
    neut_mesh_init_elsetlabels (pMesh2D);
  if (pMesh3D)
    neut_mesh_init_elsetlabels (pMesh3D);

  if (pMesh3D && (*pMesh3D).EltQty > 0)
    *ppMesh = pMesh3D;
  else if (pMesh2D && (*pMesh2D).EltQty > 0)
    *ppMesh = pMesh2D;
  else if (pMesh1D && (*pMesh1D).EltQty > 0)
    *ppMesh = pMesh1D;
  else if (pMesh0D && (*pMesh0D).EltQty > 0)
    *ppMesh = pMesh0D;

  neut_mesh_free (&Trash);

  return;
}

void
neut_mesh_fscanf_msh_elts_foot (FILE * file)
{
  int status;
  char foot[1000];

  status = fscanf (file, "%s", foot);

  if (status != 1 || strcmp (foot, "$EndElements") != 0)
    ut_print_message (2, 0, "Reading msh file: error.\n");

  return;
}

void
neut_mesh_fscanf_msh_periodicity (FILE *file, struct NODES *pNodes)
{
  int i, id;

  // header
  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &(*pNodes).PerNodeQty) != 1)
    abort ();

  (*pNodes).PerNodeNb = ut_alloc_1d_int ((*pNodes).PerNodeQty + 1);
  (*pNodes).PerNodeMaster = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pNodes).PerNodeShift = ut_alloc_2d_int ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).PerNodeQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1)
      abort ();

    (*pNodes).PerNodeNb[i] = id;

    if (fscanf (file, "%d", (*pNodes).PerNodeMaster + id) != 1)
      abort ();

    ut_array_1d_int_fscanf (file, (*pNodes).PerNodeShift[id], 3);
  }

  // footer
  ut_file_skip (file, 1);

  return;
}

void
neut_mesh_fscanf_msh_physical (FILE *file, struct MESH *pMesh0D,
                               struct MESH *pMesh1D, struct MESH *pMesh2D,
                               struct MESH *pMesh3D)
{
  int i, dim, id, qty, status;
  char *string = ut_alloc_1d_char (1000);

  // header
  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  for (i = 1; i <= qty; i++)
  {
    if (fscanf (file, "%d%d", &dim, &id) != 2)
      abort ();

    if (dim == 0)
      status = fscanf (file, "%s", pMesh0D
                       && id <=
                       (*pMesh0D).ElsetQty ? (*pMesh0D).
                       ElsetLabels[id] : string);
    else if (dim == 1)
      status = fscanf (file, "%s", pMesh1D
                       && id <=
                       (*pMesh1D).ElsetQty ? (*pMesh1D).
                       ElsetLabels[id] : string);
    else if (dim == 2)
      status = fscanf (file, "%s", pMesh2D
                       && id <=
                       (*pMesh2D).ElsetQty ? (*pMesh2D).
                       ElsetLabels[id] : string);
    else if (dim == 3)
      status = fscanf (file, "%s", pMesh3D
                       && id <=
                       (*pMesh3D).ElsetQty ? (*pMesh3D).
                       ElsetLabels[id] : string);

    if (status != 1)
      abort ();
  }

  ut_free_1d_char (&string);

  // footer
  ut_file_skip (file, 1);

  return;
}

void
neut_mesh_fscanf_msh_orientations (FILE *file, char *entity, struct MESH *pMesh)
{
  int qty;
  char *des = ut_alloc_1d_char (100), *end = NULL;
  char *tmp = ut_alloc_1d_char (100);

  if (!strcmp (entity, "elt"))
    ut_string_string ("$EndElementOrientations", &end);
  else if (!strcmp (entity, "elset"))
    ut_string_string ("$EndElsetOrientations", &end);

  // header
  ut_file_skip (file, 1);

  while (ut_file_nextstring_test (file, end) != 1)
  {
    if (fscanf (file, "%d", &qty) != 1)
      abort ();

    if (fscanf (file, "%s", des) != 1)
      ut_print_message (2, 2, "Failed to read msh file.\n");

    if (!strcmp (entity, "elset"))
    {
      ut_string_string (des, &(*pMesh).ElsetOriDes);
      neut_ori_des_expand ((*pMesh).ElsetOriDes, &(*pMesh).ElsetOriDes);

      (*pMesh).ElsetOri = ut_alloc_2d ((*pMesh).ElsetQty + 1, 4);

      neut_ori_fscanf (file, (*pMesh).ElsetOriDes, (*pMesh).ElsetOri + 1, (*pMesh).ElsetId + 1,
                       (*pMesh).ElsetQty, "id");
    }
    else if (!strcmp (entity, "elt"))
    {
      ut_string_string (des, &(*pMesh).EltOriDes);
      neut_ori_des_expand ((*pMesh).EltOriDes, &(*pMesh).EltOriDes);

      (*pMesh).EltOri = ut_alloc_2d ((*pMesh).EltQty + 1, 4);

      neut_ori_fscanf (file, (*pMesh).EltOriDes, (*pMesh).EltOri + 1, NULL,
                       (*pMesh).EltQty, "id");
    }
    else
      abort ();
  }

  if (!ut_file_nextstring_test (file, end))
    ut_print_message (2, 2, "Failed to read msh file\n");

  ut_file_skip (file, 1);

  ut_free_1d_char (&des);
  ut_free_1d_char (&end);
  ut_free_1d_char (&tmp);

  return;
}

void
neut_mesh_fscanf_msh_groups (FILE *file, struct MESH *pMesh)
{
  int i, id, qty;
  char *type = ut_alloc_1d_char (100);

  // header
  ut_file_skip (file, 1);

  if (fscanf (file, "%s", type) != 1)
    abort ();
  if (strcmp (type, "elset"))
    ut_print_message (2, 2, "Failed to read msh file.\n");

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  if (qty != (*pMesh).ElsetQty)
    ut_print_message (2, 2, "Failed to read msh file.\n");

  (*pMesh).ElsetGroup = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1)
      abort ();
    if (fscanf (file, "%d", (*pMesh).ElsetGroup + id) != 1)
      abort ();
  }

  if (!ut_file_nextstring_test (file, "$EndGroups"))
    ut_print_message (2, 2, "Failed to read msh file.\n");

  ut_file_skip (file, 1);

  ut_free_1d_char (&type);

  return;
}

void
neut_mesh_fscanf_msh_nsets (FILE *file)
{
  int i, qty, qty2;

  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  for (i = 1; i <= qty; i++)
  {
    ut_file_skip (file, 1);
    if (fscanf (file, "%d", &qty2) != 1)
      abort ();
    ut_file_skip (file, qty2);
  }

  if (!ut_file_nextstring_test (file, "$EndNSets"))
    ut_print_message (2, 2, "Failed to read msh file.\n");

  ut_file_skip (file, 1);

  return;
}

void
neut_mesh_fscanf_msh_fasets (FILE *file, struct MESH *pMesh)
{
  int i, qty, qty2, eltnodeqty;

  eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, 2, (*pMesh).EltOrder);

  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  for (i = 1; i <= qty; i++)
  {
    ut_file_skip (file, 1);
    if (fscanf (file, "%d", &qty2) != 1)
      abort ();
    ut_file_skip (file, qty2 * (1 + eltnodeqty));
  }

  if (!ut_file_nextstring_test (file, "$EndFasets"))
    ut_print_message (2, 2, "Failed to read msh file.\n");

  ut_file_skip (file, 1);

  return;
}

void
neut_mesh_fscanf_msh_nodeparts (FILE *file, struct NODES *pNodes)
{
  int i, id, qty;

  (*pNodes).NodePart = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  for (i = 1; i <= qty; i++)
  {
    if (fscanf (file, "%d", &id) != 1)
      abort ();
    if (fscanf (file, "%d", (*pNodes).NodePart + id) != 1)
      abort ();
  }

  ut_file_skip (file, 1);

  neut_nodes_init_parts (pNodes);

  return;
}
