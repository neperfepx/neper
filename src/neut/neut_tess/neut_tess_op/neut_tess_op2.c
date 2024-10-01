/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_op_.h"

void
neut_tess_init_domain_memcpy (struct TESS *pTess, struct TESS DomTess)
{
  int i;

  neut_tess_reset_domain (pTess);

  ut_string_string (DomTess.DomType, &((*pTess).DomType));

  /*
     if ((*pTess).DomFaceVerQty)
     ut_free_1d_int (&(*pTess).DomFaceVerQty);
     if ((*pTess).DomFaceVerNb)
     ut_free_2d_int (&(*pTess).DomFaceVerNb, (*pTess).DomFaceQty + 1);
     if ((*pTess).DomFaceEdgeQty)
     ut_free_1d_int (&(*pTess).DomFaceEdgeQty);
     if ((*pTess).DomEdgeVerQty)
     ut_free_1d_int (&(*pTess).DomEdgeVerQty);
     if ((*pTess).DomEdgeVerNb)
     ut_free_2d_int (&(*pTess).DomEdgeVerNb, (*pTess).DomEdgeQty + 1);
     if ((*pTess).DomEdgeFaceNb)
     ut_free_2d_int (&(*pTess).DomEdgeFaceNb, (*pTess).DomEdgeQty + 1);
     if ((*pTess).DomVerEdgeQty)
     ut_free_1d_int (&(*pTess).DomVerEdgeQty);
     if ((*pTess).DomVerEdgeNb)
     ut_free_2d_int (&(*pTess).DomVerEdgeNb, (*pTess).DomVerQty + 1);
     if ((*pTess).DomVerCoo)
     ut_free_2d (&(*pTess).DomVerCoo, (*pTess).DomVerQty + 1);
   */

  // Faces

  (*pTess).DomFaceQty = DomTess.FaceQty;
  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceType = ut_alloc_1d_pchar ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParmQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceParms = ut_alloc_1d_pdouble ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    ut_string_string ("plane", (*pTess).DomFaceType + i);
    ut_array_1d_memcpy (DomTess.FaceEq[i], 4, (*pTess).DomFaceEq[i]);
    (*pTess).DomFaceParmQty[i] = 4;
    (*pTess).DomFaceParms[i] = ut_alloc_1d (4);
    ut_array_1d_memcpy ((*pTess).DomFaceEq[i], (*pTess).DomFaceParmQty[i],
                        (*pTess).DomFaceParms[i]);
  }

  ut_array_1d_int_memcpy (DomTess.FaceVerQty, (*pTess).DomFaceQty + 1,
                          (*pTess).DomFaceVerQty);

  ut_array_1d_int_memcpy (DomTess.FaceVerQty, (*pTess).DomFaceQty + 1,
                          (*pTess).DomFaceEdgeQty);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    (*pTess).DomFaceVerNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);
    (*pTess).DomFaceEdgeNb[i] =
      ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);

    ut_array_1d_int_memcpy (DomTess.FaceVerNb[i] + 1,
                            (*pTess).DomFaceVerQty[i],
                            (*pTess).DomFaceVerNb[i] + 1);

    ut_array_1d_int_memcpy (DomTess.FaceEdgeNb[i] + 1,
                            (*pTess).DomFaceEdgeQty[i],
                            (*pTess).DomFaceEdgeNb[i] + 1);
  }

  // Edges
  (*pTess).DomEdgeQty = DomTess.EdgeQty;
  (*pTess).DomEdgeVerQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);
  (*pTess).DomEdgeFaceNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);
  (*pTess).DomEdgeType = ut_alloc_1d_pchar ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeParmQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeParms = ut_alloc_1d_pdouble ((*pTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    (*pTess).DomEdgeVerQty[i] = 2;
    ut_array_1d_int_memcpy (DomTess.EdgeVerNb[i], (*pTess).DomEdgeVerQty[i],
                            (*pTess).DomEdgeVerNb[i]);
  }

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    ut_array_1d_int_memcpy (DomTess.EdgeFaceNb[i], 2,
                            (*pTess).DomEdgeFaceNb[i]);
    if (DomTess.EdgeFaceQty[i] != 2)
      ut_print_neperbug ();
  }

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    ut_string_string ("line", (*pTess).DomEdgeType + i);
    (*pTess).DomEdgeParmQty[i] = 0;
    (*pTess).DomEdgeParms[i] = NULL;
  }

  // Vertices
  (*pTess).DomVerQty = DomTess.VerQty;
  (*pTess).DomVerEdgeQty = ut_alloc_1d_int (DomTess.VerQty + 1);
  (*pTess).DomVerEdgeNb = ut_alloc_1d_pint (DomTess.VerQty + 1);
  (*pTess).DomVerCoo = ut_alloc_2d (DomTess.VerQty + 1, 3);

  ut_array_1d_int_memcpy (DomTess.VerEdgeQty + 1, (*pTess).DomVerQty,
                          (*pTess).DomVerEdgeQty + 1);

  ut_array_2d_memcpy (DomTess.VerCoo + 1, (*pTess).DomVerQty, 3,
                      (*pTess).DomVerCoo + 1);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    (*pTess).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pTess).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy (DomTess.VerEdgeNb[i], (*pTess).DomVerEdgeQty[i],
                            (*pTess).DomVerEdgeNb[i]);
  }

  neut_tess_init_domain_label (pTess);

  return;
}

void
neut_tess_init_domain_label (struct TESS *pTess)
{
  if ((*pTess).Dim == 3)
    neut_tess_init_domain_label_3d (pTess);

  else if ((*pTess).Dim == 2)
    neut_tess_init_domain_label_2d (pTess);

  return;
}

