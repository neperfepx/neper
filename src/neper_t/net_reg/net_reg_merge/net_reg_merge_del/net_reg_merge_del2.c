/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

void
net_reg_merge_del_buffer (struct TESS Tess, int edge, int **buf,
                          struct TESS *pTessBuf)
{
  int i, id;
  int qty, *tmp = NULL;

  // buf edges: del edge + neighboring edges
  buf[1][0] = 0;
  buf[1][++buf[1][0]] = edge;
  for (i = 0; i < 2; i++)
  {
    id = Tess.EdgeVerNb[edge][i];
    ut_array_1d_int_memcpy (Tess.VerEdgeNb[id], Tess.VerEdgeQty[id],
                            buf[1] + buf[1][0] + 1);
    buf[1][0] += Tess.VerEdgeQty[id];
  }
  ut_array_1d_int_sort_uniq (buf[1] + 1, buf[1][0], &(buf[1][0]));

  // buf vertices: vertices of buf edges
  neut_tess_edges_vers (Tess, buf[1] + 1, buf[1][0], &tmp, &qty);
  buf[0][0] = qty;
  ut_array_1d_int_memcpy (tmp, buf[0][0], buf[0] + 1);
  ut_free_1d_int (&tmp);

  // buf faces: edge faces + neighboring faces
  buf[2][0] = Tess.EdgeFaceQty[edge];
  ut_array_1d_int_memcpy (Tess.EdgeFaceNb[edge], buf[2][0], buf[2] + 1);
  for (i = 0; i < 2; i++)
  {
    id = Tess.EdgeVerNb[edge][i];
    neut_tess_ver_faces (Tess, id, &tmp, &qty);
    ut_array_1d_int_memcpy (tmp, qty, buf[2] + buf[2][0] + 1);
    buf[2][0] += qty;
    ut_free_1d_int (&tmp);
  }
  ut_array_1d_int_sort_uniq (buf[2] + 1, buf[2][0], &(buf[2][0]));
  /*
     neut_tess_edges_faces (Tess, buf[1] + 1, buf[1][0], &tmp, &qty);
     buf[2][0] = qty;
     ut_array_1d_int_memcpy ( tmp, buf[2][0],buf[2] + 1);
   */

  // buf polys: edge polys
  if (Tess.Dim == 3)
  {
    neut_tess_edge_polys (Tess, edge, &tmp, &qty);
    buf[3][0] = qty;
    ut_array_1d_int_memcpy (tmp, buf[3][0], buf[3] + 1);
    ut_free_1d_int (&tmp);
  }
  else
    buf[3][0] = 0;

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

      (*pTessBuf).FaceEdgeNb[id] =
        ut_alloc_1d_int ((*pTessBuf).FaceVerQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.FaceEdgeNb[id] + 1, Tess.FaceVerQty[id],
                              (*pTessBuf).FaceEdgeNb[id] + 1);

