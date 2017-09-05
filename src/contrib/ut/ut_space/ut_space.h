/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_SPACE_H
#define UT_SPACE_H

#include "ut_config.h"
#include<gsl/gsl_rng.h>

extern double ut_space_dist_squared (double *, double *);
extern double ut_space_dist (double *, double *);
extern double ut_space_dist_int (int *, int *);
extern double ut_space_dist2d (double *, double *);
extern int ut_space_quad_convex (double *, double *, double *, double *);
extern void ut_space_bary2d (double **, int, double *);
extern void ut_space_bary3d (double **, int, double *);
extern int ut_space_quad_convex_int (int *, int *, int *, int *);
extern void ut_space_bary2d_int (int **, int, int *);
extern int ut_space_indomain2d (double **, int, double, double);
extern int ut_space_indomain2d_int (int **, int, int *);

extern int ut_space_planeside (double *, double *);
extern int ut_space_planeside_tol (double *, double *, double);
extern int ut_space_planeside_2d (double *, double *);

extern double ut_space_trianglearea (double *, double *, double *);
extern void ut_space_trianglenormal (double *, double *, double *, double *);
extern void ut_space_polygon_clean (double **, int, double ***, int *, double);
extern int ut_space_pointaligned (double *, double *, double *, double);

extern void ut_space_arrangepoints (double **, int, double **, int, int *);

extern double ut_space_triangle_area (double *, double *, double *);
extern double ut_space_triangle_perimeter (double *, double *, double *);
extern int ut_space_tet_centre (double *, double *, double *, double *, double*);
extern int ut_space_triangle_centre (double *, double *, double *, double*);
extern double ut_space_tet_volume (double *, double *, double *, double *);
extern int ut_space_hexa_center (double*, double*, double*, double*,
				 double*, double*, double*, double*,
				 double*);
extern double ut_space_hexa_volume (double*, double*, double*, double*,
                      double*, double*, double*, double*);
#ifdef HAVE_GSL
extern void ut_space_projpoint_alongonto (double *, double *, double *);
extern int ut_space_point_plane_dist (double*, double*, double*);
extern int ut_space_point_plane_dist_signed (double*, double*, double*);
extern int ut_space_segment_plane_intersect (double*, double*, double*, double*);
#endif

extern int ut_space_point_line_dist (double*, double*, double*);

extern void ut_space_points_invect_plane (double *, double *, double *,
					  double *);
extern void ut_space_points_plane (double *, double *, double *, double *);
extern void ut_space_points_line (double *, double *, double *);

extern int ut_space_triangle_point_in (double *, double *, double *,
				       double *, double, double);
extern int ut_space_tet_point_in (double *, double *, double *, double *,
				  double *);

extern int ut_space_segments_intersect (double*, double*, double*, double*);

extern void ut_space_points_covarmatrix (double**, double*, int, int, double**, double*);

extern int ut_space_points_aligned (double*, double*, double*);
extern int ut_space_points_aligned2 (double*, double*, double*);
extern int ut_space_planes_equal (double*, double*);
extern int ut_space_lines_equal (double*, double*);

extern double ut_space_segment_point_pos (double*, double*, double*);
extern int ut_space_segment_point_in (double*, double*, double*);

extern int ut_space_contour_intersect (double**, int);

extern int ut_space_points_uvect (double*, double*, double*);

extern void ut_space_string_vect (char*, double*);
extern void ut_space_string_plane (char*, double*);

extern int
ut_space_segments_intersectpt (double* P, double* B, double* Q, double* D,
    double* I);

extern int ut_space_bary2d_constrained (double** ptcoo, double* ptw, int
    ptqty, double** eqs, int eqqty, double* barycoo);
extern int ut_space_bary3d_constrained (double** ptcoo, double* ptw, int
    ptqty, double** eqs, int eqqty, double* barycoo);

extern int ut_space_pts_convexhull_2d (double **coo, int qty,
					  int **phids, int *phqty);

extern double ut_space_pts_ccw (double* p1, double* p2, double* p3);

extern void ut_space_pts_bbox (double** coos, int qty, double **bbox);

extern void ut_space_segment_point_proj (double* x1, double* x2,
					 double* x0, double *pt,
					 double *pdist, double *coo);

extern void ut_space_points_bisplane (double* A, double* B, double*
    plane);

extern void
ut_space_points_bisplane_power (double* A, double* B, double wA,
                               double wB, double* plane);

extern void ut_space_pts_dir_furthestpt (double **, int, double*, int*);
extern void ut_space_pts_dir_furthestpts (double **, int, double*,
					  double eps, int**, int*);

extern int ut_space_vectpair_samedir (double*, double*);
extern int ut_space_vectpair_samedir_tol (double*, double*, double);

extern int ut_space_polypts_point_dist (double** pts, int ptqty,
                                        double* pt, double *pdist);
extern int ut_space_polypts_point_dist_verbosity (double** pts, int
    ptqty, double* pt, double *pdist, int verbosity);
extern int ut_space_polypts_point_dist_dosimplex (double **vercoo,
                                int *list, int *pqty, double *dir,
				double eps, int verbosity, FILE *fp);

extern void ut_space_sphere_points (int n, double r, double **pts);

extern void ut_space_volume_diameq (double vol, double *pdiameq);
extern void ut_space_area_diameq (double area, double *pdiameq);
extern void ut_space_volume_radeq (double vol, double *pradeq);
extern void ut_space_diameq_area (double diameq, double *parea);
extern void ut_space_diameq_volume (double diameq, double *pvol);
extern void ut_space_area_radeq (double area, double *pradeq);
extern void ut_space_diameq_size (int dim, double diameq, double *psize);
extern void ut_space_size_diameq (int dim, double size, double *pdiameq);

extern void ut_space_random (gsl_rng *r, int *dims, int dimqty, double mindist,
			     double maxdist, double *v);

#endif /* UT_SPACE_H */

#ifdef __cplusplus
}
#endif
