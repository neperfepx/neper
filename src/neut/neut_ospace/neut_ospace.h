/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_OSPACE_H
#define NEUT_OSPACE_H

extern void neut_ospace_set_zero (struct OSPACE *pOSpace);
extern void neut_ospace_fnscanf (char *crysym, char *space, char *fr, struct OSPACE *pOSpace, char *mode);
extern void neut_ospace_fnscanf_tess (char *filename, char *space, struct OSPACE *pOSpace, char *mode);
extern void neut_ospace_fnscanf_mesh (char *crysym, char *space, char *fr, struct OSPACE *pOSpace, char *mode);
extern void neut_ospace_mesh_olset (struct OSPACE *pOSpace, struct OL_SET *pOSet);
extern void neut_ospace_mesh_olset_allnodes (struct OSPACE *pOSp, struct OL_SET *pOSet);
// extern void neut_ospace_free (struct OSPACE *pOSpace);

#endif /* NEUT_OSPACE_H */

#ifdef __cplusplus
}
#endif
