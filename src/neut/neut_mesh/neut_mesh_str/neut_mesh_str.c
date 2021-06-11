/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_str_.h"

void
neut_mesh_str (int dim, int *size, struct NODES *pNodes, struct MESH *pMesh,
               struct NSET *pNSet)
{
  if (dim == 3)
    neut_mesh_str_3d (size, pNodes, pMesh, pNSet);
  else if (dim == 2)
    neut_mesh_str_2d (size, pNodes, pMesh, pNSet);
  else if (dim == 1)
    neut_mesh_str_1d (size, pNodes, pMesh, pNSet);
  else
    ut_print_neperbug ();

  return;
}

void
neut_mesh_str_3d (int *size, struct NODES *pNodes, struct MESH *pMesh,
                  struct NSET *pNSet2D)
{
  int i, x, y, z, nid, eid, nodesperelt;
  double nx, ny, nz;

  /* General ------------------------------------------------------- */

  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 3;
  (*pMesh).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh).EltType, "quad");

  nodesperelt =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                      (*pMesh).EltOrder);

  (*pMesh).EltQty = size[0] * size[1] * size[2];

  /* Calculating NodeCoo and NodeQty ------------------------------- */

  int ***NMap = NULL;
  NMap = ut_alloc_3d_int (size[0] + 2, size[1] + 2, size[2] + 2);

  (*pNodes).NodeQty = (size[0] + 1) * (size[1] + 1) * (size[2] + 1);
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  /* Tessellation --- */
  nid = 0;

  for (z = 1; z <= size[2] + 1; z++)
  {
    nz = ((double) (z - 1)) / ((double) (size[2]));

    for (y = 1; y <= size[1] + 1; y++)
    {
      ny = ((double) (y - 1)) / ((double) (size[1]));

      for (x = 1; x <= size[0] + 1; x++)
      {
        nid++;
        nx = ((double) (x - 1)) / ((double) (size[0]));

        /* Map */
        NMap[x][y][z] = nid;

        /* NodeCoo */
        (*pNodes).NodeCoo[nid][0] = nx;
        (*pNodes).NodeCoo[nid][1] = ny;
        (*pNodes).NodeCoo[nid][2] = nz;
      }
    }
  }

  /* Connectivity */

  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, nodesperelt);

  eid = 0;

  for (z = 1; z <= size[2]; z++)
    for (y = 1; y <= size[1]; y++)
      for (x = 1; x <= size[0]; x++)
      {
        eid++;

        (*pMesh).EltNodes[eid][0] = NMap[x][y][z];
        (*pMesh).EltNodes[eid][1] = NMap[x + 1][y][z];
        (*pMesh).EltNodes[eid][2] = NMap[x + 1][y + 1][z];
        (*pMesh).EltNodes[eid][3] = NMap[x][y + 1][z];
        (*pMesh).EltNodes[eid][4] = NMap[x][y][z + 1];
        (*pMesh).EltNodes[eid][5] = NMap[x + 1][y][z + 1];
        (*pMesh).EltNodes[eid][6] = NMap[x + 1][y + 1][z + 1];
        (*pMesh).EltNodes[eid][7] = NMap[x][y + 1][z + 1];
      }

  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  ut_free_3d_int (&NMap, size[0] + 2, size[1] + 2);

  // Searching nsets

  int face, coo;
  int *tmp = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  double *domain = ut_alloc_1d (6);
  double eps = 1e-12;

  neut_nodes_bbox_vect ((*pNodes), domain);

  for (i = 0; i < 3; i++)
  {
    domain[2 * i] += eps;
    domain[2 * i + 1] -= eps;
  }

  if (pNSet2D)
  {
    (*pNSet2D).qty = 6;
    (*pNSet2D).names = ut_alloc_2d_char (7, 10);
    (*pNSet2D).NodeQty = ut_alloc_1d_int ((*pNSet2D).qty + 1);
    (*pNSet2D).nodes = ut_alloc_1d_pint ((*pNSet2D).qty + 1);

    strcpy ((*pNSet2D).names[1], "x0");
    strcpy ((*pNSet2D).names[2], "x1");
    strcpy ((*pNSet2D).names[3], "y0");
    strcpy ((*pNSet2D).names[4], "y1");
    strcpy ((*pNSet2D).names[5], "z0");
    strcpy ((*pNSet2D).names[6], "z1");

    for (face = 1; face <= 6; face++)
    {
      coo = (face - 1) / 2;

      tmp[0] = 0;
      if (face % 2 == 1)
      {
        for (i = 1; i <= (*pNodes).NodeQty; i++)
          if ((*pNodes).NodeCoo[i][coo] < domain[face - 1])
            tmp[++tmp[0]] = i;
      }
      else
      {
        for (i = 1; i <= (*pNodes).NodeQty; i++)
          if ((*pNodes).NodeCoo[i][coo] > domain[face - 1])
            tmp[++tmp[0]] = i;
      }

      (*pNSet2D).NodeQty[face] = tmp[0];
      (*pNSet2D).nodes[face] = ut_alloc_1d_int ((*pNSet2D).NodeQty[face]);
      ut_array_1d_int_memcpy (tmp + 1, tmp[0], (*pNSet2D).nodes[face]);
    }
  }

  return;
}

