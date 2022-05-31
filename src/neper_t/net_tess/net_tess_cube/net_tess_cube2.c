/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_cube_.h"

void
net_tess_cube_ids (int *N, int ***verid, int ****edgeid, int ****faceid,
                   int ***polyid)
{
  int i, j, k, id;

  id = 0;
  for (k = 0; k <= N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i <= N[0]; i++)
        verid[i][j][k] = ++id;

  id = 0;
  for (k = 0; k <= N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i < N[0]; i++)
        edgeid[0][i][j][k] = ++id;

  for (k = 0; k <= N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i <= N[0]; i++)
        edgeid[1][i][j][k] = ++id;

  for (k = 0; k < N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i <= N[0]; i++)
        edgeid[2][i][j][k] = ++id;

  id = 0;
  for (k = 0; k < N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i <= N[0]; i++)
        faceid[0][i][j][k] = ++id;

  for (k = 0; k < N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i < N[0]; i++)
        faceid[1][i][j][k] = ++id;

  for (k = 0; k <= N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i < N[0]; i++)
        faceid[2][i][j][k] = ++id;

  id = 0;
  for (k = 0; k < N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i < N[0]; i++)
        polyid[i][j][k] = ++id;

  return;
}

void
net_tess_cube_general (int TessId, struct TESS *pTess)
{
  (*pTess).Dim = 3;
  (*pTess).Level = 1;
  (*pTess).TessId = TessId;
  ut_string_string ("standard", &((*pTess).Type));

  return;
}

void
net_tess_cube_cells (int *N, struct TESS *pTess)
{
  // Cells
  (*pTess).CellQty = N[0] * N[1] * N[2];

  return;
}

void
net_tess_cube_sset (struct TESS Tess, struct SEEDSET *pSSet)
{
  int i;

  (*pSSet).Dim = Tess.Dim;
  (*pSSet).N = Tess.CellQty;
  (*pSSet).Nall = (*pSSet).N;
  (*pSSet).SeedCoo = ut_alloc_2d ((*pSSet).N + 1, 3);
  (*pSSet).SeedOri = ut_alloc_2d ((*pSSet).N + 1, 4);
  for (i = 1; i <= Tess.CellQty; i++)
    neut_tess_cell_centroid (Tess, i, (*pSSet).SeedCoo[i]);

  return;
}

void
net_tess_cube_vers (int *N, double **bbox, int ***verid, struct TESS *pTess)
{
  int i, j, k, id;
  double *size = ut_alloc_1d (3);
  double *origin = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
  {
    size[i] = bbox[i][1] - bbox[i][0];
    origin[i] = bbox[i][0];
  }

  (*pTess).VerQty = (N[0] + 1) * (N[1] + 1) * (N[2] + 1);
  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  for (k = 0; k <= N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i <= N[0]; i++)
      {
        id = verid[i][j][k];
        (*pTess).VerCoo[id][0] = (double) i / N[0] * size[0] + origin[0];
        (*pTess).VerCoo[id][1] = (double) j / N[1] * size[1] + origin[1];
        (*pTess).VerCoo[id][2] = (double) k / N[2] * size[2] + origin[2];
      }

  ut_free_1d (&size);
  ut_free_1d (&origin);

  return;
}

void
net_tess_cube_edges (int *N, int ***verid, int ****edgeid, struct TESS *pTess)
{
  int i, j, k, id;

  // Edges
  (*pTess).EdgeQty =
    (N[0] + 1) * (N[1] + 1) * (N[2]) + (N[0] + 1) * (N[1]) * (N[2] + 1) +
    (N[0]) * (N[1] + 1) * (N[2] + 1);

  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 3);
  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  for (k = 0; k <= N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i < N[0]; i++)
      {
        id = edgeid[0][i][j][k];
        (*pTess).EdgeVerNb[id][0] = verid[i + 1][j][k];
        (*pTess).EdgeVerNb[id][1] = verid[i][j][k];
      }

  for (k = 0; k <= N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i <= N[0]; i++)
      {
        id = edgeid[1][i][j][k];
        (*pTess).EdgeVerNb[id][0] = verid[i][j + 1][k];
        (*pTess).EdgeVerNb[id][1] = verid[i][j][k];
      }

  for (k = 0; k < N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i <= N[0]; i++)
      {
        id = edgeid[2][i][j][k];
        (*pTess).EdgeVerNb[id][0] = verid[i][j][k + 1];
        (*pTess).EdgeVerNb[id][1] = verid[i][j][k];
      }

  neut_tess_init_edgelength (pTess);

  return;
}

