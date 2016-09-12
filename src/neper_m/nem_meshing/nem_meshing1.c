/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_.h"

// This function applies meshing to a tessellation whose topology is
// described in the TESS structure. The edge and face morphologies (if
// not straight and flat, respectively) can be provided using with the
// RNodes, RMesh[0] and RMesh[1] structures.  This is useful for
// remeshing.
int
nem_meshing (struct IN_M In, struct MESHPARA MeshPara,
	     struct TESS Tess,
	     struct NODES RNodes, struct MESH *RMesh,
	     struct NODES *pNodes, struct MESH *Mesh)
{
  /*
     if (Tess.fmax > 0)
     >>>>>>> devel2
     {
     if (1.01 * Tess.sel < MeshPara.cl / MeshPara.pl)
     ut_print_messagewnc (1, 72,
     "Regularization was used with sel < cl / pl.  The mesh could be locally overrefined.");
     }
   */

  nem_meshing_gen (Tess, pNodes);

  if (MeshPara.dim >= 0)
  {
    nem_meshing_0D (Tess, MeshPara, pNodes, Mesh);
    neut_nodes_init_nodeslave (pNodes);
  }

  if (MeshPara.dim >= 1)
    nem_meshing_1D (MeshPara, Tess, RNodes, RMesh, pNodes, Mesh);

  if (MeshPara.dim >= 2)
  {
    nem_meshing_2D (In, MeshPara, Tess, RNodes, RMesh, pNodes, Mesh);

    if (Mesh[2].EltQty > 0 && !strcmp (In.meshface, "all"))
      nem_meshing_pinching (Tess, *pNodes, Mesh);
  }

  if (MeshPara.dim >= 3)
    nem_meshing_3D (In, MeshPara, Tess, pNodes, Mesh);

  if (Tess.CellId)
    if (Mesh[Tess.Dim].EltQty > 0)
    {
      Mesh[Tess.Dim].ElsetId = ut_alloc_1d_int (Tess.CellQty + 1);
      ut_array_1d_int_memcpy (Mesh[Tess.Dim].ElsetId + 1,
			      Tess.CellQty, Tess.CellId + 1);
    }

  neut_nodes_init_nodeslave (pNodes);

  return EXIT_SUCCESS;
}
