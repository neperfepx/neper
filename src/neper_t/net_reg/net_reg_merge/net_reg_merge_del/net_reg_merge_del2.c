/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

// Buffering the entities modified by the deletion of edges (the copies
// of an edge in a periodic tessellation): the edges, their vertices and
// the entities around, and the entities around vers (the copies of the
// vertices of the edges, in a periodic tessellation).
void
net_reg_merge_del_buffer (struct TESS Tess, int *edges, int edgeqty,
                          int *vers, int verqty, int **buf,
                          struct TESS *pTessBuf)
{
  int i, j, k, id, edge, face, poly;
  int qty, *tmp = NULL, tetqty = 0, *tets = NULL;

  // tetrahedral polys of the edges, which can be deleted (see
  // net_reg_merge_del_delpoly)
  if (Tess.Dim == 3)
    for (j = 0; j < edgeqty; j++)
    {
      neut_tess_edge_polys (Tess, edges[j], &tmp, &qty);
      for (i = 0; i < qty; i++)
        if (tmp[i] > 0 && tmp[i] <= Tess.PolyQty && Tess.PolyFaceQty[tmp[i]] == 4)
          ut_array_1d_int_list_addval (&tets, &tetqty, tmp[i]);
      ut_free_1d_int (&tmp);
    }

  // buf edges: del edges + neighboring edges (+ edges of vers) (+ edges
  // of tets)
  buf[1][0] = 0;
  for (j = 0; j < edgeqty; j++)
  {
    edge = edges[j];
    net_reg_merge_del_buffer_add (buf + 1, &edge, 1);
    for (i = 0; i < 2; i++)
    {
      id = Tess.EdgeVerNb[edge][i];
      net_reg_merge_del_buffer_add (buf + 1, Tess.VerEdgeNb[id],
                                    Tess.VerEdgeQty[id]);
    }
  }
  for (j = 0; j < verqty; j++)
    net_reg_merge_del_buffer_add (buf + 1, Tess.VerEdgeNb[vers[j]],
                                  Tess.VerEdgeQty[vers[j]]);
  for (j = 0; j < tetqty; j++)
    for (i = 1; i <= Tess.PolyFaceQty[tets[j]]; i++)
    {
      face = Tess.PolyFaceNb[tets[j]][i];
      net_reg_merge_del_buffer_add (buf + 1, Tess.FaceEdgeNb[face] + 1,
                                    Tess.FaceEdgeQty[face]);
    }
  ut_array_1d_int_sort_uniq (buf[1] + 1, buf[1][0], &(buf[1][0]));

  // buf vertices: vertices of buf edges (+ vers)
  neut_tess_edges_vers (Tess, buf[1] + 1, buf[1][0], &tmp, &qty);
  buf[0][0] = 0;
  net_reg_merge_del_buffer_add (buf, tmp, qty);
  net_reg_merge_del_buffer_add (buf, vers, verqty);
  ut_array_1d_int_sort_uniq (buf[0] + 1, buf[0][0], &(buf[0][0]));
  ut_free_1d_int (&tmp);

  // buf faces: edge faces + neighboring faces (+ faces of vers)
  buf[2][0] = 0;
  for (j = 0; j < edgeqty; j++)
  {
    edge = edges[j];
    // (not the periodic seeds of a 2D periodic tessellation)
    for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
      if (Tess.EdgeFaceNb[edge][i] >= 1
          && Tess.EdgeFaceNb[edge][i] <= Tess.FaceQty)
        net_reg_merge_del_buffer_add (buf + 2, Tess.EdgeFaceNb[edge] + i, 1);
    for (i = 0; i < 2; i++)
    {
      id = Tess.EdgeVerNb[edge][i];
      neut_tess_ver_faces (Tess, id, &tmp, &qty);
      net_reg_merge_del_buffer_add (buf + 2, tmp, qty);
      ut_free_1d_int (&tmp);
    }
  }
  for (j = 0; j < verqty; j++)
  {
    neut_tess_ver_faces (Tess, vers[j], &tmp, &qty);
    net_reg_merge_del_buffer_add (buf + 2, tmp, qty);
    ut_free_1d_int (&tmp);
  }
  ut_array_1d_int_sort_uniq (buf[2] + 1, buf[2][0], &(buf[2][0]));

