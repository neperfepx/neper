/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

using namespace std;
// #include"../../../contrib/nanoflann/nanoflann.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<limits.h>

#include"ut.h"
#include"orilib.h"
#include"neut.h"

#include "neut.h"
#include "neut_odf_fscanf.h"

extern void neut_odf_fscanf_head (FILE * file, char **pversion);
extern void neut_odf_fscanf_space (FILE * file, struct ODF *pOdf);
extern void neut_odf_fscanf_eltdata (FILE * file, struct ODF *pOdf);
extern void neut_odf_fscanf_nodedata (FILE * file, struct ODF *pOdf);

#ifdef __cplusplus
}
#endif
