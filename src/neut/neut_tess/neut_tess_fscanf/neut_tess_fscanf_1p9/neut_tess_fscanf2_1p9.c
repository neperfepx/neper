/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fscanf_1p9_.h"

/* Tessellation exportation: head */
void
neut_tess_fscanf_head_1p9 (struct TESS *pTess, FILE * file)
{
  int status;
  char *tmp = ut_alloc_1d_char (100);

  (*pTess).PolyQty = (*pTess).PolyQty;
  status = fscanf (file, "***tess%s", tmp);
  if (status != 1)
  {
    rewind (file);
    status = fscanf (file, "***tess%s", tmp);
  }

  if (status != 1)
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
  else if (strncmp (tmp, "1.9", 3) != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    printf ("Unknown / unsupported version %s\n", tmp);
    abort ();
  }

  if (strcmp (tmp, "1.9.2") == 0)
  {
    if (fscanf (file, "%s", tmp) != 1 || strcmp (tmp, "**general") != 0)
    {
      ut_print_message (2, 0,
			"Input file is not a valid tessellation file.\n");
      abort ();
    }
    (*pTess).Type = ut_alloc_1d_char (100);
    if (fscanf (file, "%*s%s%*d", (*pTess).Type) != 1)
      abort ();

    if (!strcmp ((*pTess).Type, "0"))
      strcpy ((*pTess).Type, "standard");
    else
      ut_error_reportbug ();
  }

  ut_free_1d_char (tmp);

  return;
}

/* Tessellation exportation: foot */
void
neut_tess_fscanf_foot_1p9 (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

/* Tessellation exportation: vertex */
void
neut_tess_fscanf_ver_1p9 (struct TESS *pTess, FILE * file)
{
  int i, ver;

  if (ut_file_string_scanncomp (file, "**vertex") != 0
      || fscanf (file, "%d", &((*pTess).VerQty)) != 1)
    abort ();

  // (*pTess).VerDom   = ut_alloc_2d_int ((*pTess).VerQty + 1, 4);
  (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    if (fscanf (file, "%d", &ver) != 1)
      abort ();
    ut_file_skip (file, 4);

    if (fscanf (file, "%d", &((*pTess).VerEdgeQty[ver])) != 1)
      abort ();

    (*pTess).VerEdgeNb[ver] = ut_alloc_1d_int ((*pTess).VerEdgeQty[ver]);
    ut_array_1d_int_fscanf (file, (*pTess).VerEdgeNb[ver],
			    (*pTess).VerEdgeQty[ver]);

    ut_array_1d_fscanf (file, (*pTess).VerCoo[ver], 3);

    if (fscanf (file, "%d", &((*pTess).VerState[ver])) != 1)
      abort ();
  }

  return;
}

/* Tessellation exportation: edge */
void
neut_tess_fscanf_edge_1p9 (struct TESS *pTess, FILE * file)
{
  int i, edge, status;

  if (ut_file_string_scanncomp (file, "**edge") != 0
      || fscanf (file, "%d", &((*pTess).EdgeQty)) != 1)
    abort ();

  (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    status = fscanf (file, "%d", &edge);
    ut_array_1d_int_fscanf (file, (*pTess).EdgeVerNb[edge], 2);
    status += fscanf (file, "%d", &((*pTess).EdgeFaceQty[edge]));

    (*pTess).EdgeFaceNb[i] = ut_alloc_1d_int ((*pTess).EdgeFaceQty[edge]);
    ut_array_1d_int_fscanf (file, (*pTess).EdgeFaceNb[edge],
			    (*pTess).EdgeFaceQty[edge]);

    status += fscanf (file, "%d", &((*pTess).EdgeState[edge]));

    if (status != 3)
      abort ();
  }

  return;
}

/* Tessellation exportation: face */
void
neut_tess_fscanf_face_1p9 (struct TESS *pTess, FILE * file)
{
  int i, face;

  if (ut_file_string_scanncomp (file, "**face") != 0
      || fscanf (file, "%d", &((*pTess).FaceQty)) != 1)
    abort ();

  // (*pTess).FaceDom   = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);
  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    if (fscanf (file, "%d", &face) != 1)
      abort ();
    ut_file_skip (file, 1);

    ut_array_1d_int_fscanf (file, (*pTess).FacePoly[face], 2);
    ut_array_1d_fscanf (file, (*pTess).FaceEq[face], 4);

    if (fscanf (file, "%d", &((*pTess).FaceVerQty[face])) != 1)
      abort ();

    (*pTess).FaceVerNb[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    (*pTess).FaceEdgeNb[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    (*pTess).FaceEdgeOri[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).FaceVerNb[face] + 1,
			    (*pTess).FaceVerQty[face]);

    ut_array_1d_int_fscanf (file, (*pTess).FaceEdgeNb[face] + 1,
			    (*pTess).FaceVerQty[face]);
    ut_array_1d_int_sgn ((*pTess).FaceEdgeNb[face] + 1,
			 (*pTess).FaceVerQty[face],
			 (*pTess).FaceEdgeOri[face] + 1);
    ut_array_1d_int_abs ((*pTess).FaceEdgeNb[face] + 1,
			 (*pTess).FaceVerQty[face]);

    if (fscanf (file, "%d%d", &((*pTess).FaceState[face]),
		&((*pTess).FacePt[face])) != 2)
      abort ();

    ut_array_1d_fscanf (file, (*pTess).FacePtCoo[face], 3);

    ut_file_skip (file, 1);
  }

  return;
}

/* Tessellation exportation: poly */
void
neut_tess_fscanf_poly_1p9 (struct TESS *pTess, FILE * file)
{
  int i, poly;

  if (ut_file_string_scanncomp (file, "**polyhedron") != 0
      || fscanf (file, "%d", &((*pTess).PolyQty)) != 1)
    abort ();

  (*pTess).SeedCoo = ut_alloc_2d ((*pTess).PolyQty + 1, 3);
  (*pTess).CellTrue = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).CellBody = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
  {
    if (fscanf (file, "%d", &poly) != 1)
      abort ();

    ut_array_1d_fscanf (file, (*pTess).SeedCoo[poly], 3);

    if (fscanf (file, "%d%d%d", &((*pTess).CellTrue[poly]),
		&((*pTess).CellBody[poly]),
		&((*pTess).PolyFaceQty[poly])) != 3)
      abort ();

    (*pTess).PolyFaceNb[poly] =
      ut_alloc_1d_int ((*pTess).PolyFaceQty[poly] + 1);
    (*pTess).PolyFaceOri[poly] =
      ut_alloc_1d_int ((*pTess).PolyFaceQty[poly] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).PolyFaceNb[poly] + 1,
			    (*pTess).PolyFaceQty[poly]);
    ut_array_1d_int_sgn ((*pTess).PolyFaceNb[poly] + 1,
			 (*pTess).PolyFaceQty[poly],
			 (*pTess).PolyFaceOri[poly] + 1);
    ut_array_1d_int_abs ((*pTess).PolyFaceNb[poly] + 1,
			 (*pTess).PolyFaceQty[poly]);
  }

  return;
}
