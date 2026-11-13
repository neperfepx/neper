/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_.h"

void
nem_interface (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
               struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int nodeqty, *intertypes = NULL;
  int **nodeseeds = NULL;

  nem_interface_intertypes (In, Tess, &intertypes);

  // seeds of the nodes (after duplication), [1...NodeQty][0...]
  nodeseeds = ut_alloc_1d_pint ((*pNodes).NodeQty + 1);

  if (ut_list_testelt (In.interface, NEUT_SEP_DEP, "discontinuous")
      || ut_list_testelt (In.interface, NEUT_SEP_DEP, "cohesive"))
  {
    ut_print_message (0, 3, "Making mesh discontinuous...\n");
    nem_interface_dup (Tess, intertypes, pNodes, Mesh, pBound, &nodeseeds);
  }

  if (ut_list_testelt (In.interface, NEUT_SEP_DEP, "cohesive"))
  {
    ut_print_message (0, 3, "Adding cohesive elements...\n");
    nem_interface_cohesive (Tess, intertypes, *pNodes, nodeseeds, Mesh, *pBound);
  }

  nodeqty = (*pNodes).NodeQty;
  ut_free_2d_int (&nodeseeds, nodeqty + 1);
  ut_free_1d_int (&intertypes);

  return;
}
