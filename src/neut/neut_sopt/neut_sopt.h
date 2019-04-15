/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SOPT_H
#define NEUT_SOPT_H

#ifdef HAVE_NLOPT
extern void neut_sopt_set_zero (struct SOPT *pTOpt);
extern void neut_sopt_free (struct SOPT *pTOpt);
#endif

#endif /* NEUT_SOPT_H */
