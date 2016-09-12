/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_gmsh_.h"

/* RAM */
void
ReadEltsProp (FILE * msh, struct MESH *pMesh, int **pelt_nbs, int MaxEltQty)
{
  int i, j, tmp = 0, elttype = -1, eltnodeqty, status;
  fpos_t beg_mesh, beg_elt;
  int tag_qty;
  int contiguous = 0;
  int dim, order;
  char *type = ut_alloc_1d_char (5);

  if (pelt_nbs == NULL)
    contiguous = 1;

  if (MaxEltQty == 0)
  {
    neut_mesh_set_zero (pMesh);
    return;
  }

  fgetpos (msh, &beg_mesh);

  /* determining mesh info */

  status = -1;
  if (fscanf (msh, "%d%d", &tmp, &elttype) == 2)
    status = neut_elt_gtype_prop (elttype, type, &dim, &order);

  if (status != 0 || dim != (*pMesh).Dimension)
  {
    neut_mesh_set_zero (pMesh);
    fsetpos (msh, &beg_mesh);
    ut_free_1d_char (type);
    return;
  }

  (*pMesh).EltType = ut_alloc_1d_char (strlen (type) + 1);
  sprintf ((*pMesh).EltType, "%s", type);
  (*pMesh).EltOrder = order;

  eltnodeqty =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
		      (*pMesh).EltOrder);

  /* recording mesh */

  /* first pass to determine EltQty, for allocation */

  fsetpos (msh, &beg_mesh);

  (*pMesh).EltQty = 0;

  status = 1;
  while (status)
  {
    ut_file_skip (msh, 1);
    if (fscanf (msh, "%d", &tmp) != 1)
      status = 0;
    else
    {
      if (fscanf (msh, "%d", &tag_qty) != 1)
	abort ();

      ut_file_skip (msh, tag_qty + eltnodeqty);

      if (tmp == elttype)
	(*pMesh).EltQty++;
      else
	status = 0;
    }
  }

  fsetpos (msh, &beg_mesh);

  if (contiguous == 0)
    (*pelt_nbs) = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);
  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  /* now, recording elements */

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    /* reading info */
    fgetpos (msh, &beg_elt);

    if (contiguous == 0)
      if (fscanf (msh, "%d", &((*pelt_nbs)[i])) != 1)
	abort ();

    ut_file_skip (msh, 1 + contiguous);

    if (fscanf (msh, "%d", &tag_qty) != 1)
      abort ();

    ut_file_skip (msh, 1);
    if (fscanf (msh, "%d", &((*pMesh).EltElset[i])) != 1)
      abort ();

    ut_file_skip (msh, tag_qty - 2);
    for (j = 0; j < eltnodeqty; j++)
      if (fscanf (msh, "%d", &(*pMesh).EltNodes[i][j]) != 1)
	abort ();
  }

  ut_free_1d_char (type);

  return;
}

/* RAM (tmp) */
void
SetElsets (struct MESH *pMesh, int *elt_nbs, int **pelset_nbs)
{
  int i, qty, elset, elset_inv;
  int elset_nb_max;
  int *elsets_old_new = NULL;
  int contiguous = 0;

  if (elt_nbs == NULL)
    contiguous = 1;

  if ((*pMesh).EltQty <= 0)
    return;

  if (contiguous == 0)
  {
    (*pelset_nbs) = ut_alloc_1d_int ((*pMesh).EltQty + 1);

    ut_array_1d_int_memcpy ((*pelset_nbs) + 1, (*pMesh).EltQty,
			    (*pMesh).EltElset + 1);

    ut_array_1d_int_sort_uniq ((*pelset_nbs) + 1, (*pMesh).EltQty,
			       &((*pelset_nbs)[0]));

    (*pelset_nbs) = ut_realloc_1d_int ((*pelset_nbs), (*pelset_nbs)[0] + 1);

    (*pMesh).ElsetQty = (*pelset_nbs)[0];

    elset_nb_max = ut_array_1d_int_max ((*pelset_nbs) + 1, (*pMesh).ElsetQty);

    elsets_old_new = ut_alloc_1d_int (elset_nb_max + 1);
    for (i = 1; i <= (*pelset_nbs)[0]; i++)
      elsets_old_new[(*pelset_nbs)[i]] = i;

    /* memory allocation of Elsets */
    (*pMesh).Elsets = ut_alloc_1d_pint ((*pMesh).ElsetQty + 1);
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      qty =
	ut_array_1d_int_nbofthisval ((*pMesh).EltElset + 1, (*pMesh).EltQty,
				     (*pelset_nbs)[i]);
      (*pMesh).Elsets[i] = ut_alloc_1d_int (qty + 1);
    }

    /* filling of Elsets */
    for (i = 1; i <= (*pMesh).EltQty; i++)
    {
      elset = (*pMesh).EltElset[i];
      elset_inv = elsets_old_new[elset];
      (*pMesh).Elsets[elset_inv][0]++;
      (*pMesh).Elsets[elset_inv][(*pMesh).Elsets[elset_inv][0]] = elt_nbs[i];
    }

    ut_free_1d_int (elsets_old_new);
  }
  else
  {
    (*pMesh).ElsetQty =
      ut_array_1d_int_max ((*pMesh).EltElset + 1, (*pMesh).EltQty);

    /* memory allocation of Elsets */
    (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1);
    for (i = 1; i <= (*pMesh).EltQty; i++)
      (*pMesh).Elsets[(*pMesh).EltElset[i]][0]++;

    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      (*pMesh).Elsets[i] = ut_realloc_1d_int ((*pMesh).Elsets[i],
					      (*pMesh).Elsets[i][0] + 1);
      (*pMesh).Elsets[i][0] = 0;
    }

    /* filling of Elsets */
    for (i = 1; i <= (*pMesh).EltQty; i++)
    {
      elset = (*pMesh).EltElset[i];
      (*pMesh).Elsets[elset][0]++;
      (*pMesh).Elsets[elset][(*pMesh).Elsets[elset][0]] = i;
    }
  }

  return;
}
