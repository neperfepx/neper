/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_SPACE_H
#define UT_SPACE_H

#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#endif

/// \file ut_space.h
/// \brief Space-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Compute the distance between 2 points
/// \param pt1: point 1
/// \param pt2: point 2
/// \return Euclidean distance
  extern double ut_space_dist (double *pt1, double *pt2);

/// \brief Compute the squared distance between 2 points
/// \param pt1: point 1
/// \param pt2: point 2
/// \return squared Euclidean distance
  extern double ut_space_dist2 (double *pt1, double *pt2);

/// \brief Compute the distance between 2 points, integer coordinates
/// \param pt1: point 1
/// \param pt2: point 2
/// \return Euclidean distance
  extern double ut_space_dist_int (int *pt1, int *pt2);

/// \brief Compute the distance between 2 points in 2D
/// \param pt1: point 1
/// \param pt2: point 2
/// \return Euclidean distance
  extern double ut_space_dist_2d (double *pt1, double *pt2);

/// \brief Compute the barycentre of points
/// \param pts: points
/// \param ptqty: number of points
/// \param bary: barycentre
  extern void ut_space_bary (double **pts, int ptqty, double *bary);

/// \brief Compute the barycentre of points in 2D
/// \param pts: points
/// \param ptqty: number of points
/// \param bary: barycentre
  extern void ut_space_bary_2d (double **pts, int ptqty, double *bary);

/// \brief Compute the barycentre of points in 2D, integer coordinates
/// \param pts: points
/// \param ptqty: number of points
/// \param bary: barycentre
  extern void ut_space_bary_2d_int (int **pts, int ptqty, int *bary);

/// \brief Compute the surface area of a triangle
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \return surface area
  extern double ut_space_triangle_area (double *pt1, double *pt2,
                                        double *pt3);

/// \brief Compute the perimeter of a triangle
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \return perimeter
  extern double ut_space_triangle_perimeter (double *pt1, double *pt2,
                                             double *pt3);

/// \brief Compute the normal vector of a triangle
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \param normal: normal vector
  extern void ut_space_triangle_normal (double *pt1, double *pt2, double *pt3,
                                        double *normal);

/// \brief Compute the (bary)centre of a triangle
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \param centre: centre
/// \note The centre is computed as the average of the 3 vertices, which is also the barycentre
  extern void ut_space_triangle_centre (double *pt1, double *pt2, double *pt3,
                                        double *centre);

/// \brief Compute the (bary)centre of a tetrahedron
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \param pt4: point 4
/// \param centre: centre
/// \note The centre is computed as the average of the 3 vertices, which is also the barycentre
  extern void ut_space_tet_centre (double *pt1, double *pt2, double *pt3,
                                   double *pt4, double *centre);

/// \brief Compute the volume of a tetrahedron
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \param pt4: point 4
/// \return volume
/// \note The centre is computed as the average of the 3 vertices, which is also the barycentre
/// \note volume = norm ( (pt1 - pt4) . [(pt2 - pt4) x (pt3 - pt4)] )/ 6
  extern double ut_space_tet_volume (double *pt1, double *pt2, double *pt3,
                                     double *pt4);

/// \brief Compute the centre of a hexahedron
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \param pt4: point 4
/// \param pt5: point 5
/// \param pt6: point 6
/// \param pt7: point 7
/// \param pt8: point 8
/// \param centre: centre
  extern void ut_space_hexa_centre (double *pt1, double *pt2, double *pt3,
                                    double *pt4, double *pt5, double *pt6,
                                    double *pt7, double *pt8, double *centre);

/// \brief Compute the volume of a hexahedron
/// \param pt1: point 1
/// \param pt2: point 2
/// \param pt3: point 3
/// \param pt4: point 4
/// \param pt5: point 5
/// \param pt6: point 6
/// \param pt7: point 7
/// \param pt8: point 8
/// \return volume
/// \note Computation as the sum of volumes of tetrahedra
  extern double ut_space_hexa_volume (double *pt1, double *pt2, double *pt3,
                                      double *pt4, double *pt5, double *pt6,
                                      double *pt7, double *pt8);

