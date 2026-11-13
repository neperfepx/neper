/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_msh_.h"

void
neut_mesh_fscanf_msh_head (FILE * file, char **pmode, char **pdomain, int *ptopology,
                           char **pversion)
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
    neut_mesh_fscanf_msh_version (file, pversion);

  if (ut_file_nextstring_test (file, "$Domain"))
  {
    ut_file_skip (file, 1);
    (*pdomain) = ut_alloc_1d_char (100);
    if (fscanf (file, "%s", *pdomain) != 1)
      abort ();
    ut_file_skip (file, 1);
  }

  if (ut_file_nextstring_test (file, "$Topology"))
  {
    ut_file_skip (file, 1);

    if (ptopology)
    {
      if (fscanf (file, "%d", ptopology) != 1)
        abort ();
    }
    else
      ut_file_skip (file, 1);

    ut_file_skip (file, 1);
  }

  return;
}

void
neut_mesh_fscanf_msh_nodes (FILE * file, char *mode, char *domain,
                            struct NODES *pNodes, int **pnode_nbs)
{
  ut_string_string (domain, &((*pNodes).Domain));
  neut_mesh_fscanf_msh_nodes_head (file, &(*pNodes).NodeQty);

  (*pnode_nbs) = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pnode_nbs)[0] = (*pNodes).NodeQty;

  neut_mesh_fscanf_msh_nodes_prop (file, mode, pNodes, *pnode_nbs);

  neut_mesh_fscanf_msh_nodes_foot (file);

  return;
}

int
neut_mesh_fscanf_msh_elts (FILE * file, char *mode, char *domain, int *node_nbs,
                           struct MESH *pMesh0D, struct MESH *pMesh1D,
                           struct MESH *pMesh2D, struct MESH *pMesh3D,
                           struct MESH *pMeshCo, struct MESH **ppMesh)
{
  int leftelts, dim, status;
  struct MESH Trash;

  neut_mesh_set_zero (&Trash);

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", &leftelts) != 1)
    abort ();

  dim = -1;

  char c;
  if (!strcmp (mode, "binary"))
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  status = neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh0D ? pMesh0D : &Trash, node_nbs, 0,
                 &leftelts);
  if (status != 0)
    return status;
  if (pMesh0D && (*pMesh0D).EltQty > 0)
    dim = 0;
  status = neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh1D ? pMesh1D : &Trash, node_nbs, 1,
                 &leftelts);
  if (status != 0)
    return status;
  if (pMesh1D && (*pMesh1D).EltQty > 0)
    dim = 1;
  status = neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh2D ? pMesh2D : &Trash, node_nbs, 2,
                 &leftelts);
  if (status != 0)
    return status;
  if (pMesh2D && (*pMesh2D).EltQty > 0)
    dim = 2;
  status = neut_mesh_fscanf_msh_elts_dim (file, mode, pMesh3D ? pMesh3D : &Trash, node_nbs, 3,
                   &leftelts);
  if (status != 0)
    return status;
  if (pMesh3D && (*pMesh3D).EltQty > 0)
    dim = 3;
  status = neut_mesh_fscanf_msh_elts_dim (file, mode, pMeshCo ? pMeshCo : &Trash, node_nbs, dim,
                 &leftelts);
  if (status != 0)
    return status;

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
  {
    neut_mesh_init_elsetlabels (pMesh3D);
    if (domain)
      ut_string_string (domain, &((*pMesh3D).Domain));
  }

  if (pMesh3D && (*pMesh3D).EltQty > 0)
    *ppMesh = pMesh3D;
  else if (pMesh2D && (*pMesh2D).EltQty > 0)
    *ppMesh = pMesh2D;
  else if (pMesh1D && (*pMesh1D).EltQty > 0)
    *ppMesh = pMesh1D;
  else if (pMesh0D && (*pMesh0D).EltQty > 0)
    *ppMesh = pMesh0D;

  neut_mesh_free (&Trash);

  return 0;
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

  neut_nodes_init_nodeslave (pNodes);

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

    status = fscanf (file, "%s", string);
    ut_string_fnrs (string, "\"", "", INT_MAX);
    if (status != 1)
      abort ();

    if (dim == 0 && pMesh0D && id <= (*pMesh0D).ElsetQty)
      (*pMesh0D).ElsetLabels[id] = strcpy ((*pMesh0D).ElsetLabels[id], string);
    else if (dim == 1 && pMesh1D && id <= (*pMesh1D).ElsetQty)
      (*pMesh1D).ElsetLabels[id] = strcpy ((*pMesh1D).ElsetLabels[id], string);
    else if (dim == 2 && pMesh2D && id <= (*pMesh2D).ElsetQty)
      (*pMesh2D).ElsetLabels[id] = strcpy ((*pMesh2D).ElsetLabels[id], string);
    else if (dim == 3 && pMesh3D && id <= (*pMesh3D).ElsetQty)
      (*pMesh3D).ElsetLabels[id] = strcpy ((*pMesh3D).ElsetLabels[id], string);
  }

  ut_free_1d_char (&string);

  // footer
  ut_file_skip (file, 1);

  return;
}

