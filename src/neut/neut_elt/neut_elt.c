/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_elt_.h"

void
neut_elt_name_prop (char *format, char *name, char *type, int *pDimension,
                    int *pOrder)
{
  if (strcmp (format, "geof") == 0)
  {
    if (strcmp (name, "c3d4") == 0)
    {
      sprintf (type, "tri");
      (*pDimension) = 3;
      (*pOrder) = 1;
    }
    else if (strcmp (name, "c3d10") == 0)
    {
      sprintf (type, "tri");
      (*pDimension) = 3;
      (*pOrder) = 2;
    }
    else
    {
      ut_print_message (2, 0, "Elt name unknown.\n");
      abort ();
    }
  }
  else
  {
    ut_print_message (2, 0, "Format unknown.\n");
    abort ();
  }

  return;
}

int
neut_elt_nodeqty (char *type, int dim, int order)
{
  int res;
  // numbers of nodes, for each dimension and order [dim][order]
  static const int TriNodesPerElt[4][3] =
    { {0, 1, 1}, {0, 2, 3}, {0, 3, 6}, {0, 4, 10} };
  static const int QuadNodesPerElt[4][3] =
    { {0, 1, 1}, {0, 2, 3}, {0, 4, 8}, {0, 8, 20} };
  static const int Quad9NodesPerElt[4][3] =
    { {0, 1, 1}, {0, 2, 3}, {0, 4, 9}, {0, 8, 21} };
  static const int Quad6NodesPerElt[4][3] =
    { {0, 1, 1}, {0, 2, 3}, {0, 4, 6}, {0, 0, 0} };
  static const int TriPrismNodesPerElt[4][3] =
    { {0, 0, 0}, {0, 0, 0}, {0, 4, 8}, {0, 6, 15} };

  if (dim > 3 || order > 2)
  {
    ut_print_message (2, 0, "Mesh dimension or order out of bounds\n");
    printf ("dim = %d, order = %d\n", dim, order);
    abort ();
  }

  res = -1;
  if (type == NULL || !strcmp (type, "tri"))
    res = TriNodesPerElt[dim][order];
  else if (!strcmp (type, "quad"))
    res = QuadNodesPerElt[dim][order];
  else if (!strcmp (type, "quad9"))
    res = Quad9NodesPerElt[dim][order];
  else if (!strcmp (type, "quad6"))
    res = Quad6NodesPerElt[dim][order];
  else if (!strcmp (type, "triprism"))
    res = TriPrismNodesPerElt[dim][order];
  else
    ut_print_message (2, 0, "neut_elt_nodeqty: unknown elt type `%s'.\n",
                      type);

  return res;
}

int
neut_elt_boundqty (char *type, int dim)
{
  int res;
  int *TriFacesPerElt = ut_alloc_1d_int (4);

  TriFacesPerElt[0] = 1;
  TriFacesPerElt[1] = 2;
  TriFacesPerElt[2] = 3;
  TriFacesPerElt[3] = 4;

  int *QuadFacesPerElt = ut_alloc_1d_int (4);

  QuadFacesPerElt[0] = 1;
  QuadFacesPerElt[1] = 2;
  QuadFacesPerElt[2] = 4;
  QuadFacesPerElt[3] = 6;

  res = -1;
  if (type == NULL || strcmp (type, "tri") == 0)
    res = TriFacesPerElt[dim];
  else if (strcmp (type, "quad") == 0)
    res = QuadFacesPerElt[dim];
  else
    ut_print_message (2, 0, "neut_elt_boundqty: unknown elt type `%s'.\n",
                      type);

  ut_free_1d_int (&TriFacesPerElt);
  ut_free_1d_int (&QuadFacesPerElt);

  return res;
}

