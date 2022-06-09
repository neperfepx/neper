/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_topo_.h"

void
neut_tess_ver_edges (struct TESS Tess, int ver, int **pedges, int *pedgeqty)
{
  ut_free_1d_int (pedges);

  (*pedgeqty) = Tess.VerEdgeQty[ver];
  (*pedges) = ut_alloc_1d_int (*pedgeqty);

  ut_array_1d_int_memcpy (Tess.VerEdgeNb[ver], *pedgeqty, *pedges);

  return;
}

void
neut_tess_ver_edges_all (struct TESS Tess, int ver, int **pedge,
                         int *pedgeqty)
{
  int i, master, slave, prevqty;

  ut_free_1d_int (pedge);
  (*pedgeqty) = 0;

  if (!strcmp (Tess.Type, "standard"))
  {
    (*pedgeqty) = Tess.VerEdgeQty[ver];
    (*pedge) = ut_alloc_1d_int (*pedgeqty);
    ut_array_1d_int_memcpy (Tess.VerEdgeNb[ver], *pedgeqty, *pedge);
  }

  else
  {
    master = Tess.PerVerMaster[ver] ? Tess.PerVerMaster[ver] : ver;

    (*pedgeqty) = Tess.VerEdgeQty[master];
    (*pedge) = ut_alloc_1d_int (*pedgeqty);
    ut_array_1d_int_memcpy (Tess.VerEdgeNb[master], *pedgeqty, *pedge);

    for (i = 1; i <= Tess.PerVerSlaveQty[master]; i++)
    {
      prevqty = (*pedgeqty);

      slave = Tess.PerVerSlaveNb[master][i];
      (*pedgeqty) += Tess.VerEdgeQty[slave];
      (*pedge) = ut_realloc_1d_int (*pedge, *pedgeqty);
      ut_array_1d_int_memcpy (Tess.VerEdgeNb[slave], Tess.VerEdgeQty[slave],
                              *pedge + prevqty);
    }

    // turning everyone into a master edge
    for (i = 0; i < *pedgeqty; i++)
      while (Tess.PerEdgeMaster[(*pedge)[i]] != 0)
        (*pedge)[i] = Tess.PerEdgeMaster[(*pedge)[i]];
  }

  ut_array_1d_int_sort_uniq (*pedge, *pedgeqty, pedgeqty);

  return;
}

