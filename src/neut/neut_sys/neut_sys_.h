/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/stat.h>
#include<limits.h>
#ifdef __APPLE__
#include<mach-o/dyld.h>
#endif
#include"neut.h"
#include"neut_sys.h"
