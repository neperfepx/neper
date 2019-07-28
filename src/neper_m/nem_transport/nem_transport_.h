/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<float.h>

#include"../structIn_m.h"
#include"ut.h"
#include"orilib.h"
#include"neut_m.h"
#include"../nem/nem.h"

#include"nem_transport.h"

extern void nem_transport_elt_oldelt (struct NODES, struct MESH,
				      struct NODES, struct MESH,
				      char *, int **);

extern void nem_transport_elt (char *type, char *filename, char *method,
                               struct NODES RNodes, struct MESH RMesh, struct
                               NODES Nodes, struct MESH Mesh, int **poldelt);

extern void nem_transport_node (char *type, char *filename, struct NODES
                                RNodes, struct MESH RMesh, struct NODES Nodes);

extern void nem_transportfepx_fepxfiles (struct IN_M In, struct TESS Tess,
                                         struct MESH *Mesh, char **transport);