void
neut_tess_ver_faces (struct TESS Tess, int ver, int **pface, int *pfaceqty)
{
  int i, j, tmpedge;
  int *tmp = NULL;

  (*pfaceqty) = 0;
  for (i = 0; i < Tess.VerEdgeQty[ver]; i++)
  {
    tmpedge = Tess.VerEdgeNb[ver][i];
    tmp = ut_realloc_1d_int (tmp, *pfaceqty + Tess.EdgeFaceQty[tmpedge]);
    for (j = 0; j < Tess.EdgeFaceQty[tmpedge]; j++)
      tmp[(*pfaceqty)++] = Tess.EdgeFaceNb[tmpedge][j];
  }

  if (Tess.Dim == 2)
  {
    for (i = 0; i < *pfaceqty; i++)
      if (tmp[i] < 0 || tmp[i] > Tess.FaceQty)
        tmp[i] = 0;
    (*pfaceqty) -= ut_array_1d_int_rmelt (tmp, (*pfaceqty), 0, (*pfaceqty));
  }
  ut_array_1d_int_sort_uniq (tmp, *pfaceqty, pfaceqty);

  if (pface)
  {
    (*pface) = ut_alloc_1d_int (*pfaceqty);
    ut_array_1d_int_memcpy (tmp, *pfaceqty, *pface);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_vers_faces (struct TESS Tess, int *vers, int verqty, int **pfaces,
                      int *pfaceqty)
{
  int i;
  int **verfaces = NULL, *verfaceqty = NULL;

  verfaces = ut_alloc_1d_pint (verqty);

  for (i = 0; i < verqty; i++)
    neut_tess_ver_faces (Tess, vers[i], verfaces + i, verfaceqty + i);

  ut_array_1d_int_lists_merge (verfaces, verfaceqty, verqty, pfaces,
                               pfaceqty);

  ut_free_2d_int (&verfaces, verqty);
  ut_free_1d_int (&verfaceqty);

  return;
}

void
neut_tess_vers_faces_notondomain (struct TESS Tess, int *vers, int verqty,
                                  int **pfaces, int *pfaceqty)
{
  int i;

  neut_tess_vers_faces (Tess, vers, verqty, pfaces, pfaceqty);

  for (i = 0; i < *pfaceqty; i++)
    if (Tess.FaceDom[i][0] == 2)
      ut_array_1d_int_list_rmval (pfaces, pfaceqty, (*pfaces)[i]);

  return;
}

void
neut_tess_edge_faces (struct TESS Tess, int edge, int **pfaces, int *pfaceqty)
{
  ut_free_1d_int (pfaces);

  (*pfaceqty) = Tess.EdgeFaceQty[edge];
  (*pfaces) = ut_alloc_1d_int (*pfaceqty);
  ut_array_1d_int_memcpy (Tess.EdgeFaceNb[edge], *pfaceqty, *pfaces);

  return;
}

void
neut_tess_ver_poly_faces (struct TESS Tess, int ver, int poly, int **pface,
                          int *pfaceqty)
{
  int i;

  neut_tess_ver_faces (Tess, ver, pface, pfaceqty);

  for (i = 0; i < (*pfaceqty); i++)
    if (ut_array_1d_int_eltpos (Tess.FacePoly[(*pface)[i]], 2, poly) == -1)
      (*pface)[i] = 0;

  (*pfaceqty) -= ut_array_1d_int_rmelt (*pface, *pfaceqty, 0, *pfaceqty);

  return;
}

void
neut_tess_ver_seeds (struct TESS Tess, int ver, int **pseed, int *pseedqty)
{
  int i, j, faceqty, edge, qty2;
  int *face = NULL;
  int *tmp = NULL, *tmp2 = NULL;

  (*pseedqty) = 0;

  if (Tess.Dim == 3)
  {
    neut_tess_ver_faces (Tess, ver, &face, &faceqty);

    tmp = ut_alloc_1d_int (2 * faceqty);

    (*pseedqty) = 0;
    for (i = 0; i < faceqty; i++)
      for (j = 0; j < 2; j++)
        tmp[(*pseedqty)++] = Tess.FacePoly[face[i]][j];
  }

  else
  {
    for (i = 0; i < Tess.VerEdgeQty[ver]; i++)
    {
      edge = Tess.VerEdgeNb[ver][i];
      neut_tess_edge_seeds (Tess, edge, &tmp2, &qty2);
      tmp = ut_realloc_1d_int (tmp, *pseedqty + qty2);
      ut_array_1d_int_memcpy (tmp2, qty2, tmp + *pseedqty);
      (*pseedqty) += qty2;
    }
  }

  ut_array_1d_int_sort_uniq (tmp, *pseedqty, pseedqty);

  if (pseed)
  {
    (*pseed) = ut_realloc_1d_int (*pseed, *pseedqty);
    ut_array_1d_int_memcpy (tmp, *pseedqty, *pseed);
  }

  ut_free_1d_int (&face);
  ut_free_1d_int (&tmp);

  return;
}

// same result as neut_tess_ver_cells
void
neut_tess_ver_masterseeds (struct TESS Tess, int ver, int **pseed,
                           int *pseedqty)
{
  int i;

  neut_tess_ver_seeds (Tess, ver, pseed, pseedqty);

  for (i = 0; i < (*pseedqty); i++)
    if ((*pseed)[i] > Tess.CellQty)
    {
      (*pseedqty) -=
        ut_array_1d_int_rmelt (*pseed, (*pseedqty), (*pseed)[i], 1);
      i--;
    }

  return;
}

// same result as neut_tess_edge_cells
void
neut_tess_edge_masterseeds (struct TESS Tess, int edge, int **pseed,
                            int *pseedqty)
{
  int i;

  neut_tess_edge_seeds (Tess, edge, pseed, pseedqty);

  for (i = 0; i < (*pseedqty); i++)
    if ((*pseed)[i] > Tess.CellQty)
    {
      (*pseedqty) -=
        ut_array_1d_int_rmelt (*pseed, (*pseedqty), (*pseed)[i], 1);
      i--;
    }

  return;
}

// same result as neut_tess_face_cells
void
neut_tess_face_masterseeds (struct TESS Tess, int face, int **pseed,
                            int *pseedqty)
{
  int i;

  neut_tess_face_seeds (Tess, face, pseed, pseedqty);

  for (i = 0; i < (*pseedqty); i++)
    if ((*pseed)[i] > Tess.CellQty)
    {
      (*pseedqty) -=
        ut_array_1d_int_rmelt (*pseed, (*pseedqty), (*pseed)[i], 1);
      i--;
    }

  return;
}

void
neut_tess_ver_seeds_positive (struct TESS Tess, int ver, int **pseed,
                              int *pseedqty)
{
  int i, qty, *tmp = NULL;

  neut_tess_ver_seeds (Tess, ver, &tmp, &qty);

  for (i = 0; i < qty; i++)
    if (tmp[i] < 0)
      tmp[i] = 0;

  qty -= ut_array_1d_int_rmelt (tmp, qty, 0, qty);

  if (pseed)
  {
    (*pseed) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pseed);
  }

  if (pseedqty)
    (*pseedqty) = qty;

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_ver_polys (struct TESS Tess, int ver, int **ppoly, int *ppolyqty)
{
  int i, qty, *tmp = NULL;

  neut_tess_ver_seeds (Tess, ver, &tmp, &qty);

  for (i = 0; i < qty; i++)
    if (tmp[i] < 0 || tmp[i] > Tess.PolyQty)
      tmp[i] = 0;

  qty -= ut_array_1d_int_rmelt (tmp, qty, 0, qty);

  if (ppoly)
  {
    (*ppoly) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *ppoly);
  }

  if (ppolyqty)
    (*ppolyqty) = qty;

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_ver_domedges (struct TESS Tess, int ver, int **pdedges,
                        int *pdedgeqty)
{
  if (Tess.VerDom[ver][0] == -1)
    (*pdedgeqty) = 0;
  else if (Tess.VerDom[ver][0] == 0)    // is a dom ver
  {
    int domver = Tess.VerDom[ver][1];
    (*pdedgeqty) = Tess.DomVerEdgeQty[domver];
    (*pdedges) = ut_alloc_1d_int (*pdedgeqty);
    ut_array_1d_int_memcpy (Tess.DomVerEdgeNb[domver], *pdedgeqty, *pdedges);
  }
  else if (Tess.VerDom[ver][0] == 1)    // is on a dom edge
  {
    (*pdedgeqty) = 1;

    (*pdedges) = ut_alloc_1d_int (*pdedgeqty);
    (*pdedges)[0] = Tess.VerDom[ver][1];
  }
  else
    (*pdedgeqty) = 0;

  return;
}

void
neut_tess_vers_alldomedges (struct TESS Tess, int *vers, int verqty,
                            int **pdedges, int *pdedgeqty)
{
  int i, qty;
  int *tmp = NULL;

  (*pdedgeqty) = 0;

  for (i = 0; i < verqty; i++)
  {
    neut_tess_ver_domedges (Tess, vers[i], &tmp, &qty);

    (*pdedges) = ut_realloc_1d_int (*pdedges, *pdedgeqty + qty);
    ut_array_1d_int_memcpy (tmp, qty, (*pdedges) + (*pdedgeqty));
    (*pdedgeqty) += qty;
  }

  ut_array_1d_int_sort_uniq (*pdedges, *pdedgeqty, pdedgeqty);

  return;
}

void
neut_tess_ver_domfaces (struct TESS Tess, int ver, int **pdfaces,
                        int *pdfaceqty)
{
  int i;

  ut_free_1d_int (pdfaces);
  (*pdfaceqty) = 0;

  neut_tess_ver_faces (Tess, ver, pdfaces, pdfaceqty);
  for (i = 0; i < *pdfaceqty; i++)
    (*pdfaces)[i] =
      Tess.FaceDom[(*pdfaces)[i]][0] ==
      2 ? Tess.FaceDom[(*pdfaces)[i]][1] : -1;
  ut_array_1d_int_sort_uniq (*pdfaces, *pdfaceqty, pdfaceqty);
  (*pdfaceqty) -= ut_array_1d_int_rmelt (*pdfaces, *pdfaceqty, -1, 1);

  (*pdfaces) = ut_realloc_1d_int (*pdfaces, *pdfaceqty);

  return;
}

void
neut_tess_edge_domfaces (struct TESS Tess, int edge, int **pdfaces,
                         int *pdfaceqty)
{
  int i;

  neut_tess_edge_faces (Tess, edge, pdfaces, pdfaceqty);

  for (i = 0; i < *pdfaceqty; i++)
    (*pdfaces)[i] =
      Tess.FaceDom[(*pdfaces)[i]][0] ==
      2 ? Tess.FaceDom[(*pdfaces)[i]][1] : -1;
  ut_array_1d_int_sort_uniq (*pdfaces, *pdfaceqty, pdfaceqty);
  (*pdfaceqty) -= ut_array_1d_int_rmelt (*pdfaces, *pdfaceqty, -1, 1);

  (*pdfaces) = ut_realloc_1d_int (*pdfaces, *pdfaceqty);

  return;
}

void
neut_tess_edge_cells (struct TESS Tess, int edge, int **pcell, int *pcellqty)
{
  int i, face;

  ut_free_1d_int (pcell);
  (*pcellqty) = 0;

  if (Tess.Dim == 3)
    neut_tess_edge_polys (Tess, edge, pcell, pcellqty);

  else if (Tess.Dim == 2)
  {
    for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
    {
      face = Tess.EdgeFaceNb[edge][i];
      if (face > 0 && face <= Tess.FaceQty)
        ut_array_1d_int_list_addval_nocheck (pcell, pcellqty, face);
    }
  }

  else
    abort ();

  return;
}

void
neut_tess_edge_seeds (struct TESS Tess, int edge, int **pseed, int *pseedqty)
{
  int i, j, face, p;
  int *tmp = NULL;

  // the list of seed of an edge is the union of the seed of its faces.

  (*pseedqty) = 0;

  if (Tess.Dim == 3)
    for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
    {
      face = Tess.EdgeFaceNb[edge][i];

      for (j = 0; j < 2; j++)
      {
        p = Tess.FacePoly[face][j];

        tmp = ut_realloc_1d_int (tmp, (*pseedqty) + 1);
        tmp[(*pseedqty)++] = p;
      }
    }

  else if (Tess.Dim == 2)
  {
    (*pseedqty) = Tess.EdgeFaceQty[edge];
    tmp = ut_alloc_1d_int (*pseedqty);
    ut_array_1d_int_memcpy (Tess.EdgeFaceNb[edge], *pseedqty, tmp);
  }

  ut_array_1d_int_sort_uniq (tmp, *pseedqty, pseedqty);

  if (pseed)
  {
    (*pseed) = ut_realloc_1d_int (*pseed, *pseedqty);
    ut_array_1d_int_memcpy (tmp, *pseedqty, *pseed);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_edge_seeds_positive (struct TESS Tess, int edge, int **pseed,
                               int *pseedqty)
{
  int i, qty, *tmp = NULL;

  neut_tess_edge_seeds (Tess, edge, &tmp, &qty);

  for (i = 0; i < qty; i++)
    if (tmp[i] < 0)
      tmp[i] = 0;

  qty -= ut_array_1d_int_rmelt (tmp, qty, 0, qty);

  if (pseed)
  {
    (*pseed) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pseed);
  }

  if (pseedqty)
    (*pseedqty) = qty;

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_edge_polys (struct TESS Tess, int edge, int **ppoly, int *ppolyqty)
{
  int i, qty, *tmp = NULL;

  neut_tess_edge_seeds (Tess, edge, &tmp, &qty);

  for (i = 0; i < qty; i++)
    if (tmp[i] < 0 || tmp[i] > Tess.PolyQty)
      tmp[i] = 0;

  qty -= ut_array_1d_int_rmelt (tmp, qty, 0, qty);

  if (ppoly)
  {
    (*ppoly) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *ppoly);
  }

  if (ppolyqty)
    (*ppolyqty) = qty;

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_face_polys (struct TESS Tess, int face, int **ppoly, int *ppolyqty)
{
  int i, qty;
  int *tmp = ut_alloc_1d_int (2);

  qty = 0;
  for (i = 0; i < 2; i++)
    if (Tess.FacePoly[face][i] > 0)
    {
      qty++;
      tmp[qty - 1] = Tess.FacePoly[face][i];

      if (!strcmp (Tess.Type, "periodic"))
        if (Tess.PerSeedMaster[tmp[qty - 1]] > 0)
          tmp[qty - 1] = Tess.PerSeedMaster[tmp[qty - 1]];
    }

  ut_array_1d_int_sort (tmp, qty);

  if (ppoly)
  {
    (*ppoly) = ut_realloc_1d_int (*ppoly, qty);
    ut_array_1d_int_memcpy (tmp, qty, *ppoly);
  }

  if (ppolyqty)
    (*ppolyqty) = qty;

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_face_seeds (struct TESS Tess, int face, int **pseeds, int *pseedqty)
{
  if (Tess.Dim == 3)
  {
    (*pseedqty) = 2;
    (*pseeds) = ut_alloc_1d_int (*pseedqty);

    ut_array_1d_int_memcpy (Tess.FacePoly[face], *pseedqty, *pseeds);
  }

  else
    abort ();

  return;
}

void
neut_tess_face_seeds_positive (struct TESS Tess, int face, int **pseed,
                               int *pseedqty)
{
  int i, qty, *tmp = NULL;

  neut_tess_face_seeds (Tess, face, &tmp, &qty);

  for (i = 0; i < qty; i++)
    if (tmp[i] < 0)
      tmp[i] = 0;

  qty -= ut_array_1d_int_rmelt (tmp, qty, 0, qty);

  if (pseed)
  {
    (*pseed) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pseed);
  }

  if (pseedqty)
    (*pseedqty) = qty;

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_edges_vers (struct TESS Tess, int *edge, int edgeqty, int **pver,
                      int *pverqty)
{
  int i, id;

  (*pverqty) = 0;
  (*pver) = ut_alloc_1d_int (edgeqty * 2);
  for (i = 0; i < edgeqty; i++)
  {
    id = edge[i];
    ut_array_1d_int_memcpy (Tess.EdgeVerNb[id], 2, (*pver) + (*pverqty));
    (*pverqty) += 2;
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

void
neut_tess_edges_faces (struct TESS Tess, int *edge, int edgeqty, int **pface,
                       int *pfaceqty)
{
  int i, id;

  (*pfaceqty) = 0;
  ut_free_1d_int (pface);
  for (i = 0; i < edgeqty; i++)
  {
    id = edge[i];
    (*pface) = ut_realloc_1d_int (*pface, (*pfaceqty) + Tess.EdgeFaceQty[id]);
    ut_array_1d_int_memcpy (Tess.EdgeFaceNb[id], Tess.EdgeFaceQty[id],
                            (*pface) + (*pfaceqty));
    (*pfaceqty) += Tess.EdgeFaceQty[id];
  }

  ut_array_1d_int_sort_uniq (*pface, *pfaceqty, pfaceqty);

  return;
}

void
neut_tess_faces_polys (struct TESS Tess, int *face, int faceqty, int **ppoly,
                       int *ppolyqty)
{
  int i, id, qty, *tmp = NULL;

  (*ppolyqty) = 0;
  ut_free_1d_int (ppoly);
  for (i = 0; i < faceqty; i++)
  {
    id = face[i];
    neut_tess_face_polys (Tess, id, &tmp, &qty);
    (*ppoly) = ut_realloc_1d_int (*ppoly, (*ppolyqty) + qty);
    ut_array_1d_int_memcpy (tmp, qty, (*ppoly) + (*ppolyqty));
    (*ppolyqty) += qty;
  }

  ut_array_1d_int_sort_uniq (*ppoly, *ppolyqty, ppolyqty);

  return;
}

int
neut_tess_edge_domface_faces (struct TESS Tess, int edge, int dface,
                              int **pface, int *pfaceqty)
{
  int i, face;

  ut_free_1d_int (pface);
  (*pfaceqty) = 0;

  for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
  {
    face = Tess.EdgeFaceNb[edge][i];
    if (Tess.FaceDom[face][0] == 2 && Tess.FaceDom[face][1] == dface)
    {
      (*pface) = ut_realloc_1d_int (*pface, ++(*pfaceqty));
      (*pface)[*pfaceqty - 1] = face;
    }
  }

  return ((*pfaceqty) > 0) ? 0 : -1;
}

int
neut_tess_ver_domface_edges (struct TESS Tess, int ver, int dface,
                             int **pedge, int *pedgeqty)
{
  int i, edge, dedge, status;

  ut_free_1d_int (pedge);
  (*pedgeqty) = 0;

  for (i = 0; i < Tess.VerEdgeQty[ver]; i++)
  {
    edge = Tess.VerEdgeNb[ver][i];

    status = 0;
    if (Tess.EdgeDom[edge][0] == 2 && Tess.EdgeDom[edge][1] == dface)
      status = 1;
    else if (Tess.EdgeDom[edge][0] == 1)
    {
      // checking that the dedge belongs to the dface
      dedge = Tess.EdgeDom[edge][1];
      if (ut_array_1d_int_eltpos
          (Tess.DomFaceEdgeNb[dface] + 1, Tess.DomFaceEdgeQty[dface],
           dedge) != -1)
        status = 1;
    }

    if (status == 1)
    {
      (*pedge) = ut_realloc_1d_int (*pedge, ++(*pedgeqty));
      (*pedge)[*pedgeqty - 1] = edge;
    }
  }

  return ((*pedgeqty) > 0) ? 0 : -1;
}

int
neut_tess_faces_comvers (struct TESS Tess, int *face, int faceqty, int **pver,
                         int *pverqty)
{
  int i, status, qty;
  int *nb = NULL;

  ut_free_1d_int (pver);
  (*pverqty) = 0;

  if (faceqty <= 0)
    return -1;

  nb = ut_alloc_1d_int (Tess.FaceVerQty[face[0]]);
  qty = Tess.FaceVerQty[face[0]];
  ut_array_1d_int_memcpy (Tess.FaceVerNb[face[0]] + 1,
                          Tess.FaceVerQty[face[0]], nb);

  for (i = 1; i < faceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Tess.FaceVerNb[face[i]] + 1,
                           Tess.FaceVerQty[face[i]], nb, &qty);

  if (qty > 0)
  {
    (*pver) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (nb, qty, *pver);
    (*pverqty) = qty;
    status = 0;
  }
  else
    status = -1;

  ut_free_1d_int (&nb);

  return status;
}

int
neut_tess_faces_comedges (struct TESS Tess, int *face, int faceqty,
                          int **pedge, int *pedgeqty)
{
  int i, status, qty;
  int *nb = NULL;

  ut_free_1d_int (pedge);
  (*pedgeqty) = 0;

  if (faceqty <= 0)
    return -1;

  nb = ut_alloc_1d_int (Tess.FaceVerQty[face[0]]);
  qty = Tess.FaceVerQty[face[0]];
  ut_array_1d_int_memcpy (Tess.FaceEdgeNb[face[0]] + 1,
                          Tess.FaceVerQty[face[0]], nb);

  for (i = 1; i < faceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Tess.FaceEdgeNb[face[i]] + 1,
                           Tess.FaceVerQty[face[i]], nb, &qty);

  if (qty > 0)
  {
    (*pedge) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (nb, qty, *pedge);
    (*pedgeqty) = qty;
    status = 0;
  }
  else
    status = -1;

  ut_free_1d_int (&nb);

  return status;
}

int
neut_tess_faces_comedge (struct TESS Tess, int *face, int faceqty, int *pedge)
{
  int qty, *tmp = NULL, status;

  neut_tess_faces_comedges (Tess, face, faceqty, &tmp, &qty);

  if (qty == 1)
  {
    (*pedge) = tmp[0];
    status = 0;
  }
  else
  {
    (*pedge) = -1;
    status = -1;
  }

  ut_free_1d_int (&tmp);

  return status;
}

int
neut_tess_edges_comver (struct TESS Tess, int *edges, int edgeqty, int *pver)
{
  int i, qty, *tmp = NULL, status;

  qty = 2;
  tmp = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (Tess.EdgeVerNb[edges[0]], 2, tmp);

  for (i = 1; i < edgeqty; i++)
    ut_array_1d_int_inter (tmp, qty, Tess.EdgeVerNb[edges[i]], 2, tmp, &qty);

  if (qty == 1)
  {
    (*pver) = tmp[0];
    status = 0;
  }
  else
  {
    (*pver) = -1;
    status = -1;
  }

  ut_free_1d_int (&tmp);

  return status;
}

int
neut_tess_edgepair_comver (struct TESS Tess, int edge1, int edge2, int *pver)
{
  int status, *edges = ut_alloc_1d_int (2);

  ut_array_1d_int_set_2 (edges, edge1, edge2);

  status = neut_tess_edges_comver (Tess, edges, 2, pver);

  ut_free_1d_int (&edges);

  return status;
}

int
neut_tess_faces_compolys (struct TESS Tess, int *faces, int faceqty,
                          int **ppolys, int *ppolyqty)
{
  int i, status, qty, *tmp = NULL;

  if (faceqty <= 0)
    return -1;

  neut_tess_face_polys (Tess, faces[0], ppolys, ppolyqty);

  for (i = 1; i < faceqty && *ppolyqty > 0; i++)
  {
    neut_tess_face_polys (Tess, faces[i], &tmp, &qty);
    ut_array_1d_int_inter (*ppolys, *ppolyqty, tmp, qty, *ppolys, ppolyqty);
  }

  if (*ppolyqty > 0)
  {
    (*ppolys) = ut_realloc_1d_int (*ppolys, *ppolyqty);
    status = 0;
  }
  else
  {
    ut_free_1d_int (ppolys);
    status = -1;
  }

  ut_free_1d_int (&tmp);

  return status;
}

int
neut_tess_faces_compoly (struct TESS Tess, int *faces, int faceqty,
                         int *ppoly)
{
  int status, *tmp = NULL, qty;

  status = neut_tess_faces_compolys (Tess, faces, faceqty, &tmp, &qty);

  if (qty == 1)
  {
    (*ppoly) = tmp[0];
    status = 0;
  }
  else
  {
    (*ppoly) = -1;
    status = -1;
  }

  ut_free_1d_int (&tmp);

  return status;
}

void
neut_tess_edge_vers (struct TESS Tess, int edge, int **pver, int *pverqty)
{
  (*pverqty) = 2;
  (*pver) = ut_alloc_1d_int (*pverqty);
  ut_array_1d_int_memcpy (Tess.EdgeVerNb[edge], *pverqty, *pver);

  return;
}

void
neut_tess_face_vers (struct TESS Tess, int face, int **pver, int *pverqty)
{
  (*pverqty) = Tess.FaceVerQty[face];
  (*pver) = ut_alloc_1d_int (*pverqty);
  ut_array_1d_int_memcpy (Tess.FaceVerNb[face] + 1, *pverqty, *pver);

  return;
}

void
neut_tess_poly_vers (struct TESS Tess, int poly, int **pver, int *pverqty)
{
  int i, j, face;
  int *tmp = NULL;

  (*pverqty) = 0;

  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      if (ut_array_1d_int_eltpos (tmp, (*pverqty), Tess.FaceVerNb[face][j]) ==
          -1)
      {
        tmp = ut_realloc_1d_int (tmp, (*pverqty) + 1);
        tmp[(*pverqty)++] = Tess.FaceVerNb[face][j];
      }
  }

  if (pver)
  {
    (*pver) = ut_alloc_1d_int (*pverqty);
    ut_array_1d_int_memcpy (tmp, *pverqty, *pver);
    ut_array_1d_int_sort (*pver, *pverqty);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_poly_vercoos (struct TESS Tess, int poly, int **pvers,
                        double ***pvercoos, int *pverqty)
{
  int i, *tmp = NULL, qty;

  neut_tess_poly_vers (Tess, poly, &tmp, &qty);

  if (pverqty)
    (*pverqty) = qty;

  if (pvers)
  {
    (*pvers) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pvers);
  }

  if (pvercoos)
  {
    (*pvercoos) = ut_alloc_2d (qty, 3);
    for (i = 0; i < qty; i++)
      ut_array_1d_memcpy (Tess.VerCoo[tmp[i]], 3, (*pvercoos)[i]);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_poly_edges (struct TESS Tess, int poly, int **pedges, int *pedgeqty)
{
  int i, j, face;
  int *tmp = NULL;

  (*pedgeqty) = 0;

  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];
    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      if (ut_array_1d_int_eltpos (tmp, *pedgeqty, Tess.FaceEdgeNb[face][j]) ==
          -1)
      {
        tmp = ut_realloc_1d_int (tmp, (*pedgeqty) + 1);
        tmp[(*pedgeqty)++] = Tess.FaceEdgeNb[face][j];
      }
  }

  if (pedges)
  {
    (*pedges) = ut_realloc_1d_int (*pedges, *pedgeqty);
    ut_array_1d_int_memcpy (tmp, *pedgeqty, *pedges);
    ut_array_1d_int_sort (*pedges, *pedgeqty);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_poly_edgevers (struct TESS Tess, int poly, int **pedges,
                         int ***pedgevers, int *pedgeqty)
{
  int i, *tmp = NULL, qty;

  neut_tess_poly_edges (Tess, poly, &tmp, &qty);

  if (pedgeqty)
    (*pedgeqty) = qty;

  if (pedges)
  {
    (*pedges) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pedges);
  }

  if (pedgevers)
  {
    (*pedgevers) = ut_alloc_2d_int (qty, 2);
    for (i = 0; i < qty; i++)
      ut_array_1d_int_memcpy (Tess.EdgeVerNb[tmp[i]], 2, (*pedgevers)[i]);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_poly_faces (struct TESS Tess, int poly, int **pfaces, int *pfaceqty)
{
  (*pfaceqty) = Tess.PolyFaceQty[poly];
  (*pfaces) = ut_alloc_1d_int (*pfaceqty);

  ut_array_1d_int_memcpy (Tess.PolyFaceNb[poly] + 1, *pfaceqty, *pfaces);

  return;
}

void
neut_tess_poly_faceedges (struct TESS Tess, int poly, int **pfaces,
                          int ***pfaceedges, int **pfaceedgeqty,
                          int *pfaceqty)
{
  int i, face, *tmp = NULL, qty;

  neut_tess_poly_faces (Tess, poly, &tmp, &qty);

  if (pfaceqty)
    (*pfaceqty) = qty;

  if (pfaces)
  {
    (*pfaces) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pfaces);
  }

  if (pfaceedges && pfaceedgeqty)
  {
    (*pfaceedges) = ut_alloc_1d_pint (qty);
    (*pfaceedgeqty) = ut_alloc_1d_int (qty);
    for (i = 0; i < qty; i++)
    {
      face = tmp[i];
      (*pfaceedgeqty)[i] = Tess.FaceVerQty[face];
      (*pfaceedges)[i] = ut_alloc_1d_int ((*pfaceedgeqty)[i]);
      ut_array_1d_int_memcpy (Tess.FaceEdgeNb[face] + 1, (*pfaceedgeqty)[i],
                              (*pfaceedges)[i]);
    }
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_poly_edge_faces (struct TESS Tess, int poly, int edge, int **pface)
{
  int i, f, qty = 0;

  (*pface) = ut_alloc_1d_int (2);

  for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
  {
    f = Tess.EdgeFaceNb[edge][i];
    if (ut_array_1d_int_eltpos (Tess.FacePoly[f], 2, poly) != -1)
      (*pface)[qty++] = f;
  }

  if (qty != 2)
    ut_print_neperbug ();

  return;
}

void
neut_tess_face_edge_ori (struct TESS Tess, int face, int edge, int *pedgeori)
{
  int pos;

  pos =
    1 + ut_array_1d_int_eltpos (Tess.FaceEdgeNb[face] + 1,
                                Tess.FaceVerQty[face], edge);
  if (pos == 0)
    (*pedgeori) = -1;
  else
    (*pedgeori) = Tess.FaceEdgeOri[face][pos];

  return;
}

void
neut_tess_poly_face_ori (struct TESS Tess, int poly, int face, int *pfaceori)
{
  int pos;

  pos =
    1 + ut_array_1d_int_eltpos (Tess.PolyFaceNb[poly] + 1,
                                Tess.PolyFaceQty[poly], face);
  if (pos == 0)
    (*pfaceori) = 0;
  else
    (*pfaceori) = Tess.PolyFaceOri[poly][pos];

  return;
}

int
neut_tess_cell_true (struct TESS Tess, int nb)
{
  int i, status, verid;
  double dist;
  int verqty;
  int *ver = NULL;
  double *proj = NULL, *eq = NULL;
  double domdist;

  status = 1;

  if (!Tess.SeedCoo)
    return 0;

  if ((Tess.Dim == 3 && Tess.PolyFaceQty[nb] == 0)
      || (Tess.Dim == 2 && Tess.FaceVerQty[nb] == 0))
    return 0;

  proj = ut_alloc_1d (3);
  eq = ut_alloc_1d (3);

  if (Tess.Dim == 3)
  {
    if (Tess.DomFaceQty == 0)
      ut_print_neperbug ();

    if (Tess.PolyFaceQty[nb] > 0)
    {
      domdist = DBL_MAX;
      for (i = 1; i <= Tess.DomFaceQty; i++)
      {
        dist =
          ut_space_point_plane_dist (Tess.SeedCoo[nb], Tess.DomFaceEq[i]);
        domdist = ut_num_min (domdist, dist);
      }

      neut_tess_poly_vers (Tess, nb, &ver, &verqty);

      for (i = 0; i < verqty; i++)
      {
        verid = ver[i];
        dist = ut_space_dist (Tess.SeedCoo[nb], Tess.VerCoo[verid]);
        if (dist > .5 * domdist)
        {
          status = 0;
          break;
        }
      }
    }
    else
      status = 0;
  }

  else if (Tess.Dim == 2)
  {
    if (Tess.DomEdgeQty == 0)
      ut_print_neperbug ();

    if (Tess.FaceVerQty[nb] > 0)
    {
      domdist = DBL_MAX;
      for (i = 1; i <= Tess.DomEdgeQty; i++)
      {
        neut_tess_domedge_eq (Tess, i, eq);
        ut_space_point_line_dist_2d (Tess.SeedCoo[nb], eq, &dist);
        domdist = ut_num_min (domdist, dist);
      }

      for (i = 1; i <= Tess.FaceVerQty[nb]; i++)
      {
        verid = Tess.FaceVerNb[nb][i];
        dist = ut_space_dist (Tess.SeedCoo[nb], Tess.VerCoo[verid]);
        if (dist > .5 * domdist)
        {
          status = 0;
          break;
        }
      }
    }
    else
      status = 0;
  }

  ut_free_1d_int (&ver);
  ut_free_1d (&proj);
  ut_free_1d (&eq);

  return status;
}

int
neut_tess_cell_body (struct TESS Tess, int nb)
{
  int i, id;

  if ((Tess.Dim == 3 && Tess.PolyFaceQty[nb] == 0)
      || (Tess.Dim == 2 && Tess.FaceVerQty[nb] == 0))
    return 0;

  if (Tess.Dim == 3)
  {
    if (Tess.PolyFaceQty[nb] == 0)
      return 0;
    else
      for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
      {
        id = Tess.PolyFaceNb[nb][i];
        if (Tess.FaceDom[id][0] == 2)
          return 0;
      }
  }

  else if (Tess.Dim == 2)
  {
    if (Tess.FaceVerQty[nb] == 0)
      return 0;
    else
      for (i = 1; i <= Tess.FaceVerQty[nb]; i++)
      {
        id = Tess.FaceEdgeNb[nb][i];
        if (Tess.EdgeDom[id][0] == 1)
          return 0;
      }
  }

  else
    abort ();

  return 1;
}

int
neut_tess_poly_true (struct TESS Tess, int nb)
{
  return (Tess.Dim == 3) ? Tess.CellTrue[nb] : -1;
}

int
neut_tess_poly_body (struct TESS Tess, int nb)
{
  return (Tess.Dim == 3) ? Tess.CellBody[nb] : -1;
}

int
neut_tess_face_true (struct TESS Tess, int nb)
{
  int i, res;

  if (Tess.Dim == 3)
  {
    int polyqty;
    int *polys = NULL;
    int *celltrue = NULL;
    neut_tess_face_polys (Tess, nb, &polys, &polyqty);
    celltrue = ut_alloc_1d_int (polyqty);

    for (i = 0; i < polyqty; i++)
      celltrue[i] = Tess.CellTrue[polys[i]];

    res = ut_array_1d_int_max (celltrue, polyqty);

    ut_free_1d_int (&polys);
    ut_free_1d_int (&celltrue);
  }
  else if (Tess.Dim == 2)
    res = Tess.CellTrue[nb];
  else
    res = 0;

  return res;
}

int
neut_tess_face_body (struct TESS Tess, int nb)
{
  int i, res;

  if (Tess.Dim == 3)
  {
    int polyqty;
    int *polys = NULL;
    int *cellbody = NULL;

    neut_tess_face_polys (Tess, nb, &polys, &polyqty);
    cellbody = ut_alloc_1d_int (polyqty);

    for (i = 0; i < polyqty; i++)
      cellbody[i] = Tess.CellBody[polys[i]];

    res = ut_array_1d_int_max (cellbody, polyqty);

    ut_free_1d_int (&polys);
    ut_free_1d_int (&cellbody);
  }
  else if (Tess.Dim == 2)
    res = Tess.CellBody[nb];
  else
    res = 0;

  return res;
}

int
neut_tess_edge_true (struct TESS Tess, int nb)
{
  int i, res, polyqty;
  int *polys = NULL;
  int *celltrue = NULL;

  if (Tess.Dim == 3)
  {
    neut_tess_edge_polys (Tess, nb, &polys, &polyqty);
    celltrue = ut_alloc_1d_int (polyqty);

    for (i = 0; i < polyqty; i++)
      celltrue[i] = Tess.CellTrue[polys[i]];

    res = ut_array_1d_int_max (celltrue, polyqty);

    ut_free_1d_int (&polys);
    ut_free_1d_int (&celltrue);
  }

  else if (Tess.Dim == 2)
  {
    celltrue = ut_alloc_1d_int (Tess.EdgeFaceQty[nb]);

    for (i = 0; i < Tess.EdgeFaceQty[nb]; i++)
      celltrue[i] = Tess.CellTrue[Tess.EdgeFaceNb[nb][i]];

    res = ut_array_1d_int_max (celltrue, Tess.EdgeFaceQty[nb]);

    ut_free_1d_int (&celltrue);
  }

  else
    res = 0;

  return res;
}

int
neut_tess_edge_body (struct TESS Tess, int nb)
{
  int i, res, polyqty;
  int *polys = NULL;
  int *cellbody = NULL;

  if (Tess.Dim == 3)
  {
    neut_tess_edge_polys (Tess, nb, &polys, &polyqty);
    cellbody = ut_alloc_1d_int (polyqty);

    for (i = 0; i < polyqty; i++)
      cellbody[i] = Tess.CellBody[polys[i]];

    res = ut_array_1d_int_max (cellbody, polyqty);

    ut_free_1d_int (&polys);
    ut_free_1d_int (&cellbody);
  }

  else if (Tess.Dim == 2)
  {
    cellbody = ut_alloc_1d_int (Tess.EdgeFaceQty[nb]);

    for (i = 0; i < Tess.EdgeFaceQty[nb]; i++)
      cellbody[i] = Tess.CellBody[Tess.EdgeFaceNb[nb][i]];

    res = ut_array_1d_int_max (cellbody, Tess.EdgeFaceQty[nb]);

    ut_free_1d_int (&cellbody);
  }

  else
    res = 0;

  return res;
}

int
neut_tess_ver_true (struct TESS Tess, int nb)
{
  int i, res, polyqty;
  int *polys = NULL;
  int *celltrue = NULL;

  neut_tess_ver_polys (Tess, nb, &polys, &polyqty);
  celltrue = ut_alloc_1d_int (polyqty);

  for (i = 0; i < polyqty; i++)
    celltrue[i] = Tess.CellTrue[polys[i]];

  res = ut_array_1d_int_max (celltrue, polyqty);

  ut_free_1d_int (&polys);
  ut_free_1d_int (&celltrue);

  return res;
}

int
neut_tess_ver_body (struct TESS Tess, int nb)
{
  int i, res, polyqty;
  int *polys = NULL;
  int *cellbody = NULL;

  neut_tess_ver_polys (Tess, nb, &polys, &polyqty);
  cellbody = ut_alloc_1d_int (polyqty);

  for (i = 0; i < polyqty; i++)
    cellbody[i] = Tess.CellBody[polys[i]];

  res = ut_array_1d_int_max (cellbody, polyqty);

  ut_free_1d_int (&polys);
  ut_free_1d_int (&cellbody);

  return res;
}

void
neut_tess_domedge_vers (struct TESS Tess, int dedge, int **pver, int *pverqty)
{
  neut_tess_edges_vers (Tess, Tess.DomTessEdgeNb[dedge] + 1,
                        Tess.DomTessEdgeQty[dedge], pver, pverqty);

  return;
}

void
neut_tess_domface_edges (struct TESS Tess, int dface, int **pedge,
                         int *pedgeqty)
{
  neut_tess_faces_edges (Tess, Tess.DomTessFaceNb[dface] + 1,
                         Tess.DomTessFaceQty[dface], pedge, pedgeqty);

  return;
}

void
neut_tess_domface_body_edges (struct TESS Tess, int dface, int **pedge,
                              int *pedgeqty)
{
  int i, j, dedge, edge;

  if (dface < 1 || dface > Tess.DomFaceQty)
  {
    printf ("domain face id does not fit in range: %d not in [1,%d]\n", dface,
            Tess.DomFaceQty);
    ut_print_neperbug ();
  }

  neut_tess_domface_edges (Tess, dface, pedge, pedgeqty);

  for (i = 1; i <= Tess.DomFaceEdgeQty[dface]; i++)
  {
    dedge = Tess.DomFaceEdgeNb[dface][i];

    for (j = 1; j <= Tess.DomTessEdgeQty[dedge]; j++)
    {
      edge = Tess.DomTessEdgeNb[dedge][j];

      if (ut_array_1d_int_rmelt (*pedge, *pedgeqty, edge, 1) != 1)
        ut_print_neperbug ();
    }

    (*pedgeqty) -= Tess.DomTessEdgeQty[dedge];
  }

  (*pedge) = ut_realloc_1d_int (*pedge, *pedgeqty);

  return;
}

void
neut_tess_polys_faces (struct TESS Tess, int *polys, int polyqty,
                       int **pfaces, int *pfaceqty)
{
  int i, poly;

  (*pfaceqty) = 0;
  ut_free_1d_int (pfaces);

  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    (*pfaces) =
      ut_realloc_1d_int (*pfaces, (*pfaceqty) + Tess.PolyFaceQty[poly]);
    ut_array_1d_int_memcpy (Tess.PolyFaceNb[poly] + 1, Tess.PolyFaceQty[poly],
                            (*pfaces) + (*pfaceqty));
    (*pfaceqty) += Tess.PolyFaceQty[poly];
  }

  ut_array_1d_int_sort_uniq (*pfaces, *pfaceqty, pfaceqty);

  return;
}

void
neut_tess_polys_skinfaces (struct TESS Tess, int *poly, int polyqty,
                           int **pface, int **pfacepoly, int *pfaceqty)
{
  int i, j, k, pos, p, f;

  // allocation
  (*pfaceqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    p = poly[i];
    (*pfaceqty) += Tess.PolyFaceQty[p];
  }
  (*pface) = ut_alloc_1d_int ((*pfaceqty));
  (*pfacepoly) = ut_alloc_1d_int ((*pfaceqty));

  (*pfaceqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    p = poly[i];
    for (j = 1; j <= Tess.PolyFaceQty[p]; j++)
    {
      f = Tess.PolyFaceNb[p][j];
      pos = ut_array_1d_int_eltpos (*pface, *pfaceqty, f);
      if (pos == -1)
      {
        (*pface)[(*pfaceqty)] = f;
        (*pfacepoly)[(*pfaceqty)] = p;
        (*pfaceqty)++;
      }
      else
      {
        for (k = pos; k < (*pfaceqty) - 1; k++)
        {
          (*pface)[k] = (*pface)[k + 1];
          (*pfacepoly)[k] = (*pfacepoly)[k + 1];
        }
        (*pfaceqty)--;
      }
    }
  }
  (*pface) = ut_realloc_1d_int ((*pface), (*pfaceqty));

  return;
}

void
neut_tess_cell_neighcell (struct TESS Tess, int cell, int **pncell,
                          int *pncellqty)
{
  if (Tess.Dim == 3)
    neut_tess_poly_neighpolys (Tess, cell, pncell, pncellqty);

  else if (Tess.Dim == 2)
    neut_tess_face_neighfaces (Tess, cell, pncell, pncellqty);

  else
    abort ();

  return;
}

void
neut_tess_poly_neighpolys_unsort (struct TESS Tess, int poly, int **pnpoly,
                                  int *pnpolyqty)
{
  int i, j, face, *tmp = NULL;

  (*pnpolyqty) = 0;
  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    for (j = 0; j < 2; j++)
      if (Tess.FacePoly[face][j] > 0 && Tess.FacePoly[face][j] != poly)
      {
        tmp = ut_realloc_1d_int (tmp, (*pnpolyqty) + 1);
        tmp[(*pnpolyqty)++] = Tess.FacePoly[face][j];
      }
  }

  if (!strcmp (Tess.Type, "periodic"))
    for (i = 0; i < *pnpolyqty; i++)
      if (tmp[i] > Tess.CellQty)
        tmp[i] = Tess.PerSeedMaster[tmp[i]];

  if (pnpoly)
  {
    (*pnpoly) = ut_realloc_1d_int (*pnpoly, *pnpolyqty);
    ut_array_1d_int_memcpy (tmp, *pnpolyqty, *pnpoly);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_poly_neighpolys (struct TESS Tess, int poly, int **pnpoly,
                           int *pnpolyqty)
{
  neut_tess_poly_neighpolys_unsort (Tess, poly, pnpoly, pnpolyqty);

  if (pnpoly)
    ut_array_1d_int_sort (*pnpoly, *pnpolyqty);

  return;
}

void
neut_tess_poly_neighpolys_samedomain (struct TESS Tess, int poly,
                                     int **pnpolys, int *pnpolyqty)
{
  int i, qty, *tmp = NULL, qty2, *tmp2 = NULL, scale = Tess.ScaleQty - 1;

  neut_tess_poly_neighpolys (Tess, poly, &tmp, &qty);

  qty2 = 0;
  for (i = 0; i < qty; i++)
    if (Tess.ScaleCellId[tmp[i]][scale] == Tess.ScaleCellId[poly][scale])
      ut_array_1d_int_list_addval (&tmp2, &qty2, tmp[i]);

  if (pnpolyqty)
    (*pnpolyqty) = qty2;

  if (pnpolys)
  {
    (*pnpolys) = ut_realloc_1d_int (*pnpolys, qty2);
    ut_array_1d_int_memcpy (tmp2, qty2, *pnpolys);
  }

  ut_free_1d_int (&tmp);
  ut_free_1d_int (&tmp2);

  return;
}

void
neut_tess_poly_neighseeds (struct TESS Tess, int poly, int **pnseeds,
                           int *pnseedqty)
{
  int i, j, face, *tmp = NULL;

  (*pnseedqty) = 0;
  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    for (j = 0; j < 2; j++)
      if (Tess.FacePoly[face][j] != poly)
      {
        tmp = ut_realloc_1d_int (tmp, (*pnseedqty) + 1);
        tmp[(*pnseedqty)++] = Tess.FacePoly[face][j];
      }
  }

  ut_array_1d_int_sort_uniq (tmp, *pnseedqty, pnseedqty);

  if (pnseeds)
  {
    (*pnseeds) = ut_realloc_1d_int (*pnseeds, *pnseedqty);
    ut_array_1d_int_memcpy (tmp, *pnseedqty, *pnseeds);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_face_neighfaces (struct TESS Tess, int face, int **pnface,
                           int *pnfaceqty)
{
  int i, j, edge, *tmp = NULL;

  (*pnfaceqty) = 0;
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];

    for (j = 0; j < Tess.EdgeFaceQty[edge]; j++)
      if (Tess.EdgeFaceNb[edge][j] > 0 && Tess.EdgeFaceNb[edge][j] != face)
      {
        tmp = ut_realloc_1d_int (tmp, (*pnfaceqty) + 1);
        tmp[(*pnfaceqty)++] = Tess.EdgeFaceNb[edge][j];
      }
  }

  if (pnface)
  {
    (*pnface) = ut_realloc_1d_int (*pnface, *pnfaceqty);
    ut_array_1d_int_memcpy (tmp, *pnfaceqty, *pnface);
  }

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_face_neighfaces_samedomain (struct TESS Tess, int face,
                                      int **pnfaces, int *pnfaceqty)
{
  int i, qty, *tmp = NULL, qty2, *tmp2 = NULL, scale = Tess.ScaleQty - 1;

  neut_tess_face_neighfaces (Tess, face, &tmp, &qty);

  qty2 = 0;
  for (i = 0; i < qty; i++)
    if (Tess.ScaleCellId[tmp[i]][scale] == Tess.ScaleCellId[face][scale])
      ut_array_1d_int_list_addval (&tmp2, &qty2, tmp[i]);

  if (pnfaceqty)
    (*pnfaceqty) = qty2;

  if (pnfaces)
  {
    (*pnfaces) = ut_realloc_1d_int (*pnfaces, qty2);
    ut_array_1d_int_memcpy (tmp2, qty2, *pnfaces);
  }

  ut_free_1d_int (&tmp);
  ut_free_1d_int (&tmp2);

  return;
}

void
neut_tess_faces_neighfaces (struct TESS Tess, int *faces, int faceqty,
                            int **pnfaces, int *pnfaceqty)
{
  int i;
  int **tmp = NULL, *qty = NULL;

  tmp = ut_alloc_1d_pint (faceqty);
  qty = ut_alloc_1d_int (faceqty);

  for (i = 0; i < faceqty; i++)
    neut_tess_face_neighfaces (Tess, faces[i], tmp + i, qty + i);

  ut_array_1d_int_lists_merge (tmp, qty, faceqty, pnfaces, pnfaceqty);

  ut_free_1d_int (&qty);
  ut_free_2d_int (&tmp, faceqty);

  return;
}

int
neut_tess_facepair_neigh (struct TESS Tess, int face1, int face2)
{
  int status, qty, *tmp = NULL;

  neut_tess_face_neighfaces (Tess, face1, &tmp, &qty);

  status = (ut_array_1d_int_eltpos (tmp, qty, face2) != -1);

  ut_free_1d_int (&tmp);

  return status;
}

int
neut_tess_edgepair_neigh (struct TESS Tess, int edge1, int edge2)
{
  int status, qty, *tmp = NULL;

  neut_tess_edge_neighedges (Tess, edge1, &tmp, &qty);

  status = (ut_array_1d_int_eltpos (tmp, qty, edge2) != -1);

  ut_free_1d_int (&tmp);

  return status;
}

void
neut_tess_edge_neighedges (struct TESS Tess, int edge, int **pnedge,
                           int *pnedgeqty)
{
  int i, j, ver, *tmp = NULL;

  (*pnedgeqty) = 0;
  for (i = 0; i < 2; i++)
  {
    ver = Tess.EdgeVerNb[edge][i];

    for (j = 0; j < Tess.VerEdgeQty[ver]; j++)
      if (Tess.VerEdgeNb[ver][j] > 0 && Tess.VerEdgeNb[ver][j] != edge)
      {
        tmp = ut_realloc_1d_int (tmp, (*pnedgeqty) + 1);
        tmp[(*pnedgeqty)++] = Tess.VerEdgeNb[ver][j];
      }
  }

  if (pnedge)
  {
    (*pnedge) = ut_realloc_1d_int (*pnedge, *pnedgeqty);
    ut_array_1d_int_memcpy (tmp, *pnedgeqty, *pnedge);
  }

  ut_free_1d_int (&tmp);

  return;
}

int
neut_tess_poly_skinmesh (struct TESS Tess, int nb, struct NODES *pNodes,
                         struct MESH *pMesh)
{
  int i, j, face;
  int *ver = NULL;
  int *verinv = NULL;
  int verinvqty;
  int node0;
  int verqty;
  int *eltnodes = ut_alloc_1d_int (3);
  double *centre = ut_alloc_1d (3);

  neut_mesh_set_zero (pMesh);
  neut_nodes_set_zero (pNodes);
  neut_mesh_addelset (pMesh, NULL, 0);
  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 2;
  (*pMesh).EltType = ut_alloc_1d_char (4);
  sprintf ((*pMesh).EltType, "tri");

  // adding poly vertices and calculating the inverse array
  neut_tess_poly_vers (Tess, nb, &ver, &verqty);
  ut_array_1d_int_inv (ver, verqty, &verinv, &verinvqty);
  ut_array_1d_int_addval (verinv, verinvqty, 1, verinv);

  for (i = 0; i < verqty; i++)
    neut_nodes_addnode (pNodes, Tess.VerCoo[ver[i]], 1);

  // for each face, recording a new node if necessary and recording the
  // elements
  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
  {
    face = Tess.PolyFaceNb[nb][i];

    verqty = Tess.FaceVerQty[face];

    // adding node if defined as the barycenter
    // FacePt == -1 stuff added but not tested.
    if (Tess.FaceState[face] == 0)
    {
      neut_tess_face_centre (Tess, face, Tess.FacePtCoo[face]);
      neut_nodes_addnode (pNodes, Tess.FacePtCoo[face], 1);
    }
    else if (Tess.FacePt[face] == 0)
    {
      neut_nodes_addnode (pNodes, Tess.FacePtCoo[face], 1);
    }
    else if (Tess.FacePt[face] == -1)
    {
      neut_tess_face_centre (Tess, face, centre);
      neut_nodes_addnode (pNodes, centre, 1);
    }

    // looking for the master node (as recorded in Nodes)
    node0 =
      (Tess.FacePt[face] >
       0) ? verinv[Tess.FaceVerNb[face][Tess.FacePt[face]]] : (*pNodes).
      NodeQty;

    for (j = 1; j <= verqty; j++)
    {
      // if p0 is a face vertex:
      // if the ver is p0 or the previous vertex, it is skipped
      if (j == Tess.FacePt[face]
          || ut_array_rotpos (1, verqty, j, 1) == Tess.FacePt[face])
        continue;

      // recording coo of the 1st and 2nd pts
      eltnodes[0] = node0;
      eltnodes[1] = verinv[Tess.FaceVerNb[face][j]];
      eltnodes[2] =
        verinv[Tess.FaceVerNb[face][ut_array_rotpos (1, verqty, j, 1)]];

      neut_mesh_addelt (pMesh, eltnodes);
      neut_mesh_elset_addelt (pMesh, 1, (*pMesh).EltQty);

      /* Probably this kind of test would be useful here
         // if the 3 points are aligned (on an edge of the domain), skipping.
         // use an angle-criterion instead.
         // 01/10: sounds that this never happens...
         if (ut_space_points_aligned (p1, p2, p0, 1e-6))
         continue;
       */
    }
  }

  neut_mesh_init_eltelset (pMesh, NULL);

  ut_free_1d_int (&ver);
  ut_free_1d_int (&verinv);
  ut_free_1d_int (&eltnodes);
  ut_free_1d (&centre);

  return 0;
}

int
neut_tess_poly_mesh (struct TESS Tess, int nb, struct NODES *pNodes,
                     struct MESH *pMesh)
{
  int i;
  struct MESH Mesh2D;
  int *eltnodes = ut_alloc_1d_int (4);

  neut_tess_poly_skinmesh (Tess, nb, pNodes, &Mesh2D);
  neut_nodes_addnode (pNodes, Tess.SeedCoo[nb], 1);

  neut_mesh_set_zero (pMesh);
  neut_mesh_addelset (pMesh, NULL, 0);
  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 3;
  (*pMesh).EltType = ut_alloc_1d_char (4);

  sprintf ((*pMesh).EltType, "tri");
  for (i = 1; i <= Mesh2D.EltQty; i++)
  {
    ut_array_1d_int_memcpy (Mesh2D.EltNodes[i], 3, eltnodes);
    eltnodes[3] = (*pNodes).NodeQty;

    neut_mesh_addelt (pMesh, eltnodes);
    neut_mesh_elset_addelt (pMesh, 1, (*pMesh).EltQty);
  }

  neut_mesh_init_eltelset (pMesh, NULL);

  ut_free_1d_int (&eltnodes);
  neut_mesh_free (&Mesh2D);

  return 0;
}

int
neut_tess_poly_reg (struct TESS Tess, int nb)
{
  int i;

  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
    if (Tess.FaceState[Tess.PolyFaceNb[nb][i]] != 0)
      return 1;

  return 0;
}

int
neut_tess_poly_std (struct TESS Tess, int nb)
{
  return (neut_tess_poly_reg (Tess, nb) == 0) ? 1 : 0;
}

// Returns the interpolation (triangles) of a (regularized or not) face
// under the form of a mesh.
int
neut_tess_face_interpolmesh (struct TESS Tess, int face, struct NODES *pN,
                             struct MESH *pM)
{
  int i, tqty, ver, ver0, verqty;
  double *p0 = NULL, *p1 = NULL, *p2 = NULL;
  int *nodes = ut_alloc_1d_int (3);
  int *elts = NULL;

  p0 = ut_alloc_1d (3);

  neut_nodes_reset (pN);
  neut_mesh_reset (pM);
  (*pM).Dimension = 2;
  (*pM).EltOrder = 1;
  ut_string_string ("tri", &(*pM).EltType);

  verqty = Tess.FaceVerQty[face];
  for (i = 1; i <= verqty; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    neut_nodes_addnode (pN, Tess.VerCoo[ver], 1);
  }

  if (Tess.FacePt[face] == 0)
  {
    if (Tess.FaceState[face] == 1)
      neut_nodes_addnode (pN, Tess.FacePtCoo[face], 1);
    else
    {
      neut_tess_face_centre (Tess, face, p0);
      neut_nodes_addnode (pN, p0, 1);
    }
    ver0 = verqty + 1;
  }
  else if (Tess.FacePt[face] > 0)
    ver0 = Tess.FacePt[face];
  else
  {
    neut_tess_face_centre (Tess, face, p0);
    neut_nodes_addnode (pN, p0, 1);
    ver0 = verqty + 1;
  }

  ut_array_1d_memcpy (Tess.VerCoo[ver0], 3, p0);

  tqty = 0;
  for (i = 1; i <= verqty; i++)
  {
    // if p0 is a face vertex:
    // if the ver is p0 or the previous vertex, it is skipped
    if (i == Tess.FacePt[face]
        || ut_array_rotpos (1, verqty, i, 1) == Tess.FacePt[face])
      continue;

    // recording coo of the 1st and 2nd pts
    nodes[0] = ver0;
    nodes[1] = i;
    nodes[2] = ut_array_rotpos (1, verqty, i, 1);
    p1 = Tess.VerCoo[nodes[1]];
    p2 = Tess.VerCoo[nodes[2]];

    neut_mesh_addelt (pM, nodes);

    // if the 3 points are aligned (on an edge of the domain), skipping.
    // use an angle-criterion instead.
    // 01/10: sounds that this never happens...
    if (ut_space_points_aligned (p1, p2, p0, 1e-6))
      continue;

    tqty++;
  }
  elts = ut_alloc_1d_int ((*pM).EltQty);
  for (i = 0; i < (*pM).EltQty; i++)
    elts[i] = i + 1;
  neut_mesh_addelset (pM, elts, (*pM).EltQty);

  ut_free_1d_int (&elts);
  ut_free_1d_int (&nodes);
  ut_free_1d (&p0);

  return tqty;
}

int
neut_tess_domfaces_comdomver (struct TESS Tess, int *domfaces, int domfaceqty,
                              int *pver)
{
  int i, status, domface;
  int *tmp = NULL, qty;

  if (domfaceqty <= 0)
  {
    (*pver) = -1;
    return -1;
  }

  domface = domfaces[0];

  tmp = ut_alloc_1d_int (Tess.DomFaceVerQty[domface]);
  qty = Tess.DomFaceVerQty[domface];
  ut_array_1d_int_memcpy (Tess.DomFaceVerNb[domface] + 1,
                          Tess.DomFaceVerQty[domface], tmp);

  for (i = 1; i < domfaceqty && qty > 0; i++)
  {
    domface = domfaces[i];
    ut_array_1d_int_inter (tmp, qty, Tess.DomFaceVerNb[domface] + 1,
                           Tess.DomFaceVerQty[domface], tmp, &qty);
  }

  ut_array_1d_int_list_rmval (&tmp, &qty, 0);

  if (qty == 1)
  {
    (*pver) = tmp[0];
    status = 0;
  }
  else
  {
    (*pver) = -1;
    status = -1;
  }

  ut_free_1d_int (&tmp);

  return status;
}

int
neut_tess_domfaces_comdomedge (struct TESS Tess, int *dface, int dfaceqty,
                               int *pdedge)
{
  int i, status, qty;
  int *nb = NULL;

  if (dfaceqty <= 0)
  {
    (*pdedge) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Tess.DomFaceEdgeQty[dface[0]]);
  qty = Tess.DomFaceEdgeQty[dface[0]];
  ut_array_1d_int_memcpy (Tess.DomFaceEdgeNb[dface[0]] + 1,
                          Tess.DomFaceEdgeQty[dface[0]], nb);

  for (i = 1; i < dfaceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Tess.DomFaceEdgeNb[dface[i]] + 1,
                           Tess.DomFaceEdgeQty[dface[i]], nb, &qty);

  if (qty == 1)
  {
    (*pdedge) = nb[0];
    status = 0;
  }
  else
  {
    (*pdedge) = -1;
    status = -1;
  }

  ut_free_1d_int (&nb);

  return status;
}

void
neut_tess_domvers_ver (struct TESS Tess, int *dver, int dverqty, int **pver,
                       int *pverqty)
{
  int i;

  ut_free_1d_int (pver);
  (*pverqty) = 0;

  (*pver) = ut_alloc_1d_int (dverqty);
  (*pverqty) = dverqty;

  for (i = 0; i < dverqty; i++)
    (*pver)[i] = Tess.DomTessVerNb[dver[i]];

  return;
}

int
neut_tess_domvers_comdomedge (struct TESS Tess, int *dver, int dverqty,
                              int *pdedge)
{
  int status, i, qty;
  int *id = NULL;

  // no vers: return
  if (dverqty <= 0)
  {
    (*pdedge) = -1;
    return -1;
  }

  qty = Tess.DomVerEdgeQty[dver[0]];

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (Tess.DomVerEdgeNb[dver[0]], qty, id);

  for (i = 1; i < dverqty; i++)
    ut_array_1d_int_inter (id, qty, Tess.DomVerEdgeNb[dver[i]],
                           Tess.DomVerEdgeQty[dver[i]], id, &qty);

  if (qty == 1)
  {
    (*pdedge) = id[0];
    status = 0;
  }
  else
  {
    (*pdedge) = -1;
    status = -1;
  }

  ut_free_1d_int (&id);

  return status;
}

int
neut_tess_domedges_comdomver (struct TESS Tess, int *dedge, int dedgeqty,
                              int *pdver)
{
  int status, i, qty;
  int *id = NULL;

  // no edges: return
  if (dedgeqty <= 0)
  {
    (*pdver) = -1;
    return -1;
  }

  qty = 2;

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (Tess.DomEdgeVerNb[dedge[0]], qty, id);

  for (i = 1; i < dedgeqty; i++)
    ut_array_1d_int_inter (id, qty, Tess.DomEdgeVerNb[dedge[i]], 2, id, &qty);

  if (qty == 1)
  {
    (*pdver) = id[0];
    status = 0;
  }
  else
  {
    (*pdver) = -1;
    status = -1;
  }

  ut_free_1d_int (&id);

  return status;
}

int
neut_tess_domedges_domvers (struct TESS Tess, int *domedges, int domedgeqty,
                            int **pdver, int *pdverqty)
{
  int i, domedge;

  // no edges: return
  if (domedgeqty <= 0)
  {
    (*pdver) = NULL;
    (*pdverqty) = 0;
    return -1;
  }

  (*pdver) = ut_alloc_1d_int (domedgeqty * 2);
  (*pdverqty) = 0;
  for (i = 0; i < domedgeqty; i++)
  {
    domedge = domedges[i];
    ut_array_1d_int_memcpy (Tess.DomEdgeVerNb[domedge],
                            Tess.DomEdgeVerQty[domedge], *pdver + *pdverqty);
    (*pdverqty) += Tess.DomEdgeVerQty[domedge];
  }

  ut_array_1d_int_sort_uniq (*pdver, *pdverqty, pdverqty);

  return 0;
}

int
neut_tess_domedges_comdomface (struct TESS Tess, int *dedge, int dedgeqty,
                               int *pdface)
{
  int status, i, qty;
  int *id = NULL;

  // no edges: return
  if (dedgeqty <= 0)
  {
    (*pdface) = -1;
    return -1;
  }

  qty = 2;

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (Tess.DomEdgeFaceNb[dedge[0]], qty, id);

  for (i = 1; i < dedgeqty; i++)
    ut_array_1d_int_inter (id, qty, Tess.DomEdgeFaceNb[dedge[i]], 2, id,
                           &qty);

  if (qty == 1)
  {
    (*pdface) = id[0];
    status = 0;
  }
  else
  {
    (*pdface) = -1;
    status = -1;
  }

  ut_free_1d_int (&id);

  return status;
}

void
neut_tess_domver_domface (struct TESS Tess, int dver, int **pdface,
                          int *pdfaceqty)
{
  int i, j, tmpedge;
  int qty = Tess.Dim - 1;

  (*pdfaceqty) = 0;
  for (i = 0; i < Tess.DomVerEdgeQty[dver]; i++)
  {
    tmpedge = Tess.DomVerEdgeNb[dver][i];
    (*pdface) = ut_realloc_1d_int ((*pdface), (*pdfaceqty) + qty);
    for (j = 0; j < qty; j++)
      (*pdface)[(*pdfaceqty)++] = Tess.DomEdgeFaceNb[tmpedge][j];
  }

  ut_array_1d_int_sort_uniq (*pdface, *pdfaceqty, pdfaceqty);

  return;
}

void
neut_tess_domedge_edges_sorted (struct TESS Tess, int dedge, int **pedge,
                                int **pedgeori, int *pedgeqty)
{
  int i, j, domver, firstver, ver, edge, status, pos;

  (*pedgeqty) = 0;
  ut_free_1d_int (pedge);

  // recording first ver
  domver = Tess.DomEdgeVerNb[dedge][0];
  firstver = Tess.DomTessVerNb[domver];

  // recording edges -----------
  (*pedgeqty) = Tess.DomTessEdgeQty[dedge];
  (*pedge) = ut_alloc_1d_int (*pedgeqty);
  if (pedgeori)
    (*pedgeori) = ut_alloc_1d_int (*pedgeqty);

  ver = firstver;
  for (i = 0; i < *pedgeqty; i++)
  {
    // searching edge with ver, that belongs to domedge and
    // different from prev edge (if existing)
    status = 0;
    for (j = 0; j < Tess.VerEdgeQty[ver]; j++)
    {
      edge = Tess.VerEdgeNb[ver][j];
      if (Tess.EdgeDom[edge][0] == 1 && Tess.EdgeDom[edge][1] == dedge)
        if (i == 0 || edge != (*pedge)[i - 1])
        {
          // recording edge
          (*pedge)[i] = edge;

          // recording orientation
          pos = ut_array_1d_int_eltpos (Tess.EdgeVerNb[edge], 2, ver);
          if (pos == -1)
            ut_print_neperbug ();

          if (pedgeori)
            (*pedgeori)[i] = (pos == 0) ? 1 : -1;
          status = 1;

          // recording new ver
          ver = Tess.EdgeVerNb[edge][!pos];
          break;
        }
    }

    if (status != 1)
      abort ();
  }

  return;
}

void
neut_tess_domedge_ver (struct TESS Tess, int dedge, int **pver, int *pverqty)
{
  int i, edge;

  (*pver) = ut_alloc_1d_int (Tess.DomTessEdgeQty[dedge] * 2);

  (*pverqty) = 0;
  for (i = 1; i <= Tess.DomTessEdgeQty[dedge]; i++)
  {
    edge = Tess.DomTessEdgeNb[dedge][i];

    ut_array_1d_int_memcpy (Tess.EdgeVerNb[edge], 2, (*pver) + (*pverqty));

    (*pverqty) += 2;
  }

  ut_array_1d_int_sort_uniq ((*pver), (*pverqty), pverqty);

  return;
}

void
neut_tess_domedge_body_ver (struct TESS Tess, int dedge, int **pver,
                            int *pverqty)
{
  int i, domver;

  neut_tess_domedge_ver (Tess, dedge, pver, pverqty);

  for (i = 0; i < 2; i++)
  {
    domver = Tess.DomEdgeVerNb[dedge][i];
    if (ut_array_1d_int_rmelt (*pver, *pverqty, Tess.DomTessVerNb[domver], 1)
        != 1)
      ut_print_neperbug ();
  }

  *pverqty -= 2;
  (*pver) = ut_realloc_1d_int (*pver, *pverqty);

  return;
}

void
neut_tess_domedges_body_ver (struct TESS Tess, int *dedge, int dedgeqty,
                             int **pver, int *pverqty)
{
  int i, qty;
  int *tmp = NULL;
  int *dedge2 = NULL;
  int dedgeqty2;

  ut_free_1d_int (pver);
  (*pverqty) = 0;

  dedgeqty2 = 0;
  if (dedgeqty >= 0)
  {
    dedge2 = ut_alloc_1d_int (dedgeqty);
    ut_array_1d_int_memcpy (dedge, dedgeqty, dedge2);
    dedgeqty2 = dedgeqty;
  }

  // special case where all domain edges are used
  else if (dedgeqty == -1)
  {
    dedge2 = ut_alloc_1d_int (Tess.DomEdgeQty);
    for (i = 0; i < Tess.DomEdgeQty; i++)
      dedge2[i] = i + 1;
    dedgeqty2 = Tess.DomEdgeQty;
  }

  for (i = 0; i < dedgeqty2; i++)
  {
    neut_tess_domedge_body_ver (Tess, dedge2[i], &tmp, &qty);
    (*pver) = ut_realloc_1d_int (*pver, *pverqty + qty);
    ut_array_1d_int_memcpy (tmp, qty, *pver + *pverqty);
    (*pverqty) += qty;
  }

  return;
}

void
neut_tess_domedges_ver (struct TESS Tess, int *dedge, int dedgeqty,
                        int **pver, int *pverqty)
{
  int i;
  int *tmp = NULL;
  int qty;

  ut_free_1d_int (pver);
  (*pverqty) = 0;

  for (i = 0; i < dedgeqty; i++)
  {
    neut_tess_domedge_ver (Tess, dedge[i], &tmp, &qty);
    (*pver) = ut_realloc_1d_int (*pver, *pverqty + qty);
    ut_array_1d_int_memcpy (tmp, qty, *pver + *pverqty);
    (*pverqty) += qty;
    ut_free_1d_int (&tmp);
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

void
neut_tess_domface_vers (struct TESS Tess, int dface, int **pver, int *pverqty)
{
  int i, j, face;

  (*pverqty) = 0;
  for (i = 1; i <= Tess.DomTessFaceQty[dface]; i++)
  {
    face = Tess.DomTessFaceNb[dface][i];

    (*pver) = ut_realloc_1d_int ((*pver), (*pverqty) + Tess.FaceVerQty[face]);

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      (*pver)[(*pverqty)++] = Tess.FaceVerNb[face][j];
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

void
neut_tess_domfaces_vers (struct TESS Tess, int *domfaces, int domfaceqty,
                         int **pvers, int *pverqty)
{
  int i;
  int **domfacevers = NULL, *domfaceverqty = NULL;

  domfacevers = ut_alloc_1d_pint (domfaceqty);
  domfaceverqty = ut_alloc_1d_int (domfaceqty);

  for (i = 0; i < domfaceqty; i++)
    neut_tess_domface_vers (Tess, domfaces[i], domfacevers + i,
                            domfaceverqty + i);

  ut_array_1d_int_lists_merge (domfacevers, domfaceverqty, domfaceqty, pvers,
                               pverqty);

  ut_free_2d_int (&domfacevers, domfaceqty);
  ut_free_1d_int (&domfaceverqty);

  return;
}

void
neut_tess_domface_body_ver (struct TESS Tess, int dface, int **pver,
                            int *pverqty)
{
  int i, boundverqty;
  int *boundver = NULL;

  neut_tess_domface_vers (Tess, dface, pver, pverqty);

  neut_tess_domedges_ver (Tess, Tess.DomFaceEdgeNb[dface] + 1,
                          Tess.DomFaceEdgeQty[dface], &boundver,
                          &boundverqty);

  for (i = 0; i < boundverqty; i++)
    if (ut_array_1d_int_rmelt (*pver, *pverqty, boundver[i], 1) != 1)
      ut_print_neperbug ();

  *pverqty -= boundverqty;
  (*pver) = ut_realloc_1d_int (*pver, *pverqty);

  ut_free_1d_int (&boundver);

  return;
}

void
neut_tess_domfaces_body_ver (struct TESS Tess, int *dface, int dfaceqty,
                             int **pver, int *pverqty)
{
  int i, qty;
  int *tmp = NULL;
  int *dface2 = NULL;
  int dfaceqty2;

  ut_free_1d_int (pver);
  (*pverqty) = 0;

  dfaceqty2 = 0;
  if (dfaceqty >= 0)
  {
    dface2 = ut_alloc_1d_int (dfaceqty);
    ut_array_1d_int_memcpy (dface, dfaceqty, dface2);
    dfaceqty2 = dfaceqty;
  }

  // special case where all domain faces are used
  else if (dfaceqty == -1)
  {
    dface2 = ut_alloc_1d_int (Tess.DomFaceQty);
    for (i = 0; i < Tess.DomFaceQty; i++)
      dface2[i] = i + 1;
    dfaceqty2 = Tess.DomFaceQty;
  }

  for (i = 0; i < dfaceqty2; i++)
  {
    neut_tess_domface_body_ver (Tess, dface2[i], &tmp, &qty);

    (*pver) = ut_realloc_1d_int (*pver, *pverqty + qty);
    ut_array_1d_int_memcpy (tmp, qty, *pver + *pverqty);
    (*pverqty) += qty;
  }

  return;
}

void
neut_tess_faces_vers (struct TESS Tess, int *face, int faceqty, int **pver,
                      int *pverqty)
{
  int i, id;

  if (faceqty < 1 || ut_array_1d_int_max (face, faceqty) > Tess.FaceQty)
    ut_print_neperbug ();

  (*pverqty) = 0;
  ut_free_1d_int (pver);

  for (i = 0; i < faceqty; i++)
  {
    id = face[i];
    (*pver) = ut_realloc_1d_int ((*pver), (*pverqty) + Tess.FaceVerQty[id]);
    ut_array_1d_int_memcpy (Tess.FaceVerNb[id] + 1, Tess.FaceVerQty[id],
                            (*pver) + (*pverqty));

    (*pverqty) += Tess.FaceVerQty[id];
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

void
neut_tess_faces_edges (struct TESS Tess, int *face, int faceqty, int **pedge,
                       int *pedgeqty)
{
  int i, id;

  (*pedgeqty) = 0;
  ut_free_1d_int (pedge);

  for (i = 0; i < faceqty; i++)
  {
    id = face[i];
    (*pedge) =
      ut_realloc_1d_int ((*pedge), (*pedgeqty) + Tess.FaceVerQty[id]);
    ut_array_1d_int_memcpy (Tess.FaceEdgeNb[id] + 1, Tess.FaceVerQty[id],
                            (*pedge) + (*pedgeqty));

    (*pedgeqty) += Tess.FaceVerQty[id];
  }

  ut_array_1d_int_sort_uniq (*pedge, *pedgeqty, pedgeqty);

  return;
}

int
neut_tess_edge_fake (struct TESS Tess, int edge)
{
  int domedge, face, domface1, domface2, poly1, poly2, j, status;

  status = 0;

  if (!strcmp (Tess.DomType, "cylinder"))
  {
    if (Tess.EdgeDom[edge][0] == 1)
    {
      domedge = Tess.EdgeDom[edge][1];
      domface1 = Tess.DomEdgeFaceNb[domedge][0];
      domface2 = Tess.DomEdgeFaceNb[domedge][1];

      poly1 = -1;
      poly2 = -1;
      for (j = 0; j < Tess.EdgeFaceQty[edge]; j++)
      {
        face = Tess.EdgeFaceNb[edge][j];
        if (Tess.FaceDom[face][0] == 2)
        {
          if (Tess.FaceDom[face][1] == domface1)
            poly1 = Tess.FacePoly[face][0];
          if (Tess.FaceDom[face][1] == domface2)
            poly2 = Tess.FacePoly[face][0];
        }
      }

      if (poly1 < 0 || poly2 < 0)
        ut_print_neperbug ();

      if (domface1 > 2 && domface2 > 2 && poly1 == poly2)
        status = 1;
    }
  }

  return status;
}

int
neut_tess_domface_label_id (struct TESS Tess, char *label, int *pid)
{
  int i;

  (*pid) = -1;
  for (i = 1; i <= Tess.DomFaceQty; i++)
    if (!strcmp (Tess.DomFaceLabel[i], label))
    {
      (*pid) = i;
      break;
    }

  return (*pid > 0) ? 0 : -1;
}

void
neut_tess_cell_vers (struct TESS Tess, int cell, int **pvers, int *pverqty)
{
  if (Tess.Dim == 2)
  {
    (*pverqty) = Tess.FaceVerQty[cell];
    (*pvers) = ut_alloc_1d_int (*pverqty);
    ut_array_1d_int_memcpy (Tess.FaceVerNb[cell] + 1, *pverqty, *pvers);
  }

  else if (Tess.Dim == 3)
    neut_tess_poly_vers (Tess, cell, pvers, pverqty);

  else
    ut_print_neperbug ();

  return;
}

int
neut_tess_entity_true (struct TESS Tess, char *entity, int id)
{
  if (!strcmp (entity, "cell") || !strcmp (entity, "poly"))
    return neut_tess_cell_true (Tess, id);
  else if (!strcmp (entity, "face"))
    return neut_tess_face_true (Tess, id);
  else if (!strcmp (entity, "edge"))
    return neut_tess_edge_true (Tess, id);
  else if (!strcmp (entity, "ver"))
    return neut_tess_ver_true (Tess, id);
  else
    abort ();
}

int
neut_tess_entity_body (struct TESS Tess, char *entity, int id)
{
  if (!strcmp (entity, "cell") || !strcmp (entity, "poly"))
    return neut_tess_cell_body (Tess, id);
  else if (!strcmp (entity, "face"))
    return neut_tess_face_body (Tess, id);
  else if (!strcmp (entity, "edge"))
    return neut_tess_edge_body (Tess, id);
  else if (!strcmp (entity, "ver"))
    return neut_tess_ver_body (Tess, id);
  else
    abort ();
}

int
neut_tess_seed_master_slave (struct TESS Tess, int master, int *pos, int fact)
{
  int id =
    (fact * pos[2] + 1) * 9 + (fact * pos[1] + 1) * 3 + (fact * pos[0] + 1);

  if (abs (fact) != 1)
    abort ();

  return Tess.PerSeedSlave[master][id];
}

int
neut_tess_seed_perseed (struct TESS Tess, int seed, int *shift)
{
  int seed2;

  if (seed < 0)
    return seed;
  else if (seed <= Tess.CellQty)
    return neut_tess_seed_master_slave (Tess, seed, shift, 1);
  else
  {
    if (ut_array_1d_int_opposite (shift, Tess.PerSeedShift[seed], 3))
      return Tess.PerSeedMaster[seed];
    else
    {
      int *shift2 = ut_alloc_1d_int (3);
      ut_array_1d_int_add (shift, Tess.PerSeedShift[seed], 3, shift2);
      seed2 = neut_tess_seed_perseed (Tess, Tess.PerSeedMaster[seed], shift2);
      ut_free_1d_int (&shift2);

      return seed2;
    }
  }
}

int
neut_tess_face_perface (struct TESS Tess, int face, int *shift, int *pperface)
{
  int status, slave;
  int *shiftb = ut_alloc_1d_int (3);

  status = -1;
  if (Tess.PerFaceMaster[face] == 0)
  {
    slave = Tess.PerFaceSlaveNb[face];
    ut_array_1d_int_memcpy (Tess.PerFaceShift[slave], 3, shiftb);
    ut_array_1d_int_scale (shiftb, 3, -1);

    if (ut_array_1d_int_equal (shift, 3, shiftb, 3))
    {
      (*pperface) = slave;
      status = 0;
    }
  }
  else
  {
    (*pperface) = Tess.PerFaceMaster[face];
    if (ut_array_1d_int_equal (shift, 3, Tess.PerFaceShift[face], 3))
      status = 0;
  }

  ut_free_1d_int (&shiftb);

  return status;
}

void
neut_tess_edge_perfaces (struct TESS Tess, int edge, int **pfaces,
                         int *pfaceqty)
{
  int i, face;

  (*pfaces) = ut_alloc_1d_int (Tess.EdgeFaceQty[edge]);

  (*pfaceqty) = 0;
  for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
  {
    face = Tess.EdgeFaceNb[edge][i];

    if (ut_array_1d_int_max (Tess.FacePoly[face], 2) > Tess.CellQty)
      (*pfaces)[(*pfaceqty)++] = face;
  }

  ut_array_1d_int_sort (*pfaces, *pfaceqty);
  (*pfaces) = ut_realloc_1d_int ((*pfaces), (*pfaceqty));

  return;
}

void
neut_tess_perfaces_perseeds (struct TESS Tess, int *faces, int faceqty,
                             int **pseeds, int *pseedqty)
{
  int i, face, seed;

  (*pseeds) = ut_alloc_1d_int (faceqty);

  (*pseedqty) = 0;
  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    seed = ut_array_1d_int_max (Tess.FacePoly[face], 2);
    if (seed > Tess.CellQty)
      (*pseeds)[(*pseedqty)++] = seed;
  }

  ut_array_1d_int_sort_uniq (*pseeds, *pseedqty, pseedqty);
  (*pseeds) = ut_realloc_1d_int ((*pseeds), (*pseedqty));

  return;
}

int
neut_tess_seeds_edge (struct TESS Tess, int *seeds, int seedqty, int *pedge)
{
  int i, j, face, status;
  int *seedpair = NULL;
  int *array1 = NULL, *array2 = NULL, qty1, qty2;

  if (seedqty < 3 || ut_array_1d_int_eltpos (seeds, seedqty, 0) != -1)
  {
    (*pedge) = -1;
    printf ("bad seeds.\n");
    ut_print_neperbug ();
    return -1;
  }

  status = 0;
  seedpair = ut_alloc_1d_int (2);
  qty1 = 0;
  for (i = 0; i < seedqty; i++)
  {
    seedpair[0] = seeds[i];

    for (j = i + 1; j < seedqty; j++)
    {
      seedpair[1] = seeds[j];

      if (ut_array_1d_int_min (seedpair, 2) > Tess.CellQty)
        continue;
      else if (ut_array_1d_int_min (seedpair, 2) < 0
               && ut_array_1d_int_max (seedpair, 2) > Tess.CellQty)
        continue;

      status = neut_tess_seeds_comface (Tess, seedpair, 2, &face);

      if (status != 0)
        ut_print_neperbug ();

      if (!array1)
      {
        qty1 = Tess.FaceVerQty[face];
        array1 = ut_alloc_1d_int (qty1);
        ut_array_1d_int_memcpy (Tess.FaceEdgeNb[face] + 1, qty1, array1);
      }

      else
      {
        array2 = ut_realloc_1d_int (array2, qty1);
        ut_array_1d_int_inter (array1, qty1, Tess.FaceEdgeNb[face] + 1,
                               Tess.FaceVerQty[face], array2, &qty2);
        ut_array_1d_int_memcpy (array2, qty2, array1);
        qty1 = qty2;
      }
    }

    if (status != 0)
      break;
  }

  if (status == 0 && qty1 != 1)
    status = -1;
  else
    (*pedge) = array1[0];

  ut_free_1d_int (&seedpair);
  ut_free_1d_int (&array1);
  ut_free_1d_int (&array2);

  return status;
}

int
neut_tess_seeds_ver (struct TESS Tess, int *seeds, int seedqty, int *pver)
{
  int i, j, face, status = 0;
  int *seedpair = NULL;
  int *array1 = NULL, *array2 = NULL, qty1, qty2;

  if (seedqty < 4 || ut_array_1d_int_eltpos (seeds, seedqty, 0) != -1)
  {
    (*pver) = -1;
    printf ("bad seeds.\n");
    ut_print_neperbug ();
    return -1;
  }

  seedpair = ut_alloc_1d_int (2);
  qty1 = 0;
  for (i = 0; i < seedqty; i++)
  {
    seedpair[0] = seeds[i];

    for (j = i + 1; j < seedqty; j++)
    {
      seedpair[1] = seeds[j];

      if (ut_array_1d_int_min (seedpair, 2) > Tess.CellQty)
        continue;

      else if (ut_array_1d_int_min (seedpair, 2) < 0
               && ut_array_1d_int_max (seedpair, 2) > Tess.CellQty)
        continue;

      else if (ut_array_1d_int_min (seedpair, 2) < 0
               && ut_array_1d_int_max (seedpair, 2) < 0)
        continue;

      status = neut_tess_seeds_comface (Tess, seedpair, 2, &face);

      if (status != 0)
        ut_print_neperbug ();

      if (!array1)
      {
        qty1 = Tess.FaceVerQty[face];
        array1 = ut_alloc_1d_int (qty1);
        ut_array_1d_int_memcpy (Tess.FaceVerNb[face] + 1, qty1, array1);
      }

      else
      {
        array2 = ut_realloc_1d_int (array2, qty1);
        ut_array_1d_int_inter (array1, qty1, Tess.FaceVerNb[face] + 1,
                               Tess.FaceVerQty[face], array2, &qty2);
        ut_array_1d_int_memcpy (array2, qty2, array1);
        qty1 = qty2;
      }
    }

    if (status != 0)
      break;
  }

  if (status == 0 && qty1 != 1)
    status = -1;
  else
    (*pver) = array1[0];

  ut_free_1d_int (&seedpair);
  ut_free_1d_int (&array1);
  ut_free_1d_int (&array2);

  return status;
}

int
neut_tess_seeds_comface (struct TESS Tess, int *seeds, int seedqty,
                         int *pface)
{
  // seed1 has a poly, seed2 is the other (has-a-poly or negative or periodic)
  int i, seed1, seed2, face;

  (*pface) = -1;

  if (seedqty != 2)
    return -1;

  else if (ut_array_1d_int_min (seeds, 2) > Tess.CellQty)
  {
    (*pface) = -1;
    return -1;
  }

  else if (ut_array_1d_int_min (seeds, 2) < 0
           && ut_array_1d_int_max (seeds, 2) > Tess.CellQty)
  {
    (*pface) = -1;
    return -1;
  }

  else
  {
    if (seeds[0] >= 1 && seeds[0] <= Tess.PolyQty)
    {
      seed1 = seeds[0];
      seed2 = seeds[1];
    }
    else
    {
      seed1 = seeds[1];
      seed2 = seeds[0];
    }

    (*pface) = -1;
    for (i = 1; i <= Tess.PolyFaceQty[seed1]; i++)
    {
      face = Tess.PolyFaceNb[seed1][i];
      if (face > 0
          && ut_array_1d_int_eltpos (Tess.FacePoly[face], 2, seed2) != -1)
      {
        (*pface) = face;
        break;
      }
    }
  }

  return (*pface) > 0 ? 0 : -1;
}

void
neut_tess_cells_neigh (struct TESS Tess, int *cells, int cellqty, int *pneigh)
{
  int i, qty, *tmp = NULL, qty2;

  (*pneigh) = 1;
  for (i = 0; i < cellqty; i++)
    if (Tess.PolyFaceQty[cells[i]] > 0)
    {
      neut_tess_cell_neighcell (Tess, cells[i], &tmp, &qty);

      ut_array_1d_int_inter (cells, cellqty, tmp, qty, NULL, &qty2);
      if (qty2 == 0)
      {
        (*pneigh) = -cells[i];
        break;
      }

      ut_free_1d_int (&tmp);
    }

  return;
}

void
neut_tess_domface_polys (struct TESS Tess, int domface, int **ppolys,
                         int *ppolyqty)
{
  neut_tess_faces_polys (Tess, Tess.DomTessFaceNb[domface] + 1,
                         Tess.DomTessFaceQty[domface], ppolys, ppolyqty);

  return;
}

void
neut_tess_domedge_faces (struct TESS Tess, int domedge, int **pfaces,
                         int *pfaceqty)
{
  neut_tess_edges_faces (Tess, Tess.DomTessEdgeNb[domedge] + 1,
                         Tess.DomTessEdgeQty[domedge], pfaces, pfaceqty);

  return;
}

void
neut_tess_face_domedge_edges (struct TESS Tess, int face, int domedge,
                              int **pedges, int *pedgeqty)
{
  int i, edge;

  (*pedgeqty) = 0;
  ut_free_1d_int (pedges);

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];

    if (Tess.EdgeDom[edge][0] == 1 && Tess.EdgeDom[edge][1] == domedge)
      ut_array_1d_int_list_addval (pedges, pedgeqty, edge);
  }

  return;
}

void
neut_tess_poly_domface_faces (struct TESS Tess, int poly, int domface,
                              int **pfaces, int *pfaceqty)
{
  int i, face;

  (*pfaceqty) = 0;
  ut_free_1d_int (pfaces);

  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    if (Tess.FaceDom[face][0] == 2 && Tess.FaceDom[face][1] == domface)
      ut_array_1d_int_list_addval (pfaces, pfaceqty, face);
  }

  return;
}

void
neut_tess_faces_contiguousfaces (struct TESS Tess, double coplanar,
                                 int *faces, int faceqty, int *pqty,
                                 int ***pfaces, int **pfaceqty)
{
  int i, j, face, id;
  int qty, *tmp = NULL;
  int interqty, *inter = ut_alloc_1d_int (faceqty);
  int qty2, *tmp2 = NULL;
  int *facedom = ut_alloc_1d_int (2);

  qty = faceqty;
  tmp = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (faces, qty, tmp);

  (*pqty) = 0;

  while (qty > 0)
  {
    // adding new set
    id = (*pqty)++;
    (*pfaces) = ut_realloc_1d_pint (*pfaces, *pqty);
    (*pfaces)[id] = NULL;
    (*pfaceqty) = ut_realloc_1d_int (*pfaceqty, *pqty);
    (*pfaceqty)[id] = 0;
    ut_array_1d_int_memcpy (Tess.FaceDom[tmp[0]], 2, facedom);

    // adding face to set
    ut_array_1d_int_list_addval ((*pfaces) + id, (*pfaceqty) + id, tmp[0]);
    ut_array_1d_int_list_rmval (&tmp, &qty, tmp[0]);

    // adding neighbors of the faces in the set
    for (i = 0; i < (*pfaceqty)[id]; i++)
    {
      face = (*pfaces)[id][i];

      neut_tess_face_neighfaces (Tess, face, &tmp2, &qty2);
      ut_array_1d_int_inter (tmp2, qty2, tmp, qty, inter, &interqty);

      for (j = 0; j < interqty; j++)
      {
        double scalprod;
        scalprod =
          ut_vector_scalprod (Tess.FaceEq[(*pfaces)[id][0]] + 1,
                              Tess.FaceEq[inter[j]] + 1);

        if (ut_array_1d_int_equal (Tess.FaceDom[face], 2, facedom, 2)
            && (scalprod >= coplanar - 1e-6
                || neut_tess_face_iscurved (Tess, face)))
        {
          ut_array_1d_int_list_addval ((*pfaces) + id, (*pfaceqty) + id,
                                       inter[j]);
          ut_array_1d_int_list_rmval (&tmp, &qty, inter[j]);
        }
      }
    }
  }

  ut_free_1d_int (&tmp);
  ut_free_1d_int (&tmp2);
  ut_free_1d_int (&inter);
  ut_free_1d_int (&facedom);

  return;
}

void
neut_tess_edges_contiguousedges (struct TESS Tess, double colinear,
                                 int *edges, int edgeqty, int *pqty,
                                 int ***pedges, int **pedgeqty)
{
  int i, j, edge, id;
  int qty, *tmp = NULL;
  int interqty, *inter = ut_alloc_1d_int (edgeqty);
  int qty2, *tmp2 = NULL;
  int *edgedom = ut_alloc_1d_int (2);

  qty = edgeqty;
  tmp = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (edges, qty, tmp);

  (*pqty) = 0;
  while (qty > 0)
  {
    // adding new set
    id = (*pqty)++;
    (*pedges) = ut_realloc_1d_pint (*pedges, *pqty);
    (*pedges)[id] = NULL;
    (*pedgeqty) = ut_realloc_1d_int (*pedgeqty, *pqty);
    (*pedgeqty)[id] = 0;
    if (Tess.EdgeDom)
      ut_array_1d_int_memcpy (Tess.EdgeDom[tmp[0]], 2, edgedom);
    else
      ut_array_1d_int_set (edgedom, 2, 0);

    // adding edge to set
    ut_array_1d_int_list_addval ((*pedges) + id, (*pedgeqty) + id, tmp[0]);
    ut_array_1d_int_list_rmval (&tmp, &qty, tmp[0]);

    // adding neighbors of the edges in the set
    for (i = 0; i < (*pedgeqty)[id]; i++)
    {
      edge = (*pedges)[id][i];

      neut_tess_edge_neighedges (Tess, edge, &tmp2, &qty2);
      ut_array_1d_int_inter (tmp2, qty2, tmp, qty, inter, &interqty);

      for (j = 0; j < interqty; j++)
      {
        double scalprod;
        double *v1 = ut_alloc_1d (3);
        double *v2 = ut_alloc_1d (3);

        neut_tess_edge_dir (Tess, (*pedges)[id][0], v1);
        neut_tess_edge_dir (Tess, edge, v2);
        scalprod = ut_vector_scalprod (v1, v2);

        if ((ut_array_1d_int_equal (Tess.EdgeDom[edge], 2, edgedom, 2))
            && (fabs (scalprod) >= colinear - 1e-6
                || neut_tess_edge_iscurved (Tess, edge)))
        {
          ut_array_1d_int_list_addval ((*pedges) + id, (*pedgeqty) + id,
                                       inter[j]);
          ut_array_1d_int_list_rmval (&tmp, &qty, inter[j]);
        }

        ut_free_1d (&v1);
        ut_free_1d (&v2);
      }
    }
  }

  ut_free_1d_int (&tmp);
  ut_free_1d_int (&tmp2);
  ut_free_1d_int (&inter);
  ut_free_1d_int (&edgedom);

  return;
}

void
neut_tess_ver_cells (struct TESS Tess, int ver, int **pcell, int *pcellqty)
{
  if (Tess.Dim == 3)
    neut_tess_ver_polys (Tess, ver, pcell, pcellqty);
  else if (Tess.Dim == 2)
    neut_tess_ver_faces (Tess, ver, pcell, pcellqty);
  else
    ut_print_neperbug ();

  return;
}

void
neut_tess_face_cells (struct TESS Tess, int face, int **pcell, int *pcellqty)
{
  neut_tess_face_polys (Tess, face, pcell, pcellqty);

  return;
}

int
neut_tess_seed_masterseed (struct TESS Tess, int seed, int *pmaster)
{
  if (seed < 1 || seed > Tess.SeedQty)
  {
    (*pmaster) = -1;
    return -1;
  }

  else
  {
    (*pmaster) = seed <= Tess.CellQty ? seed : Tess.PerSeedMaster[seed];
    return 0;
  }
}

int
neut_tess_seed_isper (struct TESS Tess, int seed)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerSeedMaster[seed] > 0);
}

int
neut_tess_seed_iscell (struct TESS Tess, int seed)
{
  return (seed >= 1 && seed <= Tess.CellQty);
}

int
neut_tess_ver_isper (struct TESS Tess, int ver)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerVerMaster[ver] > 0 || Tess.PerVerSlaveQty[ver] > 0);
}

int
neut_tess_edge_isper (struct TESS Tess, int edge)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerEdgeMaster[edge] > 0 || Tess.PerEdgeSlaveQty[edge] > 0);
}

int
neut_tess_face_isper (struct TESS Tess, int face)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerFaceMaster[face] > 0 || Tess.PerFaceSlaveNb[face] > 0);
}

int
neut_tess_ver_ispermaster (struct TESS Tess, int ver)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerVerSlaveQty[ver] > 0);
}

int
neut_tess_edge_ispermaster (struct TESS Tess, int edge)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerEdgeSlaveQty[edge] > 0);
}

int
neut_tess_face_ispermaster (struct TESS Tess, int face)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerFaceSlaveNb[face] > 0);
}

