/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fscanf_.h"

void
neut_tess_fscanf_version (FILE * file, char *version)
{
  int status = 0;
  char *tmp = ut_alloc_1d_char (1000);
  fpos_t pos;

  fgetpos (file, &pos);

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

  fsetpos (file, &pos);

  ut_free_1d_char (tmp);

  return;
}

/* Tessellation exportation: head */
void
neut_tess_fscanf_head (struct TESS *pTess, FILE * file)
{
  if (ut_file_string_scanncomp (file, "***tess") != 0
      || ut_file_string_scanncomp (file, "**format") != 0
      || ut_file_string_scanncomp (file, "2.0") != 0)
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

  if (fscanf (file, "%d%s", &((*pTess).Dim), (*pTess).Type) != 2)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  (*pTess).Level = 1;

  return;
}

/* Tessellation exportation: foot */
void
neut_tess_fscanf_foot (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

/* Tessellation exportation: vertex */
void
neut_tess_fscanf_cell (struct TESS *pTess, FILE * file)
{
  int i, status, id;
  double theta;
  char *string = ut_alloc_1d_char (1000);
  double *des = ut_alloc_1d (9);

  if (ut_file_string_scanncomp (file, "**cell") != 0
      || fscanf (file, "%d", &((*pTess).CellQty)) != 1)
    abort ();

  ut_file_nextstring (file, string);
  if (!strcmp (string, "*id"))
  {
    ut_file_skip (file, 1);
    (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    ut_array_1d_int_fscanf (file, (*pTess).CellId + 1, (*pTess).CellQty);
  }

  ut_file_nextstring (file, string);
  if (!strcmp (string, "*lamid"))
  {
    ut_file_skip (file, 1);
    (*pTess).CellLamId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    ut_array_1d_int_fscanf (file, (*pTess).CellLamId + 1, (*pTess).CellQty);
  }

  ut_file_nextstring (file, string);
  if (!strcmp (string, "*modeid"))
  {
    ut_file_skip (file, 1);
    (*pTess).CellModeId = ut_alloc_1d_int ((*pTess).CellQty + 1);
    ut_array_1d_int_fscanf (file, (*pTess).CellModeId + 1, (*pTess).CellQty);
  }

  ut_file_nextstring (file, string);
  if (!strcmp (string, "*seed"))
  {
    ut_file_skip (file, 1);
    (*pTess).SeedQty = (*pTess).CellQty;
    (*pTess).SeedCoo = ut_alloc_2d ((*pTess).CellQty + 1, 3);
    (*pTess).SeedWeight = ut_alloc_1d ((*pTess).CellQty + 1);

    for (i = 1; i <= (*pTess).CellQty; i++)
    {
      status = fscanf (file, "%d", &id);
      if (id != i)
	abort ();
      ut_array_1d_fscanf (file, (*pTess).SeedCoo[id], 3);
      status = fscanf (file, "%lf", &((*pTess).SeedWeight[id]));

      if (status != 1)
	abort ();
    }
  }

  ut_file_nextstring (file, string);

  if (!strcmp (string, "*ori"))
  {
    ut_file_skip (file, 1);
    (*pTess).CellOri = ut_alloc_2d ((*pTess).CellQty + 1, 4);

    if (fscanf (file, "%s", string) != 1)
      abort ();

    if (!strcmp (string, "q"))
      ut_array_2d_fscanf (file, (*pTess).CellOri + 1, (*pTess).CellQty, 4);
    else if (!strcmp (string, "e"))
      for (i = 1; i <= (*pTess).CellQty; i++)
      {
	ol_e_fscanf (file, des);
	ol_e_q (des, (*pTess).CellOri[i]);
      }
    else if (!strcmp (string, "er"))
      for (i = 1; i <= (*pTess).CellQty; i++)
      {
	ol_e_fscanf (file, des);
	ol_er_e (des, des);
	ol_e_q (des, (*pTess).CellOri[i]);
      }
    else if (!strcmp (string, "ek"))
      for (i = 1; i <= (*pTess).CellQty; i++)
      {
	ol_e_fscanf (file, des);
	ol_ek_e (des, des);
	ol_e_q (des, (*pTess).CellOri[i]);
      }
    else if (!strcmp (string, "rtheta"))
      for (i = 1; i <= (*pTess).CellQty; i++)
      {
	ol_r_fscanf (file, des);
	if (fscanf (file, "%lf", &theta) != 1)
	  abort ();
	ol_rtheta_q (des, theta, (*pTess).CellOri[i]);
      }
    else if (!strcmp (string, "R"))
      for (i = 1; i <= (*pTess).CellQty; i++)
      {
	ol_R_fscanf (file, des);
	ol_R_q (des, (*pTess).CellOri[i]);
      }
    else
      ut_print_message (2, 2, "Could not process descriptor `%s'.\n", string);
  }

  ut_free_1d_char (string);
  ut_free_1d (des);

  return;
}

/* Tessellation exportation: vertex */
void
neut_tess_fscanf_ver (struct TESS *pTess, FILE * file)
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
    status = fscanf (file, "%d", &ver);

    if (ver != i)
      abort ();

    ut_array_1d_fscanf (file, (*pTess).VerCoo[ver], 3);

    status = fscanf (file, "%d", &((*pTess).VerState[ver]));

    if (status != 1)
      abort ();
  }

  return;
}

