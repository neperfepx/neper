/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<math.h>
#include<ctype.h>

#include"../structIn_t.h"
#include"ut.h"
#include"neut_t.h"

#include"net_input.h"

extern void net_input_treatargs (int, char **, int, char **, struct IN_T *);
extern int net_input_treatargs_multiscale (char *option, char **parg,
					   int qty, char ***pvals);

extern void net_input_options_default (struct IN_T *);
extern void net_input_options_set (struct IN_T *, int, char **);
