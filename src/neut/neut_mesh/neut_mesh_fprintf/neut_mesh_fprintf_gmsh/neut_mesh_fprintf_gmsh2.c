/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_gmsh_.h"

void
neut_mesh_fprintf_gmsh_pre (struct TESS Tess, char *fasetlist, char ***pfasets,
                            int **pfasetids, int *pfasetqty)
{
  int i, j, qty, found;
  char **tmp = NULL;

  *pfasetqty = 0;
  if (fasetlist && strlen (fasetlist) > 0 && strcmp (fasetlist, "none"))
  {
    ut_string_separate (fasetlist, NEUT_SEP_NODEP, &tmp, &qty);

    for (i = 0; i < qty; i++)
    {
      found = 0;

      for (j = 1; j <= Tess.DomFaceQty; j++)
        if (!strcmp (tmp[i], Tess.DomFaceLabel[j]))
        {
          (*pfasetqty)++;
          (*pfasets) = ut_realloc_2d_char_addline (*pfasets, *pfasetqty, strlen (Tess.DomFaceLabel[j]) +1);
          (*pfasetids) = ut_realloc_1d_int (*pfasetids, *pfasetqty);

          strcpy ((*pfasets)[(*pfasetqty) - 1], Tess.DomFaceLabel[j]);
          (*pfasetids)[*pfasetqty - 1] = j;

          found = 1;
          break;
        }

      if (!found)
        ut_print_message (1, 3, "Faset `%s' not found.  Skipping...\n", tmp[i]);
    }
  }

  return;
}

void
neut_meshheader_fprintf_gmsh (FILE * file, char *mode, struct MESH Mesh0D, struct
			      MESH Mesh1D, struct MESH Mesh2D,
			      struct MESH Mesh3D)
{
  int filetype, one = 1;

  if (!strcmp (mode, "ascii"))
    filetype = 0;
  else if (!strcmp (mode, "binary"))
    filetype = 1;
  else
    abort ();

  fprintf (file, "$MeshFormat\n");
  fprintf (file, "2.2 %d %ld\n", filetype, sizeof (double));
  if (!strcmp (mode, "binary"))
  {
    fwrite (&one, sizeof (int), 1, file);
    fprintf (file, "\n");
  }

  if (!Mesh0D.ElsetId && !Mesh1D.ElsetId
      && !Mesh2D.ElsetId && !Mesh3D.ElsetId)
  {
    fprintf (file, "$Comments\n");
    fprintf (file, "contiguous\n");
    fprintf (file, "$EndComments\n");
  }
  fprintf (file, "$EndMeshFormat\n");

  return;
}

void
neut_nodes_fprintf_gmsh (FILE * file, char *mode, struct NODES Nodes)
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
  }
  fprintf (file, "\n");
  fprintf (file, "$EndNodes\n");

  return;
}

