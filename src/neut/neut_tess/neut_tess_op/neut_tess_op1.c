/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_op_.h"

void
neut_tess_set_zero (struct TESS *pTess)
{
  (*pTess).Dim = 0;
  (*pTess).PseudoDim = -1;
  (*pTess).PseudoSize = 0;

  (*pTess).Level = 0;
  (*pTess).TessId = 0;

  (*pTess).SeedQty = 0;
  (*pTess).PerSeedQty = 0;
  (*pTess).PerSeedMaster = NULL;
  (*pTess).PerSeedShift = NULL;
  (*pTess).PerSeedSlave = NULL;

  (*pTess).CellQty = 0;
  (*pTess).Type = NULL;
  (*pTess).Periodic = ut_alloc_1d_int (3);
  (*pTess).PeriodicDist = ut_alloc_1d (3);
  (*pTess).CellCrySym = NULL;

  (*pTess).VerQty = 0;
  (*pTess).FaceQty = 0;
  (*pTess).EdgeQty = 0;
  (*pTess).PolyQty = 0;
  (*pTess).VerCoo = NULL;
  (*pTess).VerEdgeQty = NULL;
  (*pTess).VerEdgeNb = NULL;
  (*pTess).VerDom = NULL;
  (*pTess).VerState = NULL;
  (*pTess).EdgeVerNb = NULL;
  (*pTess).EdgeFaceQty = NULL;
  (*pTess).EdgeFaceNb = NULL;
  (*pTess).EdgeLength = NULL;
  (*pTess).EdgeState = NULL;
  (*pTess).EdgeDel = NULL;
  (*pTess).EdgeDom = NULL;
  (*pTess).FacePoly = NULL;
  (*pTess).FaceVerQty = NULL;
  (*pTess).FaceVerNb = NULL;
  (*pTess).FaceEdgeNb = NULL;
  (*pTess).FaceEdgeOri = NULL;
  (*pTess).FaceEq = NULL;
  (*pTess).FaceState = NULL;
  (*pTess).FacePt = NULL;
  (*pTess).FacePtCoo = NULL;
  (*pTess).FaceDom = NULL;
  (*pTess).CellId = NULL;
  (*pTess).CellOri = NULL;
  (*pTess).CellCrySym = NULL;
  (*pTess).PolyFaceQty = NULL;
  (*pTess).PolyFaceNb = NULL;
  (*pTess).PolyFaceOri = NULL;
  (*pTess).PolyState = NULL;

  (*pTess).CellTrue = NULL;
  (*pTess).CellBody = NULL;
  (*pTess).CellLamId = NULL;
  (*pTess).CellModeId = NULL;
  (*pTess).SeedCoo = NULL;
  (*pTess).SeedWeight = NULL;

  (*pTess).ScaleQty = 0;
  (*pTess).ScaleCellId = NULL;

  (*pTess).PerFaceQty = 0;
  (*pTess).PerFaceNb = NULL;
  (*pTess).PerFaceMaster = NULL;
  (*pTess).PerFaceShift = NULL;
  (*pTess).PerFaceOri = NULL;
  (*pTess).PerFaceSlaveNb = NULL;

  (*pTess).PerEdgeQty = 0;
  (*pTess).PerEdgeNb = NULL;
  (*pTess).PerEdgeMaster = NULL;
  (*pTess).PerEdgeShift = NULL;
  (*pTess).PerEdgeOri = NULL;
  (*pTess).PerEdgeSlaveQty = NULL;
  (*pTess).PerEdgeSlaveNb = NULL;

  (*pTess).PerVerQty = 0;
  (*pTess).PerVerNb = NULL;
  (*pTess).PerVerMaster = NULL;
  (*pTess).PerVerShift = NULL;
  (*pTess).PerVerSlaveQty = NULL;
  (*pTess).PerVerSlaveNb = NULL;

  (*pTess).DomType = NULL;

  neut_tess_set_zero_domain (pTess);

  return;
}

void
neut_tess_set_zero_domain (struct TESS *pTess)
{
  (*pTess).DomVerQty = 0;
  (*pTess).DomVerCoo = NULL;
  (*pTess).DomVerLabel = NULL;
  (*pTess).DomVerEdgeQty = NULL;
  (*pTess).DomVerEdgeNb = NULL;
  (*pTess).DomTessVerNb = NULL;

  (*pTess).DomEdgeQty = 0;
  (*pTess).DomEdgeLabel = NULL;
  (*pTess).DomTessEdgeQty = NULL;
  (*pTess).DomEdgeVerQty = NULL;
  (*pTess).DomEdgeVerNb = NULL;
  (*pTess).DomEdgeFaceNb = NULL;
  (*pTess).DomTessEdgeNb = NULL;

  (*pTess).DomFaceQty = 0;
  (*pTess).DomFaceLabel = NULL;
  (*pTess).DomFaceEq = NULL;
  (*pTess).DomFaceType = NULL;
  (*pTess).DomFaceParmQty = NULL;
  (*pTess).DomFaceParms = NULL;
  (*pTess).DomFaceVerQty = NULL;
  (*pTess).DomFaceVerNb = NULL;
  (*pTess).DomFaceEdgeQty = NULL;
  (*pTess).DomFaceEdgeNb = NULL;
  (*pTess).DomTessFaceQty = NULL;
  (*pTess).DomTessFaceNb = NULL;

  return;
}

void
neut_tess_alloc (struct TESS *pTess, int VerQty,
		 int EdgeQty, int FaceQty, int PolyQty)
{
  if (VerQty > 0)
  {
    (*pTess).VerQty = VerQty;
    (*pTess).VerCoo = ut_alloc_1d_pdouble ((*pTess).VerQty + 1);
    (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
    (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
    (*pTess).VerDom = ut_alloc_1d_pint ((*pTess).VerQty + 1);
    (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);
  }

  if (EdgeQty > 0)
  {
    (*pTess).EdgeQty = EdgeQty;
    (*pTess).EdgeVerNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
    (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
    (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
    (*pTess).EdgeLength = ut_alloc_1d ((*pTess).EdgeQty + 1);
    (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
    (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
    (*pTess).EdgeDom = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
  }

  if (FaceQty > 0)
  {
    (*pTess).FaceQty = FaceQty;
    (*pTess).FacePoly = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
    (*pTess).FaceEq = ut_alloc_1d_pdouble ((*pTess).FaceQty + 1);
    (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
    (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
    (*pTess).FaceEdgeNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
    (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
    (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);
    (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
    (*pTess).FacePtCoo = ut_alloc_1d_pdouble ((*pTess).FaceQty + 1);
    (*pTess).FaceDom = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  }

  if (PolyQty > 0)
  {
    (*pTess).PolyQty = PolyQty;
    (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
    (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
    (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
    (*pTess).PolyState = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  }

  return;
}

void
neut_tess_free (struct TESS *pTess)
{
  ut_free_2d ((*pTess).VerCoo, (*pTess).VerQty + 1);
  ut_free_1d_int ((*pTess).VerEdgeQty);
  ut_free_2d_int ((*pTess).VerEdgeNb, (*pTess).VerQty + 1);
  ut_free_2d_int ((*pTess).VerDom, (*pTess).VerQty + 1);
  ut_free_1d_int ((*pTess).VerState);

  ut_free_2d_int ((*pTess).EdgeVerNb, (*pTess).EdgeQty + 1);
  ut_free_1d_int ((*pTess).EdgeFaceQty);
  ut_free_2d_int ((*pTess).EdgeFaceNb, (*pTess).EdgeQty + 1);
  ut_free_1d ((*pTess).EdgeLength);
  ut_free_1d_int ((*pTess).EdgeState);
  ut_free_1d_int ((*pTess).EdgeDel);
  ut_free_2d_int ((*pTess).EdgeDom, (*pTess).EdgeQty + 1);

  if ((*pTess).Dim == 3)
    ut_free_2d_int ((*pTess).FacePoly, (*pTess).FaceQty + 1);

  ut_free_1d_int ((*pTess).FaceVerQty);
  ut_free_2d_int ((*pTess).FaceVerNb, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceEdgeNb, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceEdgeOri, (*pTess).FaceQty + 1);
  ut_free_2d ((*pTess).FaceEq, (*pTess).FaceQty + 1);
  ut_free_1d_int ((*pTess).FaceState);
  ut_free_1d_int ((*pTess).FacePt);
  ut_free_2d ((*pTess).FacePtCoo, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceDom, (*pTess).FaceQty + 1);

  ut_free_2d ((*pTess).SeedCoo, (*pTess).SeedQty + 1);
  ut_free_1d ((*pTess).SeedWeight);
  ut_free_1d_int ((*pTess).PerSeedMaster);
  ut_free_2d_int ((*pTess).PerSeedShift, (*pTess).SeedQty + 1);
  ut_free_2d_int ((*pTess).PerSeedSlave, (*pTess).SeedQty + 1);
  (*pTess).PerSeedQty = 0;

  ut_free_1d_int ((*pTess).CellId);
  ut_free_2d ((*pTess).CellOri, (*pTess).CellQty + 1);
  ut_free_1d_char ((*pTess).CellCrySym);

  ut_free_1d_int ((*pTess).PolyFaceQty);
  ut_free_2d_int ((*pTess).PolyFaceNb, (*pTess).PolyQty + 1);
  ut_free_2d_int ((*pTess).PolyFaceOri, (*pTess).PolyQty + 1);
  ut_free_1d_int ((*pTess).PolyState);

  ut_free_1d_int ((*pTess).CellTrue);
  ut_free_1d_int ((*pTess).CellBody);
  ut_free_1d_int ((*pTess).CellLamId);
  ut_free_1d_int ((*pTess).CellModeId);

  ut_free_1d_char ((*pTess).Type);
  ut_free_1d_int ((*pTess).Periodic);
  ut_free_1d ((*pTess).PeriodicDist);

  ut_free_2d_int ((*pTess).ScaleCellId, (*pTess).CellQty + 1);

  (*pTess).PerFaceQty = 0;
  ut_free_1d_int ((*pTess).PerFaceNb);
  ut_free_1d_int ((*pTess).PerFaceMaster);
  ut_free_2d_int ((*pTess).PerFaceShift, (*pTess).FaceQty + 1);
  ut_free_1d_int ((*pTess).PerFaceOri);
  ut_free_1d_int ((*pTess).PerFaceSlaveNb);

  (*pTess).PerEdgeQty = 0;
  ut_free_1d_int ((*pTess).PerEdgeNb);
  ut_free_1d_int ((*pTess).PerEdgeMaster);
  ut_free_2d_int ((*pTess).PerEdgeShift, (*pTess).EdgeQty + 1);
  ut_free_1d_int ((*pTess).PerEdgeOri);
  ut_free_1d_int ((*pTess).PerEdgeSlaveQty);
  ut_free_2d_int ((*pTess).PerEdgeSlaveNb, (*pTess).EdgeQty + 1);

  (*pTess).PerVerQty = 0;
  ut_free_1d_int ((*pTess).PerVerNb);
  ut_free_1d_int ((*pTess).PerVerMaster);
  ut_free_2d_int ((*pTess).PerVerShift, (*pTess).VerQty + 1);
  ut_free_1d_int ((*pTess).PerVerSlaveQty);
  ut_free_2d_int ((*pTess).PerVerSlaveNb, (*pTess).VerQty + 1);

  neut_tess_free_domain (pTess);

  neut_tess_set_zero (pTess);

  return;
}

void
neut_tess_free_domain (struct TESS *pTess)
{
  ut_free_1d_char_ (&(*pTess).DomType);

  ut_free_2d ((*pTess).DomVerCoo, (*pTess).DomVerQty + 1);
  ut_free_2d_char ((*pTess).DomVerLabel, (*pTess).DomVerQty + 1);
  ut_free_1d_int ((*pTess).DomVerEdgeQty);
  ut_free_2d_int ((*pTess).DomVerEdgeNb, (*pTess).DomVerQty + 1);
  ut_free_1d_int ((*pTess).DomTessVerNb);

  ut_free_2d_char ((*pTess).DomEdgeLabel, (*pTess).DomEdgeQty + 1);
  ut_free_1d_int ((*pTess).DomEdgeVerQty);
  ut_free_2d_int ((*pTess).DomEdgeVerNb, (*pTess).DomEdgeQty + 1);
  ut_free_2d_int ((*pTess).DomEdgeFaceNb, (*pTess).DomEdgeQty + 1);
  ut_free_1d_int ((*pTess).DomTessEdgeQty);
  ut_free_2d_int ((*pTess).DomTessEdgeNb, (*pTess).DomEdgeQty + 1);

  ut_free_2d_char ((*pTess).DomFaceLabel, (*pTess).DomFaceQty + 1);
  // ut_free_2d_char ((*pTess).DomFaceType, (*pTess).DomFaceQty + 1);
  ut_free_2d ((*pTess).DomFaceEq, (*pTess).DomFaceQty + 1);
  // ut_free_1d_int ((*pTess).DomFaceParmQty);
  // ut_free_2d ((*pTess).DomFaceParms, (*pTess).DomFaceQty + 1);
  ut_free_1d_int ((*pTess).DomFaceVerQty);
  ut_free_2d_int ((*pTess).DomFaceVerNb, (*pTess).DomFaceQty + 1);
  ut_free_1d_int ((*pTess).DomFaceEdgeQty);
  ut_free_2d_int ((*pTess).DomFaceEdgeNb, (*pTess).DomFaceQty + 1);
  ut_free_1d_int ((*pTess).DomTessFaceQty);
  ut_free_2d_int ((*pTess).DomTessFaceNb, (*pTess).DomFaceQty + 1);

  neut_tess_set_zero_domain (pTess);

  return;
}

/* Deformation of the geoellation.
 */
void
neut_tess_scale (struct TESS *pTess, double vx, double vy, double vz)
{
  int i, j = 0, k, numa, numb;
  double norm;
  double *gsize = ut_alloc_1d (3);
  gsize[0] = vx;
  gsize[1] = vy;
  gsize[2] = vz;

  /* TODO alloc should probably not be done */
  (*pTess).EdgeLength =
    ut_realloc_1d ((*pTess).EdgeLength, (*pTess).EdgeQty + 1);

  /* changing vertex coordinates */
  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).VerCoo[i][j] *= gsize[j];

  /* updating edge lengths from the ver positions */
  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    numa = (*pTess).EdgeVerNb[i][0];
    numb = (*pTess).EdgeVerNb[i][1];

    (*pTess).EdgeLength[i] =
      ut_space_dist ((*pTess).VerCoo[numa], (*pTess).VerCoo[numb]);
  }

  /* updating face equations */
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEq[i][1] /= gsize[0];
    (*pTess).FaceEq[i][2] /= gsize[1];
    (*pTess).FaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pTess).FaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pTess).FaceEq[i][j] /= norm;
  }

  /* changing centre coordinates */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).SeedCoo[i][j] *= gsize[j];

  /* changing centre coordinates */
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if (((*pTess).FaceState[i] > 0 && (*pTess).FacePt[i] == 0) || neut_tess_face_iscurved (*pTess, i))
      for (j = 0; j < 3; j++)
	(*pTess).FacePtCoo[i][j] *= gsize[j];

  // updating domain vertex coos
  for (i = 1; i <= (*pTess).DomVerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).DomVerCoo[i][j] *= gsize[j];

  /* updating domain face equations */
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    (*pTess).DomFaceEq[i][1] /= gsize[0];
    (*pTess).DomFaceEq[i][2] /= gsize[1];
    (*pTess).DomFaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pTess).DomFaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pTess).DomFaceEq[i][j] /= norm;
  }

  /* updating domain face parms */
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    // works only if the cylinder axes and the standard axes are aligned (not 1 to 1)
    if (!strcmp ((*pTess).DomFaceType[i], "ecylinder"))
    {
      for (k = 0; k < 3; k++)
        (*pTess).DomFaceParms[i][k] *= gsize[k];

      (*pTess).DomFaceParms[i][12]
        *= ut_vector_scalprod ((*pTess).DomFaceParms[i] + 6, gsize);
      (*pTess).DomFaceParms[i][13]
        *= ut_vector_scalprod ((*pTess).DomFaceParms[i] + 9, gsize);

      // fixing face equations (= equation of the tangent plane at FacePtCoo
      int face;
      double *eq = ut_alloc_1d (4);
      for (j = 1; j <= (*pTess).DomTessFaceQty[i]; j++)
      {
        face = (*pTess).DomTessFaceNb[i][j];
        ut_array_1d_memcpy (eq, 4, (*pTess).FaceEq[face]);
        neut_primparms_point_tangentplane ("ecylinder",
                                           (*pTess).DomFaceParms[i],
                                           (*pTess).FacePtCoo[face],
                                           (*pTess).FaceEq[face]);
        if (ut_vector_scalprod (eq + 1, (*pTess).FaceEq[face] + 1) < 0)
          ut_array_1d_scale ((*pTess).FaceEq[face], 4, -1);
      }
      ut_free_1d (eq);

      if (ut_num_requal ((*pTess).DomFaceParms[i][12],
                         (*pTess).DomFaceParms[i][13], 1e-6))
      {
        ut_string_string ("cylinder", (*pTess).DomFaceType + i);
        (*pTess).DomFaceParms[i][6] = (*pTess).DomFaceParms[i][12];
        (*pTess).DomFaceParmQty[i] = 7;
        (*pTess).DomFaceParms[i] = ut_realloc_1d ((*pTess).DomFaceParms[i],
                                                  (*pTess).DomFaceParmQty[i]);
      }
    }
  }

  if ((*pTess).PeriodicDist)
    for (i = 0; i < (*pTess).Dim; i++)
      (*pTess).PeriodicDist[i] *= gsize[i];

  ut_free_1d (gsize);

  return;
}

// Shift a tessellation
void
neut_tess_shift (struct TESS *pTess, double sx, double sy, double sz)
{
  int i, j;
  double *shift = ut_alloc_1d (3);
  shift[0] = sx;
  shift[1] = sy;
  shift[2] = sz;

  // changing vertex coordinates
  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).VerCoo[i][j] += shift[j];

  // updating face equations
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).FaceEq[i][0] += shift[j] * (*pTess).FaceEq[i][j + 1];

  // changing centre coordinates
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).SeedCoo[i][j] += shift[j];

  // changing centre coordinates
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceState[i] > 0 && (*pTess).FacePt[i] == 0)
      for (j = 0; j < 3; j++)
	(*pTess).FacePtCoo[i][j] += shift[j];

  // updating domain vertex coos
  for (i = 1; i <= (*pTess).DomVerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).DomVerCoo[i][j] += shift[j];

  // updating domain face equations
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).DomFaceEq[i][0] += shift[j] * (*pTess).DomFaceEq[i][j + 1];

  ut_free_1d (shift);

  return;
}

