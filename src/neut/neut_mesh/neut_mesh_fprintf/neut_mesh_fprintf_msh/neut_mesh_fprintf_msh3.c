/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_msh_.h"

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
neut_mesh_fprintf_msh_nset (FILE * file, char *name, int NodeQty, int *nset)
{
  int i;

  fprintf (file, "%s\n", name);
  fprintf (file, "%d\n", NodeQty);

  for (i = 0; i < NodeQty; i++)
    fprintf (file, "%d\n", nset[i]);

  return;
}
