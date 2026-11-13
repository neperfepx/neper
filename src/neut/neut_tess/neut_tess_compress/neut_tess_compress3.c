/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_compress_.h"

void
neut_tess_compress_movepoly (struct TESS *pTess, int old, int new)
{
  int i, pos, face;

  (*pTess).PolyState[new] = (*pTess).PolyState[old];

  (*pTess).PolyFaceQty[new] = (*pTess).PolyFaceQty[old];

  (*pTess).PolyFaceNb[new] =
    ut_realloc_1d_int ((*pTess).PolyFaceNb[new],
                       (*pTess).PolyFaceQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).PolyFaceNb[old] + 1,
                          (*pTess).PolyFaceQty[old],
                          (*pTess).PolyFaceNb[new] + 1);

  (*pTess).PolyFaceOri[new] =
    ut_realloc_1d_int ((*pTess).PolyFaceOri[new],
                       (*pTess).PolyFaceQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).PolyFaceOri[old] + 1,
                          (*pTess).PolyFaceQty[old],
                          (*pTess).PolyFaceOri[new] + 1);

  /* correcting face poly parameters */
  for (i = 1; i <= (*pTess).PolyFaceQty[new]; i++)
  {
    face = (*pTess).PolyFaceNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pTess).FacePoly[face], 2, old);

    if (pos == -1)
    {
      printf ("FacePolyNb\n");
      abort ();
    }

    (*pTess).FacePoly[face][pos] = new;
  }

  neut_tess_compress_movecell (pTess, old, new);

  return;
}

void
neut_tess_compress_moveface (struct TESS *pTess, int old, int new)
{
  int i, pos, poly, edge;

  /* moving face parameters */
  if ((*pTess).Dim == 3)
    ut_array_1d_int_memcpy ((*pTess).FaceDom[old], 2, (*pTess).FaceDom[new]);

  (*pTess).FacePt[new] = (*pTess).FacePt[old];

  ut_array_1d_memcpy ((*pTess).FacePtCoo[old], 3, (*pTess).FacePtCoo[new]);

  (*pTess).FaceState[new] = (*pTess).FaceState[old];

  ut_array_1d_memcpy ((*pTess).FaceEq[old], 4, (*pTess).FaceEq[new]);

  (*pTess).FaceEdgeOri[new] =
    ut_realloc_1d_int ((*pTess).FaceEdgeOri[new],
                       (*pTess).FaceEdgeQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).FaceEdgeOri[old] + 1,
                          (*pTess).FaceEdgeQty[old],
                          (*pTess).FaceEdgeOri[new] + 1);

  (*pTess).FaceEdgeNb[new] =
    ut_realloc_1d_int ((*pTess).FaceEdgeNb[new],
                       (*pTess).FaceEdgeQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).FaceEdgeNb[old] + 1,
                          (*pTess).FaceEdgeQty[old],
                          (*pTess).FaceEdgeNb[new] + 1);

  (*pTess).FaceEdgeQty[new] = (*pTess).FaceEdgeQty[old];

  (*pTess).FaceVerNb[new] =
    ut_realloc_1d_int ((*pTess).FaceVerNb[new], (*pTess).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).FaceVerNb[old] + 1,
                          (*pTess).FaceVerQty[old],
                          (*pTess).FaceVerNb[new] + 1);

  (*pTess).FaceVerQty[new] = (*pTess).FaceVerQty[old];

  if ((*pTess).Dim == 3)
  {
    ut_array_1d_int_memcpy ((*pTess).FacePoly[old], 2,
                            (*pTess).FacePoly[new]);

    /* correcting poly face parameters (not for a domain face or a
       periodic seed) */
    for (i = 0; i < 2; i++)
      if ((*pTess).FacePoly[new][i] > 0
          && (*pTess).FacePoly[new][i] <= (*pTess).PolyQty)
      {
        poly = (*pTess).FacePoly[new][i];

        pos =
          ut_array_1d_int_eltpos ((*pTess).PolyFaceNb[poly] + 1,
                                  (*pTess).PolyFaceQty[poly], old);

        if (pos == -1)
        {
          printf ("PolyFaceNb\n");
          abort ();
        }

        pos++;
        (*pTess).PolyFaceNb[poly][pos] = new;
      }
  }

  /* correcting edge face parameters */
  for (i = 1; i <= (*pTess).FaceEdgeQty[new]; i++)
  {
    edge = (*pTess).FaceEdgeNb[new][i];

    pos =
      ut_array_1d_int_eltpos ((*pTess).EdgeFaceNb[edge],
                              (*pTess).EdgeFaceQty[edge], old);

    if (pos == -1)
    {
      printf ("face %d has edge %d but not vice versa\n", new, edge);
      abort ();
    }

    (*pTess).EdgeFaceNb[edge][pos] = new;
  }

  if ((*pTess).Dim == 2)
    neut_tess_compress_movecell (pTess, old, new);

  return;
}