void
neut_tess_init_domain_domedgevernb (struct TESS *pTess)
{
  int i, j, domedge;

  // Init DomEdgeVerNb
  (*pTess).DomEdgeVerQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomEdgeVerNb = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
    for (j = 0; j < (*pTess).DomVerEdgeQty[i]; j++)
    {
      domedge = (*pTess).DomVerEdgeNb[i][j];
      if (domedge == 0)
        abort ();

      if (ut_array_1d_int_eltpos
          ((*pTess).DomEdgeVerNb[domedge], (*pTess).DomEdgeVerQty[domedge],
           i) == -1)
      {
        (*pTess).DomEdgeVerQty[domedge]++;
        if ((*pTess).DomEdgeVerQty[domedge] > 2)
          abort ();
        (*pTess).DomEdgeVerNb[domedge][(*pTess).DomEdgeVerQty[domedge] - 1] =
          i;
      }
    }

  if (strcmp ((*pTess).DomType, "cut"))
  {
    int x, y;
    if (ut_array_2d_int_eltpos
        ((*pTess).DomEdgeVerNb + 1, (*pTess).DomEdgeQty, 2, 0, &x, &y) != -1)
      ut_print_neperbug ();
  }

  return;
}

void
neut_tess_init_domain_domveredge (struct TESS *pTess)
{
  int i, j, ver;

  (*pTess).DomVerEdgeQty = ut_alloc_1d_int ((*pTess).DomVerQty + 1);
  (*pTess).DomVerEdgeNb = ut_alloc_1d_pint ((*pTess).DomVerQty + 1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = 0; j < (*pTess).DomEdgeVerQty[i]; j++)
    {
      ver = (*pTess).DomEdgeVerNb[i][j];
      (*pTess).DomVerEdgeQty[ver]++;
      (*pTess).DomVerEdgeNb[ver] =
        ut_realloc_1d_int ((*pTess).DomVerEdgeNb[ver],
                           (*pTess).DomVerEdgeQty[ver]);
      (*pTess).DomVerEdgeNb[ver][(*pTess).DomVerEdgeQty[ver] - 1] = i;
    }

  return;
}

void
neut_tess_init_domain_domedgeface (struct TESS *pTess)
{
  int i, j, edge;

  int *DomEdgeFaceQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  (*pTess).DomEdgeFaceNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    for (j = 1; j <= (*pTess).DomFaceEdgeQty[i]; j++)
    {
      edge = (*pTess).DomFaceEdgeNb[i][j];
      DomEdgeFaceQty[edge]++;
      (*pTess).DomEdgeFaceNb[edge] =
        ut_realloc_1d_int ((*pTess).DomEdgeFaceNb[edge],
                           DomEdgeFaceQty[edge]);
      (*pTess).DomEdgeFaceNb[edge][DomEdgeFaceQty[edge] - 1] = i;
    }

  ut_free_1d_int (&DomEdgeFaceQty);

  return;
}

void
neut_tess_domface_tess_general (struct TESS Tess, struct TESS *pT)
{
  if (Tess.Dim != 3)
    ut_print_neperbug ();

  (*pT).Dim = 2;
  (*pT).PseudoDim = 2;
  (*pT).Level = 1;
  (*pT).TessId = 1;
  ut_string_string (Tess.Type, &((*pT).Type));

  if (Tess.Periodic)
  {
    (*pT).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy (Tess.Periodic, 3, (*pT).Periodic);
  }

  if (Tess.PeriodicDist)
  {
    (*pT).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy (Tess.PeriodicDist, 3, (*pT).PeriodicDist);
  }

  (*pT).ScaleQty = Tess.ScaleQty;

  return;
}

void
neut_tess_domface_tess_seeds (struct TESS Tess, struct TESS *pT)
{
  (*pT).SeedQty = Tess.SeedQty;

  (*pT).SeedCoo = ut_alloc_2d ((*pT).SeedQty + 1, 3);
  (*pT).SeedWeight = ut_alloc_1d ((*pT).SeedQty + 1);

  ut_array_2d_memcpy (Tess.SeedCoo + 1, (*pT).SeedQty, 3, (*pT).SeedCoo + 1);
  ut_array_1d_memcpy (Tess.SeedWeight + 1, (*pT).SeedQty,
                      (*pT).SeedWeight + 1);

  (*pT).PerSeedQty = Tess.PerSeedQty;

  if (Tess.PerSeedMaster)
  {
    (*pT).PerSeedMaster = ut_alloc_1d_int ((*pT).SeedQty + 1);
    ut_array_1d_int_memcpy (Tess.PerSeedMaster + 1, (*pT).SeedQty,
                            (*pT).PerSeedMaster + 1);
  }

  if (Tess.PerSeedShift)
  {
    (*pT).PerSeedShift = ut_alloc_2d_int ((*pT).SeedQty + 1, 3);
    ut_array_2d_int_memcpy (Tess.PerSeedShift + 1, (*pT).SeedQty, 3,
                            (*pT).PerSeedShift + 1);
  }

  if (Tess.PerSeedSlave)
  {
    (*pT).PerSeedSlave = ut_alloc_2d_int ((*pT).SeedQty + 1, 27);
    ut_array_2d_int_memcpy (Tess.PerSeedSlave + 1, (*pT).SeedQty, 27,
                            (*pT).PerSeedSlave + 1);
  }

  return;
}

void
neut_tess_domface_tess_cells (struct TESS Tess, int *oldface_newface,
                              struct TESS *pT)
{
  int i, j, newfaceqty, *newface_oldface = NULL;
  int *face_oldpoly = ut_alloc_1d_int ((*pT).FaceQty + 1);

  if (oldface_newface)
    ut_array_1d_int_inv (oldface_newface, Tess.FaceQty + 1, &newface_oldface,
                         &newfaceqty);