/* Tessellation exportation: edge */
void
neut_tess_fscanf_edge (struct TESS *pTess, FILE * file)
{
  int i, edge, status;

  if (ut_file_string_scanncomp (file, "**edge") != 0
      || fscanf (file, "%d", &((*pTess).EdgeQty)) != 1)
    abort ();

  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    status = fscanf (file, "%d", &edge);

    ut_array_1d_int_fscanf (file, (*pTess).EdgeVerNb[edge], 2);

    status = fscanf (file, "%d", &((*pTess).EdgeState[edge]));

    if (status != 1)
      abort ();
  }

  return;
}

/* Tessellation exportation: face */
void
neut_tess_fscanf_face (struct TESS *pTess, FILE * file)
{
  int i, j, face, tmp, status;

  if (ut_file_string_scanncomp (file, "**face") != 0
      || fscanf (file, "%d", &((*pTess).FaceQty)) != 1)
    abort ();

  (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
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

    // ut_array_1d_int_fscanf (file, (*pTess).FacePoly[face], 2);

    status += fscanf (file, "%d", &((*pTess).FaceVerQty[face]));

    (*pTess).FaceVerNb[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    (*pTess).FaceEdgeNb[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);
    (*pTess).FaceEdgeOri[face] =
      ut_alloc_1d_int ((*pTess).FaceVerQty[face] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).FaceVerNb[face] + 1,
			    (*pTess).FaceVerQty[face]);

    if (fscanf (file, "%d", &tmp) != 1)
      abort ();

    if (tmp != (*pTess).FaceVerQty[face])
      abort ();

    for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    {
      if (fscanf (file, "%d", &tmp) != 1)
	abort ();
      (*pTess).FaceEdgeOri[face][j] = ut_num_sgn_int (tmp);
      (*pTess).FaceEdgeNb[face][j] = tmp * (*pTess).FaceEdgeOri[face][j];
    }

    ut_array_1d_fscanf (file, (*pTess).FaceEq[face], 4);

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
neut_tess_fscanf_poly (struct TESS *pTess, FILE * file)
{
  int i, poly, status;

  if (ut_file_string_scanncomp (file, "**polyhedron") != 0
      || fscanf (file, "%d", &((*pTess).PolyQty)) != 1)
    abort ();

  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
  {
    status = fscanf (file, "%d", &poly);

    if (status != 1)
      abort ();

    // Reading PolyFace*
    if (fscanf (file, "%d", &((*pTess).PolyFaceQty[poly])) != 1)
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

  (*pTess).PolyState = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  return;
}

/* Tessellation import: domain */
void
neut_tess_fscanf_domain (struct TESS *pTess, FILE * file)
{
  int i, id, tmp;

  (*pTess).DomType = ut_alloc_1d_char (10);

  if (ut_file_string_scanncomp (file, "**domain") != 0
      || ut_file_string_scanncomp (file, "*general") != 0
      || fscanf (file, "%s", (*pTess).DomType) != 1
      || ut_file_string_scanncomp (file, "*vertex") != 0
      || fscanf (file, "%d", &((*pTess).DomVerQty)) != 1)
    abort ();

  (*pTess).DomVerCoo = ut_alloc_2d ((*pTess).DomVerQty + 1, 3);
  (*pTess).DomVerLabel = ut_alloc_2d_char ((*pTess).DomVerQty + 1, 100);
  (*pTess).DomTessVerNb = ut_alloc_1d_int ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id != i)
      abort ();

    ut_array_1d_fscanf (file, (*pTess).DomVerCoo[i], 3);

    if (fscanf (file, "%s", (*pTess).DomVerLabel[i]) != 1)
      abort ();

    if (fscanf (file, "%d", &tmp) != 1 || tmp != 1)
      abort ();

    if (fscanf (file, "%d", &((*pTess).DomTessVerNb[i])) != 1)
      abort ();
  }

  // Reading edges

  if ((*pTess).Dim > 1)
  {
    if (ut_file_string_scanncomp (file, "*edge") != 0
	|| fscanf (file, "%d", &((*pTess).DomEdgeQty)) != 1)
      abort ();

    (*pTess).DomEdgeLabel = ut_alloc_2d_char ((*pTess).DomEdgeQty + 1, 100);
    (*pTess).DomTessEdgeQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
    (*pTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
    (*pTess).DomEdgeVerNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);

    for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    {
      if (fscanf (file, "%d", &id) != 1 || id != i)
	abort ();

      if (fscanf (file, "%s", (*pTess).DomEdgeLabel[i]) != 1)
	abort ();

      if (fscanf (file, "%d", &((*pTess).DomTessEdgeQty[i])) != 1)
	abort ();

      (*pTess).DomTessEdgeNb[i] =
	ut_alloc_1d_int ((*pTess).DomTessEdgeQty[i] + 1);
      ut_array_1d_int_fscanf (file, (*pTess).DomTessEdgeNb[i] + 1,
			      (*pTess).DomTessEdgeQty[i]);
      ut_array_1d_int_fscanf (file, (*pTess).DomEdgeVerNb[i], 2);
    }

    neut_tess_init_domain_domveredge (pTess);
  }

  // Reading faces
  if ((*pTess).Dim > 2)
  {
    if (ut_file_string_scanncomp (file, "*face") != 0
	|| fscanf (file, "%d", &((*pTess).DomFaceQty)) != 1)
      abort ();

    (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
    (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
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

      if (fscanf (file, "%d", &((*pTess).DomFaceVerQty[i])) != 1)
	abort ();

      (*pTess).DomFaceVerNb[i] =
	ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);
      (*pTess).DomFaceEdgeNb[i] =
	ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);

      ut_array_1d_int_fscanf (file, (*pTess).DomFaceVerNb[i] + 1,
			      (*pTess).DomFaceVerQty[i]);

      if (fscanf (file, "%d", &tmp) != 1 && tmp != (*pTess).DomFaceVerQty[i])
	abort ();

      ut_array_1d_int_fscanf (file, (*pTess).DomFaceEdgeNb[i] + 1,
			      (*pTess).DomFaceVerQty[i]);

      ut_array_1d_fscanf (file, (*pTess).DomFaceEq[i], 4);

      if (fscanf (file, "%s", (*pTess).DomFaceLabel[i]) != 1)
	abort ();

      if (fscanf (file, "%d", &((*pTess).DomTessFaceQty[i])) != 1)
	abort ();

      (*pTess).DomTessFaceNb[i] =
	ut_alloc_1d_int ((*pTess).DomTessFaceQty[i] + 1);

      ut_array_1d_int_fscanf (file, (*pTess).DomTessFaceNb[i] + 1,
			      (*pTess).DomTessFaceQty[i]);
    }

    neut_tess_init_domain_domedgeface (pTess);
  }

  if ((*pTess).Dim == 3)
  {
    neut_tess_init_facedom (pTess);
    neut_tess_init_edgedom_fromfacedom (pTess);
    neut_tess_init_verdom_fromedgedom (pTess);
  }
  else if ((*pTess).Dim == 2)
  {
    neut_tess_init_edgedom (pTess);
    neut_tess_init_verdom_fromedgedom (pTess);
  }

  return;
}

void
neut_tess_fscanf_per (struct TESS *pTess, FILE * file)
{
  neut_tess_fscanf_per_gen (pTess, file);
  neut_tess_fscanf_per_seed (pTess, file);
  neut_tess_fscanf_per_ver (pTess, file);
  neut_tess_fscanf_per_edge (pTess, file);
  neut_tess_fscanf_per_face (pTess, file);

  return;
}

void
neut_tess_fscanf_scale (struct TESS *pTess, FILE * file)
{
  neut_tess_fscanf_scale_gen (pTess, file);
  neut_tess_fscanf_scale_cellid (pTess, file);

  return;
}