void
net_tess_cube_faces (int *N, int ***verid, int ****edgeid, int ****faceid,
                     struct TESS *pTess)
{
  int i, j, k, id;

  // Faces
  (*pTess).FaceQty =
    (N[0] + 1) * (N[1]) * (N[2]) + (N[0]) * (N[1] + 1) * (N[2]) +
    (N[0]) * (N[1]) * (N[2] + 1);
  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_2d_int ((*pTess).FaceQty + 1, 5);
  (*pTess).FaceEdgeNb = ut_alloc_2d_int ((*pTess).FaceQty + 1, 5);
  (*pTess).FaceEdgeOri = ut_alloc_2d_int ((*pTess).FaceQty + 1, 5);
  (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);

  for (k = 0; k < N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i <= N[0]; i++)
      {
        id = faceid[0][i][j][k];
        (*pTess).FaceVerQty[id] = 4;
        (*pTess).FaceVerNb[id][1] = verid[i][j + 1][k];
        (*pTess).FaceVerNb[id][2] = verid[i][j + 1][k + 1];
        (*pTess).FaceVerNb[id][3] = verid[i][j][k + 1];
        (*pTess).FaceVerNb[id][4] = verid[i][j][k];
        (*pTess).FaceEdgeNb[id][1] = edgeid[1][i][j][k];
        (*pTess).FaceEdgeNb[id][2] = edgeid[2][i][j + 1][k];
        (*pTess).FaceEdgeNb[id][3] = edgeid[1][i][j][k + 1];
        (*pTess).FaceEdgeNb[id][4] = edgeid[2][i][j][k];
        (*pTess).FaceEdgeOri[id][1] = -1;
        (*pTess).FaceEdgeOri[id][2] = -1;
        (*pTess).FaceEdgeOri[id][3] = 1;
        (*pTess).FaceEdgeOri[id][4] = 1;
        (*pTess).FaceEq[id][0] =
          (*pTess).VerCoo[(*pTess).FaceVerNb[id][1]][0];
        (*pTess).FaceEq[id][1] = 1;
      }

  for (k = 0; k < N[2]; k++)
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i < N[0]; i++)
      {
        id = faceid[1][i][j][k];
        (*pTess).FaceVerQty[id] = 4;
        (*pTess).FaceVerNb[id][1] = verid[i][j][k + 1];
        (*pTess).FaceVerNb[id][2] = verid[i + 1][j][k + 1];
        (*pTess).FaceVerNb[id][3] = verid[i + 1][j][k];
        (*pTess).FaceVerNb[id][4] = verid[i][j][k];
        (*pTess).FaceEdgeNb[id][1] = edgeid[2][i][j][k];
        (*pTess).FaceEdgeNb[id][2] = edgeid[0][i][j][k + 1];
        (*pTess).FaceEdgeNb[id][3] = edgeid[2][i + 1][j][k];
        (*pTess).FaceEdgeNb[id][4] = edgeid[0][i][j][k];
        (*pTess).FaceEdgeOri[id][1] = -1;
        (*pTess).FaceEdgeOri[id][2] = -1;
        (*pTess).FaceEdgeOri[id][3] = 1;
        (*pTess).FaceEdgeOri[id][4] = 1;
        (*pTess).FaceEq[id][0] =
          (*pTess).VerCoo[(*pTess).FaceVerNb[id][1]][1];
        (*pTess).FaceEq[id][2] = 1;
      }

  for (k = 0; k <= N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i < N[0]; i++)
      {
        id = faceid[2][i][j][k];
        (*pTess).FaceVerQty[id] = 4;
        (*pTess).FaceVerNb[id][1] = verid[i + 1][j][k];
        (*pTess).FaceVerNb[id][2] = verid[i + 1][j + 1][k];
        (*pTess).FaceVerNb[id][3] = verid[i][j + 1][k];
        (*pTess).FaceVerNb[id][4] = verid[i][j][k];
        (*pTess).FaceEdgeNb[id][1] = edgeid[0][i][j][k];
        (*pTess).FaceEdgeNb[id][2] = edgeid[1][i + 1][j][k];
        (*pTess).FaceEdgeNb[id][3] = edgeid[0][i][j + 1][k];
        (*pTess).FaceEdgeNb[id][4] = edgeid[1][i][j][k];
        (*pTess).FaceEdgeOri[id][1] = -1;
        (*pTess).FaceEdgeOri[id][2] = -1;
        (*pTess).FaceEdgeOri[id][3] = 1;
        (*pTess).FaceEdgeOri[id][4] = 1;
        (*pTess).FaceEq[id][0] =
          (*pTess).VerCoo[(*pTess).FaceVerNb[id][1]][2];
        (*pTess).FaceEq[id][3] = 1;
      }

  return;
}

