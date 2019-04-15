/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_fepx_.h"

void
neut_mesh_fscanf_fepx (FILE * parms, FILE * mesh, FILE * elsets,
		       struct NODES *pNodes, struct MESH *pMesh)
{
  int i, tmp, eltnodeqty = 0;
  char *string = ut_alloc_1d_char (1000);
  int *elt_nbs = NULL;
  int *node_nbs = NULL;
  int *elset_eltqty = NULL;

  /* read parms ----------------------------------------------------- */
  if (fscanf (parms, "%d%d", &(*pMesh).EltQty, &(*pNodes).NodeQty) != 2)
    abort ();

  (*pMesh).Dimension = 3;

  /* determining the elt order from the number of words on the first
   * line (elt_id + its nodes) */
  ut_file_line_nbwords_pointer (mesh, &eltnodeqty);
  eltnodeqty--;
  fseek (mesh, 0, 0);

  (*pMesh).EltOrder = neut_elt_order ((*pMesh).EltType, 3, eltnodeqty);
  if ((*pMesh).EltOrder == -1)
  {
    ut_print_message (2, 0, "mesh file has a bad format.\n");
    abort ();
  }

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  /* reading mesh --------------------------------------------------- */
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (mesh, "%d", &(elt_nbs[i])) != 1)
      abort ();

    elt_nbs[i]++;
    if ((*pMesh).EltOrder == 1)
      ut_array_1d_int_fscanf (mesh, (*pMesh).EltNodes[i], eltnodeqty);
    else
    {
      int status = 0;
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][0]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][4]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][1]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][5]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][2]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][6]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][7]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][9]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][8]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][3]));
      if (status != 10)
	abort ();
    }

    ut_array_1d_int_addval ((*pMesh).EltNodes[i], eltnodeqty, 1,
			    (*pMesh).EltNodes[i]);
  }

  ut_file_skip (mesh, 3);	/* skipping 1.0 1.0 1.0 */

  /* read nodes ----------------------------------------------------- */
  node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  node_nbs[0] = (*pNodes).NodeQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (fscanf (mesh, "%d", &(node_nbs[i])) != 10)
      abort ();
    node_nbs[i]++;
    ut_array_1d_fscanf (mesh, (*pNodes).NodeCoo[i], 3);
  }

  /* read elsets ---------------------------------------------------- */
  ut_file_skip (elsets, 1);	// skipping "grain-input"
  if (fscanf (elsets, "%d", &tmp) != 1)
    abort ();

  if (tmp != (*pMesh).EltQty)
  {
    ut_print_message (2, 0,
		      "Elt quantities in parms and opt do not match!\n");
    abort ();
  }
  if (fscanf (elsets, "%d", &((*pMesh).ElsetQty)) != 1)
    abort ();

  /* searching number of element per elset for allocation */
  elset_eltqty = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (elsets, "%d", &tmp) != 1)
      abort ();

    ut_file_skip (elsets, 1);	// skipping phase
    elset_eltqty[tmp]++;
  }

  (*pMesh).Elsets = ut_alloc_1d_pint ((*pMesh).ElsetQty + 1);
  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    (*pMesh).Elsets[i] = ut_alloc_1d_int (elset_eltqty[i] + 1);
  ut_free_1d_int (elset_eltqty);

  fseek (elsets, 0, SEEK_SET);
  ut_file_skip (elsets, 3);

  /* assigning each element to its elset */
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (elsets, "%d", &tmp) != 1)
      abort ();

    ut_file_skip (elsets, 1);	// skipping phase
    (*pMesh).Elsets[tmp][++(*pMesh).Elsets[tmp][0]] = i;
  }

  ut_free_1d_char (string);
  ut_free_1d_int (node_nbs);
  ut_free_1d_int (elt_nbs);

  return;
}
