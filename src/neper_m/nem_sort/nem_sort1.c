/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_sort_.h"

void
nem_sort (struct IN_M In, struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  if (In.sortnode && strcmp (In.sortnode, "none"))
    nem_sort_nodes (In, Tess, pNodes, Mesh);

  if (In.sortelt && strcmp (In.sortelt, "none"))
    nem_sort_elts (In, Tess, pNodes, Mesh);

  return;
}
