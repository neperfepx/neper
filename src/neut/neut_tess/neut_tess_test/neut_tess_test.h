/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_TEST_H
#define NEUT_TESS_TEST_H

  extern int neut_tess_test (struct TESS, int);
  extern int neut_tess_testAroundVer (struct TESS, int, int);
  extern int neut_tess_testAroundVer2 (struct TESS, int, int);
  extern int neut_tess_test_face_normal (struct TESS, int, int);
  extern int neut_tess_test_dom (struct TESS, int);

#endif				/* NEUT_TESS_TEST_H */

#ifdef __cplusplus
}
#endif
