/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_qcloud_struct.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

extern void neut_oset_kdtree (struct OL_SET *pOSet, QCLOUD
                                   *pqcloud, my_kd_tree_t **pqindex);
extern void neut_oset_clustering (struct OL_SET OSet, struct OL_SET Grid, char *method, struct OL_SET *pOSet);
extern void neut_oset_odf_clustering (struct OL_SET OSet, struct ODF Odf, char *method, struct OL_SET *pOSet);

#ifdef __cplusplus
}
#endif
