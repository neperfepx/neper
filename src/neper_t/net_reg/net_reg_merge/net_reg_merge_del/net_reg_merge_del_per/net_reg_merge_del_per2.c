/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_per_.h"

// Resetting the periodicity of a vertex (no master, no slave)
void
net_reg_merge_del_per_vermerge_reset (struct TESS *pTess, int ver)
{
  (*pTess).PerVerMaster[ver] = 0;
  ut_array_1d_int_zero ((*pTess).PerVerShift[ver], 3);
  (*pTess).PerVerSlaveQty[ver] = 0;
  ut_free_1d_int (&(*pTess).PerVerSlaveNb[ver]);

  return;
}

// Merging a vertex into another one (at the same location), and then the
// edges that link the vertex to the same vertex
void
net_reg_merge_del_per_vermerge_ver (struct TESS *pTess, int ver, int ver2)
{
  int i, j, edge, edge2, pos, faceqty, *faces = NULL;

  // faces of ver (before its edges are changed)
  neut_tess_ver_faces (*pTess, ver, &faces, &faceqty);
  for (i = 0; i < faceqty; i++)
  {
    pos = ut_array_1d_int_eltpos ((*pTess).FaceVerNb[faces[i]] + 1,
                                  (*pTess).FaceVerQty[faces[i]], ver);
    (*pTess).FaceVerNb[faces[i]][pos + 1] = ver2;
  }

  // edges of ver
  for (i = 0; i < (*pTess).VerEdgeQty[ver]; i++)
  {
    edge = (*pTess).VerEdgeNb[ver][i];
    pos = ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[edge], 2, ver);
    (*pTess).EdgeVerNb[edge][pos] = ver2;
    ut_array_1d_int_list_addval (&(*pTess).VerEdgeNb[ver2],
                                 &(*pTess).VerEdgeQty[ver2], edge);
  }

  (*pTess).VerState[ver] = -1;

  // edges of ver2 linking it to the same vertex
  for (i = 0; i < (*pTess).VerEdgeQty[ver2]; i++)
    for (j = i + 1; j < (*pTess).VerEdgeQty[ver2]; j++)
    {
      edge = (*pTess).VerEdgeNb[ver2][i];
      edge2 = (*pTess).VerEdgeNb[ver2][j];

      if (neut_tess_edge_otherver (*pTess, edge, ver2)
          == neut_tess_edge_otherver (*pTess, edge2, ver2))
      {
        net_reg_merge_del_per_vermerge_ver_edge (pTess, edge, edge2);
        j--;
      }
    }

  ut_free_1d_int (&faces);

  return;
}

// Merging an edge into another one (of the same vertices)
void
net_reg_merge_del_per_vermerge_ver_edge (struct TESS *pTess, int edge,
                                         int edge2)
{
  int i, ver, face, pos, ori;

  ori =
    ((*pTess).EdgeVerNb[edge][0] == (*pTess).EdgeVerNb[edge2][0]) ? 1 : -1;

  for (i = 0; i < (*pTess).EdgeFaceQty[edge2]; i++)
  {
    face = (*pTess).EdgeFaceNb[edge2][i];
    pos = ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[face] + 1,
                                  (*pTess).FaceEdgeQty[face], edge2) + 1;
    (*pTess).FaceEdgeNb[face][pos] = edge;
    (*pTess).FaceEdgeOri[face][pos] *= ori;
    ut_array_1d_int_list_addval (&(*pTess).EdgeFaceNb[edge],
                                 &(*pTess).EdgeFaceQty[edge], face);
  }
  ut_array_1d_int_sort ((*pTess).EdgeFaceNb[edge],
                        (*pTess).EdgeFaceQty[edge]);

  for (i = 0; i < 2; i++)
  {
    ver = (*pTess).EdgeVerNb[edge2][i];
    ut_array_1d_int_list_rmval (&(*pTess).VerEdgeNb[ver],
                                &(*pTess).VerEdgeQty[ver], edge2);
  }

  if ((*pTess).EdgeDom[edge2][0] == 1)
    ut_array_1d_int_memcpy ((*pTess).EdgeDom[edge2], 2,
                            (*pTess).EdgeDom[edge]);

  (*pTess).EdgeState[edge2] = -1;

  return;
}

// Testing whether two edges of a vertex link it to the same vertex.
// Returns -1 if it is the case, 0 otherwise.
int
net_reg_merge_del_per_vermerge_testedges (struct TESS Tess, int ver)
{
  int i, j, edge, edge2;

  for (i = 0; i < Tess.VerEdgeQty[ver]; i++)
    for (j = i + 1; j < Tess.VerEdgeQty[ver]; j++)
    {
      edge = Tess.VerEdgeNb[ver][i];
      edge2 = Tess.VerEdgeNb[ver][j];

      if (neut_tess_edge_otherver (Tess, edge, ver)
          == neut_tess_edge_otherver (Tess, edge2, ver))
        return -1;
    }

  return 0;
}