  for (i = 1; i <= (*pT).FaceQty; i++)
    if ((*pT).FaceVerQty[i])
      face_oldpoly[i] =
        oldface_newface ? Tess.FacePoly[newface_oldface[i]][0] : Tess.
        FacePoly[i][0];

  (*pT).CellQty = (*pT).FaceQty;

  (*pT).CellId = ut_alloc_1d_int ((*pT).CellQty + 1);
  ut_array_1d_int_set_id ((*pT).CellId, (*pT).CellQty + 1);
  for (i = 1; i <= (*pT).CellQty; i++)
    (*pT).CellId[i] = neut_tess_cell_id (Tess, face_oldpoly[i]);

  (*pT).ScaleCellId = ut_alloc_2d_int ((*pT).CellQty + 1, (*pT).ScaleQty + 1);
  for (i = 1; i <= (*pT).CellQty; i++)
    if (newface_oldface && Tess.ScaleCellId)
      for (j = 1; j <= (*pT).ScaleQty; j++)
        (*pT).ScaleCellId[i][j] = Tess.ScaleCellId[face_oldpoly[i]][j];

  if (Tess.CellOri)
  {
    (*pT).CellOri = ut_alloc_2d (Tess.CellQty + 1, 4);
    for (i = 1; i <= (*pT).CellQty; i++)
      ut_array_1d_memcpy (Tess.CellOri[face_oldpoly[i]], 4, (*pT).CellOri[i]);
  }

  if (Tess.CellOriDistrib)
  {
    (*pT).CellOriDistrib = ut_alloc_1d_pchar (Tess.CellQty + 1);
    for (i = 0; i <= (*pT).CellQty; i++)
      ut_string_string (Tess.CellOriDistrib[face_oldpoly[i]], (*pT).CellOriDistrib + i);
  }

  if (Tess.CellOriDes)
      ut_string_string (Tess.CellOriDes, &(*pT).CellOriDes);

  if (Tess.CellModeId)
  {
    (*pT).CellModeId = ut_alloc_1d_int (Tess.CellQty + 1);
    for (i = 1; i <= (*pT).CellQty; i++)
      (*pT).CellModeId[i] = Tess.CellModeId[face_oldpoly[i]];
  }

  if (Tess.CellGroup)
  {
    (*pT).CellGroup = ut_alloc_1d_int (Tess.CellQty + 1);
    for (i = 1; i <= (*pT).CellQty; i++)
      (*pT).CellGroup[i] = Tess.CellGroup[face_oldpoly[i]];
  }

  if (Tess.CellLamId)
  {
    (*pT).CellLamId = ut_alloc_1d_int ((*pT).CellQty + 1);
    ut_array_1d_int_memcpy (Tess.CellLamId + 1, (*pT).CellQty,
                            (*pT).CellLamId + 1);
  }

  if (Tess.CellModeId)
  {
    (*pT).CellModeId = ut_alloc_1d_int ((*pT).CellQty + 1);
    ut_array_1d_int_memcpy (Tess.CellModeId + 1, (*pT).CellQty,
                            (*pT).CellModeId + 1);
  }

  if (Tess.CellGroup)
  {
    (*pT).CellGroup = ut_alloc_1d_int ((*pT).CellQty + 1);
    ut_array_1d_int_memcpy (Tess.CellGroup + 1, (*pT).CellQty,
                            (*pT).CellGroup + 1);
  }

  ut_string_string (Tess.CellCrySym, &((*pT).CellCrySym));

  ut_free_1d_int (&newface_oldface);
  ut_free_1d_int (&face_oldpoly);

  return;
}

void
neut_tess_domface_tess_faces (struct TESS Tess, int domface, struct TESS *pT,
                              int *oldface_newface)
{
  int i, oldface;

  // Faces
  (*pT).FaceQty = Tess.DomTessFaceQty[domface];
  (*pT).FacePoly = ut_alloc_2d_int ((*pT).FaceQty + 1, 2);
  (*pT).FaceEq = ut_alloc_2d ((*pT).FaceQty + 1, 4);
  (*pT).FaceVerQty = ut_alloc_1d_int ((*pT).FaceQty + 1);
  (*pT).FaceVerNb = ut_alloc_1d_pint ((*pT).FaceQty + 1);
  (*pT).FaceEdgeNb = ut_alloc_1d_pint ((*pT).FaceQty + 1);
  (*pT).FaceEdgeOri = ut_alloc_1d_pint ((*pT).FaceQty + 1);
  (*pT).FaceState = ut_alloc_1d_int ((*pT).FaceQty + 1);
  (*pT).FacePt = ut_alloc_1d_int ((*pT).FaceQty + 1);
  (*pT).FacePtCoo = ut_alloc_2d ((*pT).FaceQty + 1, 3);
  (*pT).FaceDom = ut_alloc_2d_int ((*pT).FaceQty + 1, 2);

  for (i = 1; i <= Tess.DomTessFaceQty[domface]; i++)
  {
    oldface = Tess.DomTessFaceNb[domface][i];
    oldface_newface[oldface] = i;

    ut_array_1d_memcpy (Tess.FaceEq[oldface], 4, (*pT).FaceEq[i]);
    (*pT).FaceVerQty[i] = Tess.FaceVerQty[oldface];
    (*pT).FaceVerNb[i] = ut_alloc_1d_int ((*pT).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.FaceVerNb[oldface] + 1, (*pT).FaceVerQty[i],
                            (*pT).FaceVerNb[i] + 1);
    (*pT).FaceEdgeNb[i] = ut_alloc_1d_int ((*pT).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.FaceEdgeNb[oldface] + 1, (*pT).FaceVerQty[i],
                            (*pT).FaceEdgeNb[i] + 1);
    (*pT).FaceEdgeOri[i] = ut_alloc_1d_int ((*pT).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.FaceEdgeOri[oldface] + 1,
                            (*pT).FaceVerQty[i], (*pT).FaceEdgeOri[i] + 1);
    (*pT).FaceState[i] = Tess.FaceState[oldface];
    (*pT).FacePt[i] = -1;       // for a 2D tess, must be -1.
    ut_array_1d_int_memcpy (Tess.FaceDom[oldface], 2, (*pT).FaceDom[i]);
  }

  return;
}