  // buf polys: edge polys (+ polys of the faces of tets)
  buf[3][0] = 0;
  if (Tess.Dim == 3)
  {
    for (j = 0; j < edgeqty; j++)
    {
      neut_tess_edge_polys (Tess, edges[j], &tmp, &qty);
      net_reg_merge_del_buffer_add (buf + 3, tmp, qty);
      ut_free_1d_int (&tmp);
    }
    for (j = 0; j < tetqty; j++)
      for (i = 1; i <= Tess.PolyFaceQty[tets[j]]; i++)
      {
        face = Tess.PolyFaceNb[tets[j]][i];
        for (k = 0; k < 2; k++)
        {
          poly = Tess.FacePoly[face][k];
          if (poly > 0 && poly <= Tess.PolyQty)
            net_reg_merge_del_buffer_add (buf + 3, &poly, 1);
        }
      }
    ut_array_1d_int_sort_uniq (buf[3] + 1, buf[3][0], &(buf[3][0]));
  }

  ut_free_1d_int (&tets);

  if (pTessBuf)
  {
    // Copying modified entities in buffer tess

    for (i = 1; i <= buf[0][0]; i++)
    {
      id = buf[0][i];

      (*pTessBuf).VerCoo[id] = ut_alloc_1d (3);
      ut_array_1d_memcpy (Tess.VerCoo[id], 3, (*pTessBuf).VerCoo[id]);

      (*pTessBuf).VerEdgeQty[id] = Tess.VerEdgeQty[id];

      (*pTessBuf).VerEdgeNb[id] = ut_alloc_1d_int (Tess.VerEdgeQty[id]);
      ut_array_1d_int_memcpy (Tess.VerEdgeNb[id], Tess.VerEdgeQty[id],
                              (*pTessBuf).VerEdgeNb[id]);

      (*pTessBuf).VerDom[id] = ut_alloc_1d_int (2);
      ut_array_1d_int_memcpy (Tess.VerDom[id], 2, (*pTessBuf).VerDom[id]);

      (*pTessBuf).VerState[id] = Tess.VerState[id];

      if (Tess.PerVerMaster)
      {
        (*pTessBuf).PerVerMaster[id] = Tess.PerVerMaster[id];
        ut_array_1d_int_memcpy (Tess.PerVerShift[id], 3,
                                (*pTessBuf).PerVerShift[id]);
        (*pTessBuf).PerVerSlaveQty[id] = Tess.PerVerSlaveQty[id];
        (*pTessBuf).PerVerSlaveNb[id] =
          ut_alloc_1d_int (Tess.PerVerSlaveQty[id] + 1);
        ut_array_1d_int_memcpy (Tess.PerVerSlaveNb[id] + 1,
                                Tess.PerVerSlaveQty[id],
                                (*pTessBuf).PerVerSlaveNb[id] + 1);
      }
    }

    for (i = 1; i <= buf[1][0]; i++)
    {
      id = buf[1][i];

      (*pTessBuf).EdgeVerNb[id] = ut_alloc_1d_int (2);
      ut_array_1d_int_memcpy (Tess.EdgeVerNb[id], 2,
                              (*pTessBuf).EdgeVerNb[id]);

      (*pTessBuf).EdgeFaceQty[id] = Tess.EdgeFaceQty[id];

      (*pTessBuf).EdgeFaceNb[id] =
        ut_alloc_1d_int ((*pTessBuf).EdgeFaceQty[id]);
      ut_array_1d_int_memcpy (Tess.EdgeFaceNb[id], Tess.EdgeFaceQty[id],
                              (*pTessBuf).EdgeFaceNb[id]);

      (*pTessBuf).EdgeLength[id] = Tess.EdgeLength[id];

      (*pTessBuf).EdgeDom[id] = ut_alloc_1d_int (2);
      ut_array_1d_int_memcpy (Tess.EdgeDom[id], 2, (*pTessBuf).EdgeDom[id]);

      (*pTessBuf).EdgeState[id] = Tess.EdgeState[id];

      (*pTessBuf).EdgeDel[id] = Tess.EdgeDel[id];
    }

    for (i = 1; i <= buf[2][0]; i++)
    {
      id = buf[2][i];

      if ((*pTessBuf).Dim == 3)
      {
        (*pTessBuf).FacePoly[id] = ut_alloc_1d_int (2);
        ut_array_1d_int_memcpy (Tess.FacePoly[id], 2,
                                (*pTessBuf).FacePoly[id]);
      }

      (*pTessBuf).FaceEq[id] = ut_alloc_1d (4);
      ut_array_1d_memcpy (Tess.FaceEq[id], 4, (*pTessBuf).FaceEq[id]);

      (*pTessBuf).FaceVerQty[id] = Tess.FaceVerQty[id];

      (*pTessBuf).FaceVerNb[id] =
        ut_alloc_1d_int ((*pTessBuf).FaceVerQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.FaceVerNb[id] + 1, Tess.FaceVerQty[id],
                              (*pTessBuf).FaceVerNb[id] + 1);

      (*pTessBuf).FaceEdgeQty[id] = Tess.FaceEdgeQty[id];

      (*pTessBuf).FaceEdgeNb[id] =
        ut_alloc_1d_int ((*pTessBuf).FaceEdgeQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.FaceEdgeNb[id] + 1, Tess.FaceEdgeQty[id],
                              (*pTessBuf).FaceEdgeNb[id] + 1);

      (*pTessBuf).FaceEdgeOri[id] =
        ut_alloc_1d_int ((*pTessBuf).FaceEdgeQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.FaceEdgeOri[id] + 1, Tess.FaceEdgeQty[id],
                              (*pTessBuf).FaceEdgeOri[id] + 1);


      (*pTessBuf).FacePt[id] = Tess.FacePt[id];
      (*pTessBuf).FacePtCoo[id] = ut_alloc_1d (3);

      ut_array_1d_memcpy (Tess.FacePtCoo[id], 3, (*pTessBuf).FacePtCoo[id]);

      if ((*pTessBuf).Dim == 3)
      {
        (*pTessBuf).FaceDom[id] = ut_alloc_1d_int (2);
        ut_array_1d_int_memcpy (Tess.FaceDom[id], 2, (*pTessBuf).FaceDom[id]);
      }

      (*pTessBuf).FaceState[id] = Tess.FaceState[id];
    }

    for (i = 1; i <= buf[3][0]; i++)
    {
      id = buf[3][i];

      (*pTessBuf).PolyFaceQty[id] = Tess.PolyFaceQty[id];
      (*pTessBuf).PolyState[id] = Tess.PolyState[id];

      (*pTessBuf).PolyFaceNb[id] =
        ut_alloc_1d_int ((*pTessBuf).PolyFaceQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.PolyFaceNb[id] + 1, Tess.PolyFaceQty[id],
                              (*pTessBuf).PolyFaceNb[id] + 1);

      (*pTessBuf).PolyFaceOri[id] =
        ut_alloc_1d_int ((*pTessBuf).PolyFaceQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.PolyFaceOri[id] + 1, Tess.PolyFaceQty[id],
                              (*pTessBuf).PolyFaceOri[id] + 1);
    }
  }

