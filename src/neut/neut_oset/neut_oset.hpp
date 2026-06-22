/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_struct_qcloud.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

extern void neut_oset_kdtree (struct OL_SET *pOSet, QCLOUD *pqcloud, my_kd_tree_t **pqtree);
extern void neut_oset_kdtree_cloud (struct OL_SET *pOSet, struct QCLOUD *pqcloud);
extern void neut_oset_kdtree_build (struct QCLOUD *pqcloud, my_kd_tree_t **pqtree);

extern void neut_oset_clustering (struct OL_SET OSet, struct OL_SET Grid, char *method, struct OL_SET *pOSet);
extern void neut_oset_odf_clustering (struct OL_SET OSet, struct ODF *pOdf, char *method, struct OL_SET *pOSet);

#ifdef __cplusplus
}
#endif