void
neut_tess_3dcolumnar_2d_faces (struct TESS Tess, int domface, struct TESS *pT,
                               int *oldface_newface)
{
  int i, j, face, oldface;

  (*pT).FaceQty = Tess.PolyQty;
  (*pT).FacePoly = NULL;
  (*pT).FaceEq = ut_alloc_2d ((*pT).FaceQty + 1, 4);
  (*pT).FaceVerQty = ut_alloc_1d_int ((*pT).FaceQty + 1);
  (*pT).FaceVerNb = ut_alloc_1d_pint ((*pT).FaceQty + 1);
  (*pT).FaceEdgeNb = ut_alloc_1d_pint ((*pT).FaceQty + 1);
  (*pT).FaceEdgeOri = ut_alloc_1d_pint ((*pT).FaceQty + 1);
  (*pT).FaceState = ut_alloc_1d_int ((*pT).FaceQty + 1);
  (*pT).FacePt = ut_alloc_1d_int ((*pT).FaceQty + 1);
  (*pT).FacePtCoo = ut_alloc_2d ((*pT).FaceQty + 1, 3);
  (*pT).FaceDom = NULL;

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    if (Tess.PolyFaceQty[i] == 0)
      continue;

    oldface = -1;
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      face = Tess.PolyFaceNb[i][j];
      if (Tess.FaceDom[face][0] == 2 && Tess.FaceDom[face][1] == domface)
      {
        oldface = face;
        break;
      }
    }
    if (oldface == -1)
      ut_print_neperbug ();
    oldface_newface[oldface] = i;

    ut_array_1d_memcpy (Tess.FaceEq[oldface], 4, (*pT).FaceEq[i]);
    (*pT).FaceVerQty[i] = Tess.FaceVerQty[oldface];
    (*pT).FaceVerNb[i] = ut_alloc_1d_int ((*pT).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.FaceVerNb[oldface] + 1, (*pT).FaceVerQty[i],
                            (*pT).FaceVerNb[i] + 1);
    (*pT).FaceEdgeNb[i] = ut_alloc_1d_int ((*pT).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.FaceEdgeNb[oldface] + 1, (*pT).FaceVerQty[i],
                            (*pT).FaceEdgeNb[i] + 1);
    (*pT).FaceEdgeOri[i] = ut_alloc_1d_int ((*pT).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.FaceEdgeOri[oldface] + 1,
                            (*pT).FaceVerQty[i], (*pT).FaceEdgeOri[i] + 1);
    (*pT).FaceState[i] = Tess.FaceState[oldface];
    (*pT).FacePt[i] = -1;       // for a 2D tess, must be -1.
  }

  return;
}

void
neut_tess_domface_tess_edges (struct TESS Tess, int domface, struct TESS *pT,
                              int *oldedge_newedge)
{
  int i, oldedge, edgeqty, *edge = NULL;
  int periodic = (Tess.PerEdgeMaster != NULL);

  // Edges
  neut_tess_domface_edges (Tess, domface, &edge, &edgeqty);

  (*pT).EdgeQty = edgeqty;
  (*pT).EdgeVerNb = ut_alloc_2d_int ((*pT).EdgeQty + 1, 2);
  (*pT).EdgeFaceQty = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  (*pT).EdgeFaceNb = ut_alloc_1d_pint ((*pT).EdgeQty + 1);
  (*pT).EdgeLength = ut_alloc_1d ((*pT).EdgeQty + 1);
  (*pT).EdgeState = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  (*pT).EdgeDel = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  (*pT).EdgeDom = ut_alloc_2d_int ((*pT).EdgeQty + 1, 2);

  if (periodic)
  {
    (*pT).PerEdgeMaster = ut_alloc_1d_int ((*pT).EdgeQty + 1);
    (*pT).PerEdgeShift = ut_alloc_2d_int ((*pT).EdgeQty + 1, 3);
    (*pT).PerEdgeOri = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  }

  (*pT).PerEdgeQty = 0;
  for (i = 1; i <= edgeqty; i++)
  {
    oldedge = edge[i - 1];
    oldedge_newedge[oldedge] = i;

    ut_array_1d_int_memcpy (Tess.EdgeVerNb[oldedge], 2, (*pT).EdgeVerNb[i]);

    (*pT).EdgeFaceQty[i] = Tess.EdgeFaceQty[oldedge];
    (*pT).EdgeFaceNb[i] = ut_alloc_1d_int ((*pT).EdgeFaceQty[i]);
    ut_array_1d_int_memcpy (Tess.EdgeFaceNb[oldedge], (*pT).EdgeFaceQty[i],
                            (*pT).EdgeFaceNb[i]);

    (*pT).EdgeLength[i] = Tess.EdgeLength[oldedge];
    (*pT).EdgeState[i] = Tess.EdgeState[oldedge];
    (*pT).EdgeDel[i] = Tess.EdgeDel[oldedge];
    ut_array_1d_int_memcpy (Tess.EdgeDom[oldedge], 2, (*pT).EdgeDom[i]);

    if (periodic)
    {
      (*pT).PerEdgeMaster[i] = Tess.PerEdgeMaster[oldedge];
      ut_array_1d_int_memcpy (Tess.PerEdgeShift[oldedge], 3,
                              (*pT).PerEdgeShift[i]);
      (*pT).PerEdgeOri[i] = Tess.PerEdgeOri[oldedge];

      if ((*pT).PerEdgeMaster[i])
      {
        (*pT).PerEdgeQty++;
        (*pT).PerEdgeNb =
          ut_realloc_1d_int ((*pT).PerEdgeNb, (*pT).PerEdgeQty + 1);
        (*pT).PerEdgeNb[(*pT).PerEdgeQty] = i;
      }
    }
  }

  ut_free_1d_int (&edge);

  return;
}