  return;
}

/* Updating of EdgeState */
void
UpdateEdgeState (struct TESS *pTess, int edge)
{
  /* edge is set as useless and deleted. */
  (*pTess).EdgeState[edge] = -1;

  return;
}

/* Searching the vertex to delete and the new one (its coo will change) */
void
SearchDelNNewVer (struct TESS *pTess, int edge, int *pdelver, int *pnewver)
{
  (*pdelver) = ut_array_1d_int_max ((*pTess).EdgeVerNb[edge], 2);
  (*pnewver) = ut_array_1d_int_min ((*pTess).EdgeVerNb[edge], 2);

  return;
}

// Updating the coordinates of the new vertex.  In 3D, faces (of
// translations facetrans) are the faces whose flatness is optimized
// (NULL for the faces of newver).
int
UpdateVerCoo (struct TESS *pTess, int delver, int newver, int *faces,
              double **facetrans, int faceqty, int verbosity)
{
  if ((*pTess).Dim == 2)
    return UpdateVerCooBary (pTess, delver, newver, verbosity);
  else if ((*pTess).Dim == 3)
    return UpdateVerCooMiniFF (pTess, newver, faces, facetrans, faceqty,
                               verbosity);
  else
    abort ();
}

// Adding values to a buffer list (buffer[0][0] values, from
// buffer[0][1]), reallocating it as needed
void
net_reg_merge_del_buffer_add (int **buffer, int *vals, int valqty)
{
  if (valqty <= 0)
    return;

  (*buffer) = ut_realloc_1d_int (*buffer, (*buffer)[0] + valqty + 1);
  ut_array_1d_int_memcpy (vals, valqty, (*buffer) + (*buffer)[0] + 1);
  (*buffer)[0] += valqty;

  return;
}

