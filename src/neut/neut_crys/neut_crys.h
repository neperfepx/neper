/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_crys.h
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_CRYS_H
#define NEUT_CRYS_H

extern void neut_crys_set_zero (struct CRYS *pCrys);
extern void neut_crys_free (struct CRYS *pCrys);

#endif				/* NEUT_CRYS_H */

#ifdef __cplusplus
}
#endif
