/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_print_show_mesh.h"

extern void nev_print_show_mesh_nodes_nset (char *expr, struct NSET NSet, int nodeqty,
                                            char **pexpr, int ***pnsetnode);
#include "../nev_print_show_utils/nev_print_show_utils.h"