int
neut_tess_ver_isperslave (struct TESS Tess, int ver)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerVerMaster[ver] > 0);
}

int
neut_tess_edge_isperslave (struct TESS Tess, int edge)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerEdgeMaster[edge] > 0);
}

int
neut_tess_face_isperslave (struct TESS Tess, int face)
{
  if (!strcmp (Tess.Type, "standard"))
    return 0;

  else
    return (Tess.PerFaceMaster[face] > 0);
}

void
neut_tess_ver_inters (struct TESS Tess, int ver, int **pinters,
                      int *pinterqty)
{
  if (Tess.Dim == 2)
    neut_tess_ver_edges (Tess, ver, pinters, pinterqty);
  else if (Tess.Dim == 3)
    neut_tess_ver_faces (Tess, ver, pinters, pinterqty);
  else
    abort ();
}

void
neut_tess_inter_seeds (struct TESS Tess, int inter, int **pseed,
                       int *pseedqty)
{
  if (Tess.Dim == 2)
    neut_tess_edge_seeds (Tess, inter, pseed, pseedqty);
  else if (Tess.Dim == 3)
    neut_tess_face_seeds (Tess, inter, pseed, pseedqty);
  else
    abort ();
}

void
neut_tess_inter_seeds_positive (struct TESS Tess, int inter, int **pseeds,
                                int *pseedqty)
{
  if (Tess.Dim == 2)
    neut_tess_edge_seeds_positive (Tess, inter, pseeds, pseedqty);
  else if (Tess.Dim == 3)
    neut_tess_face_seeds_positive (Tess, inter, pseeds, pseedqty);
  else
    abort ();

  return;
}

