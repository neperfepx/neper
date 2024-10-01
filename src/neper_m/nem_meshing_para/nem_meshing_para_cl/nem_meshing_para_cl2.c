/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_cl_.h"

int
nem_meshing_para_cl_cell (char *clstring, struct MESHPARA *pMeshPara,
                          struct TESS *pTess)
{
  double **pcell_cl = NULL;

  if ((*pTess).Dim == 3)
    pcell_cl = &((*pMeshPara).poly_cl);
  else if ((*pTess).Dim == 2)
    pcell_cl = &((*pMeshPara).face_cl);
  else
    abort ();

  (*pcell_cl) = ut_alloc_1d ((*pTess).CellQty + 1);
  neut_tess_entity_expr_val (pTess, "cell", clstring, *pcell_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, (*pTess).CellQty, (*pTess).Dim, *pcell_cl);

  return 0;
}

int
nem_meshing_para_cl_face (char *clstring, struct MESHPARA *pMeshPara,
                          struct TESS *pTess)
{
  neut_tess_entity_expr_val (pTess, "face", clstring, (*pMeshPara).face_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl_face (pMeshPara, *pTess, (*pMeshPara).face_cl);

  return 0;
}

int
nem_meshing_para_cl_edge (char *clstring, struct MESHPARA *pMeshPara,
                          struct TESS *pTess)
{
  if (!(*pMeshPara).edge_cl)
    (*pMeshPara).edge_cl = ut_alloc_1d ((*pTess).FaceQty + 1);

  neut_tess_entity_expr_val (pTess, "edge", clstring, (*pMeshPara).edge_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl_edge (pMeshPara, *pTess, (*pMeshPara).edge_cl);

  return 0;
}

int
nem_meshing_para_cl_ver (char *clstring, struct MESHPARA *pMeshPara,
                         struct TESS *pTess)
{
  if (!(*pMeshPara).ver_cl)
    (*pMeshPara).ver_cl = ut_alloc_1d ((*pTess).FaceQty + 1);

  neut_tess_entity_expr_val (pTess, "ver", clstring, (*pMeshPara).ver_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl_ver (pMeshPara, *pTess, (*pMeshPara).ver_cl);

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
                               struct TESS *pTess)
{
  char *elset = ut_alloc_1d_char (100);

  sprintf (elset, "elset%dd", (*pTess).Dim);

  (*pMeshPara).poly_cl = ut_alloc_1d ((*pTess).PolyQty + 1);
  neut_mesh_entity_expr_val (Nodes, Mesh, pTess, NULL, NULL, NULL, NULL, elset,
                             clstring, (*pMeshPara).poly_cl, NULL);

  if (!strcmp ((*pMeshPara).cltype, "rel"))
    nem_meshing_para_rcl2cl (pMeshPara, (*pTess).CellQty, (*pTess).Dim,
                             (*pMeshPara).poly_cl);

  ut_free_1d_char (&elset);

  return 0;
}
