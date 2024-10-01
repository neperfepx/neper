/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_msh_.h"

void
neut_mesh_fprintf_msh_entities_dim (FILE * file, char *mode, struct TESS Tess,
                                     struct NODES Nodes, struct MESH Mesh)
{
  int i, j, k, l;
  int ElsetId, DomFaceNb, InDomFace;
  double **bbox = ut_alloc_2d (3, 2);
  (void) mode;

  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    fprintf (file, "%d ", i);
    neut_mesh_elset_bbox (Nodes, Mesh, i, bbox);
    if (Mesh.Dimension == 0)
      ut_array_1d_fprintf_nonl (file, Nodes.NodeCoo[Mesh.EltNodes[i][0]], 3, REAL_PRINT_FORMAT);
    else
      fprintf (file, REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT,
               bbox[0][0], bbox[1][0], bbox[2][0],
               bbox[0][1], bbox[1][1], bbox[2][1]);

    ElsetId = Mesh.ElsetId ? Mesh.ElsetId[i] : i;

    if (Mesh.Dimension == 0)
      fprintf (file, " 1 %d\n", ElsetId);
    else if (Mesh.Dimension == 1)
    {
      fprintf (file, " 1 %d %d ", ElsetId, 2);
      ut_array_1d_int_fprintf (file, Tess.EdgeVerNb[i], 2, "%d");
    }
    else if (Mesh.Dimension == 2)
    {
      InDomFace = 0;
      for (k = 1; k <= Tess.DomFaceQty && !InDomFace; k++)
      {
        for (l = 1; l <= Tess.DomTessFaceQty[k] && !InDomFace; l++)
        {
          if (ElsetId == Tess.DomTessFaceNb[k][l])
          {
            InDomFace = 1;
            DomFaceNb = Mesh.ElsetQty + k;
          }
        }
      }
      if (InDomFace == 1)
        fprintf (file, " 2 %d %d", ElsetId, DomFaceNb);
      else
        fprintf (file, " 1 %d", ElsetId);

      fprintf (file, " %d", Tess.FaceVerQty[i]);
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
        fprintf (file, " %d", Tess.FaceEdgeNb[i][j] * Tess.FaceEdgeOri[i][j]);
      fprintf (file, "\n");
    }
    else if (Mesh.Dimension == 3)
    {
      fprintf (file, " 1 %d %d ", ElsetId, Tess.PolyFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.PolyFaceNb[i] + 1, Tess.PolyFaceQty[i], "%d");
    }
  }

  ut_free_2d (&bbox, 3);

  return;
}

void
neut_mesh_fprintf_msh_elts_dim (FILE * file, char *mode, int shift,
                                 struct MESH Mesh)
{
  int i, elttype, elset, eltnodeqty;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  if (Mesh.EltQty && neut_gmsh_elt_nb (Mesh.EltType, Mesh.Dimension,
                                       Mesh.EltOrder, &elttype))
    ut_print_neperbug ();

  if (!strcmp (mode, "binary"))
  {
    int data[3] = { elttype, Mesh.EltQty, 3 };
    fwrite (data, sizeof (int), 3, file);
  }

  if (!Mesh.EltElset)
    neut_mesh_init_eltelset (&Mesh, NULL);

  for (i = 1; i <= Mesh.EltQty; i++)
  {
    elset = Mesh.ElsetId ? Mesh.ElsetId[Mesh.EltElset[i]] : Mesh.EltElset[i];

    if (!strcmp (mode, "ascii"))
    {
      fprintf (file, "%d %d 3 %d %d %d ", i + shift, elttype, elset, elset,
                                          Mesh.EltPart ? Mesh.EltPart[i] : 0);
      ut_array_1d_int_fprintf (file, Mesh.EltNodes[i], eltnodeqty, "%d");
    }
    else
    {
      int data[4] = { i + shift, elset, elset, Mesh.EltPart ? Mesh.EltPart[i] : 0 };
      fwrite (&data, sizeof (int), 4, file);
      fwrite (Mesh.EltNodes[i], sizeof (int), eltnodeqty, file);
    }
  }

  return;
}