int
neut_tess_inter_isperslave (struct TESS Tess, int inter)
{
  if (Tess.Dim == 2)
    return neut_tess_edge_isperslave (Tess, inter);
  else if (Tess.Dim == 3)
    return neut_tess_face_isperslave (Tess, inter);
  else
    abort ();
}

int
neut_tess_face_scale_polys (struct TESS Tess, int face, int scale, int *poly)
{
  int i;

  ut_array_1d_int_memcpy (Tess.FacePoly[face], 2, poly);

  if (Tess.ScaleQty > 1 && scale != Tess.ScaleQty)
    for (i = 0; i < 2; i++)
      if (poly[i] > 0)
        poly[i] = Tess.ScaleCellId[Tess.FacePoly[face][i]][scale];

  return 0;
}

int
neut_tess_face_scale (struct TESS Tess, int face, int *pscale)
{
  int i, poly[2];

  if (Tess.Dim < 3)
  {
    if (Tess.ScaleQty == 1)
    {
      (*pscale) = 1;
      return 0;
    }
    else
      abort ();
  }

  (*pscale) = -1;

  if (Tess.FacePoly[face][0] < 0 || Tess.FacePoly[face][1] < 0)
    (*pscale) = 0;

  else
  {
    (*pscale) = 1;
    for (i = Tess.ScaleQty - 1; i >= 0; i--)
    {
      neut_tess_face_scale_polys (Tess, face, i, poly);
      if (poly[0] == poly[1])
      {
        (*pscale) = i + 1;
        break;
      }
    }
  }

  if ((*pscale) == -1)
    ut_print_neperbug ();

  return 0;
}

