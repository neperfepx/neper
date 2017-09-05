/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_para_scale (struct MESHPARA MeshPara,
				    struct TESS *pTess, struct TESR *pTesr,
				    struct NODES *pRNodes, struct MESH *Mesh);

extern void nem_meshing_para_unscale (struct MESHPARA MeshPara,
				      struct TESS *pTess,
				      struct TESR *pTesr,
				      struct NODES *pRNodes,
				      struct NODES *pNodes,
				      struct MESH *Mesh);
