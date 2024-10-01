/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<float.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_scale.h"

#include "../nev_print_utils/nev_print_utils.h"
#include "../nev_print_png/nev_print_png.h"

extern void nev_print_png_scale (struct IN_V In, FILE * file, char *ColScheme, char *scale, char *scaletitle);
