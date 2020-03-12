/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"
#ifdef HAVE_LIBSCOTCH

/* Some things are recalculated from scratch each time.
 * It could be possible to speed things up.
 */
void
nem_partition_elts_balancing_Q_elt (struct MESH Mesh, struct PART Part,
                                    int *A, int *Q)
{
  int partqty;
  int elt, eltpart, j, neighpartqty, srcpart, destpart;
  int eltnodeqty = neut_elt_nodeqty ("tri", Mesh.Dimension, Mesh.EltOrder);
  int *neighpart = ut_alloc_1d_int (eltnodeqty);
  int **coms = ut_alloc_2d_int (Part.qty, 2);
  int srccom, destcom;

  elt = Q[0];
  eltpart = Part.elt_parts[elt];

  /* determining neighbouring partitions */
  for (j = 0; j < eltnodeqty; j++)
    neighpart[j] = Part.node_parts[Mesh.EltNodes[elt][j]];

  ut_array_1d_int_sort_uniq (neighpart, eltnodeqty, &neighpartqty);
  if (ut_array_1d_int_rmelt (neighpart, neighpartqty, eltpart, 1) != 1)
    ut_print_neperbug ();

  neighpartqty--;

  /* looking for the better partition to go into */
  neut_mesh_elt_coms (Mesh, elt, Part.qty, Part.node_parts, Part.elt_parts,
                      coms, &partqty);

  srcpart = Part.elt_parts[elt];
  srccom = neut_mesh_elt_comqty (Mesh, elt, Part.node_parts, Part.elt_parts);
  destpart = coms[0][0];

  /* modif temporarily parttab2[elt] to compute comqty */
  Part.elt_parts[elt] = destpart;
  destcom = neut_mesh_elt_comqty (Mesh, elt, Part.node_parts, Part.elt_parts);
  Part.elt_parts[elt] = srcpart;

  Q[1] = destpart;

  /* -1 forbid the moves without gain in element equilibrium.
     This could be restrictive. */
  if (A[destpart] < A[srcpart] - 1)
    Q[2] = 1000000 * (10 - (destcom - srccom)) + (A[srcpart] - A[destpart]);
  else if (A[destpart] < A[srcpart])
    Q[2] = 10000 * (10 - (destcom - srccom)) + (A[srcpart] - A[destpart]);
  else
    Q[2] = 0;

  ut_free_1d_int (&neighpart);
  ut_free_2d_int (&coms, Part.qty);

  return;
}
#endif
