/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

// Delete an edge.  Returns 0 for success, -1 if the Tess test fails
// and -2 if the edge deletion aborted.  In 3D, if the maximum face
// flatness fault is higher than fftarget (but lower than twice
// fftarget), the position of the new vertex is optimized to decrease it,
// and the Tess test is not carried out (-1 is returned) if it is higher
// than fflimit.  If delcell == 1, the deletion can delete a cell (a
// triangular cell in 2D or a tetrahedral cell in 3D).
// In a periodic tessellation, all copies of the edge are deleted, and
// all copies of the resulting vertex are set accordingly.
int
net_reg_merge_del (struct TESS *pTess, int edge, int *pver, double *pmaxff,
                   double fftarget, double fflimit, int delcell, int **buf,
                   struct TESS *pTessBuf, int verbosity)
{
  int i, j, test, status = 0, periodic, master, *shift = ut_alloc_1d_int (3);
  int edgeqty = 0, *edges = NULL, **edgeoffsets = NULL;
  int verqty = 0, *vers = NULL, **veroffsets = NULL;
  int *newvers = NULL, *delvers = NULL;
  int faceqty = 0, *faces = NULL;
  int polyqty = 0, *polys = NULL;
  double **facetrans = NULL, maxff;
  int *domfacefaceqty = NULL, *domfacefaceqty2 = NULL;

  periodic = !strcmp ((*pTess).Type, "periodic");

  // setting default values
  (*pver) = -1;
  (*pmaxff) = 888.8;

  // copies of the edge (the edge itself for a non-periodic tessellation)
  if (periodic)
  {
    // an edge linking a vertex to its periodic copy cannot be deleted
    neut_tess_ver_master (*pTess, (*pTess).EdgeVerNb[edge][0], &master,
                          shift);
    if (net_reg_merge_del_isvermaster (*pTess, (*pTess).EdgeVerNb[edge][1],
                                       master))
    {
      if (verbosity)
        printf ("edge linking a vertex to its periodic copy\n");
      status = -2;
      goto end;
    }

    neut_tess_edge_copies (*pTess, edge, &edges, &edgeoffsets, &edgeqty);
  }
  else
  {
    edgeqty = 1;
    edges = ut_alloc_1d_int (1);
    edges[0] = edge;
    edgeoffsets = ut_alloc_2d_int (1, 3);
  }

  // vertices to delete and to keep (the new vertex), for each edge copy
  delvers = ut_alloc_1d_int (edgeqty);
  newvers = ut_alloc_1d_int (edgeqty);
  SearchDelNNewVer (pTess, edge, delvers, newvers);
  neut_tess_ver_master (*pTess, newvers[0], &master, shift);
  for (i = 1; i < edgeqty; i++)
  {
    j = net_reg_merge_del_isvermaster (*pTess, (*pTess).EdgeVerNb[edges[i]][1],
                                       master);
    newvers[i] = (*pTess).EdgeVerNb[edges[i]][j];
    delvers[i] = (*pTess).EdgeVerNb[edges[i]][!j];
  }

  // Buffering modified entities ---------------------------------------

  if (buf)
  {
    // copies of the vertices of the edge
    int qty, *tmp = NULL, **tmp2 = NULL;

    if (periodic)
      for (i = 0; i < 2; i++)
      {
        neut_tess_ver_copies (*pTess, (*pTess).EdgeVerNb[edge][i],
                                         &tmp, &tmp2, &qty);
        vers = ut_realloc_1d_int (vers, verqty + qty);
        ut_array_1d_int_memcpy (tmp, qty, vers + verqty);
        verqty += qty;
        ut_free_1d_int (&tmp);
        ut_free_2d_int (&tmp2, qty);
      }

    net_reg_merge_del_buffer (*pTess, edges, edgeqty, vers, verqty, buf,
                              pTessBuf);

    ut_free_1d_int (&vers);
    verqty = 0;
  }

  // End buffering entities --------------------------------------------

  if (verbosity >= 2)
    printf
      ("\ndeleting edge %5d  (length = %f)------------------------------\n",
       edge, (*pTess).EdgeLength[edge]);

  for (i = 0; i < edgeqty; i++)
    if (net_reg_merge_del_test (*pTess, edges[i], delcell, verbosity) != 0)
    {
      status = -2;
      goto end;
    }