void
neut_mesh_fprintf_msh_elts_dim_v4 (FILE * file, char *mode, struct MESH Mesh, int shift)
{
  int i, j, elt, elttype, eltnodeqty;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  if (Mesh.EltQty && neut_gmsh_elt_nb (Mesh.EltType, Mesh.Dimension,
                                       Mesh.EltOrder, &elttype))
    ut_print_neperbug ();

  if (!Mesh.EltElset)
    neut_mesh_init_eltelset (&Mesh, NULL);

  if (!strcmp (mode, "binary"))
  {
    int data[3] = { elttype, Mesh.EltQty, 3 };
    fwrite (data, sizeof (int), 3, file);
  }

  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    // if (!strcmp (mode, "ascii"))
    {
      fprintf (file, "%d %d %d %d\n", Mesh.Dimension, i, elttype, Mesh.Elsets[i][0]);
      for (j = 1; j <= Mesh.Elsets[i][0]; j++)
      {
        elt = Mesh.Elsets[i][j];
        fprintf (file, "%d ", elt + shift);
        ut_array_1d_int_fprintf (file, Mesh.EltNodes[elt], eltnodeqty, "%d");
      }
    }
    /*
    else
      abort ();
    */
  }

  return;
}

void
neut_mesh_fprintf_msh_elts_pre (struct TESS Tess, struct MESH Mesh0D, struct MESH Mesh1D,
                                 struct MESH Mesh2D, struct MESH Mesh3D, struct MESH MeshCo,
                                 char **fasets, int fasetqty, int *fasetids, char *dim,
                                 char *numbering, int **peltfaset, int *shift, int *peltqty)
{
  int i, j, *tmpelts = NULL, tmpeltqty;

  if (fasetqty > 0 && !neut_mesh_isvoid (Mesh2D))
  {
    (*peltfaset) = ut_alloc_1d_int (Mesh2D.EltQty + 1);

    for (i = 0; i < fasetqty; i++)
    {
      neut_mesh_domface_elts (Tess, Mesh2D, fasets[i], &tmpelts, &tmpeltqty);

      for (j = 0; j < tmpeltqty; j++)
        (*peltfaset)[tmpelts[j]] = fasetids[i] + Mesh2D.ElsetQty;
    }
  }

  (*peltqty) = 0;
  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0"))
  {
    shift[0] = 0;
    (*peltqty) += Mesh0D.EltQty;
  }

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[1] = (*peltqty);
    else if (!strcmp (numbering, "from1"))
      shift[1] = 0;
    else
      ut_print_neperbug ();

    (*peltqty) += Mesh1D.EltQty;
  }

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[2] = (*peltqty);
    else if (!strcmp (numbering, "from1"))
      shift[2] = 0;
    else
      ut_print_neperbug ();

    (*peltqty) += Mesh2D.EltQty;
  }

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[3] = (*peltqty);
    else if (!strcmp (numbering, "from1"))
      shift[3] = 0;
    else
      ut_print_neperbug ();

    (*peltqty) += Mesh3D.EltQty;
  }

  if (MeshCo.EltQty > 0)
  {
    if (numbering == NULL || !strcmp (numbering, "contiguous"))
      shift[4] = (*peltqty);
    else if (!strcmp (numbering, "from1"))
      shift[4] = 0;
    else
      ut_print_neperbug ();

    (*peltqty) += MeshCo.EltQty;
  }

  ut_free_1d_int (&tmpelts);

  return;
}

void
neut_mesh_fprintf_msh_nodes_dim_v4 (FILE * file, char *mode, struct TESS Tess,
                                     struct NODES Nodes,
                                     struct MESH MeshL, struct MESH MeshU)
{
  int i, j, dim = MeshU.Dimension;
  int *nodes = NULL, NodeQty;
  (void) mode;

  if (!MeshU.Dimension)
  {
    NodeQty = 1;
    nodes = ut_alloc_1d_int (NodeQty);
  }

  for (i = 1; i <= MeshU.ElsetQty; i++)
  {
    if (dim == 0)
      nodes[0] = MeshU.EltNodes[i][0];
    else
      neut_mesh_elset_bodynodes (Tess, MeshL, MeshU, i, &nodes, &NodeQty);

    fprintf (file, "%d %d 0 %d\n", MeshU.Dimension, i, NodeQty);

    for (j = 0; j < NodeQty; j++)
      fprintf (file, "%d\n", nodes[j]);

    for (j = 0; j < NodeQty; j++)
      ut_array_1d_fprintf (file, Nodes.NodeCoo[nodes[j]], 3, REAL_PRINT_FORMAT);
  }

  ut_free_1d_int (&nodes);

  return;
}

void
neut_mesh_fprintf_msh_nset (FILE * file, char *name, int NodeQty, int *nset)
{
  int i;

  fprintf (file, "%s\n", name);
  fprintf (file, "%d\n", NodeQty);

  for (i = 0; i < NodeQty; i++)
    fprintf (file, "%d\n", nset[i]);

  return;
}
