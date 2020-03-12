/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fscanf_.h"

void
neut_tess_fscanf_domain_vers (struct TESS *pTess, FILE * file)
{
  int i, id, tmp;

  if (!ut_file_string_scanandtest (file, "*vertex")
      || fscanf (file, "%d", &((*pTess).DomVerQty)) != 1)
    abort ();

  (*pTess).DomVerCoo = ut_alloc_2d ((*pTess).DomVerQty + 1, 3);
  (*pTess).DomVerLabel = ut_alloc_2d_char ((*pTess).DomVerQty + 1, 100);
  (*pTess).DomTessVerNb = ut_alloc_1d_int ((*pTess).DomVerQty + 1);

  // Reading vertices

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

  return;
}

void
neut_tess_fscanf_domain_edges_v2p0 (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*edge")
      || fscanf (file, "%d", &((*pTess).DomEdgeQty)) != 1)
    abort ();

  (*pTess).DomEdgeLabel = ut_alloc_2d_char ((*pTess).DomEdgeQty + 1, 100);
  (*pTess).DomTessEdgeQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
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
    (*pTess).DomEdgeVerQty[i] =
      2 - ut_array_1d_int_valnb ((*pTess).DomEdgeVerNb[i], 2, 0);
  }

  neut_tess_init_domain_domveredge (pTess);

  return;
}

void
neut_tess_fscanf_domain_edges_v3p3 (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*edge")
      || fscanf (file, "%d", &((*pTess).DomEdgeQty)) != 1)
    abort ();

  (*pTess).DomEdgeLabel = ut_alloc_2d_char ((*pTess).DomEdgeQty + 1, 100);
  (*pTess).DomTessEdgeQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id != i)
      abort ();

    if (fscanf (file, "%d", &((*pTess).DomEdgeVerQty[i])) != 1)
      abort ();

    ut_array_1d_int_fscanf (file, (*pTess).DomEdgeVerNb[i],
                            (*pTess).DomEdgeVerQty[i]);

    if (fscanf (file, "%s", (*pTess).DomEdgeLabel[i]) != 1)
      abort ();

    if (fscanf (file, "%d", &((*pTess).DomTessEdgeQty[i])) != 1)
      abort ();

    (*pTess).DomTessEdgeNb[i] =
      ut_alloc_1d_int ((*pTess).DomTessEdgeQty[i] + 1);
    ut_array_1d_int_fscanf (file, (*pTess).DomTessEdgeNb[i] + 1,
                            (*pTess).DomTessEdgeQty[i]);
  }

  neut_tess_init_domain_domveredge (pTess);

  return;
}

void
neut_tess_fscanf_domain_faces_v2p0 (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*face")
      || fscanf (file, "%d", &((*pTess).DomFaceQty)) != 1)
    abort ();

  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceType = ut_alloc_1d_pchar ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParmQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParms = ut_alloc_1d_pdouble ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
  (*pTess).DomTessFaceQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
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

    ut_array_1d_int_fscanf (file, (*pTess).DomFaceVerNb[i] + 1,
                            (*pTess).DomFaceVerQty[i]);

    if (fscanf (file, "%d", &((*pTess).DomFaceEdgeQty[i])) != 1)
      abort ();

    (*pTess).DomFaceEdgeNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceEdgeQty[i] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).DomFaceEdgeNb[i] + 1,
                            (*pTess).DomFaceEdgeQty[i]);

    ut_array_1d_fscanf (file, (*pTess).DomFaceEq[i], 4);

    (*pTess).DomFaceParms[i] = ut_alloc_1d (4);

    (*pTess).DomFaceParmQty[i] = 4;
    ut_array_1d_memcpy ((*pTess).DomFaceEq[i], (*pTess).DomFaceParmQty[i],
                        (*pTess).DomFaceParms[i]);

    ut_string_string ("plane", (*pTess).DomFaceType + i);

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

  return;
}

