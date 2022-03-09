/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

#include"../structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include "nem_meshing_0d/nem_meshing_0d.h"
#include "nem_meshing_1d/nem_meshing_1d.h"
#include "nem_meshing_2d/nem_meshing_2d.h"
#include "nem_meshing_3d/nem_meshing_3d.h"
#include "nem_meshing_pinching/nem_meshing_pinching.h"

#include"nem_meshing.h"

extern void nem_meshing_post (struct TESS Tess, struct MESH *Mesh);
