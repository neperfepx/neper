/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_split_.h"

// Splitting a (conformal) tri mesh into a (conformal) quad mesh, as done
// by Gmsh (Mesh.SubdivisionAlgorithm = 1 or 2): a 1D elt is split into 2
// 1D elts at its centre, a 2D (triangular) elt into 3 quadrangular elts
// from its edge centres and centre, and a 3D (tetrahedral) elt into 4
// hexahedral elts from its edge centres, face centres and centre.  The new
// nodes are shared by all elts (of all dimensions), so that the mesh
// remains conformal, and the edge and face centres are placed on the
// curved domain edges and faces, and tied by periodicity, as the order-2
// nodes are.  The elsets are preserved.
void
nem_split (struct IN_M In, struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  int dim = neut_mesh_array_dim (Mesh), tetqty = Mesh[3].EltQty;
  int *facenode = NULL, **eltfacenodes = NULL;

  // (already split during meshing, see nem_meshing)
  if (dim < 1 || !strcmp (Mesh[dim].EltType, "quad"))
    return;

  // edge centres (order-2 nodes)
  nem_split_edgenodes (In, Tess, pNodes, Mesh);

  // face centres
  if (dim >= 2)
    nem_split_facenodes (Tess, pNodes, Mesh, &facenode, &eltfacenodes);

  nem_split_1d (Mesh + 1);

  if (dim >= 2)
    nem_split_2d (facenode, Mesh + 2);

  if (dim == 3)
    nem_split_3d (eltfacenodes, pNodes, Mesh + 3);

  neut_mesh_init_nodeelts (Mesh + 0, (*pNodes).NodeQty);

  ut_free_1d_int (&facenode);
  if (eltfacenodes)
    ut_free_2d_int (&eltfacenodes, tetqty + 1);

  return;
}

// splitting the 1D mesh only (before meshing in 2D, see nem_meshing); the
// characteristic length of an edge centre node is the mean of the ones of
// its elt
void
nem_split_mesh1d (struct IN_M In, struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, *n = NULL;

  if (neut_mesh_isvoid (Mesh[1]))
    return;

  nem_split_edgenodes (In, Tess, pNodes, Mesh);

  for (i = 1; i <= Mesh[1].EltQty; i++)
  {
    n = Mesh[1].EltNodes[i];
    (*pNodes).NodeCl[n[2]] = 0.5 * ((*pNodes).NodeCl[n[0]] + (*pNodes).NodeCl[n[1]]);
  }

  nem_split_1d (Mesh + 1);

  return;
}

// ratio of the mean edge length of the elts resulting from the split of a
// regular elt to the edge length of the regular elt: in 3D, (3 * 1/2 + 6 *
// 1/(2 sqrt(3)) + 3 * 1/(2 sqrt(6))) / 12 (edges from the tet nodes, the
// edge centres and the face centres, respectively); in 2D, (2 * 1/2 + 2 *
// 1/(2 sqrt(3))) / 4; in 1D, 1/2
double
nem_split_sizefact (int dim)
{
  if (dim == 3)
    return (1.5 + 3 / sqrt (3) + 1.5 / sqrt (6)) / 12;
  else if (dim == 2)
    return (1 + 1 / sqrt (3)) / 4;
  else
    return 0.5;
}
