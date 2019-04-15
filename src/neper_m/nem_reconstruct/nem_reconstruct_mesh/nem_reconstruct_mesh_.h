/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neut_m.h"
#include "nem_reconstruct_mesh.h"
#include "../../nem/nem.h"

extern void nem_reconstruct_mesh_2d (struct NODES,
				     struct MESH *, int, struct TESS *);
extern void nem_reconstruct_mesh_1d (struct NODES,
				     struct MESH *, int, struct TESS *);
extern void nem_reconstruct_mesh_0d (struct NODES,
				     struct MESH *, int, struct TESS *);