  // numbers of faces of the domain faces (before deletion)
  if ((*pTess).Dim == 3)
    net_reg_merge_del_domfacefaceqty (*pTess, &domfacefaceqty);

  // polys of the edge, which can be deleted (a tetrahedral poly is
  // reduced to 2 faces)
  if (delcell && (*pTess).Dim == 3)
    neut_tess_edge_polys (*pTess, edge, &polys, &polyqty);

  for (i = 0; i < edgeqty; i++)
  {
    UpdateEdgeState (pTess, edges[i]);
    (*pTess).EdgeDel[edges[i]] = 1;

    /* UpdateFace updates the properties of the parent faces
     * of the edge and more generally of delver. */
    if (UpdateFaceVerNEdge (pTess, edges[i], delvers[i], newvers[i],
                            verbosity) != 0)
    {
      status = -2;
      goto end;
    }

    if (UpdateVer (pTess, edges[i], delvers[i], newvers[i], verbosity) != 0)
    {
      status = -2;
      goto end;
    }

    UpdateEdgeVerNb (pTess, delvers[i], newvers[i], verbosity);
  }

  for (i = 0; i < polyqty; i++)
    if (polys[i] > 0 && polys[i] <= (*pTess).PolyQty
        && (*pTess).PolyFaceQty[polys[i]] < 4)
      if (net_reg_merge_del_delpoly (pTess, polys[i], verbosity) != 0)
      {
        status = -2;
        goto end;
      }

  (*pver) = newvers[0];

  // copies of the new vertex, and their faces
  if (periodic)
  {
    if (net_reg_merge_del_per_vermerge (pTess, newvers, delvers, edgeqty)
        != 0)
    {
      status = -2;
      goto end;
    }
    neut_tess_ver_copies (*pTess, *pver, &vers, &veroffsets,
                                     &verqty);
    if ((*pTess).Dim == 3)
      net_reg_merge_del_per_verfaces (*pTess, vers, veroffsets, verqty,
                                      &faces, &facetrans, &faceqty);
  }
  else
  {
    verqty = 1;
    vers = ut_alloc_1d_int (1);
    vers[0] = *pver;
    veroffsets = ut_alloc_2d_int (1, 3);
  }

  if (UpdateVerCoo (pTess, delvers[0], *pver, faces, facetrans, faceqty,
                    verbosity) != 0)
  {
    status = -2;
    goto end;
  }

  if (periodic)
    net_reg_merge_del_per_setvers (pTess, *pver, vers, veroffsets, verqty);

  for (i = 0; i < verqty; i++)
  {
    UpdateEdgeLength (pTess, vers[i]);
    net_edgedel_edgedom_fromverdom (pTess, vers[i]);
  }

  if ((*pTess).Dim == 3)
  {
    (*pmaxff) = 0;
    for (i = 0; i < verqty; i++)
    {
      maxff = net_reg_merge_del_ff (pTess, vers[i]);
      (*pmaxff) = ut_num_max (*pmaxff, maxff);
    }

    // (only for a flatness fault close enough to the target)
    if (*pmaxff > fftarget && *pmaxff < 2 * fftarget)
      net_reg_merge_del_optimcoo (pTess, *pver, vers, veroffsets, verqty,
                                  periodic, fftarget, pmaxff);

    for (i = 0; i < verqty; i++)
      net_tess_reg_ver_facedom (pTess, vers[i]);
  }
  else
    (*pmaxff) = 0;

  if (verbosity >= 2)
    ut_print_message (0, 3, "Testing tessellation...\n");
  {
    if ((*pTess).Dim == 2)
    {
      test = 0;
      (*pmaxff) = 0;
    }
    else if (*pmaxff > fflimit)
      test = -1;
    else
    {
      test = 0;
      for (i = 0; i < verqty && test == 0; i++)
        test = neut_tess_testAroundVer2 (*pTess, vers[i], 0);

      // (a cell deletion can lead to folded polys)
      if (delcell)
        for (i = 0; i < verqty && test == 0; i++)
          test = net_reg_merge_del_testfold (*pTess, vers[i], 150);

      if (test != 0)
      {
        test = -1;
        (*pmaxff) = 888.8;
      }
    }
  }