void
neut_tess_3dcolumnar_2d_edges (struct TESS Tess, int domface, struct TESS *pT,
                               int *oldedge_newedge)
{
  int i, oldedge, edgeqty, *edge = NULL;
  int periodic = (Tess.PerEdgeMaster != NULL);

  // Edges
  neut_tess_domface_edges (Tess, domface, &edge, &edgeqty);

  (*pT).EdgeQty = edgeqty;
  (*pT).EdgeVerNb = ut_alloc_2d_int ((*pT).EdgeQty + 1, 2);
  (*pT).EdgeFaceQty = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  (*pT).EdgeFaceNb = ut_alloc_1d_pint ((*pT).EdgeQty + 1);
  (*pT).EdgeLength = ut_alloc_1d ((*pT).EdgeQty + 1);
  (*pT).EdgeState = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  (*pT).EdgeDel = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  (*pT).EdgeDom = ut_alloc_2d_int ((*pT).EdgeQty + 1, 2);

  if (periodic)
  {
    (*pT).PerEdgeMaster = ut_alloc_1d_int ((*pT).EdgeQty + 1);
    (*pT).PerEdgeShift = ut_alloc_2d_int ((*pT).EdgeQty + 1, 3);
    (*pT).PerEdgeOri = ut_alloc_1d_int ((*pT).EdgeQty + 1);
  }

  (*pT).PerEdgeQty = 0;
  for (i = 1; i <= edgeqty; i++)
  {
    oldedge = edge[i - 1];
    oldedge_newedge[oldedge] = i;

    ut_array_1d_int_memcpy (Tess.EdgeVerNb[oldedge], 2, (*pT).EdgeVerNb[i]);

    neut_tess_edge_seeds_positive (Tess, oldedge, (*pT).EdgeFaceNb + i,
                                   (*pT).EdgeFaceQty + i);
    /*
       (*pT).EdgeFaceQty[i] = Tess.EdgeFaceQty[oldedge];
       (*pT).EdgeFaceNb[i] = ut_alloc_1d_int ((*pT).EdgeFaceQty[i]);
       ut_array_1d_int_memcpy ((*pT).EdgeFaceNb[i], (*pT).EdgeFaceQty[i],
       Tess.EdgeFaceNb[oldedge]);
     */

    (*pT).EdgeLength[i] = Tess.EdgeLength[oldedge];
    (*pT).EdgeState[i] = Tess.EdgeState[oldedge];
    (*pT).EdgeDel[i] = Tess.EdgeDel[oldedge];
    ut_array_1d_int_memcpy (Tess.EdgeDom[oldedge], 2, (*pT).EdgeDom[i]);
    if ((*pT).EdgeDom[i][0] == 2)
      ut_array_1d_int_set_2 ((*pT).EdgeDom[i], -1, 0);

    if (periodic)
    {
      (*pT).PerEdgeMaster[i] = Tess.PerEdgeMaster[oldedge];
      ut_array_1d_int_memcpy (Tess.PerEdgeShift[oldedge], 3,
                              (*pT).PerEdgeShift[i]);
      (*pT).PerEdgeOri[i] = Tess.PerEdgeOri[oldedge];

      if ((*pT).PerEdgeMaster[i])
      {
        (*pT).PerEdgeQty++;
        (*pT).PerEdgeNb =
          ut_realloc_1d_int ((*pT).PerEdgeNb, (*pT).PerEdgeQty + 1);
        (*pT).PerEdgeNb[(*pT).PerEdgeQty] = i;
      }
    }
    // init slaves
  }

  ut_free_1d_int (&edge);

  return;
}

void
neut_tess_domface_tess_vers (struct TESS Tess, int domface, struct TESS *pT,
                             int *oldver_newver)
{
  int i, oldver, verqty, *ver = NULL;
  int periodic = (Tess.PerVerMaster != NULL);

  // Vertices
  neut_tess_domface_vers (Tess, domface, &ver, &verqty);

  (*pT).VerQty = verqty;
  (*pT).VerCoo = ut_alloc_2d ((*pT).VerQty + 1, 3);
  (*pT).VerEdgeQty = ut_alloc_1d_int ((*pT).VerQty + 1);
  (*pT).VerEdgeNb = ut_alloc_1d_pint ((*pT).VerQty + 1);
  (*pT).VerDom = ut_alloc_2d_int ((*pT).VerQty + 1, 2);
  (*pT).VerState = ut_alloc_1d_int ((*pT).VerQty + 1);

  if (periodic)
  {
    (*pT).PerVerMaster = ut_alloc_1d_int ((*pT).VerQty + 1);
    (*pT).PerVerShift = ut_alloc_2d_int ((*pT).VerQty + 1, 3);
  }

  for (i = 1; i <= verqty; i++)
  {
    oldver = ver[i - 1];
    oldver_newver[oldver] = i;

    ut_array_1d_memcpy (Tess.VerCoo[oldver], 3, (*pT).VerCoo[i]);
    (*pT).VerEdgeQty[i] = Tess.VerEdgeQty[oldver];
    (*pT).VerEdgeNb[i] = ut_alloc_1d_int ((*pT).VerEdgeQty[i]);
    ut_array_1d_int_memcpy (Tess.VerEdgeNb[oldver], (*pT).VerEdgeQty[i],
                            (*pT).VerEdgeNb[i]);
    ut_array_1d_int_memcpy (Tess.VerDom[oldver], 2, (*pT).VerDom[i]);
    (*pT).VerState[i] = Tess.VerState[oldver];

    if (periodic)
    {
      (*pT).PerVerMaster[i] = Tess.PerVerMaster[oldver];
      ut_array_1d_int_memcpy (Tess.PerVerShift[oldver], 3,
                              (*pT).PerVerShift[i]);

      if ((*pT).PerVerMaster[i])
      {
        (*pT).PerVerQty++;
        (*pT).PerVerNb =
          ut_realloc_1d_int ((*pT).PerVerNb, (*pT).PerVerQty + 1);
        (*pT).PerVerNb[(*pT).PerVerQty] = i;
      }
    }
  }

  ut_free_1d_int (&ver);

  return;
}

