/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_cohesive_.h"

void
nem_interface_cohesive (struct TESS Tess, int *intertypes, struct NODES Nodes,
                        int **nodeseeds, struct MESH *Mesh, struct BOUNDARY Bound)
{
  int i, j, k, elt, facet, slaveseed, dim = Tess.Dim;
  int EltQty, *elts = NULL;
  int feltnodeqty, eltnodeqty, *nodes = NULL;

  Mesh[4].Dimension = dim;
  if (!strcmp (Mesh[dim].EltType, "tri"))
  {
    if (Tess.Dim == 3)
      ut_string_string ("triprism", &Mesh[4].EltType);
    else
      ut_string_string ("quad", &Mesh[4].EltType);
  }
  if (!strcmp (Mesh[dim].EltType, "quad"))
    ut_string_string ("quad", &Mesh[4].EltType);
  Mesh[4].EltOrder = Mesh[dim].EltOrder;

  feltnodeqty =
    neut_elt_nodeqty (Mesh[Tess.Dim].EltType, Mesh[Tess.Dim].Dimension - 1,
                      Mesh[Tess.Dim].EltOrder);
  eltnodeqty =
    neut_elt_nodeqty (Mesh[4].EltType, Mesh[4].Dimension, Mesh[4].EltOrder);
  nodes = ut_alloc_1d_int (eltnodeqty);

  // the cohesive elements are defined on the cohesive interfaces, and the
  // cohesive elements of a periodic interface on its master interface only
  for (i = 1; i <= Bound.BoundQty; i++)
    if (Bound.BoundDom[i][0] == -1 && intertypes[i] == NEM_INTERFACE_COHESIVE
        && !neut_tess_inter_isperslave (Tess, i))
    {
      EltQty = 0;
      elts = ut_alloc_1d_int (Bound.BoundEltQty[i]);
      for (j = 1; j <= Bound.BoundEltQty[i]; j++)
      {
        elt = Bound.BoundElts[i][0][j];
        facet = Bound.BoundEltFacets[i][0][j];
        slaveseed = Bound.BoundSeed[i][1];
        neut_elt_facet_nodes (Mesh[Tess.Dim].EltType, Tess.Dim,
                              Mesh[Tess.Dim].EltOrder,
                              Mesh[Tess.Dim].EltNodes[elt], facet, 1, nodes);

        for (k = 0; k < feltnodeqty; k++)
          nem_interface_dup_node_seed_copy (Nodes, nodeseeds, nodes[k], slaveseed,
                                            nodes + Tess.Dim + k);

        if (Mesh[4].Dimension == 2)
          ut_array_1d_int_switch (nodes, 2, 3);

        if (ut_array_1d_int_eltpos (nodes, eltnodeqty, -1) != -1)
          abort ();

        // skipping the elements of a closed interface (where it meets
        // continuous interfaces only)
        if ((Tess.Dim == 3 && ut_array_1d_int_equal (nodes, 3, nodes + 3, 3))
            || (Tess.Dim == 2 && nodes[0] == nodes[3] && nodes[1] == nodes[2]))
          continue;

        neut_mesh_addelt (Mesh + 4, nodes);
        elts[EltQty++] = Mesh[4].EltQty;
      }

      if (EltQty == 0)
      {
        ut_free_1d_int (&elts);
        continue;
      }

      neut_mesh_addelset (Mesh + 4, elts, EltQty);
      Mesh[4].ElsetId =
        ut_realloc_1d_int (Mesh[4].ElsetId, Mesh[4].ElsetQty + 1);
      Mesh[4].ElsetId[Mesh[4].ElsetQty] = Mesh[dim].ElsetQty + i;

      ut_free_1d_int (&elts);
    }

  neut_mesh_init_eltelset (Mesh + 4, NULL);

  // labeled by interface, as in the interface file
  Mesh[4].ElsetLabels = ut_alloc_2d_char (Mesh[4].ElsetQty + 1, 100);
  for (i = 1; i <= Mesh[4].ElsetQty; i++)
    sprintf (Mesh[4].ElsetLabels[i], "bound%d",
             Mesh[4].ElsetId[i] - Mesh[dim].ElsetQty);

  neut_mesh_init_nodeelts (Mesh + 4, Nodes.NodeQty);

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&elts);

  return;
}