void
neut_tess_rotate (struct TESS *pTess, double **g)
{
  int i;
  double c[3];
  double **ginv = ol_g_alloc ();

  ol_g_inverse (g, ginv);
  neut_tess_centre (*pTess, c);

  neut_tess_shift (pTess, -c[0], -c[1], -c[2]);

  for (i = 1; i <= (*pTess).VerQty; i++)
    ol_g_vect_vect (ginv, (*pTess).VerCoo[i], (*pTess).VerCoo[i]);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    ol_g_vect_vect (ginv, (*pTess).FaceEq[i] + 1, (*pTess).FaceEq[i] + 1);
    ol_g_vect_vect (ginv, (*pTess).FacePtCoo[i], (*pTess).FacePtCoo[i]);
  }

  for (i = 1; i <= (*pTess).DomVerQty; i++)
    ol_g_vect_vect (ginv, (*pTess).DomVerCoo[i], (*pTess).DomVerCoo[i]);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    ol_g_vect_vect (ginv, (*pTess).DomFaceEq[i] + 1, (*pTess).DomFaceEq[i] + 1);

  neut_tess_shift (pTess, c[0], c[1], c[2]);

  if ((*pTess).CellOri)
  {
    double *q = ol_q_alloc ();
    ol_g_q (g, q);
    for (i = 1; i <= (*pTess).CellQty; i++)
      ol_q_q_q ((*pTess).CellOri[i], q, (*pTess).CellOri[i]);

    ol_q_free (q);
  }

  ol_g_free (ginv);

  return;
}

void
neut_tess_poly_switch (struct TESS *pTess, int p1, int p2)
{
  int i, id;

  if (p1 == p2)
    return;

  // SeedCoo
  ut_array_2d_switchlines ((*pTess).SeedCoo, 3, p1, p2);

  // SeedWeight
  if ((*pTess).SeedWeight)
    ut_array_1d_switch ((*pTess).SeedWeight, p1, p2);

  // CellTrue
  if ((*pTess).CellTrue != NULL)
    ut_array_1d_int_switch ((*pTess).CellTrue, p1, p2);

  // CellBody
  if ((*pTess).CellBody != NULL)
    ut_array_1d_int_switch ((*pTess).CellBody, p1, p2);

  // CellLamId
  if ((*pTess).CellLamId != NULL)
    ut_array_1d_int_switch ((*pTess).CellLamId, p1, p2);

  // CellModeId
  if ((*pTess).CellModeId != NULL)
    ut_array_1d_int_switch ((*pTess).CellModeId, p1, p2);

  // PolyFace* (keep FaceQty at the end)
  ut_array_2d_int_switchlines_length ((*pTess).PolyFaceNb,
				      p1, (*pTess).PolyFaceQty[p1] + 1,
				      p2, (*pTess).PolyFaceQty[p2] + 1);

  ut_array_2d_int_switchlines_length ((*pTess).PolyFaceOri,
				      p1, (*pTess).PolyFaceQty[p1] + 1,
				      p2, (*pTess).PolyFaceQty[p2] + 1);

  ut_array_1d_int_switch ((*pTess).PolyFaceQty, p1, p2);

  // FacePoly | the following is to avoid problems when p1 and p2 share
  // a com face
  for (i = 1; i <= (*pTess).PolyFaceQty[p1]; i++)
  {
    id = (*pTess).PolyFaceNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).FacePoly[id], 2, p2, -1000);
  }
  for (i = 1; i <= (*pTess).PolyFaceQty[p2]; i++)
  {
    id = (*pTess).PolyFaceNb[p2][i];
    ut_array_1d_int_findnreplace ((*pTess).FacePoly[id], 2, p1, p2);
  }
  for (i = 1; i <= (*pTess).PolyFaceQty[p1]; i++)
  {
    id = (*pTess).PolyFaceNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).FacePoly[id], 2, -1000, p1);
  }

  return;
}

void
neut_tess_init_edgelength (struct TESS *pTess)
{
  int i;

  (*pTess).EdgeLength =
    ut_realloc_1d ((*pTess).EdgeLength, (*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    neut_tess_init_edgelength_edge (pTess, i);

  return;
}

void
neut_tess_init_edgelength_edge (struct TESS *pTess, int edge)
{
  int ver1, ver2;

  ver1 = (*pTess).EdgeVerNb[edge][0];
  ver2 = (*pTess).EdgeVerNb[edge][1];

  (*pTess).EdgeLength[edge] = ut_space_dist ((*pTess).VerCoo[ver1],
					     (*pTess).VerCoo[ver2]);

  return;
}

void
neut_tess_init_polytopo_fromfacepoly (struct TESS *pTess)
{
  int i, j, poly;

  (*pTess).PolyQty = 0;
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pTess).PolyQty =
	ut_num_max ((*pTess).PolyQty, (*pTess).FacePoly[i][j]);

  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_2d_int ((*pTess).PolyQty + 1, 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      if ((*pTess).FacePoly[i][j] > 0)
      {
	poly = (*pTess).FacePoly[i][j];

	(*pTess).PolyFaceQty[poly]++;
	(*pTess).PolyFaceNb[poly] =
	  ut_realloc_1d_int ((*pTess).PolyFaceNb[poly],
			     (*pTess).PolyFaceQty[poly] + 1);
	(*pTess).PolyFaceNb[poly][(*pTess).PolyFaceQty[poly]] = i;
      }

  return;
}

void
neut_tess_init_edgetopo_fromver (struct TESS *pTess)
{
  int i, j, edge;

  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < (*pTess).VerEdgeQty[i]; j++)
    {
      edge = (*pTess).VerEdgeNb[i][j];
      if (edge > 0)
      {
	if ((*pTess).EdgeVerNb[edge][0] == 0)
	  (*pTess).EdgeVerNb[edge][0] = i;
	else if ((*pTess).EdgeVerNb[edge][1] == 0)
	  (*pTess).EdgeVerNb[edge][1] = i;
	else
	  ut_error_reportbug ();
      }
    }

  return;
}

void
neut_tess_init_facetopo_fromedge (struct TESS *pTess)
{
  int i, j, face;

  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_2d_int ((*pTess).FaceQty + 1, 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    for (j = 0; j < (*pTess).EdgeFaceQty[i]; j++)
    {
      face = (*pTess).EdgeFaceNb[i][j];
      if (face > 0)
      {
	(*pTess).FaceVerQty[face]++;
	(*pTess).FaceEdgeNb[face] =
	  ut_realloc_1d_int ((*pTess).FaceEdgeNb[face],
			     (*pTess).FaceVerQty[face] + 1);
	(*pTess).FaceEdgeNb[face][(*pTess).FaceVerQty[face]] = i;
      }
    }

  // init FaceState
  (*pTess).FaceState =
    ut_realloc_1d_int ((*pTess).FaceState, (*pTess).FaceQty + 1);

  return;
}

void
neut_tess_init_facetopo_fromver (struct TESS *pTess)
{
  int i, j, k, edge, ver, pos;
  int *perm = NULL;

  // permuting FaceEdgeNb and init FaceEdgeOri & FaceVerNb
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    perm = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    perm[1] = 1;
    edge = (*pTess).FaceEdgeNb[i][1];
    ver = (*pTess).EdgeVerNb[edge][1];
    // looking for the 2nd, 3rd,... edges
    for (j = 2; j <= (*pTess).FaceVerQty[i]; j++)
    {
      // checking every edges (we know it's not the 1st)
      for (k = 2; k <= (*pTess).FaceVerQty[i]; k++)
	if (k != perm[j - 1])
	{
	  edge = (*pTess).FaceEdgeNb[i][k];
	  pos = ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[edge], 2, ver);
	  if (pos != -1)
	  {
	    perm[j] = k;
	    ver = (pos == 0) ? (*pTess).EdgeVerNb[edge][1]
	      : (*pTess).EdgeVerNb[edge][0];
	    break;
	  }
	}

      if (perm[j] == 0)
	ut_error_reportbug ();
    }

    ut_array_1d_permutation_int ((*pTess).FaceEdgeNb[i],
				 (*pTess).FaceVerQty[i] + 1, perm);

    ut_free_1d_int (perm);

    // init FaceEdgeOri and FaceVerNb
    (*pTess).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    (*pTess).FaceVerNb[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);

    edge = (*pTess).FaceEdgeNb[i][1];
    ver = (*pTess).EdgeVerNb[edge][1];
    (*pTess).FaceEdgeOri[i][1] = 1;
    (*pTess).FaceVerNb[i][1] = (*pTess).EdgeVerNb[edge][0];
    (*pTess).FaceVerNb[i][2] = (*pTess).EdgeVerNb[edge][1];

    for (j = 2; j <= (*pTess).FaceVerQty[i]; j++)
    {
      edge = (*pTess).FaceEdgeNb[i][j];
      if ((*pTess).EdgeVerNb[edge][0] == ver)
      {
	(*pTess).FaceEdgeOri[i][j] = 1;
	if (j < (*pTess).FaceVerQty[i])
	  (*pTess).FaceVerNb[i][j + 1] = (*pTess).EdgeVerNb[edge][1];
	ver = (*pTess).EdgeVerNb[edge][1];
      }
      else if ((*pTess).EdgeVerNb[edge][1] == ver)
      {
	(*pTess).FaceEdgeOri[i][j] = -1;
	if (j < (*pTess).FaceVerQty[i])
	  (*pTess).FaceVerNb[i][j + 1] = (*pTess).EdgeVerNb[edge][0];
	ver = (*pTess).EdgeVerNb[edge][0];
      }
      else
	ut_error_reportbug ();
    }
  }

  // reverse FaceVerNb and FaceEdgeNb and modify FaceEdgeOri
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if (neut_tess_test_face_normal (*pTess, i, 0) != 0)
    {
      ut_array_1d_int_reverseelts ((*pTess).FaceEdgeNb[i] + 2,
				   (*pTess).FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts ((*pTess).FaceVerNb[i] + 2,
				   (*pTess).FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts ((*pTess).FaceEdgeOri[i] + 2,
				   (*pTess).FaceVerQty[i] - 1);
      ut_array_1d_int_scale ((*pTess).FaceEdgeOri[i] + 1,
			     (*pTess).FaceVerQty[i], -1);
    }

  // init FacePt, FacePtCoo
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  ut_array_1d_int_set ((*pTess).FacePt + 1, (*pTess).FaceQty, -1);

  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);

  return;
}

double
neut_tess_face_ff (struct TESS Tess, int face)
{
  int j, k, ver1, ver2, ver3;
  double ff, thisff;
  double **eq = ut_alloc_2d (Tess.FaceVerQty[face] + 1, 4);

  if (Tess.FaceState[face] == 0)
    return 0;

  else if (Tess.FacePt[face] == 0)
  {
    ff = 0;
    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
    {
      ver2 =
	Tess.FaceVerNb[face][ut_num_rotpos (1, Tess.FaceVerQty[face], j, 0)];
      ver3 =
	Tess.FaceVerNb[face][ut_num_rotpos (1, Tess.FaceVerQty[face], j, 1)];

      ut_space_points_plane (Tess.FacePtCoo[face], Tess.VerCoo[ver2],
			     Tess.VerCoo[ver3], eq[j]);
    }

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      for (k = j + 1; k <= Tess.FaceVerQty[face]; k++)
      {
	thisff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
	ff = ut_num_max (thisff, ff);
      }
  }

  else
  {
    ff = 0;

    /* ver i is the face point. */
    ver1 = Tess.FaceVerNb[face][Tess.FacePt[face]];

    for (j = 1; j <= Tess.FaceVerQty[face] - 2; j++)
    {
      ver2 =
	Tess.FaceVerNb[face][ut_num_rotpos
			     (1, Tess.FaceVerQty[face],
			      Tess.FacePt[face], j)];
      ver3 =
	Tess.FaceVerNb[face][ut_num_rotpos
			     (1, Tess.FaceVerQty[face],
			      Tess.FacePt[face], j + 1)];

      ut_space_points_plane (Tess.VerCoo[ver1], Tess.VerCoo[ver2],
			     Tess.VerCoo[ver3], eq[j]);
    }
    for (j = 1; j <= Tess.FaceVerQty[face] - 2; j++)
      for (k = j + 1; k <= Tess.FaceVerQty[face] - 2; k++)
      {
	thisff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
	ff = ut_num_max (thisff, ff);
      }
  }

  ut_free_2d (eq, Tess.FaceVerQty[face] + 1);

  return ff;
}

void
neut_tess_init_celltrue (struct TESS *pTess)
{
  int i, j, all, cell, qty;
  int trueval, status;
  int **neighcell = NULL;
  int *neighcellqty = NULL;

  (*pTess).CellTrue =
    ut_realloc_1d_int ((*pTess).CellTrue, (*pTess).CellQty + 1);

  if (!strcmp ((*pTess).Type, "periodic"))
  {
    ut_array_1d_int_set ((*pTess).CellTrue + 1, (*pTess).CellQty, 1);
    return;
  }

  for (i = 1; i <= (*pTess).CellQty; i++)
    (*pTess).CellTrue[i] = neut_tess_cell_true (*pTess, i);

  neighcell = ut_alloc_1d_pint ((*pTess).CellQty + 1);
  neighcellqty = ut_alloc_1d_int ((*pTess).CellQty + 1);

  for (i = 1; i <= (*pTess).CellQty; i++)
    neut_tess_cell_neighcell (*pTess, i, &(neighcell[i]), &(neighcellqty[i]));

  status = 1;
  for (trueval = 2; status == 1; trueval++)
  {
    status = 0;
    for (i = 1; i <= (*pTess).CellQty; i++)
    {
      if ((*pTess).Dim == 3)
	qty = (*pTess).PolyFaceQty[i];
      else if ((*pTess).Dim == 2)
	qty = (*pTess).FaceVerQty[i];
      else
	abort ();

      if (qty == 0)
	continue;

      // if trueval is 1 lower and the cell has all neighbours
      if ((*pTess).CellTrue[i] == trueval - 1 && neighcellqty[i] == qty)
      {
	all = 1;
	for (j = 0; j < neighcellqty[i]; j++)
	{
	  cell = neighcell[i][j];

	  if ((*pTess).CellTrue[cell] < trueval - 1)
	  {
	    all = 0;
	    break;
	  }
	}
	if (all == 1)
	{
	  (*pTess).CellTrue[i] = trueval;
	  status = 1;
	}
      }
    }
  }

  ut_free_2d_int (neighcell, (*pTess).CellQty + 1);
  ut_free_1d_int (neighcellqty);

  return;
}

void
neut_tess_init_cellbody (struct TESS *pTess)
{
  int i, j, all, cell, qty;
  int body, status;
  int **neighcell = NULL;
  int *neighcellqty = NULL;

  (*pTess).CellBody =
    ut_realloc_1d_int ((*pTess).CellBody, (*pTess).CellQty + 1);

  if (!strcmp ((*pTess).Type, "periodic"))
  {
    ut_array_1d_int_set ((*pTess).CellBody + 1, (*pTess).CellQty, 1);
    return;
  }

  for (i = 1; i <= (*pTess).CellQty; i++)
    (*pTess).CellBody[i] = neut_tess_cell_body (*pTess, i);

  neighcell = ut_alloc_1d_pint ((*pTess).CellQty + 1);
  neighcellqty = ut_alloc_1d_int ((*pTess).CellQty + 1);

  for (i = 1; i <= (*pTess).CellQty; i++)
    neut_tess_cell_neighcell (*pTess, i, &(neighcell[i]), &(neighcellqty[i]));

  status = 1;
  for (body = 2; status == 1; body++)
  {
    status = 0;
    for (i = 1; i <= (*pTess).CellQty; i++)
    {
      if ((*pTess).Dim == 3)
	qty = (*pTess).PolyFaceQty[i];
      else if ((*pTess).Dim == 2)
	qty = (*pTess).FaceVerQty[i];
      else
	abort ();

      if (qty == 0)
	continue;

      // if body is 1 lower and the cell has all neighbours
      if ((*pTess).CellBody[i] == body - 1 && neighcellqty[i] == qty)
      {
	all = 1;
	for (j = 0; j < neighcellqty[i]; j++)
	{
	  cell = neighcell[i][j];

	  if ((*pTess).CellBody[cell] < body - 1)
	  {
	    all = 0;
	    break;
	  }
	}
	if (all == 1)
	{
	  (*pTess).CellBody[i] = body;
	  status = 1;
	}
      }
    }
  }

  ut_free_2d_int (neighcell, (*pTess).CellQty + 1);
  ut_free_1d_int (neighcellqty);

  return;
}

// This function should only be used if the domain is not known
// (reconstruction from a mesh or a tess <= 1.9.2).
// Otherwise, use neut_tess_init_domain_poly.
void
neut_tess_init_domain (struct TESS *pTess)
{
  if ((*pTess).Dim == 3)
    neut_tess_init_domain_3d (pTess);
  else if ((*pTess).Dim == 2)
    neut_tess_init_domain_2d (pTess);
  else
    ut_error_reportbug ();

  return;
}

void
neut_tess_init_domain_3d (struct TESS *pTess)
{
  int i, face, poly, faceori;

  neut_tess_free_domain (pTess);

  ut_string_string ("unknown", &(*pTess).DomType);

  // Init FaceDom, DomFaceQty, DomFaceEq, DomTessFaceQty and DomTessFaceNb
  (*pTess).DomFaceQty = neut_tess_init_facedom_fromfacepoly (pTess);

  neut_tess_init_domtessface (pTess);

  (*pTess).DomFaceType = ut_alloc_1d_pchar ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceParmQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParms = ut_alloc_1d_pdouble ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    face = (*pTess).DomTessFaceNb[i][1];
    poly = ut_array_1d_int_max ((*pTess).FacePoly[face], 2);
    neut_tess_poly_face_ori (*pTess, poly, face, &faceori);
    ut_array_1d_memcpy ((*pTess).DomFaceEq[i], 4, (*pTess).FaceEq[face]);
    ut_array_1d_scale ((*pTess).DomFaceEq[i], 4, faceori);
    (*pTess).DomFaceParmQty[i] = 4;
    (*pTess).DomFaceParms[i] = ut_alloc_1d ((*pTess).DomFaceParmQty[i]);
    ut_array_1d_memcpy ((*pTess).DomFaceParms[i], 4, (*pTess).DomFaceEq[i]);

    ut_string_string ("plane", (*pTess).DomFaceType + i);
  }

  neut_tess_init_domain_3d_fromdomfaces (pTess);

  return;
}

