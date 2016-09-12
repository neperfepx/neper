/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport_.h"

void
nem_transport_mesh3d_parelt (struct TESS Tess, struct NODES OldNodes,
			     struct MESH *OldMesh,
			     struct NODES NewNodes, struct MESH *NewMesh,
			     int *oldelt)
{
  int i, elset3d, status;
  double *coo = ut_alloc_1d (3);
  struct MESH Facet;
  int *elts2d = ut_alloc_1d_int (5);
  double *eq = ut_alloc_1d (4);
  char *message = ut_alloc_1d_char (1000);

  Tess.PolyQty = Tess.PolyQty;
  OldMesh[2].EltQty = OldMesh[2].EltQty;

  neut_mesh_set_zero (&Facet);

  if (NewMesh[2].NodeElts == NULL)
    neut_mesh_init_nodeelts (&NewMesh[2], NewNodes.NodeQty);

  ut_print_message (0, 3, "Searching transport information... %3d%%", 0);

  // for each new elt, determining parent elt
  for (i = 1; i <= NewMesh[3].EltQty; i++)
  {
    // parent element = old element in which the new element centre falls.
    elset3d = NewMesh[3].EltElset[i];
    neut_mesh_elt_centre (NewNodes, NewMesh[3], i, coo);
    status = neut_mesh_elset_point_elt (OldMesh[3], OldNodes, elset3d,
					coo, &(oldelt[i]));

    // if it does not fall in any old element, picking the closest.
    if (status != 0)
      status = neut_mesh_elset_point_closestelt (OldMesh[3], OldNodes,
						 elset3d, coo, &(oldelt[i]));

    ut_print_progress (stdout, i, NewMesh[3].EltQty, "%3.0f%%", message);
  }

  ut_free_1d_int (elts2d);
  neut_mesh_free (&Facet);
  ut_free_1d (coo);
  ut_free_1d_char (message);
  ut_free_1d (eq);

  return;
}
