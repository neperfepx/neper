/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_per_.h"

// Merging the copies of newver and delver, once the copies of an edge
// between them have been deleted, i.e. once each copy of delver of
// edgedelvers has been merged into the copy of newver of edgenewvers of
// the same edge copy (newver and delver are the first ones).
// The other copies of delver become copies of the merged vertex, with
// the same offsets relative to newver as they had relative to delver.
// A copy of delver of the same offset as a copy of newver (i.e. at the same
// location, which happens when the edge has no copy there) is merged into
// it, as well as the edges that then link the same vertices.  The master of
// the merged vertex is its copy of lowest id.  Returns 0 on success, -1 if
// the merged vertex has two edges linking it to the same vertex where the
// edge was deleted (which is not supported).
int
net_reg_merge_del_per_vermerge (struct TESS *pTess, int *edgenewvers,
                                int *edgedelvers, int edgeqty)
{
  int newver = edgenewvers[0], delver = edgedelvers[0];
  int i, j, qty, master, newverqty, delverqty2, *newvers = NULL, status = 0;
  int *delvers2 = NULL, **newveroffsets = NULL, **delveroffsets = NULL;
  int *vers = NULL, **offsets = NULL;

  neut_tess_ver_copies (*pTess, newver, &newvers, &newveroffsets, &newverqty);
  neut_tess_ver_copies (*pTess, delver, &delvers2, &delveroffsets,
                        &delverqty2);

  // copies of the merged vertex, and their offsets relative to newver
  vers = ut_alloc_1d_int (newverqty + delverqty2);
  offsets = ut_alloc_2d_int (newverqty + delverqty2 + 1, 3);

  qty = 0;
  for (i = 0; i < newverqty; i++)
  {
    vers[qty] = newvers[i];
    ut_array_1d_int_memcpy (newveroffsets[i], 3, offsets[qty++]);
  }

  for (i = 0; i < delverqty2; i++)
    if (ut_array_1d_int_eltpos (edgedelvers, edgeqty, delvers2[i]) == -1)
    {
      // copy of newver at the same location, if any
      for (j = 0; j < newverqty; j++)
        if (ut_array_1d_int_equal (newveroffsets[j], 3, delveroffsets[i], 3))
          break;

      if (j < newverqty)
        net_reg_merge_del_per_vermerge_ver (pTess, delvers2[i], newvers[j]);
      else
      {
        vers[qty] = delvers2[i];
        ut_array_1d_int_memcpy (delveroffsets[i], 3, offsets[qty++]);
      }
    }

  // testing for edges linking the same vertices where the edge was deleted
  for (i = 0; i < edgeqty && !status; i++)
    status = net_reg_merge_del_per_vermerge_testedges (*pTess,
                                                       edgenewvers[i]);

  // resetting the copies of newver and delver
  for (i = 0; i < newverqty; i++)
    net_reg_merge_del_per_vermerge_reset (pTess, newvers[i]);
  for (i = 0; i < delverqty2; i++)
    net_reg_merge_del_per_vermerge_reset (pTess, delvers2[i]);

  // recording the copies of the merged vertex
  master = ut_array_1d_int_min (vers, qty);
  i = ut_array_1d_int_eltpos (vers, qty, master);
  ut_array_1d_int_memcpy (offsets[i], 3, offsets[qty]);

  (*pTess).PerVerSlaveNb[master] = ut_alloc_1d_int (qty);
  for (i = 0; i < qty; i++)
    if (vers[i] != master)
    {
      (*pTess).PerVerMaster[vers[i]] = master;
      ut_array_1d_int_sub (offsets[qty], offsets[i], 3,
                           (*pTess).PerVerShift[vers[i]]);
      (*pTess).PerVerSlaveNb[master][++(*pTess).PerVerSlaveQty[master]] =
        vers[i];
    }

  ut_free_1d_int (&newvers);
  ut_free_1d_int (&delvers2);
  ut_free_2d_int (&newveroffsets, newverqty);
  ut_free_2d_int (&delveroffsets, delverqty2);
  ut_free_1d_int (&vers);
  ut_free_2d_int (&offsets, newverqty + delverqty2 + 1);

  return status;
}

// Faces of the copies of a vertex, each with the translation that brings
// it to the frame of the first copy.  A face and its periodic copy (both
// faces of copies of the vertex) are counted once.
void
net_reg_merge_del_per_verfaces (struct TESS Tess, int *vers, int **offsets,
                                int verqty, int **pfaces,
                                double ***pfacetrans, int *pfaceqty)
{
  int i, j, k, face, key, faceqty, *faces = NULL, keyqty = 0, *keys = NULL;

  (*pfaceqty) = 0;

  for (i = 0; i < verqty; i++)
  {
    neut_tess_ver_faces (Tess, vers[i], &faces, &faceqty);

    for (j = 0; j < faceqty; j++)
    {
      face = faces[j];
      key = (Tess.PerFaceMaster && Tess.PerFaceMaster[face] > 0) ?
        Tess.PerFaceMaster[face] : face;

      if (ut_array_1d_int_eltpos (keys, keyqty, key) != -1)
        continue;

      ut_array_1d_int_list_addval_nocheck (&keys, &keyqty, key);

      (*pfaces) = ut_realloc_1d_int (*pfaces, *pfaceqty + 1);
      (*pfacetrans) = ut_realloc_2d_addline (*pfacetrans, *pfaceqty + 1, 3);
      (*pfaces)[*pfaceqty] = face;
      for (k = 0; k < 3; k++)
        (*pfacetrans)[*pfaceqty][k] = -offsets[i][k] * Tess.PeriodicDist[k];
      (*pfaceqty)++;
    }

    ut_free_1d_int (&faces);
  }

  ut_free_1d_int (&keys);

  return;
}

// Setting the copies of ver to ver: coordinates (shifted by their
// offsets), domain and state, and recording their faces as modified.
void
net_reg_merge_del_per_setvers (struct TESS *pTess, int ver, int *vers,
                               int **offsets, int verqty)
{
  int i, j, faceqty, *faces = NULL;

  for (i = 0; i < verqty; i++)
    if (vers[i] != ver)
    {
      for (j = 0; j < 3; j++)
        (*pTess).VerCoo[vers[i]][j] = (*pTess).VerCoo[ver][j]
          + offsets[i][j] * (*pTess).PeriodicDist[j];

      ut_array_1d_int_memcpy ((*pTess).VerDom[ver], 2,
                              (*pTess).VerDom[vers[i]]);
      (*pTess).VerState[vers[i]] = (*pTess).VerState[ver];

      neut_tess_ver_faces (*pTess, vers[i], &faces, &faceqty);
      for (j = 0; j < faceqty; j++)
        if ((*pTess).FaceState[faces[j]] != -1)
          (*pTess).FaceState[faces[j]] = 1;
      ut_free_1d_int (&faces);
    }

  return;
}
