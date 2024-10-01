/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_elsets_.h"

void
nem_elsets (struct IN_M In, struct TESS *pTess, struct NODES Nodes,
            struct MESH *Mesh)
{
  int i, qty, *qty2 = NULL;
  char ***parts = NULL;
  double *val = ut_alloc_1d (Mesh[3].EltQty + 1);
  int *elts = NULL, EltQty;

  ut_list_break2 (In.elset, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty2,
                  &qty);

  for (i = 0; i < qty; i++)
  {
    ut_print_message (0, 2, "%s...\n", parts[i][0]);

    if (!strcmp (parts[i][0], "default") || !strcmp (parts[i][0], "polys"))
      continue;

    if (qty2[i] != 2)
      ut_print_message (2, 2, "Failed to process expression `%s'.", In.elset);

    neut_mesh_entity_expr_matches (pTess, Nodes, Mesh, "elt3d", parts[i][1],
                                   &elts, &EltQty);

    neut_mesh_addelset (Mesh + 3, elts, EltQty);
    Mesh[3].ElsetLabels =
      ut_realloc_1d_pchar_null (Mesh[3].ElsetLabels, Mesh[3].ElsetQty + 1, 1);
    ut_string_string (parts[i][0], Mesh[3].ElsetLabels + Mesh[3].ElsetQty);

    Mesh[3].CustomElsetQty++;

    ut_free_1d_int (&elts);
  }

  // ut_free_2d_char (&parts, PartQty);
  ut_free_1d_int (&qty2);
  ut_free_1d (&val);

  return;
}
