/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_face_mesh_gmsh_.h"

// Basis of a face projected onto a cylinder: a point of the axis (c), the
// (unit) axis (a), the radial direction at the middle of the face (e1), e2 =
// a x e1, and the radius (r).  The middle of the face is the middle of the
// angular range of its boundary nodes, which is the complement of the largest
// angular gap between them.
void
nem_meshing_2d_face_mesh_gmsh_cyl_basis (struct TESS Tess, int face, struct NODES Nodes,
                                         int *bnodes, int bnodeqty, double *c, double *a,
                                         double *e1, double *e2, double *pr)
{
  int i, j, domface = Tess.FaceDom[face][1];
  double h, mid, gap, maxgap = -1, *theta = ut_alloc_1d (bnodeqty);
  double x[3] = { 1, 0, 0 }, y[3] = { 0, 1, 0 };
  double *v = ut_alloc_1d (3), *u1 = ut_alloc_1d (3), *u2 = ut_alloc_1d (3);

  ut_array_1d_memcpy (Tess.DomFaceParms[domface], 3, c);
  ut_array_1d_memcpy (Tess.DomFaceParms[domface] + 3, 3, a);
  ut_array_1d_normalize (a, 3);
  (*pr) = Tess.DomFaceParms[domface][6];

  // any basis of the plane normal to the axis (u1, u2)
  ut_vector_vectprod (a, fabs (a[0]) < 0.9 ? x : y, u1);
  ut_array_1d_normalize (u1, 3);
  ut_vector_vectprod (a, u1, u2);

  // angles of the boundary nodes, and largest angular gap
  for (i = 0; i < bnodeqty; i++)
  {
    ut_array_1d_sub (c, Nodes.NodeCoo[bnodes[i]], 3, v);
    h = ut_vector_scalprod (v, a);
    for (j = 0; j < 3; j++)
      v[j] -= h * a[j];
    theta[i] = atan2 (ut_vector_scalprod (v, u2), ut_vector_scalprod (v, u1));
  }

  ut_array_1d_sort (theta, bnodeqty);

  mid = 0;
  for (i = 0; i < bnodeqty; i++)
  {
    gap = (i < bnodeqty - 1) ? theta[i + 1] - theta[i] : theta[0] + 2 * M_PI - theta[i];
    if (gap > maxgap)
    {
      maxgap = gap;
      // the middle of the face is opposite to the middle of the gap
      mid = theta[i] + 0.5 * gap + M_PI;
    }
  }

  for (i = 0; i < 3; i++)
    e1[i] = cos (mid) * u1[i] + sin (mid) * u2[i];
  ut_vector_vectprod (a, e1, e2);

  ut_free_1d (&theta);
  ut_free_1d (&v);
  ut_free_1d (&u1);
  ut_free_1d (&u2);

  return;
}

// Basis of a face projected onto a sphere: the centre (c), the unit outgoing
// direction of the middle of the face (e), which is the mean direction of
// its boundary nodes, and the radius (r)
void
nem_meshing_2d_face_mesh_gmsh_sph_basis (struct TESS Tess, int face, struct NODES Nodes,
                                         int *bnodes, int bnodeqty, double *c, double *e,
                                         double *pr)
{
  int i, j, domface = Tess.FaceDom[face][1];
  double *v = ut_alloc_1d (3);

  ut_array_1d_memcpy (Tess.DomFaceParms[domface], 3, c);
  (*pr) = Tess.DomFaceParms[domface][3];

  ut_array_1d_zero (e, 3);
  for (i = 0; i < bnodeqty; i++)
  {
    ut_space_points_uvect (c, Nodes.NodeCoo[bnodes[i]], v);
    for (j = 0; j < 3; j++)
      e[j] += v[j];
  }
  ut_array_1d_normalize (e, 3);

  ut_free_1d (&v);

  return;
}

// Basis of a face projected onto a torus: the centre (c), the (unit) axis
// (a), the radial direction at the middle of the face (e1) and e2 = a x e1
// (as for a cylinder), the major and minor radii (R and r), and the mean
// distance of the boundary nodes to the axis (rho0)
void
nem_meshing_2d_face_mesh_gmsh_tor_basis (struct TESS Tess, int face, struct NODES Nodes,
                                         int *bnodes, int bnodeqty, double *c, double *a,
                                         double *e1, double *e2, double *pR, double *pr,
                                         double *prho0)
{
  int i, j, domface = Tess.FaceDom[face][1];
  double h, *v = ut_alloc_1d (3);

  // (c and a are stored as for a cylinder)
  nem_meshing_2d_face_mesh_gmsh_cyl_basis (Tess, face, Nodes, bnodes, bnodeqty, c, a, e1, e2,
                                           pR);
  (*pR) = Tess.DomFaceParms[domface][6];
  (*pr) = Tess.DomFaceParms[domface][7];

  (*prho0) = 0;
  for (i = 0; i < bnodeqty; i++)
  {
    ut_array_1d_sub (c, Nodes.NodeCoo[bnodes[i]], 3, v);
    h = ut_vector_scalprod (v, a);
    for (j = 0; j < 3; j++)
      v[j] -= h * a[j];
    (*prho0) += ut_array_1d_norm (v, 3);
  }
  (*prho0) /= bnodeqty;

  ut_free_1d (&v);

  return;
}