/* UpdateEdgeVerNb updates the edge ver nb
 * the delver vertex is replaced by the new one
 */
void
UpdateEdgeVerNb (struct TESS *pTess, int delver, int newver, int verbosity)
{
  int i;
  int tmpedge;
  int pos;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Changing ver %d to ver %d in edges of ver %d\n", newver,
            delver, delver);
  }

  for (i = 0; i <= (*pTess).VerEdgeQty[delver] - 1; i++)
  {
    tmpedge = (*pTess).VerEdgeNb[delver][i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("      ver of edge %d: ", tmpedge);
    }

    pos = oneDIntEltPos ((*pTess).EdgeVerNb[tmpedge], 0, 1, delver);

    if (verbosity >= 3)
      printf ("%d %d become ", (*pTess).EdgeVerNb[tmpedge][0],
              (*pTess).EdgeVerNb[tmpedge][1]);

    (*pTess).EdgeVerNb[tmpedge][pos] = newver;

    if (verbosity >= 3)
      printf ("%d %d\n", (*pTess).EdgeVerNb[tmpedge][0],
              (*pTess).EdgeVerNb[tmpedge][1]);
  }

  return;
}

/* UpdateFace updates the properties of the parent faces of the edge */
int
UpdateFaceVerNEdge (struct TESS *pTess, int edge, int delver, int newver,
                    int verbosity)
{
  int i, err = 0;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("  %d parent faces: ", (*pTess).EdgeFaceQty[edge]);
    for (i = 0; i <= (*pTess).EdgeFaceQty[edge] - 1; i++)
      printf (" %d", (*pTess).EdgeFaceNb[edge][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf ("Updating face vertices\n");
  }

  UpdateFaceState (pTess, delver, newver);

  if (UpdateFaceVer (pTess, delver, newver, verbosity) != 0)
    return -1;

  if (verbosity >= 3)
  {
    printf ("\n");
    ut_print_lineheader (-1);
    printf ("Updating face edges\n");
  }

  err = UpdateFaceEdge (pTess, edge, verbosity);

  return err;
}

int
UpdateVer (struct TESS *pTess, int edge, int delver, int newver,
           int verbosity)
{
  int err = 0;

  /* Updating of VerEdgeQty & VerEdgeNb */
  UpdateVerEdge (pTess, edge, delver, newver, verbosity);

  /* Updating of VerDom */
  err = UpdateVerBound (pTess, delver, newver);

  /* Updating of VerState */
  UpdateVerState (pTess, delver, newver, verbosity);

  return err;
}

// Domain edges and domain faces a vertex belongs to (from its VerDom)
static void
net_edgedel_ver_domedgesfaces (struct TESS Tess, int ver, int **pdedges,
                               int *pdedgeqty, int **pdfaces, int *pdfaceqty)
{
  int i, j, dedge;

  (*pdedgeqty) = 0;
  (*pdfaceqty) = 0;

  if (Tess.VerDom[ver][0] == 0)
    for (i = 0; i < Tess.DomVerEdgeQty[Tess.VerDom[ver][1]]; i++)
      ut_array_1d_int_list_addval (pdedges, pdedgeqty,
                                   Tess.DomVerEdgeNb[Tess.VerDom[ver][1]][i]);
  else if (Tess.VerDom[ver][0] == 1)
    ut_array_1d_int_list_addval (pdedges, pdedgeqty, Tess.VerDom[ver][1]);
  else if (Tess.VerDom[ver][0] == 2)
    ut_array_1d_int_list_addval (pdfaces, pdfaceqty, Tess.VerDom[ver][1]);

  // (in 2D, the domain edges have no domain faces)
  if (Tess.Dim == 3)
    for (i = 0; i < *pdedgeqty; i++)
    {
      dedge = (*pdedges)[i];
      for (j = 0; j < 2; j++)
        ut_array_1d_int_list_addval (pdfaces, pdfaceqty,
                                     Tess.DomEdgeFaceNb[dedge][j]);
    }

  return;
}

// Updating the domain of the edges of a vertex, from the domain of their
// vertices: an edge is on a domain edge or a domain face if both its
// vertices are (a domain vertex being on its domain edges and domain
// faces, and a vertex on a domain edge being on its domain faces), and
// is not on the domain otherwise.
void
net_edgedel_edgedom_fromverdom (struct TESS *pTess, int ver)
{
  int i, k, edge, ver2, qty, *tmp = NULL;
  int *dedges[2] = { NULL, NULL }, dedgeqty[2];
  int *dfaces[2] = { NULL, NULL }, dfaceqty[2];

  for (i = 0; i < (*pTess).VerEdgeQty[ver]; i++)
  {
    edge = (*pTess).VerEdgeNb[ver][i];
    ver2 = neut_tess_edge_otherver (*pTess, edge, ver);

    (*pTess).EdgeDom[edge][0] = -1;
    (*pTess).EdgeDom[edge][1] = 0;

    if ((*pTess).VerDom[ver][0] == -1 || (*pTess).VerDom[ver2][0] == -1)
      continue;

    for (k = 0; k < 2; k++)
      net_edgedel_ver_domedgesfaces (*pTess, k == 0 ? ver : ver2, dedges + k,
                                     dedgeqty + k, dfaces + k, dfaceqty + k);

    tmp = ut_realloc_1d_int (tmp, dedgeqty[0] + dfaceqty[0] + 1);

    ut_array_1d_int_inter (dedges[0], dedgeqty[0], dedges[1], dedgeqty[1],
                           tmp, &qty);
    if (qty > 0)
    {
      (*pTess).EdgeDom[edge][0] = 1;
      (*pTess).EdgeDom[edge][1] = tmp[0];
    }
    else
    {
      ut_array_1d_int_inter (dfaces[0], dfaceqty[0], dfaces[1], dfaceqty[1],
                             tmp, &qty);
      if (qty > 0)
      {
        (*pTess).EdgeDom[edge][0] = 2;
        (*pTess).EdgeDom[edge][1] = tmp[0];
      }
    }

    for (k = 0; k < 2; k++)
    {
      ut_free_1d_int (dedges + k);
      ut_free_1d_int (dfaces + k);
    }
  }

  ut_free_1d_int (&tmp);

  return;
}

void
net_tess_reg_ver_facedom (struct TESS *pTess, int ver)
{
  int i;
  int faceqty;
  int *face = NULL;

  neut_tess_ver_faces (*pTess, ver, &face, &faceqty);

  for (i = 0; i < faceqty; i++)
    neut_tess_init_facedom_face_v (pTess, face[i]);

  neut_tess_init_domtessface (pTess);

  ut_free_1d_int (&face);

  return;
}

// Testing whether a tetrahedral poly can be deleted by deleting one of its
// edges: its 2 faces that do not contain the edge, which become the same
// face, must not be both on the domain or have the same other poly.
// Returns 0 if it can, -1 otherwise.
int
net_reg_merge_del_test_tet (struct TESS Tess, int poly, int edge)
{
  int i, face, qty = 0, nbs[2];

  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    if (ut_array_1d_int_eltpos (Tess.EdgeFaceNb[edge], Tess.EdgeFaceQty[edge],
                                face) != -1)
      continue;

    if (qty == 2)
      return -1;

    nbs[qty++] =
      Tess.FacePoly[face][0] == poly ? Tess.FacePoly[face][1] : Tess.FacePoly[face][0];
  }

  if (qty != 2 || nbs[0] == nbs[1] || (nbs[0] < 0 && nbs[1] < 0))
    return -1;

  return 0;
}

