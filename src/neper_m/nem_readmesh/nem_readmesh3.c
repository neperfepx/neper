/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nem_readmesh_.h"

// Making the nsets of the intersections of 2 (if pair) and of 3 (if triple)
// nsets of NSet[dim - 1] (non-body nsets), which are recorded in NSet[dim - 2]
// and NSet[dim - 3], respectively.  Empty intersections are skipped.
void
nem_readmesh_nsets_inter (struct NSET *NSet, int dim, int pair, int triple)
{
  int i, j, k, l, maxnode = 0, qty, *nodes = NULL, *mask = NULL;
  int *ids = NULL, idqty = 0;
  char *name = NULL;
  struct NSET *pNSetF = NSet + dim - 1;

  for (i = 1; i <= (*pNSetF).qty; i++)
    if (!strstr ((*pNSetF).names[i], "body"))
    {
      ut_array_1d_int_list_addval_nocheck (&ids, &idqty, i);
      if ((*pNSetF).NodeQty[i] > 0)
        maxnode = ut_num_max_int (maxnode,
                              ut_array_1d_int_max ((*pNSetF).nodes[i], (*pNSetF).NodeQty[i]));
    }

  // mask[node] = number of the nsets of the current intersection containing node
  mask = ut_alloc_1d_int (maxnode + 1);

  for (i = 0; i < idqty; i++)
  {
    nem_readmesh_nsets_inter_mask (*pNSetF, ids[i], mask, 1);

    for (j = i + 1; j < idqty; j++)
    {
      nem_readmesh_nsets_inter_mask (*pNSetF, ids[j], mask, 1);

      if (pair)
      {
        nem_readmesh_nsets_inter_nodes (*pNSetF, ids[j], mask, 2, &nodes, &qty);
        if (qty > 0)
        {
          int set[2] = { ids[i], ids[j] };
          nem_readmesh_nsets_inter_name (*pNSetF, set, 2, &name);
          nem_readmesh_nsets_add (NSet + dim - 2, name, nodes, qty);
        }
      }

      if (triple)
        for (k = j + 1; k < idqty; k++)
        {
          nem_readmesh_nsets_inter_mask (*pNSetF, ids[k], mask, 1);

          nem_readmesh_nsets_inter_nodes (*pNSetF, ids[k], mask, 3, &nodes, &qty);
          if (qty > 0)
          {
            int set[3] = { ids[i], ids[j], ids[k] };
            nem_readmesh_nsets_inter_name (*pNSetF, set, 3, &name);
            nem_readmesh_nsets_add (NSet + dim - 3, name, nodes, qty);
          }

          nem_readmesh_nsets_inter_mask (*pNSetF, ids[k], mask, -1);
        }

      nem_readmesh_nsets_inter_mask (*pNSetF, ids[j], mask, -1);
    }

    nem_readmesh_nsets_inter_mask (*pNSetF, ids[i], mask, -1);
  }

  for (l = 0; l <= maxnode; l++)
    if (mask[l])
      ut_print_neperbug ();

  ut_free_1d_int (&ids);
  ut_free_1d_int (&mask);
  ut_free_1d_int (&nodes);
  ut_free_1d_char (&name);

  return;
}

// Name of the intersection of nsets ids: the nset names, sorted
// alphabetically and pasted (as for the domain edges and vertices of a
// tessellation)
void
nem_readmesh_nsets_inter_name (struct NSET NSet, int *ids, int idqty, char **pname)
{
  int i;
  char **names = ut_alloc_1d_pchar (idqty);

  for (i = 0; i < idqty; i++)
    ut_string_string (NSet.names[ids[i]], names + i);

  ut_array_1d_pchar_sort (names, idqty);

  ut_free_1d_char (pname);
  (*pname) = ut_string_array_paste (names, idqty, NULL);

  ut_free_2d_char (&names, idqty);

  return;
}

// Adding val to the mask values of the nodes of nset id
void
nem_readmesh_nsets_inter_mask (struct NSET NSet, int id, int *mask, int val)
{
  int i;

  for (i = 0; i < NSet.NodeQty[id]; i++)
    mask[NSet.nodes[id][i]] += val;

  return;
}

// Nodes of nset id whose mask value is val
void
nem_readmesh_nsets_inter_nodes (struct NSET NSet, int id, int *mask, int val,
                                int **pnodes, int *pqty)
{
  int i;

  (*pqty) = 0;
  (*pnodes) = ut_realloc_1d_int (*pnodes, NSet.NodeQty[id] + 1);

  for (i = 0; i < NSet.NodeQty[id]; i++)
    if (mask[NSet.nodes[id][i]] == val)
      (*pnodes)[(*pqty)++] = NSet.nodes[id][i];

  return;
}

// Adding an nset (name and nodes) to *pNSet
void
nem_readmesh_nsets_add (struct NSET *pNSet, char *name, int *nodes, int qty)
{
  int id, null;

  // indexed 1...qty; entry 0 must be nulled on first allocation
  null = ((*pNSet).qty == 0) ? 2 : 1;
  id = ++(*pNSet).qty;
  (*pNSet).names = ut_realloc_1d_pchar_null ((*pNSet).names, id + 1, null);
  (*pNSet).nodes = ut_realloc_1d_pint_null ((*pNSet).nodes, id + 1, null);
  (*pNSet).NodeQty = ut_realloc_1d_int ((*pNSet).NodeQty, id + 1);

  ut_string_string (name, (*pNSet).names + id);
  (*pNSet).NodeQty[id] = qty;
  (*pNSet).nodes[id] = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (nodes, qty, (*pNSet).nodes[id]);

  return;
}
