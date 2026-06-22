/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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
#include "../nev_print_real/nev_print_real_png/nev_print_real_png.h"

extern void nev_print_png_scale_minmax (FILE *file, struct DATA Data, struct PRINT Print);