void
neut_tess_init_domain_3d_fromdomfaces (struct TESS *pTess)
{
  int i, j, k, ver, domver, domedge, face;
  int *domface_edgeqty = NULL;
  int **domface_edges = NULL;
  int *domedge_verqty = NULL;
  int **domedge_vers = NULL;
  int interqty;
  int *inter = NULL;

  // Init DomEdgeQty, DomEdgeFaceNb, DomTessEdgeQty, DomTessEdgeNb.
  domface_edges = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  domface_edgeqty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    neut_tess_domface_edges ((*pTess), i, &(domface_edges[i]),
			     &(domface_edgeqty[i]));

  (*pTess).DomEdgeQty = 0;
  (*pTess).DomEdgeFaceNb = ut_alloc_1d_pint (1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint (1);

  if (!(*pTess).EdgeDom)
    (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  ut_array_2d_int_set ((*pTess).EdgeDom + 1, (*pTess).EdgeQty, 2, -1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    for (j = i + 1; j <= (*pTess).DomFaceQty; j++)
    {
      inter = ut_alloc_1d_int (domface_edgeqty[i]);

      ut_array_1d_int_inter (domface_edges[i], domface_edgeqty[i],
			     domface_edges[j], domface_edgeqty[j],
			     inter, &interqty);

      // means that the domain faces share one or several domain edges
      if (interqty > 0)
      {
        int **edges = NULL, *edgeqty = NULL, qty = 0;

        neut_tess_edges_contiguousedges (*pTess, -1,
                                         inter, interqty,
                                         &qty, &edges, &edgeqty);

        for (k = 0; k < qty; k++)
        {
          (*pTess).DomEdgeQty++;
          domedge = (*pTess).DomEdgeQty;

          (*pTess).DomEdgeFaceNb =
            ut_realloc_2d_int_addline ((*pTess).DomEdgeFaceNb,
                                       (*pTess).DomEdgeQty + 1, 2);
          (*pTess).DomEdgeFaceNb[domedge][0] = i;
          (*pTess).DomEdgeFaceNb[domedge][1] = j;

          (*pTess).DomTessEdgeQty = ut_realloc_1d_int ((*pTess).DomTessEdgeQty,
                                                       (*pTess).DomEdgeQty + 1);
          (*pTess).DomTessEdgeNb =
            ut_realloc_2d_int_addline ((*pTess).DomTessEdgeNb,
                                       (*pTess).DomEdgeQty + 1, edgeqty[k] + 1);

          (*pTess).DomTessEdgeQty[domedge] = edgeqty[k];
          ut_array_1d_int_memcpy ((*pTess).DomTessEdgeNb[domedge] + 1, edgeqty[k],
                                  edges[k]);
        }

        ut_free_2d_int_ (&edges, qty);
        ut_free_1d_int_ (&edgeqty);
      }

      ut_free_1d_int (inter);
    }

  // Init DomVerQty, DomVerEdgeQty, DomVerEdgeNb and DomTessVerNb
  domedge_vers = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  domedge_verqty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    neut_tess_domedge_ver ((*pTess), i, &(domedge_vers[i]),
			   &(domedge_verqty[i]));

  (*pTess).DomVerQty = 0;
  (*pTess).DomVerCoo = ut_alloc_1d_pdouble (1);
  (*pTess).DomVerEdgeNb = ut_alloc_1d_pint (1);
  (*pTess).DomTessVerNb = ut_alloc_1d_int (1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = i + 1; j <= (*pTess).DomEdgeQty; j++)
    {
      inter = ut_alloc_1d_int (domedge_verqty[i]);

      ut_array_1d_int_inter (domedge_vers[i], domedge_verqty[i],
			     domedge_vers[j], domedge_verqty[j],
			     inter, &interqty);

      if (interqty < 0 || interqty > 2)
	ut_error_reportbug ();

      if (interqty == 1)	// means that the domain edges share a domain ver
      {
	ver = inter[0];
	domver =
	  ut_array_1d_int_eltpos ((*pTess).DomTessVerNb,
				  (*pTess).DomVerQty + 1, ver);

	if (domver == -1)
	{
	  domver = ++(*pTess).DomVerQty;

	  (*pTess).DomVerEdgeQty =
	    ut_realloc_1d_int ((*pTess).DomVerEdgeQty,
			       (*pTess).DomVerQty + 1);
	  (*pTess).DomVerEdgeQty[domver] = 2;

	  (*pTess).DomVerEdgeNb =
	    ut_realloc_2d_int_addline ((*pTess).DomVerEdgeNb,
				       (*pTess).DomVerQty + 1, 2);
	  (*pTess).DomVerEdgeNb[domver][0] = i;
	  (*pTess).DomVerEdgeNb[domver][1] = j;

	  (*pTess).DomTessVerNb = ut_realloc_1d_int ((*pTess).DomTessVerNb,
						     (*pTess).DomVerQty + 1);

	  (*pTess).DomTessVerNb[domver] = ver;

	  (*pTess).DomVerCoo =
	    ut_realloc_2d_addline ((*pTess).DomVerCoo,
				   (*pTess).DomVerQty + 1, 3);
	  ut_array_1d_memcpy ((*pTess).DomVerCoo[domver], 3,
			      (*pTess).VerCoo[ver]);
	}
	else
	{
	  int *tmpedge = ut_alloc_1d_int (2);
	  tmpedge[0] = i;
	  tmpedge[1] = j;

	  for (k = 0; k < 2; k++)
	    if (ut_array_1d_int_eltpos ((*pTess).DomVerEdgeNb[domver],
					(*pTess).DomVerEdgeQty[domver],
					tmpedge[k]) == -1)
	    {
	      (*pTess).DomVerEdgeQty[domver]++;

	      (*pTess).DomVerEdgeNb[domver]
		= ut_realloc_1d_int ((*pTess).DomVerEdgeNb[domver],
				     (*pTess).DomVerEdgeQty[domver]);

	      (*pTess).DomVerEdgeNb[domver][(*pTess).DomVerEdgeQty[domver] -
					    1] = tmpedge[k];
	    }

	  ut_free_1d_int (tmpedge);
	}
      }

      ut_free_1d_int (inter);
    }

  // Init DomEdgeVerNb
  neut_tess_init_domain_domedgevernb (pTess);

  // Init DomFaceVerQty, DomFaceEdgeNb and DomFaceVerNb
  (*pTess).DomFaceEdgeQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = 0; j < 2; j++)
    {
      face = (*pTess).DomEdgeFaceNb[i][j];

      (*pTess).DomFaceEdgeQty[face]++;
      (*pTess).DomFaceEdgeNb[face]
	=
	ut_realloc_1d_int ((*pTess).DomFaceEdgeNb[face],
			   (*pTess).DomFaceEdgeQty[face] + 1);
      (*pTess).DomFaceEdgeNb[face][(*pTess).DomFaceEdgeQty[face]] = i;
    }

  (*pTess).DomFaceVerQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    int *id = NULL;
    neut_tess_domedges_domvers (*pTess, (*pTess).DomFaceEdgeNb[i] + 1,
				(*pTess).DomFaceEdgeQty[i],
                                &id,
                                (*pTess).DomFaceVerQty + i);

    if (ut_array_1d_int_eltpos (id, (*pTess).DomFaceVerQty[i], 0) != -1)
      abort ();

    if ((*pTess).DomFaceVerQty[i] > (*pTess).DomFaceEdgeQty[i])
      ut_error_reportbug ();

    (*pTess).DomFaceVerNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTess).DomFaceVerNb[i] + 1,
                            (*pTess).DomFaceVerQty[i], id);

    ut_free_1d_int (id);
  }

  // Should not be used (fails for non-convex domains)
  // neut_tess_init_edgedom_fromfacedom (pTess);
  // neut_tess_init_verdom_fromfacedom (pTess);

  // updating EdgeDom

  int edge;
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = 1; j <= (*pTess).DomTessEdgeQty[i]; j++)
    {
      edge = (*pTess).DomTessEdgeNb[i][j];
      (*pTess).EdgeDom[edge][0] = 1;
      (*pTess).EdgeDom[edge][1] = i;
    }

  int *domfaces = NULL, domfaceqty;
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).EdgeDom[i][0] == -1)
    {
      neut_tess_edge_domfaces (*pTess, i, &domfaces, &domfaceqty);
      if (domfaceqty > 1)
      {
        ut_error_reportbug ();
      }
      if (domfaceqty == 1)
      {
        (*pTess).EdgeDom[i][0] = 2;
        (*pTess).EdgeDom[i][1] = domfaces[0];
      }
    }
  ut_free_1d_int_ (&domfaces);

  // updating VerDom from DomTess*
  if (!(*pTess).VerDom)
    (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);

  ut_array_2d_int_set ((*pTess).VerDom + 1, (*pTess).VerQty, 2, -1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    ver = (*pTess).DomTessVerNb[i];
    (*pTess).VerDom[ver][0] = 0;
    (*pTess).VerDom[ver][1] = i;
  }

  int *vers = NULL, verqty;
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    neut_tess_domedge_vers (*pTess, i, &vers, &verqty);

    for (j = 0; j < verqty; j++)
    {
      ver = vers[j];

      if ((*pTess).VerDom[ver][0] == -1)
      {
        (*pTess).VerDom[ver][0] = 1;
        (*pTess).VerDom[ver][1] = i;
      }
    }

    ut_free_1d_int_ (&vers);
  }

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    neut_tess_domface_vers (*pTess, i, &vers, &verqty);

    for (j = 0; j < verqty; j++)
    {
      ver = vers[j];

      if ((*pTess).VerDom[ver][0] == -1)
      {
        (*pTess).VerDom[ver][0] = 2;
        (*pTess).VerDom[ver][1] = i;
      }
    }

    ut_free_1d_int_ (&vers);
  }

  neut_tess_init_domain_label (pTess);

  return;
}

