/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_msh_.h"

void
neut_mesh_fprintf_msh_pre (struct TESS Tess, char *fasetlist,
                            char ***pfasets, int **pfasetids, int *pfasetqty)
{
  int i, j, qty, found;
  char **tmp = NULL;

  *pfasetqty = 0;
  if (fasetlist && strlen (fasetlist) > 0 && strcmp (fasetlist, "none"))
  {
    ut_list_break (fasetlist, NEUT_SEP_NODEP, &tmp, &qty);

    for (i = 0; i < qty; i++)
    {
      found = 0;

      for (j = 1; j <= Tess.DomFaceQty; j++)
        if (!strcmp (tmp[i], Tess.DomFaceLabel[j]))
        {
          (*pfasetqty)++;
          (*pfasets) =
            ut_realloc_2d_char_addline (*pfasets, *pfasetqty,
                                        strlen (Tess.DomFaceLabel[j]) + 1);
          (*pfasetids) = ut_realloc_1d_int (*pfasetids, *pfasetqty);

          strcpy ((*pfasets)[(*pfasetqty) - 1], Tess.DomFaceLabel[j]);
          (*pfasetids)[*pfasetqty - 1] = j;

          found = 1;
          break;
        }

      if (!found)
        ut_print_message (1, 3, "Faset `%s' not found.  Skipping...\n",
                          tmp[i]);
    }
  }

  if (tmp)
    ut_free_2d_char (&tmp, qty);

  return;
}


void
neut_mesh_fprintf_msh_domain (FILE * file, char *Domain)
{
  fprintf (file, "$Domain\n");
  fprintf (file, "%s\n", Domain);
  fprintf (file, "$EndDomain\n");

  return;
}

void
neut_mesh_fprintf_msh_topology (FILE * file, int topology)
{
  fprintf (file, "$Topology\n");
  fprintf (file, "%d\n", topology);
  fprintf (file, "$EndTopology\n");

  return;
}

void
neut_mesh_fprintf_msh_header (FILE * file, char *mode, char *version)
{
  int filetype, one = 1;

  if (!strcmp (mode, "ascii"))
    filetype = 0;

  else if (!strcmp (mode, "binary"))
    filetype = 1;
  else
    abort ();

  fprintf (file, "$MeshFormat\n");
  if (!strcmp (version, "msh") || !strcmp (version, "msh2") || version[0] == '2')
    fprintf (file, "2.2 %d %ld\n", filetype, sizeof (double));
  else
    abort ();

  if (!strcmp (mode, "binary"))
  {
    fwrite (&one, sizeof (int), 1, file);
    fprintf (file, "\n");
  }

  fprintf (file, "$EndMeshFormat\n");

  fprintf (file, "$MeshVersion\n");
  fprintf (file, "2.4\n");
  fprintf (file, "$EndMeshVersion\n");

  return;
}

void
neut_mesh_fprintf_msh_nodes (FILE *file, char *mode, struct NODES Nodes)
{
  int i, j;

  fprintf (file, "$Nodes\n");
  fprintf (file, "%d\n", Nodes.NodeQty);
  if (!strcmp (mode, "ascii"))
  {
    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      fprintf (file, "%d", i);
      for (j = 0; j < 3; j++)
        fprintf (file, " " REAL_PRINT_FORMAT,
                 (fabs (Nodes.NodeCoo[i][j]) <
                  1e-12) ? 0 : Nodes.NodeCoo[i][j]);
      fprintf (file, "\n");
    }
  }
  else
  {
    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      fwrite (&i, sizeof (int), 1, file);
      fwrite (Nodes.NodeCoo[i], sizeof (double), 3, file);
    }
    fprintf (file, "\n");
  }
  fprintf (file, "$EndNodes\n");

  return;
}

