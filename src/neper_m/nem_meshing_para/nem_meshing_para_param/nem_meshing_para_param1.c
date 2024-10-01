/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_param_.h"

void
nem_meshing_para_param (struct IN_M In, struct TESS Tess, struct TESR *pTesr,
                        struct NODES RNodes, struct MESH *RMesh,
                        struct MESHPARA *pMeshPara)
{
  int cellqty, dim;
  double size = 0;

  (*pMeshPara).VerQty = Tess.VerQty;
  (*pMeshPara).EdgeQty = Tess.EdgeQty;
  (*pMeshPara).FaceQty = Tess.FaceQty;
  (*pMeshPara).PolyQty = Tess.PolyQty;

  (*pMeshPara).pl = In.pl;
  (*pMeshPara).clmin = In.clmin;
  ut_string_string (In.cltype, &(*pMeshPara).cltype);
  ut_string_string (In.elttype, &(*pMeshPara).elttype); // needed by quad9

  // Testing type of input: tess or tesr or mesh
  if (In.mesh && !neut_mesh_isvoid (RMesh[Tess.Dim]))
  {
    (*pMeshPara).input = 'm';
    dim = 3;
    cellqty = RMesh[3].ElsetQty;
  }
  else if (In.tess)
  {
    (*pMeshPara).input = 't';
    dim = Tess.Dim;
    cellqty = neut_tess_dim_entityqty (Tess, dim);
  }
  else if (In.tesr)
  {
    (*pMeshPara).input = 'v';
    dim = (*pTesr).Dim;
    cellqty = (*pTesr).CellQty;
  }
  else
    return;

  if (!strcmp ((*pMeshPara).cltype, "rel"))
  {
    if ((*pMeshPara).input == 't')
    {
      if (Tess.Dim == 3)
        neut_tess_volume (Tess, &size);
      else if (Tess.Dim == 2)
        neut_tess_area (Tess, &size);
    }
    else if ((*pMeshPara).input == 'v')
      neut_tesr_size (*pTesr, &size);
    else if ((*pMeshPara).input == 'm')
      neut_mesh_volume (RNodes, RMesh[3], &size);
    else
      abort ();
  }

  // dim
  (*pMeshPara).dim = In.dim;

  (*pMeshPara).cellsize = size / cellqty;

  return;
}
