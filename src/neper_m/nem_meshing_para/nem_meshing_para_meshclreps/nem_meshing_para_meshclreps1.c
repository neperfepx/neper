/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_meshclreps_.h"

// Tolerances on the mean ratio of the elt lengths to the mesh size map, by
// face (-mesh2dclreps) and by cell (-mesh3dclreps)
int
nem_meshing_para_meshclreps (struct IN_M In, struct TESS *pTess,
                             struct TESR Tesr, struct MESHPARA *pMeshPara)
{
  if ((*pTess).FaceQty > 0)
  {
    (*pMeshPara).mesh2dclreps = ut_alloc_1d ((*pTess).FaceQty + 1);
    neut_tess_entity_expr_val (pTess, "face", In.mesh2dclrepsstring,
                               (*pMeshPara).mesh2dclreps, NULL);
  }

  if ((*pTess).CellQty > 0)
  {
    (*pMeshPara).mesh3dclreps = ut_alloc_1d ((*pTess).CellQty + 1);
    neut_tess_entity_expr_val (pTess, "cell", In.mesh3dclrepsstring,
                               (*pMeshPara).mesh3dclreps, NULL);
  }
  else if (Tesr.CellQty > 0)
  {
    (*pMeshPara).mesh3dclreps = ut_alloc_1d (Tesr.CellQty + 1);
    neut_tesr_entity_expr_val (Tesr, "cell", In.mesh3dclrepsstring,
                               (*pMeshPara).mesh3dclreps, NULL);
  }

  return 0;
}