  if (verbosity >= 2)
  {
    ut_print_lineheader (0);
    printf ("fmax=%.2f\n", (*pmaxff));
  }

  status = test;

  // the data of the modified entities must be consistent (which may not
  // be the case if the tessellation was invalid before the deletion)
  if (buf && net_reg_merge_del_testbuf (*pTess, buf) != 0)
    status = -2;

  // a domain face must not lose all its faces (which can happen for a
  // curved domain, e.g. the end faces of a cylinder, whose domain edges have
  // no domain vertices)
  if (domfacefaceqty)
  {
    net_reg_merge_del_domfacefaceqty (*pTess, &domfacefaceqty2);
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
      if (domfacefaceqty[i] > 0 && domfacefaceqty2[i] == 0)
      {
        if (verbosity)
          printf ("domain face %d would have no face\n", i);
        status = -2;
        break;
      }
  }

end:
  ut_free_1d_int (&domfacefaceqty);
  ut_free_1d_int (&domfacefaceqty2);
  ut_free_1d_int (&shift);
  ut_free_1d_int (&edges);
  ut_free_2d_int (&edgeoffsets, edgeqty);
  ut_free_1d_int (&newvers);
  ut_free_1d_int (&delvers);
  ut_free_1d_int (&vers);
  ut_free_2d_int (&veroffsets, verqty);
  ut_free_1d_int (&faces);
  ut_free_2d (&facetrans, faceqty);
  ut_free_1d_int (&polys);

  return status;
}

// Testing whether an edge can be deleted.  If delcell == 1, the
// deletion of an edge of a triangular (2D) or tetrahedral (3D) cell,
// which deletes the cell, is allowed.  Returns 0 if it can, -1
// otherwise.
int
net_reg_merge_del_test (struct TESS Tess, int edge, int delcell, int verbosity)
{
  int i;
  int ver1 = Tess.EdgeVerNb[edge][0];
  int ver2 = Tess.EdgeVerNb[edge][1];

  if (verbosity >= 2)
  {
    ut_print_lineheader (-1);
    printf ("has ver %d and %d of bounds %d %d and %d %d\n", ver1, ver2,
            Tess.VerDom[ver1][0], Tess.VerDom[ver1][1],
            Tess.VerDom[ver2][0], Tess.VerDom[ver2][1]);
  }

  // test for edge not on a domain edge, but shrinking into a domain
  // vertex - this case is not treated properly.
  if (Tess.VerDom[ver1][0] == 1 && Tess.VerDom[ver2][0] == 1
      && (Tess.VerDom[ver1][1] != Tess.VerDom[ver2][1]))
  {
    if (verbosity)
      printf ("edge not on a domain edge, but shrinking into a domain vertex - not supported\n");
    return -1;
  }

  // test for edge linking 2 domain vertices - this case is not treated properly.
  if (Tess.VerDom[ver1][0] == 0 && Tess.VerDom[ver2][0] == 0)
  {
    if (verbosity)
      printf ("edge linking 2 domain vertices - this case is not treated properly.\n");
    return -1;
  }

  // test for edge belonging to a tet polyhedron or tri face, which
  // would mean deleting the cell - this is not allowed, unless delcell
  // == 1 (in 3D, if the cell can be deleted).
  // test on the number of vertices, NOT on the number of edges or faces.
  // (a regularized 3D cell can have 4 faces, but > 4 vertices).
  if (delcell && Tess.Dim == 2)
    return net_reg_merge_del_test_link (Tess, edge);

  int *cell = NULL;
  int cellqty;
  int *ver = NULL;
  int verqty;
  neut_tess_edge_cells (Tess, edge, &cell, &cellqty);
  for (i = 0; i < cellqty; i++)
  {
    neut_tess_cell_vers (Tess, cell[i], &ver, &verqty);
    ut_free_1d_int (&ver);

    if ((Tess.Dim == 3 && verqty == 4
         && (!delcell || net_reg_merge_del_test_tet (Tess, cell[i], edge) != 0))
        || (Tess.Dim == 2 && verqty == 3))
    {
      ut_free_1d_int (&cell);
      return -1;
    }
  }
  ut_free_1d_int (&cell);

  /*
     // test for edge leading to the a "bow tie"-shaped face after being
     // deleted
     if (neut_tess_edge_fake (*pTess, edge) == 1)
     {
     int face1, face2;
     face1 = (*pTess).EdgeFaceNb[edge][0];
     face2 = (*pTess).EdgeFaceNb[edge][1];
     if ((*pTess).FaceVerQty[face1] > 3 && (*pTess).FaceVerQty[face2] > 3)
     {
     return -2;
     }
     }
   */

  return 0;
}

