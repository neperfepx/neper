/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_op_.h"

void
neut_data_set_default (struct DATA *pData)
{
  (*pData).Qty = 0;
  (*pData).Dim= 0;
  (*pData).type= NULL;

  (*pData).ColData = NULL;
  (*pData).ColDataType = NULL;
  (*pData).ColDataSize = 0;
  (*pData).ColDataName = NULL;
  (*pData).CooDataName = NULL;
  (*pData).ColScheme = NULL;
  (*pData).Col = NULL;
  (*pData).ColDataDef = NULL;

  (*pData).TrsData = NULL;
  (*pData).TrsDataType = NULL;
  (*pData).TrsScheme = NULL;
  (*pData).Trs = NULL;

  (*pData).RadData = NULL;
  (*pData).RadDataType = NULL;
  (*pData).Rad = NULL;
  (*pData).RadDataName = NULL;

  (*pData).CooData = NULL;
  (*pData).CooDataType = NULL;
  (*pData).Coo = NULL;
  (*pData).CooFact = 1;

  (*pData).LengthData = NULL;
  (*pData).LengthDataType = NULL;
  (*pData).Length = NULL;

  (*pData).BCol = NULL;
  (*pData).BRad = -1;

  (*pData).VoidCol = NULL;

  (*pData).Scale = NULL;
  (*pData).ScaleTitle = NULL;

  (*pData).Axes = NULL;
  (*pData).Space = NULL;

  (*pData).Label = NULL;
  (*pData).FontSize = -1;

  return;
}

void
neut_data_free (struct DATA *pData)
{
  ut_free_2d (&(*pData).ColData, (*pData).Qty);
  ut_free_1d_char (&(*pData).ColDataType);
  ut_free_1d_char (&(*pData).CooDataName);
  ut_free_1d_char (&(*pData).ColScheme);
  ut_free_2d_int (&(*pData).Col, (*pData).Qty);
  ut_free_1d_int (&(*pData).ColDataDef);

  ut_free_2d (&(*pData).TrsData, (*pData).Qty);
  ut_free_1d_char (&(*pData).TrsDataType);
  ut_free_1d_char (&(*pData).TrsScheme);
  ut_free_1d (&(*pData).Trs);

  ut_free_2d (&(*pData).RadData, (*pData).Qty);
  ut_free_1d_char (&(*pData).RadDataType);
  ut_free_1d (&(*pData).Rad);

  ut_free_2d (&(*pData).CooData, (*pData).Qty);
  ut_free_1d_char (&(*pData).CooDataType);
  ut_free_2d (&(*pData).Coo, (*pData).Qty);
  (*pData).CooFact = 1;
  ut_free_1d_char (&(*pData).CooDataName);

  ut_free_2d (&(*pData).LengthData, (*pData).Qty);
  ut_free_1d_char (&(*pData).LengthDataType);
  ut_free_1d (&(*pData).Length);

  ut_free_1d_int (&(*pData).BCol);
  (*pData).BRad = 0;

  ut_free_1d_int (&(*pData).VoidCol);

  ut_free_1d_char (&(*pData).Scale);
  ut_free_1d_char (&(*pData).ScaleTitle);

  ut_free_1d (&(*pData).Axes);
  ut_free_1d_char (&(*pData).Space);

  ut_free_2d_char (&(*pData).Label, 3);

  (*pData).FontSize = 0;

  (*pData).Qty = 0;
  (*pData).Dim = 0;

  return;
}

void
neut_data_mesh2slice_elts (struct DATA *Data, struct MESH SMesh,
                           int *elt_newold, struct DATA **pSData)
{
  int i, j, size;

  (*pSData)[0].Qty = 0;
  (*pSData)[1].Qty = 0;
  (*pSData)[2].Qty = SMesh.EltQty;
  (*pSData)[3].Qty = 0;

  // Mesh
  if (Data[3].ColDataType)
  {
    ut_string_string (Data[3].ColDataType,
                      &((*pSData)[2].ColDataType));

    neut_data_coldatatype_size ((*pSData)[2], &size);

    if (size > 0)
    {
      (*pSData)[2].ColData =
        ut_alloc_2d ((*pSData)[2].Qty + 1, size);

      for (i = 1; i <= (*pSData)[2].Qty; i++)
        for (j = 0; j < size; j++)
          (*pSData)[2].ColData[i][j] =
            Data[3].ColData[elt_newold[i]][j];
    }
  }

  ut_string_string (Data[3].Scale, &((*pSData)[2].Scale));
  ut_string_string (Data[3].ScaleTitle, &((*pSData)[2].ScaleTitle));
  ut_string_string (Data[3].ColScheme, &((*pSData)[2].ColScheme));

  return;
}

void
neut_data_mesh2slice_nodes (struct DATA NodeData, struct NODES SNodes,
                            int **node_newold, double *node_fact,
                            struct DATA *pSNodeData)
{
  int i, j, size;

  (*pSNodeData).Qty = SNodes.NodeQty;

  if (NodeData.ColDataType)
  {
    if (!strcmp (NodeData.ColDataType, "col")
        || !strcmp (NodeData.ColDataType, "ori"))
      size = 3;
    else if (!strcmp (NodeData.ColDataType, "rad")
             || !strcmp (NodeData.ColDataType, "real"))
      size = 1;
    else if (!strcmp (NodeData.ColDataType, "coo"))
      return;
    else
    {
      ut_print_neperbug ();
      abort (); // avoids compilation warning
    }

    ut_string_string (NodeData.ColDataType, &((*pSNodeData).ColDataType));

    (*pSNodeData).ColData = ut_alloc_2d ((*pSNodeData).Qty + 1, size);

    for (i = 1; i <= (*pSNodeData).Qty; i++)
      for (j = 0; j < 3; j++)
      {
        (*pSNodeData).ColData[i][j] =
          (1 - node_fact[i]) * NodeData.ColData[node_newold[i][0]][j] +
          node_fact[i] * NodeData.ColData[node_newold[i][1]][j];
      }
  }

  ut_string_string (NodeData.Scale, &((*pSNodeData).Scale));
  ut_string_string (NodeData.ScaleTitle, &((*pSNodeData).ScaleTitle));
  ut_string_string (NodeData.ColScheme, &((*pSNodeData).ColScheme));

  return;
}
