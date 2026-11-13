/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"

#ifdef HAVE_LIBSCOTCH

void
nem_partition_elts_direct (struct MESH *Mesh, struct PART *pPart)
{
  int i, part, elt, dim = neut_mesh_array_dim (Mesh);
  int eltnodeqty =
    neut_elt_nodeqty ("tri", Mesh[dim].Dimension, Mesh[dim].EltOrder);
  int *eltnodepartqty = NULL, **eltnodeparts = NULL, **eltnodepartnodeqty = NULL;
  int **nodeparteltqty = NULL, ***nodepartelts = NULL;
  int *availqty = ut_alloc_1d_int ((*pPart).qty);

  nem_partition_elts_direct_pre (Mesh, *pPart, &eltnodepartqty, &eltnodeparts,
                                 &eltnodepartnodeqty, &nodepartelts,
                                 &nodeparteltqty);

  nem_partition_elts_direct_recordinteriorelts (pPart, eltnodeqty,
                                                nodepartelts, nodeparteltqty);

  // number of elements available to each partition (i.e. that have nodes
  // in the partition), updated as elements are recorded
  for (i = 0; i < (*pPart).qty; i++)
    availqty[i] = ut_array_1d_int_sum (nodeparteltqty[i] + 1, eltnodeqty);

  while (!nem_partition_elts_direct_findelt (Mesh[dim], *pPart, eltnodeparts, eltnodepartqty,
                                             eltnodepartnodeqty,
                                             nodepartelts, nodeparteltqty,
                                             availqty, eltnodeqty, &part, &elt))
    nem_partition_elts_direct_recordelt (pPart, part, elt, eltnodepartqty,
                                         eltnodeparts, eltnodepartnodeqty,
                                         nodepartelts, nodeparteltqty,
                                         availqty);

  ut_free_1d_int (&availqty);
  ut_free_2d_int (&nodeparteltqty, (*pPart).qty);
  ut_free_3d_int (&nodepartelts, (*pPart).qty, eltnodeqty + 1);
  ut_free_1d_int (&eltnodepartqty);
  ut_free_2d_int (&eltnodeparts, Mesh[dim].EltQty + 1);
  ut_free_2d_int (&eltnodepartnodeqty, Mesh[dim].EltQty + 1);

  return;
}

#endif /* HAVE_LIBSCOTCH */
