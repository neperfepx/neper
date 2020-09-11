/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_msh_.h"

void
neut_mesh_fscanf_msh_nodes_head (FILE * file, int *pnodeqty)
{
  ut_file_skip (file, 1);

  if (fscanf (file, "%d", pnodeqty) != 1 || (*pnodeqty) == 0)
  {
    ut_print_message (2, 0, "Mesh has no nodes!\n");
    abort ();
  }

  return;
}

void
neut_mesh_fscanf_msh_nodes_prop (FILE * file, char *mode, struct NODES *pNodes, int *node_nbs)
{
  int status, i;
  char c;

  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  if (!strcmp (mode, "ascii"))
    for (i = 1; i <= (*pNodes).NodeQty; i++)
    {
      status = fscanf (file, "%d", node_nbs + i);
      if (status != 1)
        abort ();

      status = ut_array_1d_fscanf (file, (*pNodes).NodeCoo[i], 3);
      if (status != 1)
        abort ();
    }

  else
  {
    if (fscanf (file, "%c", &c) != 1)
      abort ();
    for (i = 1; i <= (*pNodes).NodeQty; i++)
    {
      if (fread (node_nbs + i, sizeof (int), 1, file) != 1)
        abort ();
      if (fread ((*pNodes).NodeCoo[i], sizeof (double), 3, file) != 3)
        abort ();
    }
  }

  return;
}

void
neut_mesh_fscanf_msh_nodes_foot (FILE * file)
{
  char foot[1000];

  if (fscanf (file, "%s", foot) != 1 || strcmp (foot, "$EndNodes") != 0)
    ut_print_message (2, 0,
                      "Reading msh file: error (`%s' instead of $EndNodes).\n",
                      foot);

  return;
}

int
neut_mesh_fscanf_msh_elts_dim (FILE * file, char *mode, struct MESH *pMesh, int *node_nbs,
               int Dimension, int MaxEltQty)
{
  int *elt_nbs = NULL;
  int *elset_nbs = NULL;

  neut_mesh_reset (pMesh);

  if (Dimension < 0 || Dimension > 3)
    ut_print_message (2, 0, "Wrong mesh dimension: %d!\n", Dimension);

  if (MaxEltQty != 0)
  {
    (*pMesh).Dimension = Dimension;

    neut_mesh_fscanf_msh_elts_dim_prop (file, mode, pMesh, &elt_nbs,
                                        MaxEltQty);

    neut_mesh_init_elsets_2 (pMesh, elt_nbs, &elset_nbs);

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
