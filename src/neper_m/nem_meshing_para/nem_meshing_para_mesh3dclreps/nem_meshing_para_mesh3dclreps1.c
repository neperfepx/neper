/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_mesh3dclreps_.h"

int
nem_meshing_para_mesh3dclreps (char *mesh3dclrepsstring, struct TESS *pTess,
                               struct TESR Tesr, struct MESHPARA *pMeshPara)
{
  if ((*pTess).CellQty > 0)
  {
    (*pMeshPara).mesh3dclreps = ut_alloc_1d ((*pTess).CellQty + 1);
    neut_tess_entity_expr_val (pTess, "cell", mesh3dclrepsstring,
                               (*pMeshPara).mesh3dclreps, NULL);
  }
  else if (Tesr.CellQty > 0)
  {
    (*pMeshPara).mesh3dclreps = ut_alloc_1d (Tesr.CellQty + 1);
    neut_tesr_entity_expr_val (Tesr, "cell", mesh3dclrepsstring,
                               (*pMeshPara).mesh3dclreps, NULL);
  }

  return 0;
}