void
neut_mesh_fscanf_msh_crysym (FILE *file, struct NODES *pNodes, struct MESH *pMesh0D,
                             struct MESH *pMesh1D, struct MESH *pMesh2D,
                             struct MESH *pMesh3D, struct MESH *pMeshCo)
{
  char *tmp = ut_alloc_1d_char (1000);

  // header
  ut_file_skip (file, 1);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  if (pMesh0D)
    ut_string_string (tmp, &(*pMesh0D).ElsetCrySym);
  if (pMesh1D)
    ut_string_string (tmp, &(*pMesh1D).ElsetCrySym);
  if (pMesh2D)
    ut_string_string (tmp, &(*pMesh2D).ElsetCrySym);
  if (pMesh3D)
    ut_string_string (tmp, &(*pMesh3D).ElsetCrySym);
  if (pMeshCo)
    ut_string_string (tmp, &(*pMeshCo).ElsetCrySym);
  if (pNodes)
    ut_string_string (tmp, &(*pNodes).crysym);

  // footer
  ut_file_skip (file, 1);

  ut_free_1d_char (&tmp);

  return;
}

void
neut_mesh_fscanf_msh_orientations (FILE *file, char *entity, char *version, struct MESH *pMesh)
{
  int qty, major, minor;
  char *des = ut_alloc_1d_char (100), *end = NULL;
  char *tmp = ut_alloc_1d_char (100);

  ut_string_version (version, &major, &minor, NULL);

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

      if (major <= 2 && minor <= 2)
      {
        ut_print_message (1, 3, "File version <= 2.2.  Fixing orientation convention...\n");
        neut_ori_des_fixconvention (&(*pMesh).ElsetOriDes);
      }

      (*pMesh).ElsetOri = ut_alloc_2d ((*pMesh).ElsetQty + 1, 4);

      neut_ori_fscanf (file, (*pMesh).ElsetOriDes, "ascii", (*pMesh).ElsetOri + 1, (*pMesh).ElsetId + 1,
                       (*pMesh).ElsetQty, "id");
    }
    else if (!strcmp (entity, "elt"))
    {
      ut_string_string (des, &(*pMesh).EltOriDes);
      neut_ori_des_expand ((*pMesh).EltOriDes, &(*pMesh).EltOriDes);

      (*pMesh).EltOri = ut_alloc_2d ((*pMesh).EltQty + 1, 4);

      neut_ori_fscanf (file, (*pMesh).EltOriDes, "ascii", (*pMesh).EltOri + 1, NULL,
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

// Reading the nsets, recorded in *pNSet (if not NULL; skipped otherwise), with
// node_nbs the file numbers of the nodes
void
neut_mesh_fscanf_msh_nsets (FILE *file, int *node_nbs, struct NSET *pNSet)
{
  int i, j, qty, qty2, node, maxnode = 0, *nodes_old_new = NULL;
  char *name = ut_alloc_1d_char (1000);

  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  if (pNSet)
  {
    neut_nset_free (pNSet);
    neut_nset_set_zero (pNSet);

    (*pNSet).qty = qty;
    (*pNSet).names = ut_alloc_1d_pchar (qty + 1);
    (*pNSet).NodeQty = ut_alloc_1d_int (qty + 1);
    (*pNSet).nodes = ut_alloc_1d_pint (qty + 1);

    maxnode = ut_array_1d_int_max (node_nbs + 1, node_nbs[0]);
    nodes_old_new = ut_alloc_1d_int (maxnode + 1);
    for (i = 1; i <= node_nbs[0]; i++)
      nodes_old_new[node_nbs[i]] = i;
  }

  for (i = 1; i <= qty; i++)
  {
    if (fscanf (file, "%999s", name) != 1 || fscanf (file, "%d", &qty2) != 1)
      ut_print_message (2, 2, "Failed to read msh file.\n");

    if (!pNSet)
      ut_file_skip (file, qty2);

    else
    {
      ut_string_string (name, (*pNSet).names + i);
      (*pNSet).NodeQty[i] = qty2;
      (*pNSet).nodes[i] = ut_alloc_1d_int (qty2);

      for (j = 0; j < qty2; j++)
      {
        if (fscanf (file, "%d", &node) != 1 || node < 1 || node > maxnode
            || !nodes_old_new[node])
          ut_print_message (2, 2, "Failed to read msh file (nset %s).\n", name);
        (*pNSet).nodes[i][j] = nodes_old_new[node];
      }
    }
  }

  if (!ut_file_nextstring_test (file, "$EndNSets"))
    ut_print_message (2, 2, "Failed to read msh file.\n");

  ut_file_skip (file, 1);

  ut_free_1d_int (&nodes_old_new);
  ut_free_1d_char (&name);

  return;
}

// Reading the fasets (2D elts of the domain faces, by face), as written by
// neut_mesh_fprintf_msh_fasets: for each faset, its label and elts, each
// elt being given by the id of its 3D elt and its nodes (in reverse order
// for file versions <= 2.3).  The fasets are recorded in *pMeshFasets (if
// not NULL, otherwise they are skipped), and the 3D elts of the elts (EltId)
// are recovered from *pMesh3D.  pMesh is the mesh of highest dimension and node_nbs are the
// node numbers of the file.
void
neut_mesh_fscanf_msh_fasets (FILE *file, char *version, int *node_nbs,
                             struct MESH *pMesh, struct MESH *pMesh3D,
                             struct MESH *pMeshFasets)
{
  int i, j, k, qty, eltqty, elt, eltnodeqty, eltorder, wordqty, record;
  int major = 0, minor = 0, reverse;
  int *nodes_old_new = NULL, *nodes = NULL;
  char *label = ut_alloc_1d_char (1000);
  long pos;

  record = pMeshFasets && pMesh3D && (*pMesh3D).EltQty > 0;

  if (version)
    ut_string_version (version, &major, &minor, NULL);
  reverse = major < 2 || (major == 2 && minor <= 3);

  ut_file_skip (file, 1);

  if (fscanf (file, "%d", &qty) != 1)
    abort ();

  // elt order, from the first elt (the fasets are not necessarily of the
  // order of the mesh)
  pos = ftell (file);
  eltqty = 0;
  for (i = 1; i <= qty && eltqty == 0; i++)
  {
    if (fscanf (file, "%s", label) != 1 || fscanf (file, "%d", &eltqty) != 1)
      abort ();
    if (eltqty == 0)
      continue;

    ut_file_goto_nextline (file);
    ut_file_nextlinenbwords (file, &wordqty);
  }
  fseek (file, pos, SEEK_SET);

  eltorder = (*pMesh).EltOrder;
  if (eltqty > 0)
  {
    for (eltorder = 1; eltorder <= 2; eltorder++)
      if (neut_elt_nodeqty ((*pMesh).EltType, 2, eltorder) == wordqty - 1)
        break;
    if (eltorder > 2)
      ut_print_message (2, 2, "Failed to read msh file (fasets).\n");
  }

  eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, 2, eltorder);
  nodes = ut_alloc_1d_int (eltnodeqty);

  if (record)
  {
    neut_mesh_reset (pMeshFasets);
    (*pMeshFasets).Dimension = 2;
    ut_string_string ((*pMesh).EltType, &(*pMeshFasets).EltType);
    (*pMeshFasets).EltOrder = eltorder;
    (*pMeshFasets).ElsetQty = qty;
    (*pMeshFasets).Elsets = ut_alloc_1d_pint (qty + 1);
    (*pMeshFasets).ElsetLabels = ut_alloc_1d_pchar (qty + 1);

    nodes_old_new =
      ut_alloc_1d_int (ut_array_1d_int_max (node_nbs + 1, node_nbs[0]) + 1);
    for (i = 1; i <= node_nbs[0]; i++)
      nodes_old_new[node_nbs[i]] = i;
  }

  for (i = 1; i <= qty; i++)
  {
    if (fscanf (file, "%s", label) != 1 || fscanf (file, "%d", &eltqty) != 1)
      abort ();

    if (!record)
    {
      ut_file_skip (file, eltqty * (1 + eltnodeqty));
      continue;
    }

    ut_string_string (label, (*pMeshFasets).ElsetLabels + i);
    (*pMeshFasets).Elsets[i] = ut_alloc_1d_int (eltqty + 1);

    for (j = 1; j <= eltqty; j++)
    {
      ut_file_skip (file, 1);
      for (k = 0; k < eltnodeqty; k++)
        if (fscanf (file, "%d", nodes + (reverse ? eltnodeqty - 1 - k : k)) != 1)
          abort ();

      for (k = 0; k < eltnodeqty; k++)
        nodes[k] = nodes_old_new[nodes[k]];

      neut_mesh_addelt (pMeshFasets, nodes);
      (*pMeshFasets).Elsets[i][++(*pMeshFasets).Elsets[i][0]] =
        (*pMeshFasets).EltQty;
    }
  }

  if (!ut_file_nextstring_test (file, "$EndFasets"))
    ut_print_message (2, 2, "Failed to read msh file.\n");

  ut_file_skip (file, 1);

  // 3D elts of the elts
  if (record)
  {
    if (!(*pMesh3D).NodeElts)
      neut_mesh_init_nodeelts (pMesh3D, node_nbs[0]);

    (*pMeshFasets).EltId = ut_alloc_1d_int ((*pMeshFasets).EltQty + 1);
    for (elt = 1; elt <= (*pMeshFasets).EltQty; elt++)
      if (neut_mesh_elt2d_elt3d (*pMeshFasets, elt, *pMesh3D,
                                 (*pMeshFasets).EltId + elt) != 0)
        ut_print_message (2, 2, "Failed to read msh file.\n");

    neut_mesh_init_eltelset (pMeshFasets, NULL);
    neut_mesh_init_nodeelts (pMeshFasets, node_nbs[0]);
  }

  ut_free_1d_int (&nodes_old_new);
  ut_free_1d_int (&nodes);
  ut_free_1d_char (&label);

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

void
neut_mesh_fscanf_msh_version (FILE *file, char **pversion)
{
  // header
  ut_file_skip (file, 1);

  (*pversion) = ut_alloc_1d_char (100);
  if (fscanf (file, "%s", *pversion) != 1)
    ut_print_message (2, 0, "Input file is not a valid mesh file.\n");

  // footer
  ut_file_skip (file, 1);

  return;
}
