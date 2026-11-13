/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_.h"

int
net_reg_merge (struct TESS *pTess, struct REG Reg, int *pDelId)
{
  int i, loop, testtess;
  int edge, newver;
  double lratio1;
  double fmax, minmaxff = 0;
  int ThisDelQty, DelQty = 0;
  int depth = 3;               // maximum number of edges deleted together
  int seqqty, *seq = ut_alloc_1d_int (depth);
  int ***bufs = ut_alloc_1d_ppint (depth);
  struct TESS *TessBufs = calloc (depth, sizeof (struct TESS));

  int verbosity = 0;

  // one buffer per deletion level
  for (i = 0; i < depth; i++)
  {
    bufs[i] = ut_alloc_2d_int (4, 100);
    neut_tess_set_zero (TessBufs + i);
    neut_tess_alloc (TessBufs + i, (*pTess).VerQty, (*pTess).EdgeQty,
                     (*pTess).FaceQty, (*pTess).PolyQty);
    TessBufs[i].Dim = (*pTess).Dim;
    net_reg_merge_buffer_per (*pTess, TessBufs + i);
  }

  char *string = ut_alloc_1d_char (100);
  char *prevstring = ut_alloc_1d_char (100);

  for (loop = 1; loop <= Reg.mloop; loop++)
  {
    edge = net_reg_merge_nextedge (*pTess, Reg, &lratio1);

    while (edge != -1)
    {
      if ((*pDelId) >= Reg.maxedgedelqty)
        break;

      ThisDelQty = 0;
      seqqty = 0;
      minmaxff = 0;

      testtess =
        net_reg_merge_del (pTess, edge, &newver, &fmax, Reg.fmax, DBL_MAX,
                           Reg.delcell, bufs[0], TessBufs, verbosity);

      if ((*pTess).Dim == 2)
      {
        if (testtess < 0)
          net_reg_merge_undel (pTess, bufs[0], TessBufs[0]);
        else
          ThisDelQty = 1;
      }

      else if ((*pTess).Dim == 3)
      {
        // the deletion is kept if it leads to a valid tessellation of
        // acceptable face flatness fault, otherwise edges of the new
        // vertex are deleted as well, which can be needed to obtain a
        // valid tessellation (when 2 faces would share 2 successive
        // edges) or which can reduce the flatness fault.
        if (testtess == 0 && fmax < Reg.fmax)
          ThisDelQty = 1;

        else if (testtess != -2)
        {
          // (iterative deepening: as few edges as possible are deleted)
          minmaxff = DBL_MAX;
          for (i = 1; i < depth && minmaxff >= Reg.fmax; i++)
            minmaxff =
              net_reg_merge_search_vernbs (pTess, Reg, newver, lratio1,
                                           testtess != 0, i, bufs + 1,
                                           TessBufs + 1, seq, &seqqty);

          if (minmaxff < Reg.fmax)
          {
            ThisDelQty = 1 + seqqty;

            for (i = 0; i < seqqty; i++)
            {
              net_reg_merge_del (pTess, seq[i], &newver, &fmax, Reg.fmax,
                                 DBL_MAX, Reg.delcell, NULL, NULL, verbosity);
              (*pTess).EdgeDel[seq[i]] = 1;
            }
          }
          else
            net_reg_merge_undel (pTess, bufs[0], TessBufs[0]);
        }

        else
          net_reg_merge_undel (pTess, bufs[0], TessBufs[0]);
      }

      net_reg_merge_undel_free (bufs[0], TessBufs);

      /* in all cases, edgedel <- 1 */
      net_reg_merge_del_edgedel (pTess, edge);

      DelQty += ThisDelQty;
      (*pDelId) += ThisDelQty;

      if (verbosity >= 1)
        net_reg_merge_print (loop, lratio1, ThisDelQty, edge,
                             seqqty ? seq[0] : 0, fmax, minmaxff);

      /* Searching next edge to delete */
      edge = net_reg_merge_nextedge (*pTess, Reg, &lratio1);

      sprintf (string, "loop %d/%d: %%3.0f%%%% del=%d", loop, Reg.mloop,
               DelQty);
      ut_print_progress_nonl (stdout, 100 * lratio1, 100, string, prevstring);
    }

    for (i = 1; i <= (*pTess).EdgeQty; i++)
      (*pTess).EdgeDel[i] = 0;

    if (DelQty == 0)
      break;
  }

  for (i = 0; i < depth; i++)
  {
    ut_free_2d_int (bufs + i, 4);
    neut_tess_free (TessBufs + i);
  }
  free (bufs);
  free (TessBufs);
  ut_free_1d_int (&seq);
  ut_free_1d_char (&string);
  ut_free_1d_char (&prevstring);

  return DelQty;
}