int
neut_elt_boundlist (char *type, int dim, int ***pbounds, int *pboundqty,
                    int *pnodeqty)
{
  int NodeQty;

  (*pboundqty) = neut_elt_boundqty (type, dim);
  NodeQty = neut_elt_nodeqty (type, dim - 1, 1);

  if (pnodeqty)
    (*pnodeqty) = NodeQty;

  (*pbounds) = ut_alloc_2d_int (*pboundqty, NodeQty);

  if (dim == 1)
  {
    (*pbounds)[0][0] = 0;
    (*pbounds)[1][0] = 1;
  }
  else if (dim == 2)
  {
    if (strcmp (type, "tri") == 0)
    {
      (*pbounds)[0][0] = 0;
      (*pbounds)[0][1] = 1;
      (*pbounds)[1][0] = 1;
      (*pbounds)[1][1] = 2;
      (*pbounds)[2][0] = 0;
      (*pbounds)[2][1] = 2;
    }
    else if (strcmp (type, "quad") == 0)
    {
      (*pbounds)[0][0] = 0;
      (*pbounds)[0][1] = 1;
      (*pbounds)[1][0] = 1;
      (*pbounds)[1][1] = 2;
      (*pbounds)[2][0] = 2;
      (*pbounds)[2][1] = 3;
      (*pbounds)[3][0] = 3;
      (*pbounds)[3][1] = 0;
    }
  }

  // 3D
  else if (dim == 3)
  {
    if (strcmp (type, "tri") == 0)
    {
      (*pbounds)[0][0] = 0;
      (*pbounds)[0][1] = 1;
      (*pbounds)[0][2] = 2;
      (*pbounds)[1][0] = 1;
      (*pbounds)[1][1] = 2;
      (*pbounds)[1][2] = 3;
      (*pbounds)[2][0] = 2;
      (*pbounds)[2][1] = 3;
      (*pbounds)[2][2] = 0;
      (*pbounds)[3][0] = 0;
      (*pbounds)[3][1] = 1;
      (*pbounds)[3][2] = 3;
    }
    else if (strcmp (type, "quad") == 0)
    {
      (*pbounds)[0][0] = 0;
      (*pbounds)[0][1] = 1;
      (*pbounds)[0][2] = 2;
      (*pbounds)[0][3] = 3;
      (*pbounds)[1][0] = 4;
      (*pbounds)[1][1] = 5;
      (*pbounds)[1][2] = 6;
      (*pbounds)[1][3] = 7;
      (*pbounds)[2][0] = 0;
      (*pbounds)[2][1] = 1;
      (*pbounds)[2][2] = 5;
      (*pbounds)[2][3] = 4;
      (*pbounds)[3][0] = 3;
      (*pbounds)[3][1] = 2;
      (*pbounds)[3][2] = 6;
      (*pbounds)[3][3] = 7;
      (*pbounds)[4][0] = 1;
      (*pbounds)[4][1] = 2;
      (*pbounds)[4][2] = 6;
      (*pbounds)[4][3] = 5;
      (*pbounds)[5][0] = 0;
      (*pbounds)[5][1] = 3;
      (*pbounds)[5][2] = 7;
      (*pbounds)[5][3] = 4;
    }
    else
      abort ();
  }

  return (*pboundqty > 0) ? 0 : -1;
}

int
neut_elt_order (char *type, int dim, int NodeQty)
{
  int res = -1;

  if (strcmp (type, "tri") == 0)
  {
    if (dim == 3)
    {
      if (NodeQty == 4)
        res = 1;
      else if (NodeQty == 10)
        res = 2;
    }
    if (dim == 2)
    {
      if (NodeQty == 3)
        res = 1;
      else if (NodeQty == 6)
        res = 2;
    }
    if (dim == 1)
    {
      if (NodeQty == 2)
        res = 1;
      else if (NodeQty == 3)
        res = 2;
    }
    if (dim == 0)
      res = 1;
  }

  return res;
}

int
neut_elt_gtype_prop (int gtype, char *type, int *pdim, int *porder)
{
  int status;

  status = 0;
  if (gtype == 1)               // 2-node line
  {
    sprintf (type, "tri");
    (*pdim) = 1;
    (*porder) = 1;
  }
  else if (gtype == 2)          // 3-node triangle
  {
    sprintf (type, "tri");
    (*pdim) = 2;
    (*porder) = 1;
  }
  else if (gtype == 3)          // 4-node quadrangle
  {
    sprintf (type, "quad");
    (*pdim) = 2;
    (*porder) = 1;
  }
  else if (gtype == 4)          // 4-node tetrahedron
  {
    sprintf (type, "tri");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype == 5)          // 8-node hexahedron
  {
    sprintf (type, "quad");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype == 6)          // 6-node prism
  {
    sprintf (type, "triprism");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype == 7)          // 5-node pyramid
  {
    sprintf (type, "pyr");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype == 8)          // 3-node 2nd order line
  {
    sprintf (type, "tri");
    (*pdim) = 1;
    (*porder) = 2;
  }
  else if (gtype == 9)          // 6-node 2nd order triangle
  {
    sprintf (type, "tri");
    (*pdim) = 2;
    (*porder) = 2;
  }
  else if (gtype == 11)         // 10-node 2nd order tetrahedron
  {
    sprintf (type, "tri");
    (*pdim) = 3;
    (*porder) = 2;
  }
  else if (gtype == 15)         // 1-node point
  {
    sprintf (type, "tri");
    (*pdim) = 0;
    (*porder) = 1;
  }
  else if (gtype == 16)         // 8-node 2nd order quadrangle
  {
    sprintf (type, "quad");
    (*pdim) = 2;
    (*porder) = 2;
  }
  else if (gtype == 17)         // 8-node 2nd order quadrangle
  {
    sprintf (type, "quad");
    (*pdim) = 3;
    (*porder) = 2;
  }
  else if (gtype == 18)         // 15-node 2nd order prism
  {
    sprintf (type, "triprism");
    (*pdim) = 3;
    (*porder) = 2;
  }
  else if (gtype == 100)        // 6-node 2nd order quadrangle (2D cohesive elt, Neper-specific)
  {
    sprintf (type, "quad6");
    (*pdim) = 2;
    (*porder) = 2;
  }
  else
    status = -1;

  return status;
}