void
neut_tess_fscanf_domain_faces_v3p3 (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*face")
      || fscanf (file, "%d", &((*pTess).DomFaceQty)) != 1)
    abort ();

  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceType = ut_alloc_1d_pchar ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParmQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParms = ut_alloc_1d_pdouble ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
  (*pTess).DomTessFaceQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
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

    ut_array_1d_int_fscanf (file, (*pTess).DomFaceVerNb[i] + 1,
                            (*pTess).DomFaceVerQty[i]);

    if (fscanf (file, "%d", &((*pTess).DomFaceEdgeQty[i])) != 1)
      abort ();

    (*pTess).DomFaceEdgeNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceEdgeQty[i] + 1);

    ut_array_1d_int_fscanf (file, (*pTess).DomFaceEdgeNb[i] + 1,
                            (*pTess).DomFaceEdgeQty[i]);

    (*pTess).DomFaceType[i] = ut_alloc_1d_char (10);
    if (fscanf (file, "%s", (*pTess).DomFaceType[i]) != 1)
      abort ();

    if (fscanf (file, "%d", &((*pTess).DomFaceParmQty[i])) != 1)
      abort ();

    if (!strcmp ((*pTess).DomFaceType[i], "plane")
        && (*pTess).DomFaceParmQty[i] != 4)
      abort ();

    if (!strcmp ((*pTess).DomFaceType[i], "cylinder")
        && (*pTess).DomFaceParmQty[i] != 7)
      abort ();

    if (!strcmp ((*pTess).DomFaceType[i], "sphere")
        && (*pTess).DomFaceParmQty[i] != 4)
      abort ();

    (*pTess).DomFaceParms[i] = ut_alloc_1d ((*pTess).DomFaceParmQty[i]);

    ut_array_1d_fscanf (file, (*pTess).DomFaceParms[i],
                        (*pTess).DomFaceParmQty[i]);

    if (!strcmp ((*pTess).DomFaceType[i], "plane"))
      ut_array_1d_memcpy ((*pTess).DomFaceParms[i], 4, (*pTess).DomFaceEq[i]);

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

  return;
}

void
neut_tess_fscanf_per_gen (struct TESS *pTess, FILE * file)
{
  (*pTess).Periodic = ut_alloc_1d_int (3);
  (*pTess).PeriodicDist = ut_alloc_1d (3);

  if (!ut_file_string_scanandtest (file, "**periodicity")
      || !ut_file_string_scanandtest (file, "*general")
      || ut_array_1d_int_fscanf (file, (*pTess).Periodic, 3) != 1
      || ut_array_1d_fscanf (file, (*pTess).PeriodicDist, 3) != 1)
    abort ();

  return;
}

void
neut_tess_fscanf_per_seed (struct TESS *pTess, FILE * file)
{
  int i, j, id, master;

  if (!ut_file_string_scanandtest (file, "*seed")
      || fscanf (file, "%d", &((*pTess).PerSeedQty)) != 1)
    abort ();

  if ((*pTess).SeedQty == 0)
    ut_print_message (2, 0, "**cell/*seed is missing.\n");

  (*pTess).SeedQty += (*pTess).PerSeedQty;

  (*pTess).PerSeedMaster = ut_alloc_1d_int ((*pTess).SeedQty + 1);
  (*pTess).PerSeedShift = ut_alloc_2d_int ((*pTess).SeedQty + 1, 3);
  for (i = 1; i <= (*pTess).PerSeedQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id > (*pTess).SeedQty)
      abort ();
    if (fscanf (file, "%d", (*pTess).PerSeedMaster + id) != 1
        || ut_array_1d_int_fscanf (file, (*pTess).PerSeedShift[id], 3) != 1)
      abort ();
  }

  for (i = (*pTess).CellQty + 1; i <= (*pTess).SeedQty; i++)
  {
    master = (*pTess).PerSeedMaster[i];
    (*pTess).SeedCoo = ut_realloc_2d_addline ((*pTess).SeedCoo, i + 1, 3);
    for (j = 0; j < 3; j++)
      (*pTess).SeedCoo[i][j] =
        (*pTess).SeedCoo[master][j] +
        (*pTess).PerSeedShift[i][j] * (*pTess).PeriodicDist[j];

    (*pTess).SeedWeight = ut_realloc_1d ((*pTess).SeedWeight, i + 1);
    (*pTess).SeedWeight[i] = (*pTess).SeedWeight[master];
  }

  neut_tess_init_seedslave (pTess);

  return;
}

