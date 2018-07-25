/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include"neper_config.h"

#ifdef HAVE_OPENMP
#include<omp.h>
#endif // HAVE_OPENMP

#include"../../structIn_m.h"

#include"ut.h"
#include"neut_m.h"

#include"nem_mesh_gmsh.h"
