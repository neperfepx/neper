/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_para (struct IN_M In,
			      struct TESS *pTess,
			      struct TESR *pTesr, struct NODES *pRNodes,
			      struct MESH *RMesh, struct MESHPARA *pMeshPara);

extern void nem_meshing_para_post (struct MESHPARA MeshPara,
				   struct TESS *pTess,
				   struct TESR *pTesr, struct NODES *pRNodes,
				   struct NODES *pNodes, struct MESH *Mesh);
