/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_pinching_.h"

int
nem_meshing_pinching_testpoly (struct IN_M In, int poly)
{
  int status, varqty = 1;
  char **vars = ut_alloc_1d_pchar (varqty);
  double *vals = ut_alloc_1d (varqty);

  ut_string_string ("id", vars);

  vals[0] = poly;
  if (strcmp (In.meshpoly, "all"))
    ut_math_eval_int (In.meshpoly, varqty, vars, vals, &status);
  else
    status = 1;

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

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

  neut_mesh_elset2d_bodynodes (*pTess, Mesh, elset, &bodynodes, &bodynodeqty);

  for (i = 0; i < bodynodeqty; i++)
    neut_node_proj_alongontomesh ((*pNodes).NodeCoo[bodynodes[i]],
				  MeshPara.face_eq[elset],
				  Nint, Mint, 1);

  neut_nodes_free (&Nint);
  neut_mesh_free (&Mint);
  ut_free_1d_int (bodynodes);

  return;
}

void
nem_meshing_pinching_fix (struct MESHPARA MeshPara, struct TESS *pTess,
			  struct NODES RNodes, struct MESH* RMesh,
			  struct NODES *pNodes, struct MESH *Mesh,
			  int* elts)
{
  int i, j, elt, elset, newnode;
  int *facets = NULL, facetqty;
  int *nodes = ut_alloc_1d_int (2);
  int *comelts = ut_alloc_1d_int (2);

  for (i = 0; i < 2; i++)
  {
    elt = elts[i];
    elset = Mesh[2].EltElset[elt];

    if ((*pTess).FaceState[elset] == 0)
      continue;

    nem_meshing_pinching_fix_eltfacets (Mesh, elt, &facets, &facetqty);

    for (j = 0; j < facetqty; j++)
    {
      nem_meshing_pinching_fix_split (pNodes, Mesh, elt, facets[j], &newnode);

      nem_meshing_pinching_fix_proj (MeshPara, pTess, RNodes, RMesh, pNodes,
				     elset, newnode);
    }
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (comelts);

  return;
}
