/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_scale_.h"

void
nem_meshing_para_scale (struct MESHPARA MeshPara, struct TESS *pTess,
			struct TESR *pTesr, struct NODES *pNodes,
			struct MESH *Mesh)
{
  if (MeshPara.clratio)
  {
    if ((*pTess).VerQty > 0)
      neut_tess_scale (pTess,
		       1 / MeshPara.clratio[0],
		       1 / MeshPara.clratio[1], 1 / MeshPara.clratio[2]);

    if ((*pTesr).CellQty > 0)
      neut_tesr_scale (pTesr, 1 / MeshPara.clratio[0],
		       1 / MeshPara.clratio[1], 1 / MeshPara.clratio[2],
		       NULL);

    if ((*pNodes).NodeQty > 0)
    {
      neut_nodes_scale (pNodes,
			1 / MeshPara.clratio[0],
			1 / MeshPara.clratio[1], 1 / MeshPara.clratio[2]);

      if ((*pTess).VerQty > 0)
	nem_tess_updatefrommesh_geom (pTess, *pNodes, Mesh);
    }
  }

  return;
}

void
nem_meshing_para_unscale (struct MESHPARA MeshPara,
			  struct TESS *pTess, struct TESR *pTesr,
			  struct NODES *pRNodes, struct NODES *pNodes,
			  struct MESH *Mesh)
{
  if (MeshPara.clratio)
  {
    if ((*pTess).VerQty > 0)
      if (MeshPara.clratio)
	neut_tess_scale (pTess, MeshPara.clratio[0], MeshPara.clratio[1],
			 MeshPara.clratio[2]);

    if ((*pTesr).CellQty > 0)
      neut_tesr_scale (pTesr, MeshPara.clratio[0], MeshPara.clratio[1],
		       MeshPara.clratio[2], NULL);

    if ((*pRNodes).NodeQty > 0)
      neut_nodes_scale (pRNodes, MeshPara.clratio[0], MeshPara.clratio[1],
			MeshPara.clratio[2]);

    if ((*pNodes).NodeQty > 0)
    {
      neut_nodes_scale (pNodes, MeshPara.clratio[0], MeshPara.clratio[1],
			MeshPara.clratio[2]);

      if ((*pTess).VerQty > 0)
	nem_tess_updatefrommesh_geom (pTess, *pNodes, Mesh);
    }
  }

  return;
}
