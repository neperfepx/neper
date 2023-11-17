/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fscanf_.h"

/* Tessellation exportation: head */
void
neut_tess_fscanf_head (struct TESS *pTess, FILE * file, char **pversion)
{
  if (!ut_file_string_scanandtest (file, "***tess")
      || !ut_file_string_scanandtest (file, "**format"))
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");

  (*pversion) = ut_alloc_1d_char (100);
  if (fscanf (file, "%s", *pversion) != 1)
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");

  if (strcmp (*pversion, "2.0") && strcmp (*pversion, "3.3")
      && strcmp (*pversion, "3.4"))
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");

  if (!ut_file_string_scanandtest (file, "**general"))
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");

  (*pTess).Type = ut_alloc_1d_char (100);
  if (fscanf (file, "%d%s", &((*pTess).Dim), (*pTess).Type) != 2)
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");

  if ((*pTess).Dim < 2)
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");

  (*pTess).Level = 1;

  (*pTess).ScaleQty = 1;

  (*pTess).TessId = 1;

  return;
}

/* Tessellation exportation: foot */
void
neut_tess_fscanf_foot (FILE * file)
{
  if (!ut_file_string_scanandtest (file, "***end"))
    abort ();

  return;
}

/* Tessellation exportation: vertex */
void
neut_tess_fscanf_cell (struct TESS *pTess, FILE * file)
{
  int i, status, id, level;
  char *string = ut_alloc_1d_char (1000);

  if (!ut_file_string_scanandtest (file, "**cell")
      || fscanf (file, "%d", &((*pTess).CellQty)) != 1)
    abort ();

  // ModeId set to 1 by default
  (*pTess).CellModeId = ut_alloc_1d_int ((*pTess).CellQty + 1);
  ut_array_1d_int_set ((*pTess).CellModeId + 1, (*pTess).CellQty, 1);

  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "*id"))
    {
      ut_file_skip (file, 1);
      (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);
      ut_array_1d_int_fscanf (file, (*pTess).CellId + 1, (*pTess).CellQty);
    }

    else if (!strcmp (string, "*lam") || !strcmp (string, "*lamid"))
    {
      ut_file_skip (file, 1);
      (*pTess).CellLamId = ut_alloc_1d_int ((*pTess).CellQty + 1);
      ut_array_1d_int_fscanf (file, (*pTess).CellLamId + 1, (*pTess).CellQty);
    }

    else if (!strcmp (string, "*mode") || !strcmp (string, "*modeid"))
    {
      ut_file_skip (file, 1);
      ut_array_1d_int_fscanf (file, (*pTess).CellModeId + 1,
                              (*pTess).CellQty);
    }

    else if (!strcmp (string, "*group"))
    {
      ut_file_skip (file, 1);
      (*pTess).CellGroup = ut_alloc_1d_int ((*pTess).CellQty + 1);
      ut_array_1d_int_fscanf (file, (*pTess).CellGroup + 1,
                              (*pTess).CellQty);
    }

    else if (!strcmp (string, "*crysym"))
    {
      ut_file_skip (file, 1);
      (*pTess).CellCrySym = ut_alloc_1d_char (1000);
      if (fscanf (file, "%s", (*pTess).CellCrySym) != 1)
        abort ();
    }

    else if (!strcmp (string, "*seed"))
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

    else if (!strcmp (string, "*ori"))
    {
      ut_file_skip (file, 1);
      (*pTess).CellOri = ut_alloc_2d ((*pTess).CellQty + 1, 4);
      if (fscanf (file, "%s", (*pTess).CellOriDes) != 1)
        abort ();
      neut_ori_des_expand ((*pTess).CellOriDes, &(*pTess).CellOriDes);

      neut_ori_fscanf (file, (*pTess).CellOriDes, "ascii", (*pTess).CellOri + 1, NULL, (*pTess).CellQty, NULL);
    }

    else if (!strcmp (string, "*weight"))
    {
      ut_file_skip (file, 1);
      (*pTess).CellWeight = ut_alloc_1d ((*pTess).CellQty + 1);
      ut_array_1d_fscanf (file, (*pTess).CellWeight + 1, (*pTess).CellQty);
    }

    else if (!strcmp (string, "*orispread") || !strcmp (string, "*orispread"))
    {
      ut_file_skip (file, 1);
      char *tmp = ut_alloc_1d_char (1000);
      (*pTess).CellOriDistrib = ut_alloc_1d_pchar ((*pTess).CellQty + 1);
      (*pTess).CellOriDistrib[0] = NULL;

      for (i = 1; i <= (*pTess).CellQty; i++)
      {
        if (fscanf (file, "%s", tmp) != 1)
          abort ();
        ut_string_string (tmp, (*pTess).CellOriDistrib + i);
      }

      ut_free_1d_char (&tmp);
    }

    else
      ut_print_message (2, 2, "Could not read field `%s'.\n", string);
  }

  ut_free_1d_char (&string);

  return;
}

/* Tessellation exportation: vertex */
void
neut_tess_fscanf_ver (struct TESS *pTess, FILE * file)
{
  int i, ver, status;

  if (!ut_file_string_scanandtest (file, "**vertex")
      || fscanf (file, "%d", &((*pTess).VerQty)) != 1)
    abort ();

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
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

  if (!ut_file_string_scanandtest (file, "**edge")
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

  if (!ut_file_string_scanandtest (file, "**face")
      || fscanf (file, "%d", &((*pTess).FaceQty)) != 1)
    abort ();

  if ((*pTess).Dim == 3)
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

  if (!ut_file_string_scanandtest (file, "**polyhedron")
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
neut_tess_fscanf_domain (struct TESS *pTess, char *version, FILE * file)
{
  (*pTess).DomType = ut_alloc_1d_char (1000);

  if (!ut_file_string_scanandtest (file, "**domain")
      || !ut_file_string_scanandtest (file, "*general")
      || fscanf (file, "%s", (*pTess).DomType) != 1)
    abort ();

  neut_tess_fscanf_domain_vers (pTess, file);

  if (!strcmp (version, "2.0"))
    neut_tess_fscanf_domain_edges_v2p0 (pTess, file);
  else if (!strcmp (version, "3.3") || !strcmp (version, "3.4"))
    neut_tess_fscanf_domain_edges_v3p3 (pTess, file);

  if ((*pTess).Dim == 3)
  {
    if (!strcmp (version, "2.0"))
      neut_tess_fscanf_domain_faces_v2p0 (pTess, file);
    else if (!strcmp (version, "3.3") || !strcmp (version, "3.4"))
      neut_tess_fscanf_domain_faces_v3p3 (pTess, file);
  }

  neut_tess_init_alldom_fromdomtess (pTess);

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
