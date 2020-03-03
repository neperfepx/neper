/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

#include"../structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include "nem_meshing_0D/nem_meshing_0D.h"
#include "nem_meshing_1D/nem_meshing_1D.h"
#include "nem_meshing_2D/nem_meshing_2D.h"
#include "nem_meshing_3D/nem_meshing_3D.h"
#include "nem_meshing_pinching/nem_meshing_pinching.h"

#include"nem_meshing.h"

extern void nem_meshing_gen (struct TESS Tess, struct NODES *pNodes);

extern void nem_meshing_post (struct TESS Tess, struct MESH *Mesh);