void
neut_tess_init_domain_2d (struct TESS *pTess)
{
  int i, j, domver;
  int *domedge_verqty = NULL;
  int **domedge_vers = NULL;
  int interqty;
  int *inter = NULL;

  // Init EdgeDom, DomEdgeQty, DomEdgeEq, DomTessEdgeQty and DomTessEdgeNb
  (*pTess).DomEdgeQty = neut_tess_init_edgedom_fromedgeface (pTess);

  neut_tess_init_domtessedge (pTess);

  // Init DomVerQty, DomVerEdgeNb, DomTessVerQty, DomTessVerNb.
  domedge_vers = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  domedge_verqty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    neut_tess_domedge_vers ((*pTess), i, &(domedge_vers[i]),
			    &(domedge_verqty[i]));

  (*pTess).DomVerQty = 0;
  (*pTess).DomVerCoo = ut_alloc_1d_pdouble (1);
  (*pTess).DomVerEdgeNb = ut_alloc_1d_pint (1);
  (*pTess).DomTessVerNb = ut_alloc_1d_int (1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = i + 1; j <= (*pTess).DomEdgeQty; j++)
    {
      inter = ut_alloc_1d_int (domedge_verqty[i]);

      ut_array_1d_int_inter (domedge_vers[i], domedge_verqty[i],
			     domedge_vers[j], domedge_verqty[j],
			     inter, &interqty);

      if (interqty > 0)		// means that the domain edges share a domain ver
      {
	if (interqty != 1)
	  ut_error_reportbug ();

	(*pTess).DomVerQty++;
	domver = (*pTess).DomVerQty;

	(*pTess).DomVerCoo =
	  ut_realloc_2d_addline ((*pTess).DomVerCoo, (*pTess).DomVerQty + 1,
				 3);
	(*pTess).DomVerEdgeQty =
	  ut_realloc_1d_int ((*pTess).DomVerEdgeQty, (*pTess).DomVerQty + 1);
	(*pTess).DomVerEdgeNb =
	  ut_realloc_2d_int_addline ((*pTess).DomVerEdgeNb,
				     (*pTess).DomVerQty + 1, 2);
	(*pTess).DomVerEdgeQty[domver] = 2;
	(*pTess).DomVerEdgeNb[domver][0] = i;
	(*pTess).DomVerEdgeNb[domver][1] = j;

	(*pTess).DomTessVerNb = ut_realloc_1d_int
	  ((*pTess).DomTessVerNb, (*pTess).DomVerQty + 1);

	(*pTess).DomTessVerNb[domver] = inter[0];
	ut_array_1d_memcpy ((*pTess).DomVerCoo[domver], 3,
			    (*pTess).VerCoo[inter[0]]);
      }

      ut_free_1d_int (inter);
    }

  neut_tess_init_domain_domedgevernb (pTess);
  (*pTess).DomEdgeFaceNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);

  neut_tess_init_edgedom_fromedgeface (pTess);

  neut_tess_init_verdom_fromedgedom (pTess);

  // Settings face labels
  neut_tess_init_domain_label (pTess);

  return;
}

void
neut_tess_init_domain_poly (struct TESS *pTess, struct TESS Tess,
			    int poly, int **pVerNb, int **pEdgeNb,
			    int **pFaceNb)
{
  struct TESS DomTess;
  int verqty, edgeqty;
  int *vers = NULL, *edges = NULL;

  neut_tess_set_zero (&DomTess);

  neut_tess_poly_tess (Tess, poly, &DomTess);

  if (pVerNb)
  {
    neut_tess_poly_vers (Tess, poly, &vers, &verqty);
    (*pVerNb) = ut_alloc_1d_int (verqty + 1);
    ut_array_1d_int_memcpy ((*pVerNb) + 1, verqty, vers);
  }

  if (pEdgeNb)
  {
    neut_tess_poly_edges (Tess, poly, &edges, &edgeqty);
    (*pEdgeNb) = ut_alloc_1d_int (edgeqty + 1);
    ut_array_1d_int_memcpy ((*pEdgeNb) + 1, edgeqty, edges);
  }

  if (pFaceNb)
  {
    (*pFaceNb) = ut_alloc_1d_int (Tess.PolyFaceQty[poly] + 1);
    ut_array_1d_int_memcpy ((*pFaceNb) + 1, Tess.PolyFaceQty[poly],
			    Tess.PolyFaceNb[poly] + 1);
  }

  // Copying domain to the TESS
  neut_tess_init_domain_memcpy (pTess, DomTess);

  // We are left with linking the Tess to the Domain

  neut_tess_init_facedom_fromfacepoly (pTess);
  neut_tess_init_edgedom_fromfacedom (pTess);
  neut_tess_init_verdom_fromfacedom (pTess);

  neut_tess_init_domtessface (pTess);
  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver (pTess);

  neut_tess_free (&DomTess);
  ut_free_1d_int (vers);
  ut_free_1d_int (edges);

  return;
}

void
neut_tess_init_domtessface (struct TESS *pTess)
{
  int i, id;

  if ((*pTess).Dim < 3)
    return;

  ut_free_1d_int ((*pTess).DomTessFaceQty);
  ut_free_2d_int ((*pTess).DomTessFaceNb, (*pTess).DomFaceQty + 1);

  (*pTess).DomTessFaceQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceDom[i][0] == 2)
    {
      id = (*pTess).FaceDom[i][1];
      (*pTess).DomTessFaceQty[id]++;
      (*pTess).DomTessFaceNb[id] =
	ut_realloc_1d_int ((*pTess).DomTessFaceNb[id],
			   (*pTess).DomTessFaceQty[id] + 1);
      (*pTess).DomTessFaceNb[id][(*pTess).DomTessFaceQty[id]] = i;
    }

  return;
}

void
neut_tess_init_domtessedge (struct TESS *pTess)
{
  int i, id;

  ut_free_1d_int ((*pTess).DomTessEdgeQty);
  ut_free_2d_int ((*pTess).DomTessEdgeNb, (*pTess).DomEdgeQty + 1);

  (*pTess).DomTessEdgeQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).EdgeDom[i][0] == 1)
    {
      id = (*pTess).EdgeDom[i][1];
      (*pTess).DomTessEdgeQty[id]++;
      (*pTess).DomTessEdgeNb[id] =
	ut_realloc_1d_int ((*pTess).DomTessEdgeNb[id],
			   (*pTess).DomTessEdgeQty[id] + 1);
      (*pTess).DomTessEdgeNb[id][(*pTess).DomTessEdgeQty[id]] = i;
    }

  return;
}

void
neut_tess_init_domtessver (struct TESS *pTess)
{
  int i;
  int *nb = NULL;

  ut_free_1d_int ((*pTess).DomTessVerNb);

  (*pTess).DomTessVerNb = ut_alloc_1d_int ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerDom[i][0] == 0)
      (*pTess).DomTessVerNb[(*pTess).VerDom[i][1]] = i;

  ut_free_1d_int (nb);

  return;
}

void
neut_tess_init_verdom (struct TESS *pTess)
{
  int i, ver;

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
  ut_array_2d_int_set ((*pTess).VerDom + 1, (*pTess).VerQty, 1, -1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    ver = (*pTess).DomTessVerNb[i];
    (*pTess).VerDom[ver][0] = 0;
    (*pTess).VerDom[ver][1] = i;
  }

  return;
}

void
neut_tess_init_edgedom (struct TESS *pTess)
{
  int i, j, edge;

  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  ut_array_2d_int_set ((*pTess).EdgeDom + 1, (*pTess).EdgeQty, 1, -1);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = 1; j <= (*pTess).DomTessEdgeQty[i]; j++)
    {
      edge = (*pTess).DomTessEdgeNb[i][j];
      (*pTess).EdgeDom[edge][0] = 1;
      (*pTess).EdgeDom[edge][1] = i;
    }

  return;
}

void
neut_tess_init_facedom (struct TESS *pTess)
{
  int i, j, face;

  (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  ut_array_2d_int_set ((*pTess).FaceDom + 1, (*pTess).FaceQty, 1, -1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    for (j = 1; j <= (*pTess).DomTessFaceQty[i]; j++)
    {
      face = (*pTess).DomTessFaceNb[i][j];
      (*pTess).FaceDom[face][0] = 2;
      (*pTess).FaceDom[face][1] = i;
    }

  return;
}

int
neut_tess_init_facedom_fromfacepoly (struct TESS *pTess)
{
  int i, id, max = 0;

  // Init face bound
  (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    (*pTess).FaceDom[i][0] = -1;

  if ((*pTess).Dim > 2)
    for (i = 1; i <= (*pTess).FaceQty; i++)
    {
      id = -ut_array_1d_int_min ((*pTess).FacePoly[i], 2);

      if (id > 0)
      {
	(*pTess).FaceDom[i][0] = 2;
	(*pTess).FaceDom[i][1] = id;
	max = ut_num_max (max, id);
      }
    }

  return max;
}

int
neut_tess_init_alldom_fromdomtess (struct TESS *pTess)
{
  int i, j, k, ver, edge, face;

  if ((*pTess).VerDom)
    ut_free_2d_int ((*pTess).VerDom, (*pTess).VerQty + 1);
  if ((*pTess).EdgeDom)
    ut_free_2d_int ((*pTess).EdgeDom, (*pTess).EdgeQty + 1);
  if ((*pTess).FaceDom)
    ut_free_2d_int ((*pTess).FaceDom, (*pTess).FaceQty + 1);

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
  ut_array_2d_int_col_set ((*pTess).VerDom + 1, (*pTess).VerQty, 0, -1);
  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  ut_array_2d_int_col_set ((*pTess).EdgeDom + 1, (*pTess).EdgeQty, 0, -1);

  if ((*pTess).Dim == 3)
  {
    (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
    ut_array_2d_int_col_set ((*pTess).FaceDom + 1, (*pTess).FaceQty, 0, -1);
  }

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    for (j = 1; j <= (*pTess).DomTessFaceQty[i]; j++)
    {
      face = (*pTess).DomTessFaceNb[i][j];

      // setting FaceDom
      (*pTess).FaceDom[face][0] = 2;
      (*pTess).FaceDom[face][1] = i;
      (*pTess).FacePoly[face][1] = -i;

      // setting EdgeDom
      for (k = 1; k <= (*pTess).FaceVerQty[face]; k++)
      {
	edge = (*pTess).FaceEdgeNb[face][k];
	(*pTess).EdgeDom[edge][0] = 2;
	(*pTess).EdgeDom[edge][1] = i;
      }

      // setting VerDom
      for (k = 1; k <= (*pTess).FaceVerQty[face]; k++)
      {
	ver = (*pTess).FaceVerNb[face][k];
	(*pTess).VerDom[ver][0] = 2;
	(*pTess).VerDom[ver][1] = i;
      }
    }

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = 1; j <= (*pTess).DomTessEdgeQty[i]; j++)
    {
      edge = (*pTess).DomTessEdgeNb[i][j];

      // setting EdgeDom
      (*pTess).EdgeDom[edge][0] = 1;
      (*pTess).EdgeDom[edge][1] = i;

      // setting VerDom
      for (k = 0; k < 2; k++)
      {
	ver = (*pTess).EdgeVerNb[edge][k];
	(*pTess).VerDom[ver][0] = 1;
	(*pTess).VerDom[ver][1] = i;
      }
    }

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    ver = (*pTess).DomTessVerNb[i];

    (*pTess).VerDom[ver][0] = 0;
    (*pTess).VerDom[ver][1] = i;
  }

  return 0;
}

int
neut_tess_init_edgedom_fromfacedom (struct TESS *pTess)
{
  int qty, i, j, max, face, status;
  int *nb = NULL;

  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    (*pTess).EdgeDom[i][0] = -1;

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    nb = ut_alloc_1d_int ((*pTess).EdgeFaceQty[i]);

    qty = 0;
    for (j = 0; j < (*pTess).EdgeFaceQty[i]; j++)
    {
      face = (*pTess).EdgeFaceNb[i][j];

      if (face > 0 && (*pTess).FaceDom[face][0] == 2)
	nb[qty++] = (*pTess).FaceDom[face][1];
    }

    ut_array_1d_int_sort_uniq (nb, qty, &qty);

    if (qty == 0)		// the edge is not on the domain
      (*pTess).EdgeDom[i][0] = -1;
    else if (qty == 1)		// the edge is on a domain face
    {
      (*pTess).EdgeDom[i][0] = 2;
      (*pTess).EdgeDom[i][1] = nb[1];
    }
    else if (qty == 2)		// the edge is on a domain edge
    {
      (*pTess).EdgeDom[i][0] = 1;
      status =
	neut_tess_domfaces_comdomedge (*pTess, nb, 2,
				       &((*pTess).EdgeDom[i][1]));
      if (status == -1)
	ut_error_reportbug ();
    }
    else
      ut_error_reportbug ();

    ut_free_1d_int (nb);
  }

  max = 0;
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).EdgeDom[i][0] == 1)
      max = ut_num_max (max, (*pTess).EdgeDom[i][1]);

  return max;
}

int
neut_tess_init_edgedom_fromedgeface (struct TESS *pTess)
{
  int i, id, max;

  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    (*pTess).EdgeDom[i][0] = -1;

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    id = -ut_array_1d_int_min ((*pTess).EdgeFaceNb[i],
			       (*pTess).EdgeFaceQty[i]);

    if (id > 0)
    {
      (*pTess).EdgeDom[i][0] = 1;
      (*pTess).EdgeDom[i][1] = id;
    }
  }

  max = 0;
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).EdgeDom[i][0] == 1)
      max = ut_num_max (max, (*pTess).EdgeDom[i][1]);

  return max;
}

int
neut_tess_init_verdom_fromfacedom (struct TESS *pTess)
{
  int i, j, max = 0, verbosity = 0;
  int *faces = NULL, faceqty;
  int *domfaces = NULL, domfaceqty;

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
  for (i = 1; i <= (*pTess).VerQty; i++)
    (*pTess).VerDom[i][0] = -1;

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    neut_tess_ver_faces (*pTess, i, &faces, &faceqty);

    if (verbosity)
    {
      printf ("ver_face = ");
      ut_array_1d_int_fprintf (stdout, faces, faceqty, "%d");
    }

    domfaces = ut_alloc_1d_int (faceqty + 1);

    domfaceqty = 0;
    for (j = 0; j < faceqty; j++)
      if ((*pTess).FaceDom[faces[j]][0] == 2)
	domfaces[domfaceqty++] = (*pTess).FaceDom[faces[j]][1];

    if (verbosity)
    {
      printf ("ver_domface = ");
      ut_array_1d_int_fprintf (stdout, domfaces, domfaceqty, "%d");
    }

    ut_array_1d_int_sort_uniq (domfaces, domfaceqty, &domfaceqty);

    if (verbosity)
    {
      printf ("ver_domface = ");
      ut_array_1d_int_fprintf (stdout, domfaces, faceqty, "%d");
    }

    if (domfaceqty == 0)	// the ver is not on the domain
      (*pTess).VerDom[i][0] = -1;
    else if (domfaceqty == 1)	// the ver is on a domain faces
    {
      (*pTess).VerDom[i][0] = 2;
      (*pTess).VerDom[i][1] = domfaces[0];
    }
    else if (domfaceqty == 2)	// the ver is on a domain edge
    {
      (*pTess).VerDom[i][0] = 1;
      if (neut_tess_domfaces_comdomedge
	  (*pTess, domfaces, domfaceqty, &((*pTess).VerDom[i][1])) != 0)
	ut_error_reportbug ();
    }
    else if (domfaceqty >= 3)	// the ver is on a domain ver
    {
      (*pTess).VerDom[i][0] = 0;
      if (neut_tess_domfaces_comdomver
	  (*pTess, domfaces, domfaceqty, &((*pTess).VerDom[i][1])) != 0)
      {
        printf ("domfaces = ");
        ut_array_1d_int_fprintf (stdout, domfaces, domfaceqty, "%d");
	ut_error_reportbug ();
      }
    }

    ut_free_1d_int_ (&domfaces);
    ut_free_1d_int_ (&faces);
  }

  max = 0;
  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerDom[i][0] == 0)
      max = ut_num_max (max, (*pTess).VerDom[i][1]);

  return max;
}

int
neut_tess_init_verdom_fromedgedom (struct TESS *pTess)
{
  int i, j, max = 0;

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
  for (i = 1; i <= (*pTess).VerQty; i++)
    (*pTess).VerDom[i][0] = -1;

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    int domedgeqty;
    int *domedge = ut_alloc_1d_int ((*pTess).VerEdgeQty[i]);

    domedgeqty = 0;
    for (j = 0; j < (*pTess).VerEdgeQty[i]; j++)
    {
      int edge = (*pTess).VerEdgeNb[i][j];
      if ((*pTess).EdgeDom[edge][0] == 1)
	domedge[domedgeqty++] = (*pTess).EdgeDom[edge][1];
    }

    ut_array_1d_int_sort_uniq (domedge, domedgeqty, &domedgeqty);

    if (domedgeqty == 0)	// the ver is not on the domain
      (*pTess).VerDom[i][0] = -1;
    else if (domedgeqty == 1)	// the ver is on a domain edge
    {
      (*pTess).VerDom[i][0] = 1;
      (*pTess).VerDom[i][1] = domedge[0];
    }
    else if (domedgeqty >= 2)	// the ver is on a domain ver
    {
      (*pTess).VerDom[i][0] = 0;
      if (neut_tess_domedges_comdomver
	  (*pTess, domedge, domedgeqty, &((*pTess).VerDom[i][1])) != 0)
      {
	printf ("no common domver for domedges ");
	ut_array_1d_int_fprintf (stdout, domedge, domedgeqty, "%d");
	ut_error_reportbug ();
      }
    }

    ut_free_1d_int (domedge);
  }

  max = 0;
  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerDom[i][0] == 0)
      max = ut_num_max (max, (*pTess).VerDom[i][1]);

  return max;
}

