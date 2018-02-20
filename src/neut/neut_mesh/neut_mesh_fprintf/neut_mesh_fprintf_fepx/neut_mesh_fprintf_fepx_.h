/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"orilib.h"
#include"neut.h"

#include "neut_mesh_fprintf_fepx.h"

extern void neut_mesh_surffile_update (FILE *, FILE *, struct PART);
extern void neut_mesh_bcsfile_update (FILE *, FILE *, struct PART);
