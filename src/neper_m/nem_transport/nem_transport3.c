/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport_.h"

void
nem_transport_elt_oldelt (struct NODES OldNodes,
			  struct MESH OldMesh, struct NODES NewNodes,
			  struct MESH NewMesh, int **poldelt)
{
  int i, elset3d, status;
  double *coo = ut_alloc_1d (3);
  struct MESH Facet;
  int *elts2d = ut_alloc_1d_int (5);
  double *eq = ut_alloc_1d (4);
  char *message = ut_alloc_1d_char (1000);

  (*poldelt) = ut_alloc_1d_int (OldMesh.EltQty + 1);

  neut_mesh_set_zero (&Facet);

  if (!NewMesh.NodeElts)
  {
    abort ();
    // neut_mesh_init_nodeelts (&NewMesh, NewNodes.NodeQty);
  }

  ut_print_message (0, 3, "Searching transport information... %3d%%", 0);
  strcpy (message, " 0%");

  // for each new elt, determining parent elt
  for (i = 1; i <= NewMesh.EltQty; i++)
  {
    // parent element = old element in which the new element centre falls.
    elset3d = NewMesh.EltElset[i];
    neut_mesh_elt_centre (NewNodes, NewMesh, i, coo);
    status = neut_mesh_elset_point_elt (OldMesh, OldNodes, elset3d,
					coo, &((*poldelt)[i]));

    // if it does not fall in any old element, picking the closest.
    if (status != 0)
      status = neut_mesh_elset_point_closestelt (OldMesh, OldNodes,
						 elset3d, coo, &((*poldelt)[i]));

    ut_print_progress (stdout, i, NewMesh.EltQty, "%3.0f%%", message);
  }

  ut_free_1d_int (elts2d);
  neut_mesh_free (&Facet);
  ut_free_1d (coo);
  ut_free_1d_char (message);
  ut_free_1d (eq);

  return;
}
