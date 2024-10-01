/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<math.h>
#include<ctype.h>

#include"../structIn_s.h"
#include"ut.h"
#include"orilib.h"
#include"neut.h"

#include"nes_input.h"
#include"../nes_utils/nes_utils.h"

extern void nes_input_treatargs (int, char **, int, char **, struct IN_S *);
extern void nes_input_options_default (struct IN_S *);
extern void nes_input_options_set (struct IN_S *, int, char **);
