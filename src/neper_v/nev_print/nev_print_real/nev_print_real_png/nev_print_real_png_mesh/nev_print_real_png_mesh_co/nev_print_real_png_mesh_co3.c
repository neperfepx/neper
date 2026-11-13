/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_mesh_co_.h"

// comparing two edges, given as (sorted) pairs of nodes, for qsort
int
nev_print_real_png_mesh_co_edge_cmp (const void *a, const void *b)
{
  const int *pa = (const int *) a, *pb = (const int *) b;

  return (pa[0] != pb[0]) ? pa[0] - pb[0] : pa[1] - pb[1];
}

// nev_print_real_png_mesh_co_seq returns the number of vertex nodes of the
// cohesive elts, their faces as triangles and their edges, the first
// *prefedgeqty edges being those of the interface facet (nodes 0, 1...).
void
nev_print_real_png_mesh_co_seq (struct MESH MeshCo, int *pvernodeqty, int ***pseqtri,
                                int *ptriqty, int ***pseqedge, int *pedgeqty,
                                int *prefedgeqty)
{
  // 3D prism: facet 0-1-2, copy 3-4-5
  int prismtri[8][3] = { {0, 1, 2}, {3, 5, 4}, {0, 4, 1}, {0, 3, 4},
                         {1, 5, 2}, {1, 4, 5}, {2, 3, 0}, {2, 5, 3} };
  int prismedge[9][2] = { {0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3},
                          {0, 3}, {1, 4}, {2, 5} };
  // 2D quadrangle: facet 0-1, copy 3-2
  int quadtri[2][3] = { {0, 1, 2}, {0, 2, 3} };
  int quadedge[4][2] = { {0, 1}, {1, 2}, {2, 3}, {3, 0} };
  int (*tri)[3] = NULL, (*edge)[2] = NULL;

  if (!strcmp (MeshCo.EltType, "triprism"))
  {
    (*pvernodeqty) = 6;
    (*ptriqty) = 8;
    (*pedgeqty) = 9;
    (*prefedgeqty) = 3;
    tri = prismtri;
    edge = prismedge;
  }
  else if (MeshCo.Dimension == 2
           && (!strcmp (MeshCo.EltType, "quad") || !strcmp (MeshCo.EltType, "quad6")))
  {
    (*pvernodeqty) = 4;
    (*ptriqty) = 2;
    (*pedgeqty) = 4;
    (*prefedgeqty) = 1;
    tri = quadtri;
    edge = quadedge;
  }
  else
    ut_print_message (2, 3, "Cohesive elts of type `%s' not supported.\n", MeshCo.EltType);

  (*pseqtri) = ut_alloc_2d_int (*ptriqty, 3);
  for (int i = 0; i < *ptriqty; i++)
    ut_array_1d_int_memcpy (tri[i], 3, (*pseqtri)[i]);

  (*pseqedge) = ut_alloc_2d_int (*pedgeqty, 2);
  for (int i = 0; i < *pedgeqty; i++)
    ut_array_1d_int_memcpy (edge[i], 2, (*pseqedge)[i]);

  return;
}