int
neut_tess_init_verdom_fromveredge (struct TESS *pTess)
{
  int i, id, max = 0;

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);
  for (i = 1; i <= (*pTess).VerQty; i++)
    (*pTess).VerDom[i][0] = -1;

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    id = -ut_array_1d_int_min ((*pTess).VerEdgeNb[i], (*pTess).VerEdgeQty[i]);

    if (id > 0)
    {
      (*pTess).VerDom[i][0] = 0;
      (*pTess).VerDom[i][1] = id;
      max = ut_num_max (max, id);
    }
  }

  max = 0;
  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerDom[i][0] == 0)
      max = ut_num_max (max, (*pTess).VerDom[i][1]);

  return max;
}

void
neut_tess_init_facedom_face_v (struct TESS *pTess, int face)
{
  int i, ver, qty;
  int domfaceqty1, domfaceqty2;
  int *domface1 = NULL;
  int *domface2 = NULL;

  // default value
  (*pTess).FaceDom[face][0] = -1;

  // if a ver is not on the domain, the face is not on the domain
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    ver = (*pTess).FaceVerNb[face][i];

    if ((*pTess).VerDom[ver][0] == -1)
    {
      (*pTess).FaceDom[face][0] = -1;
      return;
    }
  }

  // for all of the face vertices
  domfaceqty1 = 0;
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    ver = (*pTess).FaceVerNb[face][i];

    int *tmp = NULL;
    if ((*pTess).VerDom[ver][0] == 0)
      neut_tess_domver_domface (*pTess, (*pTess).VerDom[ver][1], &tmp, &qty);
    else if ((*pTess).VerDom[ver][0] == 1)
    {
      qty = 2;
      tmp = ut_alloc_1d_int (qty);
      ut_array_1d_int_memcpy (tmp, 2,
			      (*pTess).
			      DomEdgeFaceNb[(*pTess).VerDom[ver][1]]);
    }
    else if ((*pTess).VerDom[ver][0] == 2)
    {
      qty = 1;
      tmp = ut_alloc_1d_int (qty);
      tmp[0] = (*pTess).VerDom[ver][1];
    }

    domface1 = ut_realloc_1d_int (domface1, domfaceqty1 + qty);
    ut_array_1d_int_memcpy (domface1 + domfaceqty1, qty, tmp);
    domfaceqty1 += qty;

    ut_free_1d_int (tmp);
  }

  domface2 = ut_alloc_1d_int (domfaceqty1);
  ut_array_1d_int_memcpy (domface2, domfaceqty1, domface1);
  ut_array_1d_int_sort_uniq (domface2, domfaceqty1, &domfaceqty2);
  domface2 = ut_realloc_1d_int (domface2, domfaceqty2);

  for (i = 0; i < domfaceqty2; i++)
  {
    qty = ut_array_1d_int_nbofthisval (domface1, domfaceqty1, domface2[i]);
    if (qty == (*pTess).FaceVerQty[face])
    {
      (*pTess).FaceDom[face][0] = 2;
      (*pTess).FaceDom[face][1] = domface2[i];
      break;
    }
  }

  ut_free_1d_int (domface1);
  ut_free_1d_int (domface2);

  return;
}

void
neut_tess_cat (struct TESS *pTessA, struct TESS TessB)
{
  int i;
  int VerQty0 = (*pTessA).VerQty;
  int EdgeQty0 = (*pTessA).EdgeQty;
  int FaceQty0 = (*pTessA).FaceQty;
  int PolyQty0 = (*pTessA).PolyQty;

  // Adding vertices
  (*pTessA).VerQty += TessB.VerQty;
  (*pTessA).VerCoo
    = ut_realloc_1d_pdouble ((*pTessA).VerCoo, (*pTessA).VerQty + 1);

  (*pTessA).VerState
    = ut_realloc_1d_int ((*pTessA).VerState, (*pTessA).VerQty + 1);
  // neut_debug_tess (stdout, TessB);
  for (i = 1; i <= TessB.VerQty; i++)
  {
    (*pTessA).VerState[VerQty0 + i] = 0;
    (*pTessA).VerCoo[VerQty0 + i] = ut_alloc_1d (3);
    ut_array_1d_memcpy ((*pTessA).VerCoo[VerQty0 + i], 3, TessB.VerCoo[i]);
  }

  // Adding edges
  (*pTessA).EdgeQty += TessB.EdgeQty;
  (*pTessA).EdgeVerNb
    = ut_realloc_1d_pint ((*pTessA).EdgeVerNb, (*pTessA).EdgeQty + 1);
  (*pTessA).EdgeState
    = ut_realloc_1d_int ((*pTessA).EdgeState, (*pTessA).EdgeQty + 1);
  for (i = EdgeQty0 + 1; i <= (*pTessA).EdgeQty; i++)
  {
    (*pTessA).EdgeState[i] = 0;
    (*pTessA).EdgeVerNb[i] = ut_alloc_1d_int (2);
    ut_array_1d_int_memcpy
      ((*pTessA).EdgeVerNb[i], 2, TessB.EdgeVerNb[i - EdgeQty0]);
    ut_array_1d_int_addval ((*pTessA).EdgeVerNb[i], 2, VerQty0,
			    (*pTessA).EdgeVerNb[i]);
  }

  // Adding faces
  (*pTessA).FaceQty += TessB.FaceQty;
  (*pTessA).FaceState
    = ut_realloc_1d_int ((*pTessA).FaceState, (*pTessA).FaceQty + 1);
  (*pTessA).FaceVerQty
    = ut_realloc_1d_int ((*pTessA).FaceVerQty, (*pTessA).FaceQty + 1);
  for (i = 1; i <= TessB.FaceQty; i++)
  {
    (*pTessA).FaceState[i + FaceQty0] = 0;
    (*pTessA).FaceEdgeNb
      = ut_realloc_2d_int_addline ((*pTessA).FaceEdgeNb, FaceQty0 + i +
				   1, TessB.FaceVerQty[i] + 1);
    (*pTessA).FaceEdgeOri
      = ut_realloc_2d_int_addline ((*pTessA).FaceEdgeOri, FaceQty0 + i +
				   1, TessB.FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTessA).FaceEdgeNb[FaceQty0 + i] + 1,
			    TessB.FaceVerQty[i], TessB.FaceEdgeNb[i] + 1);
    ut_array_1d_int_memcpy ((*pTessA).FaceEdgeOri[FaceQty0 + i] + 1,
			    TessB.FaceVerQty[i], TessB.FaceEdgeOri[i] + 1);
    ut_array_1d_int_addval ((*pTessA).FaceEdgeNb[FaceQty0 + i] + 1,
			    TessB.FaceVerQty[i], EdgeQty0,
			    (*pTessA).FaceEdgeNb[FaceQty0 + i] + 1);

    (*pTessA).FaceVerQty[i + FaceQty0] = TessB.FaceVerQty[i];
  }

  // Adding polys
  (*pTessA).PolyQty += TessB.PolyQty;
  (*pTessA).PolyFaceQty
    = ut_realloc_1d_int ((*pTessA).PolyFaceQty, (*pTessA).PolyQty + 1);
  for (i = 1; i <= TessB.PolyQty; i++)
  {
    (*pTessA).PolyFaceNb
      = ut_realloc_2d_int_addline ((*pTessA).PolyFaceNb, PolyQty0 + i +
				   1, TessB.PolyFaceQty[i] + 1);
    (*pTessA).PolyFaceOri
      = ut_realloc_2d_int_addline ((*pTessA).PolyFaceOri, PolyQty0 + i +
				   1, TessB.PolyFaceQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTessA).PolyFaceOri[PolyQty0 + i] + 1,
			    TessB.PolyFaceQty[i], TessB.PolyFaceOri[i] + 1);
    ut_array_1d_int_memcpy ((*pTessA).PolyFaceNb[PolyQty0 + i] + 1,
			    TessB.PolyFaceQty[i], TessB.PolyFaceNb[i] + 1);
    ut_array_1d_int_addval ((*pTessA).PolyFaceNb[PolyQty0 + i] + 1,
			    TessB.PolyFaceQty[i], FaceQty0,
			    (*pTessA).PolyFaceNb[PolyQty0 + i] + 1);

    (*pTessA).PolyFaceQty[i + PolyQty0] = TessB.PolyFaceQty[i];
  }

  return;
}

void
neut_tess_poly_tess (struct TESS Tess, int poly, struct TESS *pTess)
{
  int i, j, ver, edge, face;
  int *VerNb_old_new = NULL;
  int VerNb_old_max;
  int *EdgeNb_old_new = NULL;
  int EdgeNb_old_max;
  int *FaceNb_old_new = NULL;
  int FaceNb_old_max;
  int *PolyVerNb = NULL;
  int PolyVerQty;
  int *PolyEdgeNb = NULL;
  int PolyEdgeQty;

  neut_tess_poly_vers (Tess, poly, &PolyVerNb, &PolyVerQty);
  neut_tess_poly_edges (Tess, poly, &PolyEdgeNb, &PolyEdgeQty);

  // General information
  (*pTess).Dim = Tess.Dim;
  (*pTess).PseudoDim = Tess.PseudoDim;
  (*pTess).PseudoSize = Tess.PseudoSize;
  (*pTess).Level = Tess.Level;
  (*pTess).TessId = Tess.TessId;
  (*pTess).CellQty = 1;

  if (Tess.Type)
    ut_string_string (Tess.Type, &((*pTess).Type));

  ut_array_1d_int_inv (PolyVerNb, PolyVerQty, &VerNb_old_new, &VerNb_old_max);
  ut_array_1d_int_addval (VerNb_old_new, VerNb_old_max, 1, VerNb_old_new);

  ut_array_1d_int_inv (PolyEdgeNb, PolyEdgeQty,
		       &EdgeNb_old_new, &EdgeNb_old_max);
  ut_array_1d_int_addval (EdgeNb_old_new, EdgeNb_old_max, 1, EdgeNb_old_new);

  ut_array_1d_int_inv (Tess.PolyFaceNb[poly] + 1, Tess.PolyFaceQty[poly],
		       &FaceNb_old_new, &FaceNb_old_max);
  ut_array_1d_int_addval (FaceNb_old_new, FaceNb_old_max, 1, FaceNb_old_new);
  FaceNb_old_max--;

  // Poly ------------

  (*pTess).PolyQty = 1;
  (*pTess).SeedQty = 1;
  (*pTess).SeedCoo = ut_alloc_2d (2, 3);
  ut_array_1d_memcpy ((*pTess).SeedCoo[1], 3, Tess.SeedCoo[poly]);

  if (Tess.SeedWeight)
  {
    (*pTess).SeedWeight = ut_alloc_1d (2);
    (*pTess).SeedWeight[1] = Tess.SeedWeight[poly];
  }

  (*pTess).CellTrue = ut_alloc_1d_int (2);
  (*pTess).CellBody = ut_alloc_1d_int (2);
  (*pTess).CellLamId = ut_alloc_1d_int (2);
  (*pTess).CellModeId = ut_alloc_1d_int (2);

  (*pTess).PolyFaceQty = ut_alloc_1d_int (2);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint (2);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint (2);
  (*pTess).PolyFaceQty[1] = Tess.PolyFaceQty[poly];
  (*pTess).PolyFaceNb[1] = ut_alloc_1d_int ((*pTess).PolyFaceQty[1] + 1);
  (*pTess).PolyFaceOri[1] = ut_alloc_1d_int ((*pTess).PolyFaceQty[1] + 1);
  for (j = 1; j <= (*pTess).PolyFaceQty[1]; j++)
  {
    (*pTess).PolyFaceNb[1][j] = FaceNb_old_new[Tess.PolyFaceNb[poly][j]];
    (*pTess).PolyFaceOri[1][j] = Tess.PolyFaceOri[poly][j];
  }

  // Faces ------------

  (*pTess).FaceQty = Tess.PolyFaceQty[poly];
  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);
  (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);
  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    (*pTess).FaceDom[i][0] = -1;

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    face = Tess.PolyFaceNb[poly][i];
    ut_array_1d_memcpy ((*pTess).FaceEq[i], 4, Tess.FaceEq[face]);

    (*pTess).FacePoly[i][0] = 1;
    (*pTess).FacePoly[i][1] = -i;	// here, loosing link with real neighbours

    (*pTess).FaceVerQty[i] = Tess.FaceVerQty[face];
    (*pTess).FaceVerNb[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
      (*pTess).FaceVerNb[i][j] = VerNb_old_new[Tess.FaceVerNb[face][j]];

    (*pTess).FaceEdgeNb[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    (*pTess).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
    {
      (*pTess).FaceEdgeNb[i][j] = EdgeNb_old_new[Tess.FaceEdgeNb[face][j]];
      (*pTess).FaceEdgeOri[i][j] = Tess.FaceEdgeOri[face][j];
    }
  }

  // Edges ---------
  (*pTess).EdgeQty = PolyEdgeQty;
  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);
  (*pTess).EdgeLength = ut_alloc_1d ((*pTess).EdgeQty + 1);
  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    (*pTess).EdgeDom[i][0] = -1;

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    edge = PolyEdgeNb[i - 1];
    for (j = 0; j < 2; j++)
    {
      (*pTess).EdgeVerNb[i][j] = VerNb_old_new[Tess.EdgeVerNb[edge][j]];
      if ((*pTess).EdgeVerNb[i][j] <= 0)
	abort ();
    }
    (*pTess).EdgeLength[i] = Tess.EdgeLength[edge];
  }

  // looping on the faces
  (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
    {
      edge = (*pTess).FaceEdgeNb[i][j];
      (*pTess).EdgeFaceQty[edge]++;
      (*pTess).EdgeFaceNb[edge]
	= ut_realloc_1d_int ((*pTess).EdgeFaceNb[edge],
			     (*pTess).EdgeFaceQty[edge]);
      (*pTess).EdgeFaceNb[edge][(*pTess).EdgeFaceQty[edge] - 1] = i;
    }

  // Vertices ---------
  (*pTess).VerQty = PolyVerQty;
  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    ver = PolyVerNb[i - 1];
    ut_array_1d_memcpy ((*pTess).VerCoo[i], 3, Tess.VerCoo[ver]);
  }

  for (i = 1; i <= (*pTess).VerQty; i++)
    (*pTess).VerDom[i][0] = -1;

  // looping on the edges
  (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    for (j = 0; j < 2; j++)
    {
      ver = (*pTess).EdgeVerNb[i][j];
      (*pTess).VerEdgeQty[ver]++;
      (*pTess).VerEdgeNb[ver]
	= ut_realloc_1d_int ((*pTess).VerEdgeNb[ver],
			     (*pTess).VerEdgeQty[ver]);
      (*pTess).VerEdgeNb[ver][(*pTess).VerEdgeQty[ver] - 1] = i;
    }

  // Domain
  if (Tess.PolyQty == 1)
    ut_string_string (Tess.DomType, &(*pTess).DomType);
  else
    ut_string_string ("poly", &(*pTess).DomType);

  ut_free_1d_int (VerNb_old_new);
  ut_free_1d_int (EdgeNb_old_new);
  ut_free_1d_int (FaceNb_old_new);
  ut_free_1d_int (PolyVerNb);
  ut_free_1d_int (PolyEdgeNb);

  return;
}

int
neut_tess_addver (struct TESS *pTess, double *coo)
{
  (*pTess).VerQty++;
  (*pTess).VerCoo
    = ut_realloc_2d_addline ((*pTess).VerCoo, (*pTess).VerQty + 1, 3);
  ut_array_1d_memcpy ((*pTess).VerCoo[(*pTess).VerQty], 3, coo);

  (*pTess).VerEdgeQty = ut_realloc_1d_int ((*pTess).VerEdgeQty,
					   (*pTess).VerQty + 1);
  (*pTess).VerEdgeQty[(*pTess).VerQty] = 0;

  (*pTess).VerEdgeNb = ut_realloc_1d_pint ((*pTess).VerEdgeNb,
					   (*pTess).VerQty + 1);
  (*pTess).VerEdgeNb[0] = NULL;
  (*pTess).VerEdgeNb[(*pTess).VerQty] = NULL;

  (*pTess).VerDom
    = ut_realloc_2d_int_addline ((*pTess).VerDom, (*pTess).VerQty + 1, 2);
  (*pTess).VerDom[0] = NULL;

  (*pTess).VerState = ut_realloc_1d_int ((*pTess).VerState,
					 (*pTess).VerQty + 1);
  (*pTess).VerState[(*pTess).VerQty] = 0;

  return (*pTess).VerQty;
}

