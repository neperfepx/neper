/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_face_mesh_gmsh_.h"

// This function should not be necessary
void
nem_meshing_2d_face_mesh_gmsh_backproj_fixboundary (struct NODES Nodes,
                                                    int *bnodes, int *lbnodes,
                                                    int bnodeqty,
                                                    struct NODES *pN)
{
  int i;

  for (i = 0; i < bnodeqty; i++)
  {
    if (bnodeqty > (*pN).NodeQty)
    {
      printf ("bnodeqty = %d (*pN).NodeQty = %d\n", bnodeqty, (*pN).NodeQty);
      ut_print_neperbug ();
    }

    if (lbnodes[i] <= 0 || lbnodes[i] > (*pN).NodeQty)
    {
      printf ("lbnodes[i] = %d (*pN).NodeQty = %d\n", lbnodes[i],
              (*pN).NodeQty);
      ut_print_neperbug ();
    }

    if (bnodes[i] <= 0 || bnodes[i] > Nodes.NodeQty)
    {
      printf ("bnodes[i] = %d (*pN).NodeQty = %d\n", bnodes[i],
              (*pN).NodeQty);
      ut_print_neperbug ();
    }

    ut_array_1d_memcpy (Nodes.NodeCoo[bnodes[i]], 3,
                        (*pN).NodeCoo[lbnodes[i]]);
  }

  return;
}

// Unrolling the boundary nodes of a face projected onto a cylinder onto the
// tangent plane of the cylinder at the middle of the face: a node at height h
// along the axis and at angle theta from the middle of the face is mapped
// onto the point of the tangent plane at height h and at distance r * theta
// (arc length) from the tangent line.  theta is made continuous along the
// boundary (a face is a disc), so that the unrolled boundary is valid even
// for a face that covers all directions around the axis.  A face whose
// boundary winds around the axis (an annulus, as obtained by cutting a
// tessellation of too few cells by the cylinder) cannot be unrolled.
void
nem_meshing_2d_face_mesh_gmsh_cyl_unroll (struct TESS Tess, int face, struct NODES Nodes,
                                          int *bnodes, int bnodeqty, double **bnodecoos)
{
  int i, j;
  double r, h, theta, prevtheta = 0, theta0 = 0, *v = ut_alloc_1d (3);
  double *c = ut_alloc_1d (3), *a = ut_alloc_1d (3), *e1 = ut_alloc_1d (3), *e2 = ut_alloc_1d (3);

  nem_meshing_2d_face_mesh_gmsh_cyl_basis (Tess, face, Nodes, bnodes, bnodeqty, c, a, e1, e2,
                                           &r);

  for (i = 0; i < bnodeqty; i++)
  {
    ut_array_1d_sub (c, bnodecoos[i], 3, v);
    h = ut_vector_scalprod (v, a);
    theta = atan2 (ut_vector_scalprod (v, e2), ut_vector_scalprod (v, e1));

    if (i > 0)
    {
      while (theta - prevtheta > M_PI)
        theta -= 2 * M_PI;
      while (theta - prevtheta < -M_PI)
        theta += 2 * M_PI;
    }
    else
      theta0 = theta;
    prevtheta = theta;

    for (j = 0; j < 3; j++)
      bnodecoos[i][j] = c[j] + h * a[j] + r * e1[j] + r * theta * e2[j];
  }

  // closing the boundary: back to theta0 (unless winding around the axis)
  if (fabs (prevtheta - theta0) > M_PI)
    ut_print_message (2, 3, "\nFace %d winds around the cylinder axis and cannot be meshed (too few cells for a curved cylinder domain; use more cells or a faceted domain, -domain \"cylinder(<height>,<diameter>,<facet_nb>)\").\n", face);

  ut_free_1d (&v);
  ut_free_1d (&c);
  ut_free_1d (&a);
  ut_free_1d (&e1);
  ut_free_1d (&e2);

  return;
}

