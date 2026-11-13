/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<ctype.h>

#include "nev_print_show_mesh_.h"

// Replaces each nset==<label> of expr by a variable, nset_<id>, where <id> is
// the node set id, and records in nsetnode[id][node] whether the nodes belong
// to the node set.  The label can be enclosed in double quotes, which is
// needed if it contains other characters than alphanumerics and underscores.
void
nev_print_show_mesh_nodes_nset (char *expr, struct NSET NSet, int nodeqty,
                                char **pexpr, int ***pnsetnode)
{
  int i, id, quoted, pos = 0;
  char *beg = expr, *start = NULL, *end = NULL, *prefix = "nset==";
  char *label = ut_alloc_1d_char (strlen (expr) + 1);

  // a variable is always shorter than twice the text it replaces
  (*pexpr) = ut_alloc_1d_char (2 * strlen (expr) + 1);
  (*pnsetnode) = ut_alloc_2d_int (NSet.qty + 1, nodeqty + 1);

  while ((start = strstr (beg, prefix)))
  {
    memcpy ((*pexpr) + pos, beg, start - beg);
    pos += start - beg;

    start += strlen (prefix);
    quoted = (*start == '"');
    start += quoted;

    if (quoted)
      end = strchr (start, '"');
    else
      for (end = start; isalnum ((unsigned char) *end) || *end == '_'; end++);

    if (!end || end == start)
      ut_print_message (2, 2, "Failed to process expression `%s'.\n", expr);

    memcpy (label, start, end - start);
    label[end - start] = '\0';

    id = 0;
    for (i = 1; i <= NSet.qty; i++)
      if (!strcmp (NSet.names[i], label))
      {
        id = i;
        break;
      }

    if (!id)
      ut_print_message (2, 2, "Node set `%s' not found.\n", label);

    for (i = 0; i < NSet.NodeQty[id]; i++)
      (*pnsetnode)[id][NSet.nodes[id][i]] = 1;

    pos += sprintf ((*pexpr) + pos, "nset_%d", id);
    beg = end + quoted;
  }

  strcpy ((*pexpr) + pos, beg);

  ut_free_1d_char (&label);

  return;
}