/// \brief Compute the bisector plane between two points
/// \param pt1: point 1
/// \param pt2: point 2
/// \param plane: bisector plane
  extern void ut_space_points_bisplane (double *pt1, double *pt2,
                                        double *plane);

/// \brief Compute the bisector plane between two weighted points (à la Laguerre)
/// \param pt1: point 1
/// \param pt2: point 2
/// \param wt1: weight of point 1
/// \param wt2: weight of point 2
/// \param plane: bisector plane
  extern void ut_space_points_bisplane_power (double *pt1, double *pt2,
                                              double wA, double wB,
                                              double *plane);

/// \brief Convert a volume into an equivalent diameter
/// \param vol: volume
/// \param *pdiameq: equivalent diameter
/// \note "Equivalent diameter" if the diameter of the sphere of equivalent volume
  extern void ut_space_volume_diameq (double vol, double *pdiameq);

/// \brief Convert a volume into an equivalent radius
/// \param vol: volume
/// \param *pradeq: equivalent radius
/// \note "Equivalent radius" if the radius of the sphere of equivalent volume
  extern void ut_space_volume_radeq (double vol, double *pradeq);

/// \brief Convert a surface area into an equivalent diameter
/// \param area: surface area
/// \param *pdiameq: equivalent diameter
/// \note "Equivalent diameter" if the diameter of the disk of equivalent surface area
  extern void ut_space_area_diameq (double area, double *pdiameq);

/// \brief Convert a surface area into an equivalent radius
/// \param area: surface area
/// \param *pradeq: equivalent radius
/// \note "Equivalent radius" if the radius of the disk of equivalent surface area
  extern void ut_space_area_radeq (double area, double *pradeq);

/// \brief Convert an equivalent diameter into a surface area
/// \param diameq: equivalent diameter
/// \param *parea: surface area
/// \note "Equivalent diameter" if the diameter of the disk of equivalent surface area
  extern void ut_space_diameq_area (double diameq, double *parea);

/// \brief Convert an equivalent diameter into a volume
/// \param diameq: equivalent diameter
/// \param *pvol: volume
/// \note "Equivalent diameter" if the diameter of the disk of equivalent volume
  extern void ut_space_diameq_volume (double diameq, double *pvol);

/// \brief Convert an equivalent diameter into a size (2D or 3D)
/// \param dim: dimension
/// \param diameq: equivalent diameter
/// \param *psize: size, i.e. area in 2D or volume in 3D
  extern void ut_space_diameq_size (int dim, double diameq, double *psize);

/// \brief Convert a size into an equivalent diameter (2D or 3D)
/// \param dim: dimension
/// \param size: size, i.e. area in 2D or volume in 3D
/// \param *pdiameq: equivalent diameter
  extern void ut_space_size_diameq (int dim, double size, double *pdiameq);

/// \brief Convert a size into an equivalent radius (2D or 3D)
/// \param dim: dimension
/// \param size: size, i.e. area in 2D or volume in 3D
/// \param *pradeq: equivalent radius
  extern void ut_space_size_radeq (int dim, double size, double *pradeq);

/// \brief Get the bounding box of points
/// \param pts: points
/// \param ptqty: number of points
/// \param bbox: bounding box, [0:2][0:1]
  extern void ut_space_points_bbox (double **pts, int ptqty, double **bbox);

/// \brief Get the equation of a plane from a point and a normal vector
/// \param pt: point
/// \param normal: normal vector
/// \param plane: plane
/// \note Plane of equation \c plane[1] \c x + c plane[2] \c y + \c plane[3] \c z = plane[0]
  extern void ut_space_point_normal_plane (double *pt, double *normal,
                                           double *plane);

/// \brief Get the unit vector between 2 points
/// \param pt1: point 1
/// \param pt2: point 2
/// \param uvect: unit vector
/// \note \c uvect goes from \c pt1 to \c pt2 and has unit length
  extern void ut_space_points_uvect (double *pt1, double *pt2, double *uvect);