int
neut_elt_dim_gtype_order (int dim, int gtype)
{
  if (dim == 0 && gtype == 15)
    return 1;
  else if (dim == 1 && gtype == 1)
    return 1;
  else if (dim == 1 && gtype == 8)
    return 2;
  else if (dim == 2 && gtype == 2)
    return 1;
  else if (dim == 2 && gtype == 9)
    return 2;
  else if (dim == 3 && gtype == 4)
    return 1;
  else if (dim == 3 && gtype == 11)
    return 2;
  else
    return -1;
}

double
neut_elt_rr_3d (double *p1, double *p2, double *p3, double *p4)
{
  int i, j, k, l;               /* loop indices */
  double facenormal[4][3];      /* the normals of each face of the tet */
  double facearea2[4];          /* areas of the faces of the tet */
  double Z;                     /* quantity needed for circumradius */
  double facesum = 0.0;         /* sum of the areas of the faces */
  double vol;
  double **point = ut_alloc_1d_pdouble (4);
  point[0] = p1;
  point[1] = p2;
  point[2] = p3;
  point[3] = p4;

  /* calculate the volume of the tetrahedron */
  vol = ut_space_tet_volume (p1, p2, p3, p4);

  /* for each vertex/face of the tetrahedron */
  for (i = 0; i < 4; i++)
  {
    j = (i + 1) & 3;
    if ((i & 1) == 0)
    {
      k = (i + 3) & 3;
      l = (i + 2) & 3;
    }
    else
    {
      k = (i + 2) & 3;
      l = (i + 3) & 3;
    }

    /* compute the normal for each face */
    facenormal[i][0] =
      (point[k][1] - point[j][1]) * (point[l][2] - point[j][2]) -
      (point[k][2] - point[j][2]) * (point[l][1] - point[j][1]);
    facenormal[i][1] =
      (point[k][2] - point[j][2]) * (point[l][0] - point[j][0]) -
      (point[k][0] - point[j][0]) * (point[l][2] - point[j][2]);
    facenormal[i][2] =
      (point[k][0] - point[j][0]) * (point[l][1] - point[j][1]) -
      (point[k][1] - point[j][1]) * (point[l][0] - point[j][0]);

    /* compute (2 *area)^2 for this face */
    facearea2[i] =
      facenormal[i][0] * facenormal[i][0] +
      facenormal[i][1] * facenormal[i][1] +
      facenormal[i][2] * facenormal[i][2];
    facesum += sqrt (facearea2[i]) * 0.5;
  }

  /* compute Z */
  Z = getZ (p1, p2, p3, p4);

  /* now we are ready to compute the radius ratio, which is
     (108 * V^2) / Z (A1 + A2 + A3 + A4) */

  ut_free_1d_pdouble (&point);

  return (108 * vol * vol) / (Z * facesum);
}

/* compute Z, a quantity associated with circumradius computation
   TODO this code is lifted from Jonathan's tetcircumcenter computation
   in primitives.c */
