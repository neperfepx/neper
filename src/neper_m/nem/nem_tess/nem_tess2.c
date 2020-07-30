/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_tess_.h"

void
nem_tess_updatefrommesh_geom_seed (struct TESS *pTess, struct NODES Nodes,
                                   struct MESH *Mesh)
{
  int i;

  // reinitializing tess properties from nodes / mesh
  // seed coo and weight
  if ((*pTess).SeedCoo == NULL)
    (*pTess).SeedCoo = ut_alloc_2d ((*pTess).CellQty + 1, 3);

  if (Mesh[(*pTess).Dim].ElsetQty > 0)
    for (i = 1; i <= (*pTess).CellQty; i++)
      neut_mesh_elset_centre (Nodes, Mesh[(*pTess).Dim], i,
                              (*pTess).SeedCoo[i]);

  if ((*pTess).SeedWeight == NULL)
    (*pTess).SeedWeight = ut_alloc_1d ((*pTess).CellQty + 1);

  return;
}

void
nem_tess_updatefrommesh_geom_ver (struct TESS *pTess, struct NODES Nodes,
                                  struct MESH *Mesh)
{
  int i, node;

  // ver coo
  if ((*pTess).VerCoo == NULL)
    (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);

  if (Mesh[0].ElsetQty > 0)
    for (i = 1; i <= (*pTess).VerQty; i++)
    {
      node = Mesh[0].EltNodes[i][0];
      ut_array_1d_memcpy (Nodes.NodeCoo[node], 3, (*pTess).VerCoo[i]);
    }

  return;
}

void
nem_tess_updatefrommesh_geom_edge (struct TESS *pTess, struct NODES Nodes,
                                   struct MESH *Mesh)
{
  int i, j;
  double length;

  // edge length (sum over the elements to account for curvature)
  if ((*pTess).EdgeState == NULL)
    (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  ut_array_1d_int_set ((*pTess).EdgeState + 1, (*pTess).EdgeQty, 1);

  if ((*pTess).EdgeLength == NULL)
    (*pTess).EdgeLength = ut_alloc_1d ((*pTess).EdgeQty + 1);

  ut_array_1d_set ((*pTess).EdgeLength + 1, (*pTess).EdgeQty, 0);

  if (Mesh[1].ElsetQty > 0)
    for (i = 1; i <= (*pTess).EdgeQty; i++)
      for (j = 1; j <= Mesh[1].Elsets[i][0]; j++)
      {
        neut_mesh_elt_length (Nodes, Mesh[1], Mesh[1].Elsets[i][j], &length);
        (*pTess).EdgeLength[i] += length;
      }

  return;
}

void
nem_tess_updatefrommesh_geom_face (struct TESS *pTess, struct NODES Nodes,
                                   struct MESH *Mesh)
{
  int i, j, elt;
  double norm;
  double *eq = ut_alloc_1d (4);
  double *eqe = ut_alloc_1d (4);

  // face equations
  // the elt normals are averaged, so as the constant for the equation
  // the constant does not really matter because it does not change the
  // projection (which is done along the normal).
  if (!(*pTess).FaceState)
    (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  ut_array_1d_int_set ((*pTess).FaceState + 1, (*pTess).FaceQty, 1);

  if ((*pTess).FaceEq == NULL)
    (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);

  if (Mesh[2].ElsetQty > 0)
    for (i = 1; i <= (*pTess).FaceQty; i++)
    {
      ut_array_1d_zero (eq, 4);
      for (j = 1; j <= Mesh[2].Elsets[i][0]; j++)
      {
        elt = Mesh[2].Elsets[i][j];
        neut_mesh_elt_eq (Mesh[2], Nodes, elt, eqe);
        ut_array_1d_add (eqe, eq, 4, eq);
      }

      norm = ut_vector_norm (eq + 1);
      ut_array_1d_scale (eq, 4, 1. / norm);

      ut_array_1d_memcpy (eq, 4, (*pTess).FaceEq[i]);
    }

  nem_tess_updatefrommesh_geom_polyfaceori (pTess, Nodes, Mesh);

  ut_free_1d (&eqe);
  ut_free_1d (&eq);

  return;
}