void
neut_mesh_str_2d (int *size, struct NODES *pNodes, struct MESH *pMesh,
                  struct NSET *pNSet1D)
{
  int i, x, y, nid, eid, nodesperelt;
  double nx, ny;

  /* General ------------------------------------------------------- */

  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 2;
  ut_string_string ("quad", &((*pMesh).EltType));

  nodesperelt =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                      (*pMesh).EltOrder);

  (*pMesh).EltQty = size[0] * size[1];

  /* Calculating NodeCoo and NodeQty ------------------------------- */

  int **NMap = NULL;
  NMap = ut_alloc_2d_int (size[0] + 2, size[1] + 2);

  (*pNodes).NodeQty = (size[0] + 1) * (size[1] + 1);
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  /* Tessellation --- */
  nid = 0;

  for (y = 1; y <= size[1] + 1; y++)
  {
    ny = ((double) (y - 1)) / ((double) (size[1]));

    for (x = 1; x <= size[0] + 1; x++)
    {
      nid++;
      nx = ((double) (x - 1)) / ((double) (size[0]));

      /* Map */
      NMap[x][y] = nid;

      /* NodeCoo */
      (*pNodes).NodeCoo[nid][0] = nx;
      (*pNodes).NodeCoo[nid][1] = ny;
    }
  }

  /* Connectivity */

  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, nodesperelt);

  eid = 0;

  for (y = 1; y <= size[1]; y++)
    for (x = 1; x <= size[0]; x++)
    {
      eid++;

      (*pMesh).EltNodes[eid][0] = NMap[x][y];
      (*pMesh).EltNodes[eid][1] = NMap[x + 1][y];
      (*pMesh).EltNodes[eid][2] = NMap[x + 1][y + 1];
      (*pMesh).EltNodes[eid][3] = NMap[x][y + 1];
    }

  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  ut_free_2d_int (&NMap, size[0] + 2);

  // Searching nsets

  int edge, coo;
  int *tmp = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  double *domain = ut_alloc_1d (6);
  double eps = 1e-12;

  neut_nodes_bbox_vect ((*pNodes), domain);

  for (i = 0; i < 2; i++)
  {
    domain[2 * i] += eps;
    domain[2 * i + 1] -= eps;
  }

  if (pNSet1D)
  {
    (*pNSet1D).qty = 4;
    (*pNSet1D).names = ut_alloc_2d_char (5, 10);
    (*pNSet1D).NodeQty = ut_alloc_1d_int ((*pNSet1D).qty + 1);
    (*pNSet1D).nodes = ut_alloc_1d_pint ((*pNSet1D).qty + 1);

    strcpy ((*pNSet1D).names[1], "x0");
    strcpy ((*pNSet1D).names[2], "x1");
    strcpy ((*pNSet1D).names[3], "y0");
    strcpy ((*pNSet1D).names[4], "y1");

    for (edge = 1; edge <= 4; edge++)
    {
      coo = (edge - 1) / 2;

      tmp[0] = 0;
      if (edge % 2 == 1)
      {
        for (i = 1; i <= (*pNodes).NodeQty; i++)
          if ((*pNodes).NodeCoo[i][coo] < domain[edge - 1])
            tmp[++tmp[0]] = i;
      }
      else
      {
        for (i = 1; i <= (*pNodes).NodeQty; i++)
          if ((*pNodes).NodeCoo[i][coo] > domain[edge - 1])
            tmp[++tmp[0]] = i;
      }

      (*pNSet1D).NodeQty[edge] = tmp[0];
      (*pNSet1D).nodes[edge] = ut_alloc_1d_int ((*pNSet1D).NodeQty[edge]);
      ut_array_1d_int_memcpy (tmp + 1, tmp[0], (*pNSet1D).nodes[edge]);
    }
  }

  return;
}