void
neut_tess_compress_moveedge (struct TESS *pTess, int old, int new)
{
  int i, pos, face, ver;

  /* moving edge parameters */
  (*pTess).EdgeDel[new] = (*pTess).EdgeDel[old];
  (*pTess).EdgeState[new] = (*pTess).EdgeState[old];
  (*pTess).EdgeLength[new] = (*pTess).EdgeLength[old];
  (*pTess).EdgeFaceQty[new] = (*pTess).EdgeFaceQty[old];

  ut_array_1d_int_memcpy ((*pTess).EdgeDom[old], 2, (*pTess).EdgeDom[new]);

  (*pTess).EdgeFaceNb[new] =
    ut_realloc_1d_int ((*pTess).EdgeFaceNb[new], (*pTess).EdgeFaceQty[old]);

  ut_array_1d_int_memcpy ((*pTess).EdgeFaceNb[old], (*pTess).EdgeFaceQty[old],
                          (*pTess).EdgeFaceNb[new]);

  ut_array_1d_int_memcpy ((*pTess).EdgeVerNb[old], 2,
                          (*pTess).EdgeVerNb[new]);

  /* correcting face edge parameters */
  for (i = 0; i < (*pTess).EdgeFaceQty[new]; i++)
  {
    face = (*pTess).EdgeFaceNb[new][i];

    // periodic seed (2D periodic tessellation)
    if (face > (*pTess).FaceQty && (*pTess).Type
        && !strcmp ((*pTess).Type, "periodic"))
      continue;

    if (face > (*pTess).FaceQty)
      abort ();

    pos =
      ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[face] + 1,
                              (*pTess).FaceEdgeQty[face], old);


    if (pos == -1)
    {
      printf ("edge %d has face %d but not vice versa\n", new, face);
      abort ();
    }

    pos++;
    (*pTess).FaceEdgeNb[face][pos] = new;
  }

  /* correcting ver edge parameters */
  for (i = 0; i < 2; i++)
  {
    ver = (*pTess).EdgeVerNb[new][i];

    pos =
      ut_array_1d_int_eltpos ((*pTess).VerEdgeNb[ver],
                              (*pTess).VerEdgeQty[ver], old);

    if (pos == -1)
    {
      printf ("VerEdgeNb\n");
      abort ();
    }

    (*pTess).VerEdgeNb[ver][pos] = new;
  }

  return;
}

void
neut_tess_compress_movever (struct TESS *pTess, int old, int new)
{
  int i, pos, edge, faceqty;
  int *face = NULL;

  /* moving ver parameters */
  (*pTess).VerState[new] = (*pTess).VerState[old];

  ut_array_1d_int_memcpy ((*pTess).VerDom[old], 2, (*pTess).VerDom[new]);

  (*pTess).VerEdgeQty[new] = (*pTess).VerEdgeQty[old];

  (*pTess).VerEdgeNb[new] =
    ut_realloc_1d_int ((*pTess).VerEdgeNb[new], (*pTess).VerEdgeQty[old]);

  ut_array_1d_int_memcpy ((*pTess).VerEdgeNb[old], (*pTess).VerEdgeQty[old],
                          (*pTess).VerEdgeNb[new]);

  ut_array_1d_memcpy ((*pTess).VerCoo[old], 3, (*pTess).VerCoo[new]);

  /* correcting edge ver parameters */
  for (i = 0; i < (*pTess).VerEdgeQty[new]; i++)
  {
    edge = (*pTess).VerEdgeNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[edge], 2, old);

    if (pos == -1)
    {
      printf ("EdgeVerNb\n");
      abort ();
    }

    (*pTess).EdgeVerNb[edge][pos] = new;
  }

  neut_tess_ver_faces (*pTess, new, &face, &faceqty);
  for (i = 0; i < faceqty; i++)
  {
    pos =
      ut_array_1d_int_eltpos ((*pTess).FaceVerNb[face[i]] + 1,
                              (*pTess).FaceVerQty[face[i]], old);

    if (pos == -1)
    {
      printf ("FaceVerNb\n");
      abort ();
    }

    pos++;
    (*pTess).FaceVerNb[face[i]][pos] = new;
  }

  ut_free_1d_int (&face);

  return;
}