void
neut_tess_3dcolumnar_2d_vers (struct TESS Tess, int domface,
                              int *oldedge_newedge, struct TESS *pT,
                              int *oldver_newver)
{
  int i, j, edge, oldver, verqty, *ver = NULL;
  int periodic = (Tess.PerVerMaster != NULL);

  // Vertices
  neut_tess_domface_vers (Tess, domface, &ver, &verqty);

  (*pT).VerQty = verqty;
  (*pT).VerCoo = ut_alloc_2d ((*pT).VerQty + 1, 3);
  (*pT).VerEdgeQty = ut_alloc_1d_int ((*pT).VerQty + 1);
  (*pT).VerEdgeNb = ut_alloc_1d_pint ((*pT).VerQty + 1);
  (*pT).VerDom = ut_alloc_2d_int ((*pT).VerQty + 1, 2);
  (*pT).VerState = ut_alloc_1d_int ((*pT).VerQty + 1);

  if (periodic)
  {
    (*pT).PerVerMaster = ut_alloc_1d_int ((*pT).VerQty + 1);
    (*pT).PerVerShift = ut_alloc_2d_int ((*pT).VerQty + 1, 3);
  }

  for (i = 1; i <= verqty; i++)
  {
    oldver = ver[i - 1];
    oldver_newver[oldver] = i;

    ut_array_1d_memcpy (Tess.VerCoo[oldver], 3, (*pT).VerCoo[i]);

    for (j = 0; j < Tess.VerEdgeQty[oldver]; j++)
    {
      edge = Tess.VerEdgeNb[oldver][j];

      // oldedge_newedge is used only to know if the edge is to be
      // recorded.  The old edge number is recorded.  It is updated
      // afterwards.
      if (oldedge_newedge[edge] != 0)
      {
        (*pT).VerEdgeQty[i]++;
        (*pT).VerEdgeNb[i] =
          ut_realloc_1d_int ((*pT).VerEdgeNb[i], (*pT).VerEdgeQty[i]);
        (*pT).VerEdgeNb[i][(*pT).VerEdgeQty[i] - 1] = edge;
      }
    }

    ut_array_1d_int_memcpy (Tess.VerDom[oldver], 2, (*pT).VerDom[i]);
    (*pT).VerState[i] = Tess.VerState[oldver];

    if (periodic)
    {
      (*pT).PerVerMaster[i] = Tess.PerVerMaster[oldver];
      ut_array_1d_int_memcpy (Tess.PerVerShift[oldver], 3,
                              (*pT).PerVerShift[i]);

      if ((*pT).PerVerMaster[i])
      {
        (*pT).PerVerQty++;
        (*pT).PerVerNb =
          ut_realloc_1d_int ((*pT).PerVerNb, (*pT).PerVerQty + 1);
        (*pT).PerVerNb[(*pT).PerVerQty] = i;
      }
    }
    // init slave
  }

  ut_free_1d_int (&ver);

  return;
}

void
neut_tess_domface_tess_renumber (struct TESS *pT, int *oldface_newface,
                                 int *oldedge_newedge, int *oldver_newver)
{
  int i, j;

  // Renumbering faces in edges
  for (i = 1; i <= (*pT).EdgeQty; i++)
  {
    for (j = 0; j < (*pT).EdgeFaceQty[i]; j++)
      (*pT).EdgeFaceNb[i][j] = oldface_newface[(*pT).EdgeFaceNb[i][j]];

    (*pT).EdgeFaceQty[i] -=
      ut_array_1d_int_rmelt ((*pT).EdgeFaceNb[i], (*pT).EdgeFaceQty[i], 0,
                             (*pT).EdgeFaceQty[i]);
  }

  // Renumbering edges in vertices and faces
  for (i = 1; i <= (*pT).VerQty; i++)
  {
    for (j = 0; j < (*pT).VerEdgeQty[i]; j++)
      (*pT).VerEdgeNb[i][j] = oldedge_newedge[(*pT).VerEdgeNb[i][j]];

    (*pT).VerEdgeQty[i] -=
      ut_array_1d_int_rmelt ((*pT).VerEdgeNb[i], (*pT).VerEdgeQty[i], 0,
                             (*pT).VerEdgeQty[i]);
  }

  for (i = 1; i <= (*pT).FaceQty; i++)
    for (j = 1; j <= (*pT).FaceVerQty[i]; j++)
      (*pT).FaceEdgeNb[i][j] = oldedge_newedge[(*pT).FaceEdgeNb[i][j]];

  // Renumbering vertices in edges and faces
  for (i = 1; i <= (*pT).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pT).EdgeVerNb[i][j] = oldver_newver[(*pT).EdgeVerNb[i][j]];

  for (i = 1; i <= (*pT).FaceQty; i++)
    for (j = 1; j <= (*pT).FaceVerQty[i]; j++)
      (*pT).FaceVerNb[i][j] = oldver_newver[(*pT).FaceVerNb[i][j]];

  return;
}

