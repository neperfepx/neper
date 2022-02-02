/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"
#include"orilib.h"
#include"structIn_s.h"

#include"nes_convert_fepxparse.h"

extern void nes_convert_fepxparse_init (struct IN_S In, struct SIM *pSim);

extern void nes_convert_fepxparse_post (struct SIM Sim);
