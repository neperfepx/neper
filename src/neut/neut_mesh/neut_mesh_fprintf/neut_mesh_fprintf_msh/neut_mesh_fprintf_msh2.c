/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
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
  else if (!strcmp (version, "msh4") || version[0] == '4')
    fprintf (file, "4.1 %d %ld\n", filetype, sizeof (double));
  else
    abort ();

  if (!strcmp (mode, "binary"))
  {
    fwrite (&one, sizeof (int), 1, file);
    fprintf (file, "\n");
  }

  fprintf (file, "$EndMeshFormat\n");

  fprintf (file, "$MeshVersion\n");
  fprintf (file, "2.2.1\n");
  fprintf (file, "$EndMeshVersion\n");

  return;
}

void
neut_mesh_fprintf_msh_entities (FILE * file, char *mode, struct TESS Tess,
                                 struct NODES Nodes,
                                 struct MESH Mesh0D, struct MESH Mesh1D,
                                 struct MESH Mesh2D, struct MESH Mesh3D)
{
  fprintf (file, "$Entities\n");

  fprintf (file, "%d %d %d %d\n", Mesh0D.ElsetQty, Mesh1D.ElsetQty, Mesh2D.ElsetQty, Mesh3D.ElsetQty);

  if (Mesh0D.EltQty)
    neut_mesh_fprintf_msh_entities_dim (file, mode, Tess, Nodes, Mesh0D);

  if (Mesh1D.EltQty)
    neut_mesh_fprintf_msh_entities_dim (file, mode, Tess, Nodes, Mesh1D);

  if (Mesh2D.EltQty)
    neut_mesh_fprintf_msh_entities_dim (file, mode, Tess, Nodes, Mesh2D);

  if (Mesh3D.EltQty)
    neut_mesh_fprintf_msh_entities_dim (file, mode, Tess, Nodes, Mesh3D);

  fprintf (file, "$EndEntities\n");

  return;
}

