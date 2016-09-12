/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fscanf_.h"

void
neut_tess_fscanf_per_gen (struct TESS *pTess, FILE * file)
{
  (*pTess).Periodic = ut_alloc_1d_int (3);
  (*pTess).PeriodicDist = ut_alloc_1d (3);

  if (ut_file_string_scanncomp (file, "**periodicity") != 0
      || ut_file_string_scanncomp (file, "*general") != 0
      || ut_array_1d_int_fscanf (file, (*pTess).Periodic, 3) != 1
      || ut_array_1d_fscanf (file, (*pTess).PeriodicDist, 3) != 1)
    abort ();

  return;
}

void
neut_tess_fscanf_per_seed (struct TESS *pTess, FILE * file)
{
  int i, j, id, master;

  if (ut_file_string_scanncomp (file, "*seed") != 0
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
      (*pTess).SeedCoo[i][j]
	= (*pTess).SeedCoo[master][j]
	+ (*pTess).PerSeedShift[i][j] * (*pTess).PeriodicDist[j];

    (*pTess).SeedWeight = ut_realloc_1d ((*pTess).SeedWeight, i + 1);
    (*pTess).SeedWeight[i] = (*pTess).SeedWeight[master];
  }

  return;
}

void
neut_tess_fscanf_per_face (struct TESS *pTess, FILE * file)
{
  int i, id;

  if ((*pTess).Dim == 2)
    return;

  if (ut_file_string_scanncomp (file, "*face") != 0)
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

  return;
}

void
neut_tess_fscanf_per_edge (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (ut_file_string_scanncomp (file, "*edge") != 0)
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

  return;
}

void
neut_tess_fscanf_per_ver (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (ut_file_string_scanncomp (file, "*vertex") != 0)
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
  if (ut_file_string_scanncomp (file, "**scale") != 0
      || ut_file_string_scanncomp (file, "*general") != 0
      || fscanf (file, "%d", &(*pTess).ScaleQty) != 1)
    abort ();

  return;
}

void
neut_tess_fscanf_scale_cellid (struct TESS *pTess, FILE * file)
{
  int i, id;

  if (ut_file_string_scanncomp (file, "*cellid") != 0)
    abort ();

  (*pTess).ScaleCellId = ut_alloc_2d_int ((*pTess).SeedQty + 1,
					  (*pTess).ScaleQty + 1);
  for (i = 1; i <= (*pTess).CellQty; i++)
  {
    if (fscanf (file, "%d", &id) != 1 || id > (*pTess).SeedQty)
      abort ();
    if (ut_array_1d_int_fscanf (file, (*pTess).ScaleCellId[id] + 1, (*pTess).ScaleQty) != 1)
      abort ();
  }

  return;
}
