/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_pinching_.h"

int
nem_meshing_pinching_testpoly (struct IN_M In, struct TESS Tess,
                               struct MESH *Mesh, int poly)
{
  int i, face, status, varqty = 1;
  char **vars = ut_alloc_1d_pchar (varqty);
  double *vals = ut_alloc_1d (varqty);

  ut_string_string ("id", vars);

  vals[0] = poly;
  if (strcmp (In.meshpoly, "all"))
    ut_math_eval_int (In.meshpoly, varqty, vars, vals, &status);
  else
    status = 1;

  if (status)
    for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
    {
      face = Tess.PolyFaceNb[poly][i];

      if (face > Mesh[2].ElsetQty || Mesh[2].Elsets[face][0] == 0)
      {
        status = 0;
        break;
      }
    }

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return (status == 1) ? 0 : -1;
}

void
nem_meshing_pinching_inter (struct MESHPARA MeshPara, struct TESS *pTess,
                            struct NODES *pNodes, struct MESH *Mesh,
                            int elset)
{
  int i;
  struct NODES Nint;
  struct MESH Mint;
  int *bodynodes = NULL, bodynodeqty;

  neut_nodes_set_zero (&Nint);
  neut_mesh_set_zero (&Mint);

  // if case of a tessellation (not remeshing), we may re-define
  // the interpolation, so re-projecting all interior nodes (to make it simple)
  (*pTess).FacePt[elset] = 0;
  neut_tess_face_centre (*pTess, elset, (*pTess).FacePtCoo[elset]);

  neut_tess_face_interpolmesh (*pTess, elset, &Nint, &Mint);

  neut_mesh_elset_bodynodes_2d (*pTess, Mesh[1], Mesh[2], elset, &bodynodes, &bodynodeqty);

  for (i = 0; i < bodynodeqty; i++)
    neut_node_proj_alongontomesh ((*pNodes).NodeCoo[bodynodes[i]],
                                  MeshPara.face_eq[elset], Nint, Mint, 1);

  neut_nodes_free (&Nint);
  neut_mesh_free (&Mint);
  ut_free_1d_int (&bodynodes);

  return;
}

void
nem_meshing_pinching_fix (struct MESHPARA MeshPara, struct TESS *pTess,
                          struct NODES RNodes, struct MESH *RMesh,
                          struct NODES *pNodes, struct MESH *Mesh, int *elts)
{
  int i, j, k, elt, elset, newnode;
  int *facets = NULL, facetqty;
  int *comelts = ut_alloc_1d_int (2);
  int *facestates = ut_alloc_1d_int (2);

  for (i = 0; i < 2; i++)
    facestates[i] = (*pTess).FaceState[Mesh[2].EltElset[elts[i]]];

  if (facestates[0] == 0 && facestates[1] == 0)
    ut_array_1d_int_set (facestates, 2, 1);

  for (i = 0; i < 2; i++)
  {
    elt = elts[i];
    elset = Mesh[2].EltElset[elt];

    if (facestates[i] == 0)
      continue;

    nem_meshing_pinching_fix_eltfacets (Mesh, elt, &facets, &facetqty);

    for (j = 0; j < facetqty; j++)
    {
      nem_meshing_pinching_fix_split (pNodes, Mesh, elt, facets[j], &newnode);

      if (!neut_tess_face_iscurved (*pTess, elset))
        nem_meshing_pinching_fix_proj (MeshPara, pTess, RNodes, RMesh, pNodes,
                                       elset, newnode);
    }

    if (neut_tess_face_iscurved (*pTess, elset))
    {
      int *nodes = NULL, NodeQty;
      neut_mesh_elset_bodynodes_2d (*pTess, Mesh[1], Mesh[2], elset, &nodes, &NodeQty);

      for (k = 0; k < NodeQty; k++)
        neut_tess_domfaces_point_proj (*pTess, &(*pTess).FaceDom[elset][1], 1,
                                       (*pNodes).NodeCoo[nodes[k]],
                                       (*pNodes).NodeCoo[nodes[k]]);

      ut_free_1d_int (&nodes);
    }
  }

  ut_free_1d_int (&comelts);
  ut_free_1d_int (&facestates);

  return;
}