void
neut_tess_polypair_commonfaces (struct TESS Tess, int poly1, int poly2,
                                int **pfaces, int *pfaceqty)
{
  (*pfaces) = ut_alloc_1d_int (Tess.PolyFaceQty[poly1]);

  ut_array_1d_int_inter (Tess.PolyFaceNb[poly1] + 1, Tess.PolyFaceQty[poly1],
                         Tess.PolyFaceNb[poly2] + 1, Tess.PolyFaceQty[poly2],
                         *pfaces, pfaceqty);

  return;
}

void
neut_tess_seedpair_commonfaces (struct TESS Tess, int seed1, int seed2,
                                int **pfaces, int *pfaceqty)
{
  int i, face, poly1, poly2;

  if (seed1 > 0 && seed2 > 0)
    neut_tess_polypair_commonfaces (Tess, seed1, seed2, pfaces, pfaceqty);
  else if (seed1 <= 0 && seed2 <= 0)
    abort ();
  else
  {
    poly1 = seed1 > 0 ? seed1 : seed2;
    poly2 = seed1 > 0 ? seed2 : seed1;

    (*pfaces) = ut_realloc_1d_int (*pfaces, Tess.PolyFaceQty[poly1]);
    (*pfaceqty) = Tess.PolyFaceQty[poly1];
    ut_array_1d_int_memcpy (Tess.PolyFaceNb[poly1] + 1, *pfaceqty, *pfaces);

    for (i = 0; i < *pfaceqty; i++)
    {
      face = (*pfaces)[i];

      if (ut_array_1d_int_eltpos (Tess.FacePoly[face], 2, poly2) == -1)
      {
        ut_array_1d_int_list_rmval (pfaces, pfaceqty, face);
        i--;
      }
    }

    (*pfaces) = ut_realloc_1d_int (*pfaces, *pfaceqty);
  }

  return;
}