// Testing whether a vertex has a given master (in a periodic
// tessellation).  Returns 1 if it has, 0 otherwise.
int
net_reg_merge_del_isvermaster (struct TESS Tess, int ver, int master)
{
  int master2, *shift = ut_alloc_1d_int (3);

  neut_tess_ver_master (Tess, ver, &master2, shift);

  ut_free_1d_int (&shift);

  return master2 == master;
}

void
net_reg_merge_undel (struct TESS *pTess, int **buf, struct TESS TessBuf)
{
  int i, id;

  for (i = 1; i <= buf[0][0]; i++)
  {
    id = buf[0][i];

    ut_array_1d_memcpy (TessBuf.VerCoo[id], 3, (*pTess).VerCoo[id]);

    (*pTess).VerEdgeQty[id] = TessBuf.VerEdgeQty[id];

    (*pTess).VerEdgeNb[id] =
      ut_realloc_1d_int ((*pTess).VerEdgeNb[id], (*pTess).VerEdgeQty[id]);
    ut_array_1d_int_memcpy (TessBuf.VerEdgeNb[id], (*pTess).VerEdgeQty[id],
                            (*pTess).VerEdgeNb[id]);

    ut_array_1d_int_memcpy (TessBuf.VerDom[id], 2, (*pTess).VerDom[id]);

    (*pTess).VerState[id] = TessBuf.VerState[id];

    if ((*pTess).PerVerMaster)
    {
      (*pTess).PerVerMaster[id] = TessBuf.PerVerMaster[id];
      ut_array_1d_int_memcpy (TessBuf.PerVerShift[id], 3,
                              (*pTess).PerVerShift[id]);
      (*pTess).PerVerSlaveQty[id] = TessBuf.PerVerSlaveQty[id];
      (*pTess).PerVerSlaveNb[id] =
        ut_realloc_1d_int ((*pTess).PerVerSlaveNb[id],
                           (*pTess).PerVerSlaveQty[id] + 1);
      ut_array_1d_int_memcpy (TessBuf.PerVerSlaveNb[id] + 1,
                              (*pTess).PerVerSlaveQty[id],
                              (*pTess).PerVerSlaveNb[id] + 1);
    }
  }

  for (i = 1; i <= buf[1][0]; i++)
  {
    id = buf[1][i];

    ut_array_1d_int_memcpy (TessBuf.EdgeVerNb[id], 2, (*pTess).EdgeVerNb[id]);

    (*pTess).EdgeFaceQty[id] = TessBuf.EdgeFaceQty[id];

    ut_array_1d_int_memcpy (TessBuf.EdgeFaceNb[id], (*pTess).EdgeFaceQty[id],
                            (*pTess).EdgeFaceNb[id]);

    (*pTess).EdgeLength[id] = TessBuf.EdgeLength[id];

    ut_array_1d_int_memcpy (TessBuf.EdgeDom[id], 2, (*pTess).EdgeDom[id]);

    (*pTess).EdgeState[id] = TessBuf.EdgeState[id];

    (*pTess).EdgeDel[id] = TessBuf.EdgeDel[id];
  }

  for (i = 1; i <= buf[2][0]; i++)
  {
    id = buf[2][i];

    if ((*pTess).Dim == 3)
      ut_array_1d_int_memcpy (TessBuf.FacePoly[id], 2, (*pTess).FacePoly[id]);

    ut_array_1d_memcpy (TessBuf.FaceEq[id], 4, (*pTess).FaceEq[id]);

    (*pTess).FaceVerQty[id] = TessBuf.FaceVerQty[id];

    ut_array_1d_int_memcpy (TessBuf.FaceVerNb[id] + 1,
                            (*pTess).FaceVerQty[id],
                            (*pTess).FaceVerNb[id] + 1);

    (*pTess).FaceEdgeQty[id] = TessBuf.FaceEdgeQty[id];

    ut_array_1d_int_memcpy (TessBuf.FaceEdgeNb[id] + 1,
                            (*pTess).FaceEdgeQty[id],
                            (*pTess).FaceEdgeNb[id] + 1);

    ut_array_1d_int_memcpy (TessBuf.FaceEdgeOri[id] + 1,
                            (*pTess).FaceEdgeQty[id],
                            (*pTess).FaceEdgeOri[id] + 1);

    (*pTess).FacePt[id] = TessBuf.FacePt[id];

    ut_array_1d_memcpy (TessBuf.FacePtCoo[id], 3, (*pTess).FacePtCoo[id]);

    if ((*pTess).Dim == 3)
      ut_array_1d_int_memcpy (TessBuf.FaceDom[id], 2, (*pTess).FaceDom[id]);

    (*pTess).FaceState[id] = TessBuf.FaceState[id];
  }

  if ((*pTess).Dim == 3)
    for (i = 1; i <= buf[3][0]; i++)
    {
      id = buf[3][i];

      (*pTess).PolyFaceQty[id] = TessBuf.PolyFaceQty[id];
      (*pTess).PolyState[id] = TessBuf.PolyState[id];

      ut_array_1d_int_memcpy (TessBuf.PolyFaceNb[id] + 1,
                              (*pTess).PolyFaceQty[id],
                              (*pTess).PolyFaceNb[id] + 1);

      ut_array_1d_int_memcpy (TessBuf.PolyFaceOri[id] + 1,
                              (*pTess).PolyFaceQty[id],
                              (*pTess).PolyFaceOri[id] + 1);
    }

  return;
}

