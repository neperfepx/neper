/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_cl_.h"

int
nem_meshing_para_cl_cell (char *clstring, struct MESHPARA *pMeshPara,
			  struct TESS Tess)
{
  double **pcell_cl = NULL;

  if (Tess.Dim == 3)
    pcell_cl = &((*pMeshPara).poly_cl);
  else if (Tess.Dim == 2)
    pcell_cl = &((*pMeshPara).face_cl);
  else
    abort ();

  (*pcell_cl) = ut_alloc_1d (Tess.CellQty + 1);
  neut_tess_entity_expr_val (Tess, "cell", clstring, *pcell_cl);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, Tess, *pcell_cl);

  return 0;
}

int
nem_meshing_para_cl_cell_tesr (char *clstring, struct MESHPARA *pMeshPara,
			       struct TESR Tesr, struct TESS Tess)
{
  double **pcell_cl = NULL;

  if (Tesr.Dim == 3)
    pcell_cl = &((*pMeshPara).poly_cl);
  else if (Tesr.Dim == 2)
    pcell_cl = &((*pMeshPara).face_cl);
  else if (Tesr.Dim == 1)
    pcell_cl = &((*pMeshPara).edge_cl);
  else if (Tesr.Dim == 0)
    pcell_cl = &((*pMeshPara).ver_cl);
  else
    abort ();

  (*pcell_cl) = ut_alloc_1d (Tesr.CellQty + 1);
  neut_tesr_entity_expr_val (Tesr, "cell", clstring, *pcell_cl);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, Tess, *pcell_cl);

  return 0;
}

int
nem_meshing_para_cl_poly_mesh (char *clstring, struct MESHPARA *pMeshPara,
			       struct NODES Nodes, struct MESH *Mesh,
			       struct TESS Tess)
{
  struct PART Part;
  neut_part_set_zero (&Part);

  (*pMeshPara).poly_cl = ut_alloc_1d (Tess.PolyQty + 1);
  neut_mesh_entity_expr_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
			     Part, Tess, NULL, NULL, NULL, NULL,
			     "elset3d", clstring, (*pMeshPara).poly_cl);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, Tess, (*pMeshPara).poly_cl);

  neut_part_free (Part);

  return 0;
}