int
neut_tess_addedge (struct TESS *pTess, int ver1, int ver2)
{
  (*pTess).EdgeQty++;
  (*pTess).EdgeVerNb = ut_realloc_2d_int_addline ((*pTess).EdgeVerNb,
						  (*pTess).EdgeQty + 1, 2);
  (*pTess).EdgeVerNb[0] = NULL;

  ut_array_1d_int_set_2 ((*pTess).EdgeVerNb[(*pTess).EdgeQty], ver1, ver2);

  (*pTess).EdgeFaceQty
    = ut_realloc_1d_int ((*pTess).EdgeFaceQty, (*pTess).EdgeQty + 1);
  (*pTess).EdgeFaceQty[(*pTess).EdgeQty] = 0;

  (*pTess).EdgeFaceNb
    = ut_realloc_1d_pint ((*pTess).EdgeFaceNb, (*pTess).EdgeQty + 1);
  (*pTess).EdgeFaceNb[(*pTess).EdgeQty] = NULL;

  (*pTess).EdgeLength
    = ut_realloc_1d ((*pTess).EdgeLength, (*pTess).EdgeQty + 1);
  (*pTess).EdgeLength[(*pTess).EdgeQty]
    = ut_space_dist ((*pTess).VerCoo[ver1], (*pTess).VerCoo[ver2]);

  (*pTess).EdgeState
    = ut_realloc_1d_int ((*pTess).EdgeState, (*pTess).EdgeQty + 1);
  (*pTess).EdgeState[(*pTess).EdgeQty] = 0;

  (*pTess).EdgeDel
    = ut_realloc_1d_int ((*pTess).EdgeDel, (*pTess).EdgeQty + 1);
  (*pTess).EdgeDel[(*pTess).EdgeQty] = 0;

  (*pTess).EdgeDom
    = ut_realloc_2d_int_addline ((*pTess).EdgeDom, (*pTess).EdgeQty + 1, 2);
  (*pTess).EdgeDom[0] = NULL;

  return (*pTess).EdgeQty;
}

