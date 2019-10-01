/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include "../structIn_t.h"
  extern void net_in_set_zero (struct IN_T *pIn);
  extern void net_in_free (struct IN_T *pIn);

  extern int net_poly_clip (struct POLY *pPoly, double *eq, int id);
  extern void CreateTesl (struct SEEDSET SSet, double **Size, struct
			  TESL *pTesl);
  extern void net_tess_poly (struct TESS Tess, int poly, struct POLY *pPoly);
  extern void net_poly_tesl (struct POLY Poly, double *, struct TESL *pTess);
  extern void net_poly_tess (struct POLY Poly, double *coo,
			     struct TESS *pTess);

  extern void net_tess_poly_tess (struct TESS Tess, int poly,
				  struct TESS *pPoly);

  extern void net_poly_centroid (struct POLY Poly, double *coo);
  extern void net_tess_tesr (char *tesrsizestring, struct TESS, struct TESR *);
  extern void net_tess_tesr_cell (struct TESS, int, struct TESR *);

  extern int net_tess_clip (struct SEEDSET SSet, struct TESS *pTess,
			     double *eq);

  extern int net_multiscale_arg_0d_fscanf (char *string, char* flag,
					   int varqty, char **vars,
					   double *vals, double *pval);
  extern int net_multiscale_arg_0d_int_fscanf (char *string,
					       char* flag, int varqty,
					       char **vars, double *vals,
					       int *pval);
  extern int net_multiscale_arg_0d_char_fscanf (char *string,
					       char* flag, char *pval);
  extern int net_multiscale_arg_1d_fscanf (char *string, char *flag,
      double *val, int valqty);
  extern int net_multiscale_arg_1d_int_fscanf (char *string, char *flag,
      int *val, int valqty);
  extern int net_multiscale_arg_2d_fscanf (char *string, char *flag,
      double **val, int size1, int size2);
  extern void net_cubetess (double **size, struct TESS *pTess);

  extern void net_pts_convexhull (double** coos, int qty, int dim, struct NODES
				  *pN, struct MESH *pM);

  extern void  net_clip (char *clip, struct SEEDSET SSet, struct TESS
      *pTess);

  extern int net_multiscale_mtess_arg_0d_char_fscanf (int level, struct MTESS MTess,
      struct TESS *Tess, int domtess, int dompoly, char *string, char **pval);

  extern int net_tess_seedset (struct TESS Tess, struct SEEDSET *pSSet);

  extern void net_seedset_tess (struct SEEDSET SSet, struct TESS *pTess);

#include "net_tess/net_tess3d/net_tess3d.h"
#include "net_polymod_poly/net_polymod_poly.h"
#include "net_legacy/net_legacy.h"

#ifdef __cplusplus
}
#endif
