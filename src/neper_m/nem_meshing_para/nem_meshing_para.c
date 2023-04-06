/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_.h"

void
nem_meshing_para (struct IN_M In, struct TESS *pTess, struct TESR *pTesr,
                  struct NODES *pRNodes, struct MESH *RMesh,
                  struct MESHPARA *pMeshPara)
{
  int i;
  double min, max;

  ut_print_message (0, 2, "Preparing... ");

  nem_meshing_para_param (In, *pTess, pTesr, *pRNodes, RMesh, pMeshPara);

  nem_meshing_para_cl (In, pTess, pTesr, *pRNodes, RMesh, pMeshPara);

  nem_meshing_para_mesh3dclreps (In.mesh3dclrepsstring, pTess, *pTesr,
                                 pMeshPara);

  if ((*pTess).VerQty > 0)
  {
    min = ut_array_1d_min ((*pMeshPara).ver_cl + 1, (*pTess).VerQty);
    max = ut_array_1d_max ((*pMeshPara).ver_cl + 1, (*pTess).VerQty);

    if (ut_num_requal (min, max, 1e-6))
      printf ("(cl = %.4g) ", min);
    else
      printf ("(cl = %.4g-%.4g) ", min, max);
  }

  nem_meshing_para_scale (*pMeshPara, pTess, pTesr, pRNodes, RMesh);

  (*pMeshPara).edge_op = ut_alloc_2d_char ((*pTess).EdgeQty + 1, 1);

  (*pMeshPara).face_op = ut_alloc_2d_char ((*pTess).FaceQty + 1, 1);

  if ((*pMeshPara).dim >= 2 && (*pTess).Dim == 3)
    nem_meshing_para_faceproj (*pTess, *pRNodes, RMesh, pMeshPara);
  else
  {
    for (i = 1; i <= (*pTess).FaceQty; i++)
      ut_string_string ("noproj", (*pMeshPara).face_op + i);
    printf ("\n");
  }

  if ((*pMeshPara).dim < In.dim)
    ut_print_message (1, 3, "Meshing will be applied in %dD (not %dD).\n",
                      (*pMeshPara).dim, In.dim);

  return;
}

void
nem_meshing_para_post (struct MESHPARA MeshPara, struct TESS *pTess,
                       struct TESR *pTesr, struct NODES *pRNodes,
                       struct NODES *pNodes, struct MESH *Mesh)
{
  nem_meshing_para_unscale (MeshPara, pTess, pTesr, pRNodes, pNodes, Mesh);

  return;
}
