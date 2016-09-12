/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
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
ReadNodesFoot (FILE * msh)
{
  char foot[1000];

  if (fscanf (msh, "%s", foot) != 1 || strcmp (foot, "$EndNodes") != 0)
    ut_print_message (2, 0, "Reading msh file: error.\n");

  return;
}

void
ReadNodesProp (FILE * msh, struct NODES *pNodes, int *node_nbs)
{
  int status, i, j, tmp;
  int contiguous = 0;

  if (node_nbs == NULL)
    contiguous = 1;

  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (contiguous == 0)
      status = fscanf (msh, "%d", &(node_nbs[i]));
    else
      status = fscanf (msh, "%d", &tmp);

    if (status != 1)
      abort ();

    for (j = 0; j < 3; j++)
    {
      status = fscanf (msh, "%lf", &(*pNodes).NodeCoo[i][j]);
      if (status != 1)
	abort ();
    }
  }

  return;
}

int
ReadMeshOfDim (FILE * msh, struct MESH *pMesh, int *node_nbs,
	       int Dimension, int MaxEltQty)
{
  int *elt_nbs = NULL;
  int *elset_nbs = NULL;
  int contiguous = 0;

  if (node_nbs == NULL)
    contiguous = 1;

  neut_mesh_free (pMesh);

  if (Dimension < 0 || Dimension > 3)
  {
    ut_print_message (2, 0, "Wrong mesh dimension: %d!\n", Dimension);
    abort ();
  }

  if (MaxEltQty != 0)
  {
    (*pMesh).Dimension = Dimension;

    if (contiguous == 0)
      ReadEltsProp (msh, pMesh, &elt_nbs, MaxEltQty);
    else
      ReadEltsProp (msh, pMesh, NULL, MaxEltQty);

    if (contiguous == 0)
      SetElsets (pMesh, elt_nbs, &elset_nbs);
    else
      SetElsets (pMesh, NULL, NULL);

    if (contiguous == 0)
      neut_mesh_renumber_continuous (pMesh, node_nbs, elt_nbs, elset_nbs);
  }

  // recording elset_nbs in ElsetId
  if (elset_nbs)
  {
    (*pMesh).ElsetId = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);
    ut_array_1d_int_memcpy ((*pMesh).ElsetId + 1, (*pMesh).ElsetQty,
			    elset_nbs + 1);
  }

  ut_free_1d_int (elt_nbs);
  ut_free_1d_int (elset_nbs);

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
