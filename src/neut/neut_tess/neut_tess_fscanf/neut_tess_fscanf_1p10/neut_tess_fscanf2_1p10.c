/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_1p10_.h"

void
neut_tess_fscanf_version_1p10 (FILE * file, char *version)
{
  int status = 0;
  char *tmp = ut_alloc_1d_char (1000);

  if (fscanf (file, "%s", tmp) != 1)
    status = -1;
  else
  {
    if (!strcmp (tmp, "***tess1.9.2") || !strcmp (tmp, "***tess1.9"))
    {
      strcpy (version, "1.9.2");
    }
    else if (!strcmp (tmp, "***tess"))
    {
      if (ut_file_string_scanncomp (file, "**format") != 0)
      {
	ut_print_message (2, 0,
			  "Input file is not a valid tessellation file.\n");
	abort ();
      }

      if (fscanf (file, "%s", version) != 1)
	status = -1;
    }
    else
      status = -1;
  }

  if (status != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  fseek (file, 0, 0);

  ut_free_1d_char (tmp);

  return;
}

/* Tessellation exportation: head */
void
neut_tess_fscanf_head_1p10 (struct TESS *pTess, FILE * file)
{
  if (ut_file_string_scanncomp (file, "***tesl") != 0
      || ut_file_string_scanncomp (file, "**format") != 0
      || ut_file_string_scanncomp (file, "1.10") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  if (ut_file_string_scanncomp (file, "**general") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }
  (*pTess).Type = ut_alloc_1d_char (100);

  if (fscanf (file, "%d%*d%s%*s", &((*pTess).CellQty), (*pTess).Type) != 2)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  int i;
  char *tmp = ut_alloc_1d_char (100);
  ut_file_nextstring (file, tmp);
  if (!strcmp (tmp, "*polyid"))
  {
    ut_file_skip (file, 1);
    (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);

    // TODO: change to PolyQty for non-standard tessellations
    for (i = 1; i <= (*pTess).CellQty; i++)
      if (fscanf (file, "%d", &((*pTess).CellId[i])) != 1)
	abort ();
  }

  ut_free_1d_char (tmp);

  return;
}

/* Tessellation exportation: foot */
void
neut_tess_fscanf_foot_1p10 (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

/* Tessellation exportation: vertex */
void
neut_tess_fscanf_ver_1p10 (struct TESS *pTess, FILE * file)
{
  int i, ver, status;

  if (ut_file_string_scanncomp (file, "**vertex") != 0
      || fscanf (file, "%d", &((*pTess).VerQty)) != 1)
    abort ();


  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
  (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    status = fscanf (file, " %d", &ver);
    ut_array_1d_int_fscanf (file, (*pTess).VerDom[ver], 2);

    status += fscanf (file, " %d", &((*pTess).VerEdgeQty[ver]));

    (*pTess).VerEdgeNb[ver] = ut_alloc_1d_int ((*pTess).VerEdgeQty[ver]);
    ut_array_1d_int_fscanf (file, (*pTess).VerEdgeNb[ver],
			    (*pTess).VerEdgeQty[ver]);

    ut_array_1d_fscanf (file, (*pTess).VerCoo[ver], 3);

    status += fscanf (file, "%d", &((*pTess).VerState[ver]));

    if (status != 3)
      abort ();
  }

  return;
}

/* Tessellation exportation: edge */
void
neut_tess_fscanf_edge_1p10 (struct TESS *pTess, FILE * file)
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
  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    status = fscanf (file, "%d", &edge);
    ut_array_1d_int_fscanf (file, (*pTess).EdgeDom[edge], 2);

    ut_array_1d_int_fscanf (file, (*pTess).EdgeVerNb[edge], 2);

    status += fscanf (file, "%d", &((*pTess).EdgeFaceQty[edge]));

    (*pTess).EdgeFaceNb[edge] = ut_alloc_1d_int ((*pTess).EdgeFaceQty[edge]);
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
neut_tess_fscanf_face_1p10 (struct TESS *pTess, FILE * file)
{
  int i, j, face, tmp, status;

  if (ut_file_string_scanncomp (file, "**face") != 0
      || fscanf (file, "%d", &((*pTess).FaceQty)) != 1)
    abort ();

  (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
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
    status = fscanf (file, "%d", &face);

    ut_array_1d_int_fscanf (file, (*pTess).FaceDom[face], 2);

    ut_array_1d_int_fscanf (file, (*pTess).FacePoly[face], 2);

    ut_array_1d_fscanf (file, (*pTess).FaceEq[face], 4);

    status += fscanf (file, "%d", &((*pTess).FaceVerQty[face]));

    (*pTess).FaceVerNb[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    (*pTess).FaceEdgeNb[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    (*pTess).FaceEdgeOri[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).FaceVerNb[face] + 1,
			    (*pTess).FaceVerQty[face]);

    for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    {
      if (fscanf (file, "%d", &tmp) != 1)
	abort ();
      (*pTess).FaceEdgeOri[face][j] = ut_num_sgn_int (tmp);
      (*pTess).FaceEdgeNb[face][j] = tmp * (*pTess).FaceEdgeOri[face][j];
    }

    status += fscanf (file, "%d", &((*pTess).FaceState[face]));
    status += fscanf (file, "%d", &((*pTess).FacePt[face]));
    ut_array_1d_fscanf (file, (*pTess).FacePtCoo[face], 3);

    if (status != 4)
      abort ();
  }

  return;
}

/* Tessellation exportation: poly */
void
neut_tess_fscanf_poly_1p10 (struct TESS *pTess, FILE * file)
{
  int i, poly, status;

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
    status = fscanf (file, "%d", &poly);

    ut_array_1d_fscanf (file, (*pTess).SeedCoo[poly], 3);

    status += fscanf (file, "%d", &((*pTess).CellTrue[poly]));
    status += fscanf (file, "%d", &((*pTess).CellBody[poly]));
    status += fscanf (file, "%d", &((*pTess).PolyFaceQty[poly]));

    if (status != 4)
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

/* Tessellation import: domain */
void
neut_tess_fscanf_domain_1p10 (struct TESS *pTess, FILE * file)
{
  int i, id;

  (*pTess).DomType = ut_alloc_1d_char (10);

  if (ut_file_string_scanncomp (file, "**domain") != 0
      || ut_file_string_scanncomp (file, "*general") != 0
      || fscanf (file, "%s", (*pTess).DomType) != 1
      || ut_file_string_scanncomp (file, "*vertex") != 0
      || fscanf (file, "%d", &((*pTess).DomVerQty)) != 1)
    abort ();

  (*pTess).DomTessVerNb = ut_alloc_1d_int ((*pTess).DomVerQty + 1);
  (*pTess).DomVerEdgeQty = ut_alloc_1d_int ((*pTess).DomVerQty + 1);
  (*pTess).DomVerEdgeNb = ut_alloc_1d_pint ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id != i)
      abort ();

    if (fscanf (file, "%d%d", &((*pTess).DomTessVerNb[i]),
		&((*pTess).DomVerEdgeQty[i])) != 2)
      abort ();

    (*pTess).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pTess).DomVerEdgeQty[i]);
    ut_array_1d_int_fscanf (file, (*pTess).DomVerEdgeNb[i],
			    (*pTess).DomVerEdgeQty[i]);
  }

  // Reading edges

  if (ut_file_string_scanncomp (file, "*edge") != 0
      || fscanf (file, "%d", &((*pTess).DomEdgeQty)) != 1)
    abort ();

  (*pTess).DomTessEdgeQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);
  (*pTess).DomEdgeFaceNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id != i)
      abort ();

    if (fscanf (file, "%d", &((*pTess).DomTessEdgeQty[i])) != 1)
      abort ();

    (*pTess).DomTessEdgeNb[i] =
      ut_alloc_1d_int ((*pTess).DomTessEdgeQty[i] + 1);
    ut_array_1d_int_fscanf (file, (*pTess).DomTessEdgeNb[i] + 1,
			    (*pTess).DomTessEdgeQty[i]);
    ut_array_1d_int_fscanf (file, (*pTess).DomEdgeVerNb[i], 2);
    ut_array_1d_int_fscanf (file, (*pTess).DomEdgeFaceNb[i], 2);
  }

  // Reading faces
  if (ut_file_string_scanncomp (file, "*face") != 0
      || fscanf (file, "%d", &((*pTess).DomFaceQty)) != 1)
    abort ();

  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  (*pTess).DomTessFaceQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id != i)
    {
      printf ("id = %d != %d\n", id, i);
      abort ();
    }

    if (fscanf (file, "%d", &((*pTess).DomTessFaceQty[i])) != 1)
      abort ();

    (*pTess).DomTessFaceNb[i] =
      ut_alloc_1d_int ((*pTess).DomTessFaceQty[i] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).DomTessFaceNb[i] + 1,
			    (*pTess).DomTessFaceQty[i]);
    ut_array_1d_fscanf (file, (*pTess).DomFaceEq[i] + 1, 3);

    if (fscanf (file, "%lf%d", &((*pTess).DomFaceEq[i][0]),
		&((*pTess).DomFaceVerQty[i])) != 2)
      abort ();

    (*pTess).DomFaceVerNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);
    (*pTess).DomFaceEdgeNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).DomFaceVerNb[i] + 1,
			    (*pTess).DomFaceVerQty[i]);
    ut_array_1d_int_fscanf (file, (*pTess).DomFaceEdgeNb[i] + 1,
			    (*pTess).DomFaceVerQty[i]);
  }

  return;
}
