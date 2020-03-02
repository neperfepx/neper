/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_pinching_.h"

void
nem_meshing_pinching_fix_eltfacets (struct MESH *Mesh, int elt,
                                    int **peltfacets, int *peltfacetqty)
{
  int elset;

  elset = Mesh[2].EltElset[elt];

  neut_mesh_elset_elt_insidefacets (Mesh[1], Mesh[2], elset, elt, peltfacets,
                                    peltfacetqty);

  return;
}

void
nem_meshing_pinching_fix_split (struct NODES *pNodes, struct MESH *Mesh,
                                int elt, int eltfacet, int *pnewnode)
{
  int i, comelt, newelt, elset = Mesh[2].EltElset[elt];
  double cl, *bary = ut_alloc_1d (3);
  int *comelts = NULL, comeltqty;
  int *nodes = ut_alloc_1d_int (2);

  neut_elt_facet_nodes ("tri", 2, 1, Mesh[2].EltNodes[elt], eltfacet, 1,
                        nodes);

  neut_mesh_nodes_elsets_comelts (Mesh[2], nodes, 2, &elset, 1, &comelts,
                                  &comeltqty);
  if (comeltqty != 2)
    ut_print_neperbug ();

  neut_nodes_bary (*pNodes, nodes, 2, bary, &cl);

  neut_nodes_addnode (pNodes, bary, cl);
  *pnewnode = (*pNodes).NodeQty;

  for (i = 0; i < 2; i++)
  {
    comelt = comelts[i];

    neut_mesh_addelt (Mesh + 2, Mesh[2].EltNodes[comelt]);
    newelt = Mesh[2].EltQty;
    neut_mesh_elset_addelt (Mesh + 2, elset, newelt);
    Mesh[2].EltElset[newelt] = elset;
    ut_array_1d_int_findnreplace (Mesh[2].EltNodes[comelt], 3, nodes[0],
                                  *pnewnode);
    ut_array_1d_int_findnreplace (Mesh[2].EltNodes[newelt], 3, nodes[1],
                                  *pnewnode);
  }

  // we could update only a few nodes instead of running
  // neut_mesh_init_nodeelts, but it's fast enough
  neut_mesh_init_nodeelts (Mesh + 2, (*pNodes).NodeQty);

  ut_free_1d (&bary);
  ut_free_1d_int (&comelts);
  ut_free_1d_int (&nodes);

  return;
}

void
nem_meshing_pinching_fix_proj (struct MESHPARA MeshPara, struct TESS *pTess,
                               struct NODES RNodes, struct MESH *RMesh,
                               struct NODES *pNodes, int elset, int newnode)
{
  struct NODES Nint;
  struct MESH Mint;

  neut_nodes_set_zero (&Nint);
  neut_mesh_set_zero (&Mint);

  // Projecting new node in case of tessellation
  if (RNodes.NodeQty == 0)
  {
    if ((*pTess).FacePt[elset] != 0)
      ut_print_neperbug ();

    neut_tess_face_interpolmesh (*pTess, elset, &Nint, &Mint);

    neut_node_proj_alongontomesh ((*pNodes).NodeCoo[newnode],
                                  MeshPara.face_eq[elset] + 1, Nint, Mint, 1);
  }

  // Projecting new node in case of remeshing
  else
    neut_node_proj_alongontomesh ((*pNodes).NodeCoo[newnode],
                                  MeshPara.face_eq[elset] + 1, RNodes,
                                  RMesh[2], elset);

  neut_nodes_free (&Nint);
  neut_mesh_free (&Mint);

  return;
}