// Rolling a point of the tangent plane back onto the cylinder (inverse of
// nem_meshing_2d_face_mesh_gmsh_cyl_unroll)
void
nem_meshing_2d_face_mesh_gmsh_cyl_roll (struct TESS Tess, int face, struct NODES Nodes,
                                        int *bnodes, int bnodeqty, double *point,
                                        double *rolled)
{
  int j;
  double r, h, theta, *v = ut_alloc_1d (3);
  double *c = ut_alloc_1d (3), *a = ut_alloc_1d (3), *e1 = ut_alloc_1d (3), *e2 = ut_alloc_1d (3);

  nem_meshing_2d_face_mesh_gmsh_cyl_basis (Tess, face, Nodes, bnodes, bnodeqty, c, a, e1, e2,
                                           &r);

  ut_array_1d_sub (c, point, 3, v);
  h = ut_vector_scalprod (v, a);
  theta = ut_vector_scalprod (v, e2) / r;

  for (j = 0; j < 3; j++)
    rolled[j] = c[j] + h * a[j] + r * (cos (theta) * e1[j] + sin (theta) * e2[j]);

  ut_free_1d (&v);
  ut_free_1d (&c);
  ut_free_1d (&a);
  ut_free_1d (&e1);
  ut_free_1d (&e2);

  return;
}

// Projecting the boundary nodes of a face of a sphere (stereographic
// projection, which is conformal) onto the tangent plane at the middle of
// the face (point P0, of unit outgoing normal e), from the opposite point,
// S = c - r e.  The lengths are multiplied by 1 + d^2 / (4 r^2), where d is
// the distance to P0, for which P0 and 4 r^2 are recorded in factparms (see
// nem_mesh_gmsh_sizefact).
void
nem_meshing_2d_face_mesh_gmsh_sph_stereo (struct TESS Tess, int face, struct NODES Nodes,
                                          int *bnodes, int bnodeqty, double **bnodecoos,
                                          double *factparms)
{
  int i, j;
  double r, t, *c = ut_alloc_1d (3), *e = ut_alloc_1d (3), *s = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3), *p0 = ut_alloc_1d (3);

  nem_meshing_2d_face_mesh_gmsh_sph_basis (Tess, face, Nodes, bnodes, bnodeqty, c, e, &r);

  for (j = 0; j < 3; j++)
  {
    s[j] = c[j] - r * e[j];
    p0[j] = c[j] + r * e[j];
  }

  for (i = 0; i < bnodeqty; i++)
  {
    ut_array_1d_sub (c, bnodecoos[i], 3, v);
    t = 2 * r / (ut_vector_scalprod (v, e) + r);
    for (j = 0; j < 3; j++)
      bnodecoos[i][j] = s[j] + t * (bnodecoos[i][j] - s[j]);
  }

  ut_array_1d_memcpy (p0, 3, factparms);
  factparms[3] = 4 * r * r;

  ut_free_1d (&c);
  ut_free_1d (&e);
  ut_free_1d (&s);
  ut_free_1d (&v);
  ut_free_1d (&p0);

  return;
}

// Projecting the nodes of a face mesh back onto the sphere (inverse of
// nem_meshing_2d_face_mesh_gmsh_sph_stereo)
void
nem_meshing_2d_face_mesh_gmsh_sph_unstereo (struct TESS Tess, int face, struct NODES Nodes,
                                            int *bnodes, int bnodeqty, struct NODES *pN)
{
  int i, j;
  double r, t, *c = ut_alloc_1d (3), *e = ut_alloc_1d (3), *s = ut_alloc_1d (3);
  double *d = ut_alloc_1d (3);

  nem_meshing_2d_face_mesh_gmsh_sph_basis (Tess, face, Nodes, bnodes, bnodeqty, c, e, &r);

  for (j = 0; j < 3; j++)
    s[j] = c[j] - r * e[j];

  for (i = 1; i <= (*pN).NodeQty; i++)
  {
    ut_array_1d_sub (s, (*pN).NodeCoo[i], 3, d);
    t = 4 * r * r / ut_vector_scalprod (d, d);
    for (j = 0; j < 3; j++)
      (*pN).NodeCoo[i][j] = s[j] + t * d[j];
  }

  ut_free_1d (&c);
  ut_free_1d (&e);
  ut_free_1d (&s);
  ut_free_1d (&d);

  return;
}

