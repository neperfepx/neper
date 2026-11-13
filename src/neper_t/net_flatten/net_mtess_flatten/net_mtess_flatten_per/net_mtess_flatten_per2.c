/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_per_.h"

// Periodic seed of a seed for a shift, or 0 if it does not exist (a
// domain face, which is along a non-periodic direction, is unchanged)
static int
net_mtess_flatten_per_seed (struct TESS Tess, int seed, int *shift)
{
  int i, master, id;
  int *pos = NULL;

  if (seed < 0)
    return seed;

  pos = ut_alloc_1d_int (3);

  if (seed <= Tess.CellQty)
    master = seed;
  else
  {
    master = Tess.PerSeedMaster[seed];
    ut_array_1d_int_memcpy (Tess.PerSeedShift[seed], 3, pos);
  }

  ut_array_1d_int_add (pos, shift, 3, pos);

  id = master;
  for (i = 0; i < 3; i++)
    if (abs (pos[i]) > 1)
      id = 0;

  if (id && (pos[0] || pos[1] || pos[2]))
    id = Tess.PerSeedSlave[master][(pos[2] + 1) * 9 + (pos[1] + 1) * 3 +
                                   (pos[0] + 1)];

  ut_free_1d_int (&pos);

  return id;
}

// Periodic seeds of seeds for a shift (sorted); returns the smallest one
// that has a poly, or -1 if a periodic seed or that poly does not exist
static int
net_mtess_flatten_per_seeds (struct TESS Tess, int *seeds, int seedqty,
                             int *shift, int *perseeds)
{
  int i;

  for (i = 0; i < seedqty; i++)
  {
    perseeds[i] = net_mtess_flatten_per_seed (Tess, seeds[i], shift);
    if (perseeds[i] == 0)
      return -1;
  }

  ut_array_1d_int_sort (perseeds, seedqty);

  for (i = 0; i < seedqty; i++)
    if (perseeds[i] > 0)
      return (perseeds[i] <= Tess.CellQty) ? perseeds[i] : -1;

  return -1;
}

// Periodic vertex of a vertex for a shift, or 0 if it is not in the
// tessellation
int
net_mtess_flatten_per_ver (struct TESS Tess, int ver, int *shift)
{
  int i, poly, seedqty, seedqty2, verqty, perver = 0;
  int *seeds = NULL, *seeds2 = NULL, *perseeds = NULL, *vers = NULL;

  neut_tess_ver_seeds (Tess, ver, &seeds, &seedqty);
  perseeds = ut_alloc_1d_int (seedqty);

  poly = net_mtess_flatten_per_seeds (Tess, seeds, seedqty, shift, perseeds);

  // the periodic vertex is a vertex of the poly of its smallest seed
  if (poly != -1)
  {
    neut_tess_poly_vers (Tess, poly, &vers, &verqty);

    for (i = 0; i < verqty && !perver; i++)
    {
      neut_tess_ver_seeds (Tess, vers[i], &seeds2, &seedqty2);
      ut_array_1d_int_sort (seeds2, seedqty2);
      if (ut_array_1d_int_equal (seeds2, seedqty2, perseeds, seedqty))
        perver = vers[i];
      ut_free_1d_int (&seeds2);
    }
  }

  ut_free_1d_int (&seeds);
  ut_free_1d_int (&perseeds);
  ut_free_1d_int (&vers);

  return perver;
}

// Periodic edge of an edge for a shift, or 0 if it is not in the
// tessellation
int
net_mtess_flatten_per_edge (struct TESS Tess, int edge, int *shift)
{
  int i, poly, seedqty, seedqty2, edgeqty, peredge = 0;
  int *seeds = NULL, *seeds2 = NULL, *perseeds = NULL, *edges = NULL;

  neut_tess_edge_seeds (Tess, edge, &seeds, &seedqty);
  perseeds = ut_alloc_1d_int (seedqty);

  poly = net_mtess_flatten_per_seeds (Tess, seeds, seedqty, shift, perseeds);

  if (poly != -1)
  {
    neut_tess_poly_edges (Tess, poly, &edges, &edgeqty);

    for (i = 0; i < edgeqty && !peredge; i++)
    {
      neut_tess_edge_seeds (Tess, edges[i], &seeds2, &seedqty2);
      ut_array_1d_int_sort (seeds2, seedqty2);
      if (ut_array_1d_int_equal (seeds2, seedqty2, perseeds, seedqty))
        peredge = edges[i];
      ut_free_1d_int (&seeds2);
    }
  }

  ut_free_1d_int (&seeds);
  ut_free_1d_int (&perseeds);
  ut_free_1d_int (&edges);

  return peredge;
}

// Periodic face of a face for a shift, or 0 if it is not in the
// tessellation
int
net_mtess_flatten_per_face (struct TESS Tess, int face, int *shift)
{
  int i, poly, face2, perface = 0;
  int *seeds = ut_alloc_1d_int (2);
  int *perseeds = ut_alloc_1d_int (2);

  ut_array_1d_int_memcpy (Tess.FacePoly[face], 2, seeds);

  poly = net_mtess_flatten_per_seeds (Tess, seeds, 2, shift, perseeds);

  if (poly != -1)
    for (i = 1; i <= Tess.PolyFaceQty[poly] && !perface; i++)
    {
      face2 = Tess.PolyFaceNb[poly][i];
      ut_array_1d_int_memcpy (Tess.FacePoly[face2], 2, seeds);
      ut_array_1d_int_sort (seeds, 2);
      if (ut_array_1d_int_equal (seeds, 2, perseeds, 2))
        perface = face2;
    }

  ut_free_1d_int (&seeds);
  ut_free_1d_int (&perseeds);

  return perface;
}
