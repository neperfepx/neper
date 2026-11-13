/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_tess_.h"

/* Recording of TFNb.  The number of faces is returned. */
int
net_tess3d_tess_txnb_tfnb (struct POLY *Poly, int PolyQty, int **TFNb)
{
  int i, j, PrevF;
  int nb = 0;                   /* tmp variable containing the face nb */
  int PrevP;                    /* number of the previous polyhedron in which
                                 * the face has already been recorded */

  for (i = 1; i <= PolyQty; i++)
  {
    TFNb[i] = ut_alloc_1d_int (Poly[i].FaceQty + 1);
    for (j = 1; j <= Poly[i].FaceQty; j++)
      /* If the face has not been created yet, we do it. */
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
        TFNb[i][j] = ++nb;
      /* Else, we search the number of the face. */
      else if (Poly[i].FacePoly[j] > 0)
      {
        PrevP = Poly[i].FacePoly[j];
        PrevF = 1 + ut_array_1d_int_eltpos (Poly[PrevP].FacePoly + 1, Poly[PrevP].FaceQty, i);
        TFNb[i][j] = TFNb[PrevP][PrevF];
      }
  }

  return nb;
}

/* Recording of TVNb.  The number of vertices is returned. */
int
net_tess3d_tess_txnb_tvnb (struct POLY *Poly, int PolyQty, int **TVNb)
{
  int PNb, VNb;                 /* polyhedron and polyhedron vertex */
  int nb = 0;                   /* number of vertices */

  /* All vertices of all polyhedra are studied successively. */
  for (PNb = 1; PNb <= PolyQty; PNb++)
  {
    TVNb[PNb] = ut_alloc_1d_int (Poly[PNb].VerQty + 1);
    for (VNb = 1; VNb <= Poly[PNb].VerQty; VNb++)
      /* net_tess3d_tess_txnb_tvnb_treat returns 1 if the ver has not
       * been recorded previously, 0 else. If 1, the vertex number has to
       * be created, else it has to be searched.
       */
      if (net_tess3d_tess_txnb_tvnb_treat (Poly, PNb, VNb) == 1)
        TVNb[PNb][VNb] = ++nb;
      else
        net_tess3d_tess_txnb_tvnb_search (Poly, PNb, VNb, TVNb);
  }

  return nb;
}

/* Recording of the equation of face j of polyhedron i, so that the face
 * vertices are ordered counter-clockwise about the face normal.  For an
 * internal face, the equation is reversed if needed (eq <-- -eq).  A domain
 * face keeps its equation, so that all the faces of a domain face have the
 * same equation, and 1 is returned if the order of the vertices must be
 * reversed instead; 0 is returned otherwise.
 */
int
net_tess3d_tess_face_eq (struct POLY *Poly, int i, int j, double *eq)
{
  int k, first, second, third;
  double a[3], b[3], c[3];

  ut_array_1d_memcpy (Poly[i].FaceEq[j], 4, eq);

  first = Poly[i].FaceVerNb[j][1];
  second = Poly[i].FaceVerNb[j][2];
  third = Poly[i].FaceVerNb[j][3];

  for (k = 0; k < 3; k++)
  {
    a[k] = Poly[i].VerCoo[second][k] - Poly[i].VerCoo[first][k];
    b[k] = Poly[i].VerCoo[third][k] - Poly[i].VerCoo[second][k];
  }
  ut_vector_vectprod (a, b, c);

  if (ut_vector_scalprod (eq + 1, c) >= 0)
    return 0;

  if (Poly[i].FacePoly[j] < 0)
    return 1;

  ut_array_1d_scale (eq, 4, -1);

  return 0;
}

/* net_tess3d_tess_poly_faceori returns 1 if face i of polyhedron poly, of
 * centre centre, has an outgoing normal, and -1 else. */
int
net_tess3d_tess_poly_faceori (struct TESS *pTess, int poly, int i, double *centre)
{
  int face = (*pTess).PolyFaceNb[poly][i];

  // skip non-planar faces, for which FaceEq = -1
  if (ut_vector_norm ((*pTess).FaceEq[face] + 1) - 1 > 1e-6)
    return 1;

  return -ut_space_point_plane_side (centre - 1, (*pTess).FaceEq[face]);
}

/* net_tess3d_tess_edge_commonseeds searchs the common parent seeds of 2
 * vertices.  It returns their numbers (nb) and their quantity (qty).
 */
int
net_tess3d_tess_edge_commonseeds (int **VerSeed, int first, int second, int *nb)
{
  int i, j;
  int qty = 0;

  for (i = 0; i <= 3; i++)
    for (j = 0; j <= 3; j++)
      if (VerSeed[first][i] == VerSeed[second][j])
        nb[qty++] = VerSeed[first][i];

  return qty;
}

/* Recording of FaceEdgeOri */
void
net_tess3d_tess_edge_faceedgeori (struct TESS *pTess)
{
  int i, j, ver, edge;

  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTess).FaceEdgeQty[i] + 1);

    for (j = 1; j <= (*pTess).FaceEdgeQty[i]; j++)
    {
      ver = (*pTess).FaceVerNb[i][j];
      edge = (*pTess).FaceEdgeNb[i][j];
      if ((*pTess).EdgeVerNb[edge][0] == ver)
        (*pTess).FaceEdgeOri[i][j] = 1;
      else if ((*pTess).EdgeVerNb[edge][1] == ver)
        (*pTess).FaceEdgeOri[i][j] = -1;
      else
        ut_print_neperbug ();
    }
  }

  return;
}

/* Recording of EdgeFaceQty & EdgeFaceNb */
void
net_tess3d_tess_edge_edgeface (struct TESS *pTess)
{
  int i, j, edge;

  (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 1; j <= (*pTess).FaceEdgeQty[i]; j++)
    {
      edge = (*pTess).FaceEdgeNb[i][j];
      ut_array_1d_int_list_addval_nocheck ((*pTess).EdgeFaceNb + edge,
                                           (*pTess).EdgeFaceQty + edge, i);
    }

  return;
}