void
neut_tess_vers_allfaces (struct TESS Tess, int *vers, int verqty,
                         int **pfaces, int *pfaceqty)
{
  int i, ver, qty, *tmp = NULL;

  (*pfaceqty) = 0;
  ut_free_1d_int (pfaces);

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    neut_tess_ver_faces (Tess, ver, &tmp, &qty);

    (*pfaces) = ut_realloc_1d_int (*pfaces, *pfaceqty + qty);
    ut_array_1d_int_memcpy (tmp, qty, *pfaces + *pfaceqty);
    (*pfaceqty) += qty;
  }

  ut_array_1d_int_sort_uniq (*pfaces, *pfaceqty, pfaceqty);
  (*pfaces) = ut_realloc_1d_int (*pfaces, *pfaceqty);

  ut_free_1d_int (&tmp);

  return;
}

void
neut_tess_face_vercoos (struct TESS Tess, int face, double ***pvercoos,
                        int *pverqty)
{
  int i, ver, *vers = NULL;

  neut_tess_face_vers (Tess, face, &vers, pverqty);

  (*pvercoos) = ut_alloc_2d (*pverqty, 3);

  for (i = 0; i < *pverqty; i++)
  {
    ver = vers[i];

    ut_array_1d_memcpy (Tess.VerCoo[ver], 3, (*pvercoos)[i]);
  }

  ut_free_1d_int (&vers);

  return;
}