void
neut_mesh_fprintf_msh_elts (FILE * file, char *mode, struct TESS Tess,
                             struct MESH Mesh0D, struct MESH Mesh1D,
                             struct MESH Mesh2D, struct MESH Mesh3D,
                             struct MESH MeshCo, char **fasets,
                             int *fasetids, int fasetqty, char *dim,
                             char *numbering, int *shift)
{
  int EltQty, *eltfaset = NULL;

  neut_mesh_fprintf_msh_elts_pre (Tess, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
                                   MeshCo, fasets, fasetqty, fasetids, dim,
                                   numbering, &eltfaset, shift, &EltQty);

  fprintf (file, "$Elements\n");
  fprintf (file, "%d\n", EltQty);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0"))
    neut_mesh_fprintf_msh_elts_dim (file, mode, shift[0], Mesh0D);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
    neut_mesh_fprintf_msh_elts_dim (file, mode, shift[1], Mesh1D);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    neut_mesh_fprintf_msh_elts_dim (file, mode, shift[2], Mesh2D);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    neut_mesh_fprintf_msh_elts_dim (file, mode, shift[3], Mesh3D);

  if (MeshCo.EltQty)
    neut_mesh_fprintf_msh_elts_dim (file, mode, shift[4], MeshCo);

  fprintf (file, "$EndElements\n");

  ut_free_1d_int (&eltfaset);

  return;
}

void
neut_mesh_fprintf_msh_periodicity (FILE *file, struct NODES Nodes)
{
  int i, id;

  fprintf (file, "$Periodicity\n");
  fprintf (file, "%d\n", Nodes.PerNodeQty);

  for (i = 1; i <= Nodes.PerNodeQty; i++)
  {
    id = Nodes.PerNodeNb[i];

    fprintf (file, "%d %d ", id, Nodes.PerNodeMaster[id]);
    ut_array_1d_int_fprintf (file, Nodes.PerNodeShift[id], 3, "%d");
  }

  fprintf (file, "$EndPeriodicity\n");

  return;
}

void
neut_mesh_fprintf_msh_physical (FILE * file, struct MESH Mesh0D,
                                 struct MESH Mesh1D, struct MESH Mesh2D,
                                 struct MESH Mesh3D, struct MESH MeshCo,
                                 char **fasets, int *fasetids, int fasetqty,
                                 char *dim)
{
  int i, physicalqty = 0;

  fprintf (file, "$PhysicalNames\n");

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0"))
    physicalqty += Mesh0D.ElsetQty;
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
    physicalqty += Mesh1D.ElsetQty;
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
  {
    physicalqty += Mesh2D.ElsetQty;
    physicalqty += fasetqty;
  }
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    physicalqty += Mesh3D.ElsetQty;
  if (MeshCo.EltQty > 0)
    physicalqty += MeshCo.ElsetQty;

  fprintf (file, "%d\n", physicalqty);

  // 0D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0"))
    for (i = 1; i <= Mesh0D.ElsetQty; i++)
    {
      if (Mesh0D.ElsetLabels)
        fprintf (file, "0 %d \"%s\"\n", i, Mesh0D.ElsetLabels[i]);
      else
        fprintf (file, "0 %d \"ver%d\"\n", Mesh0D.ElsetId ? Mesh0D.ElsetId[i] : i, Mesh0D.ElsetId ? Mesh0D.ElsetId[i] : i);
    }

  // 1D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
    for (i = 1; i <= Mesh1D.ElsetQty; i++)
    {
      if (Mesh1D.ElsetLabels)
        fprintf (file, "1 %d \"%s\"\n", i, Mesh1D.ElsetLabels[i]);
      else
        fprintf (file, "1 %d \"edge%d\"\n", Mesh1D.ElsetId ? Mesh1D.ElsetId[i] : i, Mesh1D.ElsetId ? Mesh1D.ElsetId[i] : i);
    }

  // 2D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
  {
    for (i = 1; i <= Mesh2D.ElsetQty; i++)
    {
      if (Mesh2D.ElsetLabels)
        fprintf (file, "2 %d \"%s\"\n", Mesh2D.ElsetId ? Mesh2D.ElsetId[i] : i, Mesh2D.ElsetLabels[i]);
      else
        fprintf (file, "2 %d \"face%d\"\n", Mesh2D.ElsetId ? Mesh2D.ElsetId[i] : i, i);
    }

    for (i = 0; i < fasetqty; i++)
      fprintf (file, "2 %d \"%s\"\n", fasetids[i] + Mesh2D.ElsetQty, fasets[i]);
  }

  // 3D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      if (Mesh3D.ElsetLabels)
        fprintf (file, "3 %d \"%s\"\n", Mesh3D.ElsetId ? Mesh3D.ElsetId[i] : i, Mesh3D.ElsetLabels[i]);
      else
        fprintf (file, "3 %d \"poly%d\"\n", Mesh3D.ElsetId ? Mesh3D.ElsetId[i] : i, Mesh3D.ElsetId ? Mesh3D.ElsetId[i] : i);
    }

  // Co mesh
  if (MeshCo.EltQty > 0)
  {
    for (i = 1; i <= MeshCo.ElsetQty; i++)
    {
      if (MeshCo.ElsetLabels)
        fprintf (file, "%d %d %s\n", MeshCo.Dimension, MeshCo.ElsetId[i],
                 MeshCo.ElsetLabels[i]);
      else
        abort ();
    }
  }

  fprintf (file, "$EndPhysicalNames\n");

  return;
}

