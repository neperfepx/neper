/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include"neper_config.h"

#ifdef HAVE_OPENMP
#include<omp.h>
#endif // HAVE_OPENMP

#include"ut.h"
#include"neut_m.h"

#include"nem_mesh_gmsh.h"

// maximum number of iterations on the scale factor of the mesh size map
#define NEM_MESH_GMSH_CLITERMAX 8

// initial estimate of the slope of the mean ratio of the elt lengths to the
// mesh size map versus the scale factor (log-log)
#define NEM_MESH_GMSH_CLSLOPE 0.5

// bounds of the scale factor of the mesh size map
#define NEM_MESH_GMSH_ALPHAMIN 0.2
#define NEM_MESH_GMSH_ALPHAMAX 5

