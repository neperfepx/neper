/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_tess_str (struct IN_M In, struct MESHPARA
                                  MeshPara, struct TESS *pTess,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  struct NSET *NSet);

extern void nem_meshing_tesr_str (struct IN_M In, struct MESHPARA
                                  MeshPara, struct TESR *pTesr,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  struct NSET *NSet);