// Unrolling the boundary nodes of a face of a torus (of centre c, axis a,
// major radius R and minor radius r) conformally onto a plane.  A point is
// located by its angle u about the axis (from the middle of the face, made
// continuous along the boundary, as for a cylinder) and its angle v about
// the tube, from the point closest to the axis (at distance rho = R - r cos v
// from the axis), also made continuous along the boundary (a face can cross
// v = pi).  The metric is rho^2 du^2 + r^2 dv^2, which is conformal in (u, w),
// where dw = r dv / rho (see nem_mesh_gmsh_tor_w).  A point is
// mapped onto the
// point of coordinates (rho0 u, rho0 w) of the plane of normal e1 at distance
// rho0 from the axis, where rho0 is the mean distance of the boundary nodes to
// the axis, so that the lengths are multiplied by rho0 / rho, for which c, a,
// rho0, R and r are recorded in factparms (see nem_mesh_gmsh_sizefact).
void
nem_meshing_2d_face_mesh_gmsh_tor_unroll (struct TESS Tess, int face, struct NODES Nodes,
                                          int *bnodes, int bnodeqty, double **bnodecoos,
                                          double *factparms)
{
  int i, j;
  double R, r, rho0, rho, h, u, v, w, prevu = 0, u0 = 0, prevv = 0, v0 = 0;
  double *vect = ut_alloc_1d (3);
  double *c = ut_alloc_1d (3), *a = ut_alloc_1d (3), *e1 = ut_alloc_1d (3), *e2 = ut_alloc_1d (3);

  nem_meshing_2d_face_mesh_gmsh_tor_basis (Tess, face, Nodes, bnodes, bnodeqty, c, a, e1, e2,
                                           &R, &r, &rho0);

  for (i = 0; i < bnodeqty; i++)
  {
    ut_array_1d_sub (c, bnodecoos[i], 3, vect);
    h = ut_vector_scalprod (vect, a);
    for (j = 0; j < 3; j++)
      vect[j] -= h * a[j];
    rho = ut_array_1d_norm (vect, 3);
    u = atan2 (ut_vector_scalprod (vect, e2), ut_vector_scalprod (vect, e1));
    v = atan2 (h, R - rho);

    if (i > 0)
    {
      while (u - prevu > M_PI)
        u -= 2 * M_PI;
      while (u - prevu < -M_PI)
        u += 2 * M_PI;
      while (v - prevv > M_PI)
        v -= 2 * M_PI;
      while (v - prevv < -M_PI)
        v += 2 * M_PI;
    }
    else
    {
      u0 = u;
      v0 = v;
    }
    prevu = u;
    prevv = v;

    w = nem_mesh_gmsh_tor_w (R, r, v);

    for (j = 0; j < 3; j++)
      bnodecoos[i][j] = c[j] + rho0 * (e1[j] + u * e2[j] + w * a[j]);
  }

  ut_array_1d_memcpy (c, 3, factparms);
  ut_array_1d_memcpy (a, 3, factparms + 3);
  factparms[6] = rho0;
  factparms[7] = R;
  factparms[8] = r;

  // closing the boundary: back to u0 and v0 (unless winding around the axis
  // or the tube)
  if (fabs (prevu - u0) > M_PI || fabs (prevv - v0) > M_PI)
    ut_print_message (2, 3, "\nFace %d winds around the torus and cannot be meshed (too few cells for a curved domain).\n", face);

  ut_free_1d (&vect);
  ut_free_1d (&c);
  ut_free_1d (&a);
  ut_free_1d (&e1);
  ut_free_1d (&e2);

  return;
}