void
neut_tess_fscanf_per_face (struct TESS *pTess, FILE * file)
{
  int i, id;

  if ((*pTess).Dim == 2)
    return;

  if (!ut_file_string_scanandtest (file, "*face"))
    abort ();

  if (fscanf (file, "%d", &((*pTess).PerFaceQty)) != 1)
    abort ();
  (*pTess).PerFaceNb = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).PerFaceMaster = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).PerFaceShift = ut_alloc_2d_int ((*pTess).FaceQty + 1, 3);
  (*pTess).PerFaceOri = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  for (i = 1; i <= (*pTess).PerFaceQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id > (*pTess).FaceQty)
      abort ();
    (*pTess).PerFaceNb[i] = id;
    if (fscanf (file, "%d", (*pTess).PerFaceMaster + id) != 1
        || ut_array_1d_int_fscanf (file, (*pTess).PerFaceShift[id], 3) != 1)
      abort ();
    if (fscanf (file, "%d", &((*pTess).PerFaceOri[id])) != 1)
      abort ();
  }

  neut_tess_init_faceslave (pTess);
  neut_tess_init_facepoly_per (pTess);

  return;
}

void
neut_tess_fscanf_per_edge (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*edge"))
    abort ();

  if (fscanf (file, "%d", &((*pTess).PerEdgeQty)) != 1)
    abort ();
  (*pTess).PerEdgeNb = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeMaster = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeShift = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 3);
  (*pTess).PerEdgeOri = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  for (i = 1; i <= (*pTess).PerEdgeQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id > (*pTess).EdgeQty)
      abort ();
    (*pTess).PerEdgeNb[i] = id;
    if (fscanf (file, "%d", (*pTess).PerEdgeMaster + id) != 1
        || ut_array_1d_int_fscanf (file, (*pTess).PerEdgeShift[id], 3) != 1)
      abort ();
    if (fscanf (file, "%d", &((*pTess).PerEdgeOri[id])) != 1)
      abort ();
  }
  neut_tess_init_edgeslave (pTess);

  if ((*pTess).Dim == 2)
    neut_tess_init_edgefacenb_per (pTess);

  return;
}

void
neut_tess_fscanf_per_ver (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*vertex"))
    abort ();

  if (fscanf (file, "%d", &((*pTess).PerVerQty)) != 1)
    abort ();
  (*pTess).PerVerMaster = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).PerVerShift = ut_alloc_2d_int ((*pTess).VerQty + 1, 3);
  (*pTess).PerVerNb = ut_alloc_1d_int ((*pTess).VerQty + 1);
  for (i = 1; i <= (*pTess).PerVerQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id > (*pTess).VerQty)
      abort ();
    (*pTess).PerVerNb[i] = id;
    if (fscanf (file, "%d", (*pTess).PerVerMaster + id) != 1
        || ut_array_1d_int_fscanf (file, (*pTess).PerVerShift[id], 3) != 1)
      abort ();
  }
  neut_tess_init_verslave (pTess);

  return;
}

void
neut_tess_fscanf_scale_gen (struct TESS *pTess, FILE * file)
{
  if (!ut_file_string_scanandtest (file, "**scale")
      || !ut_file_string_scanandtest (file, "*general")
      || fscanf (file, "%d", &(*pTess).ScaleQty) != 1)
    abort ();

  return;
}

void
neut_tess_fscanf_scale_cellid (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (!ut_file_string_scanandtest (file, "*cellid"))
    abort ();

  (*pTess).ScaleCellId =
    ut_alloc_2d_int ((*pTess).SeedQty + 1, (*pTess).ScaleQty + 1);
  for (i = 1; i <= (*pTess).CellQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id > (*pTess).SeedQty)
      abort ();
    if (ut_array_1d_int_fscanf
        (file, (*pTess).ScaleCellId[id] + 1, (*pTess).ScaleQty) != 1)
      abort ();
  }

  return;
}
