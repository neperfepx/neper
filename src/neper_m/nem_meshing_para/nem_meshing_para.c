/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_.h"

void
nem_meshing_para (struct IN_M In,
		  struct TESS *pTess,
		  struct TESR *pTesr, struct NODES *pRNodes,
		  struct MESH *RMesh, struct MESHPARA *pMeshPara)
{
  double min, max;

  ut_print_message (0, 2, "Preparing... ");

  nem_meshing_para_param (In, *pTess, pTesr, *pRNodes, RMesh, pMeshPara);

  nem_meshing_para_cl (In.clstring, In.clratiostring, *pTess, pTesr, pRNodes,
		       RMesh, pMeshPara);

  nem_meshing_para_mesh3dclreps (In.mesh3dclrepsstring, *pTess, pMeshPara);

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

  nem_meshing_para_faceproj (*pTess, *pRNodes, RMesh, pMeshPara);

  if ((*pMeshPara).dim < In.dim)
    ut_print_message (1, 3, "Meshing will be applied in %dD (not %dD).\n",
		      (*pMeshPara).dim, In.dim);

  return;
}

void
nem_meshing_para_post (struct MESHPARA MeshPara,
		       struct TESS *pTess,
		       struct TESR *pTesr, struct NODES *pRNodes,
		       struct NODES *pNodes, struct MESH *Mesh)
{
  nem_meshing_para_unscale (MeshPara, pTess, pTesr, pRNodes, pNodes, Mesh);

  return;
}