      (*pTessBuf).FaceEdgeOri[id] =
        ut_alloc_1d_int ((*pTessBuf).FaceVerQty[id] + 1);
      ut_array_1d_int_memcpy (Tess.FaceEdgeOri[id] + 1, Tess.FaceVerQty[id],
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

int
UpdateVerCoo (struct TESS *pTess, int delver, int newver, int verbosity)
{
  if ((*pTess).Dim == 2)
    return UpdateVerCooBary (pTess, delver, newver);
  else if ((*pTess).Dim == 3)
    return UpdateVerCooMiniFF (pTess, newver, verbosity);
  else
    abort ();
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

  UpdateFaceVer (pTess, delver, newver, verbosity);

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

/* UpdateEdgeBound updates the bound of the edges of ver */
void
net_edgedel_edgedom_fromverdom (struct TESS *pTess, int ver)
{
  int i, id, ver2, edge, domedge, domface;
  int *vers = ut_alloc_1d_int (2);

  // Scanning all edges of the new ver
  for (i = 0; i < (*pTess).VerEdgeQty[i]; i++)
  {
    edge = (*pTess).VerEdgeNb[ver][i];
    ver2 =
      ((*pTess).EdgeVerNb[edge][0] !=
       ver) ? (*pTess).EdgeVerNb[edge][0] : (*pTess).EdgeVerNb[edge][1];

    // at least one ver is not on the domain: edge not on the domain
    if ((*pTess).VerDom[ver][0] == -1 || (*pTess).VerDom[ver2][0] == -1)
      (*pTess).EdgeDom[edge][0] = -1;

    // both vers are boundary vers
    else if ((*pTess).VerDom[ver][0] == 0 && (*pTess).VerDom[ver2][0] == 0)
    {
      // test: vers are on the same domain ver: this should not happen
      if ((*pTess).VerDom[ver][1] == (*pTess).VerDom[ver2][1])
        ut_print_neperbug ();

      vers[0] = (*pTess).VerDom[ver][1];
      vers[1] = (*pTess).VerDom[ver2][1];
      if (neut_tess_domvers_comdomedge ((*pTess), vers, 2, &domedge) == 0)
      {
        (*pTess).EdgeDom[edge][0] = 1;
        (*pTess).EdgeDom[edge][1] = domedge;
      }
      else
        ut_print_neperbug ();
    }

    // one ver is on a boundary ver and the other is not
    else if (((*pTess).VerDom[ver][0] == 0 || (*pTess).VerDom[ver2][0] == 0)
             && ((*pTess).VerDom[ver][0] != 0
                 || (*pTess).VerDom[ver2][0] != 0))
    {
      // id is not on the boundary ver
      id = ((*pTess).VerDom[ver][0] != 0) ? ver : ver2;

      // id is on a domain edge or face
      if ((*pTess).VerDom[id][0] == 1 || (*pTess).VerDom[id][0] == 2)
        ut_array_1d_int_memcpy ((*pTess).VerDom[id], 2,
                                (*pTess).EdgeDom[edge]);
      else
        ut_print_neperbug ();
    }

    // both vers are on boundary edges
    else if (((*pTess).VerDom[ver][0] == 1 && (*pTess).VerDom[ver2][0] == 1))
    {
      // if on the same edge
      if ((*pTess).VerDom[ver][1] == (*pTess).VerDom[ver2][1])
        ut_array_1d_int_memcpy ((*pTess).VerDom[ver], 2,
                                (*pTess).EdgeDom[edge]);
      // if on different edges
      else
      {
        if (neut_tess_domedges_comdomface ((*pTess), vers, 2, &domface) == 0)
        {
          (*pTess).EdgeDom[edge][0] = 2;
          (*pTess).EdgeDom[edge][1] = domface;
        }
        else
          ut_print_neperbug ();

        ut_array_1d_int_memcpy ((*pTess).VerDom[ver], 2,
                                (*pTess).EdgeDom[edge]);
      }
    }

    // one ver is on a boundary edge and the other is not
    else if (((*pTess).VerDom[ver][0] == 1 || (*pTess).VerDom[ver2][0] == 1)
             && ((*pTess).VerDom[ver][0] != 1
                 || (*pTess).VerDom[ver2][0] != 1))
    {
      // id is not on the boundary edge
      id = ((*pTess).VerDom[ver][0] != 0) ? ver : ver2;

      // id is on a domain edge or face
      if ((*pTess).VerDom[id][0] == 2)
        ut_array_1d_int_memcpy ((*pTess).VerDom[id], 2,
                                (*pTess).EdgeDom[edge]);
      else
        ut_print_neperbug ();
    }

    // both vers are on boundary faces
    else if (((*pTess).VerDom[ver][0] == 2 && (*pTess).VerDom[ver2][0] == 2))
    {
      // if on the same face
      if ((*pTess).VerDom[ver][1] == (*pTess).VerDom[ver2][1])
        ut_array_1d_int_memcpy ((*pTess).VerDom[ver], 2,
                                (*pTess).EdgeDom[edge]);
      else
        (*pTess).EdgeDom[edge][0] = -1;
    }
  }

  ut_free_1d_int (&vers);

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
