/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include"orilib.h"

#include"../nev_data_utils/nev_data_utils.h"
#include "nev_meshdata.h"

extern void nev_meshdata_fscanf_set2entity (struct MESH *, char *, char *,
					    struct MESHDATA *);

extern void nev_meshdata_fscanf_elt (struct MESH, struct MESHDATA *pMD,
				     char *type, char *argument, int elset);
extern void nev_meshdata_fscanf_eltb (struct MESHDATA *pMD, char *type,
				      char *argument);

extern void nev_meshdata_elset2elt (struct MESH Mesh, char *entity,
				    char *type, struct MESHDATA MeshDataSet,
				    struct MESHDATA *pMeshData);