/// \brief Get a distribution of points on the sphere
/// \param rad: radius
/// \param ptqty: number of points
/// \param pts: points
  extern void ut_space_sphere_points (double rad, int ptqty, double **pts);

/// \brief Get the distance between a point and a plane
/// \param pt: point
/// \param plane: plane
/// \return Euclidean distance
  extern double ut_space_point_plane_dist (double *pt, double *plane);

/// \brief Get the signed distance between a point and a plane
/// \param pt: point
/// \param plane: plane
/// \return Signed Euclidean distance
/// \note The distance is negative if the plane normal point away from \c pt and positive otherwise
/// \note http://mathworld.wolfram.com/Point-PlaneDistance.html
  extern double ut_space_point_plane_dist_signed (double *pt, double *plane);

/// \brief Get the distance between a point and a line in 3D
/// \param pt: point
/// \param basept: point belonging to the line
/// \param vect: line vector
/// \param *pdist: distance
  extern void ut_space_point_line_dist (double *pt, double *basept,
                                        double *vect, double *pdist);

/// \brief Get the distance between a point and a line in 2D
/// \param pt: point
/// \param line: line parameters: line[0] = line[1] x + line[2] y
/// \param *pdist: distance
  extern void ut_space_point_line_dist_2d (double *pt, double *line,
                                           double *pdist);

/// \brief Get the distance between a point and a circle in 3D, considered in the circle plane
/// \param pt: point
/// \param centre: circle centre
/// \param normal: circle normal
/// \param rad: circle radius
/// \param *pdist: distance
/// \param ptproj: nearest point on the circle
  extern void ut_space_point_circle_dist (double *pt, double *centre,
                                          double *normal, double rad,
                                          double *pdist, double *ptproj);

/// \brief Get the distance between a point and an ellipse in 3D, considered in the ellipse plane
/// \param pt: point
/// \param centre: ellipse centre
/// \param normal: ellipse normal
/// \param dir1: ellipse base direction 1
/// \param dir2: ellipse base direction 2
/// \param rad1: ellipse radius 1
/// \param rad2: ellipse radius 2
/// \param *pdist: distance
/// \param ptproj: nearest point on the ellipse
  extern void ut_space_point_ellipse_dist (double *pt, double *centre,
                                           double *normal, double *dir1,
                                           double *dir2, double rad1,
                                           double rad2, double *pdist,
                                           double *ptproj);

/// \brief Get the side of a plane a point belongs to
/// \param plane: plane
/// \param pt: point
/// \return -1 if on negative side, 1 if on positive side
  extern int ut_space_point_plane_side (double *pt, double *plane);

/// \brief Get the side of a plane a point belongs to, within a tolerance
/// \param plane: plane
/// \param pt: point
/// \param tol: absolute tolerance
/// \return -1 if on negative side, 1 if on positive side
/// \note the tolerance is such that the negative side is favoured
  extern int ut_space_point_plane_side_tol (double *pt, double *plane,
                                            double tol);

/// \brief Get the side of a sphere a point belongs to
/// \param centre: sphere centre
/// \param rad: sphere radius
/// \param pt: point
/// \return -1 if on negative side (inside), 1 if on positive side (outside)
  extern int ut_space_point_sphere_side (double *pt, double *centre,
                                         double rad);

/// \brief Get the side of a cylinder a point belongs to
/// \param pt: point
/// \param basept: cylinder base point
/// \param normal: cylinder vector
/// \param rad: cylinder radius
/// \note the cylinder is considered infinite
/// \return -1 if on negative side (inside), 1 if on positive side (outside)
  extern int ut_space_point_cyl_side (double *pt, double *basept,
                                      double *normal, double rad);

/// \brief Get the side of an elliptic cylinder a point belongs to
/// \param pt: point
/// \param basept: cylinder ellipse basept
/// \param normal: cylinder ellipse normal
/// \param dir1: cylinder ellipse direction 1
/// \param dir2: cylinder ellipse direction 2
/// \param rad1: cylinder ellipse radius 1
/// \param rad2: cylinder ellipse radius 2
/// \note the cylinder is considered infinite
/// \return -1 if on negative side (inside), 1 if on positive side (outside)
  extern int ut_space_point_ecyl_side (double *pt, double *basept,
                                       double *normal, double *dir1,
                                       double *dir2, double rad1,
                                       double rad2);

