/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void net_tess_lam_seed (struct IN_T In, int level, char *, struct MTESS, struct TESS*,
                            int dtess, int dcell, struct TESS,
			    struct SEEDSET *, struct SEEDSET *);

#ifdef __cplusplus
}
#endif