void
neut_mesh_fprintf_msh_nodes (FILE * file, char *mode, struct NODES Nodes)
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
        fprintf (file, " %.12f",
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
neut_mesh_fprintf_msh_nodes_v4 (FILE * file, char *mode, struct TESS Tess,
                                 struct NODES Nodes,
                                 struct MESH Mesh0D, struct MESH Mesh1D,
                                 struct MESH Mesh2D, struct MESH Mesh3D)
{
  fprintf (file, "$Nodes\n");
  fprintf (file, "%d %d 1 %d\n", Mesh0D.ElsetQty + Mesh1D.ElsetQty +
                                 Mesh2D.ElsetQty + Mesh3D.ElsetQty,
                                 Nodes.NodeQty, Nodes.NodeQty);

  // if (!strcmp (mode, "ascii"))
  {
    neut_mesh_fprintf_msh_nodes_dim_v4 (file, mode, Tess, Nodes, Mesh0D, Mesh0D);
    neut_mesh_fprintf_msh_nodes_dim_v4 (file, mode, Tess, Nodes, Mesh0D, Mesh1D);
    neut_mesh_fprintf_msh_nodes_dim_v4 (file, mode, Tess, Nodes, Mesh1D, Mesh2D);
    neut_mesh_fprintf_msh_nodes_dim_v4 (file, mode, Tess, Nodes, Mesh2D, Mesh3D);
  }

  /*
  else
    abort ();
  */

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
neut_mesh_fprintf_msh_elts_v4 (FILE * file, char *mode, struct TESS Tess,
                                struct MESH Mesh0D, struct MESH Mesh1D,
                                struct MESH Mesh2D, struct MESH Mesh3D,
                                struct MESH MeshCo, char **fasets,
                                int *fasetids, int fasetqty, char *dim,
                                char *numbering, int *shift)
{
  int *eltfaset = NULL, EltQty;

  neut_mesh_fprintf_msh_elts_pre (Tess, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
                                   MeshCo, fasets, fasetqty, fasetids, dim,
                                   numbering, &eltfaset, shift, &EltQty);

  fprintf (file, "$Elements\n");
  fprintf (file, "%d %d 1 %d\n", Mesh0D.ElsetQty + Mesh1D.ElsetQty +
                                 Mesh2D.ElsetQty + Mesh3D.ElsetQty,
                                 EltQty, EltQty);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0"))
    neut_mesh_fprintf_msh_elts_dim_v4 (file, mode, Mesh0D, shift[0]);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
    neut_mesh_fprintf_msh_elts_dim_v4 (file, mode, Mesh1D, shift[1]);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    neut_mesh_fprintf_msh_elts_dim_v4 (file, mode, Mesh2D, shift[2]);

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    neut_mesh_fprintf_msh_elts_dim_v4 (file, mode, Mesh3D, shift[3]);

  if (MeshCo.EltQty)
    neut_mesh_fprintf_msh_elts_dim_v4 (file, mode, MeshCo, shift[4]);

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
    physicalqty += Mesh2D.ElsetQty;
  physicalqty += fasetqty;
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
        fprintf (file, "0 %d %s\n", i, Mesh0D.ElsetLabels[i]);
      else
        fprintf (file, "0 %d ver%d\n", i, i);
    }

  // 1D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
    for (i = 1; i <= Mesh1D.ElsetQty; i++)
    {
      if (Mesh1D.ElsetLabels)
        fprintf (file, "1 %d %s\n", i, Mesh1D.ElsetLabels[i]);
      else
        fprintf (file, "1 %d edge%d\n", i, i);
    }

  // 2D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
  {
    for (i = 1; i <= Mesh2D.ElsetQty; i++)
    {
      if (Mesh2D.ElsetLabels)
        fprintf (file, "2 %d %s\n", i, Mesh2D.ElsetLabels[i]);
      else
        fprintf (file, "2 %d face%d\n", i, i);
    }

    for (i = 0; i < fasetqty; i++)
      fprintf (file, "2 %d %s\n", fasetids[i] + Mesh2D.ElsetQty, fasets[i]);
  }

  // 3D mesh
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      if (Mesh3D.ElsetLabels)
        fprintf (file, "3 %d %s\n", i, Mesh3D.ElsetLabels[i]);
      else
        fprintf (file, "3 %d poly%d\n", i, i);
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

  if (Mesh.ElsetOri)
  {
    fprintf (file, "$ElsetOrientations\n");

    fprintf (file, "%d %s\n", Mesh.ElsetQty, Mesh.ElsetOriDes);

    neut_ori_fprintf (file, Mesh.ElsetOriDes, Mesh.ElsetOri + 1, Mesh.ElsetQty, "id");

    fprintf (file, "$EndElsetOrientations\n");
  }

  // element

  if (Mesh.EltOri)
  {
    fprintf (file, "$ElementOrientations\n");

    fprintf (file, "%d %s\n", Mesh.EltQty, Mesh.EltOriDes);

    neut_ori_fprintf (file, Mesh.EltOriDes, Mesh.EltOri + 1, Mesh.EltQty, "id");

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
    fprintf (file, "%d %d\n", i, Mesh.ElsetGroup[i]);

  fprintf (file, "$EndGroups\n");

  return;
}

void
neut_mesh_fprintf_msh_nsets (FILE * file, struct NSET NSet0D,
                              struct NSET NSet1D, struct NSET NSet2D,
                              char *nsetlist)
{
  int i, j, status;
  char **nsets;
  int nsetqty;

  ut_list_break (nsetlist, NEUT_SEP_NODEP, &nsets, &nsetqty);

  fprintf (file, "$NSets\n");
  fprintf (file, "%d\n", nsetqty);

  for (i = 0; i < nsetqty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (nsets[i], NSet0D.names[j]))
      {
        neut_mesh_fprintf_msh_nset (file, NSet0D.names[j], NSet0D.NodeQty[j],
                                     NSet0D.nodes[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (nsets[i], NSet1D.names[j]))
      {
        neut_mesh_fprintf_msh_nset (file, NSet1D.names[j], NSet1D.NodeQty[j],
                                     NSet1D.nodes[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (nsets[i], NSet2D.names[j]))
      {
        neut_mesh_fprintf_msh_nset (file, NSet2D.names[j], NSet2D.NodeQty[j],
                                     NSet2D.nodes[j]);
        status = 0;
        break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", nsets[i]);
  }

  fprintf (file, "$EndNSets\n");

  ut_free_2d_char (&nsets, nsetqty);

  return;
}

void
neut_mesh_fprintf_msh_fasets (FILE * file, struct TESS Tess,
                              struct MESH Mesh2D, struct MESH Mesh3D,
                              int *shift, char *fasetlist)
{
  int i, j, k, eltnb, EltQty, elt3dqty, id, status, qty, eltnodeqty;
  int *elt3d = NULL;
  char **name = NULL;

  if (neut_mesh_isvoid (Mesh2D))
    return;

  eltnodeqty = neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);

  ut_list_break (fasetlist, NEUT_SEP_NODEP, &name, &qty);

  fprintf (file, "$Fasets\n");

  fprintf (file, "%d\n", qty);

  for (i = 0; i < qty; i++)
  {
    status = neut_tess_domface_label_id (Tess, name[i], &id);

    if (status != 0)
      continue;

    // printing number of elements
    EltQty = 0;
    for (j = 1; j <= Tess.DomTessFaceQty[id]; j++)
      EltQty += Mesh2D.Elsets[Tess.DomTessFaceNb[id][j]][0];

    fprintf (file, "%s\n", name[i]);

    fprintf (file, "%d\n", EltQty);

    // printing elements
    for (j = 1; j <= Tess.DomTessFaceQty[id]; j++)
      for (k = 1; k <= Mesh2D.Elsets[Tess.DomTessFaceNb[id][j]][0]; k++)
      {
        eltnb = Mesh2D.Elsets[Tess.DomTessFaceNb[id][j]][k];

        neut_mesh_elt2d_elts3d (Mesh2D, eltnb, Mesh3D, &elt3d, &elt3dqty);

        if (elt3dqty != 1)
          abort ();

        fprintf (file, "%d ", elt3d[0] + shift[3]);
        ut_array_1d_int_fprintf_reverse (file, Mesh2D.EltNodes[eltnb], eltnodeqty, "%d");
      }
  }

  fprintf (file, "$EndFasets\n");

  ut_free_1d_int (&elt3d);
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