void
net_reg_merge_undel_free (int **buf, struct TESS *pTessBuf)
{
  int i, id;

  for (i = 1; i <= buf[0][0]; i++)
  {
    id = buf[0][i];

    ut_free_1d (&(*pTessBuf).VerCoo[id]);

    (*pTessBuf).VerEdgeQty[id] = 0;

    ut_free_1d_int (&(*pTessBuf).VerEdgeNb[id]);

    ut_free_1d_int (&(*pTessBuf).VerDom[id]);

    (*pTessBuf).VerState[id] = 0;

    if ((*pTessBuf).PerVerSlaveNb)
      ut_free_1d_int (&(*pTessBuf).PerVerSlaveNb[id]);
  }

  for (i = 1; i <= buf[1][0]; i++)
  {
    id = buf[1][i];

    ut_free_1d_int (&(*pTessBuf).EdgeVerNb[id]);

    (*pTessBuf).EdgeFaceQty[id] = 0;

    ut_free_1d_int (&(*pTessBuf).EdgeFaceNb[id]);

    (*pTessBuf).EdgeLength[id] = 0;

    ut_free_1d_int (&(*pTessBuf).EdgeDom[id]);

    (*pTessBuf).EdgeState[id] = 0;

    (*pTessBuf).EdgeDel[id] = 0;
  }

  for (i = 1; i <= buf[2][0]; i++)
  {
    id = buf[2][i];

    ut_free_1d_int (&(*pTessBuf).FacePoly[id]);

    ut_free_1d (&(*pTessBuf).FaceEq[id]);

    (*pTessBuf).FaceVerQty[id] = 0;
    (*pTessBuf).FaceEdgeQty[id] = 0;

    ut_free_1d_int (&(*pTessBuf).FaceVerNb[id]);

    ut_free_1d_int (&(*pTessBuf).FaceEdgeNb[id]);

    ut_free_1d_int (&(*pTessBuf).FaceEdgeOri[id]);

    (*pTessBuf).FacePt[id] = 0;

    ut_free_1d (&(*pTessBuf).FacePtCoo[id]);

    ut_free_1d_int (&(*pTessBuf).FaceDom[id]);

    (*pTessBuf).FaceState[id] = 0;
  }

  for (i = 1; i <= buf[3][0]; i++)
  {
    id = buf[3][i];

    (*pTessBuf).PolyFaceQty[id] = 0;

    ut_free_1d_int (&(*pTessBuf).PolyFaceNb[id]);

    ut_free_1d_int (&(*pTessBuf).PolyFaceOri[id]);
  }

  for (i = 0; i < 4; i++)
    buf[i][0] = 0;

  return;
}

