/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_msh_.h"

/* RAM */
void
neut_mesh_fscanf_msh_elts_dim_prop (FILE * file, char *mode, struct MESH *pMesh, int **pelt_nbs,
              int MaxEltQty)
{
  int i, j, tmp = 0, elttype = -1, eltnodeqty, status, dim, order, EltQty;
  fpos_t beg_mesh, beg_elt;
  int tagqty;
  int contiguous = 0;
  char *type = ut_alloc_1d_char (10);

  if ((*pMesh).EltQty != 0)
    abort ();

  if (pelt_nbs == NULL)
    contiguous = 1;

  if (MaxEltQty == 0)
    return;

  fgetpos (file, &beg_mesh);

  /* determining mesh info */

  if (!strcmp (mode, "ascii"))
  {
    status = -1;
    if (fscanf (file, "%d%d", &tmp, &elttype) == 2)
      status = neut_elt_gtype_prop (elttype, type, &dim, &order);

    if (status != 0 || dim != (*pMesh).Dimension)
    {
      neut_mesh_set_zero (pMesh);
      fsetpos (file, &beg_mesh);
      ut_free_1d_char (&type);
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

    fsetpos (file, &beg_mesh);

    (*pMesh).EltQty = 0;

    status = 1;
    while (status)
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%d", &tmp) != 1)
        status = 0;
      else
      {
        if (fscanf (file, "%d", &tagqty) != 1)
          abort ();

        ut_file_skip (file, tagqty + eltnodeqty);

        if (tmp == elttype)
          (*pMesh).EltQty++;
        else
          status = 0;
      }
    }

    fsetpos (file, &beg_mesh);

    if (contiguous == 0)
      (*pelt_nbs) = ut_alloc_1d_int ((*pMesh).EltQty + 1);

    (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);
    (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

    /* now, recording elements */

    (*pMesh).EltPart = ut_alloc_1d_int ((*pMesh).EltQty + 1);
    for (i = 1; i <= (*pMesh).EltQty; i++)
    {
      /* reading info */
      fgetpos (file, &beg_elt);

      if (contiguous == 0)
        if (fscanf (file, "%d", &((*pelt_nbs)[i])) != 1)
          abort ();

      ut_file_skip (file, 1 + contiguous);

      if (fscanf (file, "%d", &tagqty) != 1)
        abort ();

      ut_file_skip (file, 1);
      if (fscanf (file, "%d", &((*pMesh).EltElset[i])) != 1)
        abort ();

      ut_file_skip (file, tagqty - 3);
      if (fscanf (file, "%d", (*pMesh).EltPart + i) != 1)
        abort ();
      for (j = 0; j < eltnodeqty; j++)
        if (fscanf (file, "%d", &(*pMesh).EltNodes[i][j]) != 1)
          abort ();
    }

    (*pMesh).PartQty =  ut_array_1d_int_max ((*pMesh).EltPart + 1, (*pMesh).EltQty);
    if ((*pMesh).PartQty)
      neut_mesh_init_parts (pMesh);
    else
      ut_free_1d_int (&(*pMesh).EltPart);
  }

  else
  {
    (*pMesh).EltNodes = ut_alloc_1d_pint (MaxEltQty + 1);
    (*pMesh).EltElset = ut_alloc_1d_int (MaxEltQty + 1);

    (*pMesh).EltQty = 0;
    while ((*pMesh).EltQty < MaxEltQty)
    {
      fgetpos (file, &beg_mesh);
      status = -1;
      if (fread (&elttype, sizeof (int), 1, file) == 1
          && fread (&EltQty, sizeof (int), 1, file) == 1
          && fread (&tagqty, sizeof (int), 1, file) == 1)
        status = neut_elt_gtype_prop (elttype, type, &dim, &order);

      if (status != 0 || dim != (*pMesh).Dimension)
      {
        fsetpos (file, &beg_mesh);
        break;
      }

      ut_string_string (type, &(*pMesh).EltType);
      (*pMesh).EltOrder = order;

      eltnodeqty =
        neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                          (*pMesh).EltOrder);

      if (pelt_nbs)
        (*pelt_nbs) =
          ut_realloc_1d_int (*pelt_nbs, (*pMesh).EltQty + EltQty + 1);

      for (i = 1; i <= EltQty; i++)
      {
        (*pMesh).EltQty++;
        if (fread (pelt_nbs ? (*pelt_nbs) + (*pMesh).EltQty : &tmp, sizeof (int), 1, file)
            != 1)
          abort ();
        if (fread ((*pMesh).EltElset + (*pMesh).EltQty, sizeof (int), 1, file) != 1)
          abort ();

        for (j = 0; j < tagqty - 1; j++)
          if (fread (&tmp, sizeof (int), 1, file) != 1)
            abort ();
        (*pMesh).EltNodes[(*pMesh).EltQty] = ut_alloc_1d_int (eltnodeqty);
        if (fread ((*pMesh).EltNodes[(*pMesh).EltQty], sizeof (int), eltnodeqty, file) !=
            (unsigned int) eltnodeqty)
          abort ();
      }
    }

    (*pMesh).EltNodes = ut_realloc_1d_pint ((*pMesh).EltNodes, (*pMesh).EltQty + 1);
    (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty + 1);
  }

  ut_free_1d_char (&type);

  return;
}
