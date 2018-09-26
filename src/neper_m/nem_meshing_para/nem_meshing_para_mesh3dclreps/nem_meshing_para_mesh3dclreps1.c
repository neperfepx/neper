/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_mesh3dclreps_.h"

int
nem_meshing_para_mesh3dclreps (char *mesh3dclrepsstring, struct TESS Tess,
                               struct MESHPARA *pMeshPara)
{
  (*pMeshPara).mesh3dclreps = ut_alloc_1d (Tess.CellQty + 1);

  neut_tess_entity_expr_val (Tess, "cell", mesh3dclrepsstring, (*pMeshPara).mesh3dclreps);

  return 0;
}