/// \brief Get the tangent plane of a sphere at a given point
/// \param point: point
/// \param centre: sphere centre
/// \param rad: sphere radius
/// \param plane: plane
/// \note if the point does not belong to the sphere, the nearest point is considered
  extern void ut_space_point_sphere_tangentplane (double *pt, double *centre,
                                                  double rad, double *plane);

/// \brief Get the tangent plane of a cylinder at a given point
/// \param pt: point
/// \param basept: cylinder base point
/// \param axis: cylinder axis
/// \param rad: cylinder radius
/// \param plane: plane
/// \note the cylinder is considered infinite
/// \note if the point does not belong to the cylinder, the nearest point is considered
  extern void ut_space_point_cyl_tangentplane (double *pt, double *basept,
                                               double *axis, double rad,
                                               double *plane);

/// \brief Get the tangent plane of an elliptic cylinder at a given point
/// \param pt: point
/// \param basept: cylinder base point
/// \param axis: cylinder axis
/// \param dir1: ellipse base direction 1
/// \param dir2: ellipse base direction 2
/// \param rad1: ellipse radius 1
/// \param rad2: ellipse radius 2
/// \param plane: plane
/// \note the elliptic cylinder is considered infinite
/// \note if the point does not belong to the elliptic cylinder, the nearest point is considered
  extern void ut_space_point_ecyl_tangentplane (double *pt, double *basept,
                                                double *axis, double *dir1,
                                                double *dir2, double rad1,
                                                double rad2, double *plane);

/// \brief Get the tangent plane of a torus at a given point
/// \param pt: point
/// \param basept: cylinder base point
/// \param axis: torus axis
/// \param rad: big radius
/// \param rad2: small radius
/// \param plane: plane
/// \note if the point does not belong to the torus, the nearest point is considered
  extern void ut_space_point_torus_tangentplane (double *pt, double *basept,
                                                 double *axis, double rad,
                                                 double rad2, double *plane);

#ifdef HAVE_GSL

/// \brief Get the projection of a point along a direction onto a plane
/// \param pt: point
/// \param dir: direction
/// \param plane: plane
/// \param projpt: projected point
  extern void ut_space_point_dir_plane_proj (double *pt, double *dir,
                                             double *plane, double *projpt);

/// \brief Get the projection of a point onto a plane along its normal
/// \param pt: point
/// \param plane: plane
/// \param projpt: projected point
  extern void ut_space_point_plane_proj (double *pt, double *plane,
                                         double *projpt);

#endif

/// \brief Get the projection of a point onto a cylinder
/// \param pt: point
/// \param basept: cylinder base point
/// \param axis: cylinder axis
/// \param rad: cylinder radius
/// \param projpt: projection
/// \note the cylinder is considered infinite
  extern void ut_space_point_cyl_proj (double *pt, double *basept,
                                       double *axis, double rad,
                                       double *projpt);

/// \brief Get the projection of a point onto a sphere
/// \param pt: point
/// \param centre: sphere centre
/// \param rad: sphere radius
/// \param projpt: projection
  extern void ut_space_point_sphere_proj (double *point, double *centre,
                                          double rad, double *projpt);

/// \brief Get the projection of a point onto a torus
/// \param pt: point
/// \param centre: torus centre
/// \param axis: torus axis
/// \param rad: big radius
/// \param rad2: small radius
/// \param projpt: projected point
  extern void ut_space_point_torus_proj (double *pt, double *centre,
                                         double *axis, double rad,
                                         double rad2, double *projpt);

