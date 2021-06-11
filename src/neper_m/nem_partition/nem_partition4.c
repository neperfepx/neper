/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"

#ifdef HAVE_LIBSCOTCH

void
nem_partition_elts_direct_pre (struct MESH *Mesh, struct PART Part,
                               int **peltnodepartqty,
                               int ***peltnodeparts,
                               int ***peltnodepartnodeqty,
                               int ****pnodepartelts,
                               int ***pnodeparteltqty)
{
  int i, j, node, nodepart, dim = neut_mesh_array_dim (Mesh);
  int nodepartnodeqty, *eltnodepartsinv = NULL, *tmp = NULL;
  int eltnodeqty =
    neut_elt_nodeqty ("tri", Mesh[dim].Dimension, Mesh[dim].EltOrder);

  (*peltnodepartqty) = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
  (*peltnodeparts) = ut_alloc_2d_int (Mesh[dim].EltQty + 1, eltnodeqty);
  (*peltnodepartnodeqty) = ut_alloc_2d_int (Mesh[dim].EltQty + 1, eltnodeqty);
  (*pnodeparteltqty) = ut_alloc_2d_int (Part.qty, eltnodeqty + 1);
  (*pnodepartelts) = ut_alloc_2d_pint (Part.qty, eltnodeqty + 1);

  for (i = 1; i <= Mesh[dim].EltQty; i++)
  {
    (*peltnodepartqty)[i] = eltnodeqty;
    for (j = 0; j < eltnodeqty; j++)
      (*peltnodeparts)[i][j] = Part.node_parts[Mesh[dim].EltNodes[i][j]];

    ut_array_1d_int_sort_uniq ((*peltnodeparts)[i], (*peltnodepartqty)[i], (*peltnodepartqty) + i);

    ut_array_1d_int_inv ((*peltnodeparts)[i], (*peltnodepartqty)[i], &eltnodepartsinv, NULL);

    for (j = 0; j < eltnodeqty; j++)
    {
      node = Mesh[dim].EltNodes[i][j];
      (*peltnodepartnodeqty)[i][eltnodepartsinv[Part.node_parts[node]]]++;
    }

    tmp = ut_alloc_1d_int ((*peltnodepartqty)[i]);

    ut_array_1d_int_sort_des_index ((*peltnodepartnodeqty)[i], (*peltnodepartqty)[i], tmp);
    ut_array_1d_int_permute ((*peltnodepartnodeqty)[i], (*peltnodepartqty)[i], tmp);
    ut_array_1d_int_permute ((*peltnodeparts)[i], (*peltnodepartqty)[i], tmp);

    ut_free_1d_int (&tmp);
  }

  for (i = 1; i <= Mesh[dim].EltQty; i++)
    for (j = 0; j < (*peltnodepartqty)[i]; j++)
    {
      nodepart = (*peltnodeparts)[i][j];
      nodepartnodeqty = (*peltnodepartnodeqty)[i][j];
      ut_array_1d_int_list_addval_nocheck ((*pnodepartelts)[nodepart] + nodepartnodeqty,
                                           (*pnodeparteltqty)[nodepart] + nodepartnodeqty,
                                           i);
    }

  ut_free_1d_int (&eltnodepartsinv);

  return;
}

void
nem_partition_elts_direct_recordinteriorelts (struct PART *pPart, int eltnodeqty,
                                              int ***nodepartelts, int **nodeparteltqty)
{
  int i;

#pragma omp parallel for private(i)
  for (i = 0; i < (*pPart).qty; i++)
  {
    int j, elt;
    for (j = 0; j < nodeparteltqty[i][eltnodeqty]; j++)
    {
      elt = nodepartelts[i][eltnodeqty][j];

      (*pPart).elt_parts[elt] = i;
      (*pPart).EltQty[i]++;
    }

    nodeparteltqty[i][eltnodeqty] = 0;
  }

  return;
}

int
nem_partition_elts_direct_findelt (struct MESH Mesh, struct PART Part,
                                   int **eltnodeparts, int *eltnodepartqty,
                                   int **eltnodepartnodeqty,
                                   int ***nodepartelts, int **nodeparteltqty,
                                   int eltnodeqty, int *ppart, int *pelt)
{
  int i, j, k, part, elt, eltqty, pos, missing, avail;
  int *elts = NULL, *tmp = ut_alloc_1d_int (Part.qty);
  double *vals = NULL, targeteltqty = Mesh.EltQty / Part.qty;

  // part: the one with the least number of elements

  ut_array_1d_int_memcpy (Part.EltQty, Part.qty, tmp);

  for (i = 0; i < Part.qty; i++)
    if (ut_array_1d_int_sum (nodeparteltqty[i] + 1, eltnodeqty) == 0)
      tmp[i] = INT_MAX;

  *ppart = ut_array_1d_int_min_index (tmp, Part.qty);

  ut_free_1d_int (&tmp);

  // elt in part

  (*pelt) = 0;

  eltqty = ut_array_1d_int_sum (nodeparteltqty[*ppart] + 1, eltnodeqty);

  if (eltqty == 0)
    return -1;

  elts = ut_alloc_1d_int (eltqty);
  vals = ut_alloc_1d (eltqty);

  pos = 0;
  for (i = eltnodeqty; i >= 1; i--)
    for (j = 0; j < nodeparteltqty[*ppart][i]; j++)
    {
      elt = nodepartelts[*ppart][i][j];

      for (k = 0; k < eltnodepartqty[elt]; k++)
      {
        part = eltnodeparts[elt][k];
        missing = targeteltqty - Part.EltQty[part];
        avail = ut_array_1d_int_sum (nodeparteltqty[part] + 1, eltnodeqty);

        if (part != *ppart)
          vals[pos] += (avail <= missing) ? DBL_MAX : eltnodepartnodeqty[elt][k] / pow (avail - missing, 2);
      }
      vals[pos] /= i;

      elts[pos] = elt;

      pos++;
    }

  *pelt = elts[ut_array_1d_min_index (vals, eltqty)];

  ut_free_1d_int (&elts);
  ut_free_1d (&vals);

  return *pelt ? 0 : -1;
}

void
nem_partition_elts_direct_recordelt (struct PART *pPart, int part, int elt,
                                     int *eltnodepartqty, int **eltnodeparts,
                                     int **eltnodepartnodeqty,
                                     int ***nodepartelts, int **nodeparteltqty)
{
  int i, status, nodeqty;

  // assigning elt to partition
  if ((*pPart).elt_parts[elt] != -1)
    abort ();

  (*pPart).elt_parts[elt] = part;
  (*pPart).EltQty[part]++;

  // removing elt from nodepart*
  for (i = 0; i < eltnodepartqty[elt]; i++)
  {
    part = eltnodeparts[elt][i];
    nodeqty = eltnodepartnodeqty[elt][i];
    status = ut_array_1d_int_rmelt (nodepartelts[part][nodeqty],
                                    nodeparteltqty[part][nodeqty], elt, 1);
    if (status != 1)
      abort ();
    nodeparteltqty[part][nodeqty]--;
  }
}

#endif /* HAVE_LIBSCOTCH */
