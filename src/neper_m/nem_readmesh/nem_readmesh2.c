/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nem_readmesh_.h"

// Recording the nsets of a mesh file, NSetIn, into NSet[0], NSet[1] and
// NSet[2], from their names.  The nsets of a mesh of dimension dim are named
// after the domain faces (dim = 3) or edges (dim = 2): an nset whose name is
// made of k domain face labels (e.g. x0, x0y0 and x0y0z0 for k = 1, 2 and 3)
// is recorded in NSet[dim - k], with the domain face labels being the names
// that are not made of other names, and an optional "body" suffix.  The other
// nsets (custom nsets) are recorded in NSet[0].  If no nset of a lower level is
// present (e.g. only face nsets), the lower-level nsets are made from the
// intersections of the higher-level nsets, as for a tessellation.
void
nem_readmesh_nsets (struct NSET NSetIn, int dim, struct NSET *NSet)
{
  int i, j, k, id, null, labelqty = 0;
  int *levelqty = ut_alloc_1d_int (4);
  char **names = ut_alloc_1d_pchar (NSetIn.qty + 1);
  char **labels = ut_alloc_1d_pchar (NSetIn.qty + 1);

  // names without the body suffix
  for (i = 1; i <= NSetIn.qty; i++)
  {
    ut_string_string (NSetIn.names[i], names + i);
    if (strlen (names[i]) > 4 && !strcmp (names[i] + strlen (names[i]) - 4, "body"))
      names[i][strlen (names[i]) - 4] = '\0';
  }

  // domain face labels
  for (i = 1; i <= NSetIn.qty; i++)
    if (nem_readmesh_nsets_split (names[i], names + 1, NSetIn.qty, i - 1) == -1)
      ut_string_string (names[i], labels + labelqty++);

  for (i = 1; i <= NSetIn.qty; i++)
  {
    k = nem_readmesh_nsets_split (names[i], labels, labelqty, -1);
    j = (k >= 1 && k <= dim) ? dim - k : 0;

    // non-body nsets of each level (names[i] is NSetIn.names[i] without "body")
    if (k >= 1 && k <= dim && !strcmp (names[i], NSetIn.names[i]))
      levelqty[k]++;

    // indexed 1...qty; entry 0 must be nulled on first allocation
    null = (NSet[j].qty == 0) ? 2 : 1;
    id = ++NSet[j].qty;
    NSet[j].names = ut_realloc_1d_pchar_null (NSet[j].names, id + 1, null);
    NSet[j].nodes = ut_realloc_1d_pint_null (NSet[j].nodes, id + 1, null);
    NSet[j].NodeQty = ut_realloc_1d_int (NSet[j].NodeQty, id + 1);

    ut_string_string (NSetIn.names[i], NSet[j].names + id);
    NSet[j].NodeQty[id] = NSetIn.NodeQty[i];
    NSet[j].nodes[id] = ut_alloc_1d_int (NSetIn.NodeQty[i]);
    ut_array_1d_int_memcpy (NSetIn.nodes[i], NSetIn.NodeQty[i], NSet[j].nodes[id]);
  }

  // missing lower-level nsets
  if (dim >= 2 && levelqty[1] > 0)
    nem_readmesh_nsets_inter (NSet, dim, levelqty[2] == 0, dim == 3 && levelqty[3] == 0);

  ut_free_2d_char (&names, NSetIn.qty + 1);
  ut_free_2d_char (&labels, NSetIn.qty + 1);
  ut_free_1d_int (&levelqty);

  return;
}

// Number of labels (of labels[0...labelqty - 1], but label exclude) that make up
// name, or -1 if name is not made of labels
int
nem_readmesh_nsets_split (char *name, char **labels, int labelqty, int exclude)
{
  int i, qty, len;

  if (strlen (name) == 0)
    return 0;

  for (i = 0; i < labelqty; i++)
  {
    if (i == exclude)
      continue;

    len = strlen (labels[i]);
    if (len > 0 && !strncmp (name, labels[i], len))
    {
      qty = nem_readmesh_nsets_split (name + len, labels, labelqty, -1);
      if (qty != -1)
        return qty + 1;
    }
  }

  return -1;
}
