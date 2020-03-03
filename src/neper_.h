/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<unistd.h>

#include"neper_config.h"

#include"ut.h"
#include"neper_t/neper_t.h"
#include"neper_m/neper_m.h"
#include"neper_v/neper_v.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif
