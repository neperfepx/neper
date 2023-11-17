/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_.h"

void
neut_tess_tess_gen (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).Dim = TessA.Dim;
  (*pTessB).PseudoDim = TessA.PseudoDim;
  (*pTessB).PseudoSize = TessA.PseudoSize;
  (*pTessB).Level = TessA.Level;
  (*pTessB).TessId = TessA.TessId;

  if (TessA.Type)
    ut_string_string (TessA.Type, &((*pTessB).Type));

  if (TessA.Periodic)
  {
    (*pTessB).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy (TessA.Periodic, 3, (*pTessB).Periodic);
  }

  if (TessA.PeriodicDist)
  {
    (*pTessB).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy (TessA.PeriodicDist, 3, (*pTessB).PeriodicDist);
  }

  return;
}

void
neut_tess_tess_cell (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).CellQty = TessA.CellQty;

  if (TessA.Type)
    ut_string_string (TessA.Type, &((*pTessB).Type));

  if (TessA.CellId)
  {
    (*pTessB).CellId = ut_alloc_1d_int (TessA.CellQty + 1);
    ut_array_1d_int_memcpy (TessA.CellId + 1, TessA.CellQty,
                            (*pTessB).CellId + 1);
  }

  if (TessA.CellWeight)
  {
    (*pTessB).CellWeight = ut_alloc_1d (TessA.CellQty + 1);
    ut_array_1d_memcpy (TessA.CellWeight + 1, TessA.CellQty,
                            (*pTessB).CellWeight + 1);
  }

  if (TessA.CellOri)
  {
    (*pTessB).CellOri = ut_alloc_2d (TessA.CellQty + 1, 4);
    ut_array_2d_memcpy (TessA.CellOri + 1, TessA.CellQty, 4,
                        (*pTessB).CellOri + 1);
  }

  if (TessA.CellOriDistrib)
  {
    (*pTessB).CellOriDistrib = ut_alloc_2d_char (TessA.CellQty + 1, 1);
    for (i = 0; i <= TessA.CellQty; i++)
      ut_string_string (TessA.CellOriDistrib[i], (*pTessB).CellOriDistrib + i);
  }

  if (TessA.CellOriDes)
    ut_string_string (TessA.CellOriDes, &(*pTessB).CellOriDes);

  if (TessA.CellCrySym)
    ut_string_string (TessA.CellCrySym, &((*pTessB).CellCrySym));

  (*pTessB).ScaleQty = TessA.ScaleQty;

  if (TessA.ScaleQty > 0)
  {
    (*pTessB).ScaleQty = TessA.ScaleQty;
    if (TessA.ScaleCellId)
    {
      (*pTessB).ScaleCellId =
        ut_alloc_2d_int (TessA.CellQty + 1, TessA.ScaleQty);
      ut_array_2d_int_memcpy (TessA.ScaleCellId + 1, TessA.CellQty,
                              TessA.ScaleQty, (*pTessB).ScaleCellId + 1);
    }
  }

  neut_tess_tess_cellbody (TessA, pTessB);
  neut_tess_tess_celllamid (TessA, pTessB);
  neut_tess_tess_cellmodeid (TessA, pTessB);
  neut_tess_tess_cellgroupid (TessA, pTessB);

  return;
}

void
neut_tess_tess_seed (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).SeedQty = TessA.SeedQty;
  (*pTessB).PerSeedQty = TessA.PerSeedQty;
  neut_tess_tess_seedcoo (TessA, pTessB);
  neut_tess_tess_seedweight (TessA, pTessB);

  if (TessA.PerSeedMaster)
  {
    (*pTessB).PerSeedMaster = ut_alloc_1d_int ((*pTessB).SeedQty + 1);
    ut_array_1d_int_memcpy (TessA.PerSeedMaster + 1, (*pTessB).SeedQty,
                            (*pTessB).PerSeedMaster + 1);
  }

  if (TessA.PerSeedShift)
  {
    (*pTessB).PerSeedShift = ut_alloc_2d_int ((*pTessB).SeedQty + 1, 3);
    ut_array_2d_int_memcpy (TessA.PerSeedShift + 1, (*pTessB).SeedQty, 3,
                            (*pTessB).PerSeedShift + 1);
  }

  if (TessA.PerSeedSlave)
  {
    (*pTessB).PerSeedSlave = ut_alloc_2d_int ((*pTessB).SeedQty + 1, 27);
    ut_array_2d_int_memcpy (TessA.PerSeedSlave, (*pTessB).SeedQty, 27,
                            (*pTessB).PerSeedSlave);
  }

  return;
}

