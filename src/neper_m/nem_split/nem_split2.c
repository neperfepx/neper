/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_split_.h"

// the edge centres are the order-2 nodes, which are placed on the curved
// domain edges and faces and tied by periodicity
void
nem_split_edgenodes (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
                     struct MESH *Mesh)
{
  int i;
  struct IN_M In2 = In;

  // (no interface processing at this stage)
  In2.interface = "none";

  for (i = 1; i <= 3; i++)
    if (!neut_mesh_isvoid (Mesh[i]))
      neut_mesh_init_nodeelts (Mesh + i, (*pNodes).NodeQty);

  nem_order (In2, Tess, pNodes, Mesh);

  return;
}

// facenode[elt]: centre node of the 2D elt; eltfacenodes[elt][i]: centre
// node of the face of the 3D elt opposite to its node i
void
nem_split_facenodes (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh,
                     int **pfacenode, int ***peltfacenodes)
{
  int i;
  double coo[3], cl;

  if (!neut_mesh_isvoid (Mesh[3]))
    nem_split_facenodes_3d (pNodes, Mesh, pfacenode, peltfacenodes);

  else
  {
    (*pfacenode) = ut_alloc_1d_int (Mesh[2].EltQty + 1);

    for (i = 1; i <= Mesh[2].EltQty; i++)
    {
      neut_nodes_bary (*pNodes, Mesh[2].EltNodes[i], 3, coo, &cl);
      (*pfacenode)[i] = neut_nodes_addnode (pNodes, coo, cl);
    }
  }

  nem_split_facenodes_curved (Tess, *pfacenode, pNodes, Mesh);

  if (Tess.Type && !strcmp (Tess.Type, "periodic"))
    nem_split_facenodes_periodic (Tess, *pfacenode, pNodes, Mesh);

  return;
}

void
nem_split_1d (struct MESH *pMesh)
{
  int i, **childnodes = NULL;

  if (neut_mesh_isvoid (*pMesh))
    return;

  childnodes = ut_alloc_2d_int (2 * (*pMesh).EltQty + 1, 2);

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    childnodes[2 * i - 1][0] = (*pMesh).EltNodes[i][0];
    childnodes[2 * i - 1][1] = (*pMesh).EltNodes[i][2];
    childnodes[2 * i][0] = (*pMesh).EltNodes[i][2];
    childnodes[2 * i][1] = (*pMesh).EltNodes[i][1];
  }

  nem_split_mesh (2, childnodes, pMesh);

  ut_free_2d_int (&childnodes, (*pMesh).EltQty + 1);

  return;
}

// a tri elt (0, 1, 2) of edge centres (3, 4, 5) (between 0-1, 1-2 and 2-0)
// and centre c is split into the quad elts (0, 3, c, 5), (1, 4, c, 3) and
// (2, 5, c, 4), which have the orientation of the tri elt
void
nem_split_2d (int *facenode, struct MESH *pMesh)
{
  int i, j, *n = NULL, **childnodes = NULL;

  if (neut_mesh_isvoid (*pMesh))
    return;

  childnodes = ut_alloc_2d_int (3 * (*pMesh).EltQty + 1, 4);

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    n = (*pMesh).EltNodes[i];

    for (j = 0; j < 3; j++)
    {
      childnodes[3 * (i - 1) + j + 1][0] = n[j];
      childnodes[3 * (i - 1) + j + 1][1] = n[3 + j];
      childnodes[3 * (i - 1) + j + 1][2] = facenode[i];
      childnodes[3 * (i - 1) + j + 1][3] = n[3 + (j + 2) % 3];
    }
  }

  nem_split_mesh (3, childnodes, pMesh);

  ut_free_2d_int (&childnodes, (*pMesh).EltQty + 1);

  return;
}

// a tet elt is split into 4 hex elts, one per node l.  With (l, p, q, r) an
// even permutation of (0, 1, 2, 3), the hex elt of node l is (l, lp, lpq,
// lq, lr, lpr, c, lqr), where lp is the centre of edge l-p, lpq the centre
// of face l-p-q and c the centre of the tet elt, which has the orientation
// of the tet elt (p and q are swapped for a negatively oriented tet elt)
void
nem_split_3d (int **eltfacenodes, struct NODES *pNodes, struct MESH *pMesh)
{
  int i, j, l, p, q, r, centre, *n = NULL, *h = NULL, **sec = NULL;
  int **childnodes = NULL;
  int perm[4][3] = { {1, 2, 3}, {0, 3, 2}, {0, 1, 3}, {0, 2, 1} };
  double coo[3], cl, vol, v[3][3], w[3];

  if (neut_mesh_isvoid (*pMesh))
    return;

  neut_elt_orderarrays ("tri", 3, NULL, NULL, &sec, NULL);

  childnodes = ut_alloc_2d_int (4 * (*pMesh).EltQty + 1, 8);

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    n = (*pMesh).EltNodes[i];

    neut_nodes_bary (*pNodes, n, 4, coo, &cl);
    centre = neut_nodes_addnode (pNodes, coo, cl);

    // (sign of the volume)
    for (j = 0; j < 3; j++)
      ut_array_1d_sub ((*pNodes).NodeCoo[n[0]], (*pNodes).NodeCoo[n[j + 1]], 3, v[j]);
    ut_vector_vectprod (v[1], v[2], w);
    vol = ut_vector_scalprod (v[0], w);

    for (j = 0; j < 4; j++)
    {
      l = j;
      p = perm[j][0];
      q = perm[j][1];
      r = perm[j][2];
      if (vol < 0)
        ut_num_switch_int (&p, &q);

      h = childnodes[4 * (i - 1) + j + 1];
      h[0] = n[l];
      h[1] = n[sec[l][p]];
      h[2] = eltfacenodes[i][r];
      h[3] = n[sec[l][q]];
      h[4] = n[sec[l][r]];
      h[5] = eltfacenodes[i][q];
      h[6] = centre;
      h[7] = eltfacenodes[i][p];
    }
  }

  nem_split_mesh (4, childnodes, pMesh);

  ut_free_2d_int (&childnodes, (*pMesh).EltQty + 1);
  ut_free_2d_int (&sec, 4);

  return;
}