// Deleting a poly reduced to 2 faces (of same vertices and edges), which
// results from the deletion of an edge of a tetrahedral poly: the faces
// are merged into one, which takes the other polys of the 2 faces.
// Returns 0 on success and -1 if the poly is not as expected
// (inconsistent tessellation).
int
net_reg_merge_del_delpoly (struct TESS *pTess, int poly, int verbosity)
{
  int i, pos, face1, face2, edge, poly1, poly2, ori;

  if ((*pTess).PolyFaceQty[poly] != 2)
    return -1;

  face1 = ut_num_min ((*pTess).PolyFaceNb[poly][1], (*pTess).PolyFaceNb[poly][2]);
  face2 = ut_num_max ((*pTess).PolyFaceNb[poly][1], (*pTess).PolyFaceNb[poly][2]);

  if ((*pTess).FaceVerQty[face1] != 3 || (*pTess).FaceVerQty[face2] != 3
      || (*pTess).FaceEdgeQty[face1] != 3 || (*pTess).FaceEdgeQty[face2] != 3)
    return -1;

  // the faces have the same vertices, in the same or the opposite order
  pos = ut_array_1d_int_eltpos ((*pTess).FaceVerNb[face2] + 1, 3,
                                (*pTess).FaceVerNb[face1][1]);
  if (pos == -1)
    return -1;
  if ((*pTess).FaceVerNb[face2][1 + (pos + 1) % 3] == (*pTess).FaceVerNb[face1][2]
      && (*pTess).FaceVerNb[face2][1 + (pos + 2) % 3] == (*pTess).FaceVerNb[face1][3])
    ori = 1;
  else if ((*pTess).FaceVerNb[face2][1 + (pos + 2) % 3] == (*pTess).FaceVerNb[face1][2]
           && (*pTess).FaceVerNb[face2][1 + (pos + 1) % 3] == (*pTess).FaceVerNb[face1][3])
    ori = -1;
  else
    return -1;

  for (i = 1; i <= 3; i++)
    if (ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[face1] + 1, 3,
                                (*pTess).FaceEdgeNb[face2][i]) == -1)
      return -1;

  if (verbosity >= 2)
    printf ("deleting poly %d (merging faces %d and %d)\n", poly, face1, face2);

  poly1 = (*pTess).FacePoly[face1][0] == poly ? (*pTess).FacePoly[face1][1]
                                               : (*pTess).FacePoly[face1][0];
  poly2 = (*pTess).FacePoly[face2][0] == poly ? (*pTess).FacePoly[face2][1]
                                               : (*pTess).FacePoly[face2][0];

  if (poly1 == poly2 || (poly1 < 0 && poly2 < 0))
    return -1;

  // face1 takes the place of face2 in poly2
  if (poly2 > 0)
  {
    pos = ut_array_1d_int_eltpos ((*pTess).PolyFaceNb[poly2] + 1,
                                  (*pTess).PolyFaceQty[poly2], face2);
    if (pos == -1)
      return -1;
    (*pTess).PolyFaceNb[poly2][pos + 1] = face1;
    (*pTess).PolyFaceOri[poly2][pos + 1] *= ori;
  }

  // face1 takes the domain of face2 (if on the domain)
  else
  {
    ut_array_1d_int_memcpy ((*pTess).FaceDom[face2], 2, (*pTess).FaceDom[face1]);
    ut_array_1d_memcpy ((*pTess).FaceEq[face2], 4, (*pTess).FaceEq[face1]);
  }

  // (the domain is the second poly)
  (*pTess).FacePoly[face1][0] = poly1 > 0 ? poly1 : poly2;
  (*pTess).FacePoly[face1][1] = poly1 > 0 ? poly2 : poly1;
  (*pTess).FaceState[face1] = 1;

  for (i = 1; i <= 3; i++)
  {
    edge = (*pTess).FaceEdgeNb[face2][i];
    (*pTess).EdgeFaceQty[edge] -=
      ut_array_1d_int_rmelt ((*pTess).EdgeFaceNb[edge], (*pTess).EdgeFaceQty[edge],
                             face2, 1);
  }

  (*pTess).FaceState[face2] = -1;
  (*pTess).PolyFaceQty[poly] = 0;
  (*pTess).PolyState[poly] = -1;

  return 0;
}