void
neut_mesh_fprintf_msh_orientations (FILE *file, struct MESH Mesh)
{
  // elset

  if (Mesh.ElsetCrySym)
  {
    fprintf (file, "$ElsetCrySym\n");
    fprintf (file, "%s\n", Mesh.ElsetCrySym);
    fprintf (file, "$EndElsetCrySym\n");
  }

  if (Mesh.ElsetOri)
  {
    int i, *indexed = ut_alloc_1d_int (Mesh.ElsetQty + 1);

    fprintf (file, "$ElsetOrientations\n");

    for (i = 1; i <= Mesh.ElsetQty; i++)
      indexed[i] = (Mesh.Elsets[i][0] > 0);

    fprintf (file, "%d %s\n", ut_array_1d_int_sum (indexed + 1, Mesh.ElsetQty), Mesh.ElsetOriDes);

    neut_ori_fprintf (file, Mesh.ElsetOriDes, "ascii", Mesh.ElsetOri + 1,
                      Mesh.ElsetId ? Mesh.ElsetId + 1 : NULL, indexed + 1, Mesh.ElsetQty - Mesh.CustomElsetQty, "id");

    fprintf (file, "$EndElsetOrientations\n");

    ut_free_1d_int (&indexed);
  }

  // element

  if (Mesh.EltOri)
  {
    fprintf (file, "$ElementOrientations\n");

    fprintf (file, "%d %s\n", Mesh.EltQty, Mesh.EltOriDes);

    neut_ori_fprintf (file, Mesh.EltOriDes, "ascii", Mesh.EltOri + 1, NULL, NULL, Mesh.EltQty, "id");

    fprintf (file, "$EndElementOrientations\n");
  }

  return;
}

void
neut_mesh_fprintf_msh_elsetgroups (FILE *file, struct MESH Mesh)
{
  int i;

  fprintf (file, "$Groups\n");

  fprintf (file, "elset\n");
  fprintf (file, "%d\n", Mesh.ElsetQty);

  for (i = 1; i <= Mesh.ElsetQty; i++)
    fprintf (file, "%d %d\n", Mesh.ElsetId ? Mesh.ElsetId[i] : i, Mesh.ElsetGroup[i]);

  fprintf (file, "$EndGroups\n");

  return;
}