void
neut_tess_3dcolumnar_2d_renumber (struct TESS *pT, int *oldedge_newedge,
                                  int *oldver_newver)
{
  int i, j;

  // Renumbering edges in vertices and faces
  for (i = 1; i <= (*pT).VerQty; i++)
  {
    for (j = 0; j < (*pT).VerEdgeQty[i]; j++)
      (*pT).VerEdgeNb[i][j] = oldedge_newedge[(*pT).VerEdgeNb[i][j]];

    (*pT).VerEdgeQty[i] -=
      ut_array_1d_int_rmelt ((*pT).VerEdgeNb[i], (*pT).VerEdgeQty[i], 0,
                             (*pT).VerEdgeQty[i]);
  }

  for (i = 1; i <= (*pT).FaceQty; i++)
    for (j = 1; j <= (*pT).FaceVerQty[i]; j++)
      (*pT).FaceEdgeNb[i][j] = oldedge_newedge[(*pT).FaceEdgeNb[i][j]];

  // Renumbering vertices in edges and faces
  for (i = 1; i <= (*pT).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pT).EdgeVerNb[i][j] = oldver_newver[(*pT).EdgeVerNb[i][j]];

  for (i = 1; i <= (*pT).FaceQty; i++)
    for (j = 1; j <= (*pT).FaceVerQty[i]; j++)
      (*pT).FaceVerNb[i][j] = oldver_newver[(*pT).FaceVerNb[i][j]];

  if ((*pT).PerVerMaster)
    for (i = 1; i <= (*pT).VerQty; i++)
      if ((*pT).PerVerMaster[i] > 0)
      {
        if (oldver_newver[(*pT).PerVerMaster[i]] == 0)
          ut_print_neperbug ();
        (*pT).PerVerMaster[i] = oldver_newver[(*pT).PerVerMaster[i]];
      }

  if ((*pT).PerEdgeMaster)
    for (i = 1; i <= (*pT).EdgeQty; i++)
      if ((*pT).PerEdgeMaster[i] > 0)
      {
        if (oldedge_newedge[(*pT).PerEdgeMaster[i]] == 0)
          ut_print_neperbug ();
        (*pT).PerEdgeMaster[i] = oldedge_newedge[(*pT).PerEdgeMaster[i]];
      }

  return;
}