void
neut_tess_compress_movedomface (struct TESS *pTess, int old, int new)
{
  int i, face;

  /* moving face parameters */
  if ((*pTess).Dim == 2 || old == new)
    return;

  ut_string_string ((*pTess).DomFaceLabel[old], &(*pTess).DomFaceLabel[new]);

  ut_array_1d_memcpy ((*pTess).DomFaceEq[old], 4, (*pTess).DomFaceEq[new]);

  ut_string_string ((*pTess).DomFaceType[old], &(*pTess).DomFaceType[new]);

  (*pTess).DomFaceParmQty[new] = (*pTess).DomFaceParmQty[old];

  (*pTess).DomFaceParms[new] =
    ut_realloc_1d ((*pTess).DomFaceParms[new], (*pTess).DomFaceParmQty[new]);

  ut_array_1d_memcpy ((*pTess).DomFaceParms[old],
                      (*pTess).DomFaceParmQty[new],
                      (*pTess).DomFaceParms[new]);

  if ((*pTess).DomFaceVerQty)
  {
    (*pTess).DomFaceVerQty[new] = (*pTess).DomFaceVerQty[old];

    (*pTess).DomFaceVerNb[new] =
      ut_realloc_1d_int ((*pTess).DomFaceVerNb[new],
                         (*pTess).DomFaceVerQty[new] + 1);

    ut_array_1d_int_memcpy ((*pTess).DomFaceVerNb[old] + 1,
                            (*pTess).DomFaceVerQty[new],
                            (*pTess).DomFaceVerNb[new] + 1);
  }

  if ((*pTess).DomFaceEdgeQty)
  {
    (*pTess).DomFaceEdgeQty[new] = (*pTess).DomFaceEdgeQty[old];

    (*pTess).DomFaceEdgeNb[new] =
      ut_realloc_1d_int ((*pTess).DomFaceEdgeNb[new],
                         (*pTess).DomFaceEdgeQty[new] + 1);

    ut_array_1d_int_memcpy ((*pTess).DomFaceEdgeNb[old] + 1,
                            (*pTess).DomFaceEdgeQty[new],
                            (*pTess).DomFaceEdgeNb[new] + 1);
  }

  (*pTess).DomTessFaceQty[new] = (*pTess).DomTessFaceQty[old];

  (*pTess).DomTessFaceNb[new] =
    ut_realloc_1d_int ((*pTess).DomTessFaceNb[new],
                       (*pTess).DomTessFaceQty[new] + 1);

  ut_array_1d_int_memcpy ((*pTess).DomTessFaceNb[old] + 1,
                          (*pTess).DomTessFaceQty[new],
                          (*pTess).DomTessFaceNb[new] + 1);

  for (i = 1; i <= (*pTess).DomTessFaceQty[new]; i++)
  {
    face = (*pTess).DomTessFaceNb[new][i];

    if ((*pTess).FaceDom[face][0] != 2 || (*pTess).FaceDom[face][1] != old)
      ut_print_neperbug ();

    (*pTess).FaceDom[face][1] = new;

    if ((*pTess).FacePoly[face][1] != -old)
      ut_print_neperbug ();

    (*pTess).FacePoly[face][1] = -new;
  }

  return;
}

