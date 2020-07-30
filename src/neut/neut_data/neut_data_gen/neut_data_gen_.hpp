/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<ctype.h>

#include "neut.h"
#include "neut_data_gen.h"

#include "contrib/tinycolormap/tinycolormap.hpp"
extern int neut_data_colscheme_istinycolormap (char *colscheme);
extern tinycolormap::ColormapType neut_data_colscheme_tinycolormaptype (char *colscheme);