void
neut_tess_domface_tess_domain (struct TESS Tess, int domface, struct TESS *pT,
                               int *oldedge_newedge, int *oldver_newver)
{
  int i, j, oldedge, oldver, otherdomface;
  int *olddedge_newdedge = ut_alloc_1d_int (Tess.EdgeQty + 1);
  int *olddver_newdver = ut_alloc_1d_int (Tess.VerQty + 1);

  // Domain
  (*pT).DomType = ut_alloc_1d_char (strlen (Tess.DomType) + 1);
  strcpy ((*pT).DomType, Tess.DomType);

  // Domain faces
  (*pT).DomFaceQty = 0;

  // Domain edges
  (*pT).DomEdgeQty = Tess.DomFaceEdgeQty[domface];
  (*pT).DomEdgeLabel = ut_alloc_1d_pchar ((*pT).DomEdgeQty + 1);
  (*pT).DomEdgeVerQty = ut_alloc_1d_int ((*pT).DomEdgeQty + 1);
  (*pT).DomEdgeVerNb = ut_alloc_2d_int ((*pT).DomEdgeQty + 1, 2);
  (*pT).DomEdgeFaceNb = ut_alloc_2d_int ((*pT).DomEdgeQty + 1, 2);
  (*pT).DomTessEdgeQty = ut_alloc_1d_int ((*pT).DomEdgeQty + 1);
  (*pT).DomTessEdgeNb = ut_alloc_1d_pint ((*pT).DomEdgeQty + 1);

  (*pT).DomEdgeType = ut_alloc_1d_pchar ((*pT).DomEdgeQty + 1);
  (*pT).DomEdgeParmQty = ut_alloc_1d_int ((*pT).DomEdgeQty + 1);
  (*pT).DomEdgeParms = ut_alloc_1d_pdouble ((*pT).DomEdgeQty + 1);

  for (i = 1; i <= Tess.DomFaceEdgeQty[domface]; i++)
  {
    oldedge = Tess.DomFaceEdgeNb[domface][i];
    olddedge_newdedge[oldedge] = i;

    (*pT).DomEdgeLabel[i] =
      ut_alloc_1d_char (strlen (Tess.DomEdgeLabel[oldedge]) + 1);
    strcpy ((*pT).DomEdgeLabel[i], Tess.DomEdgeLabel[oldedge]);
    ut_string_fnrs ((*pT).DomEdgeLabel[i], Tess.DomFaceLabel[domface], "", 1);

    (*pT).DomEdgeVerQty[i] = Tess.DomEdgeVerQty[oldedge];
    ut_array_1d_int_memcpy (Tess.DomEdgeVerNb[oldedge],
                            (*pT).DomEdgeVerQty[i], (*pT).DomEdgeVerNb[i]);
    (*pT).DomEdgeFaceNb[i][0] = domface;

    (*pT).DomTessEdgeQty[i] = Tess.DomTessEdgeQty[oldedge];
    (*pT).DomTessEdgeNb[i] = ut_alloc_1d_int ((*pT).DomTessEdgeQty[i] + 1);
    ut_array_1d_int_memcpy (Tess.DomTessEdgeNb[oldedge] + 1,
                            (*pT).DomTessEdgeQty[i],
                            (*pT).DomTessEdgeNb[i] + 1);

    otherdomface = (Tess.DomEdgeFaceNb[i][0] == domface) ? Tess.DomEdgeFaceNb[i][1] : Tess.DomEdgeFaceNb[i][0];

    if (!strcmp (Tess.DomFaceType[otherdomface], "plane"))
    {
      ut_string_string ("line", (*pT).DomEdgeType + i);
      (*pT).DomEdgeParmQty[i] = 0;
    }
    else
    {
      ut_string_string (Tess.DomFaceType[otherdomface], (*pT).DomEdgeType + i);
      (*pT).DomEdgeParmQty[i] = Tess.DomFaceParmQty[otherdomface];
      (*pT).DomEdgeParms[i] = ut_alloc_1d ((*pT).DomEdgeParmQty[i]);
      ut_array_1d_memcpy (Tess.DomFaceParms[otherdomface], (*pT).DomEdgeParmQty[i], (*pT).DomEdgeParms[i]);
    }
  }

  // Domain vertices
  (*pT).DomVerQty = Tess.DomFaceVerQty[domface];
  (*pT).DomVerCoo = ut_alloc_2d ((*pT).DomVerQty + 1, 3);
  (*pT).DomVerLabel = ut_alloc_1d_pchar ((*pT).DomVerQty + 1);
  (*pT).DomVerEdgeQty = ut_alloc_1d_int ((*pT).DomVerQty + 1);
  (*pT).DomVerEdgeNb = ut_alloc_1d_pint ((*pT).DomVerQty + 1);
  (*pT).DomTessVerNb = ut_alloc_1d_int ((*pT).DomVerQty + 1);
  for (i = 1; i <= (*pT).DomVerQty; i++)
  {
    oldver = Tess.DomFaceVerNb[domface][i];
    olddver_newdver[oldver] = i;

    (*pT).DomVerLabel[i] =
      ut_alloc_1d_char (strlen (Tess.DomVerLabel[oldver]) + 1);
    strcpy ((*pT).DomVerLabel[i], Tess.DomVerLabel[oldver]);
    ut_string_fnrs ((*pT).DomVerLabel[i], Tess.DomFaceLabel[domface], "", 1);

    ut_array_1d_memcpy (Tess.DomVerCoo[oldver], 3, (*pT).DomVerCoo[i]);

    (*pT).DomVerEdgeQty[i] = Tess.DomVerEdgeQty[oldver];
    (*pT).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pT).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy (Tess.DomVerEdgeNb[oldver], (*pT).DomVerEdgeQty[i],
                            (*pT).DomVerEdgeNb[i]);
    (*pT).DomTessVerNb[i] = Tess.DomTessVerNb[oldver];
  }

  // Renumbering domain faces in domain edges
  for (i = 1; i <= (*pT).DomVerQty; i++)
    (*pT).DomEdgeFaceNb[i][0] = 1;

  // Renumbering domain edges in domain vertices
  for (i = 1; i <= (*pT).DomVerQty; i++)
  {
    for (j = 0; j < (*pT).DomVerEdgeQty[i]; j++)
      (*pT).DomVerEdgeNb[i][j] = olddedge_newdedge[(*pT).DomVerEdgeNb[i][j]];

    (*pT).DomVerEdgeQty[i] -=
      ut_array_1d_int_rmelt ((*pT).DomVerEdgeNb[i], (*pT).DomVerEdgeQty[i], 0,
                             (*pT).DomVerEdgeQty[i]);

    ut_array_1d_int_sort ((*pT).DomVerEdgeNb[i], (*pT).DomVerEdgeQty[i]);
  }

  // Renumbering domain vertices in domain edges
  for (i = 1; i <= (*pT).DomEdgeQty; i++)
    for (j = 0; j < (*pT).DomEdgeVerQty[i]; j++)
      (*pT).DomEdgeVerNb[i][j] = olddver_newdver[(*pT).DomEdgeVerNb[i][j]];

  // Renumbering edges in domain edges
  for (i = 1; i <= (*pT).DomEdgeQty; i++)
    for (j = 1; j <= (*pT).DomTessEdgeQty[i]; j++)
      (*pT).DomTessEdgeNb[i][j] = oldedge_newedge[(*pT).DomTessEdgeNb[i][j]];

  // Renumbering vers in domain vers
  for (i = 1; i <= (*pT).DomVerQty; i++)
    (*pT).DomTessVerNb[i] = oldver_newver[(*pT).DomTessVerNb[i]];

  // Renumbering domain faces in faces
  /*
     for (i = 1; i <= (*pT).FaceQty; i++)
     (*pT).FaceDom[i][1] = 1;
   */

  // Renumbering domain edges and faces in edges
  for (i = 1; i <= (*pT).EdgeQty; i++)
    if ((*pT).EdgeDom[i][0] == 2)
    {
      if ((*pT).EdgeDom[i][1] == domface)
        ut_array_1d_int_set_2 ((*pT).EdgeDom[i], -1, 0);
      else
        (*pT).EdgeDom[i][1] = 1;
    }
    else if ((*pT).EdgeDom[i][0] == 1)
    {
      (*pT).EdgeDom[i][1] = olddedge_newdedge[(*pT).EdgeDom[i][1]];
      if ((*pT).EdgeDom[i][1] == 0)
        ut_print_neperbug ();
    }

  // Renumbering domain edges and faces in vertices
  for (i = 1; i <= (*pT).VerQty; i++)
  {
    if ((*pT).VerDom[i][0] == 2)
    {
      if ((*pT).VerDom[i][1] == domface)
        ut_array_1d_int_set_2 ((*pT).VerDom[i], -1, 0);
      else
        (*pT).VerDom[i][0] = 1;
    }
    else if ((*pT).VerDom[i][0] == 1)
      (*pT).VerDom[i][1] = olddedge_newdedge[(*pT).VerDom[i][1]];
    else if ((*pT).VerDom[i][0] == 0)
      (*pT).VerDom[i][1] = olddver_newdver[(*pT).VerDom[i][1]];
  }

  ut_free_1d_int (&olddedge_newdedge);
  ut_free_1d_int (&olddver_newdver);

  return;
}
