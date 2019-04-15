/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_scaling_para (char *elttype, struct TESS *pTess,
			      struct TESR *pTesr, struct NODES *pRNodes,
			      struct MESH *RMesh, struct MESHPARA *pMeshPara);

extern void nem_scaling_post (struct MESHPARA MeshPara, struct TESS *pTess,
			      struct TESR *pTesr, struct NODES *pNodes);
