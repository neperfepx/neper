/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_geof_.h"

void
neut_mesh_fscanf_geof (FILE * file, struct NODES *pNodes, struct MESH *pMesh)
{
  int i, j, eltnodeqty = 0, dim, order, qty;
  char *string = ut_alloc_1d_char (100);
  char *type = ut_alloc_1d_char (100);
  char c;
  int *nodes_old_new = NULL;
  int *elt_nbs = NULL;
  int *node_nbs = NULL;
  int *elset_nbs = NULL;

  /* read header ---------------------------------------------------- */
  if (fscanf (file, "%s", string) != 1 || strcmp (string, "***geometry") != 0)
  {
    ut_print_message (2, 0, "Start word does not match (%s).\n", string);
    /* abort (); */
  }

  /* read nodes ----------------------------------------------------- */
  if (fscanf (file, "%s", string) != 1 || strcmp (string, "**node") != 0)
  {
    ut_print_message (2, 0, "Start word does not match (%s).\n", string);
    abort ();
  }

  if (fscanf (file, "%d%d", &((*pNodes).NodeQty), &((*pMesh).Dimension)) != 2)
    abort ();

  node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  node_nbs[0] = (*pNodes).NodeQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (fscanf (file, "%d", &(node_nbs[i])) != 1)
      abort ();
    ut_array_1d_fscanf (file, (*pNodes).NodeCoo[i], 3);
  }

  /* reading elements ----------------------------------------------- */

  if (fscanf (file, "%s", string) != 1 || strcmp (string, "**element") != 0)
  {
    ut_print_message (2, 0, "Start word does not match (%s).\n", string);
    abort ();
  }

  if (fscanf (file, "%d", &((*pMesh).EltQty)) != 1)
    abort ();

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_1d_pint ((*pMesh).EltQty + 1);

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (file, "%d%s", &(elt_nbs[i]), string) != 2)
      abort ();

    neut_elt_name_prop ("geof", string, type, &dim, &order);
    if (i == 1)
    {
      (*pMesh).EltOrder = order;
      eltnodeqty = neut_elt_nodeqty (type, dim, order);
    }
    else if ((*pMesh).EltOrder != order || (*pMesh).Dimension != dim)
    {
      ut_print_messagewnc (2, 72,
                           "Mesh dimension and element order must be the same for all elements.");
      /* abort (); */
    }

    (*pMesh).EltNodes[i] = ut_alloc_1d_int (eltnodeqty);
    if ((*pMesh).EltOrder == 1)
      ut_array_1d_int_fscanf (file, (*pMesh).EltNodes[i], eltnodeqty);
    else
    {
      int status = 0;
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][0]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][1]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][2]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][4]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][5]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][6]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][7]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][9]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][8]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][3]));
      if (status != 10)
        abort ();
    }
  }

  /* reading elsets ------------------------------------------------- */
  (*pMesh).ElsetQty = ut_file_string_number (file, "**elset");

  if ((*pMesh).ElsetQty != 0)
  {
    elset_nbs = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);
    (*pMesh).Elsets = ut_alloc_1d_pint ((*pMesh).ElsetQty + 1);

    if (ut_file_string_goto (file, "**elset") != 0)
      abort ();

    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      ut_file_skip (file, 1);
      for (j = 0; j < 5; j++)
        if (fscanf (file, "%c", &c) != 1)
          abort ();

      if (fscanf (file, "%d", &(elset_nbs[i])) != 1)
        abort ();

      if (i < (*pMesh).ElsetQty)
        qty = ut_file_string_nextpos (file, "**elset");
      else
        qty = ut_file_string_nextpos (file, "***return");

      (*pMesh).Elsets[i] = ut_alloc_1d_int (qty + 1);
      (*pMesh).Elsets[i][0] = qty;

      ut_array_1d_int_fscanf (file, (*pMesh).Elsets[i] + 1, qty);
    }
  }

  ut_free_1d_char (&string);
  ut_free_1d_char (&type);

  neut_mesh_init_eltelset (pMesh, elset_nbs);

  /* neut_nodes_renumber_continuous (pNodes, node_nbs, &nodes_old_new);
   * */
  neut_mesh_renumber_continuous (pMesh, node_nbs, elt_nbs, elset_nbs);

  ut_free_1d_int (&nodes_old_new);

  return;
}

void
neut_mesh_prop_fscanf_geof (FILE * file, int *pNodeQty, int *pEltQty)
{
  if (ut_file_string_goto (file, "**node") != 0)
    abort ();

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", pNodeQty) != 1)
    abort ();

  if (ut_file_string_goto (file, "**element") != 0)
    abort ();

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", pEltQty) != 1)
    abort ();

  return;
}