// Recording that an edge was processed (EdgeDel), as well as its copies
// in a periodic tessellation (if it was not deleted)
void
net_reg_merge_del_edgedel (struct TESS *pTess, int edge)
{
  int i, edgeqty, *edges = NULL, **offsets = NULL;

  (*pTess).EdgeDel[edge] = 1;

  if (!strcmp ((*pTess).Type, "periodic") && (*pTess).EdgeState[edge] == 0)
  {
    neut_tess_edge_copies (*pTess, edge, &edges, &offsets, &edgeqty);

    for (i = 0; i < edgeqty; i++)
      (*pTess).EdgeDel[edges[i]] = 1;

    ut_free_1d_int (&edges);
    ut_free_2d_int (&offsets, edgeqty);
  }

  return;
}

// Testing the consistency of the data of the entities of a buffer (the
// entities modified by an edge deletion).  Returns 0 if consistent, -1
// otherwise.
int
net_reg_merge_del_testbuf (struct TESS Tess, int **buf)
{
  int i, j, id, face, edge, qty, pos1, pos2;

  // faces: their edges link successive vertices (the vertex and edge lists
  // can be shifted) and contain them
  for (i = 1; i <= buf[2][0]; i++)
  {
    face = buf[2][i];
    if (Tess.FaceState[face] == -1)
      continue;

    qty = Tess.FaceVerQty[face];
    for (j = 1; j <= Tess.FaceEdgeQty[face]; j++)
    {
      edge = Tess.FaceEdgeNb[face][j];

      if (Tess.EdgeState[edge] == -1
          || ut_array_1d_int_eltpos (Tess.EdgeFaceNb[edge],
                                     Tess.EdgeFaceQty[edge], face) == -1)
        return -1;

      pos1 = ut_array_1d_int_eltpos (Tess.FaceVerNb[face] + 1, qty,
                                     Tess.EdgeVerNb[edge][0]);
      pos2 = ut_array_1d_int_eltpos (Tess.FaceVerNb[face] + 1, qty,
                                     Tess.EdgeVerNb[edge][1]);
      if (pos1 == -1 || pos2 == -1
          || (abs (pos1 - pos2) != 1 && abs (pos1 - pos2) != qty - 1))
        return -1;
    }
  }

  // edges: their faces are not deleted and contain them, and their
  // vertices contain them
  for (i = 1; i <= buf[1][0]; i++)
  {
    edge = buf[1][i];
    if (Tess.EdgeState[edge] == -1)
      continue;

    for (j = 0; j < Tess.EdgeFaceQty[edge]; j++)
    {
      face = Tess.EdgeFaceNb[edge][j];
      // (not the periodic seeds of a 2D periodic tessellation)
      if (face < 1 || face > Tess.FaceQty)
        continue;

      if (Tess.FaceState[face] == -1
          || ut_array_1d_int_eltpos (Tess.FaceEdgeNb[face] + 1,
                                     Tess.FaceEdgeQty[face], edge) == -1)

        return -1;
    }

    for (j = 0; j < 2; j++)
    {
      id = Tess.EdgeVerNb[edge][j];
      if (Tess.VerState[id] == -1
          || ut_array_1d_int_eltpos (Tess.VerEdgeNb[id], Tess.VerEdgeQty[id],
                                     edge) == -1)
        return -1;
    }
  }

  return 0;
}

// Maximum face flatness fault of the faces of vertices
static double
net_reg_merge_del_optimcoo_ff (struct TESS *pTess, int *vers, int verqty)
{
  int i;
  double ff = 0;

  for (i = 0; i < verqty; i++)
    ff = ut_num_max (ff, net_reg_merge_del_ff (pTess, vers[i]));

  return ff;
}