// Searching the deletion of an edge of a vertex, possibly followed by the
// deletion of edges of the new vertex (depth edges at most), that leads to
// a valid tessellation of lowest maximum face flatness fault.  The
// tessellation is left unchanged.  lratio is the length ratio of the edge
// whose deletion led to the vertex, and an edge of the vertex is
// considered if the sum of their length ratios is lower than 2, or, if
// invalid == 1 (the tessellation is invalid), whatever its length.
// Returns the maximum face flatness fault (DBL_MAX if none) and the edges
// to delete, in seq (seqqty of them).
double
net_reg_merge_search_vernbs (struct TESS *pTess, struct REG Reg, int ver,
                             double lratio, int invalid, int depth,
                             int ***bufs, struct TESS *TessBufs, int *seq,
                             int *pseqqty)
{
  int i, edge, newver, test, seqqty2, edgeqty, *edges = NULL;
  double ff, minff = DBL_MAX, lratio2;
  int *seq2 = NULL;

  (*pseqqty) = 0;

  if (depth <= 0)
    return minff;

  seq2 = ut_alloc_1d_int (depth);

  // (the edges of the vertex change with the deletions)
  edgeqty = (*pTess).VerEdgeQty[ver];
  edges = ut_alloc_1d_int (edgeqty);
  ut_array_1d_int_memcpy ((*pTess).VerEdgeNb[ver], edgeqty, edges);

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    if (!net_reg_merge_search_edgeok (*pTess, edge))
      continue;

    lratio2 = (*pTess).EdgeLength[edge] / Reg.EdgeSel[edge];
    if (!invalid && lratio + lratio2 >= 2)
      continue;

    // (at the last level, the tessellation needs to be tested only for
    // an acceptable flatness fault)
    test =
      net_reg_merge_del (pTess, edge, &newver, &ff, Reg.fmax,
                         (depth == 1) ? Reg.fmax : DBL_MAX, Reg.delcell,
                         bufs[0], TessBufs, 0);

    seqqty2 = 0;
    if (test == 0 && ff < Reg.fmax)
      ;
    else if (test != -2)
      ff = net_reg_merge_search_vernbs (pTess, Reg, newver, lratio, test != 0,
                                        depth - 1, bufs + 1, TessBufs + 1,
                                        seq2, &seqqty2);
    else
      ff = DBL_MAX;

    net_reg_merge_undel (pTess, bufs[0], TessBufs[0]);
    net_reg_merge_undel_free (bufs[0], TessBufs);

    if (ff < minff)
    {
      minff = ff;
      seq[0] = edge;
      ut_array_1d_int_memcpy (seq2, seqqty2, seq + 1);
      (*pseqqty) = 1 + seqqty2;
    }
  }

  ut_free_1d_int (&edges);
  ut_free_1d_int (&seq2);

  return minff;
}

// Testing whether an edge can be considered for deletion, in addition to
// another one
int
net_reg_merge_search_edgeok (struct TESS Tess, int edge)
{
  int ver1 = Tess.EdgeVerNb[edge][0];
  int ver2 = Tess.EdgeVerNb[edge][1];

  if (Tess.EdgeState[edge] != 0)
    return 0;

  // the 2 vertices are on (different) domain edges: the edge would shrink
  // to a third vertex (which is a domain vertex), which is not handled
  if (Tess.EdgeDom[edge][0] == 2 && Tess.VerDom[ver1][0] == 1
      && Tess.VerDom[ver2][0] == 1)
    return 0;

  if (Tess.EdgeDom[edge][0] == 1 && Tess.VerDom[ver1][0] == 0
      && Tess.VerDom[ver2][0] == 0)
    return 0;

  return 1;
}