void
neut_tess_domfaces_faces (struct TESS Tess, int *domfaces, int domfaceqty,
                          int **pfaces, int *pfaceqty)
{
  int i, domface;
  int **tmp = ut_alloc_1d_pint (domfaceqty);
  int *qty = ut_alloc_1d_int (domfaceqty);

  for (i = 0; i < domfaceqty; i++)
  {
    domface = domfaces[i];

    tmp[i] = Tess.DomTessFaceNb[domface] + 1;
    qty[i] = Tess.DomTessFaceQty[domface];
  }

  ut_array_1d_int_lists_merge (tmp, qty, domfaceqty, pfaces, pfaceqty);

  ut_free_1d_pint (&tmp);
  ut_free_1d_int (&qty);

  return;
}

int
neut_tess_edge_iscurved (struct TESS Tess, int edge)
{
  int i, curved;
  int domface, *domfaces = NULL, domfaceqty;

  if (Tess.Dim == 3)
  {
    neut_tess_edge_domfaces (Tess, edge, &domfaces, &domfaceqty);

    curved = 0;
    for (i = 0; i < domfaceqty; i++)
    {
      domface = domfaces[i];

      if (strcmp (Tess.DomFaceType[domface], "plane"))
      {
        curved = 1;
        break;
      }
    }
  }
  else
    curved = 0;

  ut_free_1d_int (&domfaces);

  return curved;
}