double
getZ (double *tetorg, double *tetdest, double *tetfapex, double *tettapex)
{
  double xot, yot, zot, xdt, ydt, zdt, xft, yft, zft;
  double otlength, dtlength, ftlength;
  double xcrossdf, ycrossdf, zcrossdf;
  double xcrossfo, ycrossfo, zcrossfo;
  double xcrossod, ycrossod, zcrossod;
  double xct, yct, zct;

  /* Use coordinates relative to the apex of the tetrahedron. */
  xot = tetorg[0] - tettapex[0];
  yot = tetorg[1] - tettapex[1];
  zot = tetorg[2] - tettapex[2];
  xdt = tetdest[0] - tettapex[0];
  ydt = tetdest[1] - tettapex[1];
  zdt = tetdest[2] - tettapex[2];
  xft = tetfapex[0] - tettapex[0];
  yft = tetfapex[1] - tettapex[1];
  zft = tetfapex[2] - tettapex[2];
  /* Squares of lengths of the origin, destination, and face apex edges. */
  otlength = xot * xot + yot * yot + zot * zot;
  dtlength = xdt * xdt + ydt * ydt + zdt * zdt;
  ftlength = xft * xft + yft * yft + zft * zft;
  /* Cross products of the origin, destination, and face apex vectors. */
  xcrossdf = ydt * zft - yft * zdt;
  ycrossdf = zdt * xft - zft * xdt;
  zcrossdf = xdt * yft - xft * ydt;
  xcrossfo = yft * zot - yot * zft;
  ycrossfo = zft * xot - zot * xft;
  zcrossfo = xft * yot - xot * yft;
  xcrossod = yot * zdt - ydt * zot;
  ycrossod = zot * xdt - zdt * xot;
  zcrossod = xot * ydt - xdt * yot;

  /* Calculate offset (from apex) of circumcenter. */
  xct = (otlength * xcrossdf + dtlength * xcrossfo + ftlength * xcrossod);
  yct = (otlength * ycrossdf + dtlength * ycrossfo + ftlength * ycrossod);
  zct = (otlength * zcrossdf + dtlength * zcrossfo + ftlength * zcrossod);

  /* Calculate the length of this vector, which is Z */
  return sqrt (xct * xct + yct * yct + zct * zct);
}

/* returns the radius ratio for a triangle
 * rr = 16 * A^2 / (l1.l2.l3.(l1+l2+l3)) */
double
neut_elt_rr_2d (double *p1, double *p2, double *p3)
{
  double l1, l2, l3, area;

  area = ut_space_triangle_area (p1, p2, p3);
  l1 = ut_space_dist (p1, p2);
  l2 = ut_space_dist (p2, p3);
  l3 = ut_space_dist (p3, p1);

  return 16 * area * area / (l1 * l2 * l3 * (l1 + l2 + l3));
}

// scaled Jacobian of a quadrangle (minimum sine of the corner angles, signed
// with respect to the normal of the quadrangle, given by its diagonals)
double
neut_elt_sj_quad (double *p1, double *p2, double *p3, double *p4)
{
  int i, j, k;
  double *p[4] = { p1, p2, p3, p4 }, d1[3], d2[3], n[3], u[3], v[3], w[3], nu, nv, sj = 1;

  ut_array_1d_sub (p1, p3, 3, d1);
  ut_array_1d_sub (p2, p4, 3, d2);
  ut_vector_vectprod (d1, d2, n);
  if (ut_vector_norm (n) == 0)
    return 0;
  ut_vector_uvect (n, n);

  for (i = 0; i < 4; i++)
  {
    j = (i + 1) % 4;
    k = (i + 3) % 4;
    ut_array_1d_sub (p[i], p[j], 3, u);
    ut_array_1d_sub (p[i], p[k], 3, v);
    nu = ut_vector_norm (u);
    nv = ut_vector_norm (v);
    if (nu == 0 || nv == 0)
      return 0;
    ut_vector_vectprod (u, v, w);
    sj = ut_num_min (sj, ut_vector_scalprod (w, n) / (nu * nv));
  }

  return sj;
}

// rho factor as defined in Gmsh
double
neut_elt_3d_rho (double *p1, double *p2, double *p3, double *p4)
{
  double rho;
  double *l = ut_alloc_1d (6);

  l[1] = ut_space_dist (p1, p3);
  l[0] = ut_space_dist (p1, p2);
  l[2] = ut_space_dist (p1, p4);
  l[3] = ut_space_dist (p2, p3);
  l[4] = ut_space_dist (p2, p4);
  l[5] = ut_space_dist (p3, p4);

  rho = ut_array_1d_min (l, 6) / ut_array_1d_max (l, 6);

  ut_free_1d (&l);

  return rho;
}

