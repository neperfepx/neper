/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_gmsh_.h"

static int
nem_mesh_3d_gmsh_localskin_cmp (const void *a, const void *b)
{
  return (*(const int *) a > *(const int *) b) - (*(const int *) a < *(const int *) b);
}

// nem_mesh_3d_gmsh_localskin returns the nodes of a skin mesh, and the
// skin mesh renumbered accordingly (node i of *pSkinNodes is node
// nodes[i - 1] of Nodes, where nodes are the sorted skin nodes)
void
nem_mesh_3d_gmsh_localskin (struct NODES Nodes, struct MESH Skin,
                            struct NODES *pSkinNodes, struct MESH *pSkinLoc)
{
  int i, j, qty, *nodes = NULL, *pos = NULL;
  int eltnodeqty = neut_elt_nodeqty (Skin.EltType, Skin.Dimension, Skin.EltOrder);

  nodes = ut_alloc_1d_int (Skin.EltQty * eltnodeqty);
  for (i = 1; i <= Skin.EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      nodes[(i - 1) * eltnodeqty + j] = Skin.EltNodes[i][j];
  ut_array_1d_int_sort_uniq (nodes, Skin.EltQty * eltnodeqty, &qty);

  for (i = 0; i < qty; i++)
    neut_nodes_addnode (pSkinNodes, Nodes.NodeCoo[nodes[i]], Nodes.NodeCl[nodes[i]]);

  neut_mesh_memcpy (Skin, pSkinLoc);
  for (i = 1; i <= (*pSkinLoc).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
    {
      pos = (int *) bsearch (&((*pSkinLoc).EltNodes[i][j]), nodes, qty,
                             sizeof (int), nem_mesh_3d_gmsh_localskin_cmp);
      (*pSkinLoc).EltNodes[i][j] = pos - nodes + 1;
    }

  ut_free_1d_int (&nodes);

  return;
}

// nem_mesh_gmsh_sizeratio returns the mean ratio of the elt lengths of a
// mesh to the mesh size map of boundary nodes (see neut_nodes_point_clgrad),
// at the elt centres
double
nem_mesh_gmsh_sizeratio (struct NODES BNodes, double cl, double clgrad,
                         struct NODES N, struct MESH M)
{
  int i;
  double length, sum = 0, *centre = ut_alloc_1d (3);

  for (i = 1; i <= M.EltQty; i++)
  {
    neut_mesh_elt_centre (N, M, i, centre);
    neut_mesh_elt_length (N, M, i, &length);
    sum += length / neut_nodes_point_clgrad (BNodes, centre, cl, clgrad);
  }

  ut_free_1d (&centre);

  return sum / M.EltQty;
}

// Calibration of the scale factor of the mesh size map passed to Gmsh, alpha,
// so that the mean ratio of the elt lengths to the mesh size map, r, is 1
// within a tolerance (used for both the faces and the polys).  Until alpha is
// bracketed (between a value giving r < 1, alphamin, and a value giving r >
// 1, alphamax), alpha is scaled by r^(-1 / slope), where slope is estimated
// from the last two iterations (initially NEM_MESH_GMSH_CLSLOPE); then, the
// bracket is bisected.  Iterations stop once r is within the tolerance, if
// alpha has no effect, if the bracket is narrower than 1%, after
// NEM_MESH_GMSH_CLITERMAX iterations, or if alpha gets out of
// [NEM_MESH_GMSH_ALPHAMIN, NEM_MESH_GMSH_ALPHAMAX].
void
nem_mesh_gmsh_sizecalib_init (struct NEM_SIZECALIB *pCalib)
{
  (*pCalib).iter = 0;
  (*pCalib).alpha = 1;
  (*pCalib).prevalpha = 0;
  (*pCalib).prevr = 0;
  (*pCalib).slope = NEM_MESH_GMSH_CLSLOPE;
  (*pCalib).alphamin = 0;
  (*pCalib).alphamax = DBL_MAX;
  (*pCalib).besterr = DBL_MAX;

  return;
}

// Recording the ratio, r, of the mesh obtained with the current alpha, and
// updating alpha.  *pbest is set to 1 if the mesh is the best so far (to be
// kept).  Returns 1 if iterations must stop, 0 otherwise.
int
nem_mesh_gmsh_sizecalib_next (struct NEM_SIZECALIB *pCalib, double r,
                              double clreps, int *pbest)
{
  int done = 0;
  double err = fabs (r - 1);

  (*pCalib).iter++;

  (*pbest) = (err < (*pCalib).besterr);
  if (*pbest)
    (*pCalib).besterr = err;

  if (r > 1)
    (*pCalib).alphamax = ut_num_min ((*pCalib).alphamax, (*pCalib).alpha);
  else
    (*pCalib).alphamin = ut_num_max ((*pCalib).alphamin, (*pCalib).alpha);

  if ((*pCalib).besterr <= clreps)
    done = 1;
  else if ((*pCalib).prevalpha > 0 && ut_num_requal (r, (*pCalib).prevr, 1e-9))
    done = 1;                   // alpha has no effect
  else if ((*pCalib).alphamin > 0 && (*pCalib).alphamax < DBL_MAX)
  {
    if ((*pCalib).alphamax / (*pCalib).alphamin < 1.01)
      done = 1;
    else
      (*pCalib).alpha = sqrt ((*pCalib).alphamin * (*pCalib).alphamax);
  }
  else
  {
    if ((*pCalib).prevalpha > 0)
      (*pCalib).slope =
        ut_num_bound (log (r / (*pCalib).prevr)
                      / log ((*pCalib).alpha / (*pCalib).prevalpha), 0.1, 1);
    (*pCalib).prevalpha = (*pCalib).alpha;
    (*pCalib).prevr = r;

    (*pCalib).alpha *= ut_num_bound (pow (1 / r, 1 / (*pCalib).slope), 0.5, 2);
  }

  if ((*pCalib).iter >= NEM_MESH_GMSH_CLITERMAX
      || (*pCalib).alpha < NEM_MESH_GMSH_ALPHAMIN
      || (*pCalib).alpha > NEM_MESH_GMSH_ALPHAMAX)
    done = 1;

  return done;
}
