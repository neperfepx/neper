/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_.h"

// Setting the selection value of the copies of an edge (periodic
// tessellation) to their minimum value
void
net_reg_init_peredgesel (struct TESS Tess, double *edgesel)
{
  int i, edge, master;

  for (i = 1; i <= Tess.PerEdgeQty; i++)
  {
    edge = Tess.PerEdgeNb[i];
    master = Tess.PerEdgeMaster[edge];
    edgesel[master] = ut_num_min (edgesel[master], edgesel[edge]);
  }

  for (i = 1; i <= Tess.PerEdgeQty; i++)
  {
    edge = Tess.PerEdgeNb[i];
    edgesel[edge] = edgesel[Tess.PerEdgeMaster[edge]];
  }

  return;
}

// Recording the slave vertices
void
net_reg_periodic_ver (struct TESS *pTess)
{
  int i;

  ut_free_1d_int (&(*pTess).PerVerNb);
  (*pTess).PerVerQty = 0;

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    if ((*pTess).VerState[i] == -1)
    {
      if ((*pTess).PerVerMaster[i] > 0 || (*pTess).PerVerSlaveQty[i] > 0)
        ut_print_neperbug ();
    }

    else if ((*pTess).PerVerMaster[i] > 0)
    {
      (*pTess).PerVerNb =
        ut_realloc_1d_int ((*pTess).PerVerNb, ++(*pTess).PerVerQty + 1);
      (*pTess).PerVerNb[(*pTess).PerVerQty] = i;
    }
  }

  return;
}

// Recomputing the edge periodicity from the vertex periodicity.  The
// master of an edge is its copy of lowest id, and the orientation of a
// slave edge is 1 if its first vertex is a copy of the first vertex of
// its master, -1 otherwise.
void
net_reg_periodic_edge (struct TESS *pTess)
{
  int i, j, master, qty, *edges = NULL, **offsets = NULL;
  int *done = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  int *shift = ut_alloc_1d_int (3);

  ut_free_1d_int (&(*pTess).PerEdgeNb);
  ut_free_1d_int (&(*pTess).PerEdgeMaster);
  ut_free_2d_int (&(*pTess).PerEdgeShift, (*pTess).EdgeQty + 1);
  ut_free_1d_int (&(*pTess).PerEdgeOri);
  ut_free_1d_int (&(*pTess).PerEdgeSlaveQty);
  ut_free_2d_int (&(*pTess).PerEdgeSlaveNb, (*pTess).EdgeQty + 1);

  (*pTess).PerEdgeQty = 0;
  (*pTess).PerEdgeMaster = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeShift = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 3);
  (*pTess).PerEdgeOri = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).EdgeState[i] == 0 && !done[i])
    {
      neut_tess_edge_copies (*pTess, i, &edges, &offsets, &qty);

      master = ut_array_1d_int_min (edges, qty);
      j = ut_array_1d_int_eltpos (edges, qty, master);
      ut_array_1d_int_memcpy (offsets[j], 3, shift);

      for (j = 0; j < qty; j++)
      {
        done[edges[j]] = 1;

        if (edges[j] != master)
        {
          (*pTess).PerEdgeMaster[edges[j]] = master;
          ut_array_1d_int_sub (shift, offsets[j], 3,
                               (*pTess).PerEdgeShift[edges[j]]);

          (*pTess).PerEdgeOri[edges[j]] =
            neut_tess_ver_iscopy (*pTess, (*pTess).EdgeVerNb[edges[j]][0],
                                  (*pTess).EdgeVerNb[master][0],
                                  (*pTess).PerEdgeShift[edges[j]]) ? 1 : -1;

          (*pTess).PerEdgeNb =
            ut_realloc_1d_int ((*pTess).PerEdgeNb, ++(*pTess).PerEdgeQty + 1);
          (*pTess).PerEdgeNb[(*pTess).PerEdgeQty] = edges[j];
        }
      }

      ut_free_1d_int (&edges);
      ut_free_2d_int (&offsets, qty);
    }

  ut_array_1d_int_sort ((*pTess).PerEdgeNb + 1, (*pTess).PerEdgeQty);

  neut_tess_init_edgeslave (pTess);

  ut_free_1d_int (&done);
  ut_free_1d_int (&shift);

  return;
}

// Removing the deleted faces from the face periodicity
void
net_reg_periodic_face (struct TESS *pTess)
{
  int i, face, master, qty;

  qty = 0;
  for (i = 1; i <= (*pTess).PerFaceQty; i++)
  {
    face = (*pTess).PerFaceNb[i];
    master = (*pTess).PerFaceMaster[face];

    if (((*pTess).FaceState[face] == -1) !=
        ((*pTess).FaceState[master] == -1))
      ut_print_neperbug ();

    if ((*pTess).FaceState[face] == -1)
    {
      (*pTess).PerFaceMaster[face] = 0;
      ut_array_1d_int_zero ((*pTess).PerFaceShift[face], 3);
      (*pTess).PerFaceOri[face] = 0;
    }
    else
      (*pTess).PerFaceNb[++qty] = face;
  }

  (*pTess).PerFaceQty = qty;

  ut_free_1d_int (&(*pTess).PerFaceSlaveNb);
  neut_tess_init_faceslave (pTess);

  return;
}
