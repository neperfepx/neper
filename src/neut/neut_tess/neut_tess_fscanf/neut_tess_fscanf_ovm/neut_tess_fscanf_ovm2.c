/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_ovm_.h"

void
neut_tess_fscanf_ovm_header (FILE *file)
{
  if (!ut_file_string_scanandtest (file, "OVM")
      || !ut_file_string_scanandtest (file, "ASCII"))
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  return;
}

void
neut_tess_fscanf_ovm_ver (FILE *file, struct TESS *pTess)
{
  if (!ut_file_string_scanandtest (file, "Vertices"))
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  if (fscanf (file, "%d", &(*pTess).VerQty) != 1)
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  ut_array_2d_fscanf (file, (*pTess).VerCoo + 1, (*pTess).VerQty, 3);

  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  return;
}

void
neut_tess_fscanf_ovm_edge (FILE *file, struct TESS *pTess)
{
  if (!ut_file_string_scanandtest (file, "Edges"))
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  if (fscanf (file, "%d", &(*pTess).EdgeQty) != 1)
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  ut_array_2d_int_fscanf (file, (*pTess).EdgeVerNb + 1, (*pTess).EdgeQty, 2);
  ut_array_2d_int_addval ((*pTess).EdgeVerNb + 1, (*pTess).EdgeQty, 2, 1, (*pTess).EdgeVerNb + 1);

  neut_tess_init_edgelength (pTess);
  neut_tess_init_veredge (pTess);

  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  return;
}

void
neut_tess_fscanf_ovm_face (FILE *file, struct TESS *pTess)
{
  int i, j;

  if (!ut_file_string_scanandtest (file, "Faces"))
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  if (fscanf (file, "%d", &(*pTess).FaceQty) != 1)
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);

  int edgeqty;
  int *edges = NULL;
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    if (fscanf (file, "%d", &edgeqty) != 1)
      ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

    edges = ut_alloc_1d_int (edgeqty);
    ut_array_1d_int_fscanf (file, edges, edgeqty);
    for (j = 0; j < edgeqty; j++)
      edges[j] = edges[j] / 2 + 1;

    neut_tess_face_addedges (pTess, i, edges, edgeqty);
  }

  neut_tess_init_edgeface (pTess);

  (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  neut_tess_init_faceeq (pTess);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    neut_tess_face_fixorifromedges (*pTess, i, (*pTess).FaceEq[i]);

  return;
}

void
neut_tess_fscanf_ovm_poly (FILE *file, struct TESS *pTess)
{
  int id, i, j, face, faceqty;

  if (!ut_file_string_scanandtest (file, "Polyhedra"))
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  if (fscanf (file, "%d", &(*pTess).PolyQty) != 1)
    ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
  {
    if (fscanf (file, "%d", &faceqty) != 1)
      ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

    for (j = 1; j <= faceqty; j++)
    {
      if (fscanf (file, "%d", &face) != 1)
        ut_print_message (2, 0, "Input file is not a valid OVM file.\n");

      face = face / 2 + 1;

      neut_tess_poly_addface (pTess, i, face, 1);
    }
  }

  // Setting 0 polys to -1 to allow for minimal domain reconstruction
  id = 0;
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FacePoly[i][1] == 0)
      (*pTess).FacePoly[i][1] = -(++id);

  neut_tess_init_polyfaceori (pTess);

  return;
}