void
neut_elt_nodes_facet (char *type, int dim, int *nodes, int *facetnodes, int *pfacet,
                      int *pfacetori)
{
  int i;
  int *pos = NULL;

  if (type && !strcmp (type, "quad"))
  {
    neut_elt_nodes_facet_quad (dim, nodes, facetnodes, pfacet, pfacetori);
    return;
  }

  pos = ut_alloc_1d_int (dim);

  for (i = 0; i < dim; i++)
    pos[i] = ut_array_1d_int_eltpos (nodes, dim + 1, facetnodes[i]);

  if (dim == 3)
  {
    if ((pos[0] == 0 && pos[1] == 2 && pos[2] == 1)
        || (pos[0] == 2 && pos[1] == 1 && pos[2] == 0) || (pos[0] == 1
                                                           && pos[1] == 0
                                                           && pos[2] == 2))
    {
      (*pfacet) = 1;
      (*pfacetori) = 1;
    }
    else if ((pos[0] == 1 && pos[1] == 2 && pos[2] == 0)
             || (pos[0] == 0 && pos[1] == 1 && pos[2] == 2) || (pos[0] == 2
                                                                && pos[1] == 0
                                                                && pos[2] ==
                                                                1))
    {
      (*pfacet) = 1;
      (*pfacetori) = -1;
    }
    else if ((pos[0] == 0 && pos[1] == 1 && pos[2] == 3)
             || (pos[0] == 1 && pos[1] == 3 && pos[2] == 0) || (pos[0] == 3
                                                                && pos[1] == 0
                                                                && pos[2] ==
                                                                1))
    {
      (*pfacet) = 2;
      (*pfacetori) = 1;
    }
    else if ((pos[0] == 3 && pos[1] == 1 && pos[2] == 0)
             || (pos[0] == 0 && pos[1] == 3 && pos[2] == 1) || (pos[0] == 1
                                                                && pos[1] == 0
                                                                && pos[2] ==
                                                                3))
    {
      (*pfacet) = 2;
      (*pfacetori) = -1;
    }
    else if ((pos[0] == 1 && pos[1] == 2 && pos[2] == 3)
             || (pos[0] == 2 && pos[1] == 3 && pos[2] == 1) || (pos[0] == 3
                                                                && pos[1] == 1
                                                                && pos[2] ==
                                                                2))
    {
      (*pfacet) = 3;
      (*pfacetori) = 1;
    }
    else if ((pos[0] == 3 && pos[1] == 2 && pos[2] == 1)
             || (pos[0] == 1 && pos[1] == 3 && pos[2] == 2) || (pos[0] == 2
                                                                && pos[1] == 1
                                                                && pos[2] ==
                                                                3))
    {
      (*pfacet) = 3;
      (*pfacetori) = -1;
    }
    else if ((pos[0] == 0 && pos[1] == 3 && pos[2] == 2)
             || (pos[0] == 3 && pos[1] == 2 && pos[2] == 0) || (pos[0] == 2
                                                                && pos[1] == 0
                                                                && pos[2] ==
                                                                3))
    {
      (*pfacet) = 4;
      (*pfacetori) = 1;
    }
    else if ((pos[0] == 2 && pos[1] == 3 && pos[2] == 0)
             || (pos[0] == 0 && pos[1] == 2 && pos[2] == 3) || (pos[0] == 3
                                                                && pos[1] == 0
                                                                && pos[2] ==
                                                                2))
    {
      (*pfacet) = 4;
      (*pfacetori) = -1;
    }
    else
      abort ();
  }

  else if (dim == 2)
  {
    if (pos[0] == 0 && pos[1] == 1)
    {
      (*pfacet) = 1;
      (*pfacetori) = 1;
    }
    else if (pos[0] == 1 && pos[1] == 0)
    {
      (*pfacet) = 1;
      (*pfacetori) = 1;
    }
    else if (pos[0] == 1 && pos[1] == 2)
    {
      (*pfacet) = 2;
      (*pfacetori) = 1;
    }
    else if (pos[0] == 2 && pos[1] == 1)
    {
      (*pfacet) = 2;
      (*pfacetori) = -1;
    }
    else if (pos[0] == 2 && pos[1] == 0)
    {
      (*pfacet) = 3;
      (*pfacetori) = 1;
    }
    else if (pos[0] == 0 && pos[1] == 2)
    {
      (*pfacet) = 3;
      (*pfacetori) = -1;
    }
    else
      abort ();
  }

  else
    abort ();

  ut_free_1d_int (&pos);

  return;
}

// facets of quad (2D) and hex (3D) elts, as defined in Abaqus (facet i is
// Abaqus' Si); as for tri and tet elts, the facet orientation is 1 if the
// facet nodes are in the reverse order of the Abaqus definition, and -1
// otherwise
void
neut_elt_nodes_facet_quad (int dim, int *nodes, int *facetnodes, int *pfacet,
                           int *pfacetori)
{
  int i, j, k, n, facetqty, pos[4], start;
  int quadfacets[4][2] = { {0, 1}, {1, 2}, {2, 3}, {3, 0} };
  int hexfacets[6][4] = { {0, 1, 2, 3}, {4, 7, 6, 5}, {0, 4, 5, 1},
                          {1, 5, 6, 2}, {2, 6, 7, 3}, {3, 7, 4, 0} };
  int *facet = NULL;

  if (dim != 2 && dim != 3)
    abort ();

  n = (dim == 2) ? 2 : 4;
  facetqty = (dim == 2) ? 4 : 6;

  for (i = 0; i < n; i++)
    pos[i] = ut_array_1d_int_eltpos (nodes, (dim == 2) ? 4 : 8, facetnodes[i]);

  for (k = 0; k < facetqty; k++)
  {
    facet = (dim == 2) ? quadfacets[k] : hexfacets[k];

    for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
        if (facet[j] == pos[i])
          break;
      if (j == n)
        break;
    }

    if (i == n)
      break;
  }

  if (k == facetqty)
    abort ();

  (*pfacet) = k + 1;

  start = ut_array_1d_int_eltpos (facet, n, pos[0]);
  (*pfacetori) = (pos[1] == facet[(start + 1) % n]) ? -1 : 1;

  return;
}

