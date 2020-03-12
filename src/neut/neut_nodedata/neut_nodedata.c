/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_nodedata_.h"

void
neut_nodedata_set_default (struct NODEDATA *pNodeData)
{
  (*pNodeData).NodeQty = 0;

  (*pNodeData).ColData = NULL;
  (*pNodeData).ColDataType = NULL;
  (*pNodeData).ColScheme = NULL;
  (*pNodeData).Col = NULL;

  (*pNodeData).RadData = NULL;
  (*pNodeData).RadDataType = NULL;
  (*pNodeData).Rad = NULL;

  (*pNodeData).Scale = NULL;
  (*pNodeData).ScaleTitle = NULL;

  (*pNodeData).CooData = NULL;
  (*pNodeData).CooDataType = NULL;
  (*pNodeData).Coo = NULL;
  (*pNodeData).CooFact = 1;

  return;
}

void
neut_nodedata_free (struct NODEDATA *pNodeData)
{
  if ((*pNodeData).ColData)
    ut_free_2d (&(*pNodeData).ColData, (*pNodeData).NodeQty + 1);

  ut_free_1d_char (&(*pNodeData).ColDataType);
  ut_free_1d_char (&(*pNodeData).ColScheme);

  if ((*pNodeData).Col)
    ut_free_2d_int (&(*pNodeData).Col, (*pNodeData).NodeQty + 1);

  if ((*pNodeData).RadData)
    ut_free_2d (&(*pNodeData).RadData, (*pNodeData).NodeQty + 1);

  ut_free_1d_char (&(*pNodeData).RadDataType);
  ut_free_1d (&(*pNodeData).Rad);

  ut_free_1d_char (&(*pNodeData).Scale);
  ut_free_1d_char (&(*pNodeData).ScaleTitle);

  if ((*pNodeData).CooData)
    ut_free_2d (&(*pNodeData).CooData, (*pNodeData).NodeQty + 1);

  ut_free_1d_char (&(*pNodeData).CooDataType);

  if ((*pNodeData).Coo)
    ut_free_2d (&(*pNodeData).Coo, (*pNodeData).NodeQty + 1);

  return;
}

void
neut_nodedata_coldatatype_size (struct NODEDATA NodeData, int *psize)
{
  if (!strcmp (NodeData.ColDataType, "col")
      || !strcmp (NodeData.ColDataType, "ori"))
    (*psize) = 3;
  else if (!strcmp (NodeData.ColDataType, "rad")
           || !strcmp (NodeData.ColDataType, "scal"))
    (*psize) = 1;
  else
  {
    ut_print_neperbug ();
    abort ();
  }

  return;
}

void
neut_nodedata_mesh2slice (struct NODEDATA NodeData, struct NODES SNodes,
                          int **node_newold, double *node_fact,
                          struct NODEDATA *pSNodeData)
{
  int i, j, size;

  (*pSNodeData).NodeQty = SNodes.NodeQty;

  if (NodeData.ColDataType)
  {
    neut_nodedata_coldatatype_size (NodeData, &size);

    ut_string_string (NodeData.ColDataType, &((*pSNodeData).ColDataType));

    (*pSNodeData).ColData = ut_alloc_2d ((*pSNodeData).NodeQty + 1, size);

    for (i = 1; i <= (*pSNodeData).NodeQty; i++)
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