// Optimizing the position of a (new) vertex to decrease the maximum face
// flatness fault of its faces (and of the faces of its copies, vers, in a
// periodic tessellation), from its current position and value (*pff),
// down to target.  A compass search is used, in the space allowed by the
// domain (the vertex can be on a planar domain face or a straight domain
// edge, and is fixed if it is a domain vertex).
void
net_reg_merge_del_optimcoo (struct TESS *pTess, int ver, int *vers,
                            int **offsets, int verqty, int periodic,
                            double target, double *pff)
{
  int i, j, dirqty, iter, improved, id;
  double h, h0, ff, *x0 = ut_alloc_1d (3);
  double **dirs = ut_alloc_2d (3, 3);
  double *n = ut_alloc_1d (3);

  // directions
  dirqty = 0;
  if ((*pTess).VerDom[ver][0] == -1)
  {
    dirqty = 3;
    for (i = 0; i < 3; i++)
      dirs[i][i] = 1;
  }
  else if ((*pTess).VerDom[ver][0] == 2)
  {
    id = (*pTess).VerDom[ver][1];
    if (!strncmp ((*pTess).DomFaceType[id], "plane", 5))
    {
      dirqty = 2;
      ut_array_1d_memcpy ((*pTess).DomFaceEq[id] + 1, 3, n);
      ut_array_1d_normalize (n, 3);
      // the axis least aligned with n, projected onto the plane, and its
      // cross product with n
      i = ut_array_1d_absmin_index (n, 3);
      for (j = 0; j < 3; j++)
        dirs[0][j] = ((j == i) ? 1 : 0) - n[i] * n[j];
      ut_array_1d_normalize (dirs[0], 3);
      ut_vector_vectprod (n, dirs[0], dirs[1]);
    }
  }
  else if ((*pTess).VerDom[ver][0] == 1)
  {
    id = (*pTess).VerDom[ver][1];
    // a straight edge is between two planar faces (the edge between a
    // planar face and a curved face is curved, and has no domain vertex if
    // it is closed)
    if (!strcmp ((*pTess).DomEdgeType[id], "line")
        && (*pTess).DomEdgeVerQty[id] == 2
        && !strncmp ((*pTess).DomFaceType[(*pTess).DomEdgeFaceNb[id][0]], "plane", 5)
        && !strncmp ((*pTess).DomFaceType[(*pTess).DomEdgeFaceNb[id][1]], "plane", 5))
    {
      dirqty = 1;
      ut_space_points_uvect ((*pTess).DomVerCoo[(*pTess).DomEdgeVerNb[id][0]],
                             (*pTess).DomVerCoo[(*pTess).DomEdgeVerNb[id][1]],
                             dirs[0]);
    }
  }

  if (dirqty > 0)
  {
    h0 = DBL_MAX;
    for (i = 0; i < (*pTess).VerEdgeQty[ver]; i++)
      h0 = ut_num_min (h0, (*pTess).EdgeLength[(*pTess).VerEdgeNb[ver][i]]);
    h0 *= 0.25;
    h = h0;

    iter = 0;
    while (*pff > target && h > 1e-3 * h0 && iter < 50)
    {
      improved = 0;
      for (i = 0; i < dirqty && !improved; i++)
        for (j = -1; j <= 1 && !improved; j += 2)
        {
          iter++;
          ut_array_1d_memcpy ((*pTess).VerCoo[ver], 3, x0);
          for (id = 0; id < 3; id++)
            (*pTess).VerCoo[ver][id] += j * h * dirs[i][id];
          if (periodic)
            net_reg_merge_del_per_setvers (pTess, ver, vers, offsets, verqty);

          ff = net_reg_merge_del_optimcoo_ff (pTess, vers, verqty);

          if (ff < *pff)
          {
            (*pff) = ff;
            improved = 1;
          }
          else
          {
            ut_array_1d_memcpy (x0, 3, (*pTess).VerCoo[ver]);
            if (periodic)
              net_reg_merge_del_per_setvers (pTess, ver, vers, offsets,
                                             verqty);
          }
        }

      if (!improved)
        h *= 0.5;
    }

    // (resetting the face interpolations and the edge lengths)
    (*pff) = net_reg_merge_del_optimcoo_ff (pTess, vers, verqty);
    for (i = 0; i < verqty; i++)
      UpdateEdgeLength (pTess, vers[i]);
  }

  ut_free_1d (&x0);
  ut_free_2d (&dirs, 3);
  ut_free_1d (&n);

  return;
}
