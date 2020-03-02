/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_part_.h"

int
neut_mesh_elt_coms (struct MESH Mesh, int elt, int partqty, int *parttab,
                    int *parttab2, int **coms, int *pcomqty)
{
  int i, j, pos, eltpart, nodepart, comtotalqty;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  comtotalqty = 0;
  (*pcomqty) = 0;
  ut_array_2d_int_zero (coms, partqty, 2);

  eltpart = parttab2[elt];
  for (i = 0; i < eltnodeqty; i++)
  {
    nodepart = parttab[Mesh.EltNodes[elt][i]];

    if (nodepart != eltpart)
    {
      comtotalqty++;
      pos = -1;
      for (j = 0; j < *pcomqty; j++)
        if (coms[j][0] == nodepart)
        {
          pos = j;
          break;
        }

      if (pos != -1)
        coms[pos][1]++;
      else
      {
        coms[(*pcomqty)][0] = nodepart;
        coms[(*pcomqty)][1] = 1;
        (*pcomqty)++;
      }
    }
  }

  ut_array_2d_int_sortbycol_des (coms, *pcomqty, 2, 1);

  return comtotalqty;
}

int
neut_mesh_elt_comqty (struct MESH Mesh, int elt, int *parttab, int *parttab2)
{
  int i, eltpart, nodepart, comtotalqty;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  comtotalqty = 0;
  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  eltpart = parttab2[elt];
  for (i = 0; i < eltnodeqty; i++)
  {
    nodepart = parttab[Mesh.EltNodes[elt][i]];
    if (eltpart != nodepart)
      comtotalqty++;
  }

  return comtotalqty;
}

int
neut_mesh_elt_isbound (struct MESH Mesh, int elt, int *parttab)
{
  int i, res, part;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  res = 0;
  part = parttab[Mesh.EltNodes[elt][0]];
  for (i = 1; i < eltnodeqty; i++)
  {
    if (parttab[Mesh.EltNodes[elt][i]] != part)
    {
      res = 1;
      break;
    }
  }

  return res;
}