void
neut_elt_facet_nodes (char *type, int dim, int order, int *nodes, int facet,
                      int facetori, int *facetnodes)
{
  (void) type;

  if (dim == 2)
  {
    if (order == 1)
    {
      if (facet == 1)
      {
        facetnodes[0] = nodes[0];
        facetnodes[1] = nodes[1];
      }
      else if (facet == 2)
      {
        facetnodes[0] = nodes[1];
        facetnodes[1] = nodes[2];
      }
      else if (facet == 3)
      {
        facetnodes[0] = nodes[2];
        facetnodes[1] = nodes[0];
      }
      else
        abort ();
    }
    else if (order == 2)
    {
      if (facet == 1)
      {
        facetnodes[0] = nodes[0];
        facetnodes[1] = nodes[1];
        facetnodes[2] = nodes[3];
      }
      else if (facet == 2)
      {
        facetnodes[0] = nodes[1];
        facetnodes[1] = nodes[2];
        facetnodes[2] = nodes[4];
      }
      else if (facet == 3)
      {
        facetnodes[0] = nodes[2];
        facetnodes[1] = nodes[0];
        facetnodes[2] = nodes[5];
      }
      else
        abort ();
    }
  }

  else if (dim == 3)
  {
    if (order == 1)
    {
      if (facet == 1)
      {
        facetnodes[0] = nodes[0];
        facetnodes[1] = nodes[2];
        facetnodes[2] = nodes[1];
      }
      else if (facet == 2)
      {
        facetnodes[0] = nodes[0];
        facetnodes[1] = nodes[1];
        facetnodes[2] = nodes[3];
      }
      else if (facet == 3)
      {
        facetnodes[0] = nodes[1];
        facetnodes[1] = nodes[2];
        facetnodes[2] = nodes[3];
      }
      else if (facet == 4)
      {
        facetnodes[0] = nodes[0];
        facetnodes[1] = nodes[3];
        facetnodes[2] = nodes[2];
      }
      else
        abort ();

      if (facetori == -1)
        ut_array_1d_int_reverseelts (facetnodes, 3);
    }
    else if (order == 2)
      abort ();
  }

  return;
  }

// http://pages.suddenlink.net/physics/FEA/FEM_basis_functions_for_a_triangle.pdf
void
neut_elt_tri_shapefct (double *a, double *b, double *c, double *p,
                       double *val)
{
  val[0] =
    ((p[0] - c[0]) * (b[1] - c[1]) -
     (b[0] - c[0]) * (p[1] - c[1])) / ((a[0] - c[0]) * (b[1] - c[1]) - (b[0] -
                                                                        c[0])
                                       * (a[1] - c[1]));

  val[1] =
    ((p[0] - a[0]) * (c[1] - a[1]) -
     (c[0] - a[0]) * (p[1] - a[1])) / ((b[0] - a[0]) * (c[1] - a[1]) - (c[0] -
                                                                        a[0])
                                       * (b[1] - a[1]));

  val[2] =
    ((p[0] - b[0]) * (a[1] - b[1]) -
     (a[0] - b[0]) * (p[1] - b[1])) / ((c[0] - b[0]) * (a[1] - b[1]) - (a[0] -
                                                                        b[0])
                                       * (c[1] - b[1]));

  if (ut_num_equal (ut_array_1d_sum (val, 3), 1, 1e-6) != 1)
    abort ();

  return;
}

