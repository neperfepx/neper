/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_poly.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_POLY_GEOM_H
#define NEUT_POLY_GEOM_H

  extern void neut_poly_bbox (struct POLY, double *);
  extern int neut_poly_vercentroid (struct POLY Poly, double *coo);
  extern int neut_poly_centroid (struct POLY Poly, double *coo);
  extern int neut_poly_volume (struct POLY Poly, double *coo);
  extern int neut_poly_volume_2d (struct POLY Poly, double Size, double *coo);
  extern void neut_poly_diameq (struct POLY Poly, double *val);
  extern void neut_poly_diameq_2d (struct POLY Poly, double h, double *val);
  extern void neut_poly_radeq (struct POLY Poly, double *val);
  extern void neut_poly_radeq_2d (struct POLY Poly, double h, double *val);
  extern int neut_poly_point_in (struct POLY, double *);
  extern int neut_poly_point_dist_legacy (struct POLY, double *, double *);
  extern int neut_poly_point_distfromboundary (struct POLY Poly, double *coo, double *pdist);
  extern int neut_poly_point_distfromboundary_2d (struct POLY Poly, double *coo, double *pdist);
  extern int neut_poly_point_distfromboundary_signed (struct POLY Poly, double *coo, double *pdist);
  extern int neut_poly_point_distfromboundary_2d_signed (struct POLY Poly, double *coo, double *pdist);
  extern int neut_poly_point_inface (struct POLY Poly, double *coo, int);
  extern int neut_poly_face_area (struct POLY Poly, int face, double *parea);
  extern void neut_poly_face_centre (struct POLY Poly, int face, double *coo);
  extern void neut_poly_area (struct POLY Poly, double *pval);
  extern int neut_poly_sphericity (struct POLY Poly, double *pval);
  extern int neut_poly_sphericity_2d (struct POLY Poly, double size,
				      double *pval);

  extern int neut_polys_volume (struct POLY *Poly, int *polys, int polyqty, double *pvol);
  extern int neut_polys_volume_2d (struct POLY *Poly, int *polys, int polyqty, double Size, double *pval);
  extern void neut_polys_diameq (struct POLY *Poly, int *polys, int polyqty, double *val);
  extern void neut_polys_diameq_2d (struct POLY *Poly, int *polys, int polyqty, double h, double *val);

  extern void neut_polys_area (struct POLY *Poly, int *polys,
		               int polyqty, double *pval);
  extern int neut_polys_sphericity (struct POLY *Poly, int *polys,
				    int polyqty, double *pval);
  extern int neut_polys_sphericity_2d (struct POLY *Poly, int *polys,
				       int polyqty, double Size, double *pval);
  extern int neut_polys_centroid (struct POLY *Poly, int *polys, int
				  polyqty, double *coo);

  extern int neut_polys_point_dist (struct POLY* Poly, int *polys, int polyqty, double *, double *);
  extern int neut_polys_point_dist_legacy (struct POLY* Poly, int *polys, int polyqty, double *, double *);

  extern int neut_poly_point_dist (struct POLY Poly, double *ptcoo,
                                   double *pdist);
  extern int neut_polys_convexity (struct POLY *Poly, int *polys, int
      polyqty, double *pval);
  extern int neut_polys_convexity_2d (struct POLY *Poly, int *polys, int
      polyqty, double *pval);
  extern void neut_poly_convexhull (struct POLY Poly, struct NODES *pN,
      struct MESH *pM);
  extern void neut_polys_convexhull (struct POLY *Poly, int *polys, int
      polyqty, struct NODES *pN, struct MESH *pM);

#endif				/* NEUT_POLY_GEOM_H */

#ifdef __cplusplus
}
#endif
