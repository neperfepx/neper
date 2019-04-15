/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_pinching_.h"

void
nem_meshing_pinching (struct IN_M In, struct MESHPARA MeshPara,
		      struct TESS *pTess, struct NODES RNodes,
		      struct MESH *RMesh, struct NODES *pNodes,
		      struct MESH *Mesh)
{
  int i, j, k, elt1d, status, elset;
  double n1n2angle;
  int elt1dqty, *elts1d = NULL;
  int elt2dqty, *elts2d = NULL;
  int edgeqty, *edges = NULL;

  ut_print_message (0, 2, "Fixing 2D-mesh pinches...\n");

  // for each poly, looking for pinches at every 1d elt
  for (i = 1; i <= (*pTess).PolyQty; i++)
  {
    if (nem_meshing_pinching_testpoly (In, *pTess, Mesh, i))
      continue;

    neut_tess_poly_edges (*pTess, i, &edges, &edgeqty);
    neut_mesh_elsets_elts (Mesh[1], edges, edgeqty, &elts1d, &elt1dqty);

    for (j = 0; j < elt1dqty; j++)
    {
      elt1d = elts1d[j];
      neut_mesh_elt1d_elsets2d_elts2d (Mesh[1], elt1d, Mesh[2],
				       (*pTess).PolyFaceNb[i] + 1,
				       (*pTess).PolyFaceQty[i],
				       &elts2d, &elt2dqty);
      if (elt2dqty != 2)
	ut_error_reportbug ();

      // testing for pinching
      status = neut_mesh_elt2dpair_angle (*pNodes, Mesh[2], elts2d[0], elts2d[1],
					  &n1n2angle);

      if (status != 0 || ut_num_equal (n1n2angle, 180, 1e-3))
      {
	ut_print_message (0, 3, "poly %d: elts %d & %d...\n",
			  i, elts2d[0], elts2d[1], status);

	// setting interpolations based on barycenter
	if (RNodes.NodeQty == 0)
	{
	  for (k = 0; k < 2; k++)
	  {
	    elset = Mesh[2].EltElset[elts2d[k]];
	    if ((*pTess).FaceState[elset] != 0 && (*pTess).FacePt[elset] > 0)
	      nem_meshing_pinching_inter (MeshPara, pTess, pNodes, Mesh, elset);
	  }

	  // testing again for pinching
	  status = neut_mesh_elt2dpair_angle (*pNodes, Mesh[2], elts2d[0], elts2d[1],
					      &n1n2angle);
	}

	// if the problem remains, fixing meshes
	if (status != 0 || ut_num_equal (n1n2angle, 180, 1e-3))
	  nem_meshing_pinching_fix (MeshPara, pTess, RNodes, RMesh, pNodes,
				    Mesh, elts2d);
      }
    }
  }

  ut_free_1d_int (elts2d);
  ut_free_1d_int (elts1d);
  ut_free_1d_int (edges);

  return;
}
