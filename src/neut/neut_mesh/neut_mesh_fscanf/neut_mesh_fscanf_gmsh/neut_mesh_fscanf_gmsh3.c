/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_gmsh_.h"

int
ReadNodesHead (FILE * msh)
{
  int nodeqty;

  ut_file_skip (msh, 1);

  if (fscanf (msh, "%d", &nodeqty) != 1 || nodeqty == 0)
  {
    ut_print_message (2, 0, "Mesh has no nodes!\n");
    abort ();
  }

  return nodeqty;
}

void
ReadNodesFoot (FILE * msh, char *mode)
{
  char foot[1000];

  (void) mode;

  if (fscanf (msh, "%s", foot) != 1 || strcmp (foot, "$EndNodes") != 0)
    ut_print_message (2, 0,
                      "Reading msh file: error (`%s' instead of $EndNodes).\n",
                      foot);

  return;
}

void
ReadNodesProp (FILE * msh, char *mode, struct NODES *pNodes, int *node_nbs)
{
  int status, i, tmp;
  char c;

  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  if (!strcmp (mode, "ascii"))
    for (i = 1; i <= (*pNodes).NodeQty; i++)
    {
      status = fscanf (msh, "%d", node_nbs ? node_nbs + i : &tmp);
      if (status != 1)
        abort ();

      status = ut_array_1d_fscanf (msh, (*pNodes).NodeCoo[i], 3);
      if (status != 1)
        abort ();
    }

  else
  {
    if (fscanf (msh, "%c", &c) != 1)
      abort ();
    for (i = 1; i <= (*pNodes).NodeQty; i++)
    {
      if (fread (node_nbs ? node_nbs + i : &tmp, sizeof (int), 1, msh) != 1)
        abort ();
      if (fread ((*pNodes).NodeCoo[i], sizeof (double), 3, msh) != 3)
        abort ();
    }
  }

  return;
}

int
ReadMeshOfDim (FILE * msh, char *mode, struct MESH *pMesh, int *node_nbs,
               int Dimension, int MaxEltQty)
{
  int *elt_nbs = NULL;
  int *elset_nbs = NULL;

  // neut_mesh_free (pMesh);
  neut_mesh_set_zero (pMesh);

  if (Dimension < 0 || Dimension > 3)
    ut_print_message (2, 0, "Wrong mesh dimension: %d!\n", Dimension);

  if (MaxEltQty != 0)
  {
    (*pMesh).Dimension = Dimension;

    ReadEltsProp (msh, mode, pMesh, node_nbs ? &elt_nbs : NULL, MaxEltQty);

    SetElsets (pMesh, node_nbs ? elt_nbs : NULL,
               node_nbs ? &elset_nbs : NULL);

    if (node_nbs)
      neut_mesh_renumber_continuous (pMesh, node_nbs, elt_nbs, elset_nbs);
  }

  // recording elset_nbs in ElsetId
  if (elset_nbs)
  {
    (*pMesh).ElsetId = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);
    ut_array_1d_int_memcpy (elset_nbs + 1, (*pMesh).ElsetQty,
                            (*pMesh).ElsetId + 1);
  }

  ut_free_1d_int (&elt_nbs);
  ut_free_1d_int (&elset_nbs);

  return (*pMesh).EltQty;
}

void
ReadEltsFoot (FILE * msh)
{
  int status;
  char foot[1000];

  status = fscanf (msh, "%s", foot);

  if (status != 1 || strcmp (foot, "$EndElements") != 0)
    ut_print_message (2, 0, "Reading msh file: error.\n");

  return;
}
