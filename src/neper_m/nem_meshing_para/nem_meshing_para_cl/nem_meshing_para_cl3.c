/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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

int
nem_meshing_para_rcl2cl_face (struct MESHPARA *pMeshPara, struct TESS Tess,
			      double *face_cl)
{
  int i;

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    for (i = 1; i <= Tess.FaceQty; i++)
      rcl2cl (face_cl[i], Tess.Dim, (*pMeshPara).cellsize, 1,
	      (*pMeshPara).elttype, &(face_cl[i]));

  return 0;
}

int
nem_meshing_para_rcl2cl_edge (struct MESHPARA *pMeshPara, struct TESS Tess,
			      double *edge_cl)
{
  int i;

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    for (i = 1; i <= Tess.EdgeQty; i++)
      rcl2cl (edge_cl[i], Tess.Dim, (*pMeshPara).cellsize, 1,
	      (*pMeshPara).elttype, &(edge_cl[i]));

  return 0;
}

int
nem_meshing_para_rcl2cl_ver (struct MESHPARA *pMeshPara, struct TESS Tess,
			      double *ver_cl)
{
  int i;

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    for (i = 1; i <= Tess.VerQty; i++)
      rcl2cl (ver_cl[i], Tess.Dim, (*pMeshPara).cellsize, 1,
	      (*pMeshPara).elttype, &(ver_cl[i]));

  return 0;
}
