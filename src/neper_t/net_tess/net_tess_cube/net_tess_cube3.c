/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_cube_.h"

void
net_tess_cube_vers_per (int *N, int ***verid, struct TESS *pTess)
{
  int i, j, k, pri, sec;

  (*pTess).PerVerMaster = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).PerVerShift = ut_alloc_2d_int ((*pTess).VerQty + 1, 3);
  (*pTess).PerVerQty = 0;

  // primaries may be overriden in the 2nd and 3rd sequences, but it doesn't matter to shrinkmaster

  if ((*pTess).Periodic[0])
  {
    for (k = 0; k <= N[2]; k++)
      for (j = 0; j <= N[1]; j++)
      {
        pri = verid[0][j][k];
        sec = verid[N[0]][j][k];
        (*pTess).PerVerMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerVerShift[sec], 1, 0, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerVerNb, &(*pTess).PerVerQty, sec);
      }
  }

  if ((*pTess).Periodic[1])
  {
    for (k = 0; k <= N[2]; k++)
      for (i = 0; i <= N[0]; i++)
      {
        pri = verid[i][0][k];
        sec = verid[i][N[1]][k];
        (*pTess).PerVerMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerVerShift[sec], 0, 1, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerVerNb, &(*pTess).PerVerQty, sec);
      }
  }

  if ((*pTess).Periodic[2])
  {
    for (j = 0; j <= N[1]; j++)
      for (i = 0; i <= N[0]; i++)
      {
        pri = verid[i][j][0];
        sec = verid[i][j][N[2]];
        (*pTess).PerVerMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerVerShift[sec], 0, 0, 1);

        ut_array_1d_int_list1_addval (&(*pTess).PerVerNb, &(*pTess).PerVerQty, sec);
      }
  }

  net_tess3d_periodic_ver_shrinkmaster (pTess);

  neut_tess_init_verslave (pTess);

  return;
}

void
net_tess_cube_edges_per (int *N, int ****edgeid, struct TESS *pTess)
{
  int i, j, k, pri, sec;

  (*pTess).PerEdgeMaster = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeShift = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 3);
  (*pTess).PerEdgeOri = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeQty = 0;

  ut_array_1d_int_set ((*pTess).PerEdgeOri + 1, (*pTess).EdgeQty, 1);

  // primaries may be overriden in the 2nd and 3rd sequences, but it doesn't matter to shrinkmaster

  if ((*pTess).Periodic[0])
  {
    for (k = 0; k <= N[2]; k++)
      for (j = 0; j < N[1]; j++)
      {
        pri = edgeid[1][0][j][k];
        sec = edgeid[1][N[0]][j][k];
        (*pTess).PerEdgeMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerEdgeShift[sec], 1, 0, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerEdgeNb, &(*pTess).PerEdgeQty, sec);
      }

    for (k = 0; k < N[2]; k++)
      for (j = 0; j <= N[1]; j++)
      {
        pri = edgeid[2][0][j][k];
        sec = edgeid[2][N[0]][j][k];
        (*pTess).PerEdgeMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerEdgeShift[sec], 1, 0, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerEdgeNb, &(*pTess).PerEdgeQty, sec);
      }

    for (i = 0; i <= N[0]; i++)
      for (k = 0; k < N[2]; k++)
      {
        pri = edgeid[2][i][0][k];
        sec = edgeid[2][i][N[1]][k];
        (*pTess).PerEdgeMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerEdgeShift[sec], 0, 1, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerEdgeNb, &(*pTess).PerEdgeQty, sec);
      }

    for (i = 0; i < N[0]; i++)
      for (k = 0; k <= N[2]; k++)
      {
        pri = edgeid[0][i][0][k];
        sec = edgeid[0][i][N[1]][k];
        (*pTess).PerEdgeMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerEdgeShift[sec], 0, 1, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerEdgeNb, &(*pTess).PerEdgeQty, sec);
      }

    for (j = 0; j <= N[1]; j++)
      for (i = 0; i < N[0]; i++)
      {
        pri = edgeid[0][i][j][0];
        sec = edgeid[0][i][j][N[1]];
        (*pTess).PerEdgeMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerEdgeShift[sec], 0, 0, 1);

        ut_array_1d_int_list1_addval (&(*pTess).PerEdgeNb, &(*pTess).PerEdgeQty, sec);
      }

    for (j = 0; j < N[1]; j++)
      for (i = 0; i <= N[0]; i++)
      {
        pri = edgeid[1][i][j][0];
        sec = edgeid[1][i][j][N[1]];
        (*pTess).PerEdgeMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerEdgeShift[sec], 0, 0, 1);

        ut_array_1d_int_list1_addval (&(*pTess).PerEdgeNb, &(*pTess).PerEdgeQty, sec);
      }

  }

  net_tess3d_periodic_edge_shrinkmaster (pTess);

  neut_tess_init_edgeslave (pTess);

  return;
}

void
net_tess_cube_faces_per (int *N, int ****faceid, struct TESS *pTess)
{
  int i, j, k, pri, sec;

  (*pTess).PerFaceMaster = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).PerFaceShift = ut_alloc_2d_int ((*pTess).FaceQty + 1, 3);
  (*pTess).PerFaceOri = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).PerFaceQty = 0;

  ut_array_1d_int_set ((*pTess).PerFaceOri + 1, (*pTess).FaceQty, 1);

  if ((*pTess).Periodic[0])
  {
    for (k = 0; k < N[2]; k++)
      for (j = 0; j < N[1]; j++)
      {
        pri = faceid[0][0][j][k];
        sec = faceid[0][N[0]][j][k];
        (*pTess).PerFaceMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerFaceShift[sec], 1, 0, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerFaceNb, &(*pTess).PerFaceQty, sec);
      }

    for (i = 0; i < N[0]; i++)
      for (k = 0; k < N[2]; k++)
      {
        pri = faceid[1][i][0][k];
        sec = faceid[1][i][N[1]][k];
        (*pTess).PerFaceMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerFaceShift[sec], 0, 1, 0);

        ut_array_1d_int_list1_addval (&(*pTess).PerFaceNb, &(*pTess).PerFaceQty, sec);
      }

    for (j = 0; j < N[1]; j++)
      for (i = 0; i < N[0]; i++)
      {
        pri = faceid[2][i][j][0];
        sec = faceid[2][i][j][N[2]];
        (*pTess).PerFaceMaster[sec] = pri;
        ut_array_1d_int_set_3 ((*pTess).PerFaceShift[sec], 0, 0, 1);

        ut_array_1d_int_list1_addval (&(*pTess).PerFaceNb, &(*pTess).PerFaceQty, sec);
      }
  }

  neut_tess_init_faceslave (pTess);

  return;
}