/// \brief Get the projection of a point onto an elliptic cylinder
/// \param pt: point
/// \param basept: elliptic cylinder base point
/// \param axis: elliptic cylinder axis
/// \param dir1: ellipse base direction 1
/// \param dir2: ellipse base direction 2
/// \param rad1: ellipse radius 1
/// \param rad2: ellipse radius 2
/// \param projpt: projected point
/// \note the elliptic cylinder is considered infinite
  extern void ut_space_point_ecyl_proj (double *pt, double *basept,
                                        double *axis, double *dir1,
                                        double *dir2, double rad1,
                                        double rad2, double *projpt);

/// \brief Get the mirror of a point wrt a plane
/// \param pt: point
/// \param plane: plane
/// \param mirpt: mirror point
  extern void ut_space_point_plane_mirror (double *pt, double *plane,
                                           double *mirpt);

/// \brief Get the mirror of a point wrt a sphere
/// \param pt: point
/// \param centre: sphere centre
/// \param rad: sphere radius
/// \param mirpt: mirror point
  extern void ut_space_point_sphere_mirror (double *pt, double *centre,
                                            double rad, double *mirpt);

/// \brief Get the mirror of a point wrt a cylinder
/// \param pt: point
/// \param basept: cylinder base point
/// \param axis: cylinder axis
/// \param rad: cylinder radius
/// \param mirpt: mirror point
/// \note the cylinder is considered infinite
  extern void ut_space_point_cyl_mirror (double *pt, double *basept,
                                         double *axis, double rad,
                                         double *mirpt);

/// \brief Get the mirror of a point wrt an elliptic cylinder
/// \param pt: point
/// \param basept: elliptic cylinder base point
/// \param axis: elliptic cylinder axis
/// \param dir1: ellipse base direction 1
/// \param dir2: ellipse base direction 2
/// \param rad1: ellipse radius 1
/// \param rad2: ellipse radius 2
/// \param mirpt: mirror point
/// \note the elliptic cylinder is considered infinite
/// \note if the point does not belong to the elliptic cylinder, the nearest point is considered
  extern void ut_space_point_ecyl_mirror (double *pt, double *basept,
                                          double *axis, double *dir1,
                                          double *dir2, double rad1,
                                          double rad2, double *mirpt);

/// \brief Get the mirror of a point wrt a torus
/// \param pt: point
/// \param centre: torus centre
/// \param axis: torus axis
/// \param rad: big radius
/// \param rad2: small radius
/// \param mirpt: mirror point
  extern void ut_space_point_torus_mirror (double *pt, double *centre,
                                           double *axis, double rad,
                                           double rad2, double *mirpt);

/// \brief Determine whether a quadrangle is convex
/// \param pt1: 1st point
/// \param pt2: 2nd point
/// \param pt3: 3rd point
/// \param pt4: 4th point
/// \return 1 if convex and 0 otherwise
  extern int ut_space_quad_convex (double *pt1, double *pt2, double *pt3,
                                   double *pt4);

/// \brief Determine whether a quadrangle is convex, integer coordinates
/// \param pt1: 1st point
/// \param pt2: 2nd point
/// \param pt3: 3rd point
/// \param pt4: 4th point
/// \return 1 if convex and 0 otherwise
  extern int ut_space_quad_convex_int (int *pt1, int *pt2, int *pt3,
                                       int *pt4);

/// \brief Determine whether a point is inside a 2D domain
/// \param pt: point
/// \param pts: domain points
/// \param ptqty: number of domain points
/// \return 1 if the point is inside, 0 otherwise
  extern int ut_space_point_indomain_2d (double *pt, double **pts, int ptqty);

/// \brief Determine whether a point is inside a 2D domain, integer coordinates
/// \param pt: point
/// \param pts: domain points
/// \param ptqty: number of domain points
/// \return 1 if the point is inside, 0 otherwise
  extern int ut_space_point_indomain_2d_int (int *pt, int **pts, int ptqty);

/// \brief Determine whether 3 points are aligned
/// \param pt1: 1st point
/// \param pt2: 2nd point
/// \param pt3: 3rd point
/// \param eps: angular tolerancy (in degrees)
/// \return 1 if aligned and 0 otherwise
  extern int ut_space_points_aligned (double *pt1, double *pt2, double *pt3,
                                      double eps);