void
net_tess_cube_polys (int *N, int ****faceid, int ***polyid,
                     struct TESS *pTess)
{
  int i, j, k, id;

  (*pTess).PolyQty = N[0] * N[1] * N[2];
  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_2d_int ((*pTess).PolyQty + 1, 9);
  (*pTess).PolyFaceOri = ut_alloc_2d_int ((*pTess).PolyQty + 1, 9);
  (*pTess).PolyState = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  for (k = 0; k < N[2]; k++)
    for (j = 0; j < N[1]; j++)
      for (i = 0; i < N[0]; i++)
      {
        id = polyid[i][j][k];
        (*pTess).PolyFaceQty[id] = 6;
        (*pTess).PolyFaceNb[id][1] = faceid[0][i][j][k];
        (*pTess).PolyFaceNb[id][2] = faceid[0][i + 1][j][k];
        (*pTess).PolyFaceNb[id][3] = faceid[1][i][j][k];
        (*pTess).PolyFaceNb[id][4] = faceid[1][i][j + 1][k];
        (*pTess).PolyFaceNb[id][5] = faceid[2][i][j][k];
        (*pTess).PolyFaceNb[id][6] = faceid[2][i][j][k + 1];
        (*pTess).PolyFaceOri[id][1] = -1;
        (*pTess).PolyFaceOri[id][2] = 1;
        (*pTess).PolyFaceOri[id][3] = -1;
        (*pTess).PolyFaceOri[id][4] = 1;
        (*pTess).PolyFaceOri[id][5] = -1;
        (*pTess).PolyFaceOri[id][6] = 1;
      }

  return;
}

void
net_tess_cube_facepoly (int *N, int ****faceid, struct TESS *pTess)
{
  int i, j, k, id;

  neut_tess_init_facepoly (pTess);

  for (k = 0; k < N[2]; k++)
    for (j = 0; j < N[1]; j++)
    {
      id = faceid[0][0][j][k];
      (*pTess).FacePoly[id][1] = -1;

      id = faceid[0][N[0]][j][k];
      (*pTess).FacePoly[id][1] = -2;
    }

  for (i = 0; i < N[0]; i++)
    for (k = 0; k < N[2]; k++)
    {
      id = faceid[1][i][0][k];
      (*pTess).FacePoly[id][1] = -3;

      id = faceid[1][i][N[1]][k];
      (*pTess).FacePoly[id][1] = -4;
    }

  for (j = 0; j < N[1]; j++)
    for (i = 0; i < N[0]; i++)
    {
      id = faceid[2][i][j][0];
      (*pTess).FacePoly[id][1] = -5;

      id = faceid[2][i][j][N[2]];
      (*pTess).FacePoly[id][1] = -6;
    }

  return;
}
