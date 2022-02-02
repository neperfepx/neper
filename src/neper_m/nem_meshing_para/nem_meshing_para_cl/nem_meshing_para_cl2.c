/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
  neut_tess_entity_expr_val (Tess, "cell", clstring, *pcell_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, Tess.CellQty, Tess.Dim, *pcell_cl);

  return 0;
}

int
nem_meshing_para_cl_face (char *clstring, struct MESHPARA *pMeshPara,
                          struct TESS Tess)
{
  neut_tess_entity_expr_val (Tess, "face", clstring, (*pMeshPara).face_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl_face (pMeshPara, Tess, (*pMeshPara).face_cl);

  return 0;
}

int
nem_meshing_para_cl_edge (char *clstring, struct MESHPARA *pMeshPara,
                          struct TESS Tess)
{
  if (!(*pMeshPara).edge_cl)
    (*pMeshPara).edge_cl = ut_alloc_1d (Tess.FaceQty + 1);

  neut_tess_entity_expr_val (Tess, "edge", clstring, (*pMeshPara).edge_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl_edge (pMeshPara, Tess, (*pMeshPara).edge_cl);

  return 0;
}

int
nem_meshing_para_cl_ver (char *clstring, struct MESHPARA *pMeshPara,
                         struct TESS Tess)
{
  if (!(*pMeshPara).ver_cl)
    (*pMeshPara).ver_cl = ut_alloc_1d (Tess.FaceQty + 1);

  neut_tess_entity_expr_val (Tess, "ver", clstring, (*pMeshPara).ver_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl_ver (pMeshPara, Tess, (*pMeshPara).ver_cl);

  return 0;
}

int
nem_meshing_para_cl_cell_tesr (char *clstring, struct MESHPARA *pMeshPara,
                               struct TESR Tesr)
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
  neut_tesr_entity_expr_val (Tesr, "cell", clstring, *pcell_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, Tesr.CellQty, Tesr.Dim, *pcell_cl);

  return 0;
}

int
nem_meshing_para_cl_poly_mesh (char *clstring, struct MESHPARA *pMeshPara,
                               struct NODES Nodes, struct MESH *Mesh,
                               struct TESS Tess)
{
  (*pMeshPara).poly_cl = ut_alloc_1d (Tess.PolyQty + 1);
  neut_mesh_entity_expr_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                             Tess, NULL, NULL, NULL, NULL, "elset3d",
                             clstring, (*pMeshPara).poly_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, Tess.CellQty, Tess.Dim,
                             (*pMeshPara).poly_cl);

  return 0;
}
