/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_param_.h"

void
nem_meshing_para_param (struct IN_M In, struct TESS Tess,
			struct TESR *pTesr, struct NODES RNodes,
			struct MESH *RMesh, struct MESHPARA *pMeshPara)
{
  int cellqty, dim;
  double size;

  (*pMeshPara).elttype = ut_alloc_1d_char (strlen (In.elttype) + 1);
  strcpy ((*pMeshPara).elttype, In.elttype);
  (*pMeshPara).pl = In.pl;
  (*pMeshPara).clmin = In.clmin;
  (*pMeshPara).cltype = ut_alloc_1d_char (strlen (In.cltype) + 1);
  strcpy ((*pMeshPara).cltype, In.cltype);

  // Testing type of input: tess or tesr or mesh
  if (In.mesh)
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
      else if (Tess.Dim == 1)
	neut_tess_length (Tess, &size);
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