void
neut_elts_fprintf_gmsh (FILE * file, char *mode, struct TESS Tess,
                        struct MESH Mesh0D,
			struct MESH Mesh1D, struct MESH Mesh2D,
			struct MESH Mesh3D,
			struct PART Part, struct MESH MeshCo,
                        char **fasets, int *fasetids, int fasetqty,
                        char *dim, char *numbering)
{
  int i, j, elt_type0D, elt_type1D, elt_type2D, elt_type3D, elt_typeCo, elset;
  int eltnodeqty1D, eltnodeqty2D, eltnodeqty3D, eltnodeqtyCo;
  int *shift = ut_alloc_1d_int (5);
  int *eltfaset = NULL;

  if (fasetqty > 0)
  {
    eltfaset = ut_alloc_1d_int (Mesh2D.EltQty + 1);

    for (i = 0; i < fasetqty; i++)
    {
      int *elts = NULL, eltqty;

      neut_mesh_domface_elts (Tess, Mesh2D, fasets[i], &elts, &eltqty);

      for (j = 0; j < eltqty; j++)
        eltfaset[elts[j]] = fasetids[i] + Mesh2D.ElsetQty;

      ut_free_1d_int (elts);
    }
  }

  eltnodeqty1D =
    neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  eltnodeqty2D =
    neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  eltnodeqty3D =
    neut_elt_nodeqty (Mesh3D.EltType, Mesh3D.Dimension, Mesh3D.EltOrder);
  eltnodeqtyCo =
    neut_elt_nodeqty (MeshCo.EltType, MeshCo.Dimension, MeshCo.EltOrder);

  if (Mesh0D.EltQty != 0
      && neut_gmsh_elt_nb (Mesh0D.EltType, Mesh0D.Dimension, Mesh0D.EltOrder,
			   &elt_type0D) != 0)
    ut_error_reportbug ();

  if (Mesh1D.EltQty != 0
      && neut_gmsh_elt_nb (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder,
			   &elt_type1D) != 0)
    ut_error_reportbug ();

  if (Mesh2D.EltQty != 0
      && neut_gmsh_elt_nb (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder,
			   &elt_type2D) != 0)
    ut_error_reportbug ();

  if (Mesh3D.EltQty != 0
      && neut_gmsh_elt_nb (Mesh3D.EltType, Mesh3D.Dimension, Mesh3D.EltOrder,
			   &elt_type3D) != 0)
    ut_error_reportbug ();

  if (MeshCo.EltQty != 0
      && neut_gmsh_elt_nb (MeshCo.EltType, MeshCo.Dimension, MeshCo.EltOrder,
			   &elt_typeCo) != 0)
    ut_error_reportbug ();

  fprintf (file, "$Elements\n");
  int eltqty = 0;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "0"))
  {
    shift[0] = 0;
    eltqty += Mesh0D.EltQty;
  }

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1"))
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[1] = eltqty;
    else if (!strcmp (numbering, "from1"))
      shift[1] = 0;
    else
      ut_error_reportbug ();

    eltqty += Mesh1D.EltQty;
  }

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[2] = eltqty;
    else if (!strcmp (numbering, "from1"))
      shift[2] = 0;
    else
      ut_error_reportbug ();

    eltqty += Mesh2D.EltQty;
  }

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[3] = eltqty;
    else if (!strcmp (numbering, "from1"))
      shift[3] = 0;
    else
      ut_error_reportbug ();

    eltqty += Mesh3D.EltQty;
  }

  if (MeshCo.EltQty > 0)
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[4] = eltqty;
    else if (!strcmp (numbering, "from1"))
      shift[4] = 0;
    else
      ut_error_reportbug ();

    eltqty += MeshCo.EltQty;
  }

  fprintf (file, "%d\n", eltqty);

  // 0D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "0"))
  {
    if (!strcmp (mode, "binary"))
    {
      int data[3] = {elt_type0D, Mesh0D.EltQty, 3};
      fwrite (data, sizeof (int), 3, file);
    }

    if (Mesh0D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh0D, NULL);
    for (i = 1; i <= Mesh0D.EltQty; i++)
    {
      elset = (!Mesh0D.ElsetId) ? Mesh0D.EltElset[i]
	: Mesh0D.ElsetId[Mesh0D.EltElset[i]];

      if (!strcmp (mode, "ascii"))
      {
	fprintf (file, "%d %d 3 %d %d 0 ", i, elt_type0D, elset, elset);
	ut_array_1d_int_fprintf (file, Mesh0D.EltNodes[i], 1, "%d");
      }
      else
      {
	int data[4] = {i, elset, elset, 0};
	fwrite (&data, sizeof (int), 4, file);
	fwrite (Mesh0D.EltNodes[i], sizeof (int), 1, file);
      }
    }
  }

  // 1D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1"))
  {
    if (!strcmp (mode, "binary"))
    {
      int data[3] = {elt_type1D, Mesh1D.EltQty, 3};
      fwrite (data, sizeof (int), 3, file);
    }

    if (Mesh1D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh1D, NULL);

    for (i = 1; i <= Mesh1D.EltQty; i++)
    {
      elset = (!Mesh1D.ElsetId) ? Mesh1D.EltElset[i]
	: Mesh1D.ElsetId[Mesh1D.EltElset[i]];

      if (!strcmp (mode, "ascii"))
      {
	fprintf (file, "%d %d 3 %d %d 0 ", i + shift[1], elt_type1D,
		 elset, elset);
	ut_array_1d_int_fprintf (file, Mesh1D.EltNodes[i], eltnodeqty1D, "%d");
      }
      else
      {
	int data[4] = {i + shift[1], elset, elset, 0};
	fwrite (&data, sizeof (int), 4, file);
	fwrite (Mesh1D.EltNodes[i], sizeof (int), eltnodeqty1D, file);
      }
    }
  }

  // 2D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
  {
    if (!strcmp (mode, "binary"))
    {
      int data[3] = {elt_type2D, Mesh2D.EltQty, !fasetqty ? 3 : 4};
      fwrite (data, sizeof (int), 3, file);
    }

    if (Mesh2D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh2D, NULL);

    for (i = 1; i <= Mesh2D.EltQty; i++)
    {
      elset = (!Mesh2D.ElsetId) ? Mesh2D.EltElset[i]
	: Mesh2D.ElsetId[Mesh2D.EltElset[i]];

      if (!strcmp (mode, "ascii"))
      {
        if (!fasetqty || !eltfaset[i])
          fprintf (file, "%d %d 3 %d %d 0 ", i + shift[2], elt_type2D,
                   elset, elset);
        else
          fprintf (file, "%d %d 4 %d %d 0 %d ", i + shift[2], elt_type2D,
                   elset, elset, eltfaset[i]);
        ut_array_1d_int_fprintf (file, Mesh2D.EltNodes[i], eltnodeqty2D, "%d");
      }
      else
      {
        if (!fasetqty)
        {
          int data[4] = {i + shift[2], elset, elset, 0};
          fwrite (&data, sizeof (int), 4, file);
        }
        else
        {
          int data[5] = {i + shift[2], elset, elset, 0, eltfaset[i]};
          fwrite (&data, sizeof (int), 5, file);
        }
        fwrite (Mesh2D.EltNodes[i], sizeof (int), eltnodeqty2D, file);
      }
    }
  }

  // 3D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
  {
    if (!strcmp (mode, "binary"))
    {
      int data[3] = {elt_type3D, Mesh3D.EltQty, 3};
      fwrite (data, sizeof (int), 3, file);
    }

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      elset = (!Mesh3D.ElsetId) ? Mesh3D.EltElset[i]
        : Mesh3D.ElsetId[Mesh3D.EltElset[i]];

      if (!strcmp (mode, "ascii"))
      {
        fprintf (file, "%d %d 3 %d %d %d ", i + shift[3], elt_type3D,
                 elset, elset, (Part.qty > 0) ? Part.elt_parts[i] + 1 : 0);
        ut_array_1d_int_fprintf (file, Mesh3D.EltNodes[i], eltnodeqty3D,
                                 "%d");
      }
      else
      {
        int data[4] = {i + shift[3], elset, elset,
                       (Part.qty > 0) ? Part.elt_parts[i] + 1 : 0};
        fwrite (&data, sizeof (int), 4, file);
        fwrite (Mesh3D.EltNodes[i], sizeof (int), eltnodeqty3D, file);
      }
    }
  }

  // 3D mesh
  if (MeshCo.EltQty > 0)
  {
    if (!strcmp (MeshCo.EltType, "quad6"))
      ut_print_message (1, 4, "Elt type `quad6' undefined in Gmsh (the mesh will not open).\n");

    if (!strcmp (mode, "binary"))
    {
      int data[3] = {elt_typeCo, MeshCo.EltQty, 3};
      fwrite (data, sizeof (int), 3, file);
    }

    for (i = 1; i <= MeshCo.EltQty; i++)
    {
      elset = (!MeshCo.ElsetId) ? MeshCo.EltElset[i]
        : MeshCo.ElsetId[MeshCo.EltElset[i]];

      if (!strcmp (mode, "ascii"))
      {
        fprintf (file, "%d %d 3 %d %d %d ", i + shift[4], elt_typeCo,
                 elset, elset, 0);
        ut_array_1d_int_fprintf (file, MeshCo.EltNodes[i], eltnodeqtyCo,
                                 "%d");
      }
      else
      {
        int data[4] = {i + shift[4], elset, elset, 0};
        fwrite (&data, sizeof (int), 4, file);
        fwrite (MeshCo.EltNodes[i], sizeof (int), eltnodeqtyCo, file);
      }
    }
  }

  fprintf (file, "$EndElements\n");

  ut_free_1d_int (shift);
  ut_free_1d_int (eltfaset);

  return;
}