void
neut_mesh_str_1d (int *size, struct NODES *pNodes, struct MESH *pMesh,
                  struct NSET *pNSet1D)
{
  int i, x, nid, eid, nodesperelt;
  double nx;

  /* General ------------------------------------------------------- */

  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 1;
  ut_string_string ("quad", &((*pMesh).EltType));

  nodesperelt =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                      (*pMesh).EltOrder);

  (*pMesh).EltQty = size[0];

  /* Calculating NodeCoo and NodeQty ------------------------------- */

  int *NMap = NULL;
  NMap = ut_alloc_1d_int (size[0] + 2);

  (*pNodes).NodeQty = size[0] + 1;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  /* Tessellation --- */
  nid = 0;

  for (x = 1; x <= size[0] + 1; x++)
  {
    nid++;
    nx = ((double) (x - 1)) / ((double) (size[0]));

    /* Map */
    NMap[x] = nid;

    /* NodeCoo */
    (*pNodes).NodeCoo[nid][0] = nx;
  }

  /* Connectivity */

  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, nodesperelt);

  eid = 0;

  for (x = 1; x <= size[0]; x++)
  {
    eid++;

    (*pMesh).EltNodes[eid][0] = NMap[x];
    (*pMesh).EltNodes[eid][1] = NMap[x + 1];
  }

  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  ut_free_1d_int (&NMap);

  // Searching nsets

  int ver;
  int *tmp = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  double *domain = ut_alloc_1d (6);
  double eps = 1e-12;

  neut_nodes_bbox_vect ((*pNodes), domain);

  domain[0] += eps;
  domain[1] -= eps;

  if (pNSet1D)
  {
    (*pNSet1D).qty = 2;
    (*pNSet1D).names = ut_alloc_2d_char (3, 10);
    (*pNSet1D).NodeQty = ut_alloc_1d_int ((*pNSet1D).qty + 1);
    (*pNSet1D).nodes = ut_alloc_1d_pint ((*pNSet1D).qty + 1);

    strcpy ((*pNSet1D).names[1], "x0");
    strcpy ((*pNSet1D).names[2], "x1");

    for (ver = 1; ver <= 2; ver++)
    {
      tmp[0] = 0;
      if (ver % 2 == 1)
      {
        for (i = 1; i <= (*pNodes).NodeQty; i++)
          if ((*pNodes).NodeCoo[i][0] < domain[ver - 1])
            tmp[++tmp[0]] = i;
      }
      else
      {
        for (i = 1; i <= (*pNodes).NodeQty; i++)
          if ((*pNodes).NodeCoo[i][0] > domain[ver - 1])
            tmp[++tmp[0]] = i;
      }

      (*pNSet1D).NodeQty[ver] = tmp[0];
      (*pNSet1D).nodes[ver] = ut_alloc_1d_int ((*pNSet1D).NodeQty[ver]);
      ut_array_1d_int_memcpy (tmp + 1, tmp[0], (*pNSet1D).nodes[ver]);
    }
  }

  return;
}