void
neut_tess_compress_movedomedge (struct TESS *pTess, int old, int new)
{
  int i, edge;

  if (old == new)
    return;

  ut_string_string ((*pTess).DomEdgeLabel[old], &(*pTess).DomEdgeLabel[new]);

  ut_array_1d_int_memcpy ((*pTess).DomEdgeVerNb[old], 2,
                          (*pTess).DomEdgeVerNb[new]);

  ut_array_1d_int_memcpy ((*pTess).DomEdgeFaceNb[old], 2,
                          (*pTess).DomEdgeFaceNb[new]);

  ut_string_string ((*pTess).DomEdgeType[old], &(*pTess).DomEdgeType[new]);

  (*pTess).DomEdgeParmQty[new] = (*pTess).DomEdgeParmQty[old];

  (*pTess).DomEdgeParms[new] =
    ut_realloc_1d ((*pTess).DomEdgeParms[new], (*pTess).DomEdgeParmQty[new]);


  (*pTess).DomTessEdgeQty[new] = (*pTess).DomTessEdgeQty[old];

  (*pTess).DomTessEdgeNb[new] =
    ut_realloc_1d_int ((*pTess).DomTessEdgeNb[new],
                       (*pTess).DomTessEdgeQty[new] + 1);

  ut_array_1d_int_memcpy ((*pTess).DomTessEdgeNb[old] + 1,
                          (*pTess).DomTessEdgeQty[new],
                          (*pTess).DomTessEdgeNb[new] + 1);

  for (i = 1; i <= (*pTess).DomTessEdgeQty[new]; i++)
  {
    edge = (*pTess).DomTessEdgeNb[new][i];

    if ((*pTess).EdgeDom[edge][0] != 1 || (*pTess).EdgeDom[edge][1] != old)
      ut_print_neperbug ();

    (*pTess).EdgeDom[edge][1] = new;
  }

  return;
}

void
neut_tess_compress_movedomver (struct TESS *pTess, int old, int new)
{
  int ver;

  if (old == new)
    return;

  ut_string_string ((*pTess).DomVerLabel[old], &(*pTess).DomVerLabel[new]);

  ut_array_1d_memcpy ((*pTess).DomVerCoo[old] + 1, 3,
                      (*pTess).DomVerCoo[new] + 1);

  (*pTess).DomVerEdgeQty[new] = (*pTess).DomVerEdgeQty[old];

  (*pTess).DomVerEdgeNb[new] =
    ut_realloc_1d_int ((*pTess).DomVerEdgeNb[new],
                       (*pTess).DomVerEdgeQty[new]);

  ut_array_1d_int_memcpy ((*pTess).DomVerEdgeNb[old],
                          (*pTess).DomVerEdgeQty[new],
                          (*pTess).DomVerEdgeNb[new]);

  (*pTess).DomTessVerNb[new] = (*pTess).DomTessVerNb[old];

  ver = (*pTess).DomTessVerNb[new];

  if ((*pTess).VerDom[ver][0] != 0 || (*pTess).VerDom[ver][1] != old)
    ut_print_neperbug ();

  (*pTess).VerDom[ver][1] = new;

  return;
}

/* The following functions renumber the periodicity of the vertices,
 * edges or faces after their compression: old_new is the new number of
 * each (old) entity (0 if it was deleted), and oldqty is the old number of
 * entities.  A deleted entity must have no periodicity. */

void
neut_tess_compress_vers_per (struct TESS *pTess, int *old_new, int oldqty)
{
  int i, master, *masters = ut_alloc_1d_int ((*pTess).VerQty + 1);
  int **shifts = ut_alloc_2d_int ((*pTess).VerQty + 1, 3);

  for (i = 1; i <= oldqty; i++)
    if (old_new[i] > 0)
    {
      master = (*pTess).PerVerMaster[i];
      if (master > 0 && old_new[master] == 0)
        ut_print_neperbug ();

      masters[old_new[i]] = (master > 0) ? old_new[master] : 0;
      ut_array_1d_int_memcpy ((*pTess).PerVerShift[i], 3, shifts[old_new[i]]);
    }

  ut_free_1d_int (&(*pTess).PerVerNb);
  ut_free_1d_int (&(*pTess).PerVerMaster);
  ut_free_2d_int (&(*pTess).PerVerShift, oldqty + 1);
  ut_free_1d_int (&(*pTess).PerVerSlaveQty);
  ut_free_2d_int (&(*pTess).PerVerSlaveNb, oldqty + 1);

  (*pTess).PerVerMaster = masters;
  (*pTess).PerVerShift = shifts;

  (*pTess).PerVerQty = 0;
  (*pTess).PerVerNb = ut_alloc_1d_int ((*pTess).VerQty + 1);
  for (i = 1; i <= (*pTess).VerQty; i++)
    if (masters[i] > 0)
      (*pTess).PerVerNb[++(*pTess).PerVerQty] = i;
  (*pTess).PerVerNb =
    ut_realloc_1d_int ((*pTess).PerVerNb, (*pTess).PerVerQty + 1);

  neut_tess_init_verslave (pTess);

  return;
}

