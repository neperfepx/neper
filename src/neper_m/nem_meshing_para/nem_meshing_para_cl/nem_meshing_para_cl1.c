/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_cl_.h"

int
nem_meshing_para_cl (struct IN_M In, struct TESS *pTess, struct TESR *pTesr,
                     struct NODES RNodes, struct MESH *Mesh,
                     struct MESHPARA *pMeshPara)
{
  char *clstring = In.clstring;
  char *clratiostring = In.clratiostring;

  if ((*pMeshPara).input == 't')
  {
    nem_meshing_para_cl_cell (clstring, pMeshPara, pTess);

    if ((*pTess).Dim == 3)
    {
      (*pMeshPara).face_cl = ut_alloc_1d ((*pTess).FaceQty + 1);

      neut_tess_val_poly2face ((*pTess), (*pMeshPara).poly_cl,
                               (*pMeshPara).face_cl);

      if (strcmp (In.clfacestring, "default"))
        nem_meshing_para_cl_face (In.clfacestring, pMeshPara, pTess);
    }

    if ((*pTess).Dim >= 2)
    {
      (*pMeshPara).edge_cl = ut_alloc_1d ((*pTess).EdgeQty + 1);

      neut_tess_val_face2edge ((*pTess), (*pMeshPara).face_cl,
                               (*pMeshPara).edge_cl);

      if (strcmp (In.cledgestring, "default"))
        nem_meshing_para_cl_edge (In.cledgestring, pMeshPara, pTess);
    }

    if ((*pTess).Dim >= 1)
    {
      (*pMeshPara).ver_cl = ut_alloc_1d ((*pTess).VerQty + 1);

      neut_tess_val_edge2ver ((*pTess), (*pMeshPara).edge_cl,
                              (*pMeshPara).ver_cl);

      if (strcmp (In.clverstring, "default"))
        nem_meshing_para_cl_ver (In.clverstring, pMeshPara, pTess);
    }
  }
  else if ((*pMeshPara).input == 'v')
  {
    nem_meshing_para_cl_cell_tesr (clstring, pMeshPara, *pTesr);

    if (!strcmp ((*pMeshPara).elttype, "tri"))
    {
      if ((*pTess).Dim == 3)
      {
        (*pMeshPara).face_cl = ut_alloc_1d ((*pTess).FaceQty + 1);
        neut_tess_val_poly2face ((*pTess), (*pMeshPara).poly_cl,
                                 (*pMeshPara).face_cl);
      }

      if ((*pTess).Dim >= 2)
      {
        (*pMeshPara).edge_cl = ut_alloc_1d ((*pTess).EdgeQty + 1);
        neut_tess_val_face2edge ((*pTess), (*pMeshPara).face_cl,
                                 (*pMeshPara).edge_cl);
      }

      if ((*pTess).Dim >= 1)
      {
        (*pMeshPara).ver_cl = ut_alloc_1d ((*pTess).VerQty + 1);
        neut_tess_val_edge2ver ((*pTess), (*pMeshPara).edge_cl,
                                (*pMeshPara).ver_cl);
      }
    }
  }
  else if ((*pMeshPara).input == 'm')
  {
    // if the mesh of max dimension is available, we use it
    if (!neut_mesh_isvoid (Mesh[(*pTess).Dim]))
      nem_meshing_para_cl_poly_mesh (clstring, pMeshPara, RNodes, Mesh, pTess);
    // otherwise, we use the tessellation
    else
    {
      nem_meshing_para_cl_cell (clstring, pMeshPara, pTess);
      ut_print_message (1, 3, "empty %dD mesh - used tess to compute cl.\n", (*pTess).Dim);
    }

    if (!strcmp ((*pMeshPara).elttype, "tri"))
    {
      (*pMeshPara).face_cl = ut_alloc_1d ((*pTess).FaceQty + 1);
      (*pMeshPara).edge_cl = ut_alloc_1d ((*pTess).EdgeQty + 1);
      (*pMeshPara).ver_cl = ut_alloc_1d ((*pTess).VerQty + 1);
      neut_tess_val_poly2face ((*pTess), (*pMeshPara).poly_cl,
                               (*pMeshPara).face_cl);
      neut_tess_val_face2edge ((*pTess), (*pMeshPara).face_cl,
                               (*pMeshPara).edge_cl);
      neut_tess_val_edge2ver ((*pTess), (*pMeshPara).edge_cl,
                              (*pMeshPara).ver_cl);
    }
  }

  if (clratiostring)
  {
    int j, qty;
    double gmean;
    char **tmp = NULL;

    ut_list_break (clratiostring, NEUT_SEP_DEP, &tmp, &qty);

    (*pMeshPara).clratio = ut_alloc_1d (3);
    ut_array_1d_set ((*pMeshPara).clratio, 3, 1);

    for (j = 0; j < qty; j++)
      ut_string_real (tmp[j], &((*pMeshPara).clratio[j]));
    gmean = ut_array_1d_gmean ((*pMeshPara).clratio, qty);
    ut_array_1d_scale ((*pMeshPara).clratio, qty, 1 / gmean);
  }

  return 0;
}