/// \brief Get a line from 2 points
/// \param pt1: 1st point
/// \param pt2: 2nd point
/// \param line: line
  extern void ut_space_points_line (double *pt1, double *pt2, double *line);

/// \brief Get a plane from 3 points
/// \param pt1: 1st point
/// \param pt2: 2nd point
/// \param pt3: 3rd point
/// \param plane: plane
  extern void ut_space_points_plane (double *pt1, double *pt2, double *pt3,
                                     double *plane);

/// \brief Get the best fitting plane from a set of points
/// \param pts: points
/// \param ptqty: number of points
/// \param plane: plane
  extern void ut_space_pointset_plane (double **pts, int ptqty,
                                       double *plane);

/// \brief Get a plane from 2 points and a normal vector
/// \param pt1: 1st point
/// \param pt2: 2nd point
/// \param normal: normal vector
/// \param plane: plane
  extern void ut_space_points_invect_plane (double *pt1, double *pt2,
                                            double *normal, double *plane);

/// \brief Determine whether 2 planes are coincident
/// \param plane1: 1st plane
/// \param plane2: 2nd plane
/// \return 1 if planes are coincident and 0 otherwise
  extern int ut_space_planes_equal (double *plane1, double *plane2);

/// \brief Determine whether 2 lines are coincident
/// \param line1: 1st line
/// \param line2: 2nd line
  extern int ut_space_lines_equal (double *line1, double *line2);

/// \brief Determine whether a point belongs to a segment
/// \param segpt1: 1st segment point
/// \param segpt2: 2nd segment point
/// \param pt: point
/// \return 1 of point is in and 0 otherwise
  extern int ut_space_segment_point_in (double *segpt1, double *segpt2,
                                        double *pt);

/// \brief Determine whether a point belongs to a triangle
/// \param tript1: 1st triangle point
/// \param tript2: 2nd triangle point
/// \param tript3: 3rd triangle point
/// \param pt: point
/// \param eps1: relative tolerancy on the position wrt the two edges based on tript1
/// \param eps2: relative tolerancy on the position wrt the two edges based on tript2
/// \return 1 of point is in and 0 otherwise
/// \note Barycentric approach from http://www.blackpawn.com/texts/pointinpoly/
  extern int ut_space_triangle_point_in (double *tript1, double *tript2,
                                         double *tript3, double *pt,
                                         double eps1, double eps2, double *ptol);

/// \brief Determine whether a point belongs to a tetrahedron
/// \param tetpt1: 1st tet point
/// \param tetpt2: 2nd tet point
/// \param tetpt3: 3rd tet point
/// \param tetpt4: 4th tet point
/// \param pt: point
/// \return 1 of point is in and 0 otherwise
  extern int ut_space_tet_point_in (double *tetpt1, double *tetpt2,
                                    double *tetpt3, double *tetpt4,
                                    double *pt);

/// \brief Convert a string into a plane
/// \param string: character string
/// \param plane: plane
  extern void ut_space_string_plane (char *string, double *plane);

/// \brief Get the projection of a point onto a segment
/// \param segpt1: 1st segment point
/// \param segpt2: 2nd segment point
/// \param pt: point
/// \param projpt: projected point
/// \param *ppos: relative position from segpt1 (0 at segpt1, 1 at segpt2)
/// \param *pdist: distance from segpt1
/// \note \c projpt, \c ppos and \c pdist can be \c NULL
/// \note http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
  extern void ut_space_segment_point_proj (double *segpt1, double *segpt2,
                                           double *pt, double *projpt,
                                           double *ppos, double *pdist);

/// \brief Get the barycentre of points subjected to constraints, in 2D
/// \param pts: points
/// \param weights: point weights
/// \param ptqty: number of points
/// \param planes: constraining planes
/// \param planeqty: number of constraining planes
/// \param barypt: barycentre
  extern int ut_space_points_bary_constrained (double **pts, double *weights,
                                               int ptqty, double **planes,
                                               int planeqty, double *barypt);

