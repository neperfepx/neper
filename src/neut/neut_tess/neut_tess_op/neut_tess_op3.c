/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */

#include "neut_tess_op_.h"

void
neut_tess_init_domain_label_3d (struct TESS *pTess)
{
  int i, j;

  if ((*pTess).Dim != 3)
    ut_print_neperbug ();

  if ((!strcmp ((*pTess).DomType, "cube")
       || !strcmp ((*pTess).DomType, "square")
       || (!strcmp ((*pTess).DomType, "cut") && (*pTess).DomVerQty == 8
           && (*pTess).DomEdgeQty == 12 && (*pTess).DomFaceQty == 6)))
  {
    strcpy ((*pTess).DomType, "cube");
    (*pTess).DomFaceLabel = ut_alloc_2d_char (7, 3);

    double *tmp = ut_alloc_1d (6);
    double *cst = ut_alloc_1d (2);
    int *id = ut_alloc_1d_int (6);
    int fixqty = 0;

    for (i = 1; i <= 3; i++)
    {
      for (j = 1; j <= 6; j++)
        tmp[j - 1] = fabs (fabs ((*pTess).DomFaceEq[j][i]) - 1);

      ut_array_1d_sort_index (tmp, 6, id);
      ut_array_1d_int_addval (id, 6, 1, id);

      for (j = 0; j < 2; j++)
        cst[j] = (*pTess).DomFaceEq[id[j]][i] * (*pTess).DomFaceEq[id[j]][0];

      if (cst[0] > cst[1])
        ut_array_1d_int_reverseelts (id, 2);

      for (j = 0; j < 2; j++)
      {
        // In the case of a rotated/scaled domain, it may not be possible
        // to determine which is which.  If a face has already been assigned
        // a label, we set it to the default 'f*'.
        if (strlen ((*pTess).DomFaceLabel[id[j]]) == 0)
          sprintf ((*pTess).DomFaceLabel[id[j]], "%c%d", 'x' + i - 1, j);
        else
          sprintf ((*pTess).DomFaceLabel[id[j]], "f%d", ++fixqty);
      }
    }

    // In the case of a rotated/scaled domain, it may not be possible
    // to determine which is which.  If a face has not been assigned
    // a label, we set it to the default 'f*'.
    for (i = 1; i <= 6; i++)
      if (strlen ((*pTess).DomFaceLabel[i]) == 0)
        sprintf ((*pTess).DomFaceLabel[i], "f%d", ++fixqty);

    ut_free_1d (&tmp);
    ut_free_1d_int (&id);
    ut_free_1d (&cst);
  }

  else if (!strcmp ((*pTess).DomType, "cylinder")
           || !strcmp ((*pTess).DomType, "circle"))
  {
    (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
    strcpy ((*pTess).DomFaceLabel[1], "z0");
    strcpy ((*pTess).DomFaceLabel[2], "z1");
    for (i = 3; i <= (*pTess).DomFaceQty; i++)
      sprintf ((*pTess).DomFaceLabel[i], "f%d", i - 2);
  }

  else if (!strcmp ((*pTess).DomType, "stdtriangle"))
  {
    (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
    strcpy ((*pTess).DomFaceLabel[1], "z0");
    strcpy ((*pTess).DomFaceLabel[2], "z1");
    for (i = 3; i <= (*pTess).DomFaceQty; i++)
      sprintf ((*pTess).DomFaceLabel[i], "f%d", i - 2);
  }

  else if (!strncmp ((*pTess).DomType, "rodrigues", 9))
  {
    (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
    {
      for (j = 1; j <= 3; j++)
        if (ut_num_equal (fabs ((*pTess).DomFaceEq[i][j]), 1, 1e-3))
        {
          if ((*pTess).DomFaceEq[i][0] * (*pTess).DomFaceEq[i][j] < 0)
            sprintf ((*pTess).DomFaceLabel[i], "%c0", 'x' + j - 1);
          else
            sprintf ((*pTess).DomFaceLabel[i], "%c1", 'x' + j - 1);
        }
    }

    for (i = 1; i <= (*pTess).DomFaceQty; i++)
    {
      if (strlen ((*pTess).DomFaceLabel[i]) == 0)
        sprintf ((*pTess).DomFaceLabel[i], "f%d", i);
    }
  }

  else if (strcmp ((*pTess).DomType, "cut"))
  {
    (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
      sprintf ((*pTess).DomFaceLabel[i], "f%d", i);
  }

  (*pTess).DomEdgeLabel = ut_alloc_1d_pchar ((*pTess).DomEdgeQty + 1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    int *face = (*pTess).DomEdgeFaceNb[i];
    int length =
      strlen ((*pTess).DomFaceLabel[face[0]]) +
      strlen ((*pTess).DomFaceLabel[face[1]]) + 1;
    (*pTess).DomEdgeLabel[i] = ut_alloc_1d_char (length);
    if (strcmp
        ((*pTess).DomFaceLabel[face[0]], (*pTess).DomFaceLabel[face[1]]) < 0)
      sprintf ((*pTess).DomEdgeLabel[i], "%s%s",
               (*pTess).DomFaceLabel[face[0]],
               (*pTess).DomFaceLabel[face[1]]);
    else
      sprintf ((*pTess).DomEdgeLabel[i], "%s%s",
               (*pTess).DomFaceLabel[face[1]],
               (*pTess).DomFaceLabel[face[0]]);
  }

  (*pTess).DomVerLabel = ut_alloc_1d_pchar ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    char **fnames = NULL;
    int *domface = NULL;
    int domfaceqty;

    neut_tess_domver_domface (*pTess, i, &domface, &domfaceqty);

    fnames = ut_alloc_1d_pchar (domfaceqty);
    for (j = 0; j < domfaceqty; j++)
      ut_string_string ((*pTess).DomFaceLabel[domface[j]], fnames + j);
    ut_array_1d_pchar_sort (fnames, domfaceqty);
    (*pTess).DomVerLabel[i] = ut_string_array_paste (fnames, domfaceqty, NULL);

    ut_free_2d_char (&fnames, domfaceqty);
    ut_free_1d_int (&domface);
  }

  return;
}

void
neut_tess_init_domain_label_2d (struct TESS *pTess)
{
  int i, j;
  int *ver = ut_alloc_1d_int (2);

  if ((*pTess).Dim != 2)
    ut_print_neperbug ();

  if (!strcmp ((*pTess).DomType, "square")
      || ((*pTess).DomVerQty == 4 && (*pTess).DomEdgeQty == 4))
  {
    strcpy ((*pTess).DomType, "square");
    (*pTess).DomEdgeLabel = ut_alloc_2d_char (5, 3);

    double *tmp = ut_alloc_1d (6);
    double *cst = ut_alloc_1d (2);
    int *id = ut_alloc_1d_int (6);
    double **DomEdgeEq = ut_alloc_2d (5, 3);
    int fixqty = 0;

    for (j = 1; j <= 4; j++)
    {
      for (i = 0; i < (*pTess).DomEdgeVerQty[j]; i++)
        ver[i] = (*pTess).DomTessVerNb[(*pTess).DomEdgeVerNb[j][i]];

      ut_space_points_line ((*pTess).VerCoo[ver[0]], (*pTess).VerCoo[ver[1]],
                            DomEdgeEq[j]);
    }

    for (i = 1; i <= 2; i++)
    {
      for (j = 1; j <= 4; j++)
        tmp[j - 1] = fabs (fabs (DomEdgeEq[j][i]) - 1);

      ut_array_1d_sort_index (tmp, 4, id);
      ut_array_1d_int_addval (id, 4, 1, id);

      for (j = 0; j < 2; j++)
        cst[j] = DomEdgeEq[id[j]][i] * DomEdgeEq[id[j]][0];

      if (cst[0] > cst[1])
        ut_array_1d_int_reverseelts (id, 2);

      for (j = 0; j < 2; j++)
      {
        // In the case of a rotated/scaled domain, it may not be possible
        // to determine which is which.  If an edge has already been assigned
        // a label, we set it to the default 'f*'.
        if (strlen ((*pTess).DomEdgeLabel[id[j]]) == 0)
          sprintf ((*pTess).DomEdgeLabel[id[j]], "%c%d", 'x' + i - 1, j);
        else
          sprintf ((*pTess).DomEdgeLabel[id[j]], "f%d", ++fixqty);
      }
    }

    // In the case of a rotated/scaled domain, it may not be possible
    // to determine which is which.  If an edge has not been assigned
    // a label, we set it to the default 'f*'.
    for (i = 1; i <= 4; i++)
      if (strlen ((*pTess).DomEdgeLabel[i]) == 0)
        sprintf ((*pTess).DomEdgeLabel[i], "f%d", ++fixqty);

    ut_free_1d (&tmp);
    ut_free_1d_int (&id);
    ut_free_1d (&cst);
  }

  else
  {
    (*pTess).DomEdgeLabel = ut_alloc_2d_char ((*pTess).DomEdgeQty + 1, 10);
    for (i = 1; i <= (*pTess).DomEdgeQty; i++)
      sprintf ((*pTess).DomEdgeLabel[i], "f%d", i);
  }

  (*pTess).DomVerLabel = ut_alloc_1d_pchar ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    char **enames = NULL;

    enames = ut_alloc_1d_pchar ((*pTess).DomVerEdgeQty[i]);
    for (j = 0; j < (*pTess).DomVerEdgeQty[i]; j++)
      ut_string_string ((*pTess).DomEdgeLabel[(*pTess).DomVerEdgeNb[i][j]],
                        enames + j);
    ut_array_1d_pchar_sort (enames, (*pTess).DomVerEdgeQty[i]);
    (*pTess).DomVerLabel[i] =
      ut_string_array_paste (enames, (*pTess).DomVerEdgeQty[i], NULL);

    ut_free_2d_char (&enames, (*pTess).DomVerEdgeQty[i]);
  }

  ut_free_1d_int (&ver);

  return;
}

void
neut_tess_init_domain_label_1d (struct TESS *pTess)
{
  int i;

  if ((*pTess).Dim != 1)
    ut_print_neperbug ();

  (*pTess).DomVerLabel = ut_alloc_1d_pchar ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    (*pTess).DomVerLabel[i] = ut_alloc_1d_char (3);
    sprintf ((*pTess).DomVerLabel[i], "x%d", i - 1);
  }

  return;
}