void
neut_elt_orderarrays_3d (char *elttype, int ***pfir, int *pnodeqty_tot,
                         int ***psec, int *pnodeqty_o1)
{
  int tmp_o1, tmp_tot;

  tmp_o1 = neut_elt_nodeqty (elttype, 3, 1);
  tmp_tot = neut_elt_nodeqty (elttype, 3, 2);

  if (pfir)
    (*pfir) = ut_alloc_2d_int (tmp_tot, 2);

  if (psec)
    (*psec) = ut_alloc_2d_int (tmp_o1, tmp_o1);

  if (!strcmp (elttype, "tri"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 4;
      (*psec)[1][2] = (*psec)[2][1] = 5;
      (*psec)[2][0] = (*psec)[0][2] = 6;
      (*psec)[3][0] = (*psec)[0][3] = 7;
      (*psec)[2][3] = (*psec)[3][2] = 8;
      (*psec)[3][1] = (*psec)[1][3] = 9;
    }

    if (pfir)
    {
      (*pfir)[4][0] = 0;
      (*pfir)[4][1] = 1;
      (*pfir)[5][0] = 1;
      (*pfir)[5][1] = 2;
      (*pfir)[6][0] = 2;
      (*pfir)[6][1] = 0;
      (*pfir)[7][0] = 3;
      (*pfir)[7][1] = 0;
      (*pfir)[8][0] = 2;
      (*pfir)[8][1] = 3;
      (*pfir)[9][0] = 3;
      (*pfir)[9][1] = 1;
    }
  }
  else if (!strcmp (elttype, "quad"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 8;
      (*psec)[4][5] = (*psec)[5][4] = 16;
      (*psec)[3][2] = (*psec)[2][3] = 13;
      (*psec)[7][6] = (*psec)[6][7] = 19;

      (*psec)[0][4] = (*psec)[4][0] = 10;
      (*psec)[1][5] = (*psec)[5][1] = 12;
      (*psec)[3][7] = (*psec)[7][3] = 15;
      (*psec)[2][6] = (*psec)[6][2] = 14;

      (*psec)[0][3] = (*psec)[3][0] = 9;
      (*psec)[1][2] = (*psec)[2][1] = 11;
      (*psec)[4][7] = (*psec)[7][4] = 17;
      (*psec)[5][6] = (*psec)[6][5] = 18;
    }

    if (pfir)
    {
      (*pfir)[8][0] = 0;
      (*pfir)[8][1] = 1;
      (*pfir)[9][0] = 0;
      (*pfir)[9][1] = 3;
      (*pfir)[10][0] = 0;
      (*pfir)[10][1] = 4;
      (*pfir)[11][0] = 1;
      (*pfir)[11][1] = 2;
      (*pfir)[12][0] = 1;
      (*pfir)[12][1] = 5;
      (*pfir)[13][0] = 2;
      (*pfir)[13][1] = 3;
      (*pfir)[14][0] = 2;
      (*pfir)[14][1] = 6;
      (*pfir)[15][0] = 3;
      (*pfir)[15][1] = 7;
      (*pfir)[16][0] = 4;
      (*pfir)[16][1] = 5;
      (*pfir)[17][0] = 4;
      (*pfir)[17][1] = 7;
      (*pfir)[18][0] = 5;
      (*pfir)[18][1] = 6;
      (*pfir)[19][0] = 6;
      (*pfir)[19][1] = 7;
    }
  }
  else if (!strcmp (elttype, "triprism"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 6;
      (*psec)[2][0] = (*psec)[0][2] = 7;
      (*psec)[3][0] = (*psec)[0][3] = 8;
      (*psec)[1][2] = (*psec)[2][1] = 9;
      (*psec)[1][4] = (*psec)[4][1] = 10;
      (*psec)[2][5] = (*psec)[5][2] = 11;
      (*psec)[3][4] = (*psec)[4][3] = 12;
      (*psec)[3][5] = (*psec)[5][3] = 13;
      (*psec)[4][5] = (*psec)[5][4] = 14;
    }

    if (pfir)
    {
      (*pfir)[6][0] = 0;
      (*pfir)[6][1] = 1;
      (*pfir)[7][0] = 2;
      (*pfir)[7][1] = 0;
      (*pfir)[8][0] = 0;
      (*pfir)[8][1] = 3;
      (*pfir)[9][0] = 2;
      (*pfir)[9][1] = 1;
      (*pfir)[10][0] = 4;
      (*pfir)[10][1] = 1;
      (*pfir)[11][0] = 5;
      (*pfir)[11][1] = 2;
      (*pfir)[12][0] = 3;
      (*pfir)[12][1] = 4;
      (*pfir)[13][0] = 3;
      (*pfir)[13][1] = 5;
      (*pfir)[14][0] = 4;
      (*pfir)[14][1] = 5;
    }
  }

  if (pnodeqty_o1)
    *pnodeqty_o1 = tmp_o1;
  if (pnodeqty_tot)
    *pnodeqty_tot = tmp_tot;

  return;
}

