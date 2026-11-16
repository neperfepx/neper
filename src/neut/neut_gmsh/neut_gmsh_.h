/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<signal.h>
#ifndef _WIN32
#include<sys/wait.h>
#endif

#include"neut.h"
#include"neut_gmsh.h"
