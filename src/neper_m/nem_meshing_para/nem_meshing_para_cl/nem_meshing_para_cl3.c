/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_cl_.h"

int
nem_meshing_para_rcl2cl (struct MESHPARA *pMeshPara, struct TESS Tess,
			 double *cell_cl)
{
  int i;

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    for (i = 1; i <= Tess.CellQty; i++)
      rcl2cl (cell_cl[i], Tess.Dim, (*pMeshPara).cellsize, 1,
	      (*pMeshPara).elttype, &(cell_cl[i]));

  return 0;
}