void
neut_elt_orderarrays_2d (char *elttype, int ***pfir, int *pnodeqty_tot,
                         int ***psec, int *pnodeqty_o1)
{
  int tmp_o1, tmp_tot;

  tmp_o1 = neut_elt_nodeqty (elttype, 2, 1);
  tmp_tot = neut_elt_nodeqty (elttype, 2, 2);

  if (pfir)
    (*pfir) = ut_alloc_2d_int (tmp_tot, 2);

  if (psec)
    (*psec) = ut_alloc_2d_int (tmp_o1, tmp_o1);

  if (!strcmp (elttype, "tri"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 3;
      (*psec)[1][2] = (*psec)[2][1] = 4;
      (*psec)[2][0] = (*psec)[0][2] = 5;
    }

    if (pfir)
    {
      (*pfir)[3][0] = 0;
      (*pfir)[3][1] = 1;
      (*pfir)[4][0] = 1;
      (*pfir)[4][1] = 2;
      (*pfir)[5][0] = 2;
      (*pfir)[5][1] = 0;
    }
  }

  else if (!strcmp (elttype, "quad"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 4;
      (*psec)[1][2] = (*psec)[2][1] = 5;
      (*psec)[2][3] = (*psec)[3][2] = 6;
      (*psec)[3][0] = (*psec)[0][3] = 7;
    }

    if (pfir)
    {
      (*pfir)[4][0] = 0;
      (*pfir)[4][1] = 1;
      (*pfir)[5][0] = 1;
      (*pfir)[5][1] = 2;
      (*pfir)[6][0] = 2;
      (*pfir)[6][1] = 3;
      (*pfir)[7][0] = 3;
      (*pfir)[7][1] = 0;
    }
  }

  else if (!strcmp (elttype, "quad6"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 4;
      (*psec)[2][3] = (*psec)[3][2] = 5;
    }

    if (pfir)
    {
      (*pfir)[4][0] = 0;
      (*pfir)[4][1] = 1;
      (*pfir)[5][0] = 2;
      (*pfir)[5][1] = 3;
    }
  }

  else if (!strcmp (elttype, "quad9"))
  {
    if (psec)
    {
      (*psec)[0][1] = (*psec)[1][0] = 4;
      (*psec)[1][2] = (*psec)[2][1] = 5;
      (*psec)[2][3] = (*psec)[3][2] = 6;
      (*psec)[3][0] = (*psec)[0][3] = 7;
    }

    if (pfir)
    {
      (*pfir)[4][0] = 0;
      (*pfir)[4][1] = 1;
      (*pfir)[5][0] = 1;
      (*pfir)[5][1] = 2;
      (*pfir)[6][0] = 2;
      (*pfir)[6][1] = 3;
      (*pfir)[7][0] = 3;
      (*pfir)[7][1] = 0;
      (*pfir)[8][0] = -1;       // this means the node is not between 1st-order nodes
      (*pfir)[8][1] = -1;       // this means the node is not between 1st-order nodes
    }
  }

  else
    abort ();

  if (pnodeqty_o1)
    *pnodeqty_o1 = tmp_o1;
  if (pnodeqty_tot)
    *pnodeqty_tot = tmp_tot;

  return;
}

void
neut_elt_orderarrays_1d (char *elttype, int ***pfir, int *pnodeqty_tot,
                         int ***psec, int *pnodeqty_o1)
{
  int tmp_o1, tmp_tot;

  tmp_o1 = neut_elt_nodeqty (elttype, 1, 1);
  tmp_tot = neut_elt_nodeqty (elttype, 1, 2);

  if (psec)
  {
    (*psec) = ut_alloc_2d_int (tmp_o1, tmp_o1);
    (*psec)[0][1] = (*psec)[1][0] = 2;
  }

  if (pfir)
  {
    (*pfir) = ut_alloc_2d_int (tmp_tot, 2);
    (*pfir)[2][0] = 0;
    (*pfir)[2][1] = 1;
  }

  if (pnodeqty_o1)
    *pnodeqty_o1 = tmp_o1;
  if (pnodeqty_tot)
    *pnodeqty_tot = tmp_tot;

  return;
}

void
neut_elt_orderarrays (char *elttype, int dim, int ***pfir, int *pnodeqty_tot,
                      int ***psec, int *pnodeqty_o1)
{
  if (dim == 1)
    neut_elt_orderarrays_1d (elttype, pfir, pnodeqty_tot, psec, pnodeqty_o1);
  else if (dim == 2)
    neut_elt_orderarrays_2d (elttype, pfir, pnodeqty_tot, psec, pnodeqty_o1);
  else if (dim == 3)
    neut_elt_orderarrays_3d (elttype, pfir, pnodeqty_tot, psec, pnodeqty_o1);
  else
    abort ();

  return;
}
