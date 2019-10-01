/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport_.h"

void
nem_transport_elt_oldelt (struct NODES OldNodes,
			  struct MESH OldMesh, struct NODES NewNodes,
			  struct MESH NewMesh, char *method, int **poldelt)
{
  int i, j;
  struct MESH Facet;
  int *elts2d = ut_alloc_1d_int (5);
  double *eq = ut_alloc_1d (4);

  (*poldelt) = ut_alloc_1d_int (NewMesh.EltQty + 1);

  neut_mesh_set_zero (&Facet);

  if (!NewMesh.NodeElts)
  {
    abort ();
    // neut_mesh_init_nodeelts (&NewMesh, NewNodes.NodeQty);
  }

  ut_print_message (0, 3, "Searching transport information...\n");

  int done = 0;

  // for each new elt, determining parent elt
#pragma omp parallel for private(i,j) schedule(dynamic)
  for (i = 1; i <= NewMesh.ElsetQty; i++)
  {
    double **coos = ut_alloc_2d (NewMesh.Elsets[i][0], 3);
    int *ids = ut_alloc_1d_int (NewMesh.Elsets[i][0]);

    for (j = 0; j < NewMesh.Elsets[i][0]; j++)
      neut_mesh_elt_centre (NewNodes, NewMesh, NewMesh.Elsets[i][j + 1], coos[j]);

    // parent element = old element in which the new element centre falls.
    /*
    status = neut_mesh_elset_point_elt (OldMesh, OldNodes, elset3d,
					coo, &((*poldelt)[i]));

    // if it does not fall in any old element, picking the closest.
    if (status != 0)
    */

    neut_mesh_elset_points_closestelts (OldMesh, OldNodes, i, coos,
                                        NewMesh.Elsets[i][0], method, ids);

    for (j = 0; j < NewMesh.Elsets[i][0]; j++)
      (*poldelt)[NewMesh.Elsets[i][j + 1]] = ids[j];

#pragma omp critical
    done++;

    ut_free_2d (coos, NewMesh.Elsets[i][0]);
    ut_free_1d_int (ids);
  }

  ut_free_1d_int (elts2d);
  neut_mesh_free (&Facet);
  ut_free_1d (eq);

  return;
}
