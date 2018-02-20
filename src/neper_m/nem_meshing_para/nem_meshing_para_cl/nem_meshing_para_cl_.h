/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>
#include<float.h>

#include"ut.h"
#include"orilib.h"
#include"neut_m.h"
#include"../../structIn_m.h"

#include"nem_meshing_para_cl.h"

extern int nem_meshing_para_cl_cell (char *, struct MESHPARA *,
				     struct TESS Tess);
extern int nem_meshing_para_cl_cell_tesr (char *, struct MESHPARA *pMeshPara,
					  struct TESR Tesr, struct TESS Tess);
extern int nem_meshing_para_cl_poly_mesh (char *, struct MESHPARA *pMeshPara,
					  struct NODES Nodes,
					  struct MESH *Mesh,
					  struct TESS Tess);

extern int nem_meshing_para_rcl2cl (struct MESHPARA *pMeshPara, struct
				    TESS, double *cell_cl);