// Rolling the nodes of a face mesh back onto the torus (inverse of
// nem_meshing_2d_face_mesh_gmsh_tor_unroll)
void
nem_meshing_2d_face_mesh_gmsh_tor_roll (struct TESS Tess, int face, struct NODES Nodes,
                                        int *bnodes, int bnodeqty, struct NODES *pN)
{
  int i, j;
  double R, r, rho0, rho, h, u, v, w, *vect = ut_alloc_1d (3);
  double *c = ut_alloc_1d (3), *a = ut_alloc_1d (3), *e1 = ut_alloc_1d (3), *e2 = ut_alloc_1d (3);

  nem_meshing_2d_face_mesh_gmsh_tor_basis (Tess, face, Nodes, bnodes, bnodeqty, c, a, e1, e2,
                                           &R, &r, &rho0);

  for (i = 1; i <= (*pN).NodeQty; i++)
  {
    ut_array_1d_sub (c, (*pN).NodeCoo[i], 3, vect);
    u = ut_vector_scalprod (vect, e2) / rho0;
    w = ut_vector_scalprod (vect, a) / rho0;
    v = nem_mesh_gmsh_tor_v (R, r, w);
    rho = R - r * cos (v);
    h = r * sin (v);

    for (j = 0; j < 3; j++)
      (*pN).NodeCoo[i][j] = c[j] + rho * (cos (u) * e1[j] + sin (u) * e2[j]) + h * a[j];
  }

  ut_free_1d (&vect);
  ut_free_1d (&c);
  ut_free_1d (&a);
  ut_free_1d (&e1);
  ut_free_1d (&e2);

  return;
}

// centre node of the edge of nodes a and b (created, unless already
// created); edgenodes[min (a, b)]: other node and centre node of each edge
// (pairs, after the pair quantity); for a boundary edge, the centre node is
// placed at the boundary node between a and b (see
// nem_meshing_2d_face_mesh_gmsh_split)
int
nem_meshing_2d_face_mesh_gmsh_split_edgenode (int *gpos, int gqty, double **bnodecoos,
                                              int bnodeqty, int offset, int a, int b,
                                              int **edgenodes, struct NODES *pN)
{
  int i, min = ut_num_min (a, b), max = ut_num_max (a, b), pos = -1, node;
  int *list = edgenodes[min];
  double coo[3];

  for (i = 0; list && i < list[0]; i++)
    if (list[1 + 2 * i] == max)
      return list[2 + 2 * i];

  // boundary edge (between consecutive boundary nodes passed to Gmsh)
  if (gpos[a] != -1 && gpos[b] != -1)
  {
    if (gpos[b] == (gpos[a] + 1) % gqty)
      pos = (offset + 2 * gpos[a] + 1) % bnodeqty;
    else if (gpos[a] == (gpos[b] + 1) % gqty)
      pos = (offset + 2 * gpos[b] + 1) % bnodeqty;
  }

  if (pos != -1)
    ut_array_1d_memcpy (bnodecoos[pos], 3, coo);
  else
    for (i = 0; i < 3; i++)
      coo[i] = 0.5 * ((*pN).NodeCoo[a][i] + (*pN).NodeCoo[b][i]);

  node = neut_nodes_addnode (pN, coo, -1);

  if (!list)
    list = ut_alloc_1d_int (3);
  else
    list = ut_realloc_1d_int (list, 2 * list[0] + 3);
  list[1 + 2 * list[0]] = max;
  list[2 + 2 * list[0]] = node;
  list[0]++;
  edgenodes[min] = list;

  return node;
}

// minimum scaled Jacobian of the elts of a mesh of quad elts
double
nem_meshing_2d_face_mesh_gmsh_sjmin (struct NODES N, struct MESH M)
{
  int i;
  double sjmin = 1;

  for (i = 1; i <= M.EltQty; i++)
    sjmin = ut_num_min (sjmin, neut_elt_sj_quad (N.NodeCoo[M.EltNodes[i][0]],
                                                 N.NodeCoo[M.EltNodes[i][1]],
                                                 N.NodeCoo[M.EltNodes[i][2]],
                                                 N.NodeCoo[M.EltNodes[i][3]]));

  return sjmin;
}