/* neut_tess_tesstializing the properties of the vertices */
void
neut_tess_tess_ver (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tess_verqty (TessA, pTessB);
  neut_tess_tess_vercoo (TessA, pTessB);
  neut_tess_tess_veredge (TessA, pTessB);
  neut_tess_tess_verbound (TessA, pTessB);
  neut_tess_tess_verstate (TessA, pTessB);

  return;
}

/* neut_tess_tesstializing the properties of the edges */
void
neut_tess_tess_edge (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tess_edgeqty (TessA, pTessB);
  neut_tess_tess_edgevernb (TessA, pTessB);
  if (TessA.Dim >= 2)
    neut_tess_tess_edgeface (TessA, pTessB);
  neut_tess_tess_edgelength (TessA, pTessB);
  neut_tess_tess_edgedel (TessA, pTessB);
  neut_tess_tess_edgebound (TessA, pTessB);
  neut_tess_tess_edgestate (TessA, pTessB);

  return;
}

/* neut_tess_tess initializing the properties of the faces */
void
neut_tess_tess_face (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tess_faceqty (TessA, pTessB);
  if (TessA.Dim == 3)
    neut_tess_tess_facepoly (TessA, pTessB);
  neut_tess_tess_facever (TessA, pTessB);
  neut_tess_tess_faceedge (TessA, pTessB);
  neut_tess_tess_faceeq (TessA, pTessB);
  if (TessA.Dim == 3)
    neut_tess_tess_facebound (TessA, pTessB);
  neut_tess_tess_facestate (TessA, pTessB);
  neut_tess_tess_facept (TessA, pTessB);

  return;
}

/* neut_tess_tess initializing the properties of the polyhedra */
void
neut_tess_tess_poly (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tess_polyqty (TessA, pTessB);
  neut_tess_tess_polystate (TessA, pTessB);
  neut_tess_tess_polyface (TessA, pTessB);

  return;
}

/* neut_tess_tess initializing the properties of the domain */
void
neut_tess_tess_domain (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tess_domaindata (TessA, pTessB);

  return;
}