int
neut_tess_addface_alloc (struct TESS *pTess)
{
  int face = ++(*pTess).FaceQty;

  (*pTess).FaceEq
    = ut_realloc_2d_addline ((*pTess).FaceEq, (*pTess).FaceQty + 1, 4);
  (*pTess).FaceEq[0] = NULL;

  (*pTess).FaceVerQty = ut_realloc_1d_int ((*pTess).FaceVerQty,
					   (*pTess).FaceQty + 1);
  (*pTess).FaceVerQty[face] = 0;

  (*pTess).FaceEdgeNb
    = ut_realloc_1d_pint ((*pTess).FaceEdgeNb, (*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb[0] = NULL;
  (*pTess).FaceEdgeNb[face] = NULL;

  (*pTess).FaceEdgeOri
    = ut_realloc_1d_pint ((*pTess).FaceEdgeOri, (*pTess).FaceQty + 1);
  (*pTess).FaceEdgeOri[0] = NULL;
  (*pTess).FaceEdgeOri[face] = NULL;

  (*pTess).FaceVerNb
    = ut_realloc_1d_pint ((*pTess).FaceVerNb, (*pTess).FaceQty + 1);
  (*pTess).FaceVerNb[0] = NULL;
  (*pTess).FaceVerNb[face] = NULL;

  (*pTess).FaceState
    = ut_realloc_1d_int ((*pTess).FaceState, (*pTess).FaceQty + 1);
  (*pTess).FaceState[face] = 0;

  (*pTess).FacePt = ut_realloc_1d_int ((*pTess).FacePt, (*pTess).FaceQty + 1);
  (*pTess).FacePt[face] = 0;

  (*pTess).FacePtCoo
    = ut_realloc_2d_addline ((*pTess).FacePtCoo, (*pTess).FaceQty + 1, 3);
  (*pTess).FacePtCoo[0] = NULL;

  (*pTess).FaceDom
    = ut_realloc_2d_int_addline ((*pTess).FaceDom, (*pTess).FaceQty + 1, 2);
  (*pTess).FaceDom[0] = NULL;

  (*pTess).FacePoly
    = ut_realloc_2d_int_addline ((*pTess).FacePoly, (*pTess).FaceQty + 1, 2);
  (*pTess).FacePoly[0] = NULL;

  return face;
}

int
neut_tess_addpoly_alloc (struct TESS *pTess)
{
  int poly = ++(*pTess).PolyQty;

  (*pTess).PolyFaceQty = ut_realloc_1d_int ((*pTess).PolyFaceQty,
					    (*pTess).PolyQty + 1);
  (*pTess).PolyFaceQty[poly] = 0;

  (*pTess).PolyFaceNb
    = ut_realloc_1d_pint ((*pTess).PolyFaceNb, (*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb[0] = NULL;
  (*pTess).PolyFaceNb[poly] = NULL;

  (*pTess).PolyFaceOri
    = ut_realloc_1d_pint ((*pTess).PolyFaceOri, (*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri[0] = NULL;
  (*pTess).PolyFaceOri[poly] = NULL;

  return poly;
}

int
neut_tess_addcell_alloc (struct TESS *pTess)
{
  int cell = ++(*pTess).CellQty;

  (*pTess).CellId = ut_realloc_1d_int ((*pTess).CellId, (*pTess).CellQty + 1);
  (*pTess).CellId[0] = 0;
  (*pTess).CellId[cell] = cell;

  (*pTess).CellOri =
    ut_realloc_1d_pdouble ((*pTess).CellOri, (*pTess).CellQty + 1);
  (*pTess).CellOri[0] = NULL;
  (*pTess).CellOri[cell] = ut_alloc_1d (4);

  (*pTess).CellTrue =
    ut_realloc_1d_int ((*pTess).CellTrue, (*pTess).CellQty + 1);
  (*pTess).CellTrue[0] = 0;
  (*pTess).CellTrue[cell] = 0;

  (*pTess).CellBody =
    ut_realloc_1d_int ((*pTess).CellBody, (*pTess).CellQty + 1);
  (*pTess).CellBody[0] = 0;
  (*pTess).CellBody[cell] = 0;

  (*pTess).CellLamId =
    ut_realloc_1d_int ((*pTess).CellLamId, (*pTess).CellQty + 1);
  (*pTess).CellLamId[0] = 0;
  (*pTess).CellLamId[cell] = 0;

  (*pTess).CellModeId =
    ut_realloc_1d_int ((*pTess).CellModeId, (*pTess).CellQty + 1);
  (*pTess).CellModeId[0] = 0;
  (*pTess).CellModeId[cell] = 0;

  (*pTess).SeedQty = (*pTess).CellQty;

  (*pTess).SeedCoo =
    ut_realloc_1d_pdouble ((*pTess).SeedCoo, (*pTess).CellQty + 1);
  (*pTess).SeedCoo[0] = NULL;
  (*pTess).SeedCoo[cell] = ut_alloc_1d (3);

  (*pTess).SeedWeight =
    ut_realloc_1d ((*pTess).SeedWeight, (*pTess).CellQty + 1);
  (*pTess).SeedWeight[0] = 0;
  (*pTess).SeedWeight[cell] = 0;

  return cell;
}

void
neut_tess_face_addedge (struct TESS *pTess, int face, int edge, int ori)
{
  int pos, edgeqty, lastver, lastedge, nextver;

  if (face < 0 || edge < 0 || (ori < -1 || ori > 1))
    ut_error_reportbug ();

  if (ori == 0)
  {
    // adding 1st edge, setting ori to 1
    if ((*pTess).FaceVerQty[face] == 0)
      ori = 1;

    else
    {
      lastver = (*pTess).FaceVerNb[face][(*pTess).FaceVerQty[face]];
      lastedge = (*pTess).FaceEdgeNb[face][(*pTess).FaceVerQty[face]];

      // adding 2nd edge, adapting ori of 1st edge
      if ((*pTess).FaceVerQty[face] == 1)
      {
	// if the last ver belongs to the new edge, changing it and
	// reversing last edge; otherwise, doing nothing
	if (ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[edge], 2, lastver) != -1)
	{
	  (*pTess).FaceVerNb[face][1] =
	    ((*pTess).EdgeVerNb[lastedge][0] != lastver) ?
	    (*pTess).EdgeVerNb[lastedge][0] : (*pTess).EdgeVerNb[lastedge][1];

	  (*pTess).FaceEdgeOri[face][1] *= -1;
	}
      }

      pos = ((*pTess).FaceEdgeOri[face][(*pTess).FaceVerQty[face]] == 1);
      nextver = (*pTess).EdgeVerNb[lastedge][pos];

      // determining ori
      if ((*pTess).EdgeVerNb[edge][0] == nextver)
	ori = 1;
      else if ((*pTess).EdgeVerNb[edge][1] == nextver)
	ori = -1;
      else
	ut_error_reportbug ();
    }
  }

  // adding edge
  edgeqty = ++(*pTess).FaceVerQty[face];
  (*pTess).FaceEdgeNb[face] = ut_realloc_1d_int ((*pTess).FaceEdgeNb[face],
						 edgeqty + 1);
  (*pTess).FaceEdgeOri[face] = ut_realloc_1d_int ((*pTess).FaceEdgeOri[face],
						  edgeqty + 1);

  (*pTess).FaceEdgeNb[face][edgeqty] = edge;
  (*pTess).FaceEdgeOri[face][edgeqty] = ori;

  (*pTess).FaceVerNb[face] = ut_realloc_1d_int ((*pTess).FaceVerNb[face],
						edgeqty + 1);

  pos = (ori == 1) ? 0 : 1;
  (*pTess).FaceVerNb[face][edgeqty] = (*pTess).EdgeVerNb[edge][pos];

  if (edgeqty > 1)
    if ((*pTess).FaceVerNb[face][edgeqty] ==
	(*pTess).FaceVerNb[face][edgeqty - 1])
    {
      printf ("trying to add ver %d to face %d, but ver just recorded\n",
	      (*pTess).FaceVerNb[face][edgeqty], face);
      ut_error_reportbug ();
    }

  return;
}

void
neut_tess_face_addedges (struct TESS *pTess, int face, int *edges,
			 int edgeqty)
{
  int i;

  for (i = 0; i < edgeqty; i++)
    neut_tess_face_addedge (pTess, face, edges[i], 0);

  return;
}

void
neut_tess_face_setedges (struct TESS *pTess, int face, int *edges,
			 int edgeqty, int firstedgeori)
{
  int i, qty, *tmp = NULL;
  int lastedge, lastedgeori, lastver, testedge, found;

  qty = edgeqty;
  tmp = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (tmp, qty, edges);

  (*pTess).FaceVerQty[face] = 0;
  neut_tess_face_addedge (pTess, face, edges[0], firstedgeori);

  ut_array_1d_int_list_rmelt (&tmp, &qty, edges[0]);

  while (qty > 0)
  {
    lastedge    = (*pTess).FaceEdgeNb[face][(*pTess).FaceVerQty[face]];
    lastedgeori = (*pTess).FaceEdgeOri[face][(*pTess).FaceVerQty[face]];
    lastver     = (*pTess).EdgeVerNb[lastedge][(lastedgeori == 1) ? 1 : 0];

    found = 0;
    for (i = 0; i < (*pTess).VerEdgeQty[lastver]; i++)
    {
      testedge = (*pTess).VerEdgeNb[lastver][i];
      if (ut_array_1d_int_eltpos (tmp, qty, testedge) != -1)
      {
	neut_tess_face_addedge (pTess, face, testedge, 0);
	qty
	  -= ut_array_1d_int_deletencompress (tmp, qty, testedge, 1);
	found = 1;
	break;
      }
    }

    if (!found)
    {
      FILE* fp = ut_file_open ("tess.debug", "w");
      neut_debug_tess (fp, *pTess);
      printf ("face = %d\n", face);
      printf ("edges = ");
      ut_array_1d_int_fprintf (stdout, edges, edgeqty, "%d");
      ut_file_close (fp, "tess.debug", "w");
      ut_error_reportbug ();
    }
  }

  return;
}

void
neut_tess_cell_addcentre (struct TESS *pTess, int cell, double *centre,
			  double weight)
{
  (*pTess).SeedQty = cell;
  (*pTess).SeedCoo[cell] = ut_realloc_1d ((*pTess).SeedCoo[cell], 3);
  ut_array_1d_memcpy ((*pTess).SeedCoo[cell], 3, centre);

  (*pTess).SeedWeight = ut_realloc_1d ((*pTess).SeedWeight,
				       (*pTess).CellQty + 1);
  (*pTess).SeedWeight[cell] = weight;

  return;
}

void
neut_tess_poly_addface (struct TESS *pTess, int poly, int face, int ori)
{
  int i, status, faceqty;

  if (poly < 0 || face < 0 || (ori != 1 && ori != -1))
  {
    printf ("poly = %d < 0 || face = %d < 0 || ori = %d != 1 and -1\n",
	    poly, face, ori);
    ut_error_reportbug ();
  }

  faceqty = ++(*pTess).PolyFaceQty[poly];
  (*pTess).PolyFaceNb[poly] = ut_realloc_1d_int ((*pTess).PolyFaceNb[poly],
						 faceqty + 1);
  (*pTess).PolyFaceOri[poly] = ut_realloc_1d_int ((*pTess).PolyFaceOri[poly],
						  faceqty + 1);

  (*pTess).PolyFaceNb[poly][faceqty] = face;
  (*pTess).PolyFaceOri[poly][faceqty] = ori;

  status = -1;
  for (i = 0; i < 2; i++)
    if ((*pTess).FacePoly[face][i] == 0)
    {
      (*pTess).FacePoly[face][i] = poly;
      status = 0;
      break;
    }
  if (status == -1)
    abort ();

  return;
}

void
neut_tess_poly_rmface (struct TESS *pTess, int poly, int face)
{
  int pos;

  if (poly < 0 || face < 0)
  {
    printf ("poly = %d < 0 || face = %d < 0\n", poly, face);
    ut_error_reportbug ();
  }

  pos = ut_array_1d_int_eltpos ((*pTess).PolyFaceNb[poly] + 1,
				(*pTess).PolyFaceQty[poly], face);

  if (pos == -1)
    ut_error_reportbug ();

  pos++;

  ut_array_1d_int_memcpy ((*pTess).PolyFaceNb[poly] + pos,
                          (*pTess).PolyFaceQty[poly] - pos,
			  (*pTess).PolyFaceNb[poly] + pos + 1);
  ut_array_1d_int_memcpy ((*pTess).PolyFaceOri[poly] + pos,
                          (*pTess).PolyFaceQty[poly] - pos,
			  (*pTess).PolyFaceOri[poly] + pos + 1);

  (*pTess).PolyFaceQty[poly]--;

  (*pTess).PolyFaceNb[poly] = ut_realloc_1d_int ((*pTess).PolyFaceNb[poly],
						 (*pTess).PolyFaceQty[poly] + 1);
  (*pTess).PolyFaceOri[poly] = ut_realloc_1d_int ((*pTess).PolyFaceOri[poly],
						  (*pTess).PolyFaceQty[poly] + 1);

  ut_array_1d_int_findnreplace ((*pTess).FacePoly[face], 2, poly, 0);

  return;
}

void
neut_tess_edge_rmface (struct TESS *pTess, int edge, int face)
{
  if (edge < 0 || face < 0)
  {
    printf ("edge = %d < 0 || face = %d < 0\n", edge, face);
    ut_error_reportbug ();
  }

  int qty = (*pTess).EdgeFaceQty[edge];
  ut_array_1d_int_list_rmelt (&(*pTess).EdgeFaceNb[edge],
                              &(*pTess).EdgeFaceQty[edge],
                              face);

  if ((*pTess).EdgeFaceQty[edge] + 1 != qty)
    abort ();

  return;
}

void
neut_tess_domface_addface (struct TESS *pTess, int domface, int face)
{
  if (domface <= 0 || face <= 0)
  {
    printf ("domface = %d < 0 || face = %d < 0\n", domface, face);
    ut_error_reportbug ();
  }

  neut_tess_domface_rmface (pTess, domface, face);

  (*pTess).DomTessFaceQty[domface]++;

  (*pTess).DomTessFaceNb[domface]
    = ut_realloc_1d_int ((*pTess).DomTessFaceNb[domface],
                         (*pTess).DomTessFaceQty[domface] + 1);

  (*pTess).DomTessFaceNb[domface][(*pTess).DomTessFaceQty[domface]] = face;

  return;
}

void
neut_tess_domface_rmface (struct TESS *pTess, int domface, int face)
{
  int pos;

  if (domface <= 0 || face <= 0)
  {
    printf ("domface = %d < 0 || face = %d < 0\n", domface, face);
    ut_error_reportbug ();
  }

  pos = ut_array_1d_int_eltpos ((*pTess).DomTessFaceNb[domface] + 1,
                                (*pTess).DomTessFaceQty[domface], face);

  if (pos != -1)
  {
    ut_array_1d_int_deletencompress ((*pTess).DomTessFaceNb[domface] + 1,
                                     (*pTess).DomTessFaceQty[domface],
                                     face, 1);

    (*pTess).DomTessFaceQty[domface]--;

    (*pTess).DomTessFaceNb[domface]
      = ut_realloc_1d_int ((*pTess).DomTessFaceNb[domface],
                           (*pTess).DomTessFaceQty[domface] + 1);
  }

  return;
}

void
neut_tess_domedge_rmedge (struct TESS *pTess, int domedge, int edge)
{
  int pos;

  if (domedge < 0 || edge < 0)
  {
    printf ("domedge = %d < 0 || edge = %d < 0\n", domedge, edge);
    ut_error_reportbug ();
  }

  pos = ut_array_1d_int_eltpos ((*pTess).DomTessEdgeNb[edge] + 1,
                                (*pTess).DomTessEdgeQty[edge], edge);

  if (pos != -1)
  {
    ut_array_1d_int_deletencompress ((*pTess).DomTessEdgeNb[edge] + 1,
                                     (*pTess).DomTessEdgeQty[edge],
                                     edge, 1);

    (*pTess).DomTessEdgeQty[edge]--;

    (*pTess).DomTessEdgeNb[edge]
      = ut_realloc_1d_int ((*pTess).DomTessEdgeNb[edge],
                           (*pTess).DomTessEdgeQty[edge] + 1);
  }

  return;
}

void
neut_tess_domedge_addedge (struct TESS *pTess, int domedge, int edge)
{
  int pos;

  if (domedge < 0 || edge < 0)
  {
    printf ("domedge = %d < 0 || edge = %d < 0\n", domedge, edge);
    ut_error_reportbug ();
  }

  pos = ut_array_1d_int_eltpos ((*pTess).DomTessEdgeNb[edge] + 1,
                                (*pTess).DomTessEdgeQty[edge], edge);

  if (pos == -1)
  {
    (*pTess).DomTessEdgeQty[edge]++;

    (*pTess).DomTessEdgeNb[edge]
      = ut_realloc_1d_int ((*pTess).DomTessEdgeNb[edge],
                           (*pTess).DomTessEdgeQty[edge] + 1);

    (*pTess).DomTessEdgeNb[edge][(*pTess).DomTessEdgeQty[edge]] = edge;
  }

  return;
}

void
neut_tess_ver_rmedge (struct TESS *pTess, int ver, int edge)
{
  if (ver < 0 || edge < 0)
  {
    printf ("ver = %d < 0 || edge = %d < 0\n", ver, edge);
    ut_error_reportbug ();
  }

  int qty = (*pTess).VerEdgeQty[ver];
  ut_array_1d_int_list_rmelt (&(*pTess).VerEdgeNb[ver],
                              &(*pTess).VerEdgeQty[ver],
                              edge);

  if ((*pTess).VerEdgeQty[ver] + 1 != qty)
    abort ();

  return;
}

void
neut_tess_face_changepoly (struct TESS *pTess, int face, int old,
			   int new)
{
  int ori;

  if (old == new)
    return;

  neut_tess_poly_face_ori (*pTess, old, face, &ori);
  neut_tess_poly_rmface (pTess, old, face);
  neut_tess_poly_addface (pTess, new, face, ori);
  ut_array_1d_int_findnreplace ((*pTess).FacePoly[face], 2, old, new);

  return;
}

void
neut_tess_face_addeq (struct TESS *pTess, int face, double *eq)
{
  (*pTess).FaceEq[face] = ut_realloc_1d ((*pTess).FaceEq[face], 4);
  ut_array_1d_memcpy ((*pTess).FaceEq[face], 4, eq);

  return;
}

int
neut_tess_adddomver (struct TESS *pTess, double *coo)
{
  (*pTess).DomVerQty++;
  (*pTess).DomVerCoo
    = ut_realloc_2d_addline ((*pTess).DomVerCoo, (*pTess).DomVerQty + 1, 3);
  ut_array_1d_memcpy ((*pTess).DomVerCoo[(*pTess).DomVerQty], 3, coo);

  (*pTess).DomVerEdgeQty = ut_realloc_1d_int ((*pTess).DomVerEdgeQty,
					      (*pTess).DomVerQty + 1);
  (*pTess).DomVerEdgeQty[(*pTess).DomVerQty] = 0;

  (*pTess).DomVerEdgeNb = ut_realloc_1d_pint ((*pTess).DomVerEdgeNb,
					      (*pTess).DomVerQty + 1);
  (*pTess).DomVerEdgeNb[0] = NULL;
  (*pTess).DomVerEdgeNb[(*pTess).DomVerQty] = NULL;

  (*pTess).DomVerLabel = ut_realloc_1d_pchar ((*pTess).DomVerLabel,
					      (*pTess).DomVerQty + 1);
  (*pTess).DomVerLabel[(*pTess).DomVerQty] = NULL;

  (*pTess).DomTessVerNb = ut_realloc_1d_int ((*pTess).DomTessVerNb,
					     (*pTess).DomVerQty + 1);
  (*pTess).DomTessVerNb[(*pTess).DomVerQty] = 0;

  return (*pTess).DomVerQty;
}

int
neut_tess_adddomedge (struct TESS *pTess, int ver1, int ver2)
{
  (*pTess).DomEdgeQty++;

  (*pTess).DomEdgeVerQty = ut_realloc_1d_int ((*pTess).DomEdgeVerQty,
                                              (*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerQty[(*pTess).DomEdgeQty] = 2;

  (*pTess).DomEdgeVerNb = ut_realloc_2d_int_addline ((*pTess).DomEdgeVerNb,
						     (*pTess).DomEdgeQty + 1,
						     2);
  (*pTess).DomEdgeVerNb[0] = NULL;

  ut_array_1d_int_set_2 ((*pTess).DomEdgeVerNb[(*pTess).DomEdgeQty], ver1,
			 ver2);

  (*pTess).DomEdgeFaceNb
    = ut_realloc_1d_pint ((*pTess).DomEdgeFaceNb, (*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeFaceNb[(*pTess).DomEdgeQty] = ut_alloc_1d_int (2);

  (*pTess).DomEdgeLabel = ut_realloc_1d_pchar ((*pTess).DomEdgeLabel,
					       (*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeLabel[(*pTess).DomEdgeQty] = NULL;

  (*pTess).DomTessEdgeQty = ut_realloc_1d_int ((*pTess).DomTessEdgeQty,
					       (*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeQty[(*pTess).DomEdgeQty] = 0;

  (*pTess).DomTessEdgeNb = ut_realloc_1d_pint ((*pTess).DomTessEdgeNb,
					       (*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeNb[0] = NULL;
  (*pTess).DomTessEdgeNb[(*pTess).DomEdgeQty] = NULL;

  return (*pTess).DomEdgeQty;
}

int
neut_tess_adddomface_alloc (struct TESS *pTess)
{
  (*pTess).DomFaceQty++;

  (*pTess).DomFaceLabel = ut_realloc_1d_pchar ((*pTess).DomFaceLabel,
					       (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceLabel[(*pTess).DomFaceQty] = NULL;

  (*pTess).DomFaceType = ut_realloc_1d_pchar ((*pTess).DomFaceType,
					      (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceType[0] = NULL;
  (*pTess).DomFaceType[(*pTess).DomFaceQty] = NULL;

  (*pTess).DomFaceEq = ut_realloc_2d_addline ((*pTess).DomFaceEq,
					      (*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceEq[0] = NULL;

  (*pTess).DomFaceParmQty = ut_realloc_1d_int ((*pTess).DomFaceParmQty,
					       (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParmQty[(*pTess).DomFaceQty] = 0;

  (*pTess).DomFaceParms = ut_realloc_2d_addline ((*pTess).DomFaceParms,
					         (*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceParms[0] = NULL;
  (*pTess).DomFaceParms[(*pTess).DomFaceQty] = NULL;

  (*pTess).DomFaceVerQty = ut_realloc_1d_int ((*pTess).DomFaceVerQty,
					      (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerQty[(*pTess).DomFaceQty] = 0;

  (*pTess).DomFaceVerNb
    = ut_realloc_1d_pint ((*pTess).DomFaceVerNb, (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb[(*pTess).DomFaceQty] = NULL;

  (*pTess).DomFaceEdgeQty = ut_realloc_1d_int ((*pTess).DomFaceEdgeQty,
					      (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeQty[(*pTess).DomFaceQty] = 0;

  (*pTess).DomFaceEdgeNb
    = ut_realloc_1d_pint ((*pTess).DomFaceEdgeNb, (*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeNb[(*pTess).DomFaceQty] = NULL;

  (*pTess).DomTessFaceQty = ut_realloc_1d_int ((*pTess).DomTessFaceQty,
					       (*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceQty[(*pTess).DomFaceQty] = 0;

  (*pTess).DomTessFaceNb = ut_realloc_1d_pint ((*pTess).DomTessFaceNb,
					       (*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceNb[0] = NULL;
  (*pTess).DomTessFaceNb[(*pTess).DomFaceQty] = NULL;

  return (*pTess).DomFaceQty;
}

void
neut_tess_init_veredge (struct TESS *pTess)
{
  int i, j, ver;

  (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    for (j = 0; j < 2; j++)
    {
      ver = (*pTess).EdgeVerNb[i][j];
      (*pTess).VerEdgeQty[ver]++;
      (*pTess).VerEdgeNb[ver]
	= ut_realloc_1d_int ((*pTess).VerEdgeNb[ver],
			     (*pTess).VerEdgeQty[ver]);
      (*pTess).VerEdgeNb[ver][(*pTess).VerEdgeQty[ver] - 1] = i;
    }

  return;
}

void
neut_tess_init_edgeface (struct TESS *pTess)
{
  int i, j, edge;

  (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
    {
      edge = (*pTess).FaceEdgeNb[i][j];
      (*pTess).EdgeFaceQty[edge]++;
      (*pTess).EdgeFaceNb[edge]
	= ut_realloc_1d_int ((*pTess).EdgeFaceNb[edge],
			     (*pTess).EdgeFaceQty[edge]);
      (*pTess).EdgeFaceNb[edge][(*pTess).EdgeFaceQty[edge] - 1] = i;
    }

  return;
}

void
neut_tess_init_facepoly (struct TESS *pTess)
{
  int i, j, face;
  int *FacePolyQty = NULL;

  FacePolyQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
    {
      face = (*pTess).PolyFaceNb[i][j];
      FacePolyQty[face]++;
      (*pTess).FacePoly[face][FacePolyQty[face] - 1] = i;
    }

  ut_free_1d_int (FacePolyQty);

  return;
}

void
neut_tess_face_fixorifromedges (struct TESS *pTess, int face)
{
  double *n = ut_alloc_1d (3);

  neut_tess_face_normal_fromver (*pTess, face, n);

  if (ut_vector_scalprod ((*pTess).FaceEq[face] + 1, n) < 0)
    ut_array_1d_scale ((*pTess).FaceEq[face], 4, -1);

  ut_free_1d (n);

  return;
}

void
neut_tess_domface_tess (struct TESS Tess, int domface, struct TESS *pT)
{
  int *oldver_newver = ut_alloc_1d_int (Tess.VerQty + 1);
  int *oldedge_newedge = ut_alloc_1d_int (Tess.EdgeQty + 1);
  int *oldface_newface = ut_alloc_1d_int (Tess.FaceQty + 1);

  neut_tess_free (pT);
  neut_tess_set_zero (pT);

  neut_tess_domface_tess_general (Tess, pT);

  neut_tess_domface_tess_seeds (Tess, pT);

  neut_tess_domface_tess_faces (Tess, domface, pT, oldface_newface);

  neut_tess_domface_tess_cells (Tess, oldface_newface, pT);

  neut_tess_domface_tess_edges (Tess, domface, pT, oldedge_newedge);

  neut_tess_domface_tess_vers (Tess, domface, pT, oldver_newver);

  neut_tess_domface_tess_renumber (pT, oldface_newface,
				   oldedge_newedge, oldver_newver);

  neut_tess_domface_tess_domain (Tess, domface, pT,
				 oldedge_newedge, oldver_newver);

  ut_free_1d_int (oldver_newver);
  ut_free_1d_int (oldedge_newedge);
  ut_free_1d_int (oldface_newface);

  return;
}

void
neut_tess_3dcolumnar_2d (struct TESS Tess, struct TESS *pT)
{
  int domface;
  int *oldver_newver = ut_alloc_1d_int (Tess.VerQty + 1);
  int *oldedge_newedge = ut_alloc_1d_int (Tess.EdgeQty + 1);

  neut_tess_domface_label_id (Tess, "z0", &domface);

  neut_tess_free (pT);
  neut_tess_set_zero (pT);

  neut_tess_domface_tess_general (Tess, pT);

  neut_tess_domface_tess_seeds (Tess, pT);

  neut_tess_3dcolumnar_2d_faces (Tess, domface, pT);

  neut_tess_domface_tess_cells (Tess, NULL, pT);

  neut_tess_3dcolumnar_2d_edges (Tess, domface, pT, oldedge_newedge);

  neut_tess_3dcolumnar_2d_vers (Tess, domface, oldedge_newedge, pT,
				oldver_newver);

  neut_tess_3dcolumnar_2d_renumber (pT, oldedge_newedge, oldver_newver);

  neut_tess_init_edgeslave (pT);
  neut_tess_init_verslave (pT);

  neut_tess_domface_tess_domain (Tess, domface, pT,
				 oldedge_newedge, oldver_newver);

  ut_free_1d_int (oldver_newver);
  ut_free_1d_int (oldedge_newedge);

  return;
}

void
neut_tess_init_domfaceeq (struct TESS *pTess)
{
  int i, j, face, poly, ori;
  double *eq = ut_alloc_1d (4);

  if ((*pTess).DomFaceEq == NULL)
    (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    ut_array_1d_zero ((*pTess).DomFaceEq[i], 4);

    for (j = 1; j <= (*pTess).DomTessFaceQty[i]; j++)
    {
      face = (*pTess).DomTessFaceNb[i][j];

      poly = (*pTess).FacePoly[(*pTess).DomTessFaceNb[i][j]][0];
      neut_tess_poly_face_ori (*pTess, poly, face, &ori);
      if (ori == 0)
	ut_error_reportbug ();
      ut_array_1d_memcpy (eq, 4, (*pTess).FaceEq[face]);
      ut_array_1d_scale (eq, 4, ori);
      ut_array_1d_add ((*pTess).DomFaceEq[i], eq, 4, (*pTess).DomFaceEq[i]);
    }
    ut_array_1d_scale ((*pTess).DomFaceEq[i], 4,
		       1 / ut_array_1d_norm ((*pTess).DomFaceEq[i] + 1, 3));
  }

  ut_free_1d (eq);

  return;
}

void
neut_tess_init_seedslave (struct TESS *pTess)
{
  int i, x, y, z, id, master;

  (*pTess).PerSeedSlave = ut_alloc_2d_int ((*pTess).SeedQty + 1, 27);

  for (i = (*pTess).CellQty + 1; i <= (*pTess).SeedQty; i++)
  {
    master = (*pTess).PerSeedMaster[i];

    x = (*pTess).PerSeedShift[i][0];
    y = (*pTess).PerSeedShift[i][1];
    z = (*pTess).PerSeedShift[i][2];
    id = (z + 1) * 9 + (y + 1) * 3 + (x + 1);
    (*pTess).PerSeedSlave[master][id] = i;
  }

  return;
}

void
neut_tess_init_faceslave (struct TESS *pTess)
{
  int i, master, slave;

  (*pTess).PerFaceSlaveNb = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  for (i = 1; i <= (*pTess).PerFaceQty; i++)
  {
    slave = (*pTess).PerFaceNb[i];
    master = (*pTess).PerFaceMaster[slave];
    (*pTess).PerFaceSlaveNb[master] = slave;
  }

  return;
}

void
neut_tess_init_facepoly_per (struct TESS *pTess)
{
  int i, face, masterface;

  for (i = 1; i <= (*pTess).PerFaceQty; i++)
  {
    face = (*pTess).PerFaceNb[i];
    if ((*pTess).FacePoly[face][1] != 0)
      abort ();

    masterface = (*pTess).PerFaceMaster[face];

    (*pTess).FacePoly[face][1]
      = neut_tess_seed_master_slave (*pTess,
				     (*pTess).FacePoly[masterface][0],
				     (*pTess).PerFaceShift[face], 1);

    (*pTess).FacePoly[masterface][1]
      = neut_tess_seed_master_slave (*pTess,
				     (*pTess).FacePoly[face][0],
				     (*pTess).PerFaceShift[face], -1);
  }

  return;
}

void
neut_tess_init_edgefacenb_per (struct TESS *pTess)
{
  int i, edge, masteredge;

  // This function sets EdgeEdgeNb to the slave seed numbers, for a 2D
  // tessellation.  It does not make sense to apply it to a 3D
  // tessellation.
  if ((*pTess).Dim == 3)
    abort ();

  for (i = 1; i <= (*pTess).PerEdgeQty; i++)
  {
    edge = (*pTess).PerEdgeNb[i];
    masteredge = (*pTess).PerEdgeMaster[edge];

    if ((*pTess).EdgeFaceQty[edge] != 1)
      abort ();
    if ((*pTess).EdgeFaceQty[masteredge] != 1)
      abort ();

    (*pTess).EdgeFaceQty[edge] = 2;
    (*pTess).EdgeFaceNb[edge] = ut_realloc_1d_int ((*pTess).EdgeFaceNb[edge], 2);
    (*pTess).EdgeFaceNb[edge][1]
      = neut_tess_seed_master_slave (*pTess,
				     (*pTess).EdgeFaceNb[masteredge][0],
				     (*pTess).PerEdgeShift[edge], 1);

    (*pTess).EdgeFaceQty[masteredge] = 2;
    (*pTess).EdgeFaceNb[masteredge] = ut_realloc_1d_int ((*pTess).EdgeFaceNb[masteredge], 2);
    (*pTess).EdgeFaceNb[masteredge][1]
      = neut_tess_seed_master_slave (*pTess,
				     (*pTess).EdgeFaceNb[edge][0],
				     (*pTess).PerEdgeShift[edge], -1);
  }

  return;
}

void
neut_tess_init_edgeslave (struct TESS *pTess)
{
  int i, master, slave;

  (*pTess).PerEdgeSlaveQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeSlaveNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).PerEdgeQty; i++)
  {
    slave = (*pTess).PerEdgeNb[i];
    master = (*pTess).PerEdgeMaster[slave];

    (*pTess).PerEdgeSlaveQty[master]++;
    (*pTess).PerEdgeSlaveNb[master]
      = ut_realloc_1d_int ((*pTess).PerEdgeSlaveNb[master],
			   (*pTess).PerEdgeSlaveQty[master] + 1);
    (*pTess).PerEdgeSlaveNb[master][(*pTess).PerEdgeSlaveQty[master]] = slave;
  }

  return;
}

void
neut_tess_init_verslave (struct TESS *pTess)
{
  int i, master, slave;

  (*pTess).PerVerSlaveQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).PerVerSlaveNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);

  for (i = 1; i <= (*pTess).PerVerQty; i++)
  {
    slave = (*pTess).PerVerNb[i];
    master = (*pTess).PerVerMaster[slave];

    (*pTess).PerVerSlaveQty[master]++;
    (*pTess).PerVerSlaveNb[master]
      = ut_realloc_1d_int ((*pTess).PerVerSlaveNb[master],
			   (*pTess).PerVerSlaveQty[master] + 1);
    (*pTess).PerVerSlaveNb[master][(*pTess).PerVerSlaveQty[master]] = slave;
  }

  return;
}

void
neut_tess_init_seeds_fromcell (struct TESS *pTess)
{
  int i;

  (*pTess).SeedQty = (*pTess).CellQty;
  (*pTess).SeedCoo = ut_alloc_2d ((*pTess).SeedQty + 1, 3);
  for (i = 1; i <= (*pTess).SeedQty; i++)
    neut_tess_cell_centroid (*pTess, i, (*pTess).SeedCoo[i]);
  (*pTess).SeedWeight = ut_alloc_1d ((*pTess).SeedQty + 1);

  return;
}

void
neut_tess_poly_remove_nocompress (struct TESS *pTess, int poly)
{
  int i, j, face, polyqty;
  int faceqty, *faces = NULL;
  int edge, edgeqty, *edges = NULL;
  int ver, verqty, *vers = NULL;
  int delfaceqty = 0, *delfaces = NULL;
  int keepfaceqty = 0, *keepfaces = NULL;
  int deledgeqty = 0, *deledges = NULL;
  int delverqty = 0, *delvers = NULL;

  // recording poly faces, edges and vertices as well as those to delete
  // (orphans)

  neut_tess_poly_faces (*pTess, poly, &faces, &faceqty);
  neut_tess_poly_edges (*pTess, poly, &edges, &edgeqty);
  neut_tess_poly_vers (*pTess, poly, &vers, &verqty);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];
    neut_tess_face_polys (*pTess, face, NULL, &polyqty);
    if (polyqty == 1)
      ut_array_1d_int_list_addelt (&delfaces, &delfaceqty, face);
    else
      ut_array_1d_int_list_addelt (&keepfaces, &keepfaceqty, face);
  }

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];
    neut_tess_edge_polys (*pTess, edge, NULL, &polyqty);
    if (polyqty == 1)
      ut_array_1d_int_list_addelt (&deledges, &deledgeqty, edge);
  }

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];
    neut_tess_ver_polys (*pTess, ver, NULL, &polyqty);
    if (polyqty == 1)
      ut_array_1d_int_list_addelt (&delvers, &delverqty, ver);
  }

  // removing poly and orphan faces, edges and vertices

  (*pTess).PolyState[poly] = -1;

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    ut_array_1d_int_findnreplace ((*pTess).FacePoly[face], 2,
                                  poly, -((*pTess).DomFaceQty + 1));
    ut_array_1d_int_sort_des ((*pTess).FacePoly[face], 2);
  }

  for (i = 0; i < delfaceqty; i++)
  {
    face = delfaces[i];

    for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    {
      edge = (*pTess).FaceEdgeNb[face][j];

      ut_array_1d_int_list_rmelt (&(*pTess).EdgeFaceNb[edge],
                                  &(*pTess).EdgeFaceQty[edge],
                                  face);
    }

    (*pTess).FaceState[face] = -1;
  }

  for (i = 0; i < deledgeqty; i++)
  {
    edge = deledges[i];

    for (j = 0; j < 2; j++)
    {
      ver = (*pTess).EdgeVerNb[edge][j];

      ut_array_1d_int_list_rmelt (&(*pTess).VerEdgeNb[ver],
                                  &(*pTess).VerEdgeQty[ver],
                                  edge);
    }

    (*pTess).EdgeState[edge] = -1;
  }

  for (i = 0; i < delverqty; i++)
  {
    ver = delvers[i];

    (*pTess).VerState[ver] = -1;
  }

  /*
  for (i = 0; i < keepfaceqty; i++)
  {
    face = keepfaces[i];

    for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    {
      edge = (*pTess).FaceEdgeNb[face][j];

      ut_array_1d_int_list_rmelt (&(*pTess).EdgeFaceNb[edge],
                                  &(*pTess).EdgeFaceQty[edge],
                                  face);
    }

    (*pTess).FaceState[face] = -1;
  }
  */

  char label[100] = {};
  int cutqty = 0;
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if (strstr ((*pTess).DomFaceLabel[i], "cut"))
      cutqty++;
  cutqty++;
  sprintf (label, "cut%d", cutqty);

  neut_tess_adddomface_alloc (pTess);
  ut_string_string (label, (*pTess).DomFaceLabel + (*pTess).DomFaceQty);
  neut_tess_init_facedom_fromfacepoly (pTess);

  neut_tess_init_domtessface (pTess);
  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver (pTess);

  ut_free_1d_int (faces);
  ut_free_1d_int (edges);
  ut_free_1d_int (vers);
  ut_free_1d_int (delfaces);
  ut_free_1d_int (keepfaces);
  ut_free_1d_int (deledges);
  ut_free_1d_int (delvers);

  return;
}

void
neut_tess_poly_remove (struct TESS *pTess, int poly)
{
  neut_tess_poly_remove_nocompress (pTess, poly);

  neut_tess_compress (pTess);

  return;
}

void
neut_tess_polys_remove_nocompress (struct TESS *pTess, int* polys, int polyqty)
{
  int i;

  for (i = 0; i < polyqty; i++)
    neut_tess_poly_remove_nocompress (pTess, polys[i]);

  return;
}

void
neut_tess_polys_remove (struct TESS *pTess, int* polys, int polyqty)
{
  neut_tess_polys_remove_nocompress (pTess, polys, polyqty);

  neut_tess_compress (pTess);

  return;
}

int
neut_tess_cellexpr_remove (struct TESS *pTess, char *expr)
{
  int *cells = NULL, cellqty = 0;

  if ((*pTess).Dim == 2)
    abort ();

  neut_tess_expr_celllist (*pTess, expr, &cells, &cellqty);

  neut_tess_polys_remove (pTess, cells, cellqty);

  ut_free_1d_int (cells);

  return cellqty;
}

void
neut_tess_resetcellid (struct TESS *pTess)
{
  (*pTess).CellId = ut_realloc_1d_int ((*pTess).CellId, (*pTess).CellQty + 1);
  ut_array_1d_int_set_id ((*pTess).CellId, (*pTess).CellQty + 1);

  return;
}

void
neut_tess_domain_tess (struct TESS Tess, struct TESS *pDomTess)
{
  int i, j, ver, edge;

  // Vertices
  (*pDomTess).VerQty = Tess.DomVerQty;
  (*pDomTess).VerEdgeQty = ut_alloc_1d_int (Tess.DomVerQty + 1);
  (*pDomTess).VerEdgeNb = ut_alloc_1d_pint (Tess.DomVerQty + 1);
  (*pDomTess).VerCoo = ut_alloc_2d (Tess.DomVerQty + 1, 3);

  ut_array_1d_int_memcpy ((*pDomTess).VerEdgeQty + 1, (*pDomTess).VerQty,
			  Tess.DomVerEdgeQty + 1);

  ut_array_2d_memcpy ((*pDomTess).VerCoo + 1, (*pDomTess).VerQty, 3,
		      Tess.DomVerCoo + 1);

  for (i = 1; i <= (*pDomTess).VerQty; i++)
  {
    (*pDomTess).VerEdgeNb[i] = ut_alloc_1d_int ((*pDomTess).VerEdgeQty[i]);
    ut_array_1d_int_memcpy ((*pDomTess).VerEdgeNb[i],
			    (*pDomTess).VerEdgeQty[i], Tess.DomVerEdgeNb[i]);
  }

  // Edges
  (*pDomTess).EdgeQty = Tess.DomEdgeQty;
  (*pDomTess).EdgeVerNb = ut_alloc_2d_int (Tess.DomEdgeQty + 1, 2);
  (*pDomTess).EdgeFaceNb = ut_alloc_2d_int (Tess.DomEdgeQty + 1, 2);

  for (i = 1; i <= (*pDomTess).EdgeQty; i++)
  {
    if (Tess.DomEdgeVerQty[i] != 2)
      abort ();
    ut_array_1d_int_memcpy ((*pDomTess).EdgeVerNb[i], 2,
			    Tess.DomEdgeVerNb[i]);
  }

  for (i = 1; i <= (*pDomTess).EdgeQty; i++)
  {
    ut_array_1d_int_memcpy ((*pDomTess).EdgeFaceNb[i], 2,
			    Tess.DomEdgeFaceNb[i]);
    // if (Tess.DomEdgeFaceQty[i] != 2)
    // ut_error_reportbug ();
  }

  // Faces
  (*pDomTess).FaceQty = Tess.DomFaceQty;
  (*pDomTess).FaceEq = ut_alloc_2d ((*pDomTess).FaceQty + 1, 4);
  (*pDomTess).FaceVerQty = ut_alloc_1d_int ((*pDomTess).FaceQty + 1);
  (*pDomTess).FaceVerNb = ut_alloc_1d_pint ((*pDomTess).FaceQty + 1);
  (*pDomTess).FaceEdgeNb = ut_alloc_1d_pint ((*pDomTess).FaceQty + 1);
  (*pDomTess).FaceEdgeOri = ut_alloc_1d_pint ((*pDomTess).FaceQty + 1);

  for (i = 1; i <= (*pDomTess).FaceQty; i++)
    ut_array_1d_memcpy ((*pDomTess).FaceEq[i], 4, Tess.DomFaceEq[i]);

  ut_array_1d_int_memcpy ((*pDomTess).FaceVerQty, (*pDomTess).FaceQty + 1,
			  Tess.DomFaceVerQty);

  for (i = 1; i <= (*pDomTess).FaceQty; i++)
  {
    (*pDomTess).FaceVerNb[i] =
      ut_alloc_1d_int ((*pDomTess).FaceVerQty[i] + 1);
    (*pDomTess).FaceEdgeNb[i] =
      ut_alloc_1d_int ((*pDomTess).FaceVerQty[i] + 1);
    (*pDomTess).FaceEdgeOri[i] =
      ut_alloc_1d_int ((*pDomTess).FaceVerQty[i] + 1);

    ut_array_1d_int_memcpy ((*pDomTess).FaceVerNb[i] + 1,
			    (*pDomTess).FaceVerQty[i],
			    Tess.DomFaceVerNb[i] + 1);

    ut_array_1d_int_memcpy ((*pDomTess).FaceEdgeNb[i] + 1,
			    (*pDomTess).FaceVerQty[i],
			    Tess.DomFaceEdgeNb[i] + 1);

    for (j = 1; j <= (*pDomTess).FaceVerQty[i]; j++)
    {
      edge = (*pDomTess).FaceEdgeNb[i][j];
      ver = (*pDomTess).FaceVerNb[i][j];
      if ((*pDomTess).EdgeVerNb[edge][0] == ver)
        (*pDomTess).FaceEdgeOri[i][j] = 1;
      else if ((*pDomTess).EdgeVerNb[edge][1] == ver)
        (*pDomTess).FaceEdgeOri[i][j] = -1;
      else
        abort ();
    }
  }


  neut_tess_init_domain_label (pDomTess);

  return;
}

int
neut_tess_init_domfacez0 (struct TESS *pTess)
{
  int i, status;
  double *eq = ut_alloc_1d (4);
  ut_array_1d_set_4 (eq, 0, 0, 0, -1);

  status = -1;

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if (!strcmp ((*pTess).DomFaceLabel[i], "z0"))
    {
      status = 0;
      break;
    }

  if (status == -1)
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
      if (ut_array_1d_equal ((*pTess).DomFaceEq[i], eq, 4, 1e-6))
      {
        status = 0;
        ut_string_string ("z0", (*pTess).DomFaceLabel + i);
        break;
      }

  ut_free_1d (eq);

  return status;
}
