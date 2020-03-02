/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_tess_.h"

void
nem_tess_updatefrommesh_geom_polyfaceori (struct TESS *pTess,
                                          struct NODES Nodes,
                                          struct MESH *Mesh)
{
  int i, j, face;

  if (Mesh[2].EltQty <= 0)
    abort ();

  if ((*pTess).PolyFaceOri == NULL)
  {
    (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
    (*pTess).PolyFaceOri[0] = ut_alloc_1d_int (1);
  }

  // 3D mesh available
  if (Mesh[3].EltQty > 0)
  {
    int k;
    int face, elt2d, elt3dqty, elt3d;
    int *elts3d = NULL;
    double *eq = ut_alloc_1d (4);
    double *eltcentre = ut_alloc_1d (3);
    for (i = 1; i <= (*pTess).PolyQty; i++)
    {
      (*pTess).PolyFaceOri[i] = ut_alloc_1d_int ((*pTess).PolyFaceQty[i] + 1);
      for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
      {
        face = (*pTess).PolyFaceNb[i][j];
        elt2d = Mesh[2].Elsets[face][1];
        neut_mesh_elt2d_elts3d (Mesh[2], elt2d, Mesh[3], &elts3d, &elt3dqty);
        elt3d = -1;
        for (k = 0; k < elt3dqty; k++)
        {
          if (Mesh[3].EltElset[elts3d[k]] == i)
          {
            elt3d = elts3d[k];
            break;
          }
        }
        if (elt3d == -1)
          ut_print_neperbug ();

        neut_mesh_elt_eq (Mesh[2], Nodes, elt2d, eq);
        neut_mesh_elt_centre (Nodes, Mesh[3], elt3d, eltcentre);

        (*pTess).PolyFaceOri[i][j] =
          -ut_space_point_plane_side (eltcentre - 1, eq);
        if (ut_vector_scalprod ((*pTess).FaceEq[face] + 1, eq + 1) < 0)
          (*pTess).PolyFaceOri[i][j] *= -1;
      }
    }
    ut_free_1d (&eq);
    ut_free_1d (&eltcentre);
    ut_free_1d_int (&elts3d);
  }

  // 3D mesh not available
  else
  {
    double *centre = ut_alloc_1d (3);

    for (i = 1; i <= (*pTess).PolyQty; i++)
    {
      neut_mesh_elsets_centre (Nodes, Mesh[2], (*pTess).PolyFaceNb[i] + 1,
                               (*pTess).PolyFaceQty[i], centre);

      (*pTess).PolyFaceOri[i] = ut_alloc_1d_int ((*pTess).PolyFaceQty[i] + 1);

      for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
      {
        face = (*pTess).PolyFaceNb[i][j];

        (*pTess).PolyFaceOri[i][j] =
          -ut_space_point_plane_side (centre - 1, (*pTess).FaceEq[face]);
      }
    }
    ut_free_1d (&centre);
  }

  return;
}