void
neut_tess_tess_per (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).PerVerQty = TessA.PerVerQty;

  if ((*pTessB).PerVerQty)
  {
    (*pTessB).PerVerNb = ut_alloc_1d_int ((*pTessB).PerVerQty + 1);
    ut_array_1d_int_memcpy (TessA.PerVerNb + 1, (*pTessB).PerVerQty,
                            (*pTessB).PerVerNb + 1);

    (*pTessB).PerVerMaster = ut_alloc_1d_int (TessA.VerQty + 1);
    ut_array_1d_int_memcpy (TessA.PerVerMaster + 1, TessA.VerQty,
                            (*pTessB).PerVerMaster + 1);

    (*pTessB).PerVerShift = ut_alloc_2d_int (TessA.VerQty + 1, 4);
    ut_array_2d_int_memcpy (TessA.PerVerShift + 1, TessA.VerQty, 3,
                            (*pTessB).PerVerShift + 1);

    (*pTessB).PerVerSlaveQty = ut_alloc_1d_int (TessA.VerQty + 1);
    (*pTessB).PerVerSlaveNb = ut_alloc_1d_pint (TessA.VerQty + 1);

    ut_array_1d_int_memcpy (TessA.PerVerSlaveQty + 1, TessA.VerQty,
                            (*pTessB).PerVerSlaveQty + 1);
    for (i = 1; i <= (*pTessB).VerQty; i++)
    {
      (*pTessB).PerVerSlaveNb[i] =
        ut_alloc_1d_int ((*pTessB).PerVerSlaveQty[i] + 1);
      ut_array_1d_int_memcpy (TessA.PerVerSlaveNb[i] + 1,
                              (*pTessB).PerVerSlaveQty[i],
                              (*pTessB).PerVerSlaveNb[i] + 1);
    }
  }

  (*pTessB).PerEdgeQty = TessA.PerEdgeQty;

  if (TessA.PerEdgeQty)
  {
    (*pTessB).PerEdgeNb = ut_alloc_1d_int ((*pTessB).PerEdgeQty + 1);
    ut_array_1d_int_memcpy (TessA.PerEdgeNb + 1, (*pTessB).PerEdgeQty,
                            (*pTessB).PerEdgeNb + 1);

    (*pTessB).PerEdgeMaster = ut_alloc_1d_int (TessA.EdgeQty + 1);
    ut_array_1d_int_memcpy (TessA.PerEdgeMaster + 1, TessA.EdgeQty,
                            (*pTessB).PerEdgeMaster + 1);

    (*pTessB).PerEdgeShift = ut_alloc_2d_int (TessA.EdgeQty + 1, 4);
    ut_array_2d_int_memcpy (TessA.PerEdgeShift + 1, TessA.EdgeQty, 3,
                            (*pTessB).PerEdgeShift + 1);

    (*pTessB).PerEdgeOri = ut_alloc_1d_int (TessA.EdgeQty + 1);
    ut_array_1d_int_memcpy (TessA.PerEdgeOri + 1, TessA.EdgeQty,
                            (*pTessB).PerEdgeOri + 1);

    (*pTessB).PerEdgeSlaveQty = ut_alloc_1d_int (TessA.EdgeQty + 1);
    (*pTessB).PerEdgeSlaveNb = ut_alloc_1d_pint (TessA.EdgeQty + 1);

    ut_array_1d_int_memcpy (TessA.PerEdgeSlaveQty + 1, TessA.EdgeQty,
                            (*pTessB).PerEdgeSlaveQty + 1);
    for (i = 1; i <= (*pTessB).EdgeQty; i++)
    {
      (*pTessB).PerEdgeSlaveNb[i] =
        ut_alloc_1d_int ((*pTessB).PerEdgeSlaveQty[i] + 1);
      ut_array_1d_int_memcpy (TessA.PerEdgeSlaveNb[i] + 1,
                              (*pTessB).PerEdgeSlaveQty[i],
                              (*pTessB).PerEdgeSlaveNb[i] + 1);
    }
  }

  (*pTessB).PerFaceQty = TessA.PerFaceQty;

  if (TessA.PerFaceQty)
  {
    (*pTessB).PerFaceNb = ut_alloc_1d_int ((*pTessB).PerFaceQty + 1);
    ut_array_1d_int_memcpy (TessA.PerFaceNb + 1, (*pTessB).PerFaceQty,
                            (*pTessB).PerFaceNb + 1);

    (*pTessB).PerFaceMaster = ut_alloc_1d_int (TessA.FaceQty + 1);
    ut_array_1d_int_memcpy (TessA.PerFaceMaster + 1, TessA.FaceQty,
                            (*pTessB).PerFaceMaster + 1);

    (*pTessB).PerFaceShift = ut_alloc_2d_int (TessA.FaceQty + 1, 4);
    ut_array_2d_int_memcpy (TessA.PerFaceShift + 1, TessA.FaceQty, 3,
                            (*pTessB).PerFaceShift + 1);

    (*pTessB).PerFaceOri = ut_alloc_1d_int (TessA.FaceQty + 1);
    ut_array_1d_int_memcpy (TessA.PerFaceOri + 1, TessA.FaceQty,
                            (*pTessB).PerFaceOri + 1);

    (*pTessB).PerFaceSlaveNb = ut_alloc_1d_int (TessA.FaceQty + 1);

    ut_array_1d_int_memcpy (TessA.PerFaceSlaveNb + 1, TessA.FaceQty,
                            (*pTessB).PerFaceSlaveNb + 1);
  }

  return;
}

void
neut_tess_tess_scale (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).ScaleQty = TessA.ScaleQty;
  if (TessA.ScaleCellId)
  {
    (*pTessB).ScaleCellId =
      ut_alloc_2d_int ((*pTessB).CellQty + 1, (*pTessB).ScaleQty + 1);
    ut_array_2d_int_memcpy (TessA.ScaleCellId + 1, (*pTessB).CellQty,
                            (*pTessB).ScaleQty + 1,
                            (*pTessB).ScaleCellId + 1);
  }

  return;
}