void
neut_physical_fprintf_gmsh (FILE * file, struct MESH Mesh0D,
			    struct MESH Mesh1D, struct MESH Mesh2D,
                            struct MESH Mesh3D, struct MESH MeshCo,
                            char **fasets, int *fasetids,
                            int fasetqty, char *dim)
{
  int i, physicalqty = 0;

  fprintf (file, "$PhysicalNames\n");

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "0"))
    physicalqty += Mesh0D.ElsetQty;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1"))
    physicalqty += Mesh1D.ElsetQty;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
    physicalqty += Mesh2D.ElsetQty;
  physicalqty += fasetqty;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
    physicalqty += Mesh3D.ElsetQty;
  if (MeshCo.EltQty > 0)
    physicalqty += MeshCo.ElsetQty;

  fprintf (file, "%d\n", physicalqty);

  // 0D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "0"))
    for (i = 1; i <= Mesh0D.ElsetQty; i++)
    {
      if (Mesh0D.ElsetLabels)
        fprintf (file, "0 %d %s\n", i, Mesh0D.ElsetLabels[i]);
      else
        fprintf (file, "0 %d ver%d\n", i, i);
    }

  // 1D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "1"))
    for (i = 1; i <= Mesh1D.ElsetQty; i++)
    {
      if (Mesh1D.ElsetLabels)
        fprintf (file, "1 %d %s\n", i, Mesh1D.ElsetLabels[i]);
      else
        fprintf (file, "1 %d edge%d\n", i, i);
    }

  // 2D mesh
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
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
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
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
        fprintf (file, "%d %d %s\n", MeshCo.Dimension, MeshCo.ElsetId[i], MeshCo.ElsetLabels[i]);
      else
        abort ();
    }
  }

  fprintf (file, "$EndPhysicalNames\n");

  return;
}
