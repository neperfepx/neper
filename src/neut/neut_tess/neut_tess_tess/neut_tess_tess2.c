/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_.h"

void
neut_tess_tess_gen (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).Dim = TessA.Dim;
  (*pTessB).PseudoDim = TessA.PseudoDim;
  (*pTessB).Level = TessA.Level;
  (*pTessB).TessId = TessA.TessId;

  if (TessA.Type)
    ut_string_string (TessA.Type, &((*pTessB).Type));

  if (TessA.Periodic)
  {
    (*pTessB).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy ((*pTessB).Periodic, 3, TessA.Periodic);
  }

  if (TessA.PeriodicDist)
  {
    (*pTessB).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy ((*pTessB).PeriodicDist, 3, TessA.PeriodicDist);
  }

  return;
}

void
neut_tess_tess_cell (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).CellQty = TessA.CellQty;

  if (TessA.Type)
    ut_string_string (TessA.Type, &((*pTessB).Type));

  if (TessA.CellId)
  {
    (*pTessB).CellId = ut_alloc_1d_int (TessA.CellQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).CellId + 1, TessA.CellQty,
			    TessA.CellId + 1);
  }

  if (TessA.CellOri)
  {
    (*pTessB).CellOri = ut_alloc_2d (TessA.CellQty + 1, 4);
    ut_array_2d_memcpy ((*pTessB).CellOri + 1, TessA.CellQty, 4,
			TessA.CellOri + 1);
  }

  if (TessA.CellCrySym)
    ut_string_string (TessA.CellCrySym, &((*pTessB).CellCrySym));

  (*pTessB).ScaleQty = TessA.ScaleQty;

  if (TessA.ScaleQty > 0)
  {
    (*pTessB).ScaleQty = TessA.ScaleQty;
    (*pTessB).ScaleCellId = ut_alloc_2d_int (TessA.CellQty + 1, TessA.ScaleQty);
    ut_array_2d_int_memcpy ((*pTessB).ScaleCellId + 1,
			    TessA.CellQty, TessA.ScaleQty, TessA.ScaleCellId + 1);
  }

  neut_tess_tess_celltrue (TessA, pTessB);
  neut_tess_tess_cellbody (TessA, pTessB);
  neut_tess_tess_celllamid (TessA, pTessB);
  neut_tess_tess_cellmodeid (TessA, pTessB);

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
    ut_array_1d_int_memcpy ((*pTessB).PerSeedMaster + 1,
			    (*pTessB).SeedQty, TessA.PerSeedMaster + 1);
  }

  if (TessA.PerSeedShift)
  {
    (*pTessB).PerSeedShift = ut_alloc_2d_int ((*pTessB).SeedQty + 1, 3);
    ut_array_2d_int_memcpy ((*pTessB).PerSeedShift + 1,
			    (*pTessB).SeedQty, 3, TessA.PerSeedShift + 1);
  }

  if (TessA.PerSeedSlave)
  {
    (*pTessB).PerSeedSlave = ut_alloc_2d_int ((*pTessB).SeedQty + 1, 27);
    ut_array_2d_int_memcpy ((*pTessB).PerSeedSlave,
			    (*pTessB).SeedQty, 27, TessA.PerSeedSlave);
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
    ut_array_1d_int_memcpy ((*pTessB).PerVerNb + 1,
			    (*pTessB).PerVerQty, TessA.PerVerNb + 1);

    (*pTessB).PerVerMaster = ut_alloc_1d_int (TessA.VerQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerVerMaster + 1, TessA.VerQty,
			    TessA.PerVerMaster + 1);

    (*pTessB).PerVerShift = ut_alloc_2d_int (TessA.VerQty + 1, 4);
    ut_array_2d_int_memcpy ((*pTessB).PerVerShift + 1, TessA.VerQty,
			    3, TessA.PerVerShift + 1);

    (*pTessB).PerVerSlaveQty = ut_alloc_1d_int (TessA.VerQty + 1);
    (*pTessB).PerVerSlaveNb = ut_alloc_1d_pint (TessA.VerQty + 1);

    ut_array_1d_int_memcpy ((*pTessB).PerVerSlaveQty + 1,
			    TessA.VerQty, TessA.PerVerSlaveQty + 1);
    for (i = 1; i <= (*pTessB).PerVerQty; i++)
    {
      (*pTessB).PerVerSlaveNb[i]
	= ut_alloc_1d_int ((*pTessB).PerVerSlaveQty[i] + 1);
      ut_array_1d_int_memcpy ((*pTessB).PerVerSlaveNb[i] + 1,
			      (*pTessB).PerVerSlaveQty[i],
			      TessA.PerVerSlaveNb[i] + 1);
    }
  }

  (*pTessB).PerEdgeQty = TessA.PerEdgeQty;

  if (TessA.PerEdgeQty)
  {
    (*pTessB).PerEdgeNb = ut_alloc_1d_int ((*pTessB).PerEdgeQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerEdgeNb + 1,
			    (*pTessB).PerEdgeQty, TessA.PerEdgeNb + 1);

    (*pTessB).PerEdgeMaster = ut_alloc_1d_int (TessA.EdgeQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerEdgeMaster + 1, TessA.EdgeQty,
			    TessA.PerEdgeMaster + 1);

    (*pTessB).PerEdgeShift = ut_alloc_2d_int (TessA.EdgeQty + 1, 4);
    ut_array_2d_int_memcpy ((*pTessB).PerEdgeShift + 1, TessA.EdgeQty,
			    3, TessA.PerEdgeShift + 1);

    (*pTessB).PerEdgeOri = ut_alloc_1d_int (TessA.EdgeQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerEdgeOri + 1, TessA.EdgeQty,
			    TessA.PerEdgeOri + 1);

    (*pTessB).PerEdgeSlaveQty = ut_alloc_1d_int (TessA.EdgeQty + 1);
    (*pTessB).PerEdgeSlaveNb = ut_alloc_1d_pint (TessA.EdgeQty + 1);

    ut_array_1d_int_memcpy ((*pTessB).PerEdgeSlaveQty + 1,
			    TessA.EdgeQty, TessA.PerEdgeSlaveQty + 1);
    for (i = 1; i <= (*pTessB).PerEdgeQty; i++)
    {
      (*pTessB).PerEdgeSlaveNb[i]
	= ut_alloc_1d_int ((*pTessB).PerEdgeSlaveQty[i] + 1);
      ut_array_1d_int_memcpy ((*pTessB).PerEdgeSlaveNb[i] + 1,
			      (*pTessB).PerEdgeSlaveQty[i],
			      TessA.PerEdgeSlaveNb[i] + 1);
    }
  }

  (*pTessB).PerFaceQty = TessA.PerFaceQty;

  if (TessA.PerFaceQty)
  {
    (*pTessB).PerFaceNb = ut_alloc_1d_int ((*pTessB).PerFaceQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerFaceNb + 1,
			    (*pTessB).PerFaceQty, TessA.PerFaceNb + 1);

    (*pTessB).PerFaceMaster = ut_alloc_1d_int (TessA.FaceQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerFaceMaster + 1, TessA.FaceQty,
			    TessA.PerFaceMaster + 1);

    (*pTessB).PerFaceShift = ut_alloc_2d_int (TessA.FaceQty + 1, 4);
    ut_array_2d_int_memcpy ((*pTessB).PerFaceShift + 1, TessA.FaceQty,
			    3, TessA.PerFaceShift + 1);

    (*pTessB).PerFaceOri = ut_alloc_1d_int (TessA.FaceQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PerFaceOri + 1, TessA.FaceQty,
			    TessA.PerFaceOri + 1);

    (*pTessB).PerFaceSlaveNb = ut_alloc_1d_int (TessA.FaceQty + 1);

    ut_array_1d_int_memcpy ((*pTessB).PerFaceSlaveNb + 1,
			    TessA.FaceQty, TessA.PerFaceSlaveNb + 1);
  }

  return;
}

void
neut_tess_tess_scale (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).ScaleQty = TessA.ScaleQty;
  if (TessA.ScaleCellId)
  {
    (*pTessB).ScaleCellId = ut_alloc_2d_int ((*pTessB).CellQty + 1,
	(*pTessB).ScaleQty + 1);
    ut_array_2d_int_memcpy ((*pTessB).ScaleCellId + 1,
			    (*pTessB).CellQty, (*pTessB).ScaleQty + 1,
			    TessA.ScaleCellId + 1);
  }

  return;
}
