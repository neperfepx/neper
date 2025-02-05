/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_obj_.h"

void
neut_tess_fscanf_obj_pre (struct TESS *pTess)
{
  ut_string_string ("standard", &((*pTess).Type));
  ut_string_string ("triclinic", &((*pTess).CellCrySym));

  (*pTess).VerCoo = ut_alloc_1d_pdouble (1);

  return;
}

void
neut_tess_fscanf_obj_scangroup (char *line, char ***pgroups, int *pgroupqty, struct TESS *pTess)
{
  int pos;

  neut_tess_addpoly_alloc (pTess);

  (*pgroupqty)++;
  (*pgroups) = ut_realloc_2d_char_addline (*pgroups, (*pgroupqty) + 1, 1);

  pos = ut_string_thischarpos (line, ' ', 1);

  ut_string_string (line + pos + 1, (*pgroups) + (*pgroupqty));

  return;
}

void
neut_tess_fscanf_obj_scanver (char *line, struct TESS *pTess)
{
  double *coo = ut_alloc_1d (3);

  sscanf (line, "%*s%lf%lf%lf", coo, coo + 1, coo + 2);

  neut_tess_addver (pTess, coo);

  ut_free_1d (&coo);

  return;
}

void
neut_tess_fscanf_obj_scanface (char *line, int ***pSFaceVerNb, int *pSFaceQty, struct TESS *pTess)
{
  int i, face;
  int *facevernb = NULL;
  int *sfacevernb = NULL;
  int faceverqty;
  char **parts = NULL;
  int partqty;
  char **parts2 = NULL;
  int partqty2;

  ut_list_break (line, " ", &parts, &partqty);


  // storing face temporarily
  faceverqty = partqty - 1;

  facevernb = ut_alloc_1d_int (faceverqty + 1);
  sfacevernb = ut_alloc_1d_int (faceverqty + 1);

  for (i = 1; i <= faceverqty; i++)
  {
    ut_list_break (parts[i], "/", &parts2, &partqty2);

    facevernb[i] = atoi (parts2[0]);
    sfacevernb[i] = facevernb[i];
  }
  ut_array_1d_int_sort (sfacevernb + 1, faceverqty);

  // testing against existing faces
  face = -1;
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if (ut_array_1d_int_equal ((*pSFaceVerNb)[i] + 1, (*pTess).FaceVerQty[i], sfacevernb + 1, faceverqty))
    {
      face = i;
      break;
    }

  // recording new face
  if (face == -1)
  {
    neut_tess_addface_alloc (pTess);
    face = (*pTess).FaceQty;

    *pSFaceQty = (*pTess).FaceQty;
    *pSFaceVerNb = ut_realloc_2d_int_addline (*pSFaceVerNb, *pSFaceQty + 1, 2);
    ut_array_1d_int_memcpy ((*pTess).FaceVerNb[face] + 1, (*pTess).FaceVerQty[i], (*pSFaceVerNb)[face] + 1);
    ut_array_1d_int_sort ((*pSFaceVerNb)[face] + 1, (*pTess).FaceVerQty[face]);

    (*pTess).FaceVerQty[face] = faceverqty;

    // recording vertices
    (*pTess).FaceVerNb[face] = ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    ut_array_1d_int_memcpy (facevernb + 1, (*pTess).FaceVerQty[face], (*pTess).FaceVerNb[face] + 1);
  }

  if ((*pTess).PolyQty)
    neut_tess_poly_addface (pTess, (*pTess).PolyQty, face, 1);

  ut_free_1d_int (&facevernb);
  ut_free_1d_int (&sfacevernb);

  return;
}

// If there are duplicate vertices, removing them and renumbering faces
void
neut_tess_fscanf_obj_mergever (struct TESS *pTess)
{
  int i, j;
  int *old_new = ut_alloc_1d_int ((*pTess).VerQty + 1);
  int old_verqty = (*pTess).VerQty;
  double eps, *lengths = ut_alloc_1d (3);

  neut_tess_bboxsize (*pTess, lengths);
  eps = 1e-9 * ut_array_1d_max (lengths, 3);

  ut_array_1d_int_set_id (old_new, (*pTess).VerQty + 1);
  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = i + 1; j <= (*pTess).VerQty; j++)
      if (ut_space_dist ((*pTess).VerCoo[i], (*pTess).VerCoo[j]) < eps)
      {
        old_new[j] = i;
        (*pTess).VerState[j] = -1;
      }

  // renumbering old by new in faces
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
      (*pTess).FaceVerNb[i][j] = old_new[(*pTess).FaceVerNb[i][j]];

  // compressing vertices
  (*pTess).VerQty = 0;
  for (i = 1; i <= old_verqty; i++)
    if ((*pTess).VerState[i] == 0)
    {
      old_new[i] = ++(*pTess).VerQty;
      ut_array_1d_memcpy ((*pTess).VerCoo[i], 3, (*pTess).VerCoo[(*pTess).VerQty]);
    }

  for (i = old_verqty; i > (*pTess).VerQty; i--)
    ut_free_1d ((*pTess).VerCoo + i);

  // replacing old by new in faces again
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
      (*pTess).FaceVerNb[i][j] = old_new[(*pTess).FaceVerNb[i][j]];

  // compressing list of vertices in faces
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    ut_array_1d_int_uniq ((*pTess).FaceVerNb[i] + 1, (*pTess).FaceVerQty[i], (*pTess).FaceVerQty + i);
    if ((*pTess).FaceVerNb[i][(*pTess).FaceVerQty[i]] == (*pTess).FaceVerNb[i][1])
      (*pTess).FaceVerQty[i]--;
    (*pTess).FaceVerNb[i] = ut_realloc_1d_int ((*pTess).FaceVerNb[i], (*pTess).FaceVerQty[i] + 1);
  }

  ut_free_1d_int (&old_new);
  ut_free_1d (&lengths);

  return;
}
