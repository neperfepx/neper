/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the tgeom-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>

#include "neut.h"

#include"neut_tesr_geom.h"


extern int neut_tesr_cell_boundpoints_test (struct TESR Tesr, int cell,
					    int i, int j, int k,
					    int connectivity, int interior);

extern int neut_tesr_cell_boundpoints_test_3d (struct TESR Tesr, int cell,
					       int i, int j, int k,
					       int connectivity, int interior);

extern int neut_tesr_cell_boundpoints_test_2d (struct TESR Tesr, int cell,
					       int i, int j,
					       int connectivity, int interior);
