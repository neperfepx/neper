/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_cl_.h"

int
nem_meshing_para_rcl2cl (struct MESHPARA *pMeshPara, int cellqty, int dim,
			 double *cell_cl)
{
  int i;

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    for (i = 1; i <= cellqty; i++)
      rcl2cl (cell_cl[i], dim, (*pMeshPara).cellsize, 1,
	      (*pMeshPara).elttype, &(cell_cl[i]));

  return 0;
}