/// \brief Get the barycentre of points subjected to constraints, in 2D
/// \param pts: points
/// \param weights: point weights
/// \param ptqty: number of points
/// \param planes: constraining planes
/// \param planeqty: number of constraining planes
/// \param barypt: barycentre
  extern int ut_space_points_bary_constrained_2d (double **pts,
                                                  double *weights, int ptqty,
                                                  double **planes,
                                                  int planeqty,
                                                  double *barypt);

#ifdef HAVE_GSL

/// \brief Get the intersection point between a segment and a point
/// \param segpt1: segment 1st point
/// \param segpt2: segment 2nd point
/// \param plane: plane
/// \param interpt: intersection point
/// \note the segment is considered infinite
  extern void ut_space_segment_plane_intersect (double *segpt1,
                                                double *segpt2, double *plane,
                                                double *interpt);

#endif

/// \brief Determine the intersection between 2 segments
/// \param seg1pt1: 1st segment 1st point
/// \param seg1pt2: 1st segment 2nd point
/// \param seg2pt1: 2nd segment 1st point
/// \param seg2pt2: 2nd segment 2nd point
/// \param interpt: intersection point
/// \return 1 if the segments intersect and 0 otherwise
/// \note the 2 segments must be in the same plane
/// \note http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
  extern int ut_space_segments_inter (double *seg1pt1, double *seg1pt2,
                                      double *seg2pt1, double *seg2pt2,
                                      double *interpt);

/// \brief Determine whether a contour selfintersects
/// \param pts: contour points
/// \param ptqty: number of contour points
/// \return 1 if selfintersects and 0 otherwise
  extern int ut_space_contour_selfintersect (double **pts, int ptqty);

/// \brief Determine the convex hull of points, in 2D
/// \param pts: points
/// \param ptqty: number of points
/// \param *phullptids: identifiers of the points of the convex hull
/// \param *phullptids: number of points of the convex hull
/// \note http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
  extern void ut_space_points_convexhull_2d (double **pts, int ptqty,
                                             int **phullptids,
                                             int *phullptqty);

/// \brief Divide a 3D polygon into triangles
/// \param plane: polygon plane equation
/// \param pts: polygon points
/// \param ptqty: number of polygon points
/// \param *ptriptids: identifiers of the triangle points
/// \param *ptriptqty: number of triangle points
  extern void ut_space_polygon_triangles (double *plane, double **pts,
                                          int ptqty, int ***ptriptids,
                                          int *ptriptqty);

#ifdef HAVE_GSL

/// \brief Get a random point inside a(n hyper)sphere
/// \param r: GSL random
/// \param dims: dimensions to work with
/// \param dimqty: number of dimensions to work with
/// \param radmin: (hyper)sphere minimum radius
/// \param radmax: (hyper)sphere maximum radius
/// \param pt: point
/// \note uniform distribution
  extern void ut_space_random (gsl_rng * r, int *dims, int dimqty,
                               double radmin, double radmax, double *pt);

#endif

/// @cond
  extern double ut_space_points_ccw (double *pt1, double *pt2, double *p3);

  extern int ut_space_vectpair_samedir (double *, double *);

  extern int ut_space_vectpair_samedir_tol (double *, double *, double);

  extern void ut_space_points_dir_furthestpt (double **, int, double *,
                                              int *);

  extern void ut_space_points_dir_furthestpts (double **, int, double *,
                                               double eps, int **, int *);

  extern int ut_space_polypts_point_dist_dosimplex (double **vercoo,
                                                    int *list, int *pqty,
                                                    double *dir, double eps,
                                                    int verbosity, FILE * fp);

  extern int ut_space_polypts_point_dist (double **pts, int ptqty,
                                          double *pt, double *pdist);
  extern int ut_space_polypts_point_dist_verbosity (double **pts, int
                                                    ptqty, double *pt,
                                                    double *pdist,
                                                    int verbosity);

  extern void ut_space_tet_randompt (double *v0, double *v1, double *v2, double *v3, gsl_rng *r, double *pt);

/// @endcond

#endif                          /* UT_SPACE_H */

#ifdef __cplusplus
}
#endif