// Testing whether the deletion of an edge, in 2D, leads to a valid
// tessellation (link condition): the vertices linked to both vertices of
// the edge must be the third vertices of its triangular cells (which are
// deleted).  Returns 0 if it does, -1 otherwise.
int
net_reg_merge_del_test_link (struct TESS Tess, int edge)
{
  int i, j, k, ver, face, status = 0, qty[2], *nbs[2] = { NULL, NULL };

  for (i = 0; i < 2; i++)
  {
    ver = Tess.EdgeVerNb[edge][i];
    qty[i] = 0;
    for (j = 0; j < Tess.VerEdgeQty[ver]; j++)
      ut_array_1d_int_list_addval (nbs + i, qty + i,
                                   neut_tess_edge_otherver (Tess, Tess.VerEdgeNb[ver][j], ver));
  }

  for (i = 0; i < qty[0] && !status; i++)
  {
    ver = nbs[0][i];
    if (ut_array_1d_int_eltpos (nbs[1], qty[1], ver) == -1)
      continue;

    status = -1;
    for (j = 0; j < Tess.EdgeFaceQty[edge] && status; j++)
    {
      face = Tess.EdgeFaceNb[edge][j];
      if (face >= 1 && face <= Tess.FaceQty && Tess.FaceVerQty[face] == 3)
        for (k = 1; k <= 3; k++)
          if (Tess.FaceVerNb[face][k] == ver)
            status = 0;
    }
  }

  ut_free_1d_int (nbs);
  ut_free_1d_int (nbs + 1);

  return status;
}

