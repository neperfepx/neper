/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_meshdata_.h"

void
neut_meshdata_set_default (struct MESHDATA *pMeshData)
{
  (*pMeshData).EltQty = 0;
  (*pMeshData).Dimension = 0;

  (*pMeshData).ColData = NULL;
  (*pMeshData).ColDataType = NULL;
  (*pMeshData).ColScheme = NULL;
  (*pMeshData).Col = NULL;

  (*pMeshData).RadData = NULL;
  (*pMeshData).RadDataType = NULL;
  (*pMeshData).Rad = NULL;

  (*pMeshData).Scale = NULL;
  (*pMeshData).ScaleTitle = NULL;

  (*pMeshData).BCol = NULL;
  (*pMeshData).BRad = -1;

  return;
}

void
neut_meshdata_free (struct MESHDATA *pMeshData)
{
  ut_free_2d ((*pMeshData).ColData, (*pMeshData).EltQty);
  ut_free_1d_char ((*pMeshData).ColDataType);
  ut_free_1d_char ((*pMeshData).ColScheme);
  ut_free_2d_int ((*pMeshData).Col, (*pMeshData).EltQty);

  ut_free_2d ((*pMeshData).RadData, (*pMeshData).EltQty);
  ut_free_1d_char ((*pMeshData).RadDataType);
  ut_free_1d ((*pMeshData).Rad);

  ut_free_1d_char ((*pMeshData).Scale);
  ut_free_1d_char ((*pMeshData).ScaleTitle);

  ut_free_1d_int ((*pMeshData).BCol);
  (*pMeshData).BRad = 0;

  (*pMeshData).EltQty = 0;
  (*pMeshData).Dimension = 0;

  return;
}

void
neut_meshdata_coldatatype_size (struct MESHDATA MeshData, int *psize)
{
  if (!strcmp (MeshData.ColDataType, "id"))
    (*psize) = 1;
  else if (!strcmp (MeshData.ColDataType, "col")
	   || !strcmp (MeshData.ColDataType, "orie")
	   || !strcmp (MeshData.ColDataType, "orier")
	   || !strcmp (MeshData.ColDataType, "oriek")
	   || !strcmp (MeshData.ColDataType, "oriR"))
    (*psize) = 3;
  else if (!strcmp (MeshData.ColDataType, "oriq"))
    (*psize) = 4;
  else if (!strcmp (MeshData.ColDataType, "orim"))
    (*psize) = 6;
  else if (!strcmp (MeshData.ColDataType, "orig"))
    (*psize) = 9;
  else if (!strcmp (MeshData.ColDataType, "rad")
	   || !strcmp (MeshData.ColDataType, "scal"))
    (*psize) = 1;
  else
    (*psize) = -1;

  return;
}

void
neut_meshdata_mesh2slice (struct MESHDATA *MeshData,
			  struct MESH SMesh, int *elt_newold,
			  struct MESHDATA **pSMeshData)
{
  int i, j, size;

  (*pSMeshData)[0].EltQty = 0;
  (*pSMeshData)[1].EltQty = 0;
  (*pSMeshData)[2].EltQty = SMesh.EltQty;
  (*pSMeshData)[3].EltQty = 0;

  // Mesh
  if (MeshData[3].ColDataType)
  {
    ut_string_string (MeshData[3].ColDataType,
		      &((*pSMeshData)[2].ColDataType));

    neut_meshdata_coldatatype_size ((*pSMeshData)[2], &size);

    if (size > 0)
    {
      (*pSMeshData)[2].ColData = ut_alloc_2d ((*pSMeshData)[2].EltQty + 1,
					      size);

      for (i = 1; i <= (*pSMeshData)[2].EltQty; i++)
	for (j = 0; j < size; j++)
	  (*pSMeshData)[2].ColData[i][j] =
	    MeshData[3].ColData[elt_newold[i]][j];
    }
  }

  ut_string_string (MeshData[3].Scale, &((*pSMeshData)[2].Scale));
  ut_string_string (MeshData[3].ScaleTitle, &((*pSMeshData)[2].ScaleTitle));
  ut_string_string (MeshData[3].ColScheme, &((*pSMeshData)[2].ColScheme));

  return;
}
