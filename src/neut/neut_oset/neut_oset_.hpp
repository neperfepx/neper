/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <iostream>
#include <cfloat>
using namespace std;
#include"neut_oset.hpp"
#include "neut_qcloud_struct.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#include"orilib.h"
#include"neut.h"

#ifdef __cplusplus
}
#endif

extern void neut_oset_kdtree_cloud (struct OL_SET *pOSet, struct QCLOUD *pqcloud);
extern void neut_oset_kdtree_build (struct QCLOUD *pqcloud,
                                    my_kd_tree_t **pqindex);