int
neut_tess_face_iscurved (struct TESS Tess, int face)
{
  return Tess.FaceDom && Tess.FaceDom[face][0] == 2 && Tess.DomFaceType
    && strcmp (Tess.DomFaceType[Tess.FaceDom[face][1]], "plane");
}

int
neut_tess_face_hascurvededge (struct TESS Tess, int face)
{
  int i;

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    if (neut_tess_edge_iscurved (Tess, Tess.FaceEdgeNb[face][i]))
      return 1;

  return 0;
}

void
neut_tess_edge_scale (struct TESS Tess, int edge, int *pscale)
{
  int i, faceqty, *faces = NULL;

  neut_tess_edge_faces (Tess, edge, &faces, &faceqty);

  for (i = 0; i < faceqty; i++)
    neut_tess_face_scale (Tess, faces[i], faces + i);

  (*pscale) = ut_array_1d_int_max (faces, faceqty);

  ut_free_1d_int (&faces);

  return;
}

void
neut_tess_ver_scale (struct TESS Tess, int ver, int *pscale)
{
  int i, edgeqty, *edges = NULL;

  neut_tess_ver_edges (Tess, ver, &edges, &edgeqty);

  for (i = 0; i < edgeqty; i++)
    neut_tess_edge_scale (Tess, edges[i], edges + i);

  (*pscale) = ut_array_1d_int_max (edges, edgeqty);

  ut_free_1d_int (&edges);

  return;
}
