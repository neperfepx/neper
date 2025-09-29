/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern int net_reg_merge_del (struct TESS *, int, int *, double *,
				int **, struct TESS *, int verbosity);

  extern void net_reg_merge_undel (struct TESS *pTess, int **buf,
				   struct TESS TessBuf);
  extern void net_reg_merge_undel_free (int **buf, struct TESS *pTessBuf);

#ifdef __cplusplus
}
#endif
