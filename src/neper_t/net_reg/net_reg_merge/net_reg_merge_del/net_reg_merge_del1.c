/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

// Delete an edge.  Returns 0 for success, -1 if the Tess test fails
// and -2 if the edge deletion aborted.
int
net_reg_merge_del (struct TESS *pTess, int edge, int *pver, double *pmaxff,
                   int **buf, struct TESS *pTessBuf, int verbosity)
{
  int i, delver, test;

  // Buffering modified entities ---------------------------------------
  //
  if (buf)
    net_reg_merge_del_buffer (*pTess, edge, buf, pTessBuf);

  // End buffering entities --------------------------------------------

  // setting default values
  (*pver) = -1;
  (*pmaxff) = 888.8;

  if (verbosity >= 2)
    printf
      ("\ndeleting edge %5d  (length = %f)------------------------------\n",
       edge, (*pTess).EdgeLength[edge]);

  int ver1 = (*pTess).EdgeVerNb[edge][0];
  int ver2 = (*pTess).EdgeVerNb[edge][1];

  if (verbosity >= 2)
  {
    ut_print_lineheader (-1);
    printf ("has ver %d and %d of bounds %d %d and %d %d\n", ver1, ver2,
            (*pTess).VerDom[ver1][0], (*pTess).VerDom[ver1][1],
            (*pTess).VerDom[ver2][0], (*pTess).VerDom[ver2][1]);
  }

  // test for edge not on a domain edge, but shrinking into a domain
  // vertex - this case is not treated properly.
  if ((*pTess).VerDom[ver1][0] == 1 && (*pTess).VerDom[ver2][0] == 1
      && ((*pTess).VerDom[ver1][1] != (*pTess).VerDom[ver2][1]))
  {
    if (verbosity)
      printf ("edge not on a domain edge, but shrinking into a domain vertex - not supported\n");
    return -2;
  }

  // test for edge linking 2 domain vertices - this case is not treated properly.
  if ((*pTess).VerDom[ver1][0] == 0 && (*pTess).VerDom[ver2][0] == 0)
  {
    if (verbosity)
      printf ("edge linking 2 domain vertices - this case is not treated properly.\n");
    return -2;
  }

  // test for edge equal to a domain edge.
  if ((*pTess).EdgeDom[edge][0] == 1
      && (*pTess).DomTessEdgeQty[(*pTess).EdgeDom[edge][1]] == 1)
  {
    if (verbosity)
      printf ("edge equal to a domain edge\n");
    return -2;
  }

  // test for edge belonging to a tet polyhedron or tri face, which
  // would mean deleting the cell - this is not allowed.
  // test on the number of vertices, NOT on the number of edges or faces.
  // (a regularized 3D cell can have 4 faces, but > 4 vertices).
  int *cell = NULL;
  int cellqty;
  int *ver = NULL;
  int verqty;
  neut_tess_edge_cells (*pTess, edge, &cell, &cellqty);
  for (i = 0; i < cellqty; i++)
  {
    neut_tess_cell_vers (*pTess, cell[i], &ver, &verqty);

    if (((*pTess).Dim == 3 && verqty == 4)
        || ((*pTess).Dim == 2 && verqty == 3))
    {
      ut_free_1d_int (&cell);
      return -2;
    }
    ut_free_1d_int (&ver);
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

  UpdateEdgeState (pTess, edge);
  (*pTess).EdgeDel[edge] = 1;

  SearchDelNNewVer (pTess, edge, &delver, pver);

  /* UpdateFace updates the properties of the parent faces
   * of the edge and more generally of delver. */
  if (UpdateFaceVerNEdge (pTess, edge, delver, *pver, verbosity) != 0)
    return -2;

  if (UpdateVer (pTess, edge, delver, *pver, verbosity) != 0)
    return -2;

  UpdateEdgeVerNb (pTess, delver, *pver, verbosity);

  if (UpdateVerCoo (pTess, delver, *pver, verbosity) != 0)
    return -2;

  UpdateEdgeLength (pTess, *pver);

  net_edgedel_edgedom_fromverdom (pTess, *pver);

  if ((*pTess).Dim == 3)
  {
    (*pmaxff) = net_reg_merge_del_ff (pTess, *pver);
    net_tess_reg_ver_facedom (pTess, *pver);
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
    else
    {
      test = neut_tess_testAroundVer2 (*pTess, *pver, 0);

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

  return test;
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

    ut_array_1d_int_memcpy (TessBuf.FaceEdgeNb[id] + 1,
                            (*pTess).FaceVerQty[id],
                            (*pTess).FaceEdgeNb[id] + 1);

    ut_array_1d_int_memcpy (TessBuf.FaceEdgeOri[id] + 1,
                            (*pTess).FaceVerQty[id],
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
