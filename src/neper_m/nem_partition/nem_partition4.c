/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"
#ifdef HAVE_LIBSCOTCH

int
nem_partition_elts_balancing_Q (struct MESH Mesh, struct PART *pPart,
                                double level, int BoundEltQty, int **Q)
{
  int i, j, prevsum, sum, iter, oldpart, newpart;
  int *A = ut_alloc_1d_int ((*pPart).qty);
  int *localQ = ut_alloc_1d_int ((*pPart).qty);
  int val, qty;
  int inisum;
  double *tmp = NULL;
  int *id = NULL;

  srand48 (1);
  /* Initializing stuff */
  /* A is the distance to the target number of elements, to be zero'd. */

  /* initializing */
  for (i = 0; i < (*pPart).qty; i++)
    A[i] = (*pPart).eltqty[i] - (*pPart).targeteltqty[i];

  for (i = 0; i < BoundEltQty; i++)
    nem_partition_elts_balancing_Q_elt (Mesh, *pPart, A, Q[i]);

  ut_array_2d_int_sortbycol_des (Q, BoundEltQty, 3, 2);

  inisum = ut_array_1d_int_abssum (A, (*pPart).qty);
  sum = inisum;
  prevsum = sum;

  char *progress = ut_alloc_1d_char (10);
  ut_print_progress (stdout, 0, inisum, "%2.0f", progress);
  iter = 0;
  while (sum != 0 && iter < 100000 * (double) sum / inisum)
  {
    val = Q[0][2];
    qty = 0;
    for (i = 0; i < BoundEltQty && Q[i][2] == val; i++)
    {
      if (Q[i][2] == val)
        qty++;
      else
        break;
    }

    tmp = ut_alloc_1d (qty);

    for (i = 0; i < qty; i++)
      tmp[i] = drand48 ();

    id = ut_alloc_1d_int (qty);
    ut_array_1d_sort_index (tmp, qty, id);
    ut_free_1d (&tmp);

    ut_array_2d_int_sortbycol_des (Q, BoundEltQty, 3, 2);

    for (i = 0; i < qty; i++)
    {
      val = id[i];

      ut_array_2d_int_switchlines (Q, 3, 0, val);

      (*pPart).eltqty[(*pPart).elt_parts[Q[0][0]]]--;
      A[(*pPart).elt_parts[Q[0][0]]]--;
      (*pPart).elt_parts[Q[0][0]] = Q[0][1];
      (*pPart).eltqty[(*pPart).elt_parts[Q[0][0]]]++;
      A[(*pPart).elt_parts[Q[0][0]]]++;

      oldpart = Q[0][1];
      nem_partition_elts_balancing_Q_elt (Mesh, *pPart, A, Q[0]);
      newpart = Q[0][1];

      sum = ut_array_1d_int_abssum (A, (*pPart).qty);

      if (sum < prevsum)
      {
        prevsum = sum;

        for (j = 0; j < BoundEltQty; j++)
          if (Q[j][2] == oldpart || Q[j][2] == newpart)
            nem_partition_elts_balancing_Q_elt (Mesh, *pPart, A, Q[j]);
        iter = -1;

        ut_print_progress (stdout, inisum - sum, inisum, "%2.0f", progress);

        break;
      }
    }

    if ((double) (inisum - sum) / (double) inisum > level)
      break;

    iter++;
    ut_free_1d_int (&id);
  }
  ut_free_1d_char (&progress);
  if (sum != 0)
    printf ("\n");

  ut_free_1d_int (&A);
  ut_free_1d_int (&localQ);

  return sum;
}
#endif
