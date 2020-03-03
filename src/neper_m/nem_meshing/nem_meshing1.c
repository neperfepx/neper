/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_.h"

// This function applies meshing to a tessellation whose topology is
// described in the TESS structure. The edge and face morphologies (if
// not straight and flat, respectively) can be provided using with the
// RNodes, RMesh[0] and RMesh[1] structures.  This is useful for
// remeshing.
int
nem_meshing (struct IN_M In, struct MESHPARA *pMeshPara, struct TESS *pTess,
             struct NODES RNodes, struct MESH *RMesh, struct NODES *pNodes,
             struct MESH *Mesh)
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

  nem_meshing_gen (*pTess, pNodes);

  if ((*pMeshPara).dim >= 0)
  {
    if (!neut_mesh_isvoid (Mesh[0]))
      ut_print_message (0, 2, "0D meshing... skipped\n");
    else
      nem_meshing_0D (*pTess, *pMeshPara, pNodes, Mesh);
  }

  if ((*pMeshPara).dim >= 1)
  {
    if (!neut_mesh_isvoid (Mesh[1]))
      ut_print_message (0, 2, "1D meshing... skipped\n");
    else
      nem_meshing_1D (pMeshPara, *pTess, RNodes, RMesh, pNodes, Mesh);
  }

  if ((*pMeshPara).dim >= 2)
  {
    if (!neut_mesh_isvoid (Mesh[2]))
      ut_print_message (0, 2, "2D meshing... skipped\n");
    else
      nem_meshing_2D (In, *pMeshPara, *pTess, RNodes, RMesh, pNodes, Mesh);

    if (Mesh[2].EltQty > 0 && In.mesh2dpinchfix)
      nem_meshing_pinching (In, *pMeshPara, pTess, RNodes, RMesh, pNodes,
                            Mesh);
  }

  if ((*pMeshPara).dim >= 3)
  {
    if (!neut_mesh_isvoid (Mesh[3]))
      ut_print_message (0, 2, "3D meshing... skipped\n");
    else
      nem_meshing_3D (In, *pMeshPara, *pTess, pNodes, Mesh);
  }

  if ((*pTess).CellId)
    if (Mesh[(*pTess).Dim].EltQty > 0)
    {
      Mesh[(*pTess).Dim].ElsetId = ut_alloc_1d_int ((*pTess).CellQty + 1);
      ut_array_1d_int_memcpy ((*pTess).CellId + 1, (*pTess).CellQty,
                              Mesh[(*pTess).Dim].ElsetId + 1);
    }

  nem_meshing_post (*pTess, Mesh);

  return EXIT_SUCCESS;
}