// Writing the nsets of nsetlist (an nset not found is skipped, and the
// section is not written if no nset is found)
void
neut_mesh_fprintf_msh_nsets (FILE * file, struct NSET NSet0D,
                              struct NSET NSet1D, struct NSET NSet2D,
                              char *nsetlist)
{
  int i, j, k, foundqty = 0;
  char **nsets;
  int nsetqty;
  struct NSET NSets[3] = { NSet0D, NSet1D, NSet2D };
  int *dims = NULL, *ids = NULL;

  ut_list_break (nsetlist, NEUT_SEP_NODEP, &nsets, &nsetqty);

  dims = ut_alloc_1d_int (nsetqty);
  ids = ut_alloc_1d_int (nsetqty);

  for (i = 0; i < nsetqty; i++)
  {
    ids[i] = -1;

    for (k = 0; k < 3 && ids[i] == -1; k++)
      for (j = 1; j <= NSets[k].qty; j++)
        if (!strcmp (nsets[i], NSets[k].names[j]))
        {
          dims[i] = k;
          ids[i] = j;
          foundqty++;
          break;
        }

    if (ids[i] == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", nsets[i]);
  }

  if (foundqty > 0)
  {
    fprintf (file, "$NSets\n");
    fprintf (file, "%d\n", foundqty);

    for (i = 0; i < nsetqty; i++)
      if (ids[i] != -1)
        neut_mesh_fprintf_msh_nset (file, NSets[dims[i]].names[ids[i]],
                                     NSets[dims[i]].NodeQty[ids[i]],
                                     NSets[dims[i]].nodes[ids[i]]);

    fprintf (file, "$EndNSets\n");
  }

  ut_free_2d_char (&nsets, nsetqty);
  ut_free_1d_int (&dims);
  ut_free_1d_int (&ids);

  return;
}

void
neut_mesh_fprintf_msh_fasets (FILE * file, int *shift, char *fasetlist,
                              struct MESH Mesh)

{
  int i, j, elt2d, qty, eltnodeqty;
  int *elt3d = NULL, *ids = NULL;
  char **name = NULL;

  if (Mesh.EltQty == 0)
    return;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  ut_list_break (fasetlist, NEUT_SEP_NODEP, &name, &qty);

  // elsets of the fasets (-1 if not found)
  ids = ut_alloc_1d_int (qty);
  ut_array_1d_int_set (ids, qty, -1);
  for (i = 0; i < qty; i++)
    for (j = 1; j <= Mesh.ElsetQty; j++)
      if (!strcmp (name[i], Mesh.ElsetLabels[j]))
      {
        ids[i] = j;
        break;
      }

  fprintf (file, "$Fasets\n");

  fprintf (file, "%d\n", qty - ut_array_1d_int_valnb (ids, qty, -1));

  for (i = 0; i < qty; i++)
  {
    int id = ids[i];

    if (id == -1)
      continue;

    // printing number of elements
    fprintf (file, "%s\n", name[i]);

    fprintf (file, "%d\n", Mesh.Elsets[id][0]);

    // printing elements
    for (j = 1; j <= Mesh.Elsets[id][0]; j++)
    {
      elt2d = Mesh.Elsets[id][j];
      fprintf (file, "%d ", Mesh.EltId[elt2d] + shift[3]);
      ut_array_1d_int_fprintf (file, Mesh.EltNodes[elt2d], eltnodeqty, "%d");
    }
  }

  fprintf (file, "$EndFasets\n");

  ut_free_1d_int (&elt3d);
  ut_free_1d_int (&ids);
  ut_free_2d_char (&name, qty);

  return;
}

void
neut_mesh_fprintf_msh_nodeparts (FILE * file, struct NODES Nodes)
{
  int i;

  fprintf (file, "$NodePartitions\n");

  fprintf (file, "%d\n", Nodes.NodeQty);

  for (i = 1; i <= Nodes.NodeQty; i++)
    fprintf (file, "%d %d\n", i, Nodes.NodePart[i]);

  fprintf (file, "$EndNodePartitions\n");

  return;
}
