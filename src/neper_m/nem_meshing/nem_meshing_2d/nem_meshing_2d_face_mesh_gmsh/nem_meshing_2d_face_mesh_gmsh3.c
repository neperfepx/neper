/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_face_mesh_gmsh_.h"

// This function should not be necessary
void
nem_meshing_2d_face_mesh_gmsh_backproj_fixboundary (struct NODES Nodes,
                                                    int *bnodes, int *lbnodes,
                                                    int bnodeqty,
                                                    struct NODES *pN)
{
  int i;

  for (i = 0; i < bnodeqty; i++)
  {
    if (bnodeqty > (*pN).NodeQty)
    {
      printf ("bnodeqty = %d (*pN).NodeQty = %d\n", bnodeqty, (*pN).NodeQty);
      ut_print_neperbug ();
    }

    if (lbnodes[i] <= 0 || lbnodes[i] > (*pN).NodeQty)
    {
      printf ("lbnodes[i] = %d (*pN).NodeQty = %d\n", lbnodes[i],
              (*pN).NodeQty);
      ut_print_neperbug ();
    }

    if (bnodes[i] <= 0 || bnodes[i] > Nodes.NodeQty)
    {
      printf ("bnodes[i] = %d (*pN).NodeQty = %d\n", bnodes[i],
              (*pN).NodeQty);
      ut_print_neperbug ();
    }

    ut_array_1d_memcpy (Nodes.NodeCoo[bnodes[i]], 3,
                        (*pN).NodeCoo[lbnodes[i]]);
  }

  return;
}