// Testing whether the polys of a vertex are folded, that is, whether two
// faces of a poly sharing an edge form a reflex angle and have outward
// normals at an angle higher than maxangle (in degrees).  Returns 0 if
// they are not, -1 otherwise.
int
net_reg_merge_del_testfold (struct TESS Tess, int ver, double maxangle)
{
  int i, j, k, l, m, poly, face1, face2, edge, ver2, qty, status = 0;
  int polyqty, *polys = NULL;
  double s, *n1 = ut_alloc_1d (3), *n2 = ut_alloc_1d (3), *v = ut_alloc_1d (3);
  double mincos = cos (maxangle * M_PI / 180);

  neut_tess_ver_polys (Tess, ver, &polys, &polyqty);

  for (i = 0; i < polyqty && !status; i++)
  {
    poly = polys[i];
    if (poly < 1 || poly > Tess.PolyQty || Tess.PolyState[poly] == -1)
      continue;

    for (j = 1; j <= Tess.PolyFaceQty[poly] && !status; j++)
    {
      face1 = Tess.PolyFaceNb[poly][j];
      neut_tess_face_normal_fromver (Tess, face1, n1);
      ut_array_1d_scale (n1, 3, Tess.PolyFaceOri[poly][j]);

      for (k = 1; k <= Tess.FaceEdgeQty[face1] && !status; k++)
      {
        edge = Tess.FaceEdgeNb[face1][k];

        for (l = j + 1; l <= Tess.PolyFaceQty[poly] && !status; l++)
        {
          face2 = Tess.PolyFaceNb[poly][l];
          if (ut_array_1d_int_eltpos (Tess.FaceEdgeNb[face2] + 1,
                                      Tess.FaceEdgeQty[face2], edge) == -1)
            continue;

          neut_tess_face_normal_fromver (Tess, face2, n2);
          ut_array_1d_scale (n2, 3, Tess.PolyFaceOri[poly][l]);

          if (ut_array_1d_scalprod (n1, n2, 3) > mincos)
            continue;

          // reflex angle: the other vertices of face2 are above face1
          s = 0;
          qty = 0;
          for (m = 1; m <= Tess.FaceVerQty[face2]; m++)
          {
            ver2 = Tess.FaceVerNb[face2][m];
            if (ver2 == Tess.EdgeVerNb[edge][0] || ver2 == Tess.EdgeVerNb[edge][1])
              continue;
            ut_array_1d_sub (Tess.VerCoo[Tess.EdgeVerNb[edge][0]], Tess.VerCoo[ver2], 3, v);
            s += ut_array_1d_scalprod (n1, v, 3);
            qty++;
          }

          if (qty > 0 && s > 0)
            status = -1;
        }
      }
    }
  }

  ut_free_1d_int (&polys);
  ut_free_1d (&n1);
  ut_free_1d (&n2);
  ut_free_1d (&v);

  return status;
}

// Numbers of (non-deleted) faces of the domain faces, (*pqty)[1...DomFaceQty]
void
net_reg_merge_del_domfacefaceqty (struct TESS Tess, int **pqty)
{
  int i;

  (*pqty) = ut_realloc_1d_int (*pqty, Tess.DomFaceQty + 1);
  ut_array_1d_int_zero (*pqty, Tess.DomFaceQty + 1);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceState[i] != -1 && Tess.FaceDom[i][0] == 2
        && Tess.FaceDom[i][1] >= 1 && Tess.FaceDom[i][1] <= Tess.DomFaceQty)
      (*pqty)[Tess.FaceDom[i][1]]++;

  return;
}
