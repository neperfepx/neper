/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"../../structIn_v.h"
#include"neut_v.h"
#include "nev_print_vtk.h"

extern void nev_print_vtk_coodata (FILE *file, struct DATA Data);
extern void nev_print_vtk_coldata (FILE *file, struct DATA Data);
