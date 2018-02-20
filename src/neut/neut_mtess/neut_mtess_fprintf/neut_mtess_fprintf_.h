/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"neut_mtess_fprintf.h"

extern void neut_mtess_fprintf_header (FILE * file, struct MTESS MTess,
				       struct TESS *Tess);
extern void neut_mtess_fprintf_tess (FILE * file, struct MTESS MTess,
				     struct TESS *Tess);