void
neut_tess_compress_edges_per (struct TESS *pTess, int *old_new, int oldqty)
{
  int i, master, *masters = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  int **shifts = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 3);
  int *oris = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  for (i = 1; i <= oldqty; i++)
    if (old_new[i] > 0)
    {
      master = (*pTess).PerEdgeMaster[i];
      if (master > 0 && old_new[master] == 0)
        ut_print_neperbug ();

      masters[old_new[i]] = (master > 0) ? old_new[master] : 0;
      ut_array_1d_int_memcpy ((*pTess).PerEdgeShift[i], 3,
                              shifts[old_new[i]]);
      oris[old_new[i]] = (*pTess).PerEdgeOri[i];
    }

  ut_free_1d_int (&(*pTess).PerEdgeNb);
  ut_free_1d_int (&(*pTess).PerEdgeMaster);
  ut_free_2d_int (&(*pTess).PerEdgeShift, oldqty + 1);
  ut_free_1d_int (&(*pTess).PerEdgeOri);
  ut_free_1d_int (&(*pTess).PerEdgeSlaveQty);
  ut_free_2d_int (&(*pTess).PerEdgeSlaveNb, oldqty + 1);

  (*pTess).PerEdgeMaster = masters;
  (*pTess).PerEdgeShift = shifts;
  (*pTess).PerEdgeOri = oris;

  (*pTess).PerEdgeQty = 0;
  (*pTess).PerEdgeNb = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if (masters[i] > 0)
      (*pTess).PerEdgeNb[++(*pTess).PerEdgeQty] = i;
  (*pTess).PerEdgeNb =
    ut_realloc_1d_int ((*pTess).PerEdgeNb, (*pTess).PerEdgeQty + 1);

  neut_tess_init_edgeslave (pTess);

  return;
}

void
neut_tess_compress_faces_per (struct TESS *pTess, int *old_new, int oldqty)
{
  int i, master, *masters = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  int **shifts = ut_alloc_2d_int ((*pTess).FaceQty + 1, 3);
  int *oris = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  for (i = 1; i <= oldqty; i++)
    if (old_new[i] > 0)
    {
      master = (*pTess).PerFaceMaster[i];
      if (master > 0 && old_new[master] == 0)
        ut_print_neperbug ();

      masters[old_new[i]] = (master > 0) ? old_new[master] : 0;
      ut_array_1d_int_memcpy ((*pTess).PerFaceShift[i], 3,
                              shifts[old_new[i]]);
      oris[old_new[i]] = (*pTess).PerFaceOri[i];
    }

  ut_free_1d_int (&(*pTess).PerFaceNb);
  ut_free_1d_int (&(*pTess).PerFaceMaster);
  ut_free_2d_int (&(*pTess).PerFaceShift, oldqty + 1);
  ut_free_1d_int (&(*pTess).PerFaceOri);
  ut_free_1d_int (&(*pTess).PerFaceSlaveNb);

  (*pTess).PerFaceMaster = masters;
  (*pTess).PerFaceShift = shifts;
  (*pTess).PerFaceOri = oris;

  (*pTess).PerFaceQty = 0;
  (*pTess).PerFaceNb = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if (masters[i] > 0)
      (*pTess).PerFaceNb[++(*pTess).PerFaceQty] = i;
  (*pTess).PerFaceNb =
    ut_realloc_1d_int ((*pTess).PerFaceNb, (*pTess).PerFaceQty + 1);

  neut_tess_init_faceslave (pTess);

  return;
}

// Freeing the data of the cells (and of their seeds) beyond the new
// number of cells, oldqty being the previous one
void
neut_tess_compress_cells (struct TESS *pTess, int oldqty)
{
  int i;

  if ((*pTess).CellOri)
    (*pTess).CellOri =
      ut_realloc_2d_delline ((*pTess).CellOri, oldqty + 1,
                             (*pTess).CellQty + 1);
  if ((*pTess).CellOriDistrib)
    for (i = (*pTess).CellQty + 1; i <= oldqty; i++)
      ut_free_1d_char ((*pTess).CellOriDistrib + i);
  if ((*pTess).ScaleCellId)
    (*pTess).ScaleCellId =
      ut_realloc_2d_int_delline ((*pTess).ScaleCellId, oldqty + 1,
                                 (*pTess).CellQty + 1);

  // (in a periodic tessellation, the seeds also include the periodic seeds)
  if ((*pTess).SeedCoo && (!(*pTess).Type || strcmp ((*pTess).Type, "periodic")))
    (*pTess).SeedCoo =
      ut_realloc_2d_delline ((*pTess).SeedCoo, oldqty + 1, (*pTess).CellQty + 1);

  return;
}
